#include "KDScene.h"

#include "KDEntity.h"
#include "Graphics/KDGraphics.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Components.h"

namespace KDE
{
	KDScene::KDScene(KDGraphics& gfx)
	{
		auto pvs = std::make_unique<VertexShader>(gfx, L"../KDEngine/src/Shaders/VertexShader.cso");
		auto pvsbc = pvs->Bytecode();
		binds.push_back(std::move(pvs));

		binds.push_back(std::make_unique<KDE::PixelShader>(gfx, L"../KDEngine/src/Shaders/PixelShader.cso"));

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
		binds.push_back(std::make_unique<KDE::PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		binds.push_back(std::make_unique<KDE::InputLayout>(gfx, ied, pvsbc));

		binds.push_back(std::make_unique<KDE::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		for (auto& b : binds)
			b->Bind(gfx);
	}
	KDScene::~KDScene()
	{
	}

	KDEntity KDScene::CreateEntity(const std::string& name)
	{
		auto entity = KDEntity{ m_Registry.create(), this };
		entity.AddComponent<TagComponent>();
		entity.AddComponent<TransformComponent>();

		entity.m_DebugName = name;

		return entity;
	}
	void KDScene::OnUpdate(KDGraphics& gfx, float dt)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			const auto& [transform, mesh] = m_Registry.get<TransformComponent, MeshComponent>(entity);
			transform.Bind(gfx);
			mesh.Bind(gfx);

			gfx.DrawIndexed((UINT)mesh.Mesh->Indices().size());
		}
	}
}