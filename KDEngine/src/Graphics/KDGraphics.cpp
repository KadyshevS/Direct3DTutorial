#include "KDGraphics.h"

#include "DXErr/dxerr.h"

#include <sstream>
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GFX_EXCEPT_NOINFO(hr) KDGraphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw KDGraphics::HrException( __LINE__,__FILE__,hr )

#ifdef _DEBUG
#define GFX_EXCEPT(hr) KDGraphics::HrException( __LINE__,__FILE__,(hr),m_InfoManager.Messages() )
#define GFX_THROW_INFO(hrcall) m_InfoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_THROW_INFO_ONLY(call) m_InfoManager.Set(); (call); {auto v = m_InfoManager.Messages(); if(!v.empty()) {throw KDGraphics::InfoException(__LINE__, __FILE__, v);}} 
#define GFX_EXCEPT_DEVICE_REMOVED(hr) KDGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr),m_InfoManager.Messages() )
#else
#define GFX_EXCEPT(hr) KDGraphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_THROW_INFO_ONLY(call) (call)
#define GFX_EXCEPT_DEVICE_REMOVED(hr) KDGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

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

		GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, gfxCreateFlags, nullptr, 0, D3D11_SDK_VERSION,
			&scd, &m_SwapChain, &m_Device, nullptr, &m_Context
		) );

		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_INFO( m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer) );
		GFX_THROW_INFO( m_Device->CreateRenderTargetView(
			pBackBuffer.Get(), nullptr, &m_Target
		) );
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
	}
	void KDGraphics::DrawTestTriangle()
	{
		namespace wrl = Microsoft::WRL;
		HRESULT hr;

		struct Vertex
		{
			float x;
			float y;
		};

		const Vertex vertices[3] = {
			{ 0.0f, 0.5f },
			{ 0.5f,-0.5f },
			{-0.5f,-0.5f }
		};

		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bd{};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.ByteWidth = sizeof(vertices);
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = vertices;
		GFX_THROW_INFO( m_Device->CreateBuffer(&bd, &sd, &pVertexBuffer) );
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0;
		m_Context->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		wrl::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO( D3DReadFileToBlob(L"../KDEngine/src/Shaders/PixelShader.cso", &pBlob) );
		GFX_THROW_INFO( m_Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader) );

		m_Context->PSSetShader(pPixelShader.Get(), nullptr, 0);

		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		GFX_THROW_INFO( D3DReadFileToBlob(L"../KDEngine/src/Shaders/VertexShader.cso", &pBlob) );
		GFX_THROW_INFO( m_Device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader) );
		
		m_Context->VSSetShader(pVertexShader.Get(), nullptr, 0);

		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		
		GFX_THROW_INFO(m_Device->CreateInputLayout(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		));

		m_Context->IASetInputLayout(pInputLayout.Get());

		m_Context->OMSetRenderTargets(1, m_Target.GetAddressOf(), nullptr);

		m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_VIEWPORT vp{};
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_Context->RSSetViewports(1, &vp);

		GFX_THROW_INFO_ONLY( m_Context->Draw((UINT)std::size(vertices), 0));
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