/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2021 Thoronador

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
 -------------------------------------------------------------------------------
*/

#include "HelperIO.hpp"
#include <iostream>

std::string IntTo4Char(const uint32_t value)
{
  std::string result = "1234";
  result[0] = static_cast<char>(value & 255);
  result[1] = static_cast<char>((value >> 8) & 255);
  result[2] = static_cast<char>((value >> 16) & 255);
  result[3] = static_cast<char>(value >> 24);
  return result;
}

void UnexpectedRecord(const uint32_t expected, const uint32_t unexpected)
{
  std::cerr << "Error: Expected record name \"" << IntTo4Char(expected)
            << "\" was not found. Instead, \"" << IntTo4Char(unexpected)
            << "\" was found.\n";
}
