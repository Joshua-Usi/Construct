#pragma once

#include "Mesh.hpp"

#include <array>
#include <cstdint>
#include <numbers>
#include <cmath>

namespace Construct::internal
{
	inline Mesh CapsuleHead(std::uint32_t sides)
	{
		// Sphere indice data
		static constexpr std::array<std::uint32_t, 6> SphereIndexMap = {
			2, 1, 0,
			2, 3, 1,
		};
		const uint32_t rings = sides / 2;
		const uint32_t segments = sides;
		Mesh mesh;
		// Calculate vertex counts, its similiar to a plane
		const std::uint32_t vertexCount = 2 * (rings + 1) * (segments + 1);
		const std::uint32_t indexCount = 2 * 2 * rings * segments;
		// Preallocate
		mesh.vertices.resize(3 * vertexCount, 0.0f);
		mesh.indices.resize(3 * indexCount, 0);
		mesh.textureUVs.resize(2 * vertexCount, 0.0f);
		// Calculate the vertex positions and texture coordinates
		for (std::uint32_t l = 0; l < 2; l++)
		{
			float side = ((l == 0) ? 1.0f : -1.0f);
			std::uint32_t startingIndex = l * (segments + 1) * (rings + 1);
			std::uint32_t indexStartingIndex = l * segments * rings;
			for (std::uint32_t i = 0; i <= rings; i++)
			{
				float latitude = (float)i / (float)rings;
				float theta = latitude * 0.5f * std::numbers::pi_v<float>;
				// Cache Y value, it doesn't change as often
				float y = std::cosf(theta) * 0.5;
				float sinTheta = std::sinf(theta);

				for (std::uint32_t j = 0; j <= segments; j++)
				{
					float longitude = (float)j / (float)segments;
					float phi = -longitude * 2.0f * std::numbers::pi_v<float>;
					// Calculate positions
					float x = std::cosf(phi) * sinTheta * 0.5f;
					float z = std::sinf(phi) * sinTheta * 0.5f;
					// Calculate vertex index
					std::uint32_t index = startingIndex + (i * (segments + 1) + j);
					// Add vertices
					mesh.vertices[3 * index + 0] = x;
					mesh.vertices[3 * index + 1] = (y + 0.5f) * side;
					mesh.vertices[3 * index + 2] = z;
					// Add texture UVs
					mesh.textureUVs[2 * index + 0] = 0.5f + x;
					mesh.textureUVs[2 * index + 1] = 0.25f + ((l == 0) ? 0.0f : 0.5f) + z * 0.5f;
					// n + 1 verts but n squares
					if (i < rings && j < segments)
					{
						// Calculate index of quad corners
						const std::uint32_t corners[4]
						{
							((i + 0) * (segments + 1)) + j + 0,
							((i + 0) * (segments + 1)) + j + 1,
							((i + 1) * (segments + 1)) + j + 0,
							((i + 1) * (segments + 1)) + j + 1,
						};
						std::uint32_t index = 6 * (indexStartingIndex + i * segments + j);
						// Use the plane index map to generate the triangles
						for (std::uint32_t k = 0; k < 6; k++)
						{
							std::uint32_t indexOffset = ((l == 0) ? k : 6 - k - 1);
							mesh.indices[index + indexOffset] = startingIndex + corners[SphereIndexMap[k]];
						}
					}
				}
			}
		}
		// Remap x coordinate
		for (std::uint32_t i = 0, size = mesh.textureUVs.size(); i < size; i += 2)
		{
			mesh.textureUVs[i] /= 1.0f + std::numbers::pi_v<float>;
		}
		return mesh;
	}
}