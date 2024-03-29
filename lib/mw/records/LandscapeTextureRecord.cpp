/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012  Thoronador

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

#include "LandscapeTextureRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

LandscapeTextureRecord::LandscapeTextureRecord()
: BasicRecord(),
  recordID(""),
  Index(-1),
  Path("")
{}

bool LandscapeTextureRecord::equals(const LandscapeTextureRecord& other) const
{
  return ((recordID==other.recordID) and (Index==other.Index)
         and (Path==other.Path));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool LandscapeTextureRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cLTEX, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* INTV */ +4 /* 4 bytes for INTV's length */ +4 /* size of INTV */
        +4 /* DATA */ +4 /* 4 bytes for length */
        +Path.length()+1 /* length of path plus one for NUL-termination */;
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*LandscapeTextures:
    NAME = Item ID, required
    INTV = Index (unique?), required
    DATA = Texture path, required */

  //write NAME
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = recordID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write NAME/ID
  output.write(recordID.c_str(), SubLength);

  //write INTV
  output.write((const char*) &cINTV, 4);
  SubLength = 4;
  //write INTV's length
  output.write((const char*) &SubLength, 4);
  //write Index
  output.write((const char*) &Index, SubLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  SubLength = Path.length()+1;
  //write DATA's length
  output.write((const char*) &SubLength, 4);
  //write DATA
  output.write(Path.c_str(), SubLength);
  return output.good();
}
#endif

bool LandscapeTextureRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*LandscapeTextures:
    NAME = Item ID, required
    INTV = Index (unique?), required
    DATA = Texture path, required */

  uint32_t SubRecName;
  uint32_t SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Subrecord NAME of LTEX is longer than 255 characters!\n";
    return false;
  }
  char Buffer[256];
  memset(Buffer, '\0', 256);
  //read texture ID
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of LTEX!\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read INTV
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINTV)
  {
    UnexpectedRecord(cINTV, SubRecName);
    return false;
  }
  //INTV's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Subrecord INTV of LTEX has invalid length ("<<SubLength
              << " bytes). Should be four bytes!\n";
    return false;
  }
  //read index
  Index = -1;
  in_File.read((char*) &Index, 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord INTV of LTEX!\n";
    return false;
  }

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Subrecord DATA of LTEX is longer than 255 characters!\n";
    return false;
  }
  //read texture path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of LTEX!\n";
    return false;
  }
  Path = std::string(Buffer);
  return true;
}

} //namespace
