/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013  Thoronador

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
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

AlchemyPotionRecord::AlchemyPotionRecord()
:  BasicRecord(),
  recordID(""),
  ModelPath(""),
  Name(""),
  //alchemy data
  Weight(0.0f),
  Value(-1),
  AutoCalc(0),
  //end alchemy data
  Enchs(std::vector<EnchantmentData>()),
  InventoryIcon(""),
  ScriptName("")
{
}

bool AlchemyPotionRecord::equals(const AlchemyPotionRecord& other) const
{
  if ((recordID!=other.recordID) or (ModelPath!=other.ModelPath)
   or (Name!=other.Name) or (Weight!=other.Weight) or (Value!=other.Value)
   or (AutoCalc!=other.AutoCalc) or (Enchs!=other.Enchs)
   or (InventoryIcon!=other.InventoryIcon) or (ScriptName!=other.ScriptName))
  {
    return false;
  }
  return true;
}

bool AlchemyPotionRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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
        long  Unknown1
        long  Unknown2
        long  Duration
        long  Magnitude
        long  Unknown4
    TEXT = Inventory Icon
    SCRI = Script Name (optional) */

  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
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
    std::cout << "Error: subrecord NAME of ALCH is longer than 255 characters.\n";
    return false;
  }
  //read potion ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of ALCH.\n";
    return false;
  }
  recordID = std::string(Buffer);

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
    std::cout << "Error: subrecord MODL of ALCH is longer than 255 characters.\n";
    return false;
  }
  //read potion's model path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of ALCH.\n";
    return false;
  }
  ModelPath = std::string(Buffer);

  //reset data that still needs to be read
  Enchs.clear();
  InventoryIcon = Name = ScriptName = "";

  //read data
  bool Success = false;
  while (BytesRead<Size)
  {
    //read next subrecord's name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cALDT:
           Success = readSubRecordALDT(in_File, Buffer, BytesRead);
           break;
      case cENAM:
           Success = readSubRecordENAM(in_File, Buffer, BytesRead);
           break;
      case cFNAM:
           Success = readSubRecordFNAM(in_File, Buffer, BytesRead);
           break;
      case cTEXT:
           Success = readSubRecordTEXT(in_File, Buffer, BytesRead);
           break;
      case cSCRI:
           Success = readSubRecordSCRI(in_File, Buffer, BytesRead);
           break;
      default:
           //other sub record name means error
           std::cout << "AlchemyPotionRecord: Error: expected record name ALDT,"
                     << "ENAM, FNAM, TEXT or SCRI was not found. Instead, \""
                     <<IntTo4Char(SubRecName)<<"\" was found.\nPosition is "
                     <<in_File.tellg()<<".\n";
           return false;
    }//swi
    if (!Success)
    {
      //An error occurred, quit here.
      return false;
    }//if
  }//while
  return in_File.good();
}

#ifndef MW_UNSAVEABLE_RECORDS
bool AlchemyPotionRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cALCH, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* MODL */ +4 /* 4 bytes for length */
        +ModelPath.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +Name.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* ALDT */ +4 /* ALDT's length */ +12 /*size of alchemy data (ALDT)*/
        //enchantments
        +Enchs.size()*(4 /* ENAM */ +4 /* ENAM's length */ +24 /*size of enchantment*/)
        +4 /* TEXT */ +4 /* 4 bytes for length */
        +InventoryIcon.length()+1 /* length of ID +1 byte for NUL termination */;
  if (!ScriptName.empty())
  {
    Size = Size +4 /* SCRI */ +4 /* 4 bytes for length */
          +ScriptName.length()+1; /* length of script ID +1 byte for NUL termination */
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

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
        long  Unknown1
        long  Unknown2
        long  Duration
        long  Magnitude
        long  Unknown4
    TEXT = Inventory Icon
    SCRI = Script Name (optional) */

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = recordID.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  SubLength = ModelPath.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write model path
  output.write(ModelPath.c_str(), SubLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  SubLength = Name.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write potion name
  output.write(Name.c_str(), SubLength);

  //write ALDT
  output.write((const char*) &cALDT, 4);
  //ALDT's length
  SubLength = 12; //length of ALDT (alchemy data) is always 12 bytes
  output.write((const char*) &SubLength, 4);
  // write alchemy data
  output.write((const char*) &Weight, 4);
  output.write((const char*) &Value, 4);
  output.write((const char*) &AutoCalc, 4);

  // write enchantments
  unsigned int i;
  for(i=0; i<Enchs.size(); ++i)
  {
    // write ENAM
    output.write((const char*) &cENAM, 4);
    // ENAM's length
    SubLength = 24; // length of enchantment data is always 24 bytes
    output.write((const char*) &SubLength, 4);
    // write enchantment data
    output.write((const char*) &(Enchs[i].EffectID), 2);
    output.write((const char*) &(Enchs[i].SkillID), 1);
    output.write((const char*) &(Enchs[i].AttributeID), 1);
    output.write((const char*) &(Enchs[i].RangeType), 4);
    output.write((const char*) &(Enchs[i].Area), 4);
    output.write((const char*) &(Enchs[i].Duration), 4);
    output.write((const char*) &(Enchs[i].MagnitudeMin), 4);
    output.write((const char*) &(Enchs[i].MagnitudeMax), 4);
  }//for
  //write TEXT
  output.write((const char*) &cTEXT, 4);
  //TEXT's length
  SubLength = InventoryIcon.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write inventory icon
  output.write(InventoryIcon.c_str(), SubLength);

  if (!ScriptName.empty())
  {
    //write SCRI
    output.write((const char*) &cSCRI, 4);
    //SCRI's length
    SubLength = ScriptName.length()+1;//length of string plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write script name
    output.write(ScriptName.c_str(), SubLength);
  }
  return output.good();
}
#endif

bool AlchemyPotionRecord::readSubRecordALDT(std::istream& in_File, char* Buffer, uint32_t& BytesRead)
{
  uint32_t SubLength = 0;
  //ALDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=12)
  {
    std::cout <<"Error: sub record ALDT of ALCH has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read data of potion
  in_File.read((char*) &Weight, 4);
  in_File.read((char*) &Value, 4);
  in_File.read((char*) &AutoCalc, 4);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ALDT of ALCH.\n";
    return false;
  }
  return true;
}

bool AlchemyPotionRecord::readSubRecordENAM(std::istream& in_File, char* Buffer, uint32_t& BytesRead)
{
  //ENAM's length
  uint32_t SubLength = 0;
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=24)
  {
    std::cout <<"Error: sub record ENAM of ALCH has invalid length ("<<SubLength
              <<" bytes). Should be 24 bytes.\n";
    return false;
  }
  //read potion enchantment
  EnchantmentData temp;
  in_File.read((char*) &(temp.EffectID), 2);
  in_File.read((char*) &(temp.SkillID), 1);
  in_File.read((char*) &(temp.AttributeID), 1);
  in_File.read((char*) &(temp.RangeType), 4);
  in_File.read((char*) &(temp.Area), 4);
  in_File.read((char*) &(temp.Duration), 4);
  in_File.read((char*) &(temp.MagnitudeMin), 4);
  in_File.read((char*) &(temp.MagnitudeMax), 4);
  BytesRead += 24;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ENAM of ALCH.\n";
    return false;
  }
  Enchs.push_back(temp);
  return true;
}

bool AlchemyPotionRecord::readSubRecordFNAM(std::istream& in_File, char* Buffer, uint32_t& BytesRead)
{
  //FNAM's length
  uint32_t SubLength = 0;
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord FNAM of ALCH is longer than 255 characters.\n";
    return false;
  }
  //read potion's "real" name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of ALCH!\n";
    return false;
  }
  Name = std::string(Buffer);
  return true;
}

bool AlchemyPotionRecord::readSubRecordSCRI(std::istream& in_File, char* Buffer, uint32_t& BytesRead)
{
  //SCRI's length
  uint32_t SubLength = 0;
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord SCRI of ALCH is longer than 255 characters.\n";
    return false;
  }
  //read potion script ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord SCRI of ALCH.\n";
    return false;
  }
  ScriptName = std::string(Buffer);
  return true;
}

bool AlchemyPotionRecord::readSubRecordTEXT(std::istream& in_File, char* Buffer, uint32_t& BytesRead)
{
  uint32_t SubLength = 0;
  //TEXT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord TEXT of ALCH is longer than 255 characters.\n";
    return false;
  }
  //read potion icon path
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord TEXT of ALCH.\n";
    return false;
  }
  InventoryIcon = std::string(Buffer);
  return true;
}

} //namespace
