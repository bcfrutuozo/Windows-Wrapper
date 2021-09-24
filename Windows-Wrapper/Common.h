#pragma once

// Target Windows 10 or later
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

#ifndef APPLICATION
#define WIN32_LEAN_AND_MEAN
#endif

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;

static const XMFLOAT4X4 IDENTITYMATRIX = XMFLOAT4X4(1, 0, 0, 0,
													0, 1, 0, 0, 
													0, 0, 1, 0, 
													0, 0, 0, 1);

#define arraysize(a) (sizeof(a) / sizeof(a[0]))
#define NOMINMAX
#define ALIGN_16 void* operator new(size_t i) { return _mm_malloc(i, 16); } void operator delete(void* p) { _mm_free(p); }

#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>