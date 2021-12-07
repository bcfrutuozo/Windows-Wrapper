#pragma once

#include "Exceptions.h"

#include <array>

template<typename Key, typename Value, std::size_t Size>
struct Map
{
	std::array<std::pair<Key, Value>, Size> data;
	constexpr Value at(const Key& key) const
	{
		const auto it = std::find_if(begin(data), end(data),
									 [&key](const auto& v) { return v.first == key; });

		if(it != end(data)) return it->second;
		else return "";		// Always return empty string for unknown messages (Messages created at runtime starts with C0)
	}
};