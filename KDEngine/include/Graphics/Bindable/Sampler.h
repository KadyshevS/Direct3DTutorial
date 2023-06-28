#pragma once
#include "Bindable.h"

namespace KDE
{
	class Sampler : public Bindable
	{
	public:
		Sampler(KDE::KDGraphics& gfx);
		void Bind(KDGraphics& gfx) override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}