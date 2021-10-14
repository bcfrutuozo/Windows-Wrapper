#pragma once

// Target Windows 10 or later
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

#define Align16 void* operator new(size_t i) { return _mm_malloc(i, 16); } void operator delete(void* p) { _mm_free(p); }
#define SafeDelete(p) if(p != nullptr) { delete p; p = nullptr; }