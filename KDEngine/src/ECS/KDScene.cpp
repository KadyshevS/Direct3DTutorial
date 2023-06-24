#include "KDScene.h"

#include "Graphics/Bindable/BindableBase.h"
#include "Graphics/GeoPrimitives.h"
#include "Components.h"
#include "KDEntity.h"

namespace KDE
{
	KDScene::KDScene(KDGraphics& gfx)
		: m_Graphics(&gfx)
	{
		auto pvs = std::make_unique<KDE::VertexShader>(gfx, L"assets/shaders/PhongVS.cso");
		auto pvsbc = pvs->Bytecode();
		m_Binds.emplace_back(std::move(pvs));

		m_Binds.emplace_back(std::make_unique<KDE::PixelShader>(gfx, L"assets/shaders/PhongPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
		};
		
		m_Binds.emplace_back(std::make_unique<KDE::InputLayout>(gfx, ied, pvsbc));

		m_Binds.emplace_back(std::make_unique<KDE::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		m_Binds.emplace_back(std::make_unique<KDE::Sampler>(gfx));

	//	Point Light binds
		pvs = std::make_unique<KDE::VertexShader>(gfx, L"assets/shaders/SolidVS.cso");
		pvsbc = pvs->Bytecode();
		m_LightBinds.emplace_back(std::move(pvs));
		m_LightBinds.emplace_back(std::make_unique<KDE::PixelShader>(gfx, L"assets/shaders/SolidPS.cso"));

	//	Texture binds
		pvs = std::make_unique<KDE::VertexShader>(gfx, L"assets/shaders/TextureVS.cso");
		pvsbc = pvs->Bytecode();
		m_TextureBinds.emplace_back(std::move(pvs));
		m_TextureBinds.emplace_back(std::make_unique<KDE::PixelShader>(gfx, L"assets/shaders/TexturePS.cso"));
	}

	KDEntity KDScene::CreateEntity(const std::string& name)
	{
		KDEntity ent = { m_Registry.create(), this };

		ent.AddComponent<CS::TagComponent>();
		ent.AddComponent<CS::RenderComponent>();

		auto& tag = ent.GetComponent<CS::TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		std::shared_ptr<KDMesh> mesh = std::make_shared<KDMesh>( GP::Cube::MakeIndependent() );
		ent.GetComponent<CS::RenderComponent>().Mesh = mesh;

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

	//	Light sources
		auto plview = m_Registry.view<CS::PointLightComponent, CS::RenderComponent>(); 
		for (auto& e : plview)
		{
			if (m_Registry.all_of<CS::PointLightComponent>(e))
			{
				for (auto& b : m_LightBinds)
				{
					b->Bind(*m_Graphics);
				}

				auto& plComp = plview.get<CS::PointLightComponent>(e);
				auto& rComp = plview.get<CS::RenderComponent>(e);
				plComp.Bind(*m_Graphics, reinterpret_cast<DirectX::XMFLOAT3&>(rComp.Mesh->Transform.Position));
			}
		}

	//	Render objects
		auto view = m_Registry.view<CS::TagComponent, CS::RenderComponent>();
		for (auto& e : view)
		{
			auto& componenet = view.get<CS::RenderComponent>(e);
			componenet.Bind(*m_Graphics);

			if (componenet.Texture)
			{
				for (auto& b : m_TextureBinds)
				{
					b->Bind(*m_Graphics);
				}
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					m_Binds[i]->Bind(*m_Graphics);
				}
			}

			int indCount = (int)componenet.Mesh->Indices.size();
			m_Graphics->DrawIndexed(indCount);
		}
	}
}