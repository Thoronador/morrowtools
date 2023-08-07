/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "ProbeRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"
#include "../../base/UtilityFunctions.hpp"

namespace MWTP
{

ProbeRecord::ProbeRecord(const std::string& ID)
: BasicRecord(),
  recordID(ID),
  ModelPath(std::string()),
  Name(std::string()),
  // probe data
  Weight(0.0f),
  Value(0),
  Quality(0.0f),
  Uses(0),
  // end of probe data
  InventoryIcon(std::string()),
  ScriptName(std::string())
{
}

bool ProbeRecord::equals(const ProbeRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Weight == other.Weight) && (Value == other.Value)
      && (Quality == other.Quality) && (Uses == other.Uses)
      && (InventoryIcon == other.InventoryIcon) && (ScriptName == other.ScriptName);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ProbeRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cPROB), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of path +1 byte for NUL termination */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + Name.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* PBDT */ + 4 /* PBDT's length */ + 16 /* size of PBDT */
        + 4 /* ITEX */ + 4 /* 4 bytes for length */
        + InventoryIcon.length() + 1 /* length of path +1 byte for NUL termination */;
  if (!ScriptName.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptName.length() + 1; /* length of script ID +1 byte for NUL termination */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Probe Item:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    PBDT = Probe Data (16 bytes), required
        float	Weight
        long	Value
        float	Quality
        long 	Uses
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

  // write probe name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write probe data (PBDT)
  output.write(reinterpret_cast<const char*>(&cPBDT), 4);
  SubLength = 16;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write probe data
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&Quality), 4);
  output.write(reinterpret_cast<const char*>(&Uses), 4);

  // write inventory icon path (ITEX)
  output.write(reinterpret_cast<const char*>(&cITEX), 4);
  SubLength = InventoryIcon.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(InventoryIcon.c_str(), SubLength);

  if (!ScriptName.empty())
  {
    // write script name / ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptName.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptName.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool ProbeRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read((char*) &Size, 4);
  input.read((char*) &HeaderOne, 4);
  input.read((char*) &HeaderFlags, 4);

  /*Probe Item:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    PBDT = Probe Data (16 bytes), required
        float	Weight
        long	Value
        float	Quality
        long 	Uses
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read probe ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of PROB.\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of PROB.\n";
    return false;
  }

  // read FNAM
  if (!loadString256WithHeader(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of PROB.\n";
    return false;
  }

  // read PBDT
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cPBDT)
  {
    UnexpectedRecord(cPBDT, SubRecName);
    return false;
  }
  // PBDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 16)
  {
    std::cerr << "Error: Sub record PBDT of PROB has invalid length ("
              << SubLength << " bytes). Should be 16 bytes.\n";
    return false;
  }
  // read item data
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(&Quality), 4);
  input.read(reinterpret_cast<char*>(&Uses), 4);
  BytesRead += 16;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord PBDT of PROB.\n";
    return false;
  }

  // read icon texture path (ITEX)
  if (!loadString256WithHeader(input, InventoryIcon, Buffer, cITEX, BytesRead))
  {
    std::cerr << "Error while reading subrecord ITEX of PROB.\n";
    return false;
  }

  if (BytesRead < Size)
  {
    // read optional script name (SCRI)
    if (!loadString256WithHeader(input, ScriptName, Buffer, cSCRI, BytesRead))
    {
      std::cerr << "Error while reading subrecord SCRI of PROB.\n";
      return false;
    }
  }
  else
  {
    ScriptName = "";
  }
  return true;
}

bool operator<(const ProbeRecord& left, const ProbeRecord& right)
{
  return lowerCaseCompare(left.recordID, right.recordID) < 0;
}

} // namespace
