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
		// Default constructor
		inline Mesh() = default;
		/// <summary>
		/// Initialise arrays with lengths and initialise to 0
		/// </summary>
		inline Mesh(std::uint32_t vertexCount, std::uint32_t indexCount, std::uint32_t normalCount, std::uint32_t textureUVs)
		{
			this->vertices.resize(vertexCount, 0.0f);
			this->indices.resize(indexCount, 0);
			this->normals.resize(normalCount, 0.0f);
			this->textureUVs.resize(textureUVs, 0.0f);
		}
		/// <summary>
		/// Initialise with another mesh
		/// </summary>
		inline Mesh(const Mesh& other)
		{
			this->vertices = std::vector<float>(other.vertices);
			this->indices = std::vector<std::uint32_t>(other.indices);
			this->normals = std::vector<float>(other.normals);
			this->textureUVs = std::vector<float>(other.textureUVs);
		}
	};
}