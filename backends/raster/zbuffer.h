#pragma once

#include <vector>

class ZBuffer
{
public:
    ZBuffer(unsigned size);

    bool testAndSet(unsigned x, unsigned y, float z);

private:
    unsigned m_size = 0;
    std::vector<float> m_buffer;
};
