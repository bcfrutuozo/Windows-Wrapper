#pragma once

// Target Windows 10 or later
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

#ifndef APPLICATION
#define WIN32_LEAN_AND_MEAN
#endif

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
#include <Windows.h>

// DirectX debug
#include <dxgidebug.h>

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::PackedVector;

#define NOMINMAX

#define Align16 void* operator new(size_t i) { return _mm_malloc(i, 16); } void operator delete(void* p) { _mm_free(p); }
#define ArraySize(a) (sizeof(a) / sizeof(a[0]))
#define SafeDelete(p) { delete p; p == nullptr; }

// Project libraries
#include "Enums.h"
#include "Mathlib.h"