#include "Graphics3d.h"
//#include "dxerr.h"
////#include "DepthStencil.h"
////#include "RenderTarget.h"
//
#include <sstream>
#include <d3dcompiler.h>
#include <array>
//
//Graphics::Graphics(HWND handle, int width, int height)
//	:
//	m_Width(width),
//	m_Height(height),
//{
//	DXGI_SWAP_CHAIN_DESC swapDescriptor = {};
//
//	swapDescriptor.BufferDesc.Width = width;
//	swapDescriptor.BufferDesc.Height = height;
//	swapDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//	// Layout of the pixels RGBA
//	swapDescriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	// Refresh rate
//	swapDescriptor.BufferDesc.RefreshRate.Numerator = 0;
//	swapDescriptor.BufferDesc.RefreshRate.Denominator = 0;
//	// Interlaced	
//	swapDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//	// Anti-aliasing
//	swapDescriptor.SampleDesc.Count = 1;
//	swapDescriptor.SampleDesc.Quality = 0;
//	// Buffer function 
//	swapDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	// Number of BACK-buffers
//	swapDescriptor.BufferCount = 1;
//	// Window Handle
//	swapDescriptor.OutputWindow = handle;
//	// Windowed Mode
//	swapDescriptor.Windowed = TRUE;
//	swapDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//	swapDescriptor.Flags = 0;
//
//	UINT swapCreateFlags = 0u;
//
//#ifndef NDEBUG
//	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//	// To check results of D3D function calls
//	HRESULT hr;
//
//	// Create device and front/back buffers along with the swap chain and the rendering context
//	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
//		nullptr,
//		D3D_DRIVER_TYPE_HARDWARE,
//		nullptr,
//		swapCreateFlags,
//		nullptr,
//		0,
//		D3D11_SDK_VERSION,
//		&swapDescriptor,
//		&pSwapChain,
//		&pDevice,
//		nullptr,
//		&pContext
//	));
//
//	// Gain access to texture resource in swap chain (back buffer)
//	//Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
//	GFX_THROW_INFO(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer));
//	pTarget = std::shared_ptr<Bind::RenderTarget>{ new Bind::OutputOnlyRenderTarget(*this, pBackBuffer.Get()) };
//}
//
//Graphics::~Graphics()
//{
//	
//}
//
//void Graphics::BeginFrame(float red, float green, float blue) const
//{
//	// Clearing shader inputs to prevent simultaneous in/out bind carried over from previous frame
//	ID3D11ShaderResourceView* const pNullTex = nullptr;
//	pContext->PSSetShaderResources(0, 1, &pNullTex); // fullscreen input texture
//	pContext->PSSetShaderResources(3, 1, &pNullTex); // shadow map texture
//}
//
//void Graphics::EndFrame()
//{
//	HRESULT hr;
//
//#ifndef NDEBUG
//	infoManager.Set();
//#endif
//
//	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
//	{
//		if (hr == DXGI_ERROR_DEVICE_REMOVED)
//		{
//			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
//		}
//		else
//		{
//			throw GFX_EXCEPT(hr);
//		}
//	}
//}
//
//void Graphics::DrawIndexed(UINT count) noexcept
//{
//	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
//}
//
//void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
//{
//	projection = proj;
//}
//
//DirectX::XMMATRIX Graphics::GetProjection() const noexcept
//{
//	return projection;
//}
//
//void Graphics::SetCamera(DirectX::FXMMATRIX cmr) noexcept
//{
//	camera = cmr;
//}
//
//DirectX::XMMATRIX Graphics::GetCamera() const noexcept
//{
//	return camera;
//}
//
//UINT Graphics::GetWidth() const noexcept
//{
//	return m_Width;
//}
//
//UINT Graphics::GetHeight() const noexcept
//{
//	return m_Height;
//}
//
//std::shared_ptr<Bind::RenderTarget> Graphics::GetTarget()
//{
//	return pTarget;
//}
//
// Graphics exceptions
//Graphics::HRException::HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMessages) noexcept
//	:
//	Exception(line, file),
//	hr(hr)
//{
//	// Join all messages with new lines in a single string
//	for (const auto& m : infoMessages)
//	{
//		info += m;
//		info.push_back('\n');
//	}
//
//	// Remove final new line if it exists
//	if (!info.empty())
//	{
//		info.pop_back();
//	}
//}
//
//const char* Graphics::HRException::what() const noexcept
//{
//	std::ostringstream oss;
//	oss << GetType() << std::endl
//		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
//		<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
//		<< "Error String: " << GetErrorString() << std::endl
//		<< "Description:  " << GetErrorDescription() << std::endl;
//
//	if (info != "")
//	{
//		oss << "\nError Info: \n" << GetErrorInfo() << std::endl << std::endl;
//	}
//
//	oss << GetErrorSpot();
//	m_WhatBuffer = oss.str();
//
//	return m_WhatBuffer.c_str();
//}
//
//const char* Graphics::HRException::GetType() const noexcept
//{
//	return "Application Graphics Exception";
//}
//
//HRESULT Graphics::HRException::GetErrorCode() const noexcept
//{
//	return hr;
//}
//
//const char* Graphics::HRException::GetErrorString() const noexcept
//{
//	return DXGetErrorString(hr);
//}
//
//const std::string& Graphics::HRException::GetErrorDescription() const noexcept
//{
//	char buffer[512];
//	DXGetErrorDescription(hr, buffer, sizeof(buffer));
//	return buffer;
//}
//
//
//const std::string& Graphics::HRException::GetErrorInfo() const noexcept
//{
//	return info;
//}
//
//const char* Graphics::DeviceRemovedException::GetType() const noexcept
//{
//	return "Application Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
//}
//
//Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMessages) noexcept
//	:
//	Exception(line, file)
//{
//	// Join all messages with new lines in a single string
//	for (const auto& m : infoMessages)
//	{
//		info += m;
//		info.push_back('\n');
//	}
//
//	// Remove final new line if it exists
//	if (!info.empty())
//	{
//		info.pop_back();
//	}
//}
//
//const char* Graphics::InfoException::what() const noexcept
//{
//	std::ostringstream oss;
//	oss << GetType() << std::endl
//		<< "\nError Info: \n" << GetErrorInfo() << std::endl << std::endl;
//
//	oss << GetErrorSpot();
//	m_WhatBuffer = oss.str().c_str();
//
//	return m_WhatBuffer.c_str();
//}
//
//const const char* Graphics::InfoException::GetType() const noexcept
//{
//	return "Application Graphics Info Exception";
//}
//
//const std::string& Graphics::InfoException::GetErrorInfo() const noexcept
//{
//	return info;
//}