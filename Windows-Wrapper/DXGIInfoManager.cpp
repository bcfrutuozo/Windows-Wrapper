#include "DXGIInfoManager.h"
#include "Window.h"
#include "Graphics.h"

#pragma warning(push)
#include <dxgidebug.h>
#pragma warning(pop)

#include <memory>

DXGIInfoManager::DXGIInfoManager()
	:
	m_Next(0u)
{
	// Define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// Load the DLL that contains the function DXGIGetDebugInterface
	const auto hModDXGIDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDXGIDebug == nullptr)
	{
		throw CTL_LAST_EXCEPT();
	}

	// Get the address of DXGIGetDebugInterface in DLL
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDXGIDebug, "DXGIGetDebugInterface")));

	if (DxgiGetDebugInterface == nullptr)
	{
		throw CTL_LAST_EXCEPT();
	}

	//GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_DXGIInfoQueue));
}

void DXGIInfoManager::Set() noexcept
{
	// Set the index (next) so that the next all to GetMessages()
	// will only get errors generated after this call
	m_Next = m_DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<const char*> DXGIInfoManager::GetMessages() const
{
	std::vector<const char*> messages;
	const auto end = m_DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = m_Next; i < end; i++)
	{
		SIZE_T messageLength = 0;

		// Ge the size of the message in bytes
		//GFX_THROW_NOINFO(m_DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		// Allocate memory for the message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		// Get the message and push its description into the vector
		//GFX_THROW_NOINFO(m_DXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}
