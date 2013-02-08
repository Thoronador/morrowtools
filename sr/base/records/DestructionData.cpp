/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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

#include "DestructionData.h"
#include "../SR_Constants.h"

namespace SRTP
{

/* DestructionStage's functions */

DestructionStage::DestructionStage()
: replacementModel("")
{
  unknownDSTD.setPresence(false);
  unknownDMDT.setPresence(false);
}

#ifndef SR_NO_RECORD_EQUALITY
bool DestructionStage::operator==(const DestructionStage& other) const
{
  return ((unknownDSTD==other.unknownDSTD) and (replacementModel==other.replacementModel)
      and (unknownDMDT==other.unknownDMDT));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DestructionStage::getWriteSize() const
{
  uint32_t writeSize = 4 /* DSTF */ +2 /* 2 bytes for length */;  //+zero for data :D
  if (unknownDSTD.isPresent())
  {
    writeSize = writeSize +4 /* DSTD */ +2 /* 2 bytes for length */ +unknownDSTD.getSize();
  }//if DSTD
  if (!replacementModel.empty())
  {
    writeSize = writeSize +4 /* DMDL */ +2 /* 2 bytes for length */
        +replacementModel.length()+1 /* length of model +1 byte for NUL-termination */;
  }
  if (unknownDMDT.isPresent())
  {
    writeSize = writeSize +4 /* DMDT */ +2 /* 2 bytes for length */ +unknownDMDT.getSize();
  }//if DMDT
  return writeSize;
}

bool DestructionStage::saveToStream(std::ofstream& output) const
{
  if (unknownDSTD.isPresent())
  {
    if (!unknownDSTD.saveToStream(output, cDSTD))
      return false;
  }//if DSTD
  if (!replacementModel.empty())
  {
    //write DMDL
    output.write((const char*) &cDMDL, 4);
    //DMDL's length
    uint16_t subLength = replacementModel.length()+1;
    output.write((const char*) &subLength, 2);
    //write replacement model
    output.write(replacementModel.c_str(), subLength);
  }
  if (unknownDMDT.isPresent())
  {
    if (!unknownDMDT.saveToStream(output, cDMDT))
      return false;
  }//if DMDT

  //write DSTF
  output.write((const char*) &cDSTF, 4);
  //DSTF's length
  const uint16_t subLength = 0;
  output.write((const char*) &subLength, 2);

  return output.good();
}
#endif

void DestructionStage::reset()
{
  unknownDSTD.setPresence(false);
  replacementModel.clear();
  unknownDMDT.setPresence(false);
}

/* DestructionData's functions */

DestructionData::DestructionData()
: isPresent(false), health(0), stageCount(0), unknownTwo(0), unknownThreeFour(0)
{
  stages.clear();
}

#ifndef SR_NO_RECORD_EQUALITY
bool DestructionData::operator==(const DestructionData& other) const
{
  if (isPresent!=other.isPresent) return false;
  if (!isPresent) return true;
  return ((health==other.health) and (stageCount==other.stageCount)
      and (unknownTwo==other.unknownTwo) and (unknownThreeFour==other.unknownThreeFour)
      and (stages==other.stages));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DestructionData::getWriteSize() const
{
  if (!isPresent) return 0;
  uint32_t writeSize = 4 /* DEST */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  unsigned int i;
  for (i=0; i<stages.size(); ++i)
  {
    writeSize += stages[i].getWriteSize();
  }//for
  return writeSize;
}

bool DestructionData::saveToStream(std::ofstream& output) const
{
  if (!isPresent) return true;
  //write DEST
  output.write((const char*) &cDEST, 4);
  //DEST's length
  const uint16_t subLength = 8;
  output.write((const char*) &subLength, 2);
  //write DEST
  output.write((const char*) &health, 4);
  output.write((const char*) &stageCount, 1);
  output.write((const char*) &unknownTwo, 1);
  output.write((const char*) &unknownThreeFour, 2);
  unsigned int i;
  for (i=0; i<stages.size(); ++i)
  {
    if (!stages[i].saveToStream(output))
      return false;
  }//for
  return output.good();
}
#endif

void DestructionData::clear()
{
  isPresent = false;
  health = 0;
  stageCount = 0;
  unknownTwo = 0;
  unknownThreeFour = 0;
  stages.clear();
}

} //namespace
