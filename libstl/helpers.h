#pragma once

#include <algorithm>
#include <string>

namespace stl
{
void trim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {
        return !std::isspace(c);
    }));
}

void getLineTrimmed(std::istream& in, std::string& out)
{
    std::getline(in, out);
    trim(out);
}
} // namespace
