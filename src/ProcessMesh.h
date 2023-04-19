#pragma once

#include "Mesh.h"
#include "GeneratorSettings.h"

namespace Construct::internal
{
	// Thanks to user79758 on stack overflow
	template <typename T>
	std::int8_t sign(T val)
	{
		return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
	}
	inline void ProcessMesh(Mesh& mesh, const GeneratorSetting& settings)
	{
		// Preprocess vertices for scale and offset
		// If offset or scale is left default, then ignore
		if (
			settings.offset != vec3_t<float>(0.0f, 0.0f, 0.0f) ||
			settings.scale  != vec3_t<float>(1.0f, 1.0f, 1.0f) ||
			settings.flipVertexY
		)
		{
			const float yMulti = ((settings.flipVertexY) ? -1.0f : 1.0f);
			for (std::uint64_t i = 0, size = mesh.vertices.size(); i < size; i += 3)
			{
				mesh.vertices[i + 0] = mesh.vertices[i + 0] * settings.scale.x + settings.offset.x;
				mesh.vertices[i + 1] = mesh.vertices[i + 1] * settings.scale.y * yMulti + settings.offset.y;
				mesh.vertices[i + 2] = mesh.vertices[i + 2] * settings.scale.x + settings.offset.z;
			}
		}
	}
}