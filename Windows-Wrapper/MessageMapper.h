#pragma once

#include "Common.h"

#include <unordered_map>

class MessageMapper
{
private:

	std::unordered_map<DWORD, std::string> map;

public:

	MessageMapper() noexcept;
	std::string operator() (DWORD message, LPARAM lParam, WPARAM wParam) const noexcept;
};