/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef FACTIONRECORD_H
#define FACTIONRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"

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
  std::string FactionID;
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

  /* writes the record to the given output stream and returns true on success

    parameters:
        output   - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

#endif // FACTIONRECORD_H