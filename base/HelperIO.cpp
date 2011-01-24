/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "HelperIO.h"
#include <iostream>

std::string IntTo4Char(const int32_t value)
{
  std::string result = "1234";
  result[0] = char(value&255);
  result[1] = char((value>>8)&255);
  result[2] = char((value>>16)&255);
  result[3] = char(value>>24);
  return result;
}

void UnexpectedRecord(const int32_t expected, const int32_t unexpected)
{
  std::cout << "Error: expected record name \""<<IntTo4Char(expected)
            <<"\" was not found. Instead, \""<<IntTo4Char(unexpected)
            <<"\" was found.\n";
  return;
}

bool SkipRecord(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  Size = 0;
  in_File.read((char*) &Size, 4);
  if (Size<0)
  {
    std::cout << "Error: record size is negative.\n";
    return false;
  }
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);
  /* data does not really matter here */
  in_File.seekg(Size, std::ios::cur);
  return in_File.good();
}//SkipRecord
