#include "aabb.h"

AABBox::AABBox()
{
}

AABBox::AABBox(const Mesh& triangles)
{
    lower = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    upper = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };

    for (const auto& t : triangles)
    {
        lower.x = std::min(std::min(lower.x, t.vertices[0].x), std::min(t.vertices[1].x, t.vertices[2].x));
        lower.y = std::min(std::min(lower.y, t.vertices[0].y), std::min(t.vertices[1].y, t.vertices[2].y));
        lower.z = std::min(std::min(lower.z, t.vertices[0].z), std::min(t.vertices[1].z, t.vertices[2].z));

        upper.x = std::max(std::max(upper.x, t.vertices[0].x), std::max(t.vertices[1].x, t.vertices[2].x));
        upper.y = std::max(std::max(upper.y, t.vertices[0].y), std::max(t.vertices[1].y, t.vertices[2].y));
        upper.z = std::max(std::max(upper.z, t.vertices[0].z), std::max(t.vertices[1].z, t.vertices[2].z));
    }
}
