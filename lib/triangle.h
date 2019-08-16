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

#include <array>
#include <vector>

#include "vec3.h"

namespace stl2thumb
{

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

} // namespace
