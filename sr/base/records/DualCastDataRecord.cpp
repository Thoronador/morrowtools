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

#include "DualCastDataRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* constants for flag values */
const uint32_t DualCastDataRecord::cFlagHitEffectArtInherits = 0x00000001;
const uint32_t DualCastDataRecord::cFlagProjectileInherits   = 0x00000002;
const uint32_t DualCastDataRecord::cFlagExplosionInherits    = 0x00000004;

DualCastDataRecord::DualCastDataRecord()
: BasicRecord(), editorID(""),
  //subrecord DATA
  projectileFormID(0),
  explosionFormID(0),
  effectShaderFormID(0),
  hitEffectArtFormID(0),
  impactDataSetFormID(0),
  inheritFlags(0)
  //end of DATA
{
  memset(unknownOBND, 0, 12);
}

DualCastDataRecord::~DualCastDataRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool DualCastDataRecord::equals(const DualCastDataRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (projectileFormID==other.projectileFormID) and (explosionFormID==other.explosionFormID)
      and (effectShaderFormID==other.effectShaderFormID) and (hitEffectArtFormID==other.hitEffectArtFormID)
      and (impactDataSetFormID==other.impactDataSetFormID) and (inheritFlags==other.inheritFlags));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DualCastDataRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* DATA */ +2 /* 2 bytes for length */ +24 /* fixed length of 24 bytes */);
}

bool DualCastDataRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cDUAL, 4);
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
  subLength = 12; //fixed length
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 24; //fixed length
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &projectileFormID, 4);
  output.write((const char*) &explosionFormID, 4);
  output.write((const char*) &effectShaderFormID, 4);
  output.write((const char*) &hitEffectArtFormID, 4);
  output.write((const char*) &impactDataSetFormID, 4);
  output.write((const char*) &inheritFlags, 4);

  return output.good();
}
#endif

bool DualCastDataRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of DUAL is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of DUAL!\n";
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
    std::cout <<"Error: sub record OBND of DUAL has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of DUAL!\n";
    return false;
  }

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
  if (subLength!=24)
  {
    std::cout <<"Error: sub record DATA of DUAL has invalid length ("<<subLength
              <<" bytes). Should be 24 bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) &projectileFormID, 4);
  in_File.read((char*) &explosionFormID, 4);
  in_File.read((char*) &effectShaderFormID, 4);
  in_File.read((char*) &hitEffectArtFormID, 4);
  in_File.read((char*) &impactDataSetFormID, 4);
  in_File.read((char*) &inheritFlags, 4);
  bytesRead += 24;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of DUAL!\n";
    return false;
  }

  return true;
}

uint32_t DualCastDataRecord::getRecordType() const
{
  return cDUAL;
}

bool DualCastDataRecord::hitEffectArtInheritsScale() const
{
  return ((inheritFlags & cFlagHitEffectArtInherits)!=0);
}

bool DualCastDataRecord::projectileInheritsScale() const
{
  return ((inheritFlags & cFlagProjectileInherits)!=0);
}

bool DualCastDataRecord::explosionInheritsScale() const
{
  return ((inheritFlags & cFlagExplosionInherits)!=0);
}

} //namespace
