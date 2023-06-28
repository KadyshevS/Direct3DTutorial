#include "Graphics/Bindable/Sampler.h"

#include "Graphics/GraphicsThrowMacros.h"

namespace KDE
{
	Sampler::Sampler(KDE::KDGraphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		GFX_THROW_INFO(Device(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Sampler::Bind(KDGraphics& gfx)
	{
		Context(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}
}