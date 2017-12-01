#pragma once

#include <array>
#include <vector>

#include "vec3.h"

struct Ray;

struct Triangle
{
    std::array<Vec3, 3> vertices;
    Vec3 normal;

    friend std::ostream& operator<<(std::ostream& out, const Triangle& v)
    {
        out << ">>>\n";
        out << v.normal << "\n";
        out << v.vertices[0] << "\n"
            << v.vertices[1] << "\n"
            << v.vertices[2] << "\n";
        out << "<<<\n";
        return out;
    }

    Vec3 calcNormal() const
    {
        Vec3 v0v1 = vertices[1] - vertices[0];
        Vec3 v0v2 = vertices[2] - vertices[0];
        return cross(v0v1, v0v2);
    }
};

using Mesh = std::vector<Triangle>;
