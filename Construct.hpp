#pragma once

#include <vector>
#include <cstdint>

#include "internal/GeneratorSetting.hpp"
#include "internal/Mesh.hpp"

namespace Construct
{
	/// <summary>
	/// Generates a Quad mesh facing the +z direction
	/// </summary>
	/// <param name="settings">Setting that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Quad</returns>
	Mesh Quad(const GeneratorSetting& settings = GeneratorSetting());
	/// <summary>
	/// Generates a Plane mesh facing the +y direction
	/// </summary>
	/// <param name="widthTiles">Number of tiles along the width</param>
	/// <param name="heightTiles">Number of tiles along the height</param>
	/// <param name="settings">Setting that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Plane</returns>
	Mesh Plane(std::uint32_t widthTiles, std::uint32_t heightTiles, const GeneratorSetting& settings = GeneratorSetting());
	/// <summary>
	/// Generates a Polygon mesh facing the +z direction.
	/// High side counts can be used to generate circles
	/// </summary>
	/// <param name="sides">Number of sides for the polygon</param>
	/// <param name="settings">Setting that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Polygon</returns>
	Mesh Polygon(std::uint32_t sides, const GeneratorSetting& settings = GeneratorSetting());
	/// <summary>
	/// Generates a Cube mesh
	/// Uses a 6x1 texture layout with the texture being in the order:
	/// Left, Front, Right, Back, Top, Bottom
	/// (-x, +z, +x, -x, +y, -y)
	/// </summary>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for the Cube</returns>
	Mesh Cube(const GeneratorSetting& settings = GeneratorSetting());
	/// <summary>
	/// Generates a UV Sphere mesh
	/// Uses an equirectangular texture layout
	/// </summary>
	/// <param name="rings">Number of longitude lines</param>
	/// <param name="segments">Number of latitude lines</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a UVSphere</returns>
	Mesh UVSphere(std::uint32_t rings, std::uint32_t segments, const GeneratorSetting& settings = GeneratorSetting());
	/// <summary>
	/// Generates an Icosphere mesh
	/// TODO Select texture layout for Cylinder
	/// </summary>
	/// <param name="subdivisions">Number of subdivisions, leave 0 for base case (icosahedron)</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for an Icosphere</returns>
	Mesh Icosphere(std::uint32_t subdivisions, const GeneratorSetting& settings = GeneratorSetting());
	/// <summary>
	/// Generates a Cylinder mesh
	/// TODO Select texture layout for Cylinder
	/// </summary>
	/// <param name="sides">Number of sides the cylinder mesh has</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Cylinder</returns>
	Mesh Cylinder(std::uint32_t sides, const GeneratorSetting& settings = GeneratorSetting());
	/// <summary>
	/// Generates a Capsule mesh
	/// TODO Select texture layout for capsule
	/// </summary>
	/// <param name="sides">Number of sides in the capsule mesh, this also affects the number of rings in the hemispheres</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Capsule</returns>
	Mesh Capsule(std::uint32_t sides, const GeneratorSetting& settings = GeneratorSetting());

	/// <summary>
	/// Generates a Skybox Cube
	/// Uses a 6x1 texture layout with the texture being in the order:
	/// Left, Front, Right, Back, Top, Bottom
	/// (-x, +z, +x, -x, +y, -y)
	/// </summary>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Skybox Cube</returns>
	Mesh SkyboxCube(const GeneratorSetting& settings = GeneratorSetting(WindingOrder::CW));
	/// <summary>
	/// Generates a Skybox Sphere
	/// Uses an equirectangular texture layout
	/// </summary>
	/// <param name="rings">Number of longitude lines</param>
	/// <param name="segments">Number of latitude lines</param>
	/// <param name="settings">Settings that affect how the mesh is generated</param>
	/// <returns>Mesh data for a Skybox Sphere</returns>
	Mesh SkyboxSphere(std::uint32_t rings, std::uint32_t segments, const GeneratorSetting& settings = GeneratorSetting(WindingOrder::CW));
}