#pragma once

#include <vector>
#include <cstdint>

namespace Construct
{
	/// <summary>
	/// Define a mesh of data that is usually compatible with most renderer
	/// </summary>
	struct Mesh
	{
		/// <summary>
		/// float list of 3 tuple vertices (X, Y, Z)
		/// </summary>
		std::vector<float> vertices;
		/// <summary>
		/// 32-bit int list of 3 tuple points of a triangle (p1, p2, p3).
		/// With a default winding order of counter-clockwise.
		/// </summary>
		std::vector<std::uint32_t> indices;
		/// <summary>
		/// float list of 3 tuple normals (NX, NY, NZ)
		/// Normalised to lengths of 1.0f or 0.0f if normal is a zero-vector
		/// </summary>
		std::vector<float> normals;
		/// <summary>
		/// float list of 2 tuple texture coordinates
		/// With 0.0f, 0.0f denoting bottom-left corner
		/// </summary>
		std::vector<float> textureUVs;
	};
}