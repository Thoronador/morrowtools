/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021, 2022, 2023  Dirk Stolle

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

#ifndef MW_TRAVELDESTINATION_HPP
#define MW_TRAVELDESTINATION_HPP

#include <cstdint>
#include <fstream>
#include <string>

namespace MWTP
{

struct TravelDestination
{
  float XPos, YPos, ZPos;
  float XRot, YRot, ZRot;
  std::string CellName; // is usually empty for exterior cells

  TravelDestination();

  bool operator==(const TravelDestination& other) const;

  /** \brief Loads the destination data from the given input stream, except
   *         for the cell name. The cell name has to be read separately by
   *         the caller, if a cell name (DNAM) follows.
   *
   * \param input   the input stream
   * \param bytesRead  the variable that holds the number of bytes read so far
   * \return Returns true on success (data was loaded from stream). Cell name
   *         will be empty in that case, but it may follow next in the stream.
   *         Returns false, if an error occurred.
   */
  bool partialLoadFromStream(std::istream& input, uint32_t& bytesRead);

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the instances's data would occupy in a
   *         stream.
   *
   * \return Returns the size in bytes that the instance's data needs.
   */
  uint32_t getStreamSize() const;

  /** \brief Writes the destination data to the given output stream.
   *
   * \param output  the output stream
   * \return  Returns true on success (data was written to stream).
   *          Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const;
  #endif // MW_UNSAVEABLE_RECORDS
};

} // namespace

#endif // MW_TRAVELDESTINATION_HPP
