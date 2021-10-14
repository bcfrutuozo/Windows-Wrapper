#pragma once

#include "Common.h"
#include "dxerr.h"

// DirectX debug
#include <dxgidebug.h>

#include <vector>

class DXGIInfoManager
{
private:

	unsigned long long m_Next = 0u;
	IDXGIInfoQueue* m_DXGIInfoQueue;

public:

	DXGIInfoManager();
	~DXGIInfoManager() = default;
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

	void Set() noexcept;

	std::vector<const char*> GetMessages() const;
};

