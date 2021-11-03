/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2021  Thoronador

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

#include "DestructionData.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* DestructionStage's functions */

DestructionStage::DestructionStage()
: unknownDSTD(BinarySubRecord()),
  replacementModel(""),
  unknownDMDT(BinarySubRecord())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool DestructionStage::operator==(const DestructionStage& other) const
{
  return (unknownDSTD == other.unknownDSTD) && (replacementModel == other.replacementModel)
      && (unknownDMDT == other.unknownDMDT);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DestructionStage::getWriteSize() const
{
  uint32_t writeSize = 4 /* DSTF */ + 2 /* 2 bytes for length */;  //+zero for data :D
  if (unknownDSTD.isPresent())
  {
    writeSize = writeSize + 4 /* DSTD */ + 2 /* 2 bytes for length */ + unknownDSTD.size();
  }
  if (!replacementModel.empty())
  {
    writeSize = writeSize + 4 /* DMDL */ +2 /* 2 bytes for length */
        + replacementModel.length() + 1 /* length of model +1 byte for NUL-termination */;
  }
  if (unknownDMDT.isPresent())
  {
    writeSize = writeSize + 4 /* DMDT */ + 2 /* 2 bytes for length */ + unknownDMDT.size();
  }
  return writeSize;
}

bool DestructionStage::saveToStream(std::ostream& output) const
{
  if (unknownDSTD.isPresent())
  {
    if (!unknownDSTD.saveToStream(output, cDSTD))
      return false;
  }
  if (!replacementModel.empty())
  {
    // write replacement model (DMDL)
    output.write(reinterpret_cast<const char*>(&cDMDL), 4);
    const uint16_t subLength = replacementModel.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(replacementModel.c_str(), subLength);
  }
  if (unknownDMDT.isPresent())
  {
    if (!unknownDMDT.saveToStream(output, cDMDT))
      return false;
  }

  // write DSTF
  output.write(reinterpret_cast<const char*>(&cDSTF), 4);
  const uint16_t subLength = 0;
  output.write(reinterpret_cast<const char*>(&subLength), 2);

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
: isPresent(false),
  health(0), stageCount(0), unknownTwo(0), unknownThreeFour(0),
  stages(std::vector<DestructionStage>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool DestructionData::operator==(const DestructionData& other) const
{
  if (isPresent != other.isPresent)
    return false;
  if (!isPresent)
    return true;
  return (health == other.health)
      && (stageCount == other.stageCount)
      && (unknownTwo == other.unknownTwo)
      && (unknownThreeFour == other.unknownThreeFour)
      && (stages == other.stages);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DestructionData::getWriteSize() const
{
  if (!isPresent)
    return 0;
  uint32_t writeSize = 4 /* DEST */ + 2 /* 2 bytes for length */ + 8 /* fixed size */;
  for (const auto& stage : stages)
  {
    writeSize += stage.getWriteSize();
  }
  return writeSize;
}

bool DestructionData::saveToStream(std::ostream& output) const
{
  if (!isPresent)
    return true;
  // write DEST
  output.write(reinterpret_cast<const char*>(&cDEST), 4);
  const uint16_t subLength = 8;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DEST's data
  output.write(reinterpret_cast<const char*>(&health), 4);
  output.write(reinterpret_cast<const char*>(&stageCount), 1);
  output.write(reinterpret_cast<const char*>(&unknownTwo), 1);
  output.write(reinterpret_cast<const char*>(&unknownThreeFour), 2);
  for (const auto& stage : stages)
  {
    if (!stage.saveToStream(output))
      return false;
  }
  return output.good();
}
#endif

bool DestructionData::loadFromStream(std::istream& in_File, const uint32_t recordType, char * buffer, uint32_t& bytesRead)
{
  // DEST's length
  uint16_t subLength = 0;
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength != 8)
  {
    std::cerr << "Error: Subrecord DEST of " << IntTo4Char(recordType)
              << " has invalid length (" << subLength
              << " bytes). Should be eight bytes.\n";
    return false;
  }
  // read DEST's data
  in_File.read(reinterpret_cast<char*>(&health), 4);
  in_File.read(reinterpret_cast<char*>(&stageCount), 1);
  in_File.read(reinterpret_cast<char*>(&unknownTwo), 1);
  in_File.read(reinterpret_cast<char*>(&unknownThreeFour), 2);
  bytesRead += 8;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord DEST of "
              << IntTo4Char(recordType) << "!\n";
    return false;
  }

  isPresent = true;
  stages.clear();
  DestructionStage tempStage;

  while (stages.size() < stageCount)
  {
    // read next record header
    uint32_t innerRecordName = 0;
    in_File.read(reinterpret_cast<char*>(&innerRecordName), 4);
    bytesRead += 4;
    switch (innerRecordName)
    {
      case cDSTD:
           if (tempStage.unknownDSTD.isPresent())
           {
             std::cerr << "Error: " << IntTo4Char(recordType)
                       << " seems to have more than one DSTD subrecord per stage!\n";
             return false;
           }
           // read DSTD
           if (!tempStage.unknownDSTD.loadFromStream(in_File, cDSTD, false))
             return false;
           bytesRead += (2 + tempStage.unknownDSTD.size());
           break;
      case cDMDL:
           if (!tempStage.replacementModel.empty())
           {
             std::cerr << "Error: " << IntTo4Char(recordType)
                       << " seems to have more than one DMDL subrecord per stage!\n";
             return false;
           }
           // DMDL's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength > 511)
           {
             std::cerr << "Error: Sub record DMDL of " << IntTo4Char(recordType)
                       << " is longer than 511 characters!\n";
             return false;
           }
           // read string
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DMDL of "
                       << IntTo4Char(recordType) << "!\n";
             return false;
           }
           tempStage.replacementModel = std::string(buffer);
           // check content
           if (tempStage.replacementModel.empty())
           {
             std::cerr << "Error: Subrecord DMDL of " << IntTo4Char(recordType)
                       << " is empty!\n";
             return false;
           }
           break;
      case cDMDT:
           if (tempStage.unknownDMDT.isPresent())
           {
             std::cerr << "Error: " << IntTo4Char(recordType)
                       << " seems to have more than one DMDT subrecord per stage!\n";
             return false;
           }
           // read DMDT
           if (!tempStage.unknownDMDT.loadFromStream(in_File, cDMDT, false))
             return false;
           bytesRead += (2 + tempStage.unknownDMDT.size());
           break;
      case cDSTF:
           // DSTF's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 0)
           {
             std::cerr << "Error: Subrecord DSTF of " << IntTo4Char(recordType)
                       << " has invalid length (" << subLength
                       << " bytes). Should be zero bytes.\n";
             return false;
           }
           // push it, if not empty
           if (tempStage.isReset())
           {
             std::cerr << "Error: " << IntTo4Char(recordType)
                       << " seems to have an empty destruction stage!\n";
             return false;
           }
           stages.push_back(tempStage);
           tempStage.reset();
           break;
      default:
           std::cerr << "Error: Found unexpected subrecord \""
                     << IntTo4Char(innerRecordName)
                     << "\", but only DSTD, DMDL, DMDT or DSTF are allowed here!\n";
           return false;
    }
  }

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

} // namespace
