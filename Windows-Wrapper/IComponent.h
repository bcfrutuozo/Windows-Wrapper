#pragma once

#include "IDisposable.h"
#include "EventArgs.h"
#include "ISite.h"

class IComponent : public IDisposable
{
public:

	ISite* m_Site = nullptr;
	EventArgs ArgsDisposed;
};