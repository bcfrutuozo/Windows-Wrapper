#include "Utilities.h"

namespace Utilities
{
	inline bool CaseInsensitiveCharCompareN(char a, char b)
	{
		return(toupper(a) == toupper(b));
	}

	inline bool CaseInsensitiveCharCompareW(wchar_t a, wchar_t b)
	{
		return(towupper(a) == towupper(b));
	}

	bool CaseInsensitiveCompare(const std::string& s1, const std::string& s2)
	{
		return((s1.size() == s2.size()) &&
			equal(s1.begin(), s1.end(), s2.begin(), CaseInsensitiveCharCompareN));
	}

	bool CaseInsensitiveCompare(const std::wstring& s1, const std::wstring& s2)
	{
		return((s1.size() == s2.size()) &&
			equal(s1.begin(), s1.end(), s2.begin(), CaseInsensitiveCharCompareW));
	}
}