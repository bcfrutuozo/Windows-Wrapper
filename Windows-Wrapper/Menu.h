#include "Control.h"

class MenuItem : public Control
{
	friend class Menu;

public:

	enum Type
	{
		MENU_ITEMTYPE_DISABLED,
		MENU_ITEMTYPE_STRING,
		MENU_ITEMTYPE_SEPARATOR,
	};

private:

	Type m_Type;
	IEvent* m_Callback;
	

	MenuItem(Control* parent, Type type, const std::string& text, IEvent* callback);

public:

	void SetText(const std::string& text) override
	{
		Text = text;
	}
};

class Menu : public Control
{
private:

	std::vector<MenuItem> m_SubEntries;

public:

	Menu(Control* parent);
	Menu(Control* parent, const std::string& text);
	~Menu();

	void SetText(const std::string& text) override;

	void AddEntry(MenuItem::Type type, const std::string& text, IEvent* callback);
};

class MenuBar : public Control
{
	friend class Window;

private:

	
public:

	std::vector<Menu> m_Entries;

	MenuBar(Control* parent);
	MenuBar(Control* parent, const std::string& text);
	~MenuBar();

	void SetText(const std::string& text) override;
};