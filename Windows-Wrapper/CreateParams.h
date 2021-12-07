#pragma once

#include "Object.h"
#include "IntPtr.h"

#include <string>

class CreateParams final : public Object
{
public:

    std::string ClassName;
    std::string Caption;
    int Style;
    int ExStyle;
    int ClassStyle;
    int X;
    int Y;
    int Width;
    int Height;
    IntPtr Parent;
    void* Param;

    std::string ToString() const noexcept override;
};