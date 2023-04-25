#include "../Construct.hpp"

#include "../internal/ProcessMesh.hpp"
#include "../internal/CalculateNormals.hpp"

#include "../internal/CylinderBody.hpp"
#include "../utils/Merge.hpp"

namespace Construct
{
	Mesh Cylinder(std::uint32_t sides, const GeneratorSetting& settings)
	{
		// Get parts of mesh
		Mesh body = internal::CylinderBody(sides);
		Mesh topFace = Polygon(sides, {
			WindingOrder::CCW,
			vec3(0.0f, 0.5f, 0.0f),
			vec3(1.0f, 1.0f, 1.0f),
			quat(-0.707f, 0.0f, 0.0f, 0.707f) });
		Mesh bottomFace = Polygon(sides, {
			WindingOrder::CW,
			vec3(0.0f, -0.5f, 0.0f),
			vec3(1.0f, 1.0f, 1.0f),
			quat(-0.707f, 0.0f, 0.0f, 0.707f) });
		// Remap face texture UVs
		for (std::uint32_t i = 0, size = topFace.textureUVs.size(); i < size; i += 2)
		{
			topFace.textureUVs[i + 0] *= 1.0f / (1.0f + std::numbers::pi_v<float>);
			topFace.textureUVs[i + 1] *= 0.5f;

			bottomFace.textureUVs[i + 0] *= 1.0f / (1.0f + std::numbers::pi_v<float>);
			bottomFace.textureUVs[i + 1] *= 0.5f;
			bottomFace.textureUVs[i + 1] += 0.5f;
		}
		// Merge meshes together
		Mesh mesh = Merge({ &bottomFace, &topFace, &body });
		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
}