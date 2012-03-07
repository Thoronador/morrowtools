/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SR_EFFECTBLOCK_H
#define SR_EFFECTBLOCK_H

#include <fstream>
#include <vector>
#include <stdint.h>
#include "CTDAData.h"

namespace SRTP
{

//type for effect blocks
struct EffectBlock
{
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
          output   - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* returns the size in bytes that the EffectBlocks's data would occupy in a
     file stream
  */
  uint32_t getWriteSize() const;
  #endif
}; //struct

} //namespace

#endif // SR_EFFECTBLOCK_H
