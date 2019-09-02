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

#include <iostream>

#include "args.hxx"
#include "backends/raster/rasterbackend.h"
#include "picture.h"
#include "stl/parser.h"

using namespace stl2thumb;

int main(int argc, char** argv)
{
    // command line
    args::ArgumentParser parser("Creates thumbnails from STL files", "");
    args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });

    args::Group inputGroup(parser, "Required:", args::Group::Validators::All);
    args::ValueFlag<std::string> in(inputGroup, "string", "The input stl filename", { "if" });
    args::ValueFlag<std::string> out(inputGroup, "string", "The output thumbnail picture filename", { "of" });

    args::Group sizeGroup(parser, "All or none:", args::Group::Validators::AllOrNone);
    args::ValueFlag<unsigned> picWidth(sizeGroup, "uint", "The thumbnail width", { "width" }, 512);
    args::ValueFlag<unsigned> picHeight(sizeGroup, "uint", "The thumbnail height", { "height" }, 512);

    args::Group colorGroup(parser, "All or none:", args::Group::Validators::AllOrNone);
    args::ValueFlag<float> r(colorGroup, "float [0,1]", "Red color channel of the model", { "r" });
    args::ValueFlag<float> g(colorGroup, "float [0,1]", "Green color channel of the model", { "g" });
    args::ValueFlag<float> b(colorGroup, "float [0,1]", "Blue color channel of the model", { "b" });

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        parser.Help(std::cerr);
        return 1;
    }

    // parse STL
    Parser stlParser;
    Mesh mesh;
    try
    {
        mesh = stlParser.parseFile(in.Get());
    }
    catch (...)
    {
        std::cerr << "Cannot parse file " << in.Get();
        return 1;
    }

    std::cout << "Triangles: " << mesh.size() << std::endl;

    // render using raster backend
    RasterBackend backend;

    // model color
    if (colorGroup && colorGroup.MatchedChildren() > 0)
    {
        backend.modelColor = {
            r.Get(), g.Get(), b.Get()
        };
    }

    // render
    auto pic = backend.render(picWidth.Get(), picHeight.Get(), mesh);

    // save to disk
    pic.save(out.Get());

    return 0;
}
