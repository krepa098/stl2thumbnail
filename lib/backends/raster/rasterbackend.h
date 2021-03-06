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

#include "../backend.h"
#include "math.h"

#include <glm/mat4x4.hpp>

namespace stl2thumb
{

class AABBox;

// A rasterizer based on
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation
class RasterBackend : public Backend
{
public:
    RasterBackend();
    ~RasterBackend();

    /**
     * @brief render renders a mesh in a picture of given size using a software rasterizer
     * @param imgWidth
     * @param imgHeight
     * @param mesh
     * @return
     */
    Picture render(unsigned imgWidth, unsigned imgHeight, const Mesh& mesh) override;

protected:
    float scaleInView(const glm::mat4& modelViewProj, const AABBox& aabb);
};

} // namespace
