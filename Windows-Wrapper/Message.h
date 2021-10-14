#pragma once

#include "IntPtr.h"

#include <string>

struct Message final : public Object
{
public:

	IntPtr hWnd;
	int Msg;
	IntPtr wParam;
	IntPtr lParam;
	IntPtr Result;

	constexpr bool operator==(const Message& b) { return Equals(&b); }
	constexpr bool operator!=(const Message& b) { return !Equals(&b); }

	template<typename T>
	Object* GetLParam() const noexcept
	{
		return dynamic_cast<Object*>(dynamic_cast<T*>(lParam));
	}

	static Message Create(IntPtr hwnd, int msg, IntPtr wparam, IntPtr lparam) noexcept;

	bool Equals(const Object* const b) const override;
	int GetHashCode() const noexcept override;
	std::string ToString() const noexcept override;
};