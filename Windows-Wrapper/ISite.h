#pragma once

#include <string>

class IComponent;
class IContainer;

class ISite
{
protected:

	IComponent* m_Component = nullptr;
	IContainer* m_Container = nullptr;
	bool m_DesingMode;
	std::string m_Name;

public:

	IComponent* GetComponent() { return m_Component; };
	IContainer* GetContainer() { return m_Container; };
	virtual bool IsDesignMode() = 0;
	std::string GetName() { return m_Name; }
	virtual void SetName(const std::string& name) = 0;
};