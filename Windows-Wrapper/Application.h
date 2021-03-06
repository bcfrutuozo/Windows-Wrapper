#pragma once

#include "Object.h"
#include "Appheaders.h"
#include "GraphicsType.h"

#include <list>

class Application : public Object
{
	friend class Window;
	friend class NativeWindow;
	friend class Control;

private:

	static bool m_IsRunning;
	static bool m_HasGraphicsChanged;
	static GraphicsType m_SetGraphicsType;

	static void AddWindow(Window* window);
	static bool RemoveWindow(Window* window);

protected:

	static std::list<Window*>* Windows;

	Application() = default;
	Application(const Application&) = delete;				// Copy constructor
	Application(Application&&) = delete;					// Move constructor
	Application& operator=(const Application&) = delete;	// Copy assignment constructor
	Application& operator=(Application&&) = delete;			// Move assignment constructor
	~Application() = default; 

	static void Start() noexcept;
	static void Stop() noexcept;

public:

	static constexpr bool IsRunning() { return m_IsRunning; }
	static constexpr bool HasGraphicsChanged() { return m_HasGraphicsChanged; }
	static constexpr GraphicsType GetGraphicsType() noexcept { return m_SetGraphicsType; }
	static void SetGraphicsType(GraphicsType graphicsType) noexcept;

	virtual void Initialize() = 0;
	static bool CanCloseApplication() noexcept;
	static void Exit() noexcept;
};