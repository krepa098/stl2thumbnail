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

#include "../triangle.h"
#include "../vec3.h"

namespace stl
{
class Parser
{
public:
    Parser();

    Mesh parseFile(const std::string& filename) const;

private:
    bool isBinaryFormat(std::ifstream& in) const;

    Mesh parseBinary(std::ifstream& in) const;
    Mesh parseAscii(std::ifstream& in) const;

    uint32_t readU32(std::ifstream& in) const;
    uint16_t readU16(std::ifstream& in) const;
    float readFloat(std::ifstream& in) const;

    Vec3 readVector3(std::ifstream& in) const;
    Triangle readBinaryTriangle(std::ifstream& in) const;
    Triangle readAsciiTriangle(std::ifstream& in) const;
};
} // namespace
