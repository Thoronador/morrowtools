/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "WeaponRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

WeaponRecord::WeaponRecord()
{
  WeaponID = ModelPath = Name = "";
  //weapon data
  Weight = 0.0f;
  Value = 0;
  Type = 0;
  Health = 0;
  Speed = 0.0f;
  Reach = 0.0f;
  EnchantPts = 0;
  ChopMin = 0;
  ChopMax = 0;
  SlashMin = 0;
  SlashMax = 0;
  ThrustMin = 0;
  ThrustMax = 0;
  WeaponFlags = 0;
  //end of weapon data
  InventoryIcon = EnchantmentID = ScriptID = "";
}

bool WeaponRecord::equals(const WeaponRecord& other) const
{
  return ((WeaponID==other.WeaponID) and (ModelPath==other.ModelPath)
      and (Name==other.Name) and (Weight==other.Weight) and (Value==other.Value)
      and (Type==other.Type) and (Health==other.Health) and (Speed==other.Speed)
      and (Reach==other.Reach) and (EnchantPts==other.EnchantPts)
      and (ChopMin==other.ChopMin) and (ChopMax==other.ChopMax)
      and (SlashMin==other.SlashMin) and (SlashMax==other.SlashMax)
      and (ThrustMin==other.ThrustMin) and (ThrustMax==other.ThrustMax)
      and (WeaponFlags==other.WeaponFlags) and (InventoryIcon==other.InventoryIcon)
      and (EnchantmentID==other.EnchantmentID) and (ScriptID==other.ScriptID));
}

bool WeaponRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cWEAP, 4);
  int32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +WeaponID.length()+1 /* length of mesh path +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of mesh path +1 byte for NUL termination */
        +4 /* WPDT */ +4 /* 4 bytes for length */ + 32 /* size of weapon data */;
  if (Name!="")
  {
    Size = Size +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of mesh path +1 byte for NUL termination */;
  }
  if (InventoryIcon!="")
  {
    Size = Size +4 /* ITEX */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of mesh path +1 byte for NUL termination */;
  }
  if (EnchantmentID!="")
  {
    Size = Size + 4 /* ENAM */ +4 /* 4 bytes for length */
          +EnchantmentID.length()+1 /* length of enchantment ID +1 byte for NUL termination */;
  }
  if (ScriptID!="")
  {
    Size = Size + 4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptID.length()+1 /* length of script ID +1 byte for NUL termination */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);
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

  //write NAME
  output.write((char*) &cNAME, 4);
  int32_t SubLength = WeaponID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(WeaponID.c_str(), SubLength);

  //write MODL
  output.write((char*) &cMODL, 4);
  SubLength = ModelPath.length()+1;
  //write MODL's length
  output.write((char*) &SubLength, 4);
  //write model path
  output.write(ModelPath.c_str(), SubLength);

  if (Name!="")
  {
    //write FNAM
    output.write((char*) &cFNAM, 4);
    SubLength = Name.length()+1;
    //write FNAM's length
    output.write((char*) &SubLength, 4);
    //write weapon's real name
    output.write(Name.c_str(), SubLength);
  }

  //write WPDT
  output.write((char*) &cWPDT, 4);
  SubLength = 32; //fixed size of 32 bytes
  //write WPDT's length
  output.write((char*) &SubLength, 4);
  //write weapon data
  output.write((char*) &Weight, 4);
  output.write((char*) &Value, 4);
  output.write((char*) &Type, 2);
  output.write((char*) &Health, 2);
  output.write((char*) &Speed, 4);
  output.write((char*) &Reach, 4);
  output.write((char*) &EnchantPts, 2);
  output.write((char*) &ChopMin, 1);
  output.write((char*) &ChopMax, 1);
  output.write((char*) &SlashMin, 1);
  output.write((char*) &SlashMax, 1);
  output.write((char*) &ThrustMin, 1);
  output.write((char*) &ThrustMax, 1);
  output.write((char*) &WeaponFlags, 4);

  if (InventoryIcon!="")
  {
    //write ITEX
    output.write((char*) &cITEX, 4);
    SubLength = InventoryIcon.length()+1;
    //write ITEX's length
    output.write((char*) &SubLength, 4);
    //write inventory icon
    output.write(InventoryIcon.c_str(), SubLength);
  }

  if (EnchantmentID!="")
  {
    //write ENAM
    output.write((char*) &cENAM, 4);
    SubLength = EnchantmentID.length()+1;
    //write ENAM's length
    output.write((char*) &SubLength, 4);
    //write enchantment ID
    output.write(EnchantmentID.c_str(), SubLength);
  }

  if (ScriptID!="")
  {
    //write SCRI
    output.write((char*) &cSCRI, 4);
    SubLength = ScriptID.length()+1;
    //write SCRI's length
    output.write((char*) &SubLength, 4);
    //write script ID
    output.write(ScriptID.c_str(), SubLength);
  }

  return output.good();
}

bool WeaponRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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

  int32_t SubRecName, SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of WEAP is longer than 255 bytes.\n";
    return false;
  }
  //read ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of WEAP.\n";
    return false;
  }
  WeaponID = std::string(Buffer);

  //read MODL
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, SubRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord MODL of WEAP is longer than 255 bytes.\n";
    return false;
  }
  //read model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of WEAP.\n";
    return false;
  }
  ModelPath = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cFNAM)
  {
    //FNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord FNAM of WEAP is longer than 255 bytes.\n";
      return false;
    }
    //read weapon name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord FNAM of WEAP.\n";
      return false;
    }
    Name = std::string(Buffer);

    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }
  else
  {
    Name = "";
  }

  //read WPDT
  //already read above
  if (SubRecName!=cWPDT)
  {
    UnexpectedRecord(cWPDT, SubRecName);
    return false;
  }
  in_File.read((char*) &SubLength, 4);//WPDT's length
  BytesRead += 4;
  if (SubLength!=32)
  {
    std::cout << "Error: sub record WPDT of WEAP has invalid length ("
              <<SubLength<< " bytes). Should be 32 bytes.\n";
    return false;
  }
  //read weapon data
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &Type, 2);
  in_File.read((char*) &Health, 2);
  in_File.read((char*) &Speed, 4);
  in_File.read((char*) &Reach, 4);
  in_File.read((char*) &EnchantPts, 2);
  in_File.read((char*) &ChopMin, 1);
  in_File.read((char*) &ChopMax, 1);
  in_File.read((char*) &SlashMin, 1);
  in_File.read((char*) &SlashMax, 1);
  in_File.read((char*) &ThrustMin, 1);
  in_File.read((char*) &ThrustMax, 1);
  in_File.read((char*) &WeaponFlags, 4);
  BytesRead += 32;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord WPDT of WEAP.\n";
    return false;
  }

  //optional data
  InventoryIcon = "";
  EnchantmentID = "";
  ScriptID = "";
  while (BytesRead<Size)
  {
    //read next (optional) record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cITEX:
           //ITEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ITEX of WEAP is longer than 255 bytes.\n";
             return false;
           }
           //read inventory icon
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ITEX of WEAP.\n";
             return false;
           }
           InventoryIcon = std::string(Buffer);
           break;
      case cENAM:
           //ENAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ENAM of WEAP is longer than 255 bytes.\n";
             return false;
           }
           //read enchantment name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ENAM of WEAP.\n";
             return false;
           }
           EnchantmentID = std::string(Buffer);
           break;
      case cSCRI:
           //SCRI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord SCRI of WEAP is longer than 255 bytes.\n";
             return false;
           }
           //read script name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCRI of WEAP.\n";
             return false;
           }
           ScriptID = std::string(Buffer);
           break;
      default:
           //unknown/ wron subrecord encountered
           std::cout << "Error: expected record name ENAM or SCRI was not "
                     << "found. Instead, \""<<IntTo4Char(SubRecName)
                     <<"\" was found.\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}
