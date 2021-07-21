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

#include "ReverbRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

const float ReverbRecord::cReflectDelayScale = 0.83;

ReverbRecord::ReverbRecord()
: BasicRecord(), editorID(""),
  //start of data subrecord
  decayTime(0), //in ms
  HF_reference(0), //in Hz
  roomFilter(0),
  roomHF_filter(0),
  reflections(0),
  reverbAmp(0),
  decayHF_ratio100(0), // x100, real value is one hundreth of that
  reflectDelayScaled(0), //scaled by approx. 0.83 - value 0x00 maps to 0, 0xF9 maps to 300
  reverbDelay(0), //in ms
  diffusionPercentage(0), //in %
  densitiyPercentage(0), //in %
  unknownDATA_unused(0)
  //end of DATA
{

}

ReverbRecord::~ReverbRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ReverbRecord::equals(const ReverbRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (decayTime==other.decayTime) and (HF_reference==other.HF_reference)
      and (roomFilter==other.roomFilter) and (roomHF_filter==other.roomHF_filter)
      and (reflections==other.reflections) and (reverbAmp==other.reverbAmp)
      and (decayHF_ratio100==other.decayHF_ratio100) and (reflectDelayScaled==other.reflectDelayScaled)
      and (reverbDelay==other.reverbDelay) and (diffusionPercentage==other.diffusionPercentage)
      and (densitiyPercentage==other.densitiyPercentage) and (unknownDATA_unused==other.unknownDATA_unused));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ReverbRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +14 /* fixed size */);
}

bool ReverbRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cREVB, 4);
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
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &decayTime, 2);
  output.write((const char*) &HF_reference, 2);
  output.write((const char*) &roomFilter, 1);
  output.write((const char*) &roomHF_filter, 1);
  output.write((const char*) &reflections, 1);
  output.write((const char*) &reverbAmp, 1);
  output.write((const char*) &decayHF_ratio100, 1);
  output.write((const char*) &reflectDelayScaled, 1);
  output.write((const char*) &reverbDelay, 1);
  output.write((const char*) &diffusionPercentage, 1);
  output.write((const char*) &densitiyPercentage, 1);
  output.write((const char*) &unknownDATA_unused, 1);

  return output.good();
}
#endif

bool ReverbRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of REVB is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of REVB!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read DATA
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=14)
  {
    std::cout <<"Error: sub record DATA of REVB has invalid length ("
              <<subLength<<" bytes). Should be 14 bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) &decayTime, 2);
  in_File.read((char*) &HF_reference, 2);
  in_File.read((char*) &roomFilter, 1);
  in_File.read((char*) &roomHF_filter, 1);
  in_File.read((char*) &reflections, 1);
  in_File.read((char*) &reverbAmp, 1);
  in_File.read((char*) &decayHF_ratio100, 1);
  in_File.read((char*) &reflectDelayScaled, 1);
  in_File.read((char*) &reverbDelay, 1);
  in_File.read((char*) &diffusionPercentage, 1);
  in_File.read((char*) &densitiyPercentage, 1);
  in_File.read((char*) &unknownDATA_unused, 1);

  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of REVB!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ReverbRecord::getRecordType() const
{
  return cREVB;
}

float ReverbRecord::getDecayHFRatio() const
{
  return (static_cast<float>(decayHF_ratio100)/100.0f);
}

uint16_t ReverbRecord::getReflectDelay() const
{
  return static_cast<uint16_t>(reflectDelayScaled/cReflectDelayScale);
}

} //namespace
