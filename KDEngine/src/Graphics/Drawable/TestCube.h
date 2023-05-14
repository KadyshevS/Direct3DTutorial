#pragma once
#include "DrawableBase.h"

class TestCube : public KDE::DrawableBase<TestCube>
{
public:
	TestCube(KDE::KDGraphics& gfx);
	void Update(float dt) override;
	DirectX::XMMATRIX Transform() const override;
private:
	float posX = 0.0f, posY = 0.0f, posZ = 2.0f;
	float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
};