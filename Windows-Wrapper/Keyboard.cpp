#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keyCode) const noexcept
{
	return m_KeyStates[keyCode];
}

bool Keyboard::IsKeyEmpty() const noexcept
{
	return m_KeyBuffer.empty();
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
	if (m_KeyBuffer.size() > 0u)
	{
		Keyboard::Event e = m_KeyBuffer.front();
		m_KeyBuffer.pop();
		return e;
	}
	else
	{
		return {};
	}
}

const unsigned char* Keyboard::ReadChar() noexcept
{
	if (m_CharBuffer.size() > 0u)
	{
		const unsigned char* charCode = reinterpret_cast<const unsigned char*>(&m_CharBuffer.front());
		m_CharBuffer.pop();
		return charCode;
	}
	else
	{
		return nullptr;
	}
}

bool Keyboard::IsCharEmpty() const noexcept
{
	return m_CharBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	m_KeyBuffer = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept
{
	m_CharBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	m_IsAutoRepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	m_IsAutoRepeatEnabled = false;
}

bool Keyboard::IsAutoRepeatEnabled() const noexcept
{
	return m_IsAutoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keyCode) noexcept
{
	m_KeyStates[keyCode] = true;
	m_KeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keyCode));
	TrimBuffer(m_KeyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keyCode) noexcept
{
	m_KeyStates[keyCode] = false;
	m_KeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keyCode));
	TrimBuffer(m_KeyBuffer);
}

void Keyboard::OnChar(char c) noexcept
{
	m_CharBuffer.push(c);
	TrimBuffer(m_CharBuffer);
}

void Keyboard::ClearState() noexcept
{
	m_KeyStates.reset();
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > m_BufferSize)
	{
		buffer.pop();
	}
}