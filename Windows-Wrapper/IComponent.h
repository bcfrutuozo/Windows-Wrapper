#pragma once

#include "IDisposable.h"
#include "EventArgs.h"
#include "ISite.h"

class IComponent : public IDisposable
{
protected:

	constexpr IComponent()
		:
		m_Site(nullptr)
	{	};

	virtual ~IComponent() noexcept(false) = default;

public:

	ISite* m_Site;
	EventArgs ArgsDisposed;
};