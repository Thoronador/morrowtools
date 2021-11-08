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

#include "ActivatorRecord.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

ActivatorRecord::ActivatorRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  ItemName(std::string()),
  ScriptName(std::string())
{
}

bool ActivatorRecord::equals(const ActivatorRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (ItemName == other.ItemName) && (ScriptName == other.ScriptName);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ActivatorRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cACTI), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
      + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
      + 4 /* MODL */ + 4 /* 4 bytes for MODL's length */
      + ModelPath.length() + 1 /*length of mesh plus one for NUL-termination */
      + 4 /* FNAM */ + 4 /* 4 bytes for length */
      + ItemName.length() + 1 /* length of name +1 byte for NUL termination */;
  if (!ScriptName.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
          + ScriptName.length() + 1 /*length of script ID + NUL-terminator */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Activators:
    NAME = Activator ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

  // write record ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  //write item's name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = ItemName.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ItemName.c_str(), SubLength);
  if (!ScriptName.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptName.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptName.c_str(), SubLength);
  } // if optional script ID is present
  return output.good();
}
#endif

bool ActivatorRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size = 0;
  in_File.read(reinterpret_cast<char*>(&Size), 4);
  in_File.read(reinterpret_cast<char*>(&HeaderOne), 4);
  in_File.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Activators:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    SCRI = Script Name (optional) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  //read NAME
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead = 4;
  if (SubRecName != cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength > 255)
  {
    std::cerr << "Subrecord NAME of ACTI is longer than 255 characters!\n";
    return false;
  }
  char Buffer[256];
  memset(Buffer, '\0', 256);
  // read activator ID
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord NAME of ACTI!\n";
    return false;
  }
  recordID = std::string(Buffer);

  // read MODL
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  // MODL's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength > 255)
  {
    std::cerr << "Subrecord MODL of ACTI is longer than 255 characters!\n";
    return false;
  }
  // read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord MODL of ACTI!\n";
    return false;
  }
  ModelPath = std::string(Buffer);

  // read FNAM
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  // FNAM's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength > 255)
  {
    std::cerr << "Subrecord FNAM of ACTI is longer than 255 characters!\n";
    return false;
  }
  // read item name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord FNAM of ACTI!\n";
    return false;
  }
  ItemName = std::string(Buffer);

  if (BytesRead < Size)
  {
    // read optional SCRI
    in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
    if (SubRecName != cSCRI)
    {
      UnexpectedRecord(cSCRI, SubRecName);
      return false;
    }
    // SCRI's length
    in_File.read(reinterpret_cast<char*>(&SubLength), 4);
    if (SubLength > 255)
    {
      std::cerr << "Subrecord SCRI of ACTI is longer than 255 characters!\n";
      return false;
    }
    // read script ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    if (!in_File.good())
    {
      std::cerr << "Error while reading subrecord SCRI of ACTI!\n";
      return false;
    }
    ScriptName = std::string(Buffer);
  }
  else
  {
    ScriptName.clear();
  }
  return in_File.good();
}

} // namespace
