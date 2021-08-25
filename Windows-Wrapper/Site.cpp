#include "Site.h"
#include "Component.h"
#include "Utilities.h"

Site::Site(IComponent* const component, Container* const container, const std::string& name)
{
	m_Component = component;
	m_Container = container;
	m_Name = name;
}

bool Site::IsDesignMode()
{
	return false;
}

void Site::SetName(const std::string& name)
{
	if (name == "" || m_Name == "" || !Utilities::CaseInsensitiveCompare(name, m_Name))
	{
		dynamic_cast<Container*>(m_Container)->ValidateName(m_Component, name);
		m_Name = name;
	}
}