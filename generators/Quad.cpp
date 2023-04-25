#include "../Construct.hpp"

namespace Construct
{
	Mesh Quad(const GeneratorSetting& settings)
	{
		// Create mesh
		Mesh mesh = Plane(1, 1, settings);
		return mesh;
	}
}