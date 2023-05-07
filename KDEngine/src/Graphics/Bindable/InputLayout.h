#pragma once

#include "Bindable.h"

namespace KDE
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(KDGraphics& gfx, 
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
			ID3DBlob* pVertexShaderBytecode);

		virtual void Bind(KDGraphics& gfx) override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
}