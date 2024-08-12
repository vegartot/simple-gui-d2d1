
#include "main.h"

// Wrapper for OutputDebugString to simply output
// a debug string with the result of GetLastError()
// to the windows debugger
void OutputLastError()
{
#ifdef UNICODE
    WCHAR buf[128]{};
    _snwprintf_s(buf, 128, L"Last error: %d\n", static_cast<int>(GetLastError()));
#else // ASCII CHAR buf[128]{};
    _snprintf_s(buf, 128, "Last error: %d\n", static_cast<int>(GetLastError()));
#endif // UNICODE  
    OutputDebugString(buf);
}

// Application entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int CmdShow)
{
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(CmdShow);

    SetProcessDPIAware();

    HWND window{};
    RECT windowRect = { 0, 0, 1280, 900 };
    WNDCLASSEX windowClass{};
    Renderer renderer{};
    MSG  messageLoop{};

    // Adjust RECT to so that client delta is the old RECT
    UINT dpi = GetDpiForSystem();
    AdjustWindowRectExForDpi(&windowRect, WS_OVERLAPPEDWINDOW, 0, WS_EX_LEFT, dpi);

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
        static_cast<LPVOID>(&renderer)
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
