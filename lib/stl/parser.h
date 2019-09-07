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

#include "../math.h"
#include "../triangle.h"

#include <filesystem>
#include <optional>

namespace stl2thumb
{

class Parser
{
public:
    Parser();

    std::optional<Mesh> parseFile(const std::filesystem::path& filepath) const;
    std::optional<Mesh> parseStream(std::istream& in) const;

private:
    bool isBinaryFormat(std::istream& in) const;

    Mesh parseBinary(std::istream& in) const;
    Mesh parseAscii(std::istream& in) const;

    uint32_t readU32(std::istream& in) const;
    uint16_t readU16(std::istream& in) const;
    float readFloat(std::istream& in) const;

    Vec3 readVector3(std::istream& in) const;
    Triangle readBinaryTriangle(std::istream& in) const;
    Triangle readAsciiTriangle(std::istream& in) const;
};

} // namespace
