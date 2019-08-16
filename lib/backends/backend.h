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
};

} // namespace
