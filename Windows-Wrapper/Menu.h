#include "Control.h"

class MenuItem
{
public:

	enum Type
	{
		MENU_ITEMTYPE_DISABLED,
		MENU_ITEMTYPE_STRING,
		MENU_ITEMTYPE_SEPARATOR,
	};

private:

	Type m_Type;
	std::string m_Text;
	IEvent* m_Callback;

public:

	MenuItem(Type type, const std::string& text, IEvent* callback)
		:
		m_Type(type),
		m_Text(text),
		m_Callback(callback)
	{
	}

	const Type& GetType() noexcept
	{
		return m_Type;
	}

	const std::string& GetText() noexcept
	{
		return m_Text;
	}

	void SetText(const std::string& text) noexcept
	{
		m_Text = text;
	}
};

class Menu : public Control
{
	friend class Window;

private:

	HMENU m_MenuBar;
	HMENU m_Menu;

public:

	Menu(Control* parent);
	Menu(Control* parent, const std::string& text);
	~Menu();

	void SetText(const std::string& text) override;

	void AddEntry(MenuItem::Type type, const std::string& text, IEvent* callback);
};