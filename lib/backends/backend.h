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

#include "math.h"
#include "picture.h"
#include "triangle.h"

namespace stl2thumb
{

class Backend
{
public:
    virtual ~Backend() = default;

    /**
     * @brief render renders a mesh in a picture of given size
     * @param imgWidth
     * @param imgHeight
     * @param mesh
     * @return
     */
    virtual Picture render(unsigned imgWidth, unsigned imgHeight, const Mesh& mesh) = 0;

    Vec3 modelColor      = { 0 / 255.f, 120 / 255.f, 255 / 255.f };
    Vec3 ambientColor    = { 0.5f, 0.5f, 0.5f };
    Vec3 diffuseColor    = { 0.7f, 0.7f, 0.7f };
    Vec3 specularColor   = { 0.7f, 0.7f, 0.7f };
    Vec4 backgroundColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    Vec3 lightPos        = { 11.0f, 4.0f, -5.0f };
    Vec3 viewPos         = { 1.f, 1.f, 1.f };
};

} // namespace
