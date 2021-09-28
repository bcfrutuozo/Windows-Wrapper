#include "SHA1.h"

SHA1::SHA1()
{
	Reset();
}

void SHA1::Reset()
{
	m_Digest[0] = 0x67452301;
	m_Digest[1] = 0xEFCDAB89;
	m_Digest[2] = 0x98BADCFE;
	m_Digest[3] = 0x10325476;
	m_Digest[4] = 0xC3D2E1F0;
	m_BlockByteIndex = 0;
	m_ByteCount = 0;
}

void SHA1::ProcessByte(uint8_t octet)
{
	this->m_Block[this->m_BlockByteIndex++] = octet;
	++this->m_ByteCount;
	if (m_BlockByteIndex == BlockBytes)
	{
		this->m_BlockByteIndex = 0;
		ProcessBlock();
	}
}

void SHA1::ProcessBlock(void const* const start, void const* const end)
{
	const uint8_t* begin = static_cast<const uint8_t*>(start);
	const uint8_t* finish = static_cast<const uint8_t*>(end);
	while (begin != finish)
	{
		ProcessByte(*begin);
		begin++;
	}
}

void SHA1::ProcessBytes(void const* const data, size_t const len)
{
	const uint8_t* block = static_cast<const uint8_t*>(data);
	ProcessBlock(block, block + len);
}

uint32_t const* SHA1::ComputeHash(digest32_t digest)
{
	size_t const bitCount = this->m_ByteCount * 8;
	ProcessByte(0x80);

	if (this->m_BlockByteIndex > 56) 
	{
		while (m_BlockByteIndex != 0) 
		{
			ProcessByte(0);
		}

		while (m_BlockByteIndex < 56) 
		{
			ProcessByte(0);
		}
	}
	else 
	{
		while (m_BlockByteIndex < 56) 
		{
			ProcessByte(0);
		}
	}

	ProcessByte(0);
	ProcessByte(0);
	ProcessByte(0);
	ProcessByte(0);
	ProcessByte(static_cast<unsigned char>((bitCount >> 24) & 0xFF));
	ProcessByte(static_cast<unsigned char>((bitCount >> 16) & 0xFF));
	ProcessByte(static_cast<unsigned char>((bitCount >> 8) & 0xFF));
	ProcessByte(static_cast<unsigned char>((bitCount) & 0xFF));

	memcpy(digest, m_Digest, 5 * sizeof(uint32_t));
	return digest;
}

uint8_t const* SHA1::ComputeHash(digest8_t digest)
{
	digest32_t d32;
	ComputeHash(d32);

	size_t di = 0;
	digest[di++] = ((d32[0] >> 24) & 0xFF);
	digest[di++] = ((d32[0] >> 16) & 0xFF);
	digest[di++] = ((d32[0] >> 8) & 0xFF);
	digest[di++] = ((d32[0]) & 0xFF);

	digest[di++] = ((d32[1] >> 24) & 0xFF);
	digest[di++] = ((d32[1] >> 16) & 0xFF);
	digest[di++] = ((d32[1] >> 8) & 0xFF);
	digest[di++] = ((d32[1]) & 0xFF);

	digest[di++] = ((d32[2] >> 24) & 0xFF);
	digest[di++] = ((d32[2] >> 16) & 0xFF);
	digest[di++] = ((d32[2] >> 8) & 0xFF);
	digest[di++] = ((d32[2]) & 0xFF);

	digest[di++] = ((d32[3] >> 24) & 0xFF);
	digest[di++] = ((d32[3] >> 16) & 0xFF);
	digest[di++] = ((d32[3] >> 8) & 0xFF);
	digest[di++] = ((d32[3]) & 0xFF);

	digest[di++] = ((d32[4] >> 24) & 0xFF);
	digest[di++] = ((d32[4] >> 16) & 0xFF);
	digest[di++] = ((d32[4] >> 8) & 0xFF);
	digest[di++] = ((d32[4]) & 0xFF);

	return digest;
}

void SHA1::ProcessBlock()
{
	uint32_t w[80] = { 0 };

	for (size_t i = 0; i < 16; i++) 
	{
		w[i] = (m_Block[i * 4 + 0] << 24);
		w[i] |= (m_Block[i * 4 + 1] << 16);
		w[i] |= (m_Block[i * 4 + 2] << 8);
		w[i] |= (m_Block[i * 4 + 3]);
	}

	for (size_t i = 16; i < 80; i++) 
	{
		w[i] = LeftRotate((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]), 1);
	}

	uint32_t a = m_Digest[0];
	uint32_t b = m_Digest[1];
	uint32_t c = m_Digest[2];
	uint32_t d = m_Digest[3];
	uint32_t e = m_Digest[4];

	for (std::size_t i = 0; i < 80; ++i)
	{
		uint32_t f = 0;
		uint32_t k = 0;

		if (i < 20) 
		{
			f = (b & c) | (~b & d);
			k = 0x5A827999;
		}
		else if (i < 40) 
		{
			f = b ^ c ^ d;
			k = 0x6ED9EBA1;
		}
		else if (i < 60) 
		{
			f = (b & c) | (b & d) | (c & d);
			k = 0x8F1BBCDC;
		}
		else 
		{
			f = b ^ c ^ d;
			k = 0xCA62C1D6;
		}

		uint32_t temp = LeftRotate(a, 5) + f + e + k + w[i];
		e = d;
		d = c;
		c = LeftRotate(b, 30);
		b = a;
		a = temp;
	}

	m_Digest[0] += a;
	m_Digest[1] += b;
	m_Digest[2] += c;
	m_Digest[3] += d;
	m_Digest[4] += e;
}