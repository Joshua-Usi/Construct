#include "../Construct.hpp"

#include "../internal/ProcessMesh.hpp"
#include "../internal/CalculateNormals.hpp"

#include <numbers>
#include <cmath>
#include <array>

namespace Construct
{
	Mesh UVSphere(std::uint32_t rings, std::uint32_t segments, const GeneratorSetting& settings)
	{
		// Sphere indice data
		static constexpr std::array<std::uint32_t, 6> SphereIndexMap = {
			2, 1, 0,
			2, 3, 1,
		};
		Mesh mesh;
		// Calculate vertex counts, its similiar to a plane
		const std::uint32_t vertexCount = (rings + 1) * (segments + 1);
		const std::uint32_t indexCount = 2 * 3 * rings * segments;
		// Preallocate
		mesh.vertices.resize(3 * vertexCount, 0.0f);
		mesh.indices.resize(indexCount, 0);
		mesh.textureUVs.resize(2 * vertexCount, 0.0f);
		// Calculate the vertex positions and texture coordinates
		for (std::uint32_t i = 0; i <= rings; i++)
		{
			float latitude = (float)i / (float)rings;
			float theta = latitude * std::numbers::pi_v<float>;
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
				std::uint32_t index = (i * (segments + 1) + j);
				// Add vertices
				mesh.vertices[3 * index + 0] = x;
				mesh.vertices[3 * index + 1] = y;
				mesh.vertices[3 * index + 2] = z;
				// Add texture UVs
				mesh.textureUVs[2 * index + 0] = longitude;
				mesh.textureUVs[2 * index + 1] = latitude;
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
					std::uint32_t index = 6 * (i * segments + j);
					// Use the plane index map to generate the triangles
					for (std::uint32_t k = 0; k < 6; k++)
					{
						mesh.indices[index + k] = corners[SphereIndexMap[k]];
					}
				}
			}
		}
		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
}