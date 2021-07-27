/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_UNSAVEABLE_RECORDS
#include <fstream>
#endif
#include <vector>
#include <stdint.h>
#include "CTDAData.hpp"

namespace SRTP
{

//type for effect blocks
struct EffectBlock
{
  /* constructor */
  EffectBlock();

  uint32_t effectFormID; //subrecord EFID, form ID of MGEF
  //subrecord EFIT
  float magnitude;
  uint32_t areaOfEffect;
  uint32_t duration;
  //end of subrecord EFIT
  std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;

  /* comparison operator */
  bool operator==(const EffectBlock& other) const;

  #ifndef SR_UNSAVEABLE_RECORDS
  /* tries to save the effect block to the given stream and returns true in case
     of success, false on failure

     parameters:
          output   - the output stream
  */
  bool saveToStream(std::ostream& output) const;

  /* returns the size in bytes that the EffectBlocks's data would occupy in a
     file stream
  */
  uint32_t getWriteSize() const;
  #endif
}; //struct

} //namespace

#endif // SR_EFFECTBLOCK_HPP
