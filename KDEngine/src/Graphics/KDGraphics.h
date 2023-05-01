#pragma once

#include "WinBase/KDWin.h"

#include <d3d11.h>

namespace KDE
{
	class KDGraphics
	{
	public:
		KDGraphics(HWND hWnd);
		~KDGraphics();

		void EndFrame();
		void ClearBuffer(float red, float green, float blue);
	private:
		ID3D11Device* m_Device = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11DeviceContext* m_Context = nullptr;
		ID3D11RenderTargetView* m_Target = nullptr;
	};
}