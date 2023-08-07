/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021, 2022  Dirk Stolle

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

#ifndef MW_BODYPARTASSOCIATION_HPP
#define MW_BODYPARTASSOCIATION_HPP

#include <cstdint>
#include <string>
#ifndef MW_UNSAVEABLE_RECORDS
#include <fstream>
#endif

namespace MWTP
{

struct BodyPartAssociation
{
  uint8_t Index;
  std::string MaleBodyPart;
  std::string FemaleBodyPart;

  BodyPartAssociation();

  bool operator==(const BodyPartAssociation& other) const;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the data would occupy in a file stream.
   *
   * \return Returns the size in bytes that the association would need.
   */
  uint32_t getWriteSize() const;

  /** \brief Writes the association to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (data was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const;
  #endif
};

} // namespace

#endif // MW_BODYPARTASSOCIATION_HPP
