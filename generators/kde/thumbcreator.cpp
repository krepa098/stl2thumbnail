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

#include "backends/raster/backend.h"
#include "picture.h"
#include "stl/parser.h"

#include <QString>

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
    auto pic = backend.render(width, height, mesh);

    return true;
}
