#pragma once

#include "Mesh.hpp"
#include "types.hpp"

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
		// Process vertices for rotation, scale and offset
		// If offset or scale is left default, then ignore
		if (
			settings.offset != vec3(0.0f, 0.0f, 0.0f) ||
			settings.scale != vec3(1.0f, 1.0f, 1.0f) ||
			settings.rotation != quat(0.0f, 0.0f, 0.0f, 1.0f)
			)
		{
			for (std::size_t i = 0, size = mesh.vertices.size(); i < size; i += 3)
			{
				vec3 meshVertice = vec3(mesh.vertices[i + 0], mesh.vertices[i + 1], mesh.vertices[i + 2]);
				meshVertice = meshVertice * settings.scale;
				meshVertice = settings.rotation * meshVertice;
				meshVertice = meshVertice + settings.offset;
				mesh.vertices[i + 0] = meshVertice.x;
				mesh.vertices[i + 1] = meshVertice.y;
				mesh.vertices[i + 2] = meshVertice.z;
			}
		}
		// Process for face direction
		if (settings.windingOrder == WindingOrder::CW)
		{
			// Flip indices
			for (std::size_t i = 0, size = mesh.indices.size(); i < size; i += 3)
			{
				std::uint32_t temp = mesh.indices[i + 0];
				mesh.indices[i + 0] = mesh.indices[i + 2];
				// Ignore middle index
				mesh.indices[i + 2] = temp;
			}
			// Flip normals
			for (std::size_t i = 0, size = mesh.normals.size(); i < size; i += 3)
			{
				mesh.normals[i + 0] *= -1.0f;
				mesh.normals[i + 1] *= -1.0f;
				mesh.normals[i + 2] *= -1.0f;
			}
		}
	}
}