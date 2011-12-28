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

#include "FootstepRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

FootstepRecord::FootstepRecord()
: BasicRecord()
{
  editorID = "";
  unknownDATA = 0;
  unknownANAM = "";
}

FootstepRecord::~FootstepRecord()
{
  //empty
}

bool FootstepRecord::equals(const FootstepRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownDATA==other.unknownDATA) and (unknownANAM==other.unknownANAM));
}

uint32_t FootstepRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* ANAM */ +2 /* 2 bytes for length */
        +unknownANAM.length()+1 /* length of name +1 byte for NUL termination */);
}

bool FootstepRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cFSTP, 4);
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
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &unknownDATA, 4);

  //write ANAM
  output.write((const char*) &cANAM, 4);
  //ANAM's length
  subLength = unknownANAM.length()+1;
  output.write((const char*) &subLength, 2);
  //write ANAM
  output.write(unknownANAM.c_str(), subLength);

  return output.good();
}

bool FootstepRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
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
    std::cout <<"Error: sub record EDID of FSTP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of FSTP!\n";
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
  if (subLength!=4)
  {
    std::cout <<"Error: sub record DATA of FSTP has invalid length ("
              <<subLength<<" bytes). Should be four bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) &unknownDATA, 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of FSTP!\n";
    return false;
  }

  //read ANAM
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cANAM)
  {
    UnexpectedRecord(cANAM, subRecName);
    return false;
  }
  //ANAM's length
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout <<"Error: sub record ANAM of FSTP is longer than 511 characters!\n";
    return false;
  }
  //read ANAM's stuff
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord ANAM of FSTP!\n";
    return false;
  }
  unknownANAM = std::string(buffer);

  return in_File.good();
}

int32_t FootstepRecord::getRecordType() const
{
  return cFSTP;
}

} //namespace