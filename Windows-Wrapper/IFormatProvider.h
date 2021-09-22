#pragma once

#include "Object.h"

class IFormatProvider
{
	// Interface does not need to be marked with the serializable attribute
	virtual Object GetFormat(Type formatType) const noexcept = 0;
};