/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
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

#ifndef SR_SUBBLOCK_HPP
#define SR_SUBBLOCK_HPP

#include <cstdint>
#include "BinarySubRecord.hpp"

namespace SRTP
{

/// wrapper type for unknown sub-record blocks
struct SubBlock
{
  uint32_t subType; /**< header of the sub-record */
  BinarySubRecord subData; /**< sub-record data */

  SubBlock()
  : subType(0), subData(BinarySubRecord())
  {  }

  inline bool operator==(const SubBlock& other) const
  {
    return (subType == other.subType) && (subData == other.subData);
  }
};

} // namespace
#endif // SR_SUBBLOCK_HPP
