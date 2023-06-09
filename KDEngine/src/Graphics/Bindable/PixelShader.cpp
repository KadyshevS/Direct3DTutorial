#include "Graphics/Bindable/PixelShader.h"

#include "Graphics/GraphicsThrowMacros.h"

#include <d3dcompiler.h>

namespace KDE
{
	PixelShader::PixelShader(KDGraphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
		GFX_THROW_INFO(Device(gfx)->CreatePixelShader(
			pBlob->GetBufferPointer(), 
			pBlob->GetBufferSize(), 
			nullptr, &pPixelShader));
	}

	void PixelShader::Bind(KDGraphics& gfx)
	{
		Context(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
}