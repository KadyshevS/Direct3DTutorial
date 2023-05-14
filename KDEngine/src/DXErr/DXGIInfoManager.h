#pragma once

#include "WinBase/KDWin.h"

#include <vector>
#include <string>
#include <dxgidebug.h>

namespace KDE
{
	class DXGIInfoManager
	{
	public:
		DXGIInfoManager();
		~DXGIInfoManager();
		DXGIInfoManager(const DXGIInfoManager&) = delete;
		DXGIInfoManager& operator = (const DXGIInfoManager&) = delete;

		void Set() noexcept;
		std::vector<std::string> Messages() const;
	private:
		unsigned long long next = 0u;
		IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
	};
}
