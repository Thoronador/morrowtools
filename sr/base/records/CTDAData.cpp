/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "CTDAData.h"
#include <iostream>

namespace SRTP
{

bool CTDAData::loadFromStream(std::istream& in_Stream, uint32_t& bytesRead)
{
  //CTDA's length
  uint16_t subLength = 0;
  in_Stream.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=32)
  {
    std::cout <<"Error: sub record CTDA has invalid length ("<<subLength
              <<" bytes). Should be 32 bytes.\n";
    return false;
  }
  //read CTDA data
  in_Stream.read((char*) content, 32);
  bytesRead += 32;
  if (!in_Stream.good())
  {
    std::cout << "Error while reading subrecord CTDA!\n";
    return false;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
bool CTDAData::saveToStream(std::ostream& output) const
{
  //write CTDA
  output.write((const char*) &cCTDA, 4);
  //CTDA's length
  const uint16_t subLength = 32; //fixed size
  output.write((const char*) &subLength, 2);
  //write content
  output.write((const char*) content, 32);
  return output.good();
}
#endif

bool CTDAData::operator==(const CTDAData& other) const
{
  return (memcmp(content, other.content, 32)==0);
}

void CTDAData::clear()
{
  memset(content, 0, 32);
}

} //namespace
