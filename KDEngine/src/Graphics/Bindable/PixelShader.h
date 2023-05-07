#pragma once

#include "Bindable.h"

namespace KDE
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(KDGraphics& gfx, const std::wstring& path);
		void Bind(KDGraphics& gfx) override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}