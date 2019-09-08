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

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

namespace stl2thumb
{

using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    out << glm::to_string(v);
    return out;
}

} // namespace
