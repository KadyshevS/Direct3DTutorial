#pragma once

#include "DrawableBase.h"
#include "Graphics/KDMesh.h"

#include <random>

class ChiliTest : public KDE::DrawableBase<ChiliTest>
{
public:
	ChiliTest(KDE::KDGraphics& gfx, const KDE::KDMesh& mesh, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) override;
	DirectX::XMMATRIX Transform() const override;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};