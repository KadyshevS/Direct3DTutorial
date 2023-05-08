#include "TestCube.h"

#include "Graphics/Bindable/BindableBase.h"
#include "Graphics/GraphicsThrowMacros.h"

namespace dx = DirectX;

TestCube::TestCube(KDE::KDGraphics& gfx)
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			struct
			{
				float x, y, z;
			} pos;
		};
		const std::vector<Vertex> vertices =
		{
			{ -0.5f, -0.5f, -0.5f },
			{  0.5f, -0.5f, -0.5f },
			{  0.5f,  0.5f, -0.5f },
			{ -0.5f,  0.5f, -0.5f },
			{ -0.5f, -0.5f,  0.5f },
			{  0.5f, -0.5f,  0.5f },
			{  0.5f,  0.5f,  0.5f },
			{ -0.5f,  0.5f,  0.5f },
		};
		AddStaticBind(std::make_unique<KDE::VertexBuffer>(gfx, vertices));

		auto pvs = std::make_unique<KDE::VertexShader>(gfx, L"../KDEngine/src/Shaders/VertexShader.cso");
		auto pvsbc = pvs->Bytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<KDE::PixelShader>(gfx, L"../KDEngine/src/Shaders/PixelShader.cso"));

		const std::vector<uint32_t> indices =
		{
			0, 3, 1,	1, 3, 2,
			5, 7, 4,	5, 6, 7,
			4, 7, 3,	4, 3, 0,
			2, 6, 5,	2, 5, 1,
			3, 7, 6,	3, 6, 2,
			0, 5, 4,	0, 1, 5
		};
		AddStaticIndexBuffer(std::make_unique<KDE::IndexBuffer>(gfx, indices));

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
				{ 1.0f, 0.0f, 1.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f, 1.0f },
				{ 0.0f, 1.0f, 0.0f, 1.0f },
				{ 0.0f, 0.0f, 1.0f, 1.0f },
				{ 1.0f, 1.0f, 0.0f, 1.0f },
				{ 0.0f, 1.0f, 1.0f, 1.0f },
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

	AddBind(std::make_unique<KDE::TransformCBuffer>(gfx, *this));
}

void TestCube::Update(float dt)
{
	rotX += dt;
	rotZ += dt;
}
dx::XMMATRIX TestCube::Transform() const
{
	return dx::XMMatrixRotationX(rotX) *
		dx::XMMatrixRotationZ(rotZ) *
		dx::XMMatrixTranslation(posX, posY, posZ);
}
