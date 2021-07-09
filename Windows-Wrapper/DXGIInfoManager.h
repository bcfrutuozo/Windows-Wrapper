#pragma once

#include "Common.h"
#include "dxerr.h"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

#include <vector>
#include <dxgidebug.h>
#include <string>

class DXGIInfoManager
{
private:

	unsigned long long m_Next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_DXGIInfoQueue;

public:

	DXGIInfoManager();
	~DXGIInfoManager() = default;
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

	void Set() noexcept;

	std::vector<const char*> GetMessages() const;
};

