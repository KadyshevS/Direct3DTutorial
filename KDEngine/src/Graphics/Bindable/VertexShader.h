#pragma once

#include "Bindable.h"

namespace KDE
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(KDGraphics& gfx, const std::wstring& path);
		void Bind(KDGraphics& gfx) noexcept override;
		ID3DBlob* Bytecode() const noexcept;

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}