/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012  Thoronador

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

#include "EnchantingRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

EnchantingRecord::EnchantingRecord()
: BasicRecord(),
  recordID(""),
  //enchanting data
  Type(-1),
  EnchantCost(-1),
  Charge(0),
  AutoCalc(0),
  //end of enchanting data
  Enchs(std::vector<EnchantmentData>())
{}

  /* destructor */
EnchantingRecord::~EnchantingRecord()
{

}

bool EnchantingRecord::equals(const EnchantingRecord& other) const
{
  return ((recordID==other.recordID) and (Type==other.Type)
    and (EnchantCost==other.EnchantCost) and (Charge==other.Charge)
    and (AutoCalc==other.AutoCalc) and (Enchs==other.Enchs));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool EnchantingRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cENCH, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* ENDT */ +4 /* ENDT's length */ +16 /*size of ENDT (always 16 bytes)*/
        + Enchs.size()*( 4 /* ENAM */ +4 /* 4 bytes for length */
        +24 /* length of ENAM is always 24 bytes */);
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Enchantment:
    NAME = ID string
    ENDT = Enchant Data (16 bytes)
        long Type(0=Cast Once,1=Cast Strikes,2=Cast when Used,3=Constant Effect)
        long EnchantCost
        long Charge
        long AutoCalc
    ENAM = Single enchantment data (24 bytes), can occur multiple times
        short EffectID
        byte  SkillID     (-1 if NA)
        byte  AttributeID (-1 if NA)
        long  RangeType (0 = Self,1 = Touch,2 = Target)
        long  Area
        long  Duration
        long  MagMin
        long  MagMax */

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = recordID.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write ENDT
  output.write((const char*) &cENDT, 4);
  //ENDT's length
  SubLength = 16;//fixed size, always 16 bytes
  output.write((const char*) &SubLength, 4);
  //write enchanting data
  output.write((const char*) &Type, 4);
  output.write((const char*) &EnchantCost, 4);
  output.write((const char*) &Charge, 4);
  output.write((const char*) &AutoCalc, 4);

  unsigned int i;
  for (i=0; i<Enchs.size(); ++i)
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    //ENAM's length
    SubLength = 24;//fixed size, always 24 bytes
    output.write((const char*) &SubLength, 4);
    //write effect data
    output.write((const char*) &(Enchs[i].EffectID), 2);
    output.write((const char*) &(Enchs[i].SkillID), 1);
    output.write((const char*) &(Enchs[i].AttributeID), 1);
    output.write((const char*) &(Enchs[i].Range), 4);
    output.write((const char*) &(Enchs[i].Area), 4);
    output.write((const char*) &(Enchs[i].Duration), 4);
    output.write((const char*) &(Enchs[i].MagnitudeMin), 4);
    output.write((const char*) &(Enchs[i].MagnitudeMax), 4);
  }//for
  return output.good();
}
#endif

bool EnchantingRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Enchantment:
    NAME = ID string
    ENDT = Enchant Data (16 bytes)
        long Type(0=Cast Once,1=Cast Strikes,2=Cast when Used,3=Constant Effect)
        long EnchantCost
        long Charge
        long AutoCalc
    ENAM = Single enchantment data (24 bytes), can occur multiple times
        short EffectID
        byte  SkillID     (-1 if NA)
        byte  AttributeID (-1 if NA)
        long  RangeType (0 = Self,1 = Touch,2 = Target)
        long  Area
        long  Duration
        long  MagMin
        long  MagMax */

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
    std::cout << "Error: subrecord NAME of ENCH is longer than 255 characters.\n";
    return false;
  }
  //read enchantment ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of ENCH.\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read ENDT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cENDT)
  {
    UnexpectedRecord(cENDT, SubRecName);
    return false;
  }
  //ENDT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=16)
  {
    std::cout <<"Error: sub record ENDT of ENCH has invalid length ("
             <<SubLength<<" bytes). Should be 16 bytes.\n";
    return false;
  }
  //read enchantment data
  in_File.read((char*) &Type, 4);
  in_File.read((char*) &EnchantCost, 4);
  in_File.read((char*) &Charge, 4);
  in_File.read((char*) &AutoCalc, 4);
  BytesRead += 16;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ENDT of ENCH.\n";
    return false;
  }

  //read multiple ENAMs
  Enchs.clear();
  EnchantmentData temp;
  while (BytesRead<Size)
  {
    //check for size limit - no enchantment can have more than eight effects
    if (Enchs.size()>=8)
    {
      std::cout << "Error while reading subrecord ENAM of ENCH. Enchanting "
                << "has already 8 effects, but there are still more to read. "
                << "However, no enchanting can have more than 8 effects.\n";
      return false;
    }
    //read ENAM
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cENAM)
    {
      UnexpectedRecord(cENAM, SubRecName);
      return false;
    }
    //ENAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=24)
    {
      std::cout <<"Error: sub record ENAM of ENCH has invalid length ("
               <<SubLength<<" bytes). Should be 24 bytes.\n";
      return false;
    }
    //read enchantment data
    in_File.read((char*) &(temp.EffectID), 2);
    in_File.read((char*) &(temp.SkillID), 1);
    in_File.read((char*) &(temp.AttributeID), 1);
    in_File.read((char*) &(temp.Range), 4);
    in_File.read((char*) &(temp.Area), 4);
    in_File.read((char*) &(temp.Duration), 4);
    in_File.read((char*) &(temp.MagnitudeMin), 4);
    in_File.read((char*) &(temp.MagnitudeMax), 4);
    BytesRead += 24;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord ENAM of ENCH.\n";
      return false;
    }
    //add the read data to enchantment effects
    Enchs.push_back(temp);
  }//while

  return in_File.good();
}

} //namespace
