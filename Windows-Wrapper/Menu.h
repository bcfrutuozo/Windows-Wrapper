#include "Control.h"
#include "Event.h"

#include <memory>

class MenuItem;
class MenuSeparator;

class Menu : public Control
{

protected:

public:

	unsigned int m_Id;
	unsigned int m_SubItemIndex;
	std::string m_IconPath;
	HBITMAP m_Icon;
	std::vector<std::shared_ptr<Menu>> m_MenuItems;
	std::unique_ptr<Event<>> m_ClickDelegate;

	static unsigned int m_CurrentIndex;

	Menu(Control* parent, unsigned int subitemIndex, const std::string& iconPath = { });
	Menu(Control* parent, const std::string& text, unsigned int subitemIndex, const std::string& iconPath = { });
	~Menu();

	virtual unsigned int GetId()
	{
		return 0;
	}

	void SetText(const std::string& text) override;
	virtual void Bind();

	virtual void DispatchEvent(unsigned int id) const;
	void AddSeparator();
	virtual Menu& AddMenu(const std::string& text);
	virtual MenuItem& AddItem(const std::string& text, const std::function<void()>& function, const std::string& iconPath = {});
	virtual MenuItem& AddItem(const std::string& text, const std::string& iconPath = { });
};

class MenuSeparator : public Menu
{
public:

	MenuSeparator(Menu* parent, unsigned int subitemIndex);

	void Bind() override;
};


class MenuItem : public Menu
{
public:

	MenuItem(Menu* parent, const std::string& text, const std::function<void()>& function, unsigned int i, unsigned int subitemIndex, const std::string& iconPath = { });

	void Bind() override;

	void SetText(const std::string& text) override
	{
		Text = text;
	}

	void OnClickSet(const std::function<void()>& function)
	{
		m_ClickDelegate = std::make_unique<Event<>>(function);
	}

	virtual void OnClick() const
	{
		if (m_ClickDelegate != nullptr)
		{
			m_ClickDelegate->Trigger();
		}
	};
};