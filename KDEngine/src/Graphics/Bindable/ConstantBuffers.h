#pragma once
#include "Bindable.h"
#include "Graphics/GraphicsThrowMacros.h"

namespace KDE
{
	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer(KDGraphics& gfx, const C& consts)
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(consts);
			cbd.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA csd = {};
			csd.pSysMem = &consts;
			GFX_THROW_INFO(Device(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
		}
		ConstantBuffer(KDGraphics& gfx)
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC cbd;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.ByteWidth = sizeof(C);
			cbd.StructureByteStride = 0u;
			GFX_THROW_INFO(Device(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
		}

		void Update(KDGraphics& gfx, const C& consts)
		{
			INFOMAN(gfx);

			D3D11_MAPPED_SUBRESOURCE msr;
			GFX_THROW_INFO(Context(gfx)->Map(
				pConstantBuffer.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u,
				&msr
			));
			memcpy(msr.pData, &consts, sizeof(consts));
			Context(gfx)->Unmap(pConstantBuffer.Get(), 0u);
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using Bindable::Context;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(KDGraphics& gfx) noexcept override
		{
			Context(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using Bindable::Context;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(KDGraphics& gfx) noexcept override
		{
			Context(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
		}
	};
}
