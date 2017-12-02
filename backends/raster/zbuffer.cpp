#include "zbuffer.h"

#include <limits>

ZBuffer::ZBuffer(unsigned size)
    : m_size(size)
{
    m_buffer.resize(size * size);

    for (size_t i = 0; i < m_buffer.size(); ++i)
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
