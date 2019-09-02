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

namespace stl2thumb
{

using Byte   = std::uint8_t;
using Buffer = std::vector<Byte>;

struct RGBA
{
    Byte r = 0;
    Byte g = 0;
    Byte b = 0;
    Byte a = 0;

    RGBA() {}

    RGBA(Byte r, Byte g, Byte b, Byte a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {
    }

    RGBA(float r, float g, float b, float a)
        : r(std::max(0.0f, std::min(r, 1.0f)) * 255.0f)
        , g(std::max(0.0f, std::min(g, 1.0f)) * 255.0f)
        , b(std::max(0.0f, std::min(b, 1.0f)) * 255.0f)
        , a(std::max(0.0f, std::min(a, 1.0f)) * 255.0f)
    {
    }
};

class Picture
{
public:
    /**
     * @brief Picture creates a black picture of given size
     * @param width the width of the image
     * @param height the height of the image
     */
    Picture(std::uint32_t width, std::uint32_t height);

    /**
     * @brief data returns a pointer to the raw data of the picture
     * The pixels are stored in RGBA8888 format (width x height)
     * @return
     */
    const Byte* data() const;

    /**
     * @brief dataLength returns the total size of the picture in bytes
     * @return
     */
    std::size_t dataLength() const;

    /**
     * @brief save writes the picture to a file in png format
     * @param filename
     */
    void save(const std::string& filename);

    /**
     * @brief exportEncoded returns a buffer containing the picture encoded as png
     * @return
     */
    Buffer exportEncoded();

    /**
     * @brief setPixel sets a pixel to a given value
     * @param x the x position
     * @param y the y position
     * @param color the pixel color
     */
    void setPixel(unsigned x, unsigned y, RGBA color);

    /**
     * @brief fill fills the entire picture with a given color
     * @param r the red color
     * @param g the green color
     * @param b the blue color
     * @param a the alpha color
     */
    void fill(RGBA color);

    /**
     * @brief pixel returns the pixel color at a given position
     * @param x the x position
     * @param y the y position
     * @return
     */
    RGBA pixel(unsigned x, unsigned y) const;

    /**
     * @brief width returns the width of the picture
     * @return
     */
    std::uint32_t width() const;

    /**
     * @brief height returns the height of the picture
     * @return
     */
    std::uint32_t height() const;

    /**
     * @brief stride returns the size of the stride i.e. width*depth
     * @return
     */
    std::uint32_t stride() const;

private:
    Buffer m_buffer;
    std::uint32_t m_width  = 0;
    std::uint32_t m_height = 0;
    std::uint32_t m_depth  = 4; // rgba
    std::uint32_t m_stride = 0;
};

} // namespace
