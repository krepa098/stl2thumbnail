/*
Copyright (C) 2017  Paul Kremer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <cmath>
#include <iostream>

struct Vec3
{
    float x = .0f;
    float y = .0f;
    float z = .0f;

    float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() const
    {
        float l = length();
        return { x / l, y / l, z / l };
    }

    friend std::ostream& operator<<(std::ostream& out, const Vec3& v)
    {
        out << "[X: " << v.x << " Y: " << v.y << " Z: " << v.z << "]";
        return out;
    }
};

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vec3 operator-(const Vec3& a, const Vec3& b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vec3 operator-(const Vec3& a)
{
    return { -a.x, -a.y, -a.z };
}

inline Vec3 operator*(float a, const Vec3& b)
{
    return { a * b.x, a * b.y, a * b.z };
}

inline Vec3 operator*(const Vec3& b, float a)
{
    return { a * b.x, a * b.y, a * b.z };
}

inline Vec3 operator*(const Vec3& a, const Vec3& b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

inline float dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

inline Vec3 reflect(const Vec3& i, const Vec3& n)
{
    return i - 2.0f * dot(n, i) * n;
}
