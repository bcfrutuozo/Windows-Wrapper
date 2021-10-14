#pragma once

#include "Object.h"

#include <string>

class ListItem : public Object
{
public:

	int Id;
	std::string Value;
	bool Tabulated;
	bool Visible;
	bool Selected;

	ListItem(int id, const std::string& value);
	virtual ~ListItem() = default;
};