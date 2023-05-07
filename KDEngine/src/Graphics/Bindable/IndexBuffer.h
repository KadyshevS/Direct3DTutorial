#pragma once

#include "Bindable.h"

namespace KDE
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(KDGraphics& gfx, const std::vector<uint32_t>& indices);

		void Bind(KDGraphics& gfx) override;
		UINT Count() const;
	protected:
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}