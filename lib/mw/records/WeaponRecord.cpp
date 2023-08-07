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

#include "WeaponRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

WeaponRecord::WeaponRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  // weapon data
  Weight(0.0f),
  Value(0),
  Type(0),
  Health(0),
  Speed(0.0f),
  Reach(0.0f),
  EnchantPts(0),
  ChopMin(0),
  ChopMax(0),
  SlashMin(0),
  SlashMax(0),
  ThrustMin(0),
  ThrustMax(0),
  WeaponFlags(0),
  // end of weapon data
  ScriptID(std::string()),
  InventoryIcon(std::string()),
  EnchantmentID(std::string())
{
}

bool WeaponRecord::equals(const WeaponRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Weight == other.Weight) && (Value == other.Value)
      && (Type == other.Type) && (Health == other.Health) && (Speed == other.Speed)
      && (Reach == other.Reach) && (EnchantPts == other.EnchantPts)
      && (ChopMin == other.ChopMin) && (ChopMax == other.ChopMax)
      && (SlashMin == other.SlashMin) && (SlashMax == other.SlashMax)
      && (ThrustMin == other.ThrustMin) && (ThrustMax == other.ThrustMax)
      && (WeaponFlags == other.WeaponFlags) && (InventoryIcon == other.InventoryIcon)
      && (EnchantmentID == other.EnchantmentID) && (ScriptID == other.ScriptID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool WeaponRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cWEAP), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
      + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
      + 4 /* MODL */ + 4 /* 4 bytes for length */
      + ModelPath.length() + 1 /* length of path +1 byte for NUL termination */
      + 4 /* WPDT */ + 4 /* 4 bytes for length */ + 32 /* size of data */;
  if (!Name.empty())
  {
    Size = Size + 4 /* FNAM */ + 4 /* 4 bytes for length */
         + Name.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!ScriptID.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
         + ScriptID.length() + 1 /* length of script ID +1 byte for NUL */;
  }
  if (!InventoryIcon.empty())
  {
    Size = Size + 4 /* ITEX */ + 4 /* 4 bytes for length */
         + InventoryIcon.length() + 1 /* length of icon path +1 byte for NUL */;
  }
  if (!EnchantmentID.empty())
  {
    Size = Size + 4 /* ENAM */ + 4 /* 4 bytes for length */
         + EnchantmentID.length() + 1 /* length of ID +1 byte for NUL */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);
  /*Weapons:
    NAME = item ID, required
    MODL = model filename, required
    FNAM = item name
    WPDT = Weapon Data, 0x20 bytes binary, required
        float Weight
        long  Value
        short Type? (0=ShortBladeOneHand,1=LongBladeOneHand,2=LongBladeTwoClose,
                     3=BluntOneHand,4=BluntTwoClose,5=BluntTwoWide,6=SpearTwoWide
                     7=AxeOneHand,8=AxeTwoHand,9=MarksmanBow,10=MarksmanCrossbow
                     11=MarksmanThrown,12= Arrow,13=Bolt)
        short Health
        float Speed
        float Reach
        short EnchantPts
        byte  ChopMin
        byte  ChopMax
        byte  SlashMin
        byte  SlashMax
        byte  ThrustMin
        byte  ThrustMax
        long  Flags (0 to 1) (0 = ?;1 = Ignore Normal Weapon Resistance?)
    ITEX = Iventory icon filename (optional)
    ENAM = Enchantment ID string (optional)
    SCRI = script ID string (optional) */

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
    // write weapon's real name (FNAM)
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = Name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Name.c_str(), SubLength);
  }

  // write WPDT
  output.write(reinterpret_cast<const char*>(&cWPDT), 4);
  SubLength = 32;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write weapon data
  output.write(reinterpret_cast<const char*>(&Weight), 4);
  output.write(reinterpret_cast<const char*>(&Value), 4);
  output.write(reinterpret_cast<const char*>(&Type), 2);
  output.write(reinterpret_cast<const char*>(&Health), 2);
  output.write(reinterpret_cast<const char*>(&Speed), 4);
  output.write(reinterpret_cast<const char*>(&Reach), 4);
  output.write(reinterpret_cast<const char*>(&EnchantPts), 2);
  output.write(reinterpret_cast<const char*>(&ChopMin), 1);
  output.write(reinterpret_cast<const char*>(&ChopMax), 1);
  output.write(reinterpret_cast<const char*>(&SlashMin), 1);
  output.write(reinterpret_cast<const char*>(&SlashMax), 1);
  output.write(reinterpret_cast<const char*>(&ThrustMin), 1);
  output.write(reinterpret_cast<const char*>(&ThrustMax), 1);
  output.write(reinterpret_cast<const char*>(&WeaponFlags), 4);

  if (!ScriptID.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = ScriptID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ScriptID.c_str(), SubLength);
  }

  if (!InventoryIcon.empty())
  {
    // write inventory icon (ITEX)
    output.write(reinterpret_cast<const char*>(&cITEX), 4);
    SubLength = InventoryIcon.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(InventoryIcon.c_str(), SubLength);
  }

  if (!EnchantmentID.empty())
  {
    // write enchantment ID (ENAM)
    output.write(reinterpret_cast<const char*>(&cENAM), 4);
    SubLength = EnchantmentID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(EnchantmentID.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool WeaponRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Weapons:
    NAME = item ID, required
    MODL = model filename, required
    FNAM = weapon name (optional)
    WPDT = Weapon Data (32 bytes), required
        float Weight
        long  Value
        short Type? (0=ShortBladeOneHand,1=LongBladeOneHand,2=LongBladeTwoClose,
                     3=BluntOneHand,4=BluntTwoClose,5=BluntTwoWide,6=SpearTwoWide
                     7=AxeOneHand,8=AxeTwoHand,9=MarksmanBow,10=MarksmanCrossbow
                     11=MarksmanThrown,12= Arrow,13=Bolt)
        short Health
        float Speed
        float Reach
        short EnchantPts
        byte  ChopMin
        byte  ChopMax
        byte  SlashMin
        byte  SlashMax
        byte  ThrustMin
        byte  ThrustMax
        long  Flags (0 to 1) (0 = ?;1 = Ignore Normal Weapon Resistance?)
    ITEX = Iventory icon filename (optional)
    ENAM = Enchantment ID string (optional)
    SCRI = script ID string (optional) */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of WEAP.\n";
    return false;
  }

  // read model path (MODL)
  if (!loadString256WithHeader(input, ModelPath, Buffer, cMODL, BytesRead))
  {
    std::cerr << "Error while reading subrecord MODL of WEAP.\n";
    return false;
  }

  // read FNAM
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName == cFNAM)
  {
    // read weapon's name
    if (!loadString256(input, Name, Buffer, cFNAM, BytesRead))
    {
      std::cerr << "Error while reading subrecord FNAM of WEAP.\n";
      return false;
    }

    //read next subrecord
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
  }
  else
  {
    Name.clear();
  }

  // read WPDT
  // header already read above
  if (SubRecName != cWPDT)
  {
    UnexpectedRecord(cWPDT, SubRecName);
    return false;
  }
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 32)
  {
    std::cerr << "Error: Sub record WPDT of WEAP has invalid length ("
              << SubLength << " bytes). Should be 32 bytes.\n";
    return false;
  }
  // read weapon data
  input.read(reinterpret_cast<char*>(&Weight), 4);
  input.read(reinterpret_cast<char*>(&Value), 4);
  input.read(reinterpret_cast<char*>(&Type), 2);
  input.read(reinterpret_cast<char*>(&Health), 2);
  input.read(reinterpret_cast<char*>(&Speed), 4);
  input.read(reinterpret_cast<char*>(&Reach), 4);
  input.read(reinterpret_cast<char*>(&EnchantPts), 2);
  input.read(reinterpret_cast<char*>(&ChopMin), 1);
  input.read(reinterpret_cast<char*>(&ChopMax), 1);
  input.read(reinterpret_cast<char*>(&SlashMin), 1);
  input.read(reinterpret_cast<char*>(&SlashMax), 1);
  input.read(reinterpret_cast<char*>(&ThrustMin), 1);
  input.read(reinterpret_cast<char*>(&ThrustMax), 1);
  input.read(reinterpret_cast<char*>(&WeaponFlags), 4);
  BytesRead += 32;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord WPDT of WEAP.\n";
    return false;
  }

  // optional data
  ScriptID.clear();
  InventoryIcon.clear();
  EnchantmentID.clear();
  while (BytesRead < Size)
  {
    // read next (optional) record name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cITEX:
           if (!InventoryIcon.empty())
           {
             std::cerr << "Error: WEAP seems to have more than one ITEX subrecord!\n";
             return false;
           }
           // read inventory icon
           if (!loadString256(input, InventoryIcon, Buffer, cITEX, BytesRead))
           {
             std::cerr << "Error while reading subrecord ITEX of WEAP.\n";
             return false;
           }
           if (InventoryIcon.empty())
           {
             std::cerr << "Error: Subrecord ITEX of WEAP is empty!\n";
             return false;
           }
           break;
      case cENAM:
           if (!EnchantmentID.empty())
           {
             std::cerr << "Error: WEAP seems to have more than one ENAM subrecord!\n";
             return false;
           }
           // read enchantment name
           if (!loadString256(input, EnchantmentID, Buffer, cENAM, BytesRead))
           {
             std::cerr << "Error while reading subrecord ENAM of WEAP.\n";
             return false;
           }
           if (EnchantmentID.empty())
           {
             std::cerr << "Error: Subrecord ENAM of WEAP is empty!\n";
             return false;
           }
           break;
      case cSCRI:
           if (!ScriptID.empty())
           {
             std::cerr << "Error: WEAP seems to have more than one SCRI subrecord!\n";
             return false;
           }
           // read script name
           if (!loadString256(input, ScriptID, Buffer, cSCRI, BytesRead))
           {
             std::cerr << "Error while reading subrecord SCRI of WEAP.\n";
             return false;
           }
           if (ScriptID.empty())
           {
             std::cerr << "Error: Subrecord SCRI of WEAP is empty!\n";
             return false;
           }
           break;
      default:
           // unknown / wrong subrecord encountered
           std::cerr << "Error: Expected record name ENAM, ITEX or SCRI was "
                     << "not found. Instead, \"" << IntTo4Char(SubRecName)
                     << "\" was found.\n";
           return false;
           break;
    }
  }

  return true;
}

} // namespace
