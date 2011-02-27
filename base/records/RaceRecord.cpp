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

#include "RaceRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

RaceRecord::RaceRecord()
{
  RaceID = RaceName = "";
  //race data
  Boni.clear();
  Strength[0] = Strength[1] = 0;
  Intelligence[0] = Intelligence[1] = 0;
  Willpower[0] = Willpower[1] = 0;
  Agility[0] = Agility[1] = 0;
  Speed[0] = Speed[1] = 0;
  Endurance[0] = Endurance[1] = 0;
  Personality[0] = Personality[1] = 0;
  Luck[0] = Luck[1] = 0;
  Weight[0] = Weight[1] = 0.0f;
  Height[0] = Height[1] = 1.0f;
  RaceFlags = 0;
  //end of race data
  Powers.clear();
  Description = "";
}

RaceRecord::RaceRecord(const std::string& ID)
{
  RaceID = ID;
  RaceName = "";
  //race data
  Boni.clear();
  Strength[0] = Strength[1] = 0;
  Intelligence[0] = Intelligence[1] = 0;
  Willpower[0] = Willpower[1] = 0;
  Agility[0] = Agility[1] = 0;
  Speed[0] = Speed[1] = 0;
  Endurance[0] = Endurance[1] = 0;
  Personality[0] = Personality[1] = 0;
  Luck[0] = Luck[1] = 0;
  Weight[0] = Weight[1] = 0.0f;
  Height[0] = Height[1] = 1.0f;
  RaceFlags = 0;
  //end of race data
  Powers.clear();
  Description = "";
}

RaceRecord::~RaceRecord()
{
  //empty
}

bool RaceRecord::equals(const RaceRecord& other) const
{
  if ((RaceID==other.RaceID) and (RaceName==other.RaceName)
    and (Boni.size()==other.Boni.size()) and (Description==other.Description)
    and (Strength[0]==other.Strength[0]) and (Strength[1]==other.Strength[1])
    and (Intelligence[0]==other.Intelligence[0]) and (Intelligence[1]==other.Intelligence[1])
    and (Willpower[0]==other.Willpower[0]) and (Willpower[1]==other.Willpower[1])
    and (Agility[0]==other.Agility[0]) and (Agility[1]==other.Agility[1])
    and (Speed[0]==other.Speed[0]) and (Speed[1]==other.Speed[1])
    and (Endurance[0]==other.Endurance[0]) and (Endurance[1]==other.Endurance[1])
    and (Personality[0]==other.Personality[0]) and (Personality[1]==other.Personality[1])
    and (Luck[0]==other.Luck[0]) and (Luck[1]==other.Luck[1])
    and (Weight[0]==other.Weight[0]) and (Weight[1]==other.Weight[1])
    and (Height[0]==other.Height[0]) and (Height[1]==other.Height[1])
    and (RaceFlags==other.RaceFlags) and (Powers==other.Powers))
  {
    unsigned int i;
    for(i=0;i<Boni.size(); ++i)
    {
      if ((Boni[i].SkillID!=other.Boni[i].SkillID)
        or (Boni[i].SkillBonus!=other.Boni[i].SkillBonus))
      {
        return false;
      }
    }//for
    return true;
  }
  return false;
}

bool RaceRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

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
    NPCS = Special power/ability name string (32 bytes), multiple
    DESC = Race description */

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
    std::cout << "Error: subrecord NAME of RACE is longer than 255 characters.\n";
    return false;
  }
  //read race ID
  char Buffer[1024]; //so long because of description text
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of RACE.\n";
    return false;
  }
  RaceID = std::string(Buffer);

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
    std::cout << "Error: subrecord FNAM of RACE is longer than 255 characters.\n";
    return false;
  }
  //read race name
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FNAM of RACE.\n";
    return false;
  }
  RaceName = std::string(Buffer);

  //read RADT
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cRADT)
  {
    UnexpectedRecord(cRADT, SubRecName);
    return false;
  }
  //RADT's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=140)
  {
    std::cout << "Error: sub record RADT of RACE has invalid length ("
              <<SubLength<< " bytes). Should be 140 bytes.\n";
    return false;
  }//if
  //read race data
  // ---- boni first, 7 times
  unsigned int i;
  SkillBonus sb;
  Boni.clear();
  for (i=0; i<7; ++i)
  {
    in_File.read((char*) &(sb.SkillID), 4);
    in_File.read((char*) &(sb.SkillBonus), 4);
    Boni.push_back(sb);
  }//for
  // ---- attributes
  in_File.read((char*) &(Strength[0]), 4);
  in_File.read((char*) &(Strength[1]), 4);
  in_File.read((char*) &(Intelligence[0]), 4);
  in_File.read((char*) &(Intelligence[1]), 4);
  in_File.read((char*) &(Willpower[0]), 4);
  in_File.read((char*) &(Willpower[1]), 4);
  in_File.read((char*) &(Agility[0]), 4);
  in_File.read((char*) &(Agility[1]), 4);
  in_File.read((char*) &(Speed[0]), 4);
  in_File.read((char*) &(Speed[1]), 4);
  in_File.read((char*) &(Endurance[0]), 4);
  in_File.read((char*) &(Endurance[1]), 4);
  in_File.read((char*) &(Personality[0]), 4);
  in_File.read((char*) &(Personality[1]), 4);
  in_File.read((char*) &(Luck[0]), 4);
  in_File.read((char*) &(Luck[1]), 4);
  // ---- height
  in_File.read((char*) &(Height[0]), 4);
  in_File.read((char*) &(Height[1]), 4);
  // ---- weight
  in_File.read((char*) &(Weight[0]), 4);
  in_File.read((char*) &(Weight[1]), 4);
  // ---- flags
  in_File.read((char*) &RaceFlags, 4);
  BytesRead += 140;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord RADT of RACE.\n";
    return false;
  }

  //read multiple NPCS
  Powers.clear();
  while (BytesRead<Size)
  {
    //read NPCS
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cNPCS)
    {
      break;
    }
    //NPCS's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=32)
    {
      std::cout << "Error: sub record NPCS of RACE has invalid length ("
                <<SubLength<< " bytes). Should be 32 bytes.\n";
      return false;
    }//if
    //read spell ID
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, 32);
    BytesRead += 32;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord NPCS of RACE.\n";
      return false;
    }
    //add spell ID
    Powers.push_back(std::string(Buffer));
  } //while

  //read DESC (record name was already read in loop above)
  if (SubRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, SubRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>1023)
  {
    std::cout << "Error: subrecord DESC of RACE is longer than 1023 characters.\n";
    return false;
  }
  //read race description
  memset(Buffer, '\0', 1024);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DESC of RACE.\n";
    return false;
  }
  Description = std::string(Buffer);
  return in_File.good();
}

bool RaceRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cRACE, 4);
  int32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +RaceID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */
        +RaceName.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* RADT */ +4 /* RADT's length */ +140 /*size of RADT (always 140 bytes)*/
        +Powers.size()*(4 /* NPCS */ +4 /* NPCS's length */
              +32 /*size of NPCS (always 32 bytes, rest is filled with null bytes)*/)
        +4 /* DESC */ +4 /* 4 bytes for length */
        +Description.length() /* length of decription (no NUL termination) */;
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

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
    NPCS = Special power/ability name string (32 bytes), multiple
    DESC = Race description */

  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  int32_t SubLength;
  SubLength = RaceID.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(RaceID.c_str(), SubLength);

  //write FNAM
  output.write((char*) &cFNAM, 4);
  //FNAM's length
  SubLength = RaceName.length()+1;//length of string plus one for NUL-termination
  output.write((char*) &SubLength, 4);
  //write race's name
  output.write(RaceName.c_str(), SubLength);

  //write RADT
  output.write((char*) &cRADT, 4);
  //RADT's length
  SubLength = 140;//length is always 140 bytes
  output.write((char*) &SubLength, 4);
  //write race data
  // ---- skill boni
  unsigned int i;
  for (i=0; (i<Boni.size()) and (i<7); ++i)
  {
    output.write((char*) &Boni[i].SkillID, 4);
    output.write((char*) &Boni[i].SkillBonus, 4);
  }//for
  //If there are less than seven bonus entries, add some more to reach the
  // required total size.
  const int32_t SkillID_None = -1;
  const int32_t SkillBonus_None = 0;
  for (i=Boni.size(); i<7; ++i)
  {
    output.write((char*) &SkillID_None, 4);
    output.write((char*) &SkillBonus_None, 4);
  }//for
  // ---- attributes
  output.write((char*) &(Strength[0]), 4);
  output.write((char*) &(Strength[1]), 4);
  output.write((char*) &(Intelligence[0]), 4);
  output.write((char*) &(Intelligence[1]), 4);
  output.write((char*) &(Willpower[0]), 4);
  output.write((char*) &(Willpower[1]), 4);
  output.write((char*) &(Agility[0]), 4);
  output.write((char*) &(Agility[1]), 4);
  output.write((char*) &(Speed[0]), 4);
  output.write((char*) &(Speed[1]), 4);
  output.write((char*) &(Endurance[0]), 4);
  output.write((char*) &(Endurance[1]), 4);
  output.write((char*) &(Personality[0]), 4);
  output.write((char*) &(Personality[1]), 4);
  output.write((char*) &(Luck[0]), 4);
  output.write((char*) &(Luck[1]), 4);
  // ---- height
  output.write((char*) &(Height[0]), 4);
  output.write((char*) &(Height[1]), 4);
  // ---- weight
  output.write((char*) &(Weight[0]), 4);
  output.write((char*) &(Weight[1]), 4);
  // ---- flags
  output.write((char*) &RaceFlags, 4);

  //write powers
  for (i=0; i<Powers.size(); ++i)
  {
    //write NPCS
    output.write((char*) &cNPCS, 4);
    //NPCS's length
    SubLength = 32;//length of string is always 32, rest is filled with zeroes
    output.write((char*) &SubLength, 4);
    //write spell ID
    unsigned int len = Powers.at(i).length();
    if (len>31)
    {
      SubLength=31;
    }
    else
    {
      SubLength = len;
    }
    output.write(Powers.at(i).c_str(), SubLength);
    //fill rest with zeroes
    output.write(NULof32, 32-SubLength);
  }//for

  //write DESC
  output.write((char*) &cDESC, 4);
  //DESC's length
  SubLength = Description.length();//length of string (no NUL-termination)
  output.write((char*) &SubLength, 4);
  //write description
  output.write(Description.c_str(), SubLength);

  return output.good();
}

bool operator<(const RaceRecord& left, const RaceRecord& right)
{
  return (left.RaceID.compare(right.RaceID)<0);
}
