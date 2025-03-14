/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2013, 2021, 2025  Dirk Stolle

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

#include "MiscItemRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

MiscItemRecord::MiscItemRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  // miscellaneous item data
  Weight(0.0f),
  Value(0),
  OtherStuff(0),
  // end of miscellaneous item data
  InventoryIcon(std::string()),
  ScriptID(std::string())
{}

bool MiscItemRecord::equals(const MiscItemRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Weight == other.Weight)
      && (Value == other.Value) && (OtherStuff == other.OtherStuff)
      && (InventoryIcon == other.InventoryIcon) && (ScriptID == other.ScriptID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool MiscItemRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cMISC), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
    + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
    + 4 /* MODL */ + 4 /* 4 bytes for length */
    + ModelPath.length() + 1 /* length of name +1 byte for NUL termination */
    + 4 /* MCDT */ + 4 /* 4 bytes for length */ + 12 /* size of misc. data */
    + 4 /* ITEX */ + 4 /* 4 bytes for length */
    + InventoryIcon.length() + 1 /* length of path +1 byte for NUL termination */;
  if (!Name.empty())
  {
    Size = Size + 4 /* FNAM */ + 4 /* 4 bytes for length */
         + Name.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1 /* length of ID +1 byte for NUL termination */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Misc. Items:
    NAME = item ID, required
    MODL = model filename, required
    FNAM = item name (not present in one case)
    MCDT = Misc Data, 12 bytes, required
        float Weight
        long  Value
        long  Unknown
    ITEX = Inventory icon filename
    SCRI = script ID string (optional)
  */

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

  if (!Name.empty())
  {
    // write misc. item's name (FNAM)
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = Name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Name.c_str(), SubLength);
  }

  // write misc. data (MCDT)
  output.write(reinterpret_cast<const char*>(&cMCDT), 4);
  SubLength = 12;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&OtherStuff), 4);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  // write inventory icon (ITEX)
  output.write(reinterpret_cast<const char*>(&cITEX), 4);
  SubLength = InventoryIcon.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(InventoryIcon.c_str(), SubLength);

  return output.good();
}
#endif

bool MiscItemRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Misc. Items:
    NAME = item ID, required
    MODL = model filename, required
    FNAM = item name (not present in one case (Tribunal))
    MCDT = Misc Data, 12 bytes, required
        float Weight
        long  Value
        long  Unknown
    ITEX = Inventory icon filename
    SCRI = script ID string (optional)
  */

  uint32_t bytesRead = 0;

  // read item ID (NAME)
  char buffer[256];
  if (!loadString256WithHeader(input, recordID, buffer, cNAME, bytesRead))
  {
    std::cerr << "Error while reading sub record NAME of MISC!\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, buffer, cMODL, bytesRead))
  {
    std::cerr << "Error while reading sub record MODL of MISC!\n";
    return false;
  }

  // read FNAM (may not be present sometimes)
  uint32_t SubRecName = 0;
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  bytesRead += 4;
  if (SubRecName == cFNAM)
  {
    // read item's name (FNAM)
    if (!loadString256(input, Name, buffer, cFNAM, bytesRead))
    {
      std::cerr << "Error while reading sub record FNAM of MISC!\n";
      return false;
    }

    // read MCDT
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    bytesRead += 4;
  }
  else
  {
    Name.clear();
  }

  // read MCDT
  // sub record name was already read above
  if (SubRecName != cMCDT)
  {
    UnexpectedRecord(cMCDT, SubRecName);
    return false;
  }
  // MCDT's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  bytesRead += 4;
  if (SubLength != 12)
  {
    std::cerr << "Error: Sub record MODL of MISC has invalid length ("
              << SubLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read misc. data
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(&OtherStuff), 4);
  bytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record MCDT of MISC!\n";
    return false;
  }

  InventoryIcon.clear();
  ScriptID.clear();
  bool hasITEX = false;
  bool hasSCRI = false;
  while (bytesRead < Size)
  {
    // read next sub record header
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    bytesRead += 4;
    switch (SubRecName)
    {
      case cITEX:
           if (hasITEX)
           {
             std::cerr << "Error: Record MISC seems to have two ITEX sub records.\n";
             return false;
           }
           // read inventory icon (ITEX)
           if (!loadString256(input, InventoryIcon, buffer, cITEX, bytesRead))
           {
             std::cerr << "Error while reading sub record ITEX of MISC!\n";
             return false;
           }
           hasITEX = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cerr << "Error: Record MISC seems to have two SCRI sub records.\n";
             return false;
           }
           // read script ID (SCRI)
           if (!loadString256(input, ScriptID, buffer, cSCRI, bytesRead))
           {
             std::cerr << "Error while reading sub record SCRI of MISC!\n";
             return false;
           }
           hasSCRI = true;
           break;
      default:
           std::cerr << "Error while reading MISC: Expected record name ITEX "
                     << "or SCRI was not found. Instead, \""
                     << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
           break;
    }
  }

  return true;
}

} // namespace
