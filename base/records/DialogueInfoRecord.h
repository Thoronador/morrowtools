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

#ifndef DIALOGUEINFORECORD_H
#define DIALOGUEINFORECORD_H

#include <string>
#include <fstream>
#include <vector>
#include "BasicRecord.h"

struct FuncVarRecord
{
  uint8_t Index;
  uint8_t Type;
  uint16_t Function;
  uint8_t CompareOp;
  std::string Name;

  bool isFloat;
  float fVal;
  int32_t iVal;

  bool operator==(const FuncVarRecord& other) const;
};//struct

struct DialogueInfoRecord: public BasicRecord
{
  std::string InfoID;
  std::string PreviousInfoID;
  std::string NextInfoID;
  //info data
  int32_t UnknownLong;
  int32_t Disposition;
  uint8_t Rank;
  uint8_t Gender;
  uint8_t PCRank;
  uint8_t UnknownByte;
  //end of info data
  std::string ActorID;
  std::string RaceID;
  std::string ClassID;
  std::string FactionID;
  std::string CellID;
  std::string PCFactionID;
  std::string SoundFile;
  bool isQuestName;
  bool isQuestFinished;
  bool isQuestRestart;
  std::string Response;
  std::vector<FuncVarRecord> Functions;
  std::string ResultString;

  /* constructor */
  DialogueInfoRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const DialogueInfoRecord& other) const;

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

#endif // DIALOGUEINFORECORD_H
