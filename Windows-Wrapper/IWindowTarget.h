#pragma once

#include "Object.h"
#include "IntPtr.h"
#include "Message.h"

class IWindowTarget : public Object
{
public:

	virtual void OnHandleChange(IntPtr newHandle) = 0;
	virtual void OnMessage(Message& m) = 0;
};