#include "KDScene.h"

#include "Graphics/Bindable/BindableBase.h"

namespace KDE
{
	KDScene::KDScene(KDGraphics& gfx)
		: m_Graphics(&gfx)
	{
		auto pvs = std::make_unique<KDE::VertexShader>(gfx, L"../KDEngine/src/Shaders/PhongVS.cso");
		auto pvsbc = pvs->Bytecode();
		m_Binds.emplace_back(std::move(pvs));

		m_Binds.emplace_back(std::make_unique<KDE::PixelShader>(gfx, L"../KDEngine/src/Shaders/PhongPS.cso"));

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
		m_Binds.emplace_back(std::make_unique<KDE::PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(float) * 4,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};
		
		m_Binds.emplace_back(std::make_unique<KDE::InputLayout>(gfx, ied, pvsbc));

		m_Binds.emplace_back(std::make_unique<KDE::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	KDEntity KDScene::CreateEntity()
	{
		return KDEntity(m_Registry.create(), this);
	}

	void KDScene::Bind()
	{
		for (auto& b : m_Binds)
		{
			b.Bind(*m_Graphics);
		}
	}
	void KDScene::Update()
	{

	}
}