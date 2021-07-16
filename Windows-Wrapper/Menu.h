#include "Control.h"
#include "Event.h"

#include <memory>

class MenuItem;

class Menu : public Control
{

protected:
	
	unsigned int m_Id;
	std::vector<std::shared_ptr<Menu>> m_MenuItems;
	std::unique_ptr<Event<>> m_ClickDelegate;
	
public:

	static unsigned int m_CurrentIndex;

	Menu(Control* parent);
	Menu(Control* parent, const std::string& text);
	~Menu();

	virtual unsigned int GetId()
	{
		return 0;
	}

	void SetText(const std::string& text) override;
	virtual void Bind();

	virtual void DispatchEvent(unsigned int id) const;

	virtual MenuItem& AddItem(const std::string& text, const std::function<void()>& function);
	virtual MenuItem& AddItem(const std::string& text);
};

class MenuItem : public Menu
{
private:
	

public:

	MenuItem(Control* parent, const std::string& text, const std::function<void()>& function, unsigned int i);

	virtual void Bind() override;

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

//class MenuBar : public Control
//{
//
//private:
//
//	MenuBar();
//
//public:
//
//	std::vector<Menu> m_MenuItems;
//
//	MenuBar(Control* parent);
//	~MenuBar();
//
//	Menu& AddMenu(const std::string& text);
//	MenuItem& AddItem(const std::string& text, const std::function<void()>& function);
//	MenuItem& AddItem(const std::string& text);
//
//	void SetText(const std::string& text) override;
//};