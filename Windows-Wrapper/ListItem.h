#pragma once

#include "Object.h"

class ListItem : public Object
{
public:

	int Id;
	std::string Value;
	bool Visible;
	bool Selected;

	ListItem(uint32_t id, const std::string& value);
	ListItem(const ListItem& src);
	virtual ~ListItem() = default;
};