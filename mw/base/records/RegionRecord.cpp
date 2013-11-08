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

#include "RegionRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"
#include "../../../base/UtilityFunctions.h"

namespace MWTP
{

RegionRecord::RegionRecord()
: BasicRecord(),
  recordID(""), RegionName(""),
  Clear(0), Cloudy(0), Foggy(0), Overcast(0), Rain(0), Thunder(0), Ash(0), Blight(0),
  Snow(0), Blizzard(0),
  SleepCreature(""),
  Red(0), Green(0), Blue(0), Zero(0),
  SoundChances(std::vector<SoundChanceRecord>())
{}

RegionRecord::RegionRecord(const std::string& ID)
: BasicRecord()
  recordID(ID),
  RegionName(""),
  Clear(0), Cloudy(0), Foggy(0), Overcast(0), Rain(0), Thunder(0), Ash(0), Blight(0),
  Snow(0), Blizzard(0),
  SleepCreature(""),
  Red(0), Green(0), Blue(0), Zero(0),
  SoundChances(std::vector<SoundChanceRecord>())
{}

RegionRecord::~RegionRecord()
{
  //empty
}

bool RegionRecord::equals(const RegionRecord& other) const
{
  if ((recordID!=other.recordID) or (RegionName!=other.RegionName)
    or (Clear!=other.Clear) or (Cloudy!=other.Cloudy) or (Foggy!=other.Foggy)
    or (Overcast!=other.Overcast) or (Rain!=other.Rain)
    or (Thunder!=other.Thunder) or (Ash!=other.Ash) or (Blight!=other.Blight)
    or (Snow!=other.Snow) or (Blizzard!=other.Blizzard)
    or (SleepCreature!=other.SleepCreature) or (Red!=other.Red)
    or (Green!=other.Green) or (Blue!=other.Blue) or (Zero!=other.Zero)
    or (SoundChances.size()!=other.SoundChances.size()))
 {
   return false;
 }
 unsigned int i;
 for (i=0; i<SoundChances.size(); ++i)
 {
   if ((SoundChances[i].Sound!=other.SoundChances[i].Sound)
       or (SoundChances[i].Chance!=other.SoundChances[i].Chance)) return false;
 }//for
 return true;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool RegionRecord::saveToStream(std::ofstream& output) const
{
  return saveToStream(output, false);
}

bool RegionRecord::saveToStream(std::ofstream& output, const bool forceBloodmoonStyle) const
{
  output.write((const char*) &cREGN, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +RegionName.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* WEAT */ +4 /* WEAT's length */ +8 /*size of WEAT*/;
  if (forceBloodmoonStyle or (Snow!=0) or (Blizzard!=0))
  {
    Size +=2; //We need two additional bytes for Bloodmoon-sytled Weather data.
  }
  if (!SleepCreature.empty())
  {
    Size = Size +4 /* BNAM */ +4 /* 4 bytes for length */
          +SleepCreature.length()+1; /* length of script creature ID +1 byte for NUL termination */;
  }
  Size = Size +4 /* CNAM */ +4 /* 4 bytes for length */ +4 /*length of colour data */
         //add length of sounds
         +SoundChances.size()*(4 /* SNAM */ +4 /* 4 bytes for length */
                               +33 /* length of SNAM record */);
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Regions:
    NAME = Region ID string
    FNAM = Region name string
    WEAT = Weather Data (8 bytes, or 10 in Bloodmoon)
        byte Clear
        byte Cloudy
        byte Foggy
        byte Overcast
        byte Rain
        byte Thunder
        byte Ash
        byte Blight
    BNAM = Sleep creature string (note: optional)
    CNAM = Map Color (4 bytes, COLORREF)
        byte Red
        byte Green
        byte Blue
        byte Null
    SNAM = Sound Record
        byte SoundName[32] (lots of extra junk beyond string?)
        byte Chance
        Multiple records with the order determining the sound priority */

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = recordID.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  SubLength = RegionName.length()+1;//length of string plus one for NUL-termination
  output.write((const char*) &SubLength, 4);
  //write region name
  output.write(RegionName.c_str(), SubLength);

  //write WEAT
  output.write((const char*) &cWEAT, 4);
  //WEAT's length
  if (forceBloodmoonStyle or (Snow!=0) or (Blizzard!=0))
  {
    SubLength = 10; //fixed size is 8 bytes for Bloodmoon-styled weather
  }
  else
  {
    SubLength = 8; //fixed size is 8 bytes for Morrowind/Tribunal-styled weather
  }
  output.write((const char*) &SubLength, 4);
  //write region weather
  output.write((const char*) &Clear, 1);
  output.write((const char*) &Cloudy, 1);
  output.write((const char*) &Foggy, 1);
  output.write((const char*) &Overcast, 1);
  output.write((const char*) &Rain, 1);
  output.write((const char*) &Thunder, 1);
  output.write((const char*) &Ash, 1);
  output.write((const char*) &Blight, 1);
  if (forceBloodmoonStyle or (Snow!=0) or (Blizzard!=0))
  {
    output.write((const char*) &Snow, 1);
    output.write((const char*) &Blizzard, 1);
  }

  if (!SleepCreature.empty())
  {
    //write BNAM
    output.write((const char*) &cBNAM, 4);
    //BNAM's length
    SubLength = SleepCreature.length()+1;//length of string plus one for NUL-termination
    output.write((const char*) &SubLength, 4);
    //write sleep creature
    output.write(SleepCreature.c_str(), SubLength);
  }

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  SubLength = 4;//fixed size is four bytes
  output.write((const char*) &SubLength, 4);
  //write region map colour
  output.write((const char*) &Red, 1);
  output.write((const char*) &Green, 1);
  output.write((const char*) &Blue, 1);
  output.write((const char*) &Zero, 1);

  //write sound chances
  unsigned int i, len;
  for (i=0; i<SoundChances.size(); ++i)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    SubLength = 33;//fixed size is 33 bytes - 32 for name and one for chance
    output.write((const char*) &SubLength, 4);
    //write sound's name/ID
    /* The sound name get's truncated here, if it's longer than 31 characters. */
    len = SoundChances.at(i).Sound.length()+1;
    if (len>32)
    {
      len=32;
      std::cout << "RegionRecord::saveToStream: Warning: sound name of region \""
                << recordID<< "\" got truncated for index "<<i<<".\n";
    }
    output.write(SoundChances.at(i).Sound.c_str(), len);
    if (len<32)
    {
      //fill rest with zeros
      output.write(NULof32, 32-len);
    }
    //write chance
    output.write((const char*) &(SoundChances.at(i).Chance), 1);
  }//for

  return output.good();
}
#endif

bool RegionRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Regions:
    NAME = Region ID string
    FNAM = Region name string
    WEAT = Weather Data (8 bytes, or 10 in Bloodmoon)
        byte Clear
        byte Cloudy
        byte Foggy
        byte Overcast
        byte Rain
        byte Thunder
        byte Ash
        byte Blight
    BNAM = Sleep creature string (note: optional)
    CNAM = Map Color (4 bytes, COLORREF)
        byte Red
        byte Green
        byte Blue
        byte Null
    SNAM = Sound Record
        byte SoundName[32] (lots of extra junk beyond string?)
        byte Chance
        Multiple records with the order determining the sound priority */

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
    std::cout << "Error: subrecord NAME of REGN is longer than 255 characters.\n";
    return false;
  }
  //read region ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of REGN.\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord FNAM of REGN is longer than 255 characters.\n";
    return false;
  }
  //read region name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of REGN.\n";
    return false;
  }
  RegionName = std::string(Buffer);

  //read WEAT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cWEAT)
  {
    UnexpectedRecord(cWEAT, SubRecName);
    return false;
  }
  //WEAT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if ((SubLength!=8) && (SubLength!=10))
  {
    std::cout << "Error: sub record WEAT of REGN has invalid length ("
              <<SubLength<< " bytes). Should be 8 or 10 bytes.\n";
    return false;
  }//if
  //read region weather
  in_File.read((char*) &Clear, 1);
  in_File.read((char*) &Cloudy, 1);
  in_File.read((char*) &Foggy, 1);
  in_File.read((char*) &Overcast, 1);
  in_File.read((char*) &Rain, 1);
  in_File.read((char*) &Thunder, 1);
  in_File.read((char*) &Ash, 1);
  in_File.read((char*) &Blight, 1);
  BytesRead += 8;
  //check for BM chances
  if (SubLength==10)
  {
    in_File.read((char*) &Snow, 1);
    in_File.read((char*) &Blizzard, 1);
    BytesRead += 2;
  }
  else
  {
    Snow = 0;
    Blizzard = 0;
  }
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord WEAT of REGN.\n";
    return false;
  }

  //read BNAM (optional)
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName==cBNAM)
  {
    //BNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>255)
    {
      std::cout << "Error: subrecord BNAM of REGN is longer than 255 characters.\n";
      return false;
    }
    //read sleep creature name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord BNAM of REGN.\n";
      return false;
    }
    SleepCreature = std::string(Buffer);
    //read next record name
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
  }
  else
  {
    SleepCreature = "";
  }

  //read CNAM
  if (SubRecName!=cCNAM)
  {
    UnexpectedRecord(cCNAM, SubRecName);
    return false;
  }
  //CNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout << "Error: sub record CNAM of REGN has invalid length ("
              <<SubLength<< " bytes). Should be four bytes.\n";
    return false;
  }//if
  //read colour values
  in_File.read((char*) &Red, 1);
  in_File.read((char*) &Green, 1);
  in_File.read((char*) &Blue, 1);
  in_File.read((char*) &Zero, 1);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CNAM of REGN.\n";
    return false;
  }

  SoundChances.clear();
  //read multiple SNAM
  while (BytesRead<Size)
  {
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cSNAM)
    {
      UnexpectedRecord(cSNAM, SubRecName);
      return false;
    }
    //SNAM's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=33)
    {
      std::cout << "Error: subrecord BNAM of REGN has invalid length ("
                << SubLength << " bytes). Should be 33 bytes.\n";
      return false;
    }
    //read sound name
    memset(Buffer, '\0', 33);
    in_File.read(Buffer, 32); //only the first 32 bytes are the sound's name
    BytesRead += 32;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord SNAM of REGN (1st part).\n";
      return false;
    }
    SoundChanceRecord temp;
    temp.Sound = std::string(Buffer);
    //read chance
    in_File.read((char*) &(temp.Chance), 1);
    BytesRead += 1;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord SNAM of REGN (2nd part).\n";
      return false;
    }
    //push the read record data onto the end of the vector
    SoundChances.push_back(temp);
  }//while
  return in_File.good();
}

bool operator<(const RegionRecord& left, const RegionRecord& right)
{
  return (lowerCaseCompare(left.recordID, right.recordID)<0);
}

} //namespace
