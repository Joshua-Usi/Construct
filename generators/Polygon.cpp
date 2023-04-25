#include "../Construct.hpp"

#include "../internal/ProcessMesh.hpp"
#include "../internal/CalculateNormals.hpp"

#include <numbers>

namespace Construct
{
	Mesh Polygon(std::uint32_t sides, const GeneratorSetting& settings)
	{
		// Calculate number of vertices for preallocation
		const std::uint32_t vertexCount = (sides + 1);
		// Preallocate
		Mesh mesh(3 * vertexCount, 3 * sides, 0, 2 * vertexCount);
		// Add center vertice data
		// Vertices already initialised to 0
		mesh.textureUVs[0] = 0.5f;
		mesh.textureUVs[1] = 0.5f;
		for (std::uint32_t i = 1; i <= sides; i++)
		{
			const float angle = static_cast<float>(i) / sides * 2.0f * std::numbers::pi_v<float>;
			const float x = std::cosf(angle) * 0.5f;
			const float y = std::sinf(angle) * 0.5f;
			// Push mesh vertex
			mesh.vertices[3 * i + 0] = x;
			mesh.vertices[3 * i + 1] = y;
			// Push mesh UV
			mesh.textureUVs[2 * i + 0] = x + 0.5f;
			mesh.textureUVs[2 * i + 1] = -y + 0.5f;
			// First vertex is always center
			mesh.indices[3 * (i - 1) + 1] = i;
			mesh.indices[3 * (i - 1) + 2] = i % sides + 1;
		}
		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
}