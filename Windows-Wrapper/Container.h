#pragma once

#include "IContainer.h"
#include "IComponent.h"

#include <algorithm>

class Container : public IContainer
{
	friend class Site;

private:

	std::vector<ISite*> m_Sites;
	size_t SiteCount;

	void Remove(IComponent* component, bool preserveSite);

protected:

	void Dispose(bool disposing);
	virtual ISite* CreateSite(IComponent* const component, const std::string& name);
	virtual void ValidateName(IComponent* component, const std::string& name);
	void RemoveWithoutUnsiting(IComponent* component);

public:

	Container();
	virtual ~Container();

	virtual void Add(IComponent* const component) override;
	virtual void Add(IComponent* const component, const std::string& name);
	virtual void Remove(IComponent* const component) override;
};

