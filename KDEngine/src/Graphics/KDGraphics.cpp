#include "Graphics/KDGraphics.h"

#include "DXErr/dxerr.h"
#include "Graphics/GraphicsThrowMacros.h"

#include <sstream>
#include <d3dcompiler.h>
#include <imgui/imgui.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace KDE
{
//////////////////////////////////////////////////////////////////////////
////	Graphics
	KDGraphics::KDGraphics(HWND hWnd, uint32_t width, uint32_t height)
		: m_WindowHandle(hWnd)
	{
		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // Изменено на правильный формат
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

		GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, gfxCreateFlags, nullptr, 0, D3D11_SDK_VERSION,
			&scd, &m_SwapChain, &m_Device, nullptr, &m_Context
		));

		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW_INFO(m_Device->CreateRenderTargetView(
			pBackBuffer.Get(), nullptr, &m_Target
		));

		D3D11_DEPTH_STENCIL_DESC dsc{};
		dsc.DepthEnable = TRUE;
		dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsc.DepthFunc = D3D11_COMPARISON_LESS;
		wrl::ComPtr<ID3D11DepthStencilState> pDSState;
		GFX_THROW_INFO(m_Device->CreateDepthStencilState(&dsc, &pDSState));

		m_Context->OMSetDepthStencilState(pDSState.Get(), 1);

		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth{};
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		GFX_THROW_INFO(m_Device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(m_Device->CreateDepthStencilView(
			pDepthStencil.Get(), &descDSV, &m_DepthStencilView
		));

		m_Context->OMSetRenderTargets(1, m_Target.GetAddressOf(), m_DepthStencilView.Get());

		D3D11_VIEWPORT vp{};
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_Context->RSSetViewports(1, &vp);

		Camera.Projection.Width = static_cast<float>(width);
		Camera.Projection.Height = static_cast<float>(height);
	}

	void KDGraphics::EndFrame()
	{
		HRESULT hr;
#ifdef _DEBUG
		m_InfoManager.Set();
#endif
		
		if ( FAILED(hr = m_SwapChain->Present(1u, 0u)) )
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
				throw GFX_EXCEPT_DEVICE_REMOVED(m_Device->GetDeviceRemovedReason());
			else
				throw GFX_EXCEPT(hr);
		}
	}
	void KDGraphics::ClearBuffer(float red, float green, float blue)
	{
		float rgba[4] = {red, green, blue, 1.0f};
		m_Context->ClearRenderTargetView(m_Target.Get(), rgba);
		m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	void KDGraphics::DrawIndexed(UINT count)
	{
		GFX_THROW_INFO_ONLY(m_Context->DrawIndexed(count, 0, 0));
	}

	void KDGraphics::OnResize(uint32_t width, uint32_t height)
	{
		HRESULT hr;

		m_Context->OMSetRenderTargets(0, nullptr, nullptr);
		m_Target.Reset();
		m_DepthStencilView.Reset();

		DXGI_SWAP_CHAIN_DESC scd{};
		m_SwapChain->GetDesc(&scd);

		GFX_THROW_INFO(m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW_INFO(m_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_Target));

		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth{};
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		GFX_THROW_INFO(m_Device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(m_Device->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_DepthStencilView));

		m_Context->OMSetRenderTargets(1, m_Target.GetAddressOf(), m_DepthStencilView.Get());

		D3D11_VIEWPORT vp{};
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_Context->RSSetViewports(1, &vp);

		Camera.Projection.Width = float(width);
		Camera.Projection.Height = float(height);
	}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
////	Graphics Exception
	KDGraphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs)
		: Exception(line, file), hr(hr) 
	{
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}

		if (!info.empty())
			info.pop_back();
	}

	const char* KDGraphics::HrException::what() const
	{
		std::ostringstream oss;
		oss << Type() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << ErrorCode()
			<< std::dec << " (" << (unsigned long)ErrorCode() << ")" << std::endl
			<< "[Error String] " << ErrorString() << std::endl
			<< "[Description] " << ErrorDescription() << std::endl;
		if (!info.empty())
		{
			oss << "\n[Error Info]\n" << ErrorInfo() << std::endl << std::endl;
		}
		oss << StringOrigin();

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
	std::string KDGraphics::HrException::ErrorInfo() const
	{
		return info;
	}

	KDGraphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs)
		: Exception(line, file)
	{
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}

		if (!info.empty())
			info.pop_back();
	}
	const char* KDGraphics::InfoException::what() const
	{
		std::ostringstream oss;
		oss << Type() << std::endl;
		if (!info.empty())
		{
			oss << "\n[Error Info]\n" << ErrorInfo() << std::endl << std::endl;
		}
		oss << StringOrigin();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* KDGraphics::InfoException::Type() const
	{
		return "KDGraphics Info Exception";
	}
	std::string KDGraphics::InfoException::ErrorInfo() const
	{
		return info;
	}

	const char* KDGraphics::DeviceRemovedException::Type() const
	{
		return "KDGraphics Exception [DEVICE REMOVED]";
	}
	
	//////////////////////////////////////////////////////////////////////////
}