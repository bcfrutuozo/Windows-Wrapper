#pragma once

#include "Object.h"

class ListItem : public Object
{
public:

	uint32_t Id;
	std::string Value;

	ListItem(uint32_t id, const std::string& value);
};