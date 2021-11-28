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

#include "LockpickRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

LockpickRecord::LockpickRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  Weight(0.0f),
  Value(0),
  Quality(0.0f),
  Uses(0),
  InventoryIcon(std::string()),
  ScriptID(std::string())
{}

bool LockpickRecord::equals(const LockpickRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Weight == other.Weight)
      && (Value == other.Value) && (Quality == other.Quality)
      && (Uses == other.Uses) && (InventoryIcon == other.InventoryIcon)
      && (ScriptID == other.ScriptID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool LockpickRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cLOCK), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
    + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
    + 4 /* MODL */ + 4 /* 4 bytes for length */
    + ModelPath.length() + 1 /* length of path +1 byte for NUL termination */
    + 4 /* FNAM */ + 4 /* 4 bytes for length */
    + Name.length() + 1 /* length of name +1 byte for NUL termination */
    + 4 /* LKDT */ + 4 /* 4 bytes for length */ + 16 /* size of lockpick data */
    + 4 /* ITEX */ + 4 /* 4 bytes for length */
    + InventoryIcon.length() + 1 /* length of icon path +1 byte for NUL termination */;

  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
          + ScriptID.length() + 1 /* length of script ID +1 byte for NUL termination */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Lockpicks:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    LKDT = Lock Data (16 bytes), required
        float Weight
        long  Value
        float Quality
        long  Uses
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  // write lock pick's name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write lock pick data (LKDT)
  output.write(reinterpret_cast<const char*>(&cLKDT), 4);
  SubLength = 16;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&Quality), 4);
  output.write(reinterpret_cast<const char*>(&Uses), 4);

  // write inventory icon (ITEX)
  output.write(reinterpret_cast<const char*>(&cITEX), 4);
  SubLength = InventoryIcon.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(InventoryIcon.c_str(), SubLength);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool LockpickRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Lockpicks:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    LKDT = Lock Data (16 bytes), required
        float Weight
        long  Value
        float Quality
        long  Uses
    ITEX = Inventory Icon
    SCRI = Script Name (optional)

    Note: Sequence of ITEX and SCRI could also be changed.
  */

  uint32_t BytesRead = 0;

  // read NAME
  char buffer[256];
  if (!loadString256WithHeader(input, recordID, buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of LOCK.\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of LOCK.\n";
    return false;
  }

  // read object's name (FNAM)
  if (!loadString256WithHeader(input, Name, buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of LOCK.\n";
    return false;
  }

  // read LKDT
  uint32_t SubRecName = 0;
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cLKDT)
  {
    UnexpectedRecord(cLKDT, SubRecName);
    return false;
  }
  // LKDT's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 16)
  {
    std::cerr << "Error: Sub record LKDT of LOCK has invalid length ("
              << SubLength << " bytes). Should be 16 bytes.\n";
    return false;
  }
  // read lock pick data
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(&Quality), 4);
  input.read(reinterpret_cast<char*>(&Uses), 4);
  BytesRead += 16;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord LKDT of LOCK.\n";
    return false;
  }

  ScriptID = "";
  InventoryIcon = "";
  bool hasITEX = false;
  bool hasSCRI = false;

  while (BytesRead < Size)
  {
    // read next subrecord
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cITEX:
           if (hasITEX)
           {
             std::cerr << "Error: Record LOCK seems to have two ITEX subrecords.\n";
             return false;
           }
           if (!loadString256(input, InventoryIcon, buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading subrecord ITEX of LOCK.\n";
             return false;
           }
           hasITEX = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cerr << "Error: Record LOCK seems to have two SCRI subrecords.\n";
             return false;
           }
           if (!loadString256(input, ScriptID, buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of LOCK.\n";
             return false;
           }
           hasSCRI = true;
           break;
      default:
           // unexpected record found
           std::cerr << "Error: Expected record name ITEX or SCRI was not "
                     << "found. Instead, \"" << IntTo4Char(SubRecName)
                     << "\" was found.\n";
           return false;
    }
  }

  // at least ITEX should have been present
  if (!hasITEX)
  {
    std::cerr << "Error: Record LOCK seems to have no ITEX subrecord.\n";
    return false;
  }

  return true;
}

} // namespace
