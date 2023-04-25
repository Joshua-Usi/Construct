#include "../Construct.hpp"

#include "../internal/ProcessMesh.hpp"
#include "../internal/CalculateNormals.hpp"

#include "../internal/IcosphereBase.hpp"
#include "../internal/IcosphereSubdivide.hpp"

namespace Construct
{
	Mesh Icosphere(std::uint32_t subdivisions, const GeneratorSetting& settings)
	{
		// Generate Icosphere base case
		Mesh mesh = internal::IcosphereSubdivide(internal::IcosphereBase(), subdivisions);
		// Calculate normals
		mesh.normals = internal::CalculateNormals(mesh.vertices, mesh.indices);
		// Process mesh for transforms
		internal::ProcessMesh(mesh, settings);
		return mesh;
	}
}