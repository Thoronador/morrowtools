/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021  Dirk Stolle

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

#ifndef MW_ITEMRECORD_HPP
#define MW_ITEMRECORD_HPP

#include <cstdint>
#include <string>
#include <fstream>

namespace MWTP
{

struct ItemRecord
{
  int32_t Count;
  std::string Item;

  ItemRecord();

  bool operator==(const ItemRecord& other) const;

  /** \brief Loads the record from the given input stream.
   *
   * \param input    the input stream
   * \param buffer   pre-allocated buffer that can hold at least 33 chars
   * \param bytesRead   the variable that holds the number of bytes read so far
   * \return Returns true on success (record was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& input, char* buffer, uint32_t& bytesRead);

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Writes the record to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (record was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const;
  #endif
};

} // namespace

#endif // MW_ITEMRECORD_HPP
