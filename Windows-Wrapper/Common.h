#pragma once

// Target Windows 10 or later
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

#ifndef APPLICATION
#define WIN32_LEAN_AND_MEAN
#endif

//#define NOMINMAX

// Default C++ libraries
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <functional>
#include <algorithm>
#include <string>
#include <typeinfo>
#include <queue>
#include <span>
#include <optional>
#include <bitset>
#include <unordered_map>
#include <thread>
#include <array>
#include <iterator>
#include <random>
#include <span>
#include <chrono>
#include <exception>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <limits>
#include <filesystem>
#include <Windows.h>

// Linker for Direct2D
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

// Linker for ::_TrackMouseEvent function (WinAPI.cpp usage)
#pragma comment(lib, "comctl32.lib")
#include <Commctrl.h>

// Direct2D
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

// DirectX debug
#include <dxgidebug.h>

// COM
#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

// DirectX extension libraries
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;

#define Align16 void* operator new(size_t i) { return _mm_malloc(i, 16); } void operator delete(void* p) { _mm_free(p); }
#define ArraySize(a) (sizeof(a) / sizeof(a[0]))
#define SafeDelete(p) if(p != nullptr) { delete p; p = nullptr; }

// Template function to clear COM interfaces
template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = nullptr;
	}
}

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

// Project libraries
#include "Enums.h"
#include "Mathlib.h"