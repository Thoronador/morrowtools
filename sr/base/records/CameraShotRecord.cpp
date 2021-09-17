/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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

#include "CameraShotRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

// constants for camera action
const uint32_t CameraShotRecord::cActionShoot = 0x00000000;
const uint32_t CameraShotRecord::cActionFly   = 0x00000001;
const uint32_t CameraShotRecord::cActionHit   = 0x00000002;
const uint32_t CameraShotRecord::cActionZoom  = 0x00000003;

// constants for location/target
const uint32_t CameraShotRecord::cLocationAttacker   = 0x00000000;
const uint32_t CameraShotRecord::cLocationProjectile = 0x00000001;
const uint32_t CameraShotRecord::cLocationTarget     = 0x00000002;
const uint32_t CameraShotRecord::cLocationLeadActor  = 0x00000003;

CameraShotRecord::CameraShotRecord()
: BasicRecord(), editorID(""),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  // subrecord DATA
  dataLen(DataLengthType::dlt40Byte),
  cameraAction(cActionShoot),
  cameraLocation(cLocationAttacker),
  cameraTarget(cLocationTarget),
  cameraFlags(0),
  timeMultPlayer(1.0f),
  timeMultTarget(1.0f),
  timeMultGlobal(1.0f),
  maxTime(20.0f),
  minTime(0.0f),
  targetPercentBetweenActors(0.0f),
  nearTargetDistance(0.0f),
  // end of subrecord DATA
  imageSpaceModFormID(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool CameraShotRecord::equals(const CameraShotRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (modelPath == other.modelPath) && (unknownMODT == other.unknownMODT)
      // subrecord DATA
      && (dataLen == other.dataLen) && (cameraAction == other.cameraAction)
      && (cameraLocation == other.cameraLocation) && (cameraTarget == other.cameraTarget)
      && (cameraFlags == other.cameraFlags) && (timeMultPlayer == other.timeMultPlayer)
      && (timeMultTarget == other.timeMultTarget) && (timeMultGlobal == other.timeMultGlobal)
      && (maxTime == other.maxTime) && (minTime == other.minTime)
      && (targetPercentBetweenActors == other.targetPercentBetweenActors)
      && (nearTargetDistance == other.nearTargetDistance)
      // end of subrecord data
      && (imageSpaceModFormID == other.imageSpaceModFormID);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CameraShotRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
      + editorID.length() + 1 /* length of name +1 byte for NUL termination */
      + 4 /* DATA */ + 2 /* 2 bytes for length */
      + static_cast<uint16_t>(dataLen) /* length */;
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */
        + modelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODL */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* length */;
  }
  if (imageSpaceModFormID != 0)
  {
    writeSize = writeSize + 4 /* MNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length */;
  }
  return writeSize;
}

bool CameraShotRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cCAMS), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  if (!modelPath.empty())
  {
    // write model path (MODL)
    output.write(reinterpret_cast<const char*>(&cMODL), 4);
    subLength = modelPath.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(modelPath.c_str(), subLength);
  }

  if (unknownMODT.isPresent())
  {
    // write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of CAMS!\n";
      return false;
    }
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = static_cast<uint16_t>(dataLen);
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write cam shot data
  output.write(reinterpret_cast<const char*>(&cameraAction), 4);
  output.write(reinterpret_cast<const char*>(&cameraLocation), 4);
  output.write(reinterpret_cast<const char*>(&cameraTarget), 4);
  output.write(reinterpret_cast<const char*>(&cameraFlags), 4);
  output.write(reinterpret_cast<const char*>(&timeMultPlayer), 4);
  output.write(reinterpret_cast<const char*>(&timeMultTarget), 4);
  output.write(reinterpret_cast<const char*>(&timeMultGlobal), 4);
  output.write(reinterpret_cast<const char*>(&maxTime), 4);
  output.write(reinterpret_cast<const char*>(&minTime), 4);
  output.write(reinterpret_cast<const char*>(&targetPercentBetweenActors), 4);
  if (dataLen >= DataLengthType::dlt44Byte)
  {
    output.write(reinterpret_cast<const char*>(&nearTargetDistance), 4);
  }

  if (imageSpaceModFormID != 0)
  {
    // write image space modifier form ID (MNAM)
    output.write(reinterpret_cast<const char*>(&cMNAM), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&imageSpaceModFormID), 4);
  }

  return output.good();
}
#endif

bool CameraShotRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  // read editor ID (EDID)
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  imageSpaceModFormID = 0;
  while (bytesRead < readSize)
  {
    // read next subrecord's name
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: CAMS seems to have more than one MODL subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
           {
             return false;
           }
           if (modelPath.empty())
           {
             std::cerr << "Error: subrecord MODL of CAMS is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: CAMS seems to have more than one MODT subrecord!\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of CAMS!\n";
             return false;
           }
           bytesRead += (2 + unknownMODT.size());
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: CAMS seems to have more than one DATA subrecord!\n";
             return false;
           }
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if ((subLength != 40) && (subLength != 44))
           {
             std::cerr << "Error: sub record DATA of CAMS has invalid length ("
                       << subLength << " bytes). Should be 40 or 44 bytes!\n";
             return false;
           }
           // now load DATA
           in_File.read(reinterpret_cast<char*>(&cameraAction), 4);
           in_File.read(reinterpret_cast<char*>(&cameraLocation), 4);
           in_File.read(reinterpret_cast<char*>(&cameraTarget), 4);
           in_File.read(reinterpret_cast<char*>(&cameraFlags), 4);
           in_File.read(reinterpret_cast<char*>(&timeMultPlayer), 4);
           in_File.read(reinterpret_cast<char*>(&timeMultTarget), 4);
           in_File.read(reinterpret_cast<char*>(&timeMultGlobal), 4);
           in_File.read(reinterpret_cast<char*>(&maxTime), 4);
           in_File.read(reinterpret_cast<char*>(&minTime), 4);
           in_File.read(reinterpret_cast<char*>(&targetPercentBetweenActors), 4);
           if (subLength >= 44)
           {
             in_File.read(reinterpret_cast<char*>(&nearTargetDistance), 4);
             dataLen = DataLengthType::dlt44Byte;
             // check allowed range right here
             if ((nearTargetDistance > 2000.0f) || (nearTargetDistance < 0.0f))
             {
               std::cerr << "Error while reading subrecord DATA of CAMS: near "
                         << "target distanct is not in range [0;2000]! Its "
                         << "current value is " << nearTargetDistance << ".\n";
               return false;
             }
           }
           else
           {
             nearTargetDistance = 0.0f;
             dataLen = DataLengthType::dlt40Byte;
           }
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of CAMS!\n";
             return false;
           }
           // integrity check
           if ((cameraAction > 3) || (cameraLocation > 3) || (cameraTarget > 3))
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: camera "
                       << "action, location or target is invalid!\n";
             return false;
           }
           if (cameraFlags > 0x3F)
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: unknown"
                       << " flag values set!\n";
             return false;
           }
           if ((timeMultPlayer > 20.0f) || (timeMultPlayer < 0.0f))
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: time "
                       << " multiplier for player is not in range [0;20]! Its "
                       << "current value is " << timeMultPlayer << ".\n";
             return false;
           }
           if ((timeMultTarget > 20.0f) || (timeMultTarget < 0.0f))
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: time "
                       << " multiplier for target is not in range [0;20]! Its "
                       << "current value is " << timeMultTarget << ".\n";
             return false;
           }
           if ((timeMultGlobal > 1.0f) || (timeMultGlobal < 0.0f))
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: global "
                       << " time multiplier is not in range [0;1]! Its current"
                       << " value is " << timeMultGlobal << ".\n";
             return false;
           }
           if ((maxTime > 120.0f) || (maxTime < 0.0f))
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: max. "
                       << "time is not in range [0;120]! Its current value is "
                       << maxTime << ".\n";
             return false;
           }
           if ((minTime > 120.0f) || (minTime < 0.0f))
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: min. "
                       << "time is not in range [0;120]! Its current value is "
                       << minTime <<".\n";
             return false;
           }
           if ((targetPercentBetweenActors > 100.0f) || (targetPercentBetweenActors < 0.0f))
           {
             std::cerr << "Error while reading subrecord DATA of CAMS: target "
                       << "percentage between actors is not in range [0;100]! "
                       << "Its current value is " << targetPercentBetweenActors
                       << ".\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cMNAM:
           if (imageSpaceModFormID != 0)
           {
             std::cerr << "Error: CAMS seems to have more than one MNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, imageSpaceModFormID, false))
           {
             std::cerr << "Error while reading subrecord MNAM of CAMS!\n";
             return false;
           }
           bytesRead += 6;
           if (imageSpaceModFormID == 0)
           {
             std::cerr << "Error: subrecord MNAM of CAMS has value zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, DATA or MNAM are allowed here!\n";
           return false;
           break;
    }
  }

  // presence checks
  if (!hasReadDATA)
  {
    std::cerr << "Error: at least one required subrecord of CAMS is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t CameraShotRecord::getRecordType() const
{
  return cCAMS;
}

} // namespace
