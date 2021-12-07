#pragma once

#include "Control.h"
#include "CreateParams.h"
#include "MDILayout.h"

#include <vector>

class Window;

class MDIClient : public Control
{
private:

	std::vector<Window*> m_Children;

protected:

	CreateParams* CreateParameters() override;

public:

	MDIClient();
	virtual ~MDIClient() = default;

	const std::vector<Window*>& GetMDIChildren() const noexcept;
	void LayoutMDI(MDILayout value) noexcept;
};