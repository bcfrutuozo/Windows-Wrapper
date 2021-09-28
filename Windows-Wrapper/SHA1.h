#pragma once

#include <stdint.h>
#include <iostream>

class SHA1
{
public:

	using digest32_t = uint32_t[5];
	using digest8_t = uint8_t[20];

	SHA1();

	void Reset();
	void ProcessByte(uint8_t octet);
	void ProcessBlock(void const* const start, void const* const end);
	void ProcessBytes(void const* const data, size_t const len);
	uint32_t const* ComputeHash(digest32_t digest);
	uint8_t const* ComputeHash(digest8_t digest);

	inline static uint32_t LeftRotate(uint32_t value, size_t const count) { return (value << count) ^ (value >> (32 - count)); }

	static constexpr unsigned int BlockBytes = 64;

private:

	digest32_t m_Digest;
	uint8_t m_Block[64];
	size_t m_BlockByteIndex;
	size_t m_ByteCount;

	void ProcessBlock();
};