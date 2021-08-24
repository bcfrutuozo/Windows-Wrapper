//#pragma once
//
//#include "MenuRoot.h"
//#include "IHidable.h"
//
//class Window;
//
//class MenuBar : public MenuRoot, public IHidable
//{
//	friend class Window;
//
//protected:
//
//	void Initialize() override;
//
//public:
//
//	MenuBar(Control* parent);
//	virtual ~MenuBar() = default;
//
//	void Hide() noexcept override;
//	void Show() noexcept override;
//};