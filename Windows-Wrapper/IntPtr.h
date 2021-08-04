#pragma once

#include "Common.h"
#include "IEquatable.h"

struct IntPtr : IEquatable<IntPtr>
{
private:

	void* m_Ptr;

public:

	IntPtr(int ptr);
	IntPtr(long ptr);
	IntPtr(void*);
	virtual ~IntPtr();

	inline bool Equals(const IntPtr& p) const noexcept override;
	int ToInt32() const noexcept;
	long ToInt64() const noexcept;
	void* ToPointer() const noexcept;
	int Size() const noexcept;
	bool IsNull() const noexcept;
	inline static IntPtr Add(IntPtr p, int offset) noexcept;
	IntPtr& operator=(int p) noexcept;
	IntPtr& operator=(long p) noexcept;
	IntPtr& operator=(void* p) noexcept;
	bool operator==(int p) const noexcept;
	bool operator==(long p) const noexcept;
	bool operator==(void* p) const noexcept;
	bool operator==(IntPtr p) const noexcept;

	static IntPtr Zero() { return IntPtr(0); }
	static IntPtr MaxValue() { return IntPtr(0x7fffffff); }
	static IntPtr MinValue() { return IntPtr(static_cast<int>(0x80000000)); }
};