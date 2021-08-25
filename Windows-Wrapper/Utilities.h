#pragma once

#include <string>

namespace Utilities
{
	inline bool CaseInsensitiveCharCompareN(char a, char b);
	inline bool CaseInsensitiveCharCompareW(wchar_t a, wchar_t b);
	bool CaseInsensitiveCompare(const std::string& s1, const std::string& s2);
	bool CaseInsensitiveCompare(const std::wstring& s1, const std::wstring& s2);
}