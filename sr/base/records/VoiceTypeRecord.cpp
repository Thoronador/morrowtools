/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "VoiceTypeRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

VoiceTypeRecord::VoiceTypeRecord()
: BasicRecord()
{
  editorID = "";
  unknownDNAM = 0;
}

VoiceTypeRecord::~VoiceTypeRecord()
{
  //empty
}

bool VoiceTypeRecord::equals(const VoiceTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownDNAM==other.unknownDNAM));
}

bool VoiceTypeRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cVTYP, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +1 /* fixed length */;
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write DNAM's stuff
  output.write((const char*) &unknownDNAM, 1);

  return output.good();
}

bool VoiceTypeRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
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
    std::cout <<"Error: sub record EDID of VTYP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of VTYP!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read DNAM
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cDNAM)
  {
    UnexpectedRecord(cDNAM, subRecName);
    return false;
  }
  //DNAM's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=1)
  {
    std::cout <<"Error: subrecord DNAM of VTYP has invalid length ("<<subLength
              <<" bytes). Should be one byte!\n";
    return false;
  }
  //read DNAM's stuff
  in_File.read((char*) &unknownDNAM, 1);
  bytesRead += 1;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DNAM of VTYP!\n";
    return false;
  }

  return in_File.good();
}

int32_t VoiceTypeRecord::getRecordType() const
{
  return cVTYP;
}

    std::string editorID;
    uint8_t unknownDNAM;

} //namespace
