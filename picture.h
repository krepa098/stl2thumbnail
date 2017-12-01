#pragma once

#include <string>
#include <vector>

using Byte   = unsigned char;
using Buffer = std::vector<Byte>;

class Picture
{
public:
    Picture(unsigned size);

    Byte* data();
    void save(const std::string& filename);
    void setRGB(unsigned x, unsigned y, Byte r, Byte g, Byte b);

private:
    Buffer m_buffer;
    unsigned m_size   = 0;
    unsigned m_depth  = 4; // rgba
    unsigned m_stride = 0;
};
