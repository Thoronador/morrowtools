/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef MW_RACERECORD_HPP
#define MW_RACERECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"

namespace MWTP
{

struct SkillBonus
{
  int32_t SkillID;
  int32_t SkillBonus;
};//struct

struct RaceRecord: public BasicRecord
{
  std::string recordID; //formerly RaceID
  std::string RaceName;
  //race data
  std::vector<SkillBonus> Boni;
  int32_t Strength[2];
  int32_t Intelligence[2];
  int32_t Willpower[2];
  int32_t Agility[2];
  int32_t Speed[2];
  int32_t Endurance[2];
  int32_t Personality[2];
  int32_t Luck[2];
  float Weight[2];
  float Height[2];
  int32_t RaceFlags;
  //end of race data
  std::vector<std::string> Powers;
  std::string Description;

  /* constructor */
  RaceRecord();

  /* alternative constructor */
  RaceRecord(const std::string& ID);

  /* destructor */
  ~RaceRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const RaceRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;
  #endif

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);

  /* returns true, if it is a playable race */
  bool isPlayable() const;

  /* returns true, if it is a beast race */
  bool isBeastRace() const;
};//struct

//comparison operator for ordered set
bool operator<(const RaceRecord& left, const RaceRecord& right);

} //namespace

#endif // MW_RACERECORD_HPP
