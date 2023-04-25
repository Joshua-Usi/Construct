#pragma once

#include <array>
#include <unordered_map>
#include <vector>

namespace Construct::internal
{
	struct FloatArrayHash
	{
		std::size_t operator()(const std::array<float, 5>& arr) const
		{
			std::size_t seed = 0;
			for (float f : arr)
			{
				// Bitwise manipulation to mix the bits of the float representation and the seed
				seed ^= std::hash<float>{}(f)+0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};
	struct FloatArrayEqual
	{
		bool operator()(const std::array<float, 5>& lhs, const std::array<float, 5>& rhs) const {
			const float epsilon = 1e-5f; // Adjust this value depending on the required precision
			for (size_t i = 0; i < 5; ++i)
			{
				if (std::abs(lhs[i] - rhs[i]) > epsilon)
				{
					return false;
				}
			}
			return true;
		}
	};
	inline void ComputeHalfVertex(float* vertexArray, float* textureUVArray, std::size_t v1Index, std::size_t v2Index, std::size_t outputIndex)
	{
		// Vertex Midpoint calculation
		{
			float vx = vertexArray[v1Index * 3 + 0] + vertexArray[v2Index * 3 + 0];
			float vy = vertexArray[v1Index * 3 + 1] + vertexArray[v2Index * 3 + 1];
			float vz = vertexArray[v1Index * 3 + 2] + vertexArray[v2Index * 3 + 2];
			float length = 0.5f / std::sqrtf(vx * vx + vy * vy + vz * vz);
			vertexArray[outputIndex * 3 + 0] = vx * length;
			vertexArray[outputIndex * 3 + 1] = vy * length;
			vertexArray[outputIndex * 3 + 2] = vz * length;
		}
		// Texture UV midpoint calculation
		{
			float tx = (textureUVArray[v1Index * 2 + 0] + textureUVArray[v2Index * 2 + 0]) / 2.0f;
			float ty = (textureUVArray[v1Index * 2 + 1] + textureUVArray[v2Index * 2 + 1]) / 2.0f;
			textureUVArray[outputIndex * 2 + 0] = tx;
			textureUVArray[outputIndex * 2 + 1] = ty;
		}
	}
	/// <summary>
	/// Subdivide an Icosphere to generate higher quality meshes
	/// </summary>
	/// <param name="inputMesh"></param>
	/// <param name="subdivisions"></param>
	/// <returns></returns>
	inline Mesh IcosphereSubdivide(const Mesh& inputMesh, std::uint32_t subdivisions)
	{
		// takes a 2, 1, 0 triangle, subdivides it serpinski style and remaps the indices to this array
		static constexpr std::array<std::uint32_t, 12> IcosphereSubdivisionRemapIndices = {
			0, 1, 2,
			1, 3, 4,
			2, 4, 5,
			2, 1, 4,
		};

		Mesh currentMesh = Mesh(inputMesh);
		// Perform the specified number of subdivisions
		for (std::uint32_t i = 0; i < subdivisions; i++)
		{
			// Point map so we don't duplicate vertices
			// Maps a vertex and texture coordinate to an index
			std::unordered_map<std::array<float, 5>, std::uint32_t, FloatArrayHash, FloatArrayEqual> vertexMap;
			std::uint32_t nextFreeIndex = 0;
			Mesh outputMesh;
			for (std::uint32_t j = 0, size = currentMesh.indices.size() / 3; j < size; j++)
			{
				const std::uint32_t index1 = currentMesh.indices[j * 3 + 0];
				const std::uint32_t index2 = currentMesh.indices[j * 3 + 1];
				const std::uint32_t index3 = currentMesh.indices[j * 3 + 2];

				float vertices[6 * 3] = {
					currentMesh.vertices[index1 * 3 + 0], currentMesh.vertices[index1 * 3 + 1], currentMesh.vertices[index1 * 3 + 2],
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					currentMesh.vertices[index2 * 3 + 0], currentMesh.vertices[index2 * 3 + 1], currentMesh.vertices[index2 * 3 + 2],
					0.0f, 0.0f, 0.0f,
					currentMesh.vertices[index3 * 3 + 0], currentMesh.vertices[index3 * 3 + 1], currentMesh.vertices[index3 * 3 + 2],
				};
				float textureUVs[6 * 2] = {
					currentMesh.textureUVs[index1 * 2 + 0], currentMesh.textureUVs[index1 * 2 + 1],
					0.0f, 0.0f,
					0.0f, 0.0f,
					currentMesh.textureUVs[index2 * 2 + 0], currentMesh.textureUVs[index2 * 2 + 1],
					0.0f, 0.0f,
					currentMesh.textureUVs[index3 * 2 + 0], currentMesh.textureUVs[index3 * 2 + 1],
				};
				// Compute half vertices and remap to surface
				ComputeHalfVertex(vertices, textureUVs, 0, 3, 1);
				ComputeHalfVertex(vertices, textureUVs, 0, 5, 2);
				ComputeHalfVertex(vertices, textureUVs, 3, 5, 4);
				for (std::uint32_t k = 0, size2 = IcosphereSubdivisionRemapIndices.size(); k < size2; k++)
				{
					const std::uint32_t index = IcosphereSubdivisionRemapIndices[k];
					const std::array<float, 5> key = {
						vertices[index * 3 + 0],
						vertices[index * 3 + 1],
						vertices[index * 3 + 2],
						textureUVs[index * 2 + 0],
						textureUVs[index * 2 + 1]
					};
					// Vertex Already exists, don't add
					if (vertexMap.find(key) != vertexMap.end())
					{
						outputMesh.indices.push_back(vertexMap[key]);
					}
					// Vertex doesn't exist, add
					else
					{
						// Add index to map
						vertexMap[key] = nextFreeIndex;
						// Index
						outputMesh.indices.push_back(nextFreeIndex);
						// Vertex
						outputMesh.vertices.push_back(key[0]);
						outputMesh.vertices.push_back(key[1]);
						outputMesh.vertices.push_back(key[2]);
						// Texture UVs
						outputMesh.textureUVs.push_back(key[3]);
						outputMesh.textureUVs.push_back(key[4]);
						// Increment for next free index
						nextFreeIndex++;
					}
				}
			}
			currentMesh = Mesh(outputMesh);
		}
		return currentMesh;
	}
}