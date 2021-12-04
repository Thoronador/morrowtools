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

#include "AlchemyPotionRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

AlchemyPotionRecord::AlchemyPotionRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  // alchemy data
  Weight(0.0f),
  Value(0),
  AutoCalc(0),
  // end of alchemy data
  Enchs(std::vector<EnchantmentData>()),
  InventoryIcon(std::string()),
  ScriptID(std::string())
{
}

bool AlchemyPotionRecord::equals(const AlchemyPotionRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
   && (Name == other.Name) && (Weight == other.Weight) && (Value == other.Value)
   && (AutoCalc == other.AutoCalc) && (Enchs == other.Enchs)
   && (InventoryIcon == other.InventoryIcon) && (ScriptID ==other.ScriptID);
}

bool AlchemyPotionRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Alchemy Potions:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    ALDT = Alchemy Data (12 bytes), required
        float  Weight
        long   Value
        long   AutoCalc
    ENAM = Enchantment (24 bytes) 1 to 8 per record
        short EffectID
        byte  SkillID		for skill related effects, -1/0 otherwise
        byte  AttributeID	for attribute related effects, -1/0 otherwise
        long  Range Type    (not really relevant for potion)
        long  Area          (not really relevant for potion)
        long  Duration
        long  MagnitudeMin
        long  MagnitudeMax
    TEXT = Inventory Icon
    SCRI = Script Name (optional) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read potion ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of ALCH.\n";
    return false;
  }

  // read potion's model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of ALCH.\n";
    return false;
  }
  if (ModelPath.empty())
  {
    std::cerr << "Error: ALCH has empty MODL subrecord!\n";
    return false;
  }

  // reset data that still needs to be read
  Enchs.clear();
  InventoryIcon.clear();
  Name.clear();
  ScriptID.clear();

  // read data
  bool hasReadALDT = false;
  while (BytesRead < Size)
  {
    // read next subrecord's name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cALDT:
           if (hasReadALDT)
           {
             std::cerr << "Error: ALCH seems to have more than one ALCT subrecord.\n";
             return false;
           }
           // ALDT's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 12)
           {
             std::cerr << "Error: Sub record ALDT of ALCH has invalid length ("
                       << SubLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read data of potion
           input.read(reinterpret_cast<char*>(&Weight), 4);
           input.read(reinterpret_cast<char*>(&Value), 4);
           input.read(reinterpret_cast<char*>(&AutoCalc), 4);
           BytesRead += 12;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord ALDT of ALCH.\n";
             return false;
           }
           hasReadALDT = true;
           break;
      case cENAM:
           {
             EnchantmentData data;
             if (!data.loadFromStream(input))
             {
               std::cerr << "Error while reading subrecord ENAM of ALCH.\n";
               return false;
             }
             BytesRead += 28;
             Enchs.emplace_back(data);
           }
           break;
      case cFNAM:
           if (!Name.empty())
           {
             std::cerr << "Error: ALCH seems to have more than one FNAM subrecord.\n";
             return false;
           }
           if (!loadString256(input, Name, Buffer, cFNAM, BytesRead))
           {
             std::cerr << "Error while reading subrecord FNAM of ALCH.\n";
             return false;
           }
           if (Name.empty())
           {
             std::cerr << "Error: ALCH has empty FNAM subrecord!\n";
             return false;
           }
           break;
      case cTEXT:
           if (!InventoryIcon.empty())
           {
             std::cerr << "Error: ALCH seems to have more than one TEXT subrecord.\n";
             return false;
           }
           if (!loadString256(input, InventoryIcon, Buffer, cTEXT, BytesRead))
           {
             std::cerr << "Error while reading subrecord TEXT of ALCH.\n";
             return false;
           }
           if (InventoryIcon.empty())
           {
             std::cerr << "Error: ALCH has empty TEXT subrecord!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cerr << "Error: ALCH seems to have more than one SCRI subrecord.\n";
             return false;
           }
           if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of ALCH.\n";
             return false;
           }
           if (ScriptID.empty())
           {
             std::cerr << "Error: ALCH has empty SCRI subrecord!\n";
             return false;
           }
           break;
      default:
           // other sub record name means error
           std::cout << "AlchemyPotionRecord: Error: Expected record name ALDT,"
                     << "ENAM, FNAM, TEXT or SCRI was not found. Instead, \""
                     << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
    }
  }

  if (!hasReadALDT)
  {
    std::cerr << "Error: Subrecord ALDT of ALCH is missing!\n";
    return false;
  }

  return true;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool AlchemyPotionRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cALCH), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
      + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
      + 4 /* MODL */ + 4 /* 4 bytes for length */
      + ModelPath.length() + 1 /* length of path +1 byte for NUL termination */
      + 4 /* TEXT */ + 4 /* 4 bytes for length */
      + InventoryIcon.length() + 1 /* length of icon path +1 byte for NUL */
      + 4 /* FNAM */ + 4 /* 4 bytes for length */
      + Name.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* ALDT */ + 4 /* ALDT's length */ + 12 /* size of ALDT */
      + Enchs.size() * 32 /* 4 + 4 + 24 */;
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1; /* length of script ID +1 byte for NUL */
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Alchemy Potions:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    ALDT = Alchemy Data (12 bytes), required
        float  Weight
        long   Value
        long   AutoCalc
    ENAM = Enchantment (24 bytes) 1 to 8 per record
        short EffectID
        byte  SkillID		for skill related effects, -1/0 otherwise
        byte  AttributeID	for attribute related effects, -1/0 otherwise
        long  Range Type
        long  Area
        long  Duration
        long  MagnitudeMin
        long  MagnitudeMax
    TEXT = Inventory Icon
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

  // write inventory icon (TEXT)
  output.write(reinterpret_cast<const char*>(&cTEXT), 4);
  SubLength = InventoryIcon.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(InventoryIcon.c_str(), SubLength);

  // write potion name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write alchemy data (ALDT)
  output.write(reinterpret_cast<const char*>(&cALDT), 4);
  SubLength = 12;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&AutoCalc), 4);

  // write enchantments
  for(const auto& enchantment: Enchs)
  {
    if (!enchantment.saveToStream(output))
    {
      std::cerr << "Error while writing enchantment data of potion to stream!\n";
      return false;
    }
  }

  if (!ScriptID.empty())
  {
    // write script name / id (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  return output.good();
}
#endif

} // namespace
