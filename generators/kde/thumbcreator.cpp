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

Q_LOGGING_CATEGORY(LOG_STL_THUMBS, "STLModelThumbs")

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
    //qCDebug(LOG_STL_THUMBS) << "Creating thumbnail for " << path;

    stl::Parser stlParser;
    Mesh mesh;
    mesh = stlParser.parseFile(path.toStdString());

    // render using raster backend
    RasterBackend backend;
    auto pic = backend.render(width, height, mesh);

    // copy to QImage
    img = QImage(pic.data(), width, height, pic.stride(), QImage::Format_RGBA8888);

    return true;
}
