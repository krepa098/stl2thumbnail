#pragma once

#include "triangle.h"

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
