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

#ifndef DIALOGUETOPICRECORD_H
#define DIALOGUETOPICRECORD_H

#include <string>
#include <fstream>
#include "BasicRecord.h"

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

#endif // DIALOGUETOPICRECORD_H
