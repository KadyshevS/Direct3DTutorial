#include "KDGraphics.h"

#include "DXErr/dxerr.h"
#include "GraphicsThrowMacros.h"

#include <sstream>
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

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

		D3D11_DEPTH_STENCIL_DESC dsc{};
		dsc.DepthEnable = TRUE;
		dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsc.DepthFunc = D3D11_COMPARISON_LESS;
		wrl::ComPtr<ID3D11DepthStencilState> pDSState;
		GFX_THROW_INFO( m_Device->CreateDepthStencilState(&dsc, &pDSState) );

		m_Context->OMSetDepthStencilState(pDSState.Get(), 1);

		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth{};
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.Width = 800;
		descDepth.Height = 600;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		GFX_THROW_INFO( m_Device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil) );

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(m_Device->CreateDepthStencilView(
			pDepthStencil.Get(), &descDSV, &m_DepthStencilView
		) );
		m_Context->OMSetRenderTargets(1, m_Target.GetAddressOf(), m_DepthStencilView.Get());

		D3D11_VIEWPORT vp{};
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_Context->RSSetViewports(1, &vp);
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
	void KDGraphics::DrawTestTriangle(float x, float z, float val)
	{
		namespace wrl = Microsoft::WRL;
		HRESULT hr;

		struct Vertex
		{
			dx::XMVECTOR pos;
		};

		const Vertex vertices[] = {
			{ -0.5f, -0.5f, -0.5f },
			{  0.5f, -0.5f, -0.5f },
			{  0.5f,  0.5f, -0.5f },
			{ -0.5f,  0.5f, -0.5f },
			{ -0.5f, -0.5f,  0.5f },
			{  0.5f, -0.5f,  0.5f },
			{  0.5f,  0.5f,  0.5f },
			{ -0.5f,  0.5f,  0.5f },
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

		const uint16_t indices[] = {
			0, 3, 1,	1, 3, 2,
			5, 7, 4,	5, 6, 7,
			4, 7, 3,	4, 3, 0,
			2, 6, 5,	2, 5, 1,
			3, 7, 6,	3, 6, 2,
			0, 5, 4,	0, 1, 5
		};

		wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
		D3D11_BUFFER_DESC ibd{};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.ByteWidth = sizeof(indices);
		ibd.StructureByteStride = sizeof(uint16_t);
		D3D11_SUBRESOURCE_DATA isd{};
		isd.pSysMem = indices;
		GFX_THROW_INFO(m_Device->CreateBuffer(&ibd, &isd, &pIndexBuffer));
		const UINT indexOffset = 0;
		m_Context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, indexOffset);

		struct ConstantBuffer
		{
			dx::XMMATRIX transform;
		};

		const ConstantBuffer cb = {
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(val) *
				dx::XMMatrixRotationY(val) *
				dx::XMMatrixTranslation(x, 0.0f, z + 2.0f) *
				dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
			)
		};

		wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
		D3D11_BUFFER_DESC cbd{};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.ByteWidth = sizeof(cb);
		cbd.StructureByteStride = sizeof(ConstantBuffer);
		D3D11_SUBRESOURCE_DATA csd{};
		csd.pSysMem = &cb;
		GFX_THROW_INFO(m_Device->CreateBuffer(&cbd, &csd, &pConstantBuffer));
		m_Context->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

		struct ConstantBuffer2
		{
			dx::XMVECTOR face_colors[6];
		};

		const ConstantBuffer2 cb2 = {
			{
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 0.0f },
				{ 0.0f, 1.0f, 1.0f },
				{ 1.0f, 0.0f, 1.0f }
			}
		};

		wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
		D3D11_BUFFER_DESC cbd2{};
		cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd2.Usage = D3D11_USAGE_DYNAMIC;
		cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd2.MiscFlags = 0;
		cbd2.ByteWidth = sizeof(cb2);
		cbd2.StructureByteStride = sizeof(ConstantBuffer);
		D3D11_SUBRESOURCE_DATA csd2{};
		csd2.pSysMem = &cb2;
		GFX_THROW_INFO(m_Device->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));
		m_Context->PSSetConstantBuffers(0, 1, pConstantBuffer2.GetAddressOf());
		
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
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		
		GFX_THROW_INFO(m_Device->CreateInputLayout(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		));

		m_Context->IASetInputLayout(pInputLayout.Get());

		m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DrawIndexed((UINT)std::size(indices));
	}
	void KDGraphics::DrawIndexed(UINT count)
	{
		GFX_THROW_INFO_ONLY(m_Context->DrawIndexed(count, 0, 0));
	}
	void KDGraphics::SetProjection(DirectX::FXMMATRIX proj)
	{
		m_Projection = proj;
	}
	DirectX::XMMATRIX KDGraphics::Projection() const
	{
		return m_Projection;
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