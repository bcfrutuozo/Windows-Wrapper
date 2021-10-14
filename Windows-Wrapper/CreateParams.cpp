#include "CreateParams.h"

#include <sstream>

std::string CreateParams::ToString() const noexcept
{
    std::ostringstream oss;
    oss << "CreateParams {'"
        << ClassName
        << "', '"
        << Caption
        << "', 0x"
        << std::hex << Style
        << ", 0x"
        << std::hex << ExStyle
        << ", {"
        << X
        << ", "
        << Y
        << ", "
        << Width
        << ", "
        << Height
        << "}"
        << "}";
    return oss.str().c_str();
}