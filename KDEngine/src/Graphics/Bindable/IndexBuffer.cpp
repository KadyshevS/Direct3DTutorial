#include "IndexBuffer.h"

#include "Graphics/GraphicsThrowMacros.h"

namespace KDE
{
	IndexBuffer::IndexBuffer(KDGraphics& gfx, const std::vector<uint32_t>& indices)
		: count((UINT)indices.size())
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT(count * sizeof(uint32_t));
		ibd.StructureByteStride = sizeof(uint32_t);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		GFX_THROW_INFO(Device(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	}

	void IndexBuffer::Bind(KDGraphics& gfx)
	{
		Context(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}
	UINT IndexBuffer::Count() const
	{
		return count;
	}
}