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

#include "triangle.h"

#include <limits>

namespace stl2thumb
{

class AABBox
{
public:
    AABBox();

    /**
     * @brief AABBox creates an AABB comprising the mesh
     * @param mesh
     */
    AABBox(const Mesh& mesh);

    /**
     * @brief diagonalLength returns the length of the diagonal
     * @return
     */
    float diagonalLength() const
    {
        return std::sqrt(size().x * size().x + size().y * size().y + size().z * size().z);
    }

    /**
     * @brief size returns the size of the AABB
     * @return
     */
    Vec3 size() const
    {
        return upper - lower;
    }

    /**
     * @brief center returns the center position of the AABB
     * @return
     */
    Vec3 center() const
    {
        return lower + size() * 0.5f;
    }

    Vec3 lower;
    Vec3 upper;
};

} // namespace
