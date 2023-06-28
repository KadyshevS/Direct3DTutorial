#include "Graphics/Bindable/Texture.h"

#include "Graphics/KDSurface.h"
#include "Graphics/GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

namespace KDE
{
	Texture::Texture(KDE::KDGraphics& gfx, const KDSurface& surface)
	{
		INFOMAN(gfx);
		
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = surface.GetWidth();
		textureDesc.Height = surface.GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = surface.GetBufferPtr();
		sd.SysMemPitch = surface.GetWidth() * sizeof(KDSurface::Color);
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(Device(gfx)->CreateTexture2D(
			&textureDesc, &sd, &pTexture
		));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(Device(gfx)->CreateShaderResourceView(
			pTexture.Get(), &srvDesc, &pTextureView
		));
	}

	void Texture::Bind(KDGraphics& gfx)
	{
		Context(gfx)->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());
	}
}