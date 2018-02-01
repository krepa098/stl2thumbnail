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

ZBuffer::ZBuffer(unsigned size)
    : m_size(size)
{
    m_buffer.resize(size * size);

    for (std::size_t i = 0; i < m_buffer.size(); ++i)
        m_buffer[i] = std::numeric_limits<float>::infinity();
}

bool ZBuffer::testAndSet(unsigned x, unsigned y, float z)
{
    if (z < m_buffer[y * m_size + x])
    {
        m_buffer[y * m_size + x] = z;
        return true;
    }

    return false;
}

unsigned ZBuffer::size() const
{
    return m_size;
}
