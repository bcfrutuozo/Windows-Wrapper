#pragma once

#include <queue>
#include <optional>
#include <bitset>

class Keyboard
{
	friend class Window;

public:

	class Event
	{

	public:

		enum class Type
		{
			Press,
			Release,
			Invalid
		};

		Event();
		Event(Type type, unsigned char code) noexcept;

		bool IsPress() const noexcept;
		bool IsRelease() const noexcept;
		bool IsValid() const noexcept;
		unsigned char GetCode() const noexcept;

	private:

		Type m_Type;
		unsigned char m_Code;
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// Key event handling
	bool IsKeyPressed(unsigned char keyCode) const noexcept;
	std::optional<Event> ReadKey() noexcept;
	bool IsKeyEmpty() const noexcept;
	void FlushKey() noexcept;

	// Char event handling
	const unsigned char* ReadChar() noexcept;
	bool IsCharEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;

	// Autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool IsAutoRepeatEnabled() const noexcept;

private:
	
	static constexpr unsigned int m_NumberOfKeys = 256u;
	static constexpr unsigned int m_BufferSize = 16u;
	bool m_IsAutoRepeatEnabled = false;
	std::bitset<m_NumberOfKeys> m_KeyStates;
	std::queue<Event> m_KeyBuffer;
	std::queue<char> m_CharBuffer;

	void OnKeyPressed(unsigned char keyCode) noexcept;
	void OnKeyReleased(unsigned char keyCode) noexcept;
	void OnChar(char c) noexcept;
	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
};

