#pragma once

#include "Object.h"

class ListItem : public Object
{
public:

	std::string Value;
	bool Visible;
	bool Selected;

	ListItem(const std::string& value);
	ListItem(const ListItem& src);
};