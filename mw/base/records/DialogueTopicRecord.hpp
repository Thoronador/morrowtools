/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef MW_DIALOGUETOPICRECORD_HPP
#define MW_DIALOGUETOPICRECORD_HPP

#include <string>
#include <fstream>
#include "BasicRecord.hpp"

namespace MWTP
{

struct DialogueTopicRecord: public BasicRecord
{
  std::string DialogueID;
  uint8_t Type;

  /* constructor */
  DialogueTopicRecord();

  /* returns true, if the other record contains the same data */
  bool equals(const DialogueTopicRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output stream
  */
  bool saveToStream(std::ostream& output) const override;
  #endif

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input stream
  */
  bool loadFromStream(std::istream& in_File) override;

  /* type constants to indicate the type of a topic (regular topic, voice,
     greeting, persuasion, Journal) */
  static const uint8_t dttRegular;
  static const uint8_t dttVoice;
  static const uint8_t dttGreeting;
  static const uint8_t dttPersuasion;
  static const uint8_t dttJournal;

  /* returns true, if the topic type indicates a regular topic */
  bool isRegularTopic() const;

  /* returns true, if the topic type indicates a greeting entry */
  bool isGreeting() const;

  /* returns true, if the topic type indicates a journal entry */
  bool isJournal() const;
};//struct

} //namespace

#endif // MW_DIALOGUETOPICRECORD_HPP
