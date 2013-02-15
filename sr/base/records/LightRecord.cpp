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

#include "LightRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//flicker effect flags
const uint32_t LightRecord::cFlagFlicker               = 0x00000008;
const uint32_t LightRecord::cFlagPulse                 = 0x00000080;
//carry flags
const uint32_t LightRecord::cFlagCanBeCarried          = 0x00000002;
const uint32_t LightRecord::cFlagOffByDefault          = 0x00000020;
//type flags
const uint32_t LightRecord::cFlagShadowSpot            = 0x00000400;
const uint32_t LightRecord::cFlagShadowHemisphere      = 0x00000800;
const uint32_t LightRecord::cFlagShadowOmnidirectional = 0x00001000;


LightRecord::LightRecord()
: BasicRecord(), editorID(""),
  modelPath(""),
  nameStringID(0),
  //subrecord DATA
  time(0),
  radius(0),
  colour(0),
  flags(0),
  falloffExponent(0.0f),
  FOV(0.0f),
  nearClip(0.0f),
  unknownDATA_08(0),
  intensityAmplitude(0.0f),
  movementAmplitude(0),
  value(0),
  weight(0.0f),
  //end of subrecord DATA
  fadeTime(0.0f),
  soundFormID(0)
{
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(false);
}

LightRecord::~LightRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LightRecord::equals(const LightRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (nameStringID==other.nameStringID)
      and (time==other.time) and (radius==other.radius)
      and (colour==other.colour) and (flags==other.flags)
      and (falloffExponent==other.falloffExponent) and (FOV==other.FOV)
      and (nearClip==other.nearClip) and (unknownDATA_08==other.unknownDATA_08)
      and (intensityAmplitude==other.intensityAmplitude) and (movementAmplitude==other.movementAmplitude)
      and (value==other.value) and (weight==other.weight)
      and (fadeTime==other.fadeTime) and (soundFormID==other.soundFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LightRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */
        +4 /* DATA */ +2 /* 2 bytes for length */ +48 /* fixed size of 48 bytes */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */
        +unknownMODT.getSize() /* length of MODT data */;
  }
  if (nameStringID!=0)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  if (soundFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  return writeSize;
}

bool LightRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cLIGH, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND stuff
  output.write((const char*) unknownOBND, 12);

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
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of LIGH!\n";
      return false;
    }
  }//if MODT

  if (nameStringID!=0)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FULL's stuff
    output.write((const char*) &nameStringID, 4);
  }//if FULL

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 48; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &time, 4);
  output.write((const char*) &radius, 4);
  output.write((const char*) &colour, 4);
  output.write((const char*) &flags, 4);
  output.write((const char*) &falloffExponent, 4);
  output.write((const char*) &FOV, 4);
  output.write((const char*) &nearClip, 4);
  output.write((const char*) &unknownDATA_08, 4);
  output.write((const char*) &intensityAmplitude, 4);
  output.write((const char*) &movementAmplitude, 4);
  output.write((const char*) &value, 4);
  output.write((const char*) &weight, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &fadeTime, 4);

  if (soundFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write SNAM's stuff
    output.write((const char*) &soundFormID, 4);
  }//if SNAM

  return output.good();
}
#endif

bool LightRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
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
    std::cout <<"Error: sub record EDID of LIGH is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of LIGH!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: sub record OBND of LIGH has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  memset(unknownOBND, 0, 12);
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of LIGH!\n";
    return false;
  }

  modelPath.clear();
  unknownMODT.setPresence(false);
  nameStringID = 0;
  bool hasReadDATA = false;
  bool hasReadFNAM = false;
  soundFormID = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: record LIGH seems to have more than one MODL subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           //consistency check
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of LIGH contains empty string!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: record LIGH seems to have more than one MODT subrecord!\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of LIGH!\n";
             return false;
           }
           bytesRead += (2 + unknownMODT.getSize());
           break;
      case cFULL:
           if (nameStringID!=0)
           {
             std::cout << "Error: record LIGH seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID, false))
             return false;
           //check value
           if (nameStringID==0)
           {
             std::cout << "Error: subrecord FULL of LIGH has invalid value zero!\n";
             return false;
           }
           bytesRead += 6;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: record LIGH seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=48)
           {
             std::cout <<"Error: sub record DATA of LIGH has invalid length ("
                       <<subLength<<" bytes). Should be 48 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &time, 4);
           in_File.read((char*) &radius, 4);
           in_File.read((char*) &colour, 4);
           in_File.read((char*) &flags, 4);
           in_File.read((char*) &falloffExponent, 4);
           in_File.read((char*) &FOV, 4);
           in_File.read((char*) &nearClip, 4);
           in_File.read((char*) &unknownDATA_08, 4);
           in_File.read((char*) &intensityAmplitude, 4);
           in_File.read((char*) &movementAmplitude, 4);
           in_File.read((char*) &value, 4);
           in_File.read((char*) &weight, 4);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of LIGH!\n";
             return false;
           }
           bytesRead += 48;
           hasReadDATA = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: record LIGH seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record FNAM of LIGH has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read FNAM's stuff
           in_File.read((char*) &fadeTime, 4);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of LIGH!\n";
             return false;
           }
           bytesRead += 4;
           hasReadFNAM = true;
           break;
      case cSNAM:
           if (soundFormID!=0)
           {
             std::cout << "Error: record LIGH seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, soundFormID, false))
             return false;
           //check value
           if (soundFormID==0)
           {
             std::cout << "Error: subrecord SNAM of LIGH has invalid value zero!\n";
             return false;
           }
           bytesRead += 6;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, FULL, DATA, FNAM or SNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadDATA and hasReadFNAM))
  {
    std::cout << "Error: At least one required subrecord of LIGH is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t LightRecord::getRecordType() const
{
  return cLIGH;
}

bool LightRecord::canBeCarried() const
{
  return ((flags & cFlagCanBeCarried) != 0);
}

bool LightRecord::offByDefault() const
{
  return ((flags & cFlagOffByDefault) != 0);
}

LightRecord::FlickerEffectType LightRecord::getFlickerType() const
{
  if ((flags & cFlagFlicker) != 0) return fetFlicker;
  if ((flags & cFlagPulse) != 0) return fetPulse;
  return fetNone;
}

LightRecord::LightType LightRecord::getLightType() const
{
  if ((flags & cFlagShadowSpot) != 0) return ltShadowSpot;
  if ((flags & cFlagShadowHemisphere) != 0) return ltShadowHemisphere;
  if ((flags & cFlagShadowOmnidirectional) != 0) return ltShadowOmnidirectional;
  return ltOmnidirectional;
}

} //namespace
