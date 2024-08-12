#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <windows.h>

// Include renderer struct from main
#include "main.h"

// Application window procedure function
LRESULT CALLBACK WindowProcedure(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

#endif // _WINDOW_H_
