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

#ifndef MW_FACTIONRECORD_H
#define MW_FACTIONRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"

namespace MWTP
{

struct FactionRankData
{
  int32_t Attr1;
  int32_t Attr2;
  int32_t FirstSkill;
  int32_t SecondSkill;
  int32_t Faction;

  /* equality operator */
  bool operator==(const FactionRankData& other) const;
};//struct

struct FactionReactionData
{
  std::string OtherFaction;
  int32_t Disposition;

  /* equality operator */
  bool operator==(const FactionReactionData& other) const;
};//struct

struct FactionRecord: public BasicRecord
{
  std::string recordID; //formerly FactionID
  std::string Name;
  std::vector<std::string> RankNames;
  //faction data
  int32_t AttrID1;
  int32_t AttrID2;
  std::vector<FactionRankData> RankData;
  int32_t SkillIDs[6];
  int32_t Unknown;
  int32_t FactionFlags;
  //end of faction data
  std::vector<FactionReactionData> Reactions;

  /* constructor */
  FactionRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const FactionRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output   - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;
  #endif

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

} //namespace

#endif // MW_FACTIONRECORD_H
