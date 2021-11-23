/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013, 2021  Thoronador

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

#ifndef MW_GMSTRECORD_HPP
#define MW_GMSTRECORD_HPP

#include <string>
#include "BasicRecord.hpp"

namespace MWTP
{

enum class GMSTType { Integer, Float, String };

/** Holds the name and value of a game setting. */
struct GMSTRecord: public BasicRecord
{
  GMSTRecord();

  std::string SettingName;
  GMSTType Type;
  int32_t iVal;     /**< only valid if @Type is Integer */
  float fVal;       /**< only valid if @Type is Float */
  std::string sVal; /**< only valid if @Type is String */

  /** \brief Checks whether another instance contains the same relevant data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as instance.
   *         Returns false otherwise.
   */
  bool equals(const GMSTRecord& other) const;

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
};

} // namespace

#endif // MW_GMSTRECORD_HPP
