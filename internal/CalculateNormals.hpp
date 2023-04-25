#pragma once

#include <vector>
#include <cstdint>
#include <cmath>

namespace Construct::internal
{
	inline const std::vector<float> CalculateNormals(const std::vector<float>& vertices, const std::vector<std::uint32_t>& indices)
	{
		std::vector<float> normals(vertices.size(), 0.0f);
		for (std::uint32_t i = 0; i < indices.size(); i += 3) {
			// Get index of vertices in triangle
			const std::uint32_t i1 = indices[i + 0] * 3,
				i2 = indices[i + 1] * 3,
				i3 = indices[i + 2] * 3;
			// Vertex 1
			const float x1 = vertices[i1 + 0],
				y1 = vertices[i1 + 1],
				z1 = vertices[i1 + 2];
			// Vertex 2
			const float x2 = vertices[i2 + 0],
				y2 = vertices[i2 + 1],
				z2 = vertices[i2 + 2];
			// Vertex 3
			const float x3 = vertices[i3 + 0],
				y3 = vertices[i3 + 1],
				z3 = vertices[i3 + 2];
			const float ux = x2 - x1,
				uy = y2 - y1,
				uz = z2 - z1;
			const float vx = x3 - x1,
				vy = y3 - y1,
				vz = z3 - z1;
			// Calculate cross dots
			float nx = (uy * vz) - (uz * vy),
				ny = (uz * vx) - (ux * vz),
				nz = (ux * vy) - (uy * vx);
			// Normalise to 1
			const float len = std::sqrtf(nx * nx + ny * ny + nz * nz);
			if (len != 0.0f)
			{
				nx /= len;
				ny /= len;
				nz /= len;
			}
			// Append to array
			// Vertex 1 normals
			normals[i1 + 0] = nx;
			normals[i1 + 1] = ny;
			normals[i1 + 2] = nz;
			// Vertex 2 normals
			normals[i2 + 0] = nx;
			normals[i2 + 1] = ny;
			normals[i2 + 2] = nz;
			// Vertex 3 normals
			normals[i3 + 0] = nx;
			normals[i3 + 1] = ny;
			normals[i3 + 2] = nz;
		}
		return normals;
	}
}