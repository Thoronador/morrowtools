/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2022  Dirk Stolle

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

#ifndef MW_CELLRECORD_HPP
#define MW_CELLRECORD_HPP

#include <optional>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "ReferencedObject.hpp"

namespace MWTP
{

struct AmbientLight
{
  bool isPresent;
  uint32_t AmbientColour;
  uint32_t SunlightColour;
  uint32_t FogColour;
  float FogDensity;

  AmbientLight();

  bool operator==(const AmbientLight& other) const;
}; // struct

struct CellRecord: public BasicRecord
{
  std::string CellID;
  // cell data
  uint32_t CellFlags;
  int32_t GridX;
  int32_t GridY;
  // end of cell data
  std::string RegionID;
  uint32_t NumReferences;
  std::optional<uint32_t> ColourRef;

  std::optional<float> WaterHeight; // WHGT
  AmbientLight Ambience; // AMBI

  std::vector<ReferencedObject> ReferencesPersistent;
  std::vector<ReferencedObject> ReferencesOther;

  CellRecord();

  /** \brief Checks whether another instance contains the same data.
   *
   * \param other   the other record to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const CellRecord& other) const;

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

  /** \brief Checks whether this is an interior cell.
   *
   * \return Returns true, if the interior cell flag is set.
   */
  bool isInterior() const;

  /** \brief Checks whether this cell has water.
   *
   * \return Returns true, if the water flag is set.
   */
  bool hasWater() const;
}; // struct

} // namespace

#endif // MW_CELLRECORD_HPP
