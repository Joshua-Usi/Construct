# Construct
A C++ Primitive shape generator library

## 2D Primitives
```C++
Quad()
```
Generates a quad facing the -Z direction by default
![Quad Texture UV](./textures/quad-uv.png)
```C++
Plane(unsigned int width, unsigned int height)
```
Generates a plane of width * height tiles facing the +Y direction by default
![Plane Texture UV](./textures/quad-uv.png)
```C++
Polygon(unsigned int sides)
```
Generates a polygon facing the -Z direction by default. High side counts can be used for circles
![Polygon Texture UV](./textures/polygon-uv.png)

## 3D Primitives
```C++
Cube()
```
Generates a 24 vertex cube with correct normals and example texture shown below (Front facing -Z)
![Cube Texture UV](./textures/cube-uv.png)
```C++
UVSphere(unsigned int rings, unsigned int segments)
```
Generates a UV sphere with a number of rings and segments. Useful for easy texturing
![UV Sphere Texture UV](./textures/uv-sphere-uv.png)
```C++
Icosphere(unsigned int subdivisions)
```
Generates a subdivided icosphere. More organic looking due to it's uniformity. Useful for heightmapping.
Generally keep subdivisions below 10, Number of triangles grows exponentially
![Icosphere Texture UV](./textures/uv-sphere-uv.png)
```C++
Cylinder(unsigned int sides)
```
Generates a cylinder with a given number of sides. Low side counts can be used for prisms
![Cylinder Texture UV](./textures/cylinder-uv.png)
```C++
Capsule()
```
Generates the classic capsule shape.
![Capsule Texture UV](./textures/cylinder-uv.png)

## Skyboxes
```C++
SkyboxCube()
```
Generates a Cube skybox
![Skybox Cube Texture UV](./textures/cube-uv.png)
```C++
SkyboxSphere()
```
Generates a Sphere skybox
![Skybox Sphere Texture UV](./textures/uv-sphere-uv.png)
