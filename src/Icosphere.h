#pragma once

#include <vector>
#include <cstdint>
#include <numbers>
#include <cmath>
#include <array>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <functional>

#include "Mesh.h"
#include "../data/data.h"
#include "vec3_t.h"

namespace Construct::internal
{
	struct FloatArrayHash {
		std::size_t operator()(const std::array<float, 5>& arr) const {
			std::size_t seed = 0;
			for (float f : arr) {
				// Bitwise manipulation to mix the bits of the float representation and the seed
				seed ^= std::hash<float>{}(f) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};
	struct FloatArrayEqual {
		bool operator()(const std::array<float, 5>& lhs, const std::array<float, 5>& rhs) const {
			const float epsilon = 1e-5f; // Adjust this value depending on the required precision
			for (size_t i = 0; i < 5; ++i) {
				if (std::abs(lhs[i] - rhs[i]) > epsilon) {
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
				for (std::uint32_t k = 0, size2 = data.IcosphereSubdivisionRemapIndices.size(); k < size2; k++)
				{
					const std::uint32_t index = data.IcosphereSubdivisionRemapIndices[k];
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
	/// <summary>
	/// Generate the mesh for a base case Icosphere
	/// </summary>
	/// <returns>Icosphere subdivision 0 mesh</returns>
	inline Mesh IcosphereBase()
	{
		Mesh mesh;
		// 22 points, 5 * 2 for the poles (+10), 6 * 2 for the rings
		mesh.vertices = std::vector<float>(22 * 3, 0.0f);

		// Cache common calculations
		const float va = std::atanf(0.5f);
		const float xy = std::cosf(va) * 0.5f;
		const float z = std::sinf(va) * 0.5f;
		for (std::uint32_t i = 0; i <= 5; i++)
		{
			// Calculate angle of rings
			const float angle = static_cast<float>(i) / 5.0f * 2.0f * std::numbers::pi_v<float>;
			const float angle2 = angle - 0.2f * std::numbers::pi_v<float>;
			// Precalculate indices
			const std::uint32_t topRingIndex = (5 + i) * 3;
			const std::uint32_t bottomRingIndex = (11 + i) * 3;
			// Top ring
			mesh.vertices[topRingIndex + 0] = xy * std::cosf(angle);
			mesh.vertices[topRingIndex + 1] = z;
			mesh.vertices[topRingIndex + 2] = xy * std::sinf(angle);
			// Bottom ring
			mesh.vertices[bottomRingIndex + 0] = xy * std::cosf(angle2);
			mesh.vertices[bottomRingIndex + 1] = -z;
			mesh.vertices[bottomRingIndex + 2] = xy * std::sinf(angle2);

			// Generate pole vertices
			if (i < 5)
			{
				// X and Z are already 0 initialised
				// North Pole
				mesh.vertices[i * 3 + 1] = 0.5f;
				// South Pole
				mesh.vertices[(i + 17) * 3 + 1] = -0.5f;
			}
		}
		// Add indices
		mesh.indices = std::vector<std::uint32_t>(data.IcosphereBaseIndices);
		mesh.textureUVs = std::vector<float>(data.IcosphereBaseTextureUVs);
		// Remap U component by dividing by 30.0f
		for (std::uint32_t i = 0, size = mesh.textureUVs.size(); i < size; i += 2)
		{
			mesh.textureUVs[i] /= 30.0f;
		}

		return mesh;
	}
}