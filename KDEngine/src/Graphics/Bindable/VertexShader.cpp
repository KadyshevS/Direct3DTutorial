#include "VertexShader.h"

#include "Graphics/GraphicsThrowMacros.h"

#include <d3dcompiler.h>

namespace KDE
{
	VertexShader::VertexShader(KDGraphics& gfx, const std::wstring& path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
		GFX_THROW_INFO(Device(gfx)->CreateVertexShader(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr, &pVertexShader
		));
	}

	void VertexShader::Bind(KDGraphics& gfx) noexcept
	{
		Context(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}
	ID3DBlob* VertexShader::Bytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}
}
