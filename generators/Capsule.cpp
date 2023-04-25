#include "../Construct.hpp"

#include "../internal/ProcessMesh.hpp"
#include "../internal/CalculateNormals.hpp"

#include "../internal/CapsuleHead.hpp"
#include "../internal/CylinderBody.hpp"
#include "../utils/Merge.hpp"

namespace Construct
{
	Mesh Capsule(std::uint32_t sides, const GeneratorSetting& settings)
	{
		// Get parts of mesh
		Mesh capsuleHead = internal::CapsuleHead(sides);
		Mesh cylinderBody = internal::CylinderBody(sides);
		// Merge meshes together
		Mesh mesh = Merge({ &capsuleHead, &cylinderBody });
		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
}