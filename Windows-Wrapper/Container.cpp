#include "Container.h"
#include "Site.h"
#include "IComponent.h"
#include "Utilities.h"

void Container::Remove(IComponent* component, bool preserveSite)
{
	if (component == nullptr)
	{
		return;
	}

	ISite* site = component->m_Site;

	if (site == nullptr || site->GetContainer() != this)
	{
		return;
	}

	if (!preserveSite)
	{
		component->m_Site = nullptr;
	}

	for (size_t i = 0; i < SiteCount; ++i)
	{
		if (m_Sites[i] == site)
		{
			--SiteCount;
			m_Sites.erase(m_Sites.begin() + i);
		}
	}
}

void Container::Dispose(bool disposing)
{
	if (disposing)
	{
		while (SiteCount > 0)
		{
			ISite* site = m_Sites[--SiteCount];
			delete site->GetComponent()->m_Site;
			site->GetComponent()->Dispose();
		}

		m_Sites.clear();
	}
}

ISite* Container::CreateSite(IComponent* const component, const std::string& name)
{
	return new Site(component, this, name);
}

void Container::ValidateName(IComponent* component, const std::string& name)
{
	if (component == nullptr)
	{
		throw;
	}

	if (name.empty())
	{
		for (size_t i = 0; i < (std::min)(SiteCount, m_Sites.size()); ++i)
		{
			ISite* s = m_Sites[i];
			if (s != nullptr &&
				s->GetName() != "" &&
				Utilities::CaseInsensitiveCompare(name, s->GetName()) &&
				s->GetComponent() != component)
			{
				// Duplicate name for site. Create Exception for it
				throw;
			}
		}
	}
}

void Container::RemoveWithoutUnsiting(IComponent* component)
{
	Remove(component, true);
}

Container::Container()
	:
	SiteCount(0)
{
	m_Sites.reserve(4);
}

Container::~Container()
{
	Dispose(false);
}

void Container::Add(IComponent* const component)
{
	Add(component, "");
}

void Container::Add(IComponent* const component, const std::string& name)
{
	if (component == nullptr)
	{
		return;
	}

	ISite* site = component->m_Site;

	if (site != nullptr && site->GetContainer() == this)
	{
		return;
	}

	ValidateName(component, name);

	if (m_Sites.size() == SiteCount)
	{
		std::vector<ISite*> newSites(SiteCount * 2);
		std::copy(m_Sites.begin(), m_Sites.end(), newSites.begin());
		m_Sites = newSites;
	}

	if (site != nullptr)
	{
		site->GetContainer()->Remove(component);
	}

	ISite* newSite = CreateSite(component, name);
	m_Sites[SiteCount++] = newSite;
	component->m_Site = site;
}

void Container::Remove(IComponent* const component)
{
	Remove(component, false);
}