#pragma once

#include "Object.h"

class ListItem : public Object
{
public:

	uint64_t Id;
	std::string Value;
	bool Visible;
	bool Selected;

	ListItem(uint64_t id, const std::string& value);
	ListItem(const ListItem& src);
	virtual ~ListItem() = default;
};