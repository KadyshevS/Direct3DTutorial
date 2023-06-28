#pragma once
#include "Bindable.h"

namespace KDE
{
	class KDTexture : public Bindable
	{
	public:
		KDTexture(KDE::KDGraphics& gfx, const class KDSurface& surface);
		KDTexture(KDE::KDGraphics& gfx, const std::string& filePath);
		void Bind(KDGraphics& gfx) override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}