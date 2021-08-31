#include "Mouse.h"

Mouse::Event::Event(const Type type, const Mouse& parent) noexcept
	:
	m_Type(type),
	m_IsLeftPressed(parent.m_IsLeftPressed),
	m_IsRightPressed(parent.m_IsRightPressed),
	m_X(parent.m_X),
	m_Y(parent.m_Y)
{}

Mouse::Event::Type Mouse::Event::GetType() const noexcept
{
	return m_Type;
}

std::pair<int, int> Mouse::Event::GetPos() const noexcept
{
	return{ m_X, m_Y };
}

int Mouse::Event::GetPosX() const noexcept
{
	return m_X;
}

int Mouse::Event::GetPosY() const noexcept
{
	return m_Y;
}

bool Mouse::Event::IsLeftPressed() const noexcept
{
	return m_IsLeftPressed;
}

bool Mouse::Event::IsRightPressed() const noexcept
{
	return m_IsRightPressed;
}


std::pair<int, int> Mouse::GetPos() const noexcept
{
	return{ m_X, m_Y };
}

const std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
{
	if (m_RawDeltaBuffer.empty())
	{
		return {};
	}
	const RawDelta d = m_RawDeltaBuffer.front();
	m_RawDeltaBuffer.pop();
	return d;
}

int Mouse::GetPosX() const noexcept
{
	return m_X;
}

int Mouse::GetPosY() const noexcept
{
	return m_Y;
}

int Mouse::GetDelta() const noexcept
{
	return m_WheelDeltaCarry;
}

bool Mouse::IsInWindow() const noexcept
{
	return m_IsInWindow;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return m_IsLeftPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return m_IsRightPressed;
}

const std::optional<Mouse::Event> Mouse::Read() noexcept
{
	if (m_Buffer.size() > 0u)
	{
		const Event e = m_Buffer.front();
		m_Buffer.pop();
		return e;
	}

	return {};
}

bool Mouse::IsEmpty() const noexcept
{
	return m_Buffer.empty();
}

void Mouse::Flush() noexcept
{
	m_Buffer = std::queue<Event>();
}

void Mouse::EnableRaw() noexcept
{
	isRawEnabled = true;
}

void Mouse::DisableRaw() noexcept
{
	isRawEnabled = false;
}

bool Mouse::IsRawEnabled() const noexcept
{
	return isRawEnabled;
}

void Mouse::OnMouseMove(const int newX, const int newY) noexcept
{
	m_X = newX;
	m_Y = newY;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	m_IsInWindow = false;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	m_IsInWindow = true;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnRawDelta(const int dx, const int dy) noexcept
{
	m_RawDeltaBuffer.push({ dx,dy });
	TrimBuffer();
}

void Mouse::OnLeftPressed() noexcept
{
	m_IsLeftPressed = true;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased() noexcept
{
	m_IsLeftPressed = false;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed() noexcept
{
	m_IsRightPressed = true;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased() noexcept
{
	m_IsRightPressed = false;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp() noexcept
{
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown() noexcept
{
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (m_Buffer.size() > m_BufferSize)
	{
		m_Buffer.pop();
	}
}

void Mouse::TrimRawInputBuffer() noexcept
{
	while (m_RawDeltaBuffer.size() > m_BufferSize)
	{
		m_RawDeltaBuffer.pop();
	}
}

void Mouse::OnWheelDelta(const int x, const int y, const int delta) noexcept
{
	m_WheelDeltaCarry += delta;

	// Generate events for every 120 
	while (m_WheelDeltaCarry >= WHEEL_DELTA_VALUE)
	{
		m_WheelDeltaCarry -= WHEEL_DELTA_VALUE;
		OnWheelUp();
	}
	while (m_WheelDeltaCarry <= -WHEEL_DELTA_VALUE)
	{
		m_WheelDeltaCarry += WHEEL_DELTA_VALUE;
		OnWheelDown();
	}
}
