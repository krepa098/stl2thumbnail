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
#include "vec4.h"

// A rasterizer based on
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation
class RasterBackend : public Backend
{
public:
    RasterBackend(unsigned size);
    ~RasterBackend();

    Picture render(const Mesh& triangles);

private:
    unsigned m_size        = 0;
    Vec3 m_modelColor      = { 1.0f, 0.925f, 0.208f };
    Vec3 m_ambientColor    = { 0.3f, 0.3f, 0.3f };
    Vec3 m_diffuseColor    = { 0.5f, 0.5f, 0.5f };
    Vec3 m_specColor       = { 1.0f, 1.0f, 1.0f };
    Vec4 m_backgroundColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    Vec3 m_lightPos        = { 4, -2, 2 };
};
