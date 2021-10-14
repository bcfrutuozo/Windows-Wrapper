#pragma once

#include "Object.h"

#include <string>

class File : public Object
{
private:

public:

	static bool Exists(const std::string& path) noexcept;
};