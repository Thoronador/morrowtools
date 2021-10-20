/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#ifndef SR_EFFECTBLOCK_HPP
#define SR_EFFECTBLOCK_HPP

#include <cstdint>
#ifndef SR_UNSAVEABLE_RECORDS
#include <fstream>
#endif
#include <vector>
#include "CTDAData.hpp"

namespace SRTP
{

/// Holds information about concrete magic effects.
struct EffectBlock
{
  /** Constructor, creates an empty block. */
  EffectBlock();

  uint32_t effectFormID; /**< form ID of MGEF, subrecord EFID */
  // subrecord EFIT
  float magnitude;
  uint32_t areaOfEffect;
  uint32_t duration;
  // end of subrecord EFIT
  std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;

  #ifndef SR_NO_RECORD_EQUALITY
  /** equality operator */
  bool operator==(const EffectBlock& other) const;
  #endif

  #ifndef SR_UNSAVEABLE_RECORDS
  /** \brief Writes the block data to the given output stream.
   *
   * \param output  the output stream
   * \return Returns true on success (block was written to stream).
   *         Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const;

  /** \brief Gets the size in bytes that the block's data would occupy in a file
   *         stream.
   *
   * \return Returns the size in bytes that the block would need.
   */
  uint32_t getWriteSize() const;
  #endif
}; // struct

} // namespace

#endif // SR_EFFECTBLOCK_HPP
