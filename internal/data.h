#pragma once

#include <vector>

namespace Construct::internal
{
	const inline struct
	{
		const std::vector<float> QuadVertices = {
			-0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
		};
		const std::vector<std::uint32_t> QuadIndices = {
			0, 1, 2,
			3, 1, 0,
		};
		const std::vector<float> QuadTextureUVs = {
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
		};
		const std::vector<std::uint32_t> PlaneIndexMap = {
			2, 1, 0,
			2, 3, 1,
		};
		// Cube data using 6x1 texture strip, 24 vertices for normals and textures
		const std::vector<float> CubeVertices = {
			// Left
			-0.5f, -0.5f, -0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			// Front
			-0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			// Right
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f, -0.5f, -0.5f,
			// Back
			 0.5f, -0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			// Top
			-0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			// Bottom
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
		};
		const std::vector<uint32_t> CubeIndices = {
			// Left
			0, 1, 2,
			3, 1, 0,
			// Front
			4, 5, 6,
			7, 5, 4,
			// Right
			8, 9, 10,
			11, 9, 8,
			// Back
			12, 13, 14,
			15, 13, 12,
			// Top
			16, 17, 18,
			19, 17, 16,
			// Bottom
			20, 21, 22,
			23, 21, 20,
		};
		// U component will be divided by 6 to map to range [0.0f, 1.0f]
		const std::vector<float> CubeTextureUVs = {
			// Left
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			// Front
			1.0f, 1.0f,
			2.0f, 0.0f,
			1.0f, 0.0f,
			2.0f, 1.0f,
			// Right
			2.0f, 1.0f,
			3.0f, 0.0f,
			2.0f, 0.0f,
			3.0f, 1.0f,
			// Back
			3.0f, 1.0f,
			4.0f, 0.0f,
			3.0f, 0.0f,
			4.0f, 1.0f,
			// Top
			4.0f, 1.0f,
			5.0f, 0.0f,
			4.0f, 0.0f,
			5.0f, 1.0f,
			// Bottom
			6.0f, 0.0f,
			5.0f, 1.0f,
			6.0f, 1.0f,
			5.0f, 0.0f,
		};
		const std::vector<std::uint32_t> IcosphereBaseIndices = {
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
		const std::vector<float> IcosphereBaseTextureUVs = {
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
		// takes a 2, 1, 0 triangle, subdivides it serpinski style and remaps the indices to this array
		const std::vector<std::uint32_t> IcosphereSubdivisionRemapIndices = {
			0, 1, 2,
			1, 3, 4,
			2, 4, 5,
			2, 1, 4,
		};
	} data;
}