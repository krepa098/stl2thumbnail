#pragma once

#include "../triangle.h"
#include "../vec3.h"

namespace stl
{
class Parser
{
public:
    Parser();

    Mesh parseFile(const std::string& filename) const;

private:
    bool isBinaryFormat(std::ifstream& in) const;

    Mesh parseBinary(std::ifstream& in) const;
    Mesh parseAscii(std::ifstream& in) const;

    u_int32_t readU32(std::ifstream& in) const;
    u_int16_t readU16(std::ifstream& in) const;
    float readFloat(std::ifstream& in) const;

    Vec3 readVector3(std::ifstream& in) const;
    Triangle readBinaryTriangle(std::ifstream& in) const;
    Triangle readAsciiTriangle(std::ifstream& in) const;
};
} // namespace
