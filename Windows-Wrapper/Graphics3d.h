#pragma once

#include "Common.h"
#include "Exception.h"
#include "DxgiInfoManager.h"

#include <d3d11.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

// HRESULT hr should exist in the local scope for these macros to work
#define GFX_EXCEPT_NOINFO(hr) Graphics::HRException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HRException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HRException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HRException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

// Macro for importing infoManager into local scope
#ifdef NDEBUG
#define INFOMAN(graphics) HRESULT hr
#else
#define INFOMAN(graphics) HRESULT hr; DXGIInfoManager& infoManager = GetInfoManager((graphics))
#endif

#ifdef NDEBUG
#define INFOMAN_NOHR(graphics)
#else
#define INFOMAN_NOHR(graphics) DXGIInfoManager& infoManager = GetInfoManager((graphics))
#endif

class DepthStencil;

namespace Bind
{
	class Bindable;
	class RenderTarget;
}

class Graphics3d
{
	friend class Bind::Bindable;

public:
	
	//class GraphicsException : public Exception
	//{
	//	
	//};
	//
	//class HRException : public Exception
	//{
	//
	//public:
	//	
	//	HRException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMessages = {}) noexcept;
	//	
	//	const char* what() const noexcept override;
	//	const char* GetType() const noexcept override;
	//	HRESULT GetErrorCode() const noexcept;
	//	const char* GetErrorString() const noexcept;
	//	const std::string& GetErrorDescription() const noexcept;
	//	const std::string& GetErrorInfo() const noexcept;
	//
	//private:
	//	
	//	HRESULT hr;
	//	std::string info;
	//};
	//
	//// Info Exception
	//class InfoException : public Exception
	//{
	//
	//public:
	//	
	//	InfoException(int line, const char* file, std::vector<std::string> infoMessages) noexcept;
	//	
	//	const char* what() const noexcept override;
	//	const char* GetType() const noexcept override;
	//	const std::string& GetErrorInfo() const noexcept;
	//
	//private:
	//	std::string info;
	//};

	//// Device Removed Exception
	//class DeviceRemovedException : public HRException
	//{
	//	using HRException::HRException;
	//public:
	//	const char* GetType() const noexcept override;
	//private:
	//	std::string reason;
	//};

public:

	//Graphics(HWND handle, int width, int height);
	//Graphics(const Graphics&) = delete;
	//Graphics& operator=(const Graphics&) = delete;
	//~Graphics();
	//void BeginFrame(float red, float green, float blue) const;
	//void EndFrame();
	//void DrawIndexed(UINT count) noexcept;
	//void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	//DirectX::XMMATRIX GetProjection() const noexcept;
	//void SetCamera(DirectX::FXMMATRIX cmr) noexcept;
	//DirectX::XMMATRIX GetCamera() const noexcept;
	//UINT GetWidth() const noexcept;
	//UINT GetHeight() const noexcept;
	//std::shared_ptr<Bind::RenderTarget> GetTarget();

private:

	UINT m_Width;
	UINT m_Height;
	bool isImGuiEnabled;
	DirectX::XMMATRIX camera;
	DirectX::XMMATRIX projection;

#ifndef NDEBUG
	DXGIInfoManager infoManager;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	std::shared_ptr<Bind::RenderTarget> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
};