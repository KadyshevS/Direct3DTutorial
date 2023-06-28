#pragma once
#include "Bindable.h"

namespace KDE
{
	class Texture : public Bindable
	{
	public:
		Texture(KDE::KDGraphics& gfx, const class KDSurface& surface);
		void Bind(KDGraphics& gfx) override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}