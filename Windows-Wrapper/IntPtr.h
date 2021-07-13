#pragma once

#include "IEquatable.h"

struct IntPtr : IEquatable<IntPtr>
{
private:

	void* m_Ptr;

public:

	IntPtr(int ptr);
	IntPtr(long ptr);
	IntPtr(void*);
	~IntPtr();

	inline bool Equals(IntPtr p) const noexcept override;
	void* ToPointer() const noexcept;
	int ToInt32() const noexcept;
	long ToInt64() const noexcept;
	int Size() const noexcept;
	bool IsNull() const noexcept;

	static IntPtr Zero() { return IntPtr(0); }
	static IntPtr MaxValue() { return IntPtr(0x7fffffff); }
	static IntPtr MinValue() { return IntPtr((int)0x80000000); }
};