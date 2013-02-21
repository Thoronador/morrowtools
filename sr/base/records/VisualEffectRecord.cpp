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

#include "VisualEffectRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* constants for flag values */
const uint32_t VisualEffectRecord::cFlagRotateToFaceTarget = 0x00000001;
const uint32_t VisualEffectRecord::cFlagAttachToCamera     = 0x00000002;
const uint32_t VisualEffectRecord::cFlagInheritRotation    = 0x00000004;

VisualEffectRecord::VisualEffectRecord()
: BasicRecord(), editorID(""),
  //subrecord DATA
  effectArtFormID(0),
  shaderFormID(0),
  flagsVFX(0)
  //end of DATA
{

}

VisualEffectRecord::~VisualEffectRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool VisualEffectRecord::equals(const VisualEffectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (effectArtFormID==other.effectArtFormID) and (shaderFormID==other.shaderFormID)
      and (flagsVFX==other.flagsVFX));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t VisualEffectRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */);
}

bool VisualEffectRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cRFCT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 12; //fixed length
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &effectArtFormID, 4);
  output.write((const char*) &shaderFormID, 4);
  output.write((const char*) &flagsVFX, 4);

  return output.good();
}
#endif

bool VisualEffectRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
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
    std::cout <<"Error: sub record EDID of RFCT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of RFCT!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read DATA
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: sub record DATA of RFCT has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read DATA
  in_File.read((char*) &effectArtFormID, 4);
  in_File.read((char*) &shaderFormID, 4);
  in_File.read((char*) &flagsVFX, 4);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of RFCT!\n";
    return false;
  }

  return true;
}

uint32_t VisualEffectRecord::getRecordType() const
{
  return cRFCT;
}

bool VisualEffectRecord::rotateToFaceTarget() const
{
  return ((flagsVFX & cFlagRotateToFaceTarget)!=0);
}

bool VisualEffectRecord::isAttachedToCamera() const
{
  return ((flagsVFX & cFlagAttachToCamera)!=0);
}

bool VisualEffectRecord::inheritsRotation() const
{
  return ((flagsVFX & cFlagInheritRotation)!=0);
}

} //namespace
