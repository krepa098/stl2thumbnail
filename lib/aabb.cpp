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

#include "aabb.h"

namespace stl2thumb
{

AABBox::AABBox()
{
}

AABBox::AABBox(const Mesh& mesh)
{
    lower = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    upper = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };

    for (const auto& t : mesh)
    {
        lower.x = std::min(std::min(lower.x, t.vertices[0].x), std::min(t.vertices[1].x, t.vertices[2].x));
        lower.y = std::min(std::min(lower.y, t.vertices[0].y), std::min(t.vertices[1].y, t.vertices[2].y));
        lower.z = std::min(std::min(lower.z, t.vertices[0].z), std::min(t.vertices[1].z, t.vertices[2].z));

        upper.x = std::max(std::max(upper.x, t.vertices[0].x), std::max(t.vertices[1].x, t.vertices[2].x));
        upper.y = std::max(std::max(upper.y, t.vertices[0].y), std::max(t.vertices[1].y, t.vertices[2].y));
        upper.z = std::max(std::max(upper.z, t.vertices[0].z), std::max(t.vertices[1].z, t.vertices[2].z));
    }
}

} // namespace
