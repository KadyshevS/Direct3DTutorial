#include "KDGraphics.h"

#include "DXErr/dxerr.h"

#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_THROW_FAILED(hrcall) if( FAILED(hr = (hrcall)) ) throw KDGraphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_EXCEPT_DEVICE_REMOVED(hr) KDGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )

namespace KDE
{
//////////////////////////////////////////////////////////////////////////
////	Graphics
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

		UINT gfxCreateFlags = 0;

#ifdef _DEBUG
		gfxCreateFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr;

		GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, gfxCreateFlags, nullptr, 0, D3D11_SDK_VERSION,
			&scd, &m_SwapChain, &m_Device, nullptr, &m_Context
		) );

		ID3D11Resource* pBackBuffer = nullptr;
		GFX_THROW_FAILED( m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)) );
		GFX_THROW_FAILED( m_Device->CreateRenderTargetView(
			pBackBuffer, nullptr, &m_Target
		) );
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
		HRESULT hr;
		if ( FAILED(hr = m_SwapChain->Present(1u, 0u)) )
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
				throw GFX_EXCEPT_DEVICE_REMOVED(m_Device->GetDeviceRemovedReason());
			else
				GFX_THROW_FAILED(hr);
		}
	}
	void KDGraphics::ClearBuffer(float red, float green, float blue)
	{
		float rgba[4] = {red, green, blue, 1.0f};
		m_Context->ClearRenderTargetView(m_Target, rgba);
	}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
////	Graphics Exception
	KDGraphics::HrException::HrException(int line, const char* file, HRESULT hr)
		: Exception(line, file), hr(hr) {}

	const char* KDGraphics::HrException::what() const
	{
		std::ostringstream oss;
		oss << Type() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << ErrorCode()
			<< std::dec << " (" << (unsigned long)ErrorCode() << ")" << std::endl
			<< "[Error String] " << ErrorString() << std::endl
			<< "[Description] " << ErrorDescription() << std::endl
			<< StringOrigin();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* KDGraphics::HrException::Type() const
	{
		return "KDGraphics Exception";
	}
	HRESULT KDGraphics::HrException::ErrorCode() const
	{
		return hr;
	}
	std::string KDGraphics::HrException::ErrorString() const
	{
		return DXGetErrorString(hr);
	}
	std::string KDGraphics::HrException::ErrorDescription() const
	{
		char buf[512];
		DXGetErrorDescription(hr, buf, sizeof(buf));
		return buf;
	}

	const char* KDGraphics::DeviceRemovedException::Type() const noexcept
	{
		return "KDGraphics Exception [DEVICE REMOVED]";
	}
//////////////////////////////////////////////////////////////////////////
}