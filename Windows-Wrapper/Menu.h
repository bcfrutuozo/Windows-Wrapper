#pragma once

#include <string>
#include <functional>

template<class Return, class... Args>
class MenuItem
{
public:

	enum class Type
	{
		Disabled,
		String,
		Separator,
	};

private:

	Type m_Type;
	std::string m_Text;
	static unsigned int m_Index;
	const std::function<Return(Args...)> m_Function;

public:

	MenuItem(Type type, const std::string& text, const std::function<Return(Args...)>& function)
		:
		m_Type(type),
		m_Text(text),
		m_Function(function)
	{
		std::function(m_Function);
	}

	Return Invoke(Args&&... args)
	{
		return m_Function(std::forward<Args>(args)...);
	}
};

//int test(std::string abc, std::string xyz, int z) { return z; }

//int test3(std::string abc, int xyz, int z) { return z; }

//std::string test2(std::string abc, std::string xyz, int z) { return "z"; }

//int x()
//{
//	std::string ube("ABC");
//	MenuItem<int, std::string, std::string, int> menu(test);
//	return 4;
//	return menu.Invoke("abc", "abc", 4);
//}