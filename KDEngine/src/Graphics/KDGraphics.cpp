#include "KDGraphics.h"

#pragma comment(lib, "d3d11.lib")

namespace KDE
{
	KDGraphics::KDGraphics(HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hWnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
			&scd, &m_SwapChain, &m_Device, nullptr, &m_Context
		);

		ID3D11Resource* pBackBuffer = nullptr;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
		m_Device->CreateRenderTargetView(
			pBackBuffer, nullptr, &m_Target
		);
		pBackBuffer->Release();
	}
	KDGraphics::~KDGraphics()
	{
		if (!m_Target)
			m_Target->Release();
		if (!m_Context)
			m_Context->Release();
		if (!m_SwapChain)
			m_SwapChain->Release();
		if (!m_Device)
			m_Device->Release();
	}

	void KDGraphics::EndFrame()
	{
		m_SwapChain->Present(1, 0);
	}
	void KDGraphics::ClearBuffer(float red, float green, float blue)
	{
		float rgba[4] = {red, green, blue, 1.0f};
		m_Context->ClearRenderTargetView(m_Target, rgba);
	}
}