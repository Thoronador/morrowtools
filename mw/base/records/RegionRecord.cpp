/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2022  Dirk Stolle

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

#include "RegionRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"
#include "../../../base/UtilityFunctions.hpp"

namespace MWTP
{

SoundChanceRecord::SoundChanceRecord()
: Sound(""),
  Chance(0)
{
}

bool SoundChanceRecord::operator==(const SoundChanceRecord& other) const
{
  return (Sound == other.Sound) && (Chance == other.Chance);
}

RegionRecord::RegionRecord()
: BasicRecord(),
  recordID(""),
  RegionName(""),
  Clear(0), Cloudy(0), Foggy(0), Overcast(0),
  Rain(0), Thunder(0), Ash(0), Blight(0),
  Snow(0), Blizzard(0),
  SleepCreature(""),
  Red(0), Green(0), Blue(0), Zero(0),
  SoundChances(std::vector<SoundChanceRecord>())
{}

RegionRecord::RegionRecord(const std::string& ID)
: BasicRecord(),
  recordID(ID),
  RegionName(""),
  Clear(0), Cloudy(0), Foggy(0), Overcast(0),
  Rain(0), Thunder(0), Ash(0), Blight(0),
  Snow(0), Blizzard(0),
  SleepCreature(""),
  Red(0), Green(0), Blue(0), Zero(0),
  SoundChances(std::vector<SoundChanceRecord>())
{}

bool RegionRecord::equals(const RegionRecord& other) const
{
  return (recordID == other.recordID) && (RegionName == other.RegionName)
      && (Clear == other.Clear) && (Cloudy == other.Cloudy) && (Foggy == other.Foggy)
      && (Overcast == other.Overcast) && ( Rain == other.Rain)
      && (Thunder == other.Thunder) && (Ash == other.Ash) && (Blight == other.Blight)
      && (Snow == other.Snow) && (Blizzard == other.Blizzard)
      && (SleepCreature == other.SleepCreature) && (Red == other.Red)
      && (Green == other.Green) && (Blue == other.Blue) && (Zero == other.Zero)
      && (SoundChances == other.SoundChances);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool RegionRecord::saveToStream(std::ostream& output) const
{
  return saveToStream(output, false);
}

bool RegionRecord::saveToStream(std::ostream& output, const bool forceBloodmoonStyle) const
{
  output.write(reinterpret_cast<const char*>(&cREGN), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
                + recordID.length() + 1 /* length of ID +1 byte for NUL */
                + 4 /* FNAM */ + 4 /* 4 bytes for length */
                + RegionName.length() + 1 /* length of name +1 byte for NUL */
                + 4 /* WEAT */ + 4 /* WEAT's length */ + 8 /*size of WEAT*/
                + 4 /* CNAM */ + 4 /* 4 bytes for length */
                + 4 /* length of colour data */
                // add length of sounds
                + (4 /* SNAM */ + 4 /* 4 bytes for length */
                   + 33 /* length of SNAM record */) * SoundChances.size();
  if (forceBloodmoonStyle || (Snow != 0) || (Blizzard != 0))
  {
    // We need two additional bytes for Bloodmoon-styled Weather data.
    Size +=2;
  }
  if (!SleepCreature.empty())
  {
    Size += 4 /* BNAM */ + 4 /* 4 bytes for length */
          + SleepCreature.length() + 1; /* length of ID +1 byte for NUL */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write region ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write region name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = RegionName.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(RegionName.c_str(), SubLength);

  // write weather data (WEAT)
  output.write(reinterpret_cast<const char*>(&cWEAT), 4);
  //WEAT's length
  if (forceBloodmoonStyle || (Snow != 0) || (Blizzard != 0))
  {
    SubLength = 10; // 10 bytes for Bloodmoon-styled weather
  }
  else
  {
    SubLength = 8; // 8 bytes for Morrowind/Tribunal-styled weather
  }
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write region weather
  output.write(reinterpret_cast<const char*>(&Clear), 1);
  output.write(reinterpret_cast<const char*>(&Cloudy), 1);
  output.write(reinterpret_cast<const char*>(&Foggy), 1);
  output.write(reinterpret_cast<const char*>(&Overcast), 1);
  output.write(reinterpret_cast<const char*>(&Rain), 1);
  output.write(reinterpret_cast<const char*>(&Thunder), 1);
  output.write(reinterpret_cast<const char*>(&Ash), 1);
  output.write(reinterpret_cast<const char*>(&Blight), 1);
  if (forceBloodmoonStyle || (Snow != 0) || (Blizzard != 0))
  {
    output.write(reinterpret_cast<const char*>(&Snow), 1);
    output.write(reinterpret_cast<const char*>(&Blizzard), 1);
  }

  if (!SleepCreature.empty())
  {
    // write sleep creature ID (BNAM)
    output.write(reinterpret_cast<const char*>(&cBNAM), 4);
    SubLength = SleepCreature.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(SleepCreature.c_str(), SubLength);
  }

  // write region map colour (CNAM)
  output.write(reinterpret_cast<const char*>(&cCNAM), 4);
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Red), 1);
  output.write(reinterpret_cast<const char*>(&Green), 1);
  output.write(reinterpret_cast<const char*>(&Blue), 1);
  output.write(reinterpret_cast<const char*>(&Zero), 1);

  // write sound chances
  for (const auto& element: SoundChances)
  {
    // write SNAM
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    SubLength = 33; // 32 bytes for name and one for chance
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    // write sound's name / ID
    /* The sound name gets truncated here, if it's longer than 31 characters. */
    auto len = element.Sound.length() + 1;
    if (len > 32)
    {
      len = 32;
      std::cout << "RegionRecord::saveToStream: Warning: Sound name of region \""
                << recordID<< "\" got truncated.\n";
    }
    output.write(element.Sound.c_str(), len);
    if (len < 32)
    {
      // fill rest with zeros
      output.write(NULof32, 32 - len);
    }
    // write chance
    output.write(reinterpret_cast<const char*>(&element.Chance), 1);
  }

  return output.good();
}
#endif

bool RegionRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read region ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of REGN.\n";
    return false;
  }

  // read translated region name (FNAM)
  if (!loadString256WithHeader(input, RegionName, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading sub record FNAM of REGN.\n";
    return false;
  }
  RegionName = std::string(Buffer);

  // read weather chances (WEAT)
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cWEAT)
  {
    UnexpectedRecord(cWEAT, SubRecName);
    return false;
  }
  // WEAT's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if ((SubLength != 8) && (SubLength != 10))
  {
    std::cerr << "Error: Sub record WEAT of REGN has invalid length ("
              << SubLength << " bytes). Should be 8 or 10 bytes.\n";
    return false;
  }
  // read region weather
  input.read(reinterpret_cast<char*>(&Clear), 1);
  input.read(reinterpret_cast<char*>(&Cloudy), 1);
  input.read(reinterpret_cast<char*>(&Foggy), 1);
  input.read(reinterpret_cast<char*>(&Overcast), 1);
  input.read(reinterpret_cast<char*>(&Rain), 1);
  input.read(reinterpret_cast<char*>(&Thunder), 1);
  input.read(reinterpret_cast<char*>(&Ash), 1);
  input.read(reinterpret_cast<char*>(&Blight), 1);
  BytesRead += 8;
  // check for BM chances
  if (SubLength == 10)
  {
    input.read(reinterpret_cast<char*>(&Snow), 1);
    input.read(reinterpret_cast<char*>(&Blizzard), 1);
    BytesRead += 2;
  }
  else
  {
    Snow = 0;
    Blizzard = 0;
  }
  if (!input.good())
  {
    std::cerr << "Error while reading sub record WEAT of REGN.\n";
    return false;
  }

  // read BNAM (optional)
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName == cBNAM)
  {
    // read sleep creature name (BNAM)
    if (!loadString256(input, SleepCreature, Buffer, cBNAM, BytesRead))
    {
      std::cerr << "Error while reading sub record BNAM of REGN.\n";
      return false;
    }
    // read next record name
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
  }
  else
  {
    SleepCreature = "";
  }

  // read CNAM
  if (SubRecName != cCNAM)
  {
    UnexpectedRecord(cCNAM, SubRecName);
    return false;
  }
  // CNAM's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record CNAM of REGN has invalid length ("
              << SubLength << " bytes). Should be four bytes.\n";
    return false;
  }
  // read colour values
  input.read(reinterpret_cast<char*>(&Red), 1);
  input.read(reinterpret_cast<char*>(&Green), 1);
  input.read(reinterpret_cast<char*>(&Blue), 1);
  input.read(reinterpret_cast<char*>(&Zero), 1);
  BytesRead += 4;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record CNAM of REGN.\n";
    return false;
  }

  SoundChances.clear();
  // read multiple SNAM
  while (BytesRead < Size)
  {
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    if (SubRecName != cSNAM)
    {
      UnexpectedRecord(cSNAM, SubRecName);
      return false;
    }
    // SNAM's length
    input.read(reinterpret_cast<char*>(&SubLength), 4);
    BytesRead += 4;
    if (SubLength != 33)
    {
      std::cerr << "Error: Sub record BNAM of REGN has invalid length ("
                << SubLength << " bytes). Should be 33 bytes.\n";
      return false;
    }
    // read sound name
    memset(Buffer, '\0', 33);
    input.read(Buffer, 32); // only the first 32 bytes are the sound's name
    BytesRead += 32;
    if (!input.good())
    {
      std::cerr << "Error while reading sub record SNAM of REGN (1st part).\n";
      return false;
    }
    SoundChanceRecord temp;
    temp.Sound = std::string(Buffer);
    // read chance
    input.read(reinterpret_cast<char*>(&temp.Chance), 1);
    BytesRead += 1;
    if (!input.good())
    {
      std::cerr << "Error while reading sub record SNAM of REGN (2nd part).\n";
      return false;
    }
    // push the read record data onto the end of the vector
    SoundChances.emplace_back(temp);
  }

  return true;
}

bool operator<(const RegionRecord& left, const RegionRecord& right)
{
  return lowerCaseCompare(left.recordID, right.recordID) < 0;
}

} // namespace
