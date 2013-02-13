/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "CameraShotRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//constants for camera action
const uint32_t CameraShotRecord::cActionShoot = 0x00000000;
const uint32_t CameraShotRecord::cActionFly   = 0x00000001;
const uint32_t CameraShotRecord::cActionHit   = 0x00000002;
const uint32_t CameraShotRecord::cActionZoom  = 0x00000003;

//constants for location/target
const uint32_t CameraShotRecord::cLocationAttacker   = 0x00000000;
const uint32_t CameraShotRecord::cLocationProjectile = 0x00000001;
const uint32_t CameraShotRecord::cLocationTarget     = 0x00000002;
const uint32_t CameraShotRecord::cLocationLeadActor  = 0x00000003;

CameraShotRecord::CameraShotRecord()
: BasicRecord(), editorID(""),
  modelPath(""),
  //subrecord DATA
  dataLen(dlt40Byte),
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
  //end of subrecord DATA
  imageSpaceModFormID(0)
{
  unknownMODT.setPresence(false);
}

CameraShotRecord::~CameraShotRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool CameraShotRecord::equals(const CameraShotRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      //subrecord DATA
      and (dataLen==other.dataLen) and (cameraAction==other.cameraAction)
      and (cameraLocation==other.cameraLocation) and (cameraTarget==other.cameraTarget)
      and (cameraFlags==other.cameraFlags) and (timeMultPlayer==other.timeMultPlayer)
      and (timeMultTarget==other.timeMultTarget) and (timeMultGlobal==other.timeMultGlobal)
      and (maxTime==other.maxTime) and (minTime==other.minTime)
      and (targetPercentBetweenActors==other.targetPercentBetweenActors)
      and (nearTargetDistance==other.nearTargetDistance)
      //end of subrecord data
      and (imageSpaceModFormID==other.imageSpaceModFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CameraShotRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* length */;
  }
  if (dataLen==dlt40Byte)
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +40 /* length */;
  }
  else
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +44 /* length */;
  }
  if (imageSpaceModFormID!=0)
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if MNAM
  return writeSize;
}

bool CameraShotRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cCAMS, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of CAMS!\n";
      return false;
    }
  }//if MODT

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = dataLen;
  output.write((const char*) &subLength, 2);
  //write cam shot data
  output.write((const char*) &cameraAction, 4);
  output.write((const char*) &cameraLocation, 4);
  output.write((const char*) &cameraTarget, 4);
  output.write((const char*) &cameraFlags, 4);
  output.write((const char*) &timeMultPlayer, 4);
  output.write((const char*) &timeMultTarget, 4);
  output.write((const char*) &timeMultGlobal, 4);
  output.write((const char*) &maxTime, 4);
  output.write((const char*) &minTime, 4);
  output.write((const char*) &targetPercentBetweenActors, 4);
  if (dataLen>=dlt44Byte)
  {
    output.write((const char*) &nearTargetDistance, 4);
  }

  if (imageSpaceModFormID!=0)
  {
    //write MNAM
    output.write((const char*) &cMNAM, 4);
    //MNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write image space modifier form ID
    output.write((const char*) &imageSpaceModFormID, 4);
  }//if MNAM

  return output.good();
}
#endif

bool CameraShotRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of CAMS is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of CAMS!\n";
    return false;
  }
  editorID = std::string(buffer);

  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  imageSpaceModFormID = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: CAMS seems to have more than one MODL subrecord!\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
           {
             return false;
           }
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of CAMS is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: CAMS seems to have more than one MODT subrecord!\n";
             return false;
           }
           //load MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of CAMS!\n";
             return false;
           }
           bytesRead += (2 + unknownMODT.getSize());
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: CAMS seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength!=40) and (subLength!=44))
           {
             std::cout << "Error: sub record DATA of CAMS has invalid length ("
                       << subLength<<" bytes). Should be 40 or 44 bytes!\n";
             return false;
           }
           //now load DATA
           in_File.read((char*) &cameraAction, 4);
           in_File.read((char*) &cameraLocation, 4);
           in_File.read((char*) &cameraTarget, 4);
           in_File.read((char*) &cameraFlags, 4);
           in_File.read((char*) &timeMultPlayer, 4);
           in_File.read((char*) &timeMultTarget, 4);
           in_File.read((char*) &timeMultGlobal, 4);
           in_File.read((char*) &maxTime, 4);
           in_File.read((char*) &minTime, 4);
           in_File.read((char*) &targetPercentBetweenActors, 4);
           if (subLength>=44)
           {
             in_File.read((char*) &nearTargetDistance, 4);
             dataLen = dlt44Byte;
             //check it right here
             if ((nearTargetDistance>2000.0f) or (nearTargetDistance<0.0f))
             {
               std::cout << "Error while reading subrecord DATA of CAMS: near "
                         << "target distanct is not in range [0;100]!Its "
                         << "current value is "<< nearTargetDistance<<".\n";
               return false;
             }//if (range check)
           }
           else
           {
             nearTargetDistance = 0.0f;
             dataLen = dlt40Byte;
           }
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of CAMS!\n";
             return false;
           }
           //integrity check
           if ((cameraAction>3) or (cameraLocation>3) or (cameraTarget>3))
           {
             std::cout << "Error while reading subrecord DATA of CAMS: camera "
                       << "action, location or target is invalid!\n";
             return false;
           }
           if (cameraFlags>0x3F)
           {
             std::cout << "Error while reading subrecord DATA of CAMS: unknown"
                       << " flag values set!\n";
             return false;
           }
           if ((timeMultPlayer>20.0f) or (timeMultPlayer<0.0f))
           {
             std::cout << "Error while reading subrecord DATA of CAMS: time "
                       << " multiplier for player is not in range [0;20]! Its "
                       << "current value is "<<timeMultPlayer<<".\n";
             return false;
           }
           if ((timeMultTarget>20.0f) or (timeMultTarget<0.0f))
           {
             std::cout << "Error while reading subrecord DATA of CAMS: time "
                       << " multiplier for target is not in range [0;20]! Its "
                       << "current value is "<<timeMultTarget<<".\n";
             return false;
           }
           if ((timeMultGlobal>1.0f) or (timeMultGlobal<0.0f))
           {
             std::cout << "Error while reading subrecord DATA of CAMS: global "
                       << " time multiplier is not in range [0;1]! Its current"
                       << " value is "<<timeMultGlobal<<".\n";
             return false;
           }
           if ((maxTime>120.0f) or (maxTime<0.0f))
           {
             std::cout << "Error while reading subrecord DATA of CAMS: max. "
                       << "time is not in range [0;120]! Its current value is "
                       << maxTime<<".\n";
             return false;
           }
           if ((minTime>120.0f) or (minTime<0.0f))
           {
             std::cout << "Error while reading subrecord DATA of CAMS: min. "
                       << "time is not in range [0;120]! Its current value is "
                       << minTime<<".\n";
             return false;
           }
           if ((targetPercentBetweenActors>100.0f) or (targetPercentBetweenActors<0.0f))
           {
             std::cout << "Error while reading subrecord DATA of CAMS: target "
                       << "percentage between actors is not in range [0;100]! "
                       << "Its current value is "<< targetPercentBetweenActors
                       <<".\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cMNAM:
           if (imageSpaceModFormID!=0)
           {
             std::cout << "Error: CAMS seems to have more than one MNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, imageSpaceModFormID, false))
           {
             std::cout << "Error while reading subrecord MNAM of CAMS!\n";
             return false;
           }
           bytesRead += 6;
           if (imageSpaceModFormID==0)
           {
             std::cout << "Error: subrecord MNAM of CAMS has value zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, DATA or MNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!hasReadDATA)
  {
    std::cout << "Error: at least one required subrecord of CAMS is missing!\n";
    std::cout << "DATA: "<< hasReadDATA<<"\n";
    return false;
  }

  return in_File.good();
}

uint32_t CameraShotRecord::getRecordType() const
{
  return cCAMS;
}

} //namespace
