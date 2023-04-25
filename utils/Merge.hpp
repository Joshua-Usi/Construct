#pragma once

#include "../internal/Mesh.hpp"

namespace Construct
{
	/// <summary>
	/// Naively merges meshes into one mesh
	/// </summary>
	/// <param name="meshes">A vector of pointers to meshes</param>
	/// <returns>Merged mesh</returns>
    inline Mesh Merge(const std::vector<Mesh*>& meshes)
    {
        Mesh mergedMesh;
        std::uint32_t currentIndex = 0;
        for (Mesh* mesh : meshes)
        {
            // Add vertices to the merged mesh
            mergedMesh.vertices.insert(mergedMesh.vertices.end(), mesh->vertices.begin(), mesh->vertices.end());
            // Add indices to the merged mesh
            for (std::uint32_t index : mesh->indices)
            {
                mergedMesh.indices.push_back(index + currentIndex);
            }
            // Add normals to the merged mesh
            mergedMesh.normals.insert(mergedMesh.normals.end(), mesh->normals.begin(), mesh->normals.end());
            // Add textureUVs to the merged mesh
            mergedMesh.textureUVs.insert(mergedMesh.textureUVs.end(), mesh->textureUVs.begin(), mesh->textureUVs.end());
            // Update the current index to be the last vertex index + 1
            currentIndex += mesh->vertices.size() / 3;
        }
        return mergedMesh;
    }
}