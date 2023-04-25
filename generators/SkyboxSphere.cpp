#include "../Construct.hpp"

namespace Construct
{
	Mesh SkyboxSphere(std::uint32_t rings, std::uint32_t segments, const GeneratorSetting& settings)
	{
		Mesh mesh = UVSphere(rings, segments, settings);
		return mesh;
	}
}