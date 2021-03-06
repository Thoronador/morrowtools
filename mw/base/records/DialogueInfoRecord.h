/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef MW_DIALOGUEINFORECORD_H
#define MW_DIALOGUEINFORECORD_H

#include <string>
#include <fstream>
#include <vector>
#include "BasicRecord.h"

namespace MWTP
{

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

  /* constructor */
  FuncVarRecord();

  /* comparison operator */
  bool operator==(const FuncVarRecord& other) const;
};//struct

struct DialogueInfoRecord: public BasicRecord
{
  std::string recordID; //formerly InfoID
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

#endif // MW_DIALOGUEINFORECORD_H
