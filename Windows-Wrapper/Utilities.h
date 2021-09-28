#pragma once

#include <string>
#include <algorithm>

namespace Utilities
{
	inline bool CaseInsensitiveCharCompareN(char a, char b);
	inline bool CaseInsensitiveCharCompareW(wchar_t a, wchar_t b);
	bool CaseInsensitiveCompare(const std::string& s1, const std::string& s2);
	bool CaseInsensitiveCompare(const std::wstring& s1, const std::wstring& s2);
}

namespace std
{
	template<class _container,
		class _Ty> inline
		bool contains(_container _C, const _Ty& _Val)
	{
		return std::find(_C.begin(), _C.end(), _Val) != _C.end();
	}
};