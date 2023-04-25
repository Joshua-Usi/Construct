#include "../Construct.hpp"

namespace Construct
{
	Mesh SkyboxCube(const GeneratorSetting& settings)
	{
		Mesh mesh = Cube(settings);
		return mesh;
	}
}