#include "Control.h"
#include "Event.h"

class MenuItem;

class Menu : public Control
{
private:

	std::vector<Menu> m_MenuItems;

public:

	Menu(Control* parent);
	Menu(Control* parent, const std::string& text);
	~Menu();

	void SetText(const std::string& text) override;
	virtual void Bind();
	MenuItem& AddItem(const std::string& text, IEvent* callback);

	virtual void OnMouseLeftDown() const {};
	virtual void OnMouseRightDown() const {};
	virtual void OnMouseOver() const {}

};

class MenuItem : public Menu
{

public:

	enum Type
	{
		MENU_ITEMTYPE_DISABLED,
		MENU_ITEMTYPE_STRING,
		MENU_ITEMTYPE_SEPARATOR,
		MENU_ITEMTYPE_POPUP,
	};

public:

	MenuItem(Control* parent, const std::string& text, IEvent* callback);

	virtual void Bind() override;

	void SetText(const std::string& text) override
	{
		Text = text;
	}
};

class MenuBar : public Control
{

private:

	MenuBar();

public:

	std::vector<Menu> m_MenuItems;

	MenuBar(Control* parent);
	~MenuBar();

	Menu& AddMenu(const std::string& text);
	MenuItem& AddItem(const std::string& text, IEvent* callback);

	void SetText(const std::string& text) override;
};