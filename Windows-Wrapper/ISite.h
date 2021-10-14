#pragma once

#include "Object.h"

#include <string>

class IComponent;
class IContainer;

class ISite : public Object
{
	friend class Container;

protected:

	IComponent* m_Component;
	IContainer* m_Container;
	bool m_DesingMode;
	std::string m_Name;

	constexpr ISite()
		:
		m_Component(nullptr),
		m_Container(nullptr),
		m_DesingMode(false)
	{	}

	virtual ~ISite() = default;

public:

	virtual bool IsDesignMode() = 0;
	virtual void SetName(const std::string& name) = 0;

	constexpr IComponent* GetComponent() const noexcept { return m_Component; };
	constexpr IContainer* GetContainer() const noexcept { return m_Container; };
	constexpr std::string GetName() const noexcept { return m_Name; }
};