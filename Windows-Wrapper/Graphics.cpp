#include "Graphics.h"
#include "GDI.h"
#include "Direct2D.h"
#include "Application.h"

Graphics::Graphics(IntPtr window, Size size)
	:
	WindowHandle(window),
	WindowSize(size)
{

}

Graphics* Graphics::Create(IntPtr window, Size size)
{
	switch (Application::GetGraphicsType())
	{
	case GraphicsType::D2D: return new Direct2D(window, size);
	case GraphicsType::GDI: return new GDI(window, size);
	default:
		throw std::runtime_error("Invalid graphics type format");
	}
}