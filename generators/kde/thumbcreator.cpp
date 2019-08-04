/*
Copyright (C) 2019  Paul Kremer

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

#include "thumbcreator.h"

#include "backends/raster/rasterbackend.h"
#include "picture.h"
#include "stl/parser.h"

#include <QImage>
#include <QString>

extern "C"
{
    // Factory method
    Q_DECL_EXPORT ThumbCreator* new_creator()
    {
        return new StlThumbCreator();
    }
};

StlThumbCreator::StlThumbCreator()
{
}

bool StlThumbCreator::create(const QString& path, int width, int height, QImage& img)
{
    stl::Parser stlParser;
    Mesh mesh;
    try
    {
        mesh = stlParser.parseFile(path.toStdString());
    }
    catch (...)
    {
        std::cerr << "Cannot parse file " << path.toStdString();
        return false;
    }

    // render using raster backend
    RasterBackend backend;
    const auto pic = backend.render(width, height, mesh);

    // copy to QImage
    for (size_t y = 0; y < pic.height(); ++y)
    {
        for (size_t x = 0; x < pic.width(); ++x)
            img.setPixelColor(x, y, qRgba(pic.pixelRGBA(x, y).r, pic.pixelRGBA(x, y).g, pic.pixelRGBA(x, y).b, pic.pixelRGBA(x, y).a));
    }

    return true;
}
