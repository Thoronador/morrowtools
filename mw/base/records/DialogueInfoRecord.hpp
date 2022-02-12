/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2022  Dirk Stolle

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

#ifndef MW_DIALOGUEINFORECORD_HPP
#define MW_DIALOGUEINFORECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"

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

  FuncVarRecord();

  bool operator==(const FuncVarRecord& other) const;
}; // struct

struct DialogueInfoRecord: public BasicRecord
{
  std::string recordID;
  std::string PreviousInfoID;
  std::string NextInfoID;
  // info data
  int32_t UnknownLong;
  int32_t Disposition;
  uint8_t Rank;
  uint8_t Gender;
  uint8_t PCRank;
  uint8_t UnknownByte;
  // end of info data
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

  DialogueInfoRecord();

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const DialogueInfoRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the record's data would occupy in a
   *         stream, NOT including the header data.
   *
   * \return Returns the size in bytes that the record would need. Size of the
   *         header is not included.
   */
  uint32_t getWriteSize() const;

  /** \brief Writes the record to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (record was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif

  /** \brief Loads the record from the given input stream.
   *
   * \param input    the input stream
   * \return Returns true on success (record was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& input) override;
}; // struct

} // namespace

#endif // MW_DIALOGUEINFORECORD_HPP
