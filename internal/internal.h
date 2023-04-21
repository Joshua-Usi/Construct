#pragma once

#include "./data.h"

#include <vector>
#include <cstdint>
#include <cmath>
#include <numbers>
#include <array>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <functional>

namespace Construct
{
    /// <summary>
    /// Simple templated vector3 type for use in this library
    /// </summary>
    /// <typeparam name="type">Vector type</typeparam>
    template <typename type>
    struct vec3_t
	{
        type x, y, z;

        // Default constructor
        vec3_t() : x(0), y(0), z(0) {}

        // Constructor with arguments
        vec3_t(type x_, type y_, type z_) : x(x_), y(y_), z(z_) {}

        // Copy constructor
        vec3_t(const vec3_t<type>& other) : x(other.x), y(other.y), z(other.z) {}

        // Assignment operator
        vec3_t<type>& operator=(const vec3_t<type>& other)
		{
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        // Equality operator overload
        friend bool operator==(const vec3_t<type>& v1, const vec3_t<type>& v2)
		{
            return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
        }
    };
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
		Mesh() = default;
		/// <summary>
		/// Initialise arrays with lengths and initialise to 0
		/// </summary>
		inline Mesh(std::uint32_t vertexCount, std::uint32_t indexCount, std::uint32_t normalCount, std::uint32_t textureUVs)
		{
			this->vertices = std::vector<float>(vertexCount, 0.0f);
			this->indices = std::vector<std::uint32_t>(indexCount, 0);
			this->normals = std::vector<float>(normalCount, 0.0f);
			this->textureUVs = std::vector<float>(textureUVs, 0.0f);
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
	/// <summary>
	/// Enum for GeneratorSetting that defines face direction.
	/// CCW by default.
	/// </summary>
	enum class FaceDirection : std::uint8_t { CCW, CW };
	/// <summary>
	/// Defines generator settings to generate specific data or data manipulations
	/// </summary>
	struct GeneratorSetting
	{
		vec3_t<float> offset;
		vec3_t<float> scale;
		FaceDirection faceDirection;

		/// <summary>
		/// Define generator settings
		/// </summary>
		/// <param name="direction">Defines what direction the generated face will be, CCW by default</param>
		/// <param name="off">Offset vector for the center of models, defaults to 0.0f, 0.0f, 0.0f</param>
		/// <param name="sc">Scale vector for the size of models, defaults to 1.0f, 1.0f, 1.0f</param>
		inline GeneratorSetting(FaceDirection direction = FaceDirection::CCW, const vec3_t<float>& off = vec3_t<float>(0.0f, 0.0f, 0.0f), const vec3_t<float>& sc = vec3_t<float>(1.0f, 1.0f, 1.0f))
		{
			this->faceDirection = direction;
			this->offset = off;
			this->scale = sc;
		}
	};
	namespace internal
	{
		inline const std::vector<float> CalculateNormals(const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
		{
			std::vector<float> normals(vertices.size(), 0.0f);
			for (uint32_t i = 0; i < indices.size(); i += 3) {
				// Get index of vertices in triangle
				const uint32_t i1 = indices[i + 0] * 3,
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
		// Thanks to user79758 on stack overflow
		template <typename T>
		std::int8_t sign(T val)
		{
			return (static_cast<T>(0) < val) - (val < static_cast<T>(0));
		}
		inline void ProcessMesh(Mesh& mesh, const GeneratorSetting& settings)
		{
			// Process vertices for scale and offset
			// If offset or scale is left default, then ignore
			if (
				settings.offset != vec3_t<float>(0.0f, 0.0f, 0.0f) ||
				settings.scale != vec3_t<float>(1.0f, 1.0f, 1.0f)
				)
			{
				for (std::size_t i = 0, size = mesh.vertices.size(); i < size; i += 3)
				{
					mesh.vertices[i + 0] = mesh.vertices[i + 0] * settings.scale.x + settings.offset.x;
					mesh.vertices[i + 1] = mesh.vertices[i + 1] * settings.scale.y + settings.offset.y;
					mesh.vertices[i + 2] = mesh.vertices[i + 2] * settings.scale.x + settings.offset.z;
				}
			}
			// Process for face direction
			if (settings.faceDirection == FaceDirection::CW)
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
}