#pragma once

#include <queue>
#include <optional>

class Mouse
{
	friend class Window;
public:
	struct RawDelta
	{
		int x, y;
	};
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
		};
	private:
		Type type;
		bool isLeftPressed;
		bool isRightPressed;
		int x;
		int y;
	public:

		Event(const Type type, const Mouse& parent) noexcept
			:
			type(type),
			isLeftPressed(parent.isLeftPressed),
			isRightPressed(parent.isRightPressed),
			x(parent.x),
			y(parent.y)
		{}
		Type GetType() const noexcept
		{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return{ x, y };
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool IsLeftPressed() const noexcept
		{
			return isLeftPressed;
		}
		bool IsRightPressed() const noexcept
		{
			return isRightPressed;
		}
	};
public:

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	const std::optional<RawDelta> ReadRawDelta() noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	const std::optional<Mouse::Event> Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}
	void Flush() noexcept;
	void EnableRaw() noexcept;
	void DisableRaw() noexcept;
	bool IsRawEnabled() const noexcept;
private:

	void OnMouseMove(int newX, int newY) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnRawDelta(int dx, int dy) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void TrimRawInputBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	static constexpr unsigned int buffer_size = 16u;
	int x;
	int y;
	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	bool isRawEnabled = false;
	std::queue<Event> buffer;
	std::queue<RawDelta> rawDeltaBuffer;
};
