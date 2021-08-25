#pragma once

#include "IComponent.h"

#include <string>
#include <vector>

class IContainer : public IDisposable
{
protected:

	std::vector<IComponent*> Components;

public:

	virtual void Add(IComponent* const component) = 0;
	virtual void Add(IComponent* const component, const std::string& name) = 0;
	const std::vector<IComponent*>& GetComponents() { return Components; }
	virtual void Remove(IComponent* const component) = 0;
};