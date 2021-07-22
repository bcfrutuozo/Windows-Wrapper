#pragma once

#include "Base.h"
#include "Common.h"
#include "IHandle.h"
#include "Size.h"
#include "Event.h"

class Window;

class Control : public IHandle, public Base
{
protected:

	std::string Text;
	Size Size;
	Point Location;

public:

	Control* Parent;
	std::vector<std::shared_ptr<Control>> Controls;

	Control();
	Control(Control* parent, const std::string& text);
	Control(Control* parent, const std::string& text, int width, int height, int x, int y);
	Control(const std::string& text);
	Control(const std::string& text, int width, int height, int x, int y);
	Control(Control&& other) = default;
	Control& operator=(const Control&) = default;
	Control& operator=(Control&&) = default;
	virtual ~Control();

	virtual void Bind() noexcept = 0;
	const std::string& GetText() const noexcept;

	template<typename T, typename... Args>
	T& Create(Args... a)
	{
		auto obj = std::make_shared<T>(a...);
		Controls.push_back(obj);
		obj->Bind();
		return dynamic_cast<T&>(*Controls.back());
	}
};

