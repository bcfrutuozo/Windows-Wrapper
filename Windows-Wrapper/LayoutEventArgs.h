#pragma once

#include "EventArgs.h"
#include "IComponent.h"

#include <string>

class Control;

class LayoutEventArgs : public EventArgs
{
private:

	const IComponent* m_AffectedComponent;
	const std::string m_AffectedProperty;

public:

	LayoutEventArgs(IComponent* const affectedComponent, const std::string& affectedProperty)
		:
		m_AffectedComponent(affectedComponent),
		m_AffectedProperty(affectedProperty)
	{}

	LayoutEventArgs(Control* const affectedControl, const std::string& affectedProperty)
		:
		m_AffectedComponent((IComponent*)affectedControl),
		m_AffectedProperty(affectedProperty)
	{}

	const IComponent* AffectedComponent() { return m_AffectedComponent; }
	const Control* AffectedControl() { return (Control*)m_AffectedComponent; }
	const std::string AffectedProperty() { return m_AffectedProperty; }
};