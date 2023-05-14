#include "ChiliTest.h"

#include <Graphics/Bindable/BindableBase.h>

ChiliTest::ChiliTest(
	KDE::KDGraphics& gfx, const KDE::KDMesh& mesh, std::mt19937& rng, 
	std::uniform_real_distribution<float>& adist, 
	std::uniform_real_distribution<float>& ddist, 
	std::uniform_real_distribution<float>& odist, 
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if (!IsStaticInitialized())
	{
		auto pvs = std::make_unique<KDE::VertexShader>(gfx, L"../KDEngine/src/Shaders/VertexShader.cso");
		auto pvsbc = pvs->Bytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<KDE::PixelShader>(gfx, L"../KDEngine/src/Shaders/PixelShader.cso"));

		struct ConstantBuffer2
		{
			struct
			{
				float r, g, b, a;
			} face_colors[6];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f,0.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,1.0f,1.0f },
			}
		};
		AddStaticBind(std::make_unique<KDE::PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<KDE::InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<KDE::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	
	AddBind(std::make_unique<KDE::VertexBuffer>(gfx, mesh.Vertices()));
	AddIndexBuffer(std::make_unique<KDE::IndexBuffer>(gfx, mesh.Indices()));
	AddBind(std::make_unique<KDE::TransformCBuffer>(gfx, *this));
}

void ChiliTest::Update(float dt)
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}
DirectX::XMMATRIX ChiliTest::Transform() const
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
