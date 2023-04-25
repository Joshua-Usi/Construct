#pragma once

#include <cstring>

namespace Construct
{
    class vec3 {
    public:
        float x, y, z;
        // Default constructor
        vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
        // Copy constructor
        vec3(const vec3& other) : x(other.x), y(other.y), z(other.z) {}
        //Adding operator
        // Multiplying operator
        vec3 operator+(const vec3& other) const {
            return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
        }
        // Multiplying operator
        vec3 operator*(const vec3& other) const {
            return vec3(this->x * other.x, this->y * other.y, this->z * other.z);
        }
        // Equality operator
        bool operator==(const vec3& other) const {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }
        // Inequality operator
        bool operator!=(const vec3& other) const {
            return !(*this == other);
        }
    };
    class quat {
    public:
        float x, y, z, w;
        // Default constructor
        quat(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
        // Copy constructor
        quat(const quat& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
        // Calculate the conjugate of a quaternion
        quat conjugate() const {
            return quat(-x, -y, -z, w);
        }
        // Equality operator
        bool operator==(const quat& other) const {
            return this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w;
        }
        // Inequality operator
        bool operator!=(const quat& other) const {
            return !(*this == other);
        }
        /// <summary>
        /// Multiplies 2 quaternions by each other
        /// </summary>
        /// <param name="q1">Quaternion</param>
        /// <returns>multiplied quaternion</returns>
        quat operator*(const quat& other) const {
            return quat(
                this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y,
                this->w * other.y - this->x * other.z + this->y * other.w + this->z * other.x,
                this->w * other.z + this->x * other.y - this->y * other.x + this->z * other.w,
                this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z
            );
        }
        /// <summary>
        /// Multiplies a quaternion and vec3
        /// </summary>
        /// <param name="v">Vector 3</param>
        /// <returns>Transformed point</returns>
        vec3 operator*(const vec3& v) const {
            quat p_quat(v.x, v.y, v.z, 0.0f);
            quat q_conj = this->conjugate();
            quat p_rotated_quat = *this * p_quat * q_conj;

            return vec3(p_rotated_quat.x, p_rotated_quat.y, p_rotated_quat.z);
        }
    };
}