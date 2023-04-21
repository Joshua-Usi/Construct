#include <vector>
#include <cstdint>
#include <numbers>

#include "internal/internal.h"

// Defines to minimise "magic" numbers
#define VERTEX_ELEMENT_COUNT 3
#define NORMAL_ELEMENT_COUNT 3
#define TEXTURE_ELEMENT_COUNT 2
#define TRIANGLE_INDEX_COUNT 3
#define QUAD_INDEX_COUNT 2 * TRIANGLE_INDEX_COUNT

namespace Construct
{
	/// <summary>
	/// Generates a Quad mesh facing the +z direction
	/// </summary>
	/// <param name="settings">Setting that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Quad</returns>
	inline Mesh Quad(const GeneratorSetting& settings = GeneratorSetting())
	{
		Mesh mesh;
		// Copy into mesh
		mesh.vertices = std::vector<float>(internal::data.QuadVertices);
		mesh.indices = std::vector<std::uint32_t>(internal::data.QuadIndices);
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		mesh.textureUVs = std::vector<float>(internal::data.QuadTextureUVs);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
	/// <summary>
	/// Generates a Plane mesh facing the +y direction
	/// </summary>
	/// <param name="widthTiles">Number of tiles along the width</param>
	/// <param name="heightTiles">Number of tiles along the height</param>
	/// <param name="settings">Setting that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Plane</returns>
	inline Mesh Plane(std::uint32_t widthTiles, std::uint32_t heightTiles, const GeneratorSetting& settings = GeneratorSetting())
	{
		Mesh mesh;
		// Calculate number of vertices for preallocation
		// You need n + 1 vertices to generates n segments
		const uint32_t vertexCount = (widthTiles + 1) * (heightTiles + 1);
		const uint32_t quadCount = widthTiles * heightTiles;
		// Preallocate
		mesh.vertices = std::vector<float>(VERTEX_ELEMENT_COUNT * vertexCount, 0.0f);
		mesh.indices = std::vector<std::uint32_t>(QUAD_INDEX_COUNT * quadCount, 0.0f);
		// Can't preallocate normals though
		mesh.textureUVs = std::vector<float>(TEXTURE_ELEMENT_COUNT * vertexCount, 0.0f);
		
		for (std::uint32_t i = 0; i <= heightTiles; i++)
		{
			for (std::uint32_t j = 0; j <= widthTiles; j++)
			{
				// Calculate position of vertex tile, mapped to [0.0f, 1.0f];
				const float x = static_cast<float>(j) / static_cast<float>(widthTiles);
				const float z = static_cast<float>(i) / static_cast<float>(heightTiles);
				// Calculate required index
				const std::uint32_t index = (i * (widthTiles + 1) + j);
				// Push mesh vertex, y vertex is 0.0f
				mesh.vertices[VERTEX_ELEMENT_COUNT * index + 0] = x - 0.5f;
				mesh.vertices[VERTEX_ELEMENT_COUNT * index + 2] = z - 0.5f;
				// Push mesh UV
				mesh.textureUVs[TEXTURE_ELEMENT_COUNT * index + 0] = x;
				mesh.textureUVs[TEXTURE_ELEMENT_COUNT * index + 1] = z;
				// Calculate indices
				if (i < heightTiles && j < widthTiles)
				{
					// Calculate the indices of the 4 corners of the square
					const std::uint32_t corners[4]
					{
						((i + 0) * (widthTiles + 1)) + j + 0,
						((i + 0) * (widthTiles + 1)) + j + 1,
						((i + 1) * (widthTiles + 1)) + j + 0,
						((i + 1) * (widthTiles + 1)) + j + 1,
					};
					std::uint32_t index = QUAD_INDEX_COUNT * (i * widthTiles + j);
					// Use the index map to generate the triangles
					for (std::uint32_t k = 0; k < QUAD_INDEX_COUNT; k++)
					{
						mesh.indices[index + k] = corners[internal::data.PlaneIndexMap[k]];
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
	/// <summary>
	/// Generates a Polygon mesh facing the +z direction.
	/// High side counts can be used to generate circles
	/// </summary>
	/// <param name="sides">Number of sides for the polygon</param>
	/// <param name="settings">Setting that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Polygon</returns>
	inline Mesh Polygon(std::uint32_t sides, const GeneratorSetting& settings = GeneratorSetting())
	{
		Mesh mesh;
		// Calculate number of vertices for preallocation
		const std::uint32_t vertexCount = (sides + 1);
		// Preallocate
		mesh.vertices = std::vector<float>(VERTEX_ELEMENT_COUNT * vertexCount, 0.0f);
		mesh.indices = std::vector<std::uint32_t>(TRIANGLE_INDEX_COUNT * sides, 0.0f);
		// Can't preallocate normals though
		mesh.textureUVs = std::vector<float>(TEXTURE_ELEMENT_COUNT * vertexCount, 0.0f);

		// Add center vertice data
		// Vertices already initialised to 0
		mesh.textureUVs[0] = 0.5f;
		mesh.textureUVs[1] = 0.5f;

		for (std::uint32_t i = 1; i <= sides; i++)
		{
			const float angle = static_cast<float>(i) / static_cast<float>(sides) * 2.0f * std::numbers::pi_v<float>;
			const float x = std::cosf(angle) * 0.5f;
			const float y = std::sinf(angle) * 0.5f;
			// Push mesh vertex
			mesh.vertices[VERTEX_ELEMENT_COUNT * i + 0] = x;
			mesh.vertices[VERTEX_ELEMENT_COUNT * i + 1] = y;
			// Push mesh UV
			mesh.textureUVs[TEXTURE_ELEMENT_COUNT * i + 0] = x + 0.5f;
			mesh.textureUVs[TEXTURE_ELEMENT_COUNT * i + 1] = -y + 0.5f;
			// First vertex is always center
			mesh.indices[TRIANGLE_INDEX_COUNT * (i - 1) + 1] = i;
			mesh.indices[TRIANGLE_INDEX_COUNT * (i - 1) + 2] = i % sides + 1;
		}

		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
	/// <summary>
	/// Generates a Cube mesh
	/// Uses a 6x1 texture layout with the texture being in the order:
	/// Left, Front, Right, Back, Top, Bottom
	/// (-x, +z, +x, -x, +y, -y)
	/// </summary>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Cube</returns>
	inline Mesh Cube(const GeneratorSetting& settings = GeneratorSetting())
	{
		Mesh mesh;
		mesh.vertices = std::vector<float>(internal::data.CubeVertices);
		mesh.indices = std::vector<std::uint32_t>(internal::data.CubeIndices);
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		mesh.textureUVs = std::vector<float>(internal::data.CubeTextureUVs);
		// Remap U component of texture UVs from [0.0f, 6.0f] to [0.0f, 1.0f]
		for (std::uint32_t i = 0, size = mesh.textureUVs.size(); i < size; i += TEXTURE_ELEMENT_COUNT)
		{
			mesh.textureUVs[i] /= 6.0f;
		}
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
	/// <summary>
	/// Generates a UV Sphere mesh
	/// Uses an equirectangular texture layout
	/// </summary>
	/// <param name="rings">Number of longitude lines</param>
	/// <param name="segments">Number of latitude lines</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a UVSphere</returns>
	inline Mesh UVSphere(std::uint32_t rings, std::uint32_t segments, const GeneratorSetting& settings = GeneratorSetting())
	{
		Mesh mesh;
		// Calculate vertex counts, its similiar to a plane
		const std::uint32_t vertexCount = (rings + 1) * (segments + 1);
		const std::uint32_t indexCount = QUAD_INDEX_COUNT * rings * segments;
		// Preallocate
		mesh.vertices = std::vector<float>(VERTEX_ELEMENT_COUNT * vertexCount, 0.0f);
		mesh.indices = std::vector<std::uint32_t>(indexCount, 0.0f);
		mesh.textureUVs = std::vector<float>(TEXTURE_ELEMENT_COUNT * vertexCount, 0.0f);

		// Calculate the vertex positions and texture coordinates
		for (std::uint32_t i = 0; i <= rings; i++)
		{
			float latitude = (float)i / (float)rings;
			float theta = latitude * std::numbers::pi_v<float>;
			// Cache values that, don't change as often
			float y = std::cosf(theta) * 0.5f;
			float sinTheta = std::sinf(theta);

			for (std::uint32_t j = 0; j <= segments; j++)
			{
				float longitude = (float)j / (float)segments;
				float phi = -longitude * 2.0f * std::numbers::pi_v<float>;

				float x = std::cosf(phi) * sinTheta * 0.5f;
				float z = std::sinf(phi) * sinTheta * 0.5f;

				std::uint32_t index = (i * (segments + 1) + j);

				mesh.vertices[VERTEX_ELEMENT_COUNT * index + 0] = x;
				mesh.vertices[VERTEX_ELEMENT_COUNT * index + 1] = y;
				mesh.vertices[VERTEX_ELEMENT_COUNT * index + 2] = z;

				mesh.textureUVs[TEXTURE_ELEMENT_COUNT * index + 0] = longitude;
				mesh.textureUVs[TEXTURE_ELEMENT_COUNT * index + 1] = latitude;

				if (i < rings && j < segments)
				{
					// Calculate the indices of the 4 corners of the square
					const std::uint32_t corners[4]
					{
						((i + 0) * (segments + 1)) + j + 0,
						((i + 0) * (segments + 1)) + j + 1,
						((i + 1) * (segments + 1)) + j + 0,
						((i + 1) * (segments + 1)) + j + 1,
					};
					std::uint32_t index = QUAD_INDEX_COUNT * (i * segments + j);
					// Use the plane index map to generate the triangles
					for (std::uint32_t k = 0; k < QUAD_INDEX_COUNT; k++)
					{
						mesh.indices[index + k] = corners[internal::data.PlaneIndexMap[k]];
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
	/// <summary>
	/// Generates an Icosphere mesh
	/// TODO Select texture layout for Cylinder
	/// </summary>
	/// <param name="subdivisions">Number of subdivisions, leave 0 for base case (icosahedron)</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for an Icosphere</returns>
	inline Mesh Icosphere(std::uint32_t subdivisions, const GeneratorSetting& settings = GeneratorSetting())
	{
		// Generate Icosphere base case
		Mesh mesh = internal::IcosphereSubdivide(internal::IcosphereBase(), subdivisions);

		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
	/// <summary>
	/// Generates a Cylinder mesh
	/// TODO Select texture layout for Cylinder
	/// </summary>
	/// <param name="sides">Number of sides the cylinder mesh has</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Cylinder</returns>
	inline Mesh Cylinder(std::uint32_t sides, const GeneratorSetting& settings = GeneratorSetting())
	{
		Mesh mesh;
		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
	/// <summary>
	/// Generates a Capsule mesh
	/// TODO Select texture layout for capsule
	/// </summary>
	/// <param name="sides">Number of sides in the capsule mesh, this also affects the number of rings in the hemispheres</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Capsule</returns>
	inline Mesh Capsule(std::uint32_t sides, const GeneratorSetting& settings = GeneratorSetting())
	{
		Mesh mesh;
		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}

	/// <summary>
	/// Generates a Skybox Cube
	/// Uses a 6x1 texture layout with the texture being in the order:
	/// Left, Front, Right, Back, Top, Bottom
	/// (-x, +z, +x, -x, +y, -y)
	/// </summary>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Skybox Cube</returns>
	inline Mesh SkyboxCube(const GeneratorSetting& settings = GeneratorSetting(FaceDirection::CW))
	{
		Mesh mesh = Cube(settings);
		return mesh;
	}
	/// <summary>
	/// Generates a Skybox Sphere
	/// Uses an equirectangular texture layout
	/// </summary>
	/// <param name="rings">Number of longitude lines</param>
	/// <param name="segments">Number of latitude lines</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Skybox Sphere</returns>
	inline Mesh SkyboxSphere(std::uint32_t rings, std::uint32_t segments, const GeneratorSetting& settings = GeneratorSetting(FaceDirection::CW))
	{
		Mesh mesh = UVSphere(rings, segments, settings);
		return mesh;
	}
}
// Undefine so it is not polluting the global namespace
// Also good etiquette
#undef VERTEX_ELEMENT_COUNT
#undef NORMAL_ELEMENT_COUNT
#undef TEXTURE_ELEMENT_COUNT
#undef TRIANGLE_INDEX_COUNT
#undef QUAD_INDEX_COUNT