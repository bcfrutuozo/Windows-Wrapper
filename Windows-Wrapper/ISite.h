#pragma once

#include <string>

class IComponent;
class IContainer;

class ISite
{
	friend class Container;

protected:

	IComponent* m_Component;
	IContainer* m_Container;
	bool m_DesingMode;
	std::string m_Name;

	ISite()
		:
		m_Component(nullptr),
		m_Container(nullptr),
		m_DesingMode(false)
	{	}

	virtual ~ISite() = default;

public:

	IComponent* GetComponent() { return m_Component; };
	IContainer* GetContainer() { return m_Container; };
	virtual bool IsDesignMode() = 0;
	std::string GetName() { return m_Name; }
	virtual void SetName(const std::string& name) = 0;
};