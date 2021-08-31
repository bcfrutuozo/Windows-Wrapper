#pragma once

#include "IDisposable.h"
#include "EventArgs.h"
#include "ISite.h"

class IComponent : public IDisposable
{
protected:

	IComponent()
		:
		m_Site(nullptr)
	{	};

	virtual ~IComponent() = default;

public:

	ISite* m_Site;
	EventArgs ArgsDisposed;
};