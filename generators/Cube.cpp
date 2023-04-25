#include "../Construct.hpp"

#include "../internal/ProcessMesh.hpp"
#include "../internal/CalculateNormals.hpp"

#include <array>

namespace Construct
{
	Mesh Cube(const GeneratorSetting& settings)
	{
		// Cube data using 6x1 texture strip, 24 vertices for normals and textures
		static constexpr std::array<float, 24 * 3> CubeVertices = {
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
		static constexpr std::array<uint32_t, 6 * 6> CubeIndices = {
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
		static constexpr std::array<float, 2 * 24> CubeTextureUVs = {
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
		// Create mesh
		Mesh mesh;
		// Move data into mesh
		mesh.vertices = std::vector<float>(CubeVertices.begin(), CubeVertices.end());
		mesh.indices = std::vector<std::uint32_t>(CubeIndices.begin(), CubeIndices.end());
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		mesh.textureUVs = std::vector<float>(CubeTextureUVs.begin(), CubeTextureUVs.end());
		// Remap U component of texture UVs from [0.0f, 6.0f] to [0.0f, 1.0f]
		for (std::uint32_t i = 0, size = mesh.textureUVs.size(); i < size; i += 2)
		{
			mesh.textureUVs[i] /= 6.0f;
		}
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
}