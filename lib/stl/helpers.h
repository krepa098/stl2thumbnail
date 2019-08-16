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

#include <algorithm>
#include <string>

namespace stl2thumb
{

/**
 * @brief trim removes all whitespaces from a string
 * @param s
 */
void trim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {
        return !std::isspace(c);
    }));
}

/**
 * @brief getTrimmedLine reads a line and trims it
 * @param in input stream
 * @param out trimmed string
 */
void getTrimmedLine(std::istream& in, std::string& out)
{
    std::getline(in, out);
    trim(out);
}

} // namespace
