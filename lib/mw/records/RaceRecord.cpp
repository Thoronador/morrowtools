/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2023  Dirk Stolle

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

#include "RaceRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"
#include "../../base/UtilityFunctions.hpp"

namespace MWTP
{

const uint32_t rfPlayable = 0x00000001;
const uint32_t rfBeastRace = 0x00000002;

bool SkillBonus::operator==(const SkillBonus& other) const
{
  return (SkillID == other.SkillID) && (Bonus == other.Bonus);
}

RaceRecord::RaceRecord()
: BasicRecord(),
  recordID(""), Name(""),
  // race data
  Boni(std::vector<SkillBonus>()),
  Strength{0, 0},
  Intelligence{0, 0},
  Willpower{0, 0},
  Agility{0, 0},
  Speed{0, 0},
  Endurance{0, 0},
  Personality{0, 0},
  Luck{0, 0},
  Height{1.0f, 1.0f},
  Weight{0.0f, 0.0f},
  Flags(0),
  // end of race data
  Powers(std::vector<std::string>()),
  Description("")
{
}

RaceRecord::RaceRecord(const std::string& ID)
: BasicRecord(),
  recordID(ID),
  Name(""),
  // race data
  Boni(std::vector<SkillBonus>()),
  Strength{0, 0},
  Intelligence{0, 0},
  Willpower{0, 0},
  Agility{0, 0},
  Speed{0, 0},
  Endurance{0, 0},
  Personality{0, 0},
  Luck{0, 0},
  Height{1.0f, 1.0f},
  Weight{0.0f, 0.0f},
  Flags(0),
  // end of race data
  Powers(std::vector<std::string>()),
  Description("")
{
}

bool RaceRecord::equals(const RaceRecord& other) const
{
  return (recordID == other.recordID) && (Name == other.Name)
    && (Boni == other.Boni) && (Description == other.Description)
    && (Strength[0] == other.Strength[0]) && (Strength[1] == other.Strength[1])
    && (Intelligence[0] == other.Intelligence[0]) && (Intelligence[1] == other.Intelligence[1])
    && (Willpower[0] == other.Willpower[0]) && (Willpower[1] == other.Willpower[1])
    && (Agility[0] == other.Agility[0]) && (Agility[1] == other.Agility[1])
    && (Speed[0] == other.Speed[0]) && (Speed[1] == other.Speed[1])
    && (Endurance[0] == other.Endurance[0]) && (Endurance[1] == other.Endurance[1])
    && (Personality[0] == other.Personality[0]) && (Personality[1] == other.Personality[1])
    && (Luck[0] == other.Luck[0]) && (Luck[1] == other.Luck[1])
    && (Weight[0] == other.Weight[0]) && (Weight[1] == other.Weight[1])
    && (Height[0] == other.Height[0]) && (Height[1] == other.Height[1])
    && (Flags == other.Flags) && (Powers == other.Powers);
}

bool RaceRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Race Definition:
    NAME = Race ID string
    FNAM = Race name string
    RADT = Race data (140 bytes)
        SkillBonuses[7]
            long SkillID
            long Bonus
        long Strength[2]     (Male/Female)
        long Intelligence[2]
        long Willpower[2]
        long Agility[2]
        long Speed[2]
        long Endurance[2]
        long Personality[2]
        long Luck[2]
        float Height[2]
        float Weight[2]
        long Flags (1 = Playable,2 = Beast Race)
    NPCS = Special power/ability name string (32 bytes), occurs 0+ times
    DESC = Race description (optional) */

  uint32_t BytesRead = 0;

  // read record ID (NAME)
  char Buffer[1024]; // so long because of description text
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of RACE.\n";
    return false;
  }

  // read localized name (FNAM)
  if (!loadString256WithHeader(input, Name, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading subrecord FNAM of RACE.\n";
    return false;
  }

  // read RADT
  uint32_t SubRecName = 0;
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cRADT)
  {
    UnexpectedRecord(cRADT, SubRecName);
    return false;
  }
  // RADT's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 140)
  {
    std::cerr << "Error: Sub record RADT of RACE has invalid length ("
              << SubLength << " bytes). Should be 140 bytes.\n";
    return false;
  }
  // read race data
  // ---- boni first, 7 times
  SkillBonus sb;
  Boni.clear();
  for (unsigned int i = 0; i < 7; ++i)
  {
    input.read(reinterpret_cast<char*>(&(sb.SkillID)), 4);
    input.read(reinterpret_cast<char*>(&(sb.Bonus)), 4);
    Boni.push_back(sb);
  }
  // ---- attributes
  input.read(reinterpret_cast<char*>(&(Strength[0])), 4);
  input.read(reinterpret_cast<char*>(&(Strength[1])), 4);
  input.read(reinterpret_cast<char*>(&(Intelligence[0])), 4);
  input.read(reinterpret_cast<char*>(&(Intelligence[1])), 4);
  input.read(reinterpret_cast<char*>(&(Willpower[0])), 4);
  input.read(reinterpret_cast<char*>(&(Willpower[1])), 4);
  input.read(reinterpret_cast<char*>(&(Agility[0])), 4);
  input.read(reinterpret_cast<char*>(&(Agility[1])), 4);
  input.read(reinterpret_cast<char*>(&(Speed[0])), 4);
  input.read(reinterpret_cast<char*>(&(Speed[1])), 4);
  input.read(reinterpret_cast<char*>(&(Endurance[0])), 4);
  input.read(reinterpret_cast<char*>(&(Endurance[1])), 4);
  input.read(reinterpret_cast<char*>(&(Personality[0])), 4);
  input.read(reinterpret_cast<char*>(&(Personality[1])), 4);
  input.read(reinterpret_cast<char*>(&(Luck[0])), 4);
  input.read(reinterpret_cast<char*>(&(Luck[1])), 4);
  // ---- height
  input.read(reinterpret_cast<char*>(&(Height[0])), 4);
  input.read(reinterpret_cast<char*>(&(Height[1])), 4);
  // ---- weight
  input.read(reinterpret_cast<char*>(&(Weight[0])), 4);
  input.read(reinterpret_cast<char*>(&(Weight[1])), 4);
  // ---- flags
  input.read(reinterpret_cast<char*>(&Flags), 4);
  BytesRead += 140;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record RADT of RACE.\n";
    return false;
  }

  // read multiple NPCS
  Powers.clear();
  Description.clear();
  bool hasDESC = false;
  while (BytesRead < Size)
  {
    // read NPCS or DESC
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cNPCS:
           // NPCS's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 32)
           {
             std::cerr << "Error: Sub record NPCS of RACE has invalid length ("
                       << SubLength << " bytes). Should be 32 bytes.\n";
             return false;
           }
           // read spell ID
           std::memset(Buffer, '\0', 256);
           input.read(Buffer, 32);
           BytesRead += 32;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record NPCS of RACE.\n";
             return false;
           }
           // add spell ID
           Powers.push_back(std::string(Buffer));
           break;
      case cDESC:
           if (hasDESC)
           {
             std::cerr << "Error: Record RACE seems to have two DESC sub records.\n";
             return false;
           }
           // DESC's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 1023)
           {
             std::cerr << "Error: Sub record DESC of RACE is longer than 1023 characters.\n";
             return false;
           }
           // read race description
           std::memset(Buffer, '\0', 1024);
           input.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record DESC of RACE.\n";
             return false;
           }
           Description = std::string(Buffer);
           hasDESC = true;
           break;
      default:
           std::cerr << "Unexpected record name \"" << IntTo4Char(SubRecName)
                     << "\" found. Expected NPCS or DESC.\n";
           return false;
    }
  }

  return input.good();
}

#ifndef MW_UNSAVEABLE_RECORDS
bool RaceRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cRACE), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
      + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
      + 4 /* FNAM */ + 4 /* 4 bytes for length */
      + Name.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* RADT */ + 4 /* RADT's length */ + 140 /*size of RADT (always 140 bytes)*/
      + Powers.size() * (4 /* NPCS */ + 4 /* NPCS's length */
      + 32 /*size of NPCS (always 32 bytes, rest is filled with null bytes)*/);
  if (!Description.empty())
  {
    Size = Size + 4 /* DESC */ + 4 /* 4 bytes for length */
          + Description.length() /* length of description (no NUL termination) */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Race Definition:
    NAME = Race ID string
    FNAM = Race name string
    RADT = Race data (140 bytes)
        SkillBonuses[7]
            long SkillID
            long Bonus
        long Strength[2]     (Male/Female)
        long Intelligence[2]
        long Willpower[2]
        long Agility[2]
        long Speed[2]
        long Endurance[2]
        long Personality[2]
        long Luck[2]
        float Height[2]
        float Weight[2]
        long Flags (1 = Playable,2 = Beast Race)
    NPCS = Special power/ability name string (32 bytes), occurs 0+ times
    DESC = Race description (optional) */

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write race's name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = Name.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Name.c_str(), SubLength);

  // write race data (RADT)
  output.write(reinterpret_cast<const char*>(&cRADT), 4);
  SubLength = 140; // fixed size, is always 140 bytes
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // ---- skill boni
  for (unsigned int i = 0; (i < Boni.size()) && (i < 7); ++i)
  {
    output.write(reinterpret_cast<const char*>(&Boni[i].SkillID), 4);
    output.write(reinterpret_cast<const char*>(&Boni[i].Bonus), 4);
  }
  // If there are less than seven bonus entries, add some more to reach the
  // required total size.
  const int32_t SkillID_None = -1;
  const int32_t SkillBonus_None = 0;
  for (unsigned int i = Boni.size(); i < 7; ++i)
  {
    output.write(reinterpret_cast<const char*>(&SkillID_None), 4);
    output.write(reinterpret_cast<const char*>(&SkillBonus_None), 4);
  }
  // ---- attributes
  output.write(reinterpret_cast<const char*>(&(Strength[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Strength[1])), 4);
  output.write(reinterpret_cast<const char*>(&(Intelligence[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Intelligence[1])), 4);
  output.write(reinterpret_cast<const char*>(&(Willpower[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Willpower[1])), 4);
  output.write(reinterpret_cast<const char*>(&(Agility[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Agility[1])), 4);
  output.write(reinterpret_cast<const char*>(&(Speed[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Speed[1])), 4);
  output.write(reinterpret_cast<const char*>(&(Endurance[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Endurance[1])), 4);
  output.write(reinterpret_cast<const char*>(&(Personality[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Personality[1])), 4);
  output.write(reinterpret_cast<const char*>(&(Luck[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Luck[1])), 4);
  // ---- height
  output.write(reinterpret_cast<const char*>(&(Height[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Height[1])), 4);
  // ---- weight
  output.write(reinterpret_cast<const char*>(&(Weight[0])), 4);
  output.write(reinterpret_cast<const char*>(&(Weight[1])), 4);
  // ---- flags
  output.write(reinterpret_cast<const char*>(&Flags), 4);

  // write powers
  for (const auto& power: Powers)
  {
    // write spell ID (NPCS)
    output.write(reinterpret_cast<const char*>(&cNPCS), 4);
    SubLength = 32; // length of string is always 32, rest is filled with zeroes
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    //write spell ID
    const unsigned int len = power.length();
    if (len > 31)
    {
      SubLength = 31;
    }
    else
    {
      SubLength = len;
    }
    output.write(power.c_str(), SubLength);
    // fill rest with zeroes
    output.write(NULof32, 32-SubLength);
  }

  if (!Description.empty())
  {
    // write description (DESC)
    output.write(reinterpret_cast<const char*>(&cDESC), 4);
    SubLength = Description.length(); // length of string (no NUL-termination)
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Description.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool RaceRecord::isPlayable() const
{
  return (rfPlayable & Flags) != 0;
}

bool RaceRecord::isBeastRace() const
{
  return (rfBeastRace & Flags) != 0;
}

bool operator<(const RaceRecord& left, const RaceRecord& right)
{
  return lowerCaseCompare(left.recordID, right.recordID) < 0;
}

} // namespace
