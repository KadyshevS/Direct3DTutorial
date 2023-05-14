#pragma once

#include "Graphics/KDMesh.h"
#include "Graphics/KDMath.h"
#include "KDGraphics.h"

#include <vector>
#include <array>

namespace KDE::GP
{
	class Cube
	{
	public:
		static KDMesh Make()
		{
			namespace dx = DirectX;

			constexpr float side = 3.0f / 4.0f;

			std::vector<dx::XMFLOAT3> vertices;
			vertices.emplace_back(-side, -side, -side);
			vertices.emplace_back( side, -side, -side); 
			vertices.emplace_back(-side,  side, -side); 
			vertices.emplace_back( side,  side, -side); 
			vertices.emplace_back(-side, -side,  side); 
			vertices.emplace_back( side, -side,  side);
			vertices.emplace_back(-side,  side,  side);
			vertices.emplace_back( side,  side,  side); 

			std::vector<Vertex> verts(vertices.size());
			for (size_t i = 0; i < vertices.size(); i++)
			{
				verts[i].pos = vertices[i];
			}
			return{
				std::move(verts),{
					0,2,1, 2,3,1,
					1,3,5, 3,7,5,
					2,6,3, 3,6,7,
					4,5,7, 4,7,6,
					0,4,2, 2,4,6,
					0,1,4, 1,5,4
				}
			};
		}
	};
	class Cone
	{
	public:
		static KDMesh MakeTesselated(int longDiv)
		{
			namespace dx = DirectX;
			assert(longDiv >= 3);

			const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
			const float longitudeAngle = 2.0f * Math::PI / longDiv;

			
			std::vector<Vertex> vertices;
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
			
			vertices.emplace_back();
			vertices.back().pos = { 0.0f,0.0f,-1.0f };
			const auto iCenter = (uint32_t)(vertices.size() - 1);
			
			vertices.emplace_back();
			vertices.back().pos = { 0.0f,0.0f,1.0f };
			const auto iTip = (uint32_t)(vertices.size() - 1);

			std::vector<uint32_t> indices;
			for (uint32_t iLong = 0; iLong < (uint32_t)longDiv; iLong++)
			{
				indices.push_back(iCenter);
				indices.push_back((iLong + 1) % longDiv);
				indices.push_back(iLong);
			}

			for (uint32_t iLong = 0; iLong < (uint32_t)longDiv; iLong++)
			{
				indices.push_back(iLong);
				indices.push_back((iLong + 1) % longDiv);
				indices.push_back(iTip);
			}

			return { std::move(vertices), std::move(indices) };
		}
		static KDMesh Make()
		{
			return MakeTesselated(24);
		}
	};
	class Plane
	{
	public:
		static KDMesh MakeTesselated(int divisions_x, int divisions_y)
		{
			namespace dx = DirectX;
			assert(divisions_x >= 1);
			assert(divisions_y >= 1);

			constexpr float width = 2.0f;
			constexpr float height = 2.0f;
			const int nVertices_x = divisions_x + 1;
			const int nVertices_y = divisions_y + 1;
			std::vector<Vertex> vertices(nVertices_x * nVertices_y);

			{
				const float side_x = width / 2.0f;
				const float side_y = height / 2.0f;
				const float divisionSize_x = width / float(divisions_x);
				const float divisionSize_y = height / float(divisions_y);
				const auto bottomLeft = dx::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

				for (int y = 0, i = 0; y < nVertices_y; y++)
				{
					const float y_pos = float(y) * divisionSize_y;
					for (int x = 0; x < nVertices_x; x++, i++)
					{
						const auto v = dx::XMVectorAdd(
							bottomLeft,
							dx::XMVectorSet(float(x) * divisionSize_x, y_pos, 0.0f, 0.0f)
						);
						dx::XMStoreFloat3(&vertices[i].pos, v);
					}
				}
			}

			std::vector<uint32_t> indices;
			indices.reserve(Math::Sq(divisions_x * divisions_y) * 6);
			{
				const auto vxy2i = [nVertices_x](size_t x, size_t y)
				{
					return (uint32_t)(y * nVertices_x + x);
				};
				for (size_t y = 0; y < divisions_y; y++)
				{
					for (size_t x = 0; x < divisions_x; x++)
					{
						const std::array<uint32_t, 4> indexArray = {
							vxy2i(x,y), vxy2i(x + 1,y),
							vxy2i(x,y + 1), vxy2i(x + 1,y + 1) };

						indices.push_back(indexArray[0]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[3]);
					}
				}
			}

			return{ std::move(vertices),std::move(indices) };
		}
		static KDMesh MakeTextured()
		{
			namespace dx = DirectX;

			constexpr float width = 2.0f;
			constexpr float height = 2.0f;
			const int nVertices_x = 1 + 1;
			const int nVertices_y = 1 + 1;
			std::vector<Vertex> vertices(nVertices_x * nVertices_y);

			{
				const float side_x = width / 2.0f;
				const float side_y = height / 2.0f;
				const float divisionSize_x = width / float(1);
				const float divisionSize_y = height / float(1);
				const auto bottomLeft = dx::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

				for (int y = 0, i = 0; y < nVertices_y; y++)
				{
					const float y_pos = float(y) * divisionSize_y;
					for (int x = 0; x < nVertices_x; x++, i++)
					{
						const auto v = dx::XMVectorAdd(
							bottomLeft,
							dx::XMVectorSet(float(x) * divisionSize_x, y_pos, 0.0f, 0.0f)
						);
						dx::XMStoreFloat3(&vertices[i].pos, v);
					}
				}
			}

			vertices[0].tex = {0.0f, 0.0f};
			vertices[2].tex = {1.0f, 0.0f};
			vertices[1].tex = {0.0f, 1.0f};
			vertices[3].tex = {1.0f, 1.0f};

			std::vector<uint32_t> indices;
			indices.reserve(Math::Sq(1 * 1) * 6);
			{
				const auto vxy2i = [nVertices_x](size_t x, size_t y)
				{
					return (uint32_t)(y * nVertices_x + x);
				};
				for (size_t y = 0; y < 1; y++)
				{
					for (size_t x = 0; x < 1; x++)
					{
						const std::array<uint32_t, 4> indexArray = {
							vxy2i(x,y), vxy2i(x + 1,y),
							vxy2i(x,y + 1), vxy2i(x + 1,y + 1) };

						indices.push_back(indexArray[0]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[3]);
					}
				}
			}

			return{ std::move(vertices),std::move(indices) };
		}

		static KDMesh Make()
		{
			return MakeTesselated(1, 1);
		}
	};
	class Prism
	{
	public:
		static KDMesh MakeTesselated(int longDiv)
		{
			namespace dx = DirectX;
			assert(longDiv >= 3);

			const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
			const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
			const float longitudeAngle = 2.0f * Math::PI / longDiv;

			
			std::vector<Vertex> vertices;
			vertices.emplace_back();
			vertices.back().pos = { 0.0f,0.0f,-1.0f };
			const auto iCenterNear = (uint32_t)(vertices.size() - 1);
			
			vertices.emplace_back();
			vertices.back().pos = { 0.0f,0.0f,1.0f };
			const auto iCenterFar = (uint32_t)(vertices.size() - 1);

			
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				{
					vertices.emplace_back();
					auto v = dx::XMVector3Transform(
						base,
						dx::XMMatrixRotationZ(longitudeAngle * iLong)
					);
					dx::XMStoreFloat3(&vertices.back().pos, v);
				}
				{
					vertices.emplace_back();
					auto v = dx::XMVector3Transform(
						base,
						dx::XMMatrixRotationZ(longitudeAngle * iLong)
					);
					v = dx::XMVectorAdd(v, offset);
					dx::XMStoreFloat3(&vertices.back().pos, v);
				}
			}

			std::vector<uint32_t> indices;
			for (uint32_t iLong = 0; iLong < (uint32_t)longDiv; iLong++)
			{
				const auto i = iLong * 2;
				const auto mod = longDiv * 2;
				indices.push_back(i + 2);
				indices.push_back((i + 2) % mod + 2);
				indices.push_back(i + 1 + 2);
				indices.push_back((i + 2) % mod + 2);
				indices.push_back((i + 3) % mod + 2);
				indices.push_back(i + 1 + 2);
			}

			for (uint32_t iLong = 0; iLong < (uint32_t)longDiv; iLong++)
			{
				const auto i = iLong * 2;
				const auto mod = longDiv * 2;
				indices.push_back(i + 2);
				indices.push_back(iCenterNear);
				indices.push_back((i + 2) % mod + 2);
				indices.push_back(iCenterFar);
				indices.push_back(i + 1 + 2);
				indices.push_back((i + 3) % mod + 2);
			}

			return { std::move(vertices),std::move(indices) };
		}
		static KDMesh Make()
		{
			return MakeTesselated(24);
		}
	};
	class Sphere
	{
	public:
		static KDMesh MakeTesselated(int latDiv, int longDiv)
		{
			namespace dx = DirectX;
			assert(latDiv >= 3);
			assert(longDiv >= 3);

			constexpr float radius = 1.0f;
			const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
			const float lattitudeAngle = Math::PI / latDiv;
			const float longitudeAngle = 2.0f * Math::PI / longDiv;

			std::vector<Vertex> vertices;
			for (int iLat = 1; iLat < latDiv; iLat++)
			{
				const auto latBase = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationX(lattitudeAngle * iLat)
				);
				for (int iLong = 0; iLong < longDiv; iLong++)
				{
					vertices.emplace_back();
					auto v = dx::XMVector3Transform(
						latBase,
						dx::XMMatrixRotationZ(longitudeAngle * iLong)
					);
					dx::XMStoreFloat3(&vertices.back().pos, v);
				}
			}

			const auto iNorthPole = (uint32_t)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3(&vertices.back().pos, base);
			const auto iSouthPole = (uint32_t)vertices.size();
			vertices.emplace_back();
			dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

			const auto calcIdx = [latDiv, longDiv](uint32_t iLat, uint32_t iLong)
			{ return iLat * longDiv + iLong; };
			std::vector<uint32_t> indices;
			for (uint32_t iLat = 0; iLat < uint32_t(latDiv - 2); iLat++)
			{
				for (uint32_t iLong = 0; iLong < (uint32_t)longDiv - 1; iLong++)
				{
					indices.push_back(calcIdx(iLat, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong + 1));
				}
				indices.push_back(calcIdx(iLat, (uint32_t)longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, 0));
			}

			for (uint32_t iLong = 0; iLong < (uint32_t)longDiv - 1; iLong++)
			{
				indices.push_back(iNorthPole);
				indices.push_back(calcIdx(0, iLong));
				indices.push_back(calcIdx(0, iLong + 1));
	
				indices.push_back(calcIdx(latDiv - 2, iLong + 1));
				indices.push_back(calcIdx(latDiv - 2, iLong));
				indices.push_back(iSouthPole);
			}
			
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, longDiv - 1));
			indices.push_back(calcIdx(0, 0));
			
			indices.push_back(calcIdx(latDiv - 2, 0));
			indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
			indices.push_back(iSouthPole);

			return { std::move(vertices),std::move(indices) };
		}
		static KDMesh Make()
		{
			return MakeTesselated(12, 24);
		}
	};
}