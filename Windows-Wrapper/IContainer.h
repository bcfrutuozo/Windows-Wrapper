#pragma once

#include "IDisposable.h"

#include <string>
#include <vector>

class IComponent;

class IContainer : public IDisposable
{
protected:

	std::vector<IComponent*> Components;

public:

	virtual ~IContainer() = default;

	virtual void Add(IComponent* const component) = 0;
	virtual void Add(IComponent* const component, const std::string& name) = 0;
	virtual void Remove(IComponent* const component) = 0;

	constexpr std::vector<IComponent*>& GetComponents() { return Components; }
};