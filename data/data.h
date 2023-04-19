#pragma once

#include <vector>

namespace Construct::internal
{
	struct
	{
		const std::vector<float> QuadVertices = {
			-0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
		};
		const std::vector<uint32_t> QuadIndices = {
			0, 1, 2,
			3, 1, 0,
		};
		const std::vector<float> QuadTextureUVs = {
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
		};
		const std::vector<uint32_t> PlaneIndexMap = {
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
		// U component will be divided by 6
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
	} data;
}