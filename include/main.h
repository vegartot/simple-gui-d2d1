#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <windowsx.h>

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <dxgi1_6.h>
#include <dwrite_3.h>

#include <iostream>
#include <wrl/client.h>

#include "window.h"
#include "resource.h"
#include "enums.h" 
#include "board.h"

// Window style - should have minimize and close buttom with no resizing
#define WINDOW_STYLE WS_SYSMENU | WS_MINIMIZEBOX

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

#endif // _MAIN_H_
