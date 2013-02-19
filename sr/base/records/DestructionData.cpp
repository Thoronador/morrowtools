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
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

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
: isPresent(false), health(0), stageCount(0), unknownTwo(0), unknownThreeFour(0),
  stages(std::vector<DestructionStage>())
{

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

bool DestructionData::loadFromStream(std::ifstream& in_File, const uint32_t recordType, char * buffer, uint32_t& bytesRead)
{
  //DEST's length
  uint16_t subLength = 0;
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=8)
  {
    std::cout << "Error: subrecord DEST of "<<IntTo4Char(recordType)<<" has invalid length ("
              << subLength << " bytes). Should be eight bytes.\n";
    return false;
  }
  //read DEST
  in_File.read((char*) &health, 4);
  in_File.read((char*) &stageCount, 1);
  in_File.read((char*) &unknownTwo, 1);
  in_File.read((char*) &unknownThreeFour, 2);
  bytesRead += 8;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DEST of "<<IntTo4Char(recordType)<<"!\n";
    return false;
  }

  isPresent = true;
  stages.clear();
  DestructionStage tempStage;

  while (stages.size()<stageCount)
  {
    //read next record
    uint32_t innerRecordName = 0;
    in_File.read((char*) &innerRecordName, 4);
    bytesRead += 4;
    switch (innerRecordName)
    {
      case cDSTD:
           if (tempStage.unknownDSTD.isPresent())
           {
             std::cout << "Error: "<<IntTo4Char(recordType)<<" seems to have more than one DSTD subrecord per stage!\n";
             return false;
           }//if
           //read DSTD
           if (!tempStage.unknownDSTD.loadFromStream(in_File, cDSTD, false)) return false;
           bytesRead += (2+tempStage.unknownDSTD.getSize());
           break;
      case cDMDL:
           if (!tempStage.replacementModel.empty())
           {
             std::cout << "Error: "<<IntTo4Char(recordType)<<" seems to have more than one DMDL subrecord per stage!\n";
             return false;
           }//if
           //DMDL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record DMDL of "<<IntTo4Char(recordType)<<" is longer than 511 characters!\n";
             return false;
           }
           //read string
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DMDL of "<<IntTo4Char(recordType)<<"!\n";
             return false;
           }
           tempStage.replacementModel = std::string(buffer);
           //check content
           if (tempStage.replacementModel.empty())
           {
             std::cout << "Error: subrecord DMDL of "<<IntTo4Char(recordType)<<" is empty!\n";
             return false;
           }
           break;
      case cDMDT:
           if (tempStage.unknownDMDT.isPresent())
           {
             std::cout << "Error: "<<IntTo4Char(recordType)
                       << " seems to have more than one DMDT subrecord per stage!\n";
             return false;
           }//if
           //read DMDT
           if (!tempStage.unknownDMDT.loadFromStream(in_File, cDMDT, false)) return false;
           bytesRead += (2+tempStage.unknownDMDT.getSize());
           break;
      case cDSTF:
           //DSTF's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout << "Error: subrecord DSTF of "<<IntTo4Char(recordType)
                       << " has invalid length (" << subLength
                       << " bytes). Should be zero bytes.\n";
             return false;
           }
           //push it, if not empty
           if (tempStage.isReset())
           {
             std::cout << "Error: "<<IntTo4Char(recordType)<<" seems to have an empty destruction stage!\n";
             return false;
           }
           stages.push_back(tempStage);
           tempStage.reset();
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(innerRecordName)
                     << "\", but only DSTD, DMDL, DMDT or DSTF are allowed here!\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

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
