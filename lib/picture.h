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

#include <cstdint>
#include <string>
#include <vector>

using Byte   = std::uint8_t;
using Buffer = std::vector<Byte>;

class Picture
{
public:
    Picture(std::uint32_t width, std::uint32_t height);

    Byte* data();
    void save(const std::string& filename);
    void setRGB(unsigned x, unsigned y, Byte r, Byte g, Byte b, Byte a = 255);
    void setRGB(unsigned x, unsigned y, float r, float g, float b, float a = 1.0f);
    void fill(float r, float g, float b, float a);

    std::uint32_t width() const;
    std::uint32_t height() const;

private:
    Buffer m_buffer;
    std::uint32_t m_width  = 0;
    std::uint32_t m_height = 0;
    std::uint32_t m_depth  = 4; // rgba
    std::uint32_t m_stride = 0;
};
