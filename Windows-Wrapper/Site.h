#pragma once

#include "ISite.h"
#include "Container.h"

class Site : public ISite
{
public:

	Site(IComponent* const component, Container* const container, const std::string& name);

	bool IsDesignMode() override;
	void SetName(const std::string& name) override;
};