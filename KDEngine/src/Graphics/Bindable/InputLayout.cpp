#include "Graphics/Bindable/InputLayout.h"

#include "Graphics/GraphicsThrowMacros.h"

namespace KDE
{
	InputLayout::InputLayout(KDGraphics& gfx, 
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
		ID3DBlob* pVertexShaderBytecode)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(Device(gfx)->CreateInputLayout(
			layout.data(), (UINT)layout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&pInputLayout
		));
	}

	void InputLayout::Bind(KDGraphics& gfx)
	{
		Context(gfx)->IASetInputLayout(pInputLayout.Get());
	}
}