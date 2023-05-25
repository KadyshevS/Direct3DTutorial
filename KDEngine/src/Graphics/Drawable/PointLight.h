#pragma once

#include "Graphics/KDGraphics.h"
#include "Graphics/GeoPrimitives.h"
#include "Graphics/Bindable/ConstantBuffers.h"

class PointLight
{
public:
	PointLight(KDE::KDGraphics& gfx, float radius = 0.5f);
	void SpawnControlWindow();
	void Reset();
	void Draw(KDE::KDGraphics& gfx) const;
	void Bind(KDE::KDGraphics& gfx) const;
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };;
	mutable KDE::KDMesh mesh;
	mutable KDE::PixelConstantBuffer<PointLightCBuf> cbuf;
};