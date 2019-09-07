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
#include <vector>

class ZBuffer
{
public:
    ZBuffer(std::uint32_t width, std::uint32_t height);

    bool testAndSet(std::uint32_t x, std::uint32_t y, float z);

    std::uint32_t width() const;
    std::uint32_t height() const;

private:
    std::uint32_t m_width  = 0;
    std::uint32_t m_height = 0;
    std::vector<float> m_buffer;
};
