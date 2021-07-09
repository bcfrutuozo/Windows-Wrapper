#include "Common.h"
#include "Mouse.h"


std::pair<int, int> Mouse::GetPos() const noexcept
{
	return{ x, y };
}

const std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
{
	if (rawDeltaBuffer.empty())
	{
		return {};
	}
	const RawDelta d = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return d;
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return isLeftPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return isRightPressed;
}

const std::optional<Mouse::Event> Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		const Event e = buffer.front();
		buffer.pop();
		return e;
	}

	return {};
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Event>();
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
	x = newX;
	y = newY;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnRawDelta(const int dx, const int dy) noexcept
{
	rawDeltaBuffer.push({ dx,dy });
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	isLeftPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	isLeftPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	isRightPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	isRightPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > buffer_size)
	{
		buffer.pop();
	}
}

void Mouse::TrimRawInputBuffer() noexcept
{
	while (rawDeltaBuffer.size() > buffer_size)
	{
		rawDeltaBuffer.pop();
	}
}

void Mouse::OnWheelDelta(const int x, const int y, const int delta) noexcept
{
	wheelDeltaCarry += delta;

	// Generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
