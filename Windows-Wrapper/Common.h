#pragma once

// Target Windows 10 or later
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>
// The following #defines disable a bunch of unused windows stuff. If you 
// get weird errors when trying to do some windows stuff, try removing some
// (or all) of these defines (it will increase build time though).
#ifndef APPLICATION
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
//#define NOSYSMETRICS
//#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

// Globalize IntPtr.h struct
#include "IntPtr.h"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;
static const XMFLOAT4X4 IDENTITYMATRIX = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

//#define HIDWORD(dw, hw) LOWORD(dw) | (hw << 16)
//#define LODWORD(dw, lw) (HIWORD(dw) << 16) | lw
#define arraysize(a) (sizeof(a) / sizeof(a[0]))
#define NOMINMAX
#define ALIGN_16 void* operator new(size_t i) { return _mm_malloc(i, 16); } void operator delete(void* p) { _mm_free(p); }

#include <Windows.h>