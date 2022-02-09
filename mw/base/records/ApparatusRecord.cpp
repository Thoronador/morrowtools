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

#include "ApparatusRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

ApparatusRecord::ApparatusRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  type(Type::MortarAndPestle),
  Quality(0.0f),
  Weight(0.0f),
  Value(0),
  InventoryIcon(std::string()),
  ScriptID(std::string())
{}

bool ApparatusRecord::equals(const ApparatusRecord& other) const
{
  return (recordID == other.recordID)
      && (ModelPath == other.ModelPath) && (Name == other.Name)
      && (type == other.type) && (Quality == other.Quality)
      && (Weight == other.Weight) && (Value == other.Value)
      && (InventoryIcon == other.InventoryIcon) && (ScriptID == other.ScriptID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ApparatusRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cAPPA), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for MODL's length */
        + ModelPath.length() + 1 /*length of mesh plus one for NUL-termination */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + Name.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* AADT */ + 4 /* 4 bytes for length */ + 16 /* length of AADT */
        + 4 /* ITEX */ + 4 /* 4 bytes for length */
        + InventoryIcon.length() + 1 /* length of icon path +1 byte for NUL termination */;
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1 /* length of script ID + one byte for NUL */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Alchemy Apparatus:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    AADT = Alchemy Data (16 bytes), required
        long    Type (0=Mortar and Pestle,1=Alembic,2=Calcinator,3=Retort)
        float	Quality
        float	Weight
        long	Value
    ITEX = Inventory Icon
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

  // write item name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write apparatus data (AADT)
  output.write(reinterpret_cast<const char*>(&cAADT), 4);
  SubLength = 16;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write alchemy apparatus data
  output.write(reinterpret_cast<const char*>(&type), 4);
  output.write(reinterpret_cast<const char*>(&Quality), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);

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

bool ApparatusRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Alchemy Apparatus:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    AADT = Alchemy Data (16 bytes), required
        long    Type (0=Mortar and Pestle,1=Alembic,2=Calcinator,3=Retort)
        float	Quality
        float	Weight
        long	Value
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read NAME
  char buffer[256];
  if (!loadString256WithHeader(input, recordID, buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of APPA!\n";
    return false;
  }

  ModelPath.clear();
  Name.clear();
  bool hasReadAADT = false;
  InventoryIcon.clear();
  ScriptID.clear();

  while (BytesRead < Size)
  {
    // read sub record's name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cMODL:
           if (!ModelPath.empty())
           {
             std::cerr << "Error: APPA seems to have more than one MODL sub record!\n";
             return false;
           }
           // read model path
           if (!loadString256(input, ModelPath, buffer, cMODL, BytesRead))
           {
             std::cerr << "Error while reading sub record MODL of APPA!\n";
             return false;
           }
           if (ModelPath.empty())
           {
             std::cerr << "Error: Sub record MODL of APPA is empty!\n";
             return false;
           }
           break;
      case cFNAM:
           if (!Name.empty())
           {
             std::cerr << "Error: APPA seems to have more than one FNAM sub record!\n";
             return false;
           }
           // read apparatus name
           if (!loadString256(input, Name, buffer, cFNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record FNAM of APPA!\n";
             return false;
           }
           if (Name.empty())
           {
             std::cerr << "Error: Sub record FNAM of APPA is empty!\n";
             return false;
           }
           break;
      case cAADT:
           if (hasReadAADT)
           {
             std::cerr << "Error: APPA seems to have more than one AADT sub record!\n";
             return false;
           }
           // AADT's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 16)
           {
             std::cerr << "Error: Sub record AADT of APPA has invalid length ("
                       << SubLength << " bytes). Should be 16 bytes.\n";
             return false;
           }
           // read apparatus data
           input.read(reinterpret_cast<char*>(&type), 4);
           input.read(reinterpret_cast<char*>(&Quality), 4);
           input.read(reinterpret_cast<char*>(&Weight), 4);
           input.read(reinterpret_cast<char*>(&Value), 4);
           BytesRead += 16;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record AADT of APPA!\n";
             return false;
           }
           hasReadAADT = true;
           break;
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cerr << "Error: APPA seems to have more than one ITEX sub record!\n";
             return false;
           }
           // read apparatus icon path
           if (!loadString256(input, InventoryIcon, buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading sub record ITEX of APPA!\n";
             return false;
           }
           if (InventoryIcon.empty())
           {
             std::cerr << "Error: Sub record ITEX of APPA is empty!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cerr << "Error: APPA seems to have more than one SCRI sub record!\n";
             return false;
           }
           // read script ID
           if (!loadString256(input, ScriptID, buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading sub record SCRI of APPA!\n";
             return false;
           }
           if (ScriptID.empty())
           {
             std::cerr << "Error: Sub record SCRI of APPA is empty!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: Unexpected record type \"" << IntTo4Char(SubRecName)
                     << "\" found, but only MODL, FNAM, AADT, ITEX or SCRI are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (ModelPath.empty() || Name.empty() || !hasReadAADT || InventoryIcon.empty())
  {
    std::cerr << "Error: At least one required sub record of APPA is missing!\n";
    return false;
  }

  return true;
}

} // namespace
