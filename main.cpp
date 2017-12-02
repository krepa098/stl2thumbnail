#include <iostream>
#include <parser.h>

#include "args.hxx"
#include "backends/opengl/backend.h"
#include "backends/raster/backend.h"
#include "picture.h"

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
    auto triangles = stlParser.parseFile(in.Get());

    std::cout << "Triangles: " << triangles.size() << std::endl;

    // render using openGL backend
    //    OpenGLBackend backend(picSize.Get());
    //    auto pic = backend.render(triangles);

    // render using raster backend
    RasterBackend backend(picSize.Get());
    auto pic = backend.render(triangles);

    // save to disk
    pic.save(out.Get());

    return 0;
}
