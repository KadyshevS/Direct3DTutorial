#include "KDScene.h"

#include "KDEntity.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Components.h"

namespace KDE
{
	KDScene::KDScene(KDGraphics& gfx)
		: m_Graphics(&gfx)
	{
		auto pvs = std::make_unique<KDE::VertexShader>(gfx, L"../KDEngine/src/Shaders/VertexShader.cso");
		auto pvsbc = pvs->Bytecode();
		m_Binds.emplace_back(std::move(pvs));

		m_Binds.emplace_back(std::make_unique<KDE::PixelShader>(gfx, L"../KDEngine/src/Shaders/PixelShader.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,sizeof(float) * 2,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(float) * 5,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};
		
		m_Binds.emplace_back(std::make_unique<KDE::InputLayout>(gfx, ied, pvsbc));

		m_Binds.emplace_back(std::make_unique<KDE::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	KDEntity KDScene::CreateEntity(const std::string& name)
	{
		KDEntity ent = { m_Registry.create(), this };
		ent.AddComponent<CS::TagComponent>();
		ent.AddComponent<CS::RenderComponent>();

		auto& tag = ent.GetComponent<CS::TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return ent;
	}
	void KDScene::DestroyEntity(KDEntity entity)
	{
		m_Registry.destroy(entity.EntityHandle());
	}

	void KDScene::Bind()
	{
		for (auto& b : m_Binds)
		{
			b->Bind(*m_Graphics);
		}
	}
	void KDScene::Draw()
	{
		Bind();

		auto view = m_Registry.view<CS::TagComponent, CS::RenderComponent>();
		for (auto& e : view)
		{
			auto& componenet = view.get<CS::RenderComponent>(e);
			componenet.Bind(*m_Graphics);

			int indCount = (int)componenet.Mesh->Indices().size();
			m_Graphics->DrawIndexed(indCount);
		}
	}
}