#pragma once

#include "CommonObject.h"

class Mouse : public Object
{
	friend class Window;

public:

	struct RawDelta
	{
		int X;
		int Y;
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

		Event(const Type type, const Mouse& parent) noexcept;

		Type GetType() const noexcept;
		std::pair<int, int> GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool IsLeftPressed() const noexcept;
		bool IsRightPressed() const noexcept;

	private:

		Type m_Type;
		bool m_IsLeftPressed;
		bool m_IsRightPressed;
		int m_X;
		int m_Y;
	};
public:

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	virtual ~Mouse() = default;

	std::pair<int, int> GetPos() const noexcept;
	const std::optional<RawDelta> ReadRawDelta() noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	int GetDelta() const noexcept;
	bool IsInWindow() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	const std::optional<Mouse::Event> Read() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;
	void EnableRaw() noexcept;
	void DisableRaw() noexcept;
	bool IsRawEnabled() const noexcept;

private:

	static constexpr int WHEEL_DELTA_VALUE = 120;
	static constexpr unsigned int m_BufferSize = 16u;
	int m_X = 0;
	int m_Y = 0;
	bool m_IsLeftPressed = false;
	bool m_IsRightPressed = false;
	bool m_IsInWindow = false;
	int m_WheelDeltaCarry = 0;
	bool isRawEnabled = false;
	std::queue<Event> m_Buffer;
	std::queue<RawDelta> m_RawDeltaBuffer;

	void OnMouseMove(int newX, int newY) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnRawDelta(int dx, int dy) noexcept;
	void OnLeftPressed() noexcept;
	void OnLeftReleased() noexcept;
	void OnRightPressed() noexcept;
	void OnRightReleased() noexcept;
	void OnWheelUp() noexcept;
	void OnWheelDown() noexcept;
	void TrimBuffer() noexcept;
	void TrimRawInputBuffer() noexcept;
	void OnWheelDelta(int delta) noexcept;
};
