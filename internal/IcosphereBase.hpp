#pragma once

#include "Mesh.hpp"

#include <vector>
#include <cstdint>
#include <cmath>
#include <numbers>
#include <array>

namespace Construct::internal
{
	/// <summary>
	/// Generate the mesh for a base case Icosphere
	/// </summary>
	/// <returns>Icosphere subdivision 0 mesh</returns>
	inline Mesh IcosphereBase()
	{
		static constexpr std::array<std::uint32_t, 3 * 20> IcosphereBaseIndices = {
			6, 5, 0,
			7, 6, 1,
			8, 7, 2,
			9, 8, 3,
			10, 9, 4,
			12, 11, 5,
			5, 6, 12,
			13, 12, 6,
			6, 7, 13,
			14, 13, 7,
			7, 8, 14,
			15, 14, 8,
			8, 9, 15,
			16, 15, 9,
			9, 10, 16,
			11, 12, 17,
			12, 13, 18,
			13, 14, 19,
			14, 15, 20,
			15, 16, 21,
		};
		// U component divided by 30.0f
		static constexpr std::array<float, 22 * 2> IcosphereBaseTextureUVs = {
			 3.0f, 0.0f,
			 9.0f, 0.0f,
			15.0f, 0.0f,
			21.0f, 0.0f,
			27.0f, 0.0f,

			 0.0f, 1.0f / 3.0f,
			 6.0f, 1.0f / 3.0f,
			12.0f, 1.0f / 3.0f,
			18.0f, 1.0f / 3.0f,
			24.0f, 1.0f / 3.0f,
			30.0f, 1.0f / 3.0f,

			-3.0f, 2.0f / 3.0f,
			 3.0f, 2.0f / 3.0f,
			 9.0f, 2.0f / 3.0f,
			15.0f, 2.0f / 3.0f,
			21.0f, 2.0f / 3.0f,
			27.0f, 2.0f / 3.0f,

			 6.0f, 1.0f,
			12.0f, 1.0f,
			18.0f, 1.0f,
			24.0f, 1.0f,
			30.0f, 1.0f,
		};

		Mesh mesh;
		// 22 points, 5 * 2 for the poles (+10), 6 * 2 for the rings
		mesh.vertices = std::vector<float>(22 * 3, 0.0f);

		// Cache common calculations
		const float va = std::atanf(0.5f);
		const float xy = std::cosf(va) * 0.5f;
		const float z = std::sinf(va) * 0.5f;
		for (std::uint32_t i = 0; i <= 5; i++)
		{
			// Calculate angle of rings
			const float angle = static_cast<float>(i) / 5.0f * 2.0f * std::numbers::pi_v<float>;
			const float angle2 = angle - 0.2f * std::numbers::pi_v<float>;
			// Precalculate indices
			const std::uint32_t topRingIndex = (5 + i) * 3;
			const std::uint32_t bottomRingIndex = (11 + i) * 3;
			// Top ring
			mesh.vertices[topRingIndex + 0] = xy * std::cosf(angle);
			mesh.vertices[topRingIndex + 1] = z;
			mesh.vertices[topRingIndex + 2] = xy * std::sinf(angle);
			// Bottom ring
			mesh.vertices[bottomRingIndex + 0] = xy * std::cosf(angle2);
			mesh.vertices[bottomRingIndex + 1] = -z;
			mesh.vertices[bottomRingIndex + 2] = xy * std::sinf(angle2);

			// Generate pole vertices
			if (i < 5)
			{
				// X and Z are already 0 initialised
				// North Pole
				mesh.vertices[i * 3 + 1] = 0.5f;
				// South Pole
				mesh.vertices[(i + 17) * 3 + 1] = -0.5f;
			}
		}
		// Add indices
		mesh.indices = std::vector<std::uint32_t>(IcosphereBaseIndices.begin(), IcosphereBaseIndices.end());
		mesh.textureUVs = std::vector<float>(IcosphereBaseTextureUVs.begin(), IcosphereBaseTextureUVs.end());
		// Remap U component by dividing by 30.0f
		for (std::uint32_t i = 0, size = mesh.textureUVs.size(); i < size; i += 2)
		{
			mesh.textureUVs[i] /= 30.0f;
		}
		return mesh;
	}
}