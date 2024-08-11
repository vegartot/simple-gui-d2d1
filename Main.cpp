
#pragma warning(push, 3)
// System header file specific warnings
#pragma warning(disable: 4668)      // Not defined preprocessor macros          : winioctl.h
#pragma warning(disable: 4365)      // Conversion from long to unsigned int     : xmemory atomic
#pragma warning(disable: 4820)      // Added padding to data structures         : d2d1.h, d2d1_1.h d3d10.h d3d10_1.h d3d10sdklayers.h d3d10effect.h d3d10shader.h dxgi.h dxgi1_2.h dxgi1_3.h dxgi1_6.h
#pragma warning(disable: 5039)      // Potential undefined behaviour            : winbase.h
#pragma warning(disable: 5246)      // Wrap initialization in braces            : d2d1_1helper.h
#pragma warning(disable: 4264)      // No override available for virtual        : dwrite_3.h
#pragma warning(disable: 4263)      // Function doesn't override                : dwrite_3.h

#include <Windows.h>
#include <windowsx.h>

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <dxgi1_6.h>
#include <dwrite_3.h>

#include <iostream>
#include <wrl/client.h>

#pragma warning(pop)

#include "resource.h"

// Window style - should have minimize and close buttom with no resizing
#define WINDOW_STYLE WS_SYSMENU | WS_MINIMIZEBOX

// All hits to track within the window client area
typedef enum
{
    HIT_NONE,
    HIT_SQUARE_1,
    HIT_SQUARE_2,
    HIT_SQUARE_3,
    HIT_SQUARE_4,
    HIT_SQUARE_5,
    HIT_SQUARE_6,
    HIT_SQUARE_7,
    HIT_SQUARE_8,
    HIT_SQUARE_9,
    HIT_RESET,
    HIT_ERROR = 0xff
} GAME_SQUARES;

// Determine what if any game square was clicked on
// by the player.
GAME_SQUARES ValidateClick(D2D1_POINT_2F point)
{
    // Case: Click is outside the rectangle play area
    if ((0.f < point.x && point.x < 100.f) || (1000.f < point.x) || (0.f < point.y && point.y < 50.f) || (850.f < point.y))
    {
        if (1030.f <= point.x && point.x <= 1250.f && 150.f <= point.y && point.y <= 200.f) return HIT_RESET;
        return HIT_NONE;
    }

    // Handle every other case of the 9 squares
    // For first row:
    if (point.y < 50.f + 800.f / 3.f)
    {
        if (point.x < 100.f + 900.f / 3) return HIT_SQUARE_1;
        else if (point.x < 100.f + 2.f * 900.f / 3) return HIT_SQUARE_2;
        else return HIT_SQUARE_3;
    }

    // For second row:
    if (point.y < 50.f + 2.f * 800.f / 3.f)
    {
        if (point.x < 100.f + 900.f / 3) return HIT_SQUARE_4;
        else if (point.x < 100.f + 2.f * 900.f / 3) return HIT_SQUARE_5;
        else return HIT_SQUARE_6;
    }

    // For third row:
    if (point.y < 50.f + 800.f)
    {
        if (point.x < 100.f + 900.f / 3) return HIT_SQUARE_7;
        else if (point.x < 100.f + 2.f * 900.f / 3) return HIT_SQUARE_8;
        else return HIT_SQUARE_9;
    }

    // Should never arrive in this case, but compiler complains
    return HIT_ERROR;
}

// Given a game square, returns the screen cooridates of the center
// of that square.
D2D1_POINT_2F CenterOfSquare(GAME_SQUARES square)
{
    float x = 100.f + 900.f / 6.f + static_cast<float>(square % 3) * 900.f / 3.f;
    float y = 50.f + 800.f / 6.f + static_cast<float>(square / 3) * 800.f / 3.f;
    return D2D1::Point2F(x, y);
}

typedef enum
{
    BOARD_IS_VALID,
    BOARD_PLAYER_WON,
    BOARD_COMPUTER_WON,
    BOARD_DRAW

}VALIDATE_BOARD;

bool ValidateBoard(char board[9])
{
    for (int i = 0; i < 3; i++)
    {
        if (i >= 3) break;

        // Horizontal lines
        if (abs(board[i * 3] + board[i * 3 + 1] + board[i * 3 + 2]) == 3)
        {
            return false;
        }
        // Vertical lines
        if (abs(board[i] + board[i + 3] + board[i + 6]) == 3)
        {
            return false;
        }
    }
    if (abs(board[0] + board[4] + board[8]) == 3 || abs(board[2] + board[4] + board[6]) == 3) return false;
    return true;
}

bool PlayMove( char board[9])
{
    for (int i = 0; i < 9; i++)
    {

        if (board[i] == 0 && ValidateBoard(board))
        {
            board[i] = -1;
            return true;
        }
    }
    return false;
}

// Structure containing all renderer specific objects
typedef struct Renderer
{
    Microsoft::WRL::ComPtr<ID2D1Factory1> m_factory;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_wBrush;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_rBrush;

    Microsoft::WRL::ComPtr<IDWriteFactory1> m_writeFactory;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;

} Renderer;

// Wrapper for OutputDebugString to simply output
// a debug string with the result of GetLastError()
// to the windows debugger
void OutputLastError()
{
#ifdef UNICODE
    WCHAR buf[128]{};
    _snwprintf_s(buf, 128, L"Last error: %d\n", static_cast<int>(GetLastError()));
#else // ASCII
    CHAR buf[128]{};
    _snprintf_s(buf, 128, "Last error: %d\n", static_cast<int>(GetLastError()));
#endif // UNICODE  
    OutputDebugString(buf);
}

// Application window procedure function
LRESULT CALLBACK WindowProcedure(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Application entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int CmdShow)
{
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(CmdShow);

    HWND window{};
    RECT windowRect = { 0, 0, 1280, 900 };
    WNDCLASSEX windowClass{};
    Renderer renderer{};
    MSG  messageLoop{};

    // Adjust RECT to so that client delta is the old RECT
    UINT dpi = GetDpiForSystem();
    AdjustWindowRectExForDpi(&windowRect, WS_OVERLAPPEDWINDOW, NULL, WS_EX_LEFT, dpi);

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = sizeof(Renderer*);
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    windowClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
    windowClass.hbrBackground = NULL;
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = TEXT("main-window-class");
    windowClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClassEx(&windowClass);

    window = CreateWindowEx(
        WS_EX_LEFT,
        windowClass.lpszClassName,
        TEXT("Window"),
        WINDOW_STYLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        (LPVOID)&renderer
    );

    if (window == NULL)
    {
        DWORD error = GetLastError();

        if (error)
        {
            WCHAR buf[128];
            _snwprintf_s(buf, 128, TEXT("Failed to create window context!\nERROR CODE: %d"), static_cast<int>(error));

            MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);
            OutputLastError();
        }

        return EXIT_FAILURE;
    }


    SetWindowPos(
        window,
        NULL, NULL, NULL,
        static_cast<int>(static_cast<float>(windowRect.right - windowRect.left) * static_cast<float>(dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI)),
        static_cast<int>(static_cast<float>(windowRect.bottom - windowRect.top - GetSystemMetrics(SM_CYCAPTION)) * static_cast<float>(dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI)),
        SWP_NOMOVE | SWP_SHOWWINDOW);

   

    while (GetMessage(&messageLoop, window, 0, 0) > 0)
    {
        TranslateMessage(&messageLoop);
        DispatchMessage(&messageLoop);
    }

	return 0;
}

LRESULT CALLBACK WindowProcedure(_In_ HWND hWnd, _In_ UINT Message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    HRESULT hr{};
    static Renderer* renderer = nullptr;
    static D2D1_POINT_2F pos{};
    static char playGrid[9]{};


    // Returns 0 in most cases
    LRESULT result = 0;

    switch (Message)
    {
        case WM_NCCREATE:
        {
            // Window class structure
            auto wc = ((CREATESTRUCT*)lParam);

            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)wc->lpCreateParams);
            renderer = (Renderer*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), (D2D1_FACTORY_OPTIONS*)D2D1_DEBUG_LEVEL_NONE, &renderer->m_factory);
            if (hr != S_OK)
            {
                WCHAR buf[128]{};
                _snwprintf_s(buf, 128, TEXT("Failed to create D2D1Factory!\nConsider updating graphics drivers.\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);

                OutputDebugString(TEXT("Failed to create D2D1 factory interface.\n"));
                break;
            }

            D2D1_RENDER_TARGET_PROPERTIES renderTargetDesc{};
            renderTargetDesc.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
            renderTargetDesc.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
            renderTargetDesc.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
            renderTargetDesc.usage = D2D1_RENDER_TARGET_USAGE_NONE;
            renderTargetDesc.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

            UINT dpi = GetDpiForWindow(hWnd);

            switch (dpi)
            {
                case (DPI_AWARENESS_UNAWARE):
                {
                    renderTargetDesc.dpiX = USER_DEFAULT_SCREEN_DPI;
                    renderTargetDesc.dpiY = USER_DEFAULT_SCREEN_DPI;
                    break;
                }

                default:
                {
                    renderTargetDesc.dpiX = static_cast<float>(dpi);
                    renderTargetDesc.dpiY = static_cast<float>(dpi);
                }
            }


            D2D1_HWND_RENDER_TARGET_PROPERTIES handleDesc{};
            handleDesc.hwnd = hWnd;
            handleDesc.pixelSize.width = 1280;
            handleDesc.pixelSize.height = 900;
            handleDesc.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

            hr = renderer->m_factory->CreateHwndRenderTarget(&renderTargetDesc, &handleDesc, &renderer->m_renderTarget);


            if (hr != S_OK)
            {
                WCHAR buf[128];
                _snwprintf_s(buf, 128, TEXT("Failed to create D2D1HwndRenderTarget!\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);

                OutputDebugString(TEXT("Failed to create D2D1 HwndRenderTarget interface.\n"));
                break;
            }

            hr = renderer->m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &renderer->m_wBrush);
            if (hr != S_OK)
            {
                WCHAR buf[128];
                _snwprintf_s(buf, 128, TEXT("Failed to create solid color brush!\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);
                break;
            }

            hr = renderer->m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::OrangeRed), &renderer->m_rBrush);
            if (hr != S_OK)
            {
                WCHAR buf[128];
                _snwprintf_s(buf, 128, TEXT("Failed to create solid color brush!\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);
                break;
            }

            hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory7), &renderer->m_writeFactory);
            if (hr != S_OK)
            {
                WCHAR buf[128];
                _snwprintf_s(buf, 128, TEXT("Failed to create DWriterFactory!\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);
                break;
            }

            hr = renderer->m_writeFactory->CreateTextFormat(
                L"Arial", 
                nullptr, 
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                300.f,
                L"en-us",
                &renderer->m_textFormat);
            if (hr != S_OK)
            {
                WCHAR buf[128];
                _snwprintf_s(buf, 128, TEXT("Failed to create DWrite text format!\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);
                break;
            }

            hr = renderer->m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            if (hr != S_OK)
            {
                WCHAR buf[128];
                _snwprintf_s(buf, 128, TEXT("Failed to set text alignment!\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);
                break;
            }

            hr = renderer->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            if (hr != S_OK)
            {
                WCHAR buf[128];
                _snwprintf_s(buf, 128, TEXT("Failed to set paragraph alignment!\nERROR CODE: 0x%x"), static_cast<int>(hr));

                MessageBox(NULL, buf, TEXT("FATAL ERROR!"), MB_OK | MB_ICONERROR);
                break;
            }

            renderer->m_renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

            // WM_NCCREATE should return TRUE on success
            result = TRUE;
            break;
        }

        case WM_SIZE:
        {
            hr = renderer->m_renderTarget->Resize(D2D1::SizeU(static_cast<UINT32>(LOWORD(lParam)), static_cast<UINT32>(HIWORD(lParam))));
            if (FAILED(hr)) DestroyWindow(hWnd);
            break;
        }

        case WM_DPICHANGED:
        {
            // TODO: Adjust dpi and window size when dpi changes
            RECT* rc = (RECT*)lParam;
            UINT dpi = (HIWORD(wParam));
            SetWindowPos(
                hWnd,
                NULL,
                rc->left, 
                rc->top,
                static_cast<int>(static_cast<float>(rc->right - rc->left) * static_cast<float>(dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI)),
                static_cast<int>(static_cast<float>(rc->bottom - rc->top - GetSystemMetricsForDpi(SM_CYCAPTION, dpi)) * static_cast<float>(dpi) / static_cast<float>(USER_DEFAULT_SCREEN_DPI)),
                SWP_NOZORDER);

            //PostMessage(hWnd, WM_SIZE, NULL, MAKELPARAM(rc->right - rc->left, rc->bottom - rc->top));
            break;
        }

        case WM_CLOSE:
        {
            DestroyWindow(hWnd);
            OutputDebugString(TEXT("Destroying window...\n"));
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }

        case WM_PAINT:
        {

            renderer->m_renderTarget->BeginDraw();

            renderer->m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

            renderer->m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateGray));

            renderer->m_renderTarget->DrawRoundedRectangle(
                D2D1::RoundedRect(D2D1::RectF(100.f, 50.f, 1000.f, 850.f), 5.f, 5.f),
                renderer->m_wBrush.Get(),
                8.f
            );

            // Horizontal lines
            renderer->m_renderTarget->DrawLine(
                D2D1::Point2F(100.f, 50.f + 800.f / 3.f),
                D2D1::Point2F(1000.f, 50.f + 800.f / 3.f),
                renderer->m_wBrush.Get(),
                8.f
            );
            renderer->m_renderTarget->DrawLine(
                D2D1::Point2F(100.f, 50.f + 2.f * 800.f / 3.f),
                D2D1::Point2F(1000.f, 50.f + 2.f * 800.f / 3.f),
                renderer->m_wBrush.Get(),
                8.f
            );

            // Verticle lines
            renderer->m_renderTarget->DrawLine(
                D2D1::Point2F(100.f + 900.f / 3.f, 50.f),
                D2D1::Point2F(100.f + 900.f / 3.f, 850),
                renderer->m_wBrush.Get(),
                8.f
            );
            renderer->m_renderTarget->DrawLine(
                D2D1::Point2F(100.f + 2.f * 900.f / 3.f, 50.f),
                D2D1::Point2F(100.f + 2.f * 900.f / 3.f, 850),
                renderer->m_wBrush.Get(),
                8.f
            );

            // Reset button
            renderer->m_renderTarget->FillRoundedRectangle(
                D2D1::RoundedRect(D2D1::RectF(1030.f, 150, 1250.f, 200.f), 5, 5),
                renderer->m_wBrush.Get()
            );

            // Draw played moves in play grid
            for (int i = 0; i < 9; i++)
            {
                // Avoid compiler complaining about spectre
                if (i < 0 || i >= 9) break;

                // Draw ellipse or cross in square if it's been played
                D2D1_POINT_2F center = CenterOfSquare((GAME_SQUARES)i);
                if (!playGrid[i]) {}
                else if (playGrid[i] == 1)
                {
                    renderer->m_renderTarget->DrawTextW(L"X", 1, renderer->m_textFormat.Get(), D2D1::RectF(center.x, center.y, center.x, center.y), renderer->m_wBrush.Get());
                }
                else
                {
                    renderer->m_renderTarget->DrawTextW(L"O", 1, renderer->m_textFormat.Get(), D2D1::RectF(center.x, center.y, center.x, center.y), renderer->m_rBrush.Get());
                }
            }

            if (lParam)
            {

                pos.x = static_cast<float>(GET_X_LPARAM(lParam));
                pos.y = static_cast<float>(GET_Y_LPARAM(lParam));

                GAME_SQUARES square = ValidateClick(pos);

                if (square && square != HIT_RESET && square != HIT_ERROR)
                {
                    WCHAR buf[64]{};
                    OutputDebugString(buf);
                    _snwprintf_s(buf, 64, L"Hit Square: %d\n", square);
                    if (playGrid[square - 1] == 0 && ValidateBoard(playGrid))
                    {
                        playGrid[square - 1] = 1;
                        PlayMove(playGrid);
                    }
                    
                }
                else if (square == HIT_RESET)
                {
                    OutputDebugString(TEXT("Hit reset\n"));

                    // Reset board
                    for (int i = 0; i < 9; i++)
                    {
                        playGrid[i] = 0;
                    }
                }
            }


            hr = renderer->m_renderTarget->EndDraw();
            if (hr == D2DERR_RECREATE_TARGET)
            {
                DestroyWindow(hWnd);
            }

            break;

        }

        case WM_LBUTTONDOWN:
        {
            // Adjust hit coordinates for dpi scaled window
            UINT dpi = GetDpiForWindow(hWnd);

            float _x = static_cast<float>(GET_X_LPARAM(lParam)) * static_cast<float>(USER_DEFAULT_SCREEN_DPI) / static_cast<float>(dpi);
            float _y = static_cast<float>(GET_Y_LPARAM(lParam)) * static_cast<float>(USER_DEFAULT_SCREEN_DPI) / static_cast<float>(dpi);

            PostMessage(hWnd, WM_PAINT, NULL, MAKELPARAM(static_cast<int>(_x), static_cast<int>(_y)));
            break;
        }


        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                {
                    DestroyWindow(hWnd);
                    break;
                }
            }
            break;
        }

        default:
        {
            result = DefWindowProc(hWnd, Message, wParam, lParam);
        };
    }
    return result;
}
