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

#include "zbuffer.h"

#include <limits>

ZBuffer::ZBuffer(std::uint32_t width, std::uint32_t height)
    : m_width(width)
    , m_height(height)
{
    m_buffer.resize(width * height, std::numeric_limits<float>::max());
}

bool ZBuffer::testAndSet(std::uint32_t x, std::uint32_t y, float z)
{
    if (z < m_buffer[y * m_width + x])
    {
        m_buffer[y * m_width + x] = z;
        return true;
    }

    return false;
}

std::uint32_t ZBuffer::width() const
{
    return m_width;
}

std::uint32_t ZBuffer::height() const
{
    return m_height;
}
