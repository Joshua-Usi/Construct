#include "../Construct.hpp"

#include "../internal/ProcessMesh.hpp"
#include "../internal/CalculateNormals.hpp"

#include <array>

namespace Construct
{
	Mesh Plane(std::uint32_t widthTiles, std::uint32_t heightTiles, const GeneratorSetting& settings)
	{
        // Plane indice data
		static constexpr std::array<std::uint32_t, 6> PlaneIndexMap = {
			0, 1, 2,
			1, 3, 2,
		};
        // Precalculate required vertices
        const uint32_t vertexCount = (widthTiles + 1) * (heightTiles + 1);
        const uint32_t quadCount = widthTiles * heightTiles;
        // Preallocate mesh
        Mesh mesh(3 * vertexCount, 2 * 3 * quadCount, 0, 2 * vertexCount);
        // Create lambda to get indices
        auto index2D = [&](uint32_t i, uint32_t j) { return i * (widthTiles + 1) + j; };
        for (std::uint32_t i = 0; i <= heightTiles; i++)
        {
            for (std::uint32_t j = 0; j <= widthTiles; j++)
            {
                // Calculate vertex position
                float x = static_cast<float>(j) / widthTiles - 0.5f;
                float z = static_cast<float>(i) / heightTiles - 0.5f;
                // Calculate vertex index
                std::uint32_t vertexIndex = 3 * index2D(i, j);
                std::uint32_t textureIndex = 2 * index2D(i, j);
                // Add vertex to data
                mesh.vertices[vertexIndex + 0] = x;
                mesh.vertices[vertexIndex + 1] = z;
                // Add texture to data
                mesh.textureUVs[textureIndex + 0] = x + 0.5f;
                mesh.textureUVs[textureIndex + 1] = 0.5f - z;
                // n + 1 verts but n squares
                if (i < heightTiles && j < widthTiles)
                {
                    // Calculate index of quad corners
                    uint32_t corners[4] = {
                        index2D(i + 0, j + 0),
                        index2D(i + 0, j + 1),
                        index2D(i + 1, j + 0),
                        index2D(i + 1, j + 1)
                    };
                    uint32_t index = 6 * (i * widthTiles + j);
                    // Add quad indices to plane
                    for (std::uint32_t k = 0; k < 6; k++)
                    {
                        mesh.indices[index + k] = corners[PlaneIndexMap[k]];
                    }
                }
            }
        }
        // Calculate mesh normals
        mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
        // Process mesh for transforms
        internal::ProcessMesh(mesh, settings);
        return mesh;
	}
}