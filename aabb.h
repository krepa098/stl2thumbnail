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

struct AABBox
{
    Vec3 lower;
    Vec3 upper;

    AABBox();
    AABBox(const Mesh& triangles);

    float largestStride() const
    {
        return std::max(upper.x - lower.x, std::max(upper.y - lower.y, upper.z - lower.z));
    }

    Vec3 size() const
    {
        return upper - lower;
    }

    Vec3 center() const
    {
        return lower + size() * 0.5f;
    }
};
