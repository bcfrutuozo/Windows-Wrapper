#pragma once

#include "CommonObject.h"

class MessageMapper : public Object
{
private:

	std::unordered_map<DWORD, std::string> map;

public:

	MessageMapper() noexcept;
	virtual ~MessageMapper() = default;

	std::string operator() (DWORD message, LPARAM lParam, WPARAM wParam) const noexcept;
};