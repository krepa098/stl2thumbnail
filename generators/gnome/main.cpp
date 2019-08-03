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
#include "backends/raster/backend.h"
#include "picture.h"
#include "stl/parser.h"

int main(int argc, char** argv)
{
    // command line
    args::ArgumentParser parser("Creates thumbnails from STL files", "");
    args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });

    args::Group group(parser, "This group is all exclusive:", args::Group::Validators::All);
    args::Positional<std::string> in(group, "in", "The stl filename");
    args::Positional<std::string> out(group, "out", "The thumbnail picture filename");
    args::ValueFlag<unsigned> picSize(group, "size", "The thumbnail size", { 's' });

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
    catch (args::ValidationError)
    {
        std::cout << parser;
        return 0;
    }

    // parse STL
    stl::Parser stlParser;
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
    auto pic = backend.render(picSize.Get(), picSize.Get(), mesh);

    // save to disk
    pic.save(out.Get());

    return 0;
}
