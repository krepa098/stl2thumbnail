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

#include "picture.h"

#include <png.h>

Byte floatToByte(float v)
{
    v = std::max(0.0f, std::min(v, 1.0f));
    return Byte(v * 255.0f);
}

Picture::Picture(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height)
    , m_stride(width * m_depth)
{
    m_buffer.resize(height * m_stride); // rgba 8bit
}

Byte* Picture::data()
{
    return m_buffer.data();
}

void Picture::save(const std::string& filename)
{
    auto fd = fopen(filename.c_str(), "wb");

    auto png_ptr  = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    auto info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fd);
    png_set_IHDR(png_ptr, info_ptr, m_width, m_height,
        8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    for (std::uint32_t row = 0; row < m_height; ++row)
    {
        png_write_row(png_ptr, &m_buffer[row * m_stride]);
    }

    png_write_end(png_ptr, nullptr);

    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, nullptr);

    fclose(fd);
}

Buffer Picture::exportEncoded()
{
    auto png_ptr  = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    auto info_ptr = png_create_info_struct(png_ptr);

    Buffer encoded;
    // This callback will be called each time libpng wants to write an encoded chunk.
    // https://github.com/Prior99/node-libpng/blob/master/native/encode.cpp
    png_set_write_fn(png_ptr, &encoded, [] (png_structp png_ptr, png_bytep data, png_size_t length) {
        auto encoded = reinterpret_cast<Buffer*>(png_get_io_ptr(png_ptr));
        encoded->insert(encoded->end(), data, data + length);
    }, nullptr);

    png_set_IHDR(png_ptr, info_ptr, m_width, m_height,
        8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    for (std::uint32_t row = 0; row < m_height; ++row)
    {
        png_write_row(png_ptr, &m_buffer[row * m_stride]);
    }

    png_write_end(png_ptr, nullptr);

    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, nullptr);
    
    return encoded;
}

void Picture::setRGB(unsigned x, unsigned y, Byte r, Byte g, Byte b, Byte a)
{
    if (x >= m_width || y >= m_height)
        return;

    m_buffer[y * m_stride + x * m_depth + 0] = r;
    m_buffer[y * m_stride + x * m_depth + 1] = g;
    m_buffer[y * m_stride + x * m_depth + 2] = b;
    m_buffer[y * m_stride + x * m_depth + 3] = a;
}

void Picture::setRGB(unsigned x, unsigned y, float r, float g, float b, float a)
{
    if (x >= m_width || y >= m_height)
        return;

    m_buffer[y * m_stride + x * m_depth + 0] = floatToByte(r);
    m_buffer[y * m_stride + x * m_depth + 1] = floatToByte(g);
    m_buffer[y * m_stride + x * m_depth + 2] = floatToByte(b);
    m_buffer[y * m_stride + x * m_depth + 3] = floatToByte(a);
}

void Picture::fill(float r, float g, float b, float a)
{
    for (unsigned y = 0; y < m_height; ++y)
        for (unsigned x = 0; x < m_width; ++x)
            setRGB(x, y, r, g, b, a);
}

Picture::RGBA Picture::pixelRGBA(unsigned x, unsigned y) const
{
    if (x >= m_width || y >= m_height)
        return {};

    return {
        m_buffer[y * m_stride + x * m_depth + 0],
        m_buffer[y * m_stride + x * m_depth + 1],
        m_buffer[y * m_stride + x * m_depth + 2],
        m_buffer[y * m_stride + x * m_depth + 3]
    };
}

uint32_t Picture::width() const
{
    return m_width;
}

uint32_t Picture::height() const
{
    return m_height;
}

uint32_t Picture::stride() const
{
    return m_stride;
}
