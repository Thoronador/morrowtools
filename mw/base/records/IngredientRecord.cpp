/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "IngredientRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

IngredientRecord::IngredientRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  // IngredientData
  Weight(0.0f),
  Value(0),
  EffectID({ 0, 0, 0, 0 }),
  SkillID({ 0, 0, 0, 0 }),
  AttributeID({ 0, 0, 0, 0 }),
  // end of Ingredient data
  InventoryIcon(std::string()),
  ScriptID(std::string())
{
}

bool IngredientRecord::equals(const IngredientRecord& other) const
{
  return (recordID==other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Weight == other.Weight)
      && (Value == other.Value) && (EffectID == other.EffectID)
      && (SkillID == other.SkillID) && (AttributeID == other.AttributeID)
      && (InventoryIcon == other.InventoryIcon) && (ScriptID == other.ScriptID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool IngredientRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cINGR), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
      + recordID.length() + 1 /* length of ID +1 byte for NUL-termination */
      + 4 /* MODL */ + 4 /* 4 bytes for MODL's length */
      + ModelPath.length() + 1 /* length of path +1 for NUL-termination */
      + 4 /* FNAM */ + 4 /* 4 bytes for FNAM's length */
      + Name.length() + 1 /* length of name plus one for NUL-termination */
      + 4 /* IRDT */ + 4 /* IRDT's length */ + 56 /* size of ingredient data */
      + 4 /* ITEX */ + 4 /* ITEX's length */
      + InventoryIcon.length() +1 /* length of path +1 byte for NUL */;
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for SCRI's length */
         + ScriptID.length() + 1 /* length of script ID +1 byte for NUL */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Ingredients:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    IRDT = Ingredient Data (56 bytes), required
        float  Weight
        long   Value
        long   EffectID[4]	0 or -1 means no effect
        long   SkillID[4]	only for Skill related effects, 0 or -1 otherwise
        long   AttributeID[4]  only for Attribute related effects, 0 or -1 otherwise
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

  // write ingredient name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write ingredient data (IRDT)
  output.write(reinterpret_cast<const char*>(&cIRDT), 4);
  SubLength = 56;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write ingredient data
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(EffectID.data()), 16);
  output.write(reinterpret_cast<const char*>(SkillID.data()), 16);
  output.write(reinterpret_cast<const char*>(AttributeID.data()), 16);

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

bool IngredientRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Ingredients:
    NAME = Item ID, required
    MODL = Model Name, required
    FNAM = Item Name, required
    IRDT = Ingredient Data (56 bytes), required
        float  Weight
        long   Value
        long   EffectID[4]	0 or -1 means no effect
        long   SkillID[4]	only for Skill related effects, 0 or -1 otherwise
        long   AttributeID[4]  only for Attribute related effects, 0 or -1 otherwise
    ITEX = Inventory Icon
    SCRI = Script Name (optional) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read record ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of INGR.\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of INGR.\n";
    return false;
  }

  // read "real" name of ingredient (FNAM)
  if (!loadString256WithHeader(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of INGR.\n";
    return false;
  }

  // read IRDT
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cIRDT)
  {
    UnexpectedRecord(cIRDT, SubRecName);
    return false;
  }
  // IRDT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 56)
  {
    std::cerr << "Error: sub record IRDT of INGR has invalid length ("
              << SubLength << " bytes). Should be 56 bytes.\n";
    return false;
  }
  // read ingredient data
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(EffectID.data()), 16);
  input.read(reinterpret_cast<char*>(SkillID.data()), 16);
  input.read(reinterpret_cast<char*>(AttributeID.data()), 16);
  BytesRead += 56;
  if (!input.good())
  {
    std::cerr << "Error while reading IRDT values.\n";
    return false;
  }

  // read SCRI (optional) and ITEX (mandatory)
  ScriptID.clear();
  InventoryIcon.clear();
  while (BytesRead < Size)
  {
    // read next record name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cerr << "Error: INGR seems to have more than one ITEX subrecord!\n";
             return false;
           }
           // read icon path (ITEX)
           if (!loadString256(input, InventoryIcon, Buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading icon path of INGR.\n";
             return false;
           }
           if (InventoryIcon.empty())
           {
             std::cerr << "Error: Subrecord ITEX of INGR is empty!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cerr << "Error: INGR seems to have more than one SCRI subrecord!\n";
             return false;
           }
           // read ingredient's script name (SCRI)
           if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of INGR.\n";
             return false;
           }
           if (ScriptID.empty())
           {
             std::cerr << "Error: Subrecord SCRI of INGR is empty!\n";
             return false;
           }
           break;
      default:
           // other subrecord means error, so quit here
           std::cerr << "IngredientRecord: Error: Expected record name ITEX or SCRI. "
                     << "Instead, \"" << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
    }
  }
  if (InventoryIcon.empty())
  {
    std::cerr << "IngredientRecord::loadFromStream: Error: Empty or no "
              << "inventory icon path read.\n";
    return false;
  }

  return true;
}

} // namespace
