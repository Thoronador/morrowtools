/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021  Thoronador

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

#include "StaticRecord.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

StaticRecord::StaticRecord()
: BasicRecord(),
  recordID(""),
  Mesh("")
{
}

bool StaticRecord::equals(const StaticRecord& other) const
{
  return (recordID == other.recordID) && (Mesh == other.Mesh);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool StaticRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSTAT), 4);
  const uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for MODL's length */
        + Mesh.length() + 1 /*length of mesh plus one for NUL-termination */;
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Static:
    NAME = ID string
    MODL = NIF model*/

  // write record ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str() ,SubLength);

  // write mesh path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = Mesh.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Mesh.c_str() ,SubLength);
  return output.good();
}
#endif

bool StaticRecord::loadFromStream(std::istream& input)
{
  uint32_t Size;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Static:
    NAME = ID string
    MODL = NIF model*/

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  // read NAME
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  // NAME's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength > 255)
  {
    std::cout << "Error: Subrecord NAME of STAT is longer than 255 characters.\n";
    return false;
  }
  // read NAME
  char Buffer[256];
  memset(Buffer, '\0', 256);
  input.read(Buffer, SubLength);
  if (!input.good())
  {
    std::cout << "Error while reading subrecord NAME of STAT.\n";
    return false;
  }
  recordID = std::string(Buffer);

  // read MODL
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  // MODL's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength > 255)
  {
    std::cout << "Error: Subrecord MODL of STAT is longer than 255 characters.\n";
    return false;
  }
  // read MODL
  memset(Buffer, '\0', 256);
  input.read(Buffer, SubLength);
  if (!input.good())
  {
    std::cout << "Error while reading subrecord MODL of STAT.\n";
    return false;
  }
  Mesh = std::string(Buffer);
  return true;
}

} // namespace
