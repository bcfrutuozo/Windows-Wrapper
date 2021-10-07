#pragma once

#include "CommonObject.h"

class File : public Object
{
private:

public:

	static bool Exists(const std::string& path) noexcept;
};