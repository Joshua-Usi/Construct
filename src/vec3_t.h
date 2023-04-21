#pragma once

#include <cstdint>

namespace Construct
{
	/// <summary>
	/// Simple templated vector3 type for use in this library
	/// </summary>
	/// <typeparam name="type">Vector type</typeparam>
    template <typename type>
    struct vec3_t {
        type x, y, z;

        // Default constructor
        vec3_t() : x(0), y(0), z(0) {}

        // Constructor with arguments
        vec3_t(type x_, type y_, type z_) : x(x_), y(y_), z(z_) {}

        // Copy constructor
        vec3_t(const vec3_t<type>& other) : x(other.x), y(other.y), z(other.z) {}

        // Assignment operator
        vec3_t<type>& operator=(const vec3_t<type>& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        // Equality operator overload
        friend bool operator==(const vec3_t<type>& v1, const vec3_t<type>& v2) {
            return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
        }
    };
}