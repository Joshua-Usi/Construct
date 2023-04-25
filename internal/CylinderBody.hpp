#pragma once

#include "Mesh.hpp"

#include <numbers>
#include <cmath>
#include <cstdint>

#include <iostream>
#include <vector>
#include <algorithm>

namespace Construct::internal
{
	inline Mesh CylinderBody(std::uint32_t sides)
	{
		Mesh mesh;
		// Calculate number of vertices for preallocation
		const std::uint32_t vertexCount = 2 * (sides + 1);
		const std::uint32_t triangleCount = 2 * sides + 1;
		// Preallocate
		mesh.vertices = std::vector<float>(3 * vertexCount, 0.0f);
		mesh.indices = std::vector<std::uint32_t>(3 * triangleCount, 0);
		// Can't preallocate normals though
		// Texture UV's U component is between [0.0f, 1.0f + pi], remapped to [0.0f, 1.0f] later
		mesh.textureUVs = std::vector<float>(2 * vertexCount, 0.0f);
		for (std::uint32_t i = 0; i < 2; i++)
		{
			std::uint32_t vertexOffset = i * (sides + 1);
			for (std::uint32_t j = 0; j <= sides; j++)
			{
				const float angle = static_cast<float>(j) / static_cast<float>(sides) * 2.0f * std::numbers::pi_v<float>;
				const float x = std::cosf(angle) * 0.5f;
				const float z = std::sinf(angle) * 0.5f;
				const float textureU = 1.0f + (1.0f - static_cast<float>(j) / sides) * std::numbers::pi_v<float>;
				const std::uint32_t index = (j + vertexOffset);
				// Push side vertices
				mesh.vertices[index * 3 + 0] = x;
				mesh.vertices[index * 3 + 1] = (i == 0) ? 0.5f : -0.5f;
				mesh.vertices[index * 3 + 2] = z;
				// Push side mesh UV
				mesh.textureUVs[index * 2 + 0] = textureU;
				mesh.textureUVs[index * 2 + 1] = (i == 0) ? 0.0f : 1.0f;
				// Push side mesh indices 
				if (j < sides)
				{
					mesh.indices[index * 3 + 0] = index;
					mesh.indices[index * 3 + 1] = j + 1;
					mesh.indices[index * 3 + 2] = sides + j + 1 + i;
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