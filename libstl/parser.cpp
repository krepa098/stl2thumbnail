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

#include "parser.h"
#include "helpers.h"

#include <fstream>

#define HEADER_SIZE 80

// STL format specifications: http://www.fabbers.com/tech/STL_Format

namespace stl
{
Parser::Parser()
{
}

Mesh Parser::parseFile(const std::string& filename) const
{
    std::ifstream stream(filename, std::ifstream::in | std::ifstream::binary);

    if (!stream)
        throw("Cannot open file");

    if (isBinaryFormat(stream))
        return parseBinary(stream);

    return parseAscii(stream);
}

bool Parser::isBinaryFormat(std::ifstream& in) const
{
    // Note: A file starting with "solid" is no indicator for having an ASCII file
    // Some exporters put "solid <name>" in the binary header

    std::string line;
    getLineTrimmed(in, line); // skip potential string: solid <name>
    getLineTrimmed(in, line); // has to start with "facet" otherwise it is a binary file

    // return to begin of file
    in.clear();
    in.seekg(0, in.beg);

    return line.substr(0, 5) != "facet";
}

Mesh Parser::parseBinary(std::ifstream& in) const
{
    // skip header
    in.seekg(HEADER_SIZE);

    Mesh triangles;

    // get the number of triangles in the stl
    uint32_t triangleCount = readU32(in);
    triangles.reserve(triangleCount);

    // parse triangles
    for (size_t i = 0; i < triangleCount && in; ++i)
    {
        Triangle triangle = readBinaryTriangle(in);

        triangles.push_back(triangle);
    }

    return triangles;
}

Mesh Parser::parseAscii(std::ifstream& in) const
{
    Mesh triangles;

    // solid name
    std::string line;
    std::getline(in, line);

    while (in)
    {
        triangles.push_back(readAsciiTriangle(in));
    }

    return triangles;
}

uint32_t Parser::readU32(std::ifstream& in) const
{
    uint32_t v;
    in.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

uint16_t Parser::readU16(std::ifstream& in) const
{
    uint16_t v;
    in.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

float Parser::readFloat(std::ifstream& in) const
{
    float v;
    in.read(reinterpret_cast<char*>(&v), sizeof(v));
    return v;
}

Vec3 Parser::readVector3(std::ifstream& in) const
{
    Vec3 v;
    v.x = readFloat(in);
    v.y = readFloat(in);
    v.z = readFloat(in);

    return v;
}

Triangle Parser::readAsciiTriangle(std::ifstream& in) const
{
    Triangle t;

    std::string line;

    getLineTrimmed(in, line);
    std::sscanf(line.c_str(), "facet normal %e %e %e", &t.normal.x, &t.normal.y, &t.normal.z);

    std::getline(in, line); // outer loop

    for (size_t i = 0; i < 3; ++i)
    {
        getLineTrimmed(in, line);
        std::sscanf(line.c_str(), "vertex %e %e %e", &t.vertices[i].x, &t.vertices[i].y, &t.vertices[i].z);
    }

    std::getline(in, line); // endloop
    std::getline(in, line); // endfacet

    return t;
}

Triangle Parser::readBinaryTriangle(std::ifstream& in) const
{
    Triangle t;

    t.normal      = readVector3(in).normalize();
    t.vertices[0] = readVector3(in);
    t.vertices[1] = readVector3(in);
    t.vertices[2] = readVector3(in);
    readU16(in); // attributes

    // some stl files have garbage normals
    // we may recalculate them here
    //t.normal = t.calcNormal().normalize();

    return t;
}
} // namespace
