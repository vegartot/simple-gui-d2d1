#ifndef _MAIN_H_
#define _MAIN_H_

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
#include "enums.h" 
#include "board.h"

// Window style - should have minimize and close buttom with no resizing
#define WINDOW_STYLE WS_SYSMENU | WS_MINIMIZEBOX


#endif // _MAIN_H_