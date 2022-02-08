/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2022  Dirk Stolle

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
#include "BasicRecord.hpp"

namespace MWTP
{

enum class DialogueTopicType: std::uint8_t
{
  Regular = 0,
  Voice = 1,
  Greeting = 2,
  Persuasion = 3,
  Journal = 4
};

struct DialogueTopicRecord: public BasicRecord
{
  std::string DialogueID;
  DialogueTopicType Type;

  DialogueTopicRecord();

  /** \brief Constructs a RegionRecord with the given region ID.
   *
   * \param ID  the region ID
   */
  bool equals(const DialogueTopicRecord& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
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

#endif // MW_DIALOGUETOPICRECORD_HPP
