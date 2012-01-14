/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "EncounterZoneRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

EncounterZoneRecord::EncounterZoneRecord()
: BasicRecord()
{
  editorID = "";
  unknownDATA.setPresence(false);
}

EncounterZoneRecord::~EncounterZoneRecord()
{
  //empty
}

bool EncounterZoneRecord::equals(const EncounterZoneRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownDATA==other.unknownDATA));
}

uint32_t EncounterZoneRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (unknownDATA.isPresent())
  {
    writeSize = writeSize +4 /* DATA */ +2 /* 2 bytes for length */ +unknownDATA.getSize() /* size */;
  }
  return writeSize;
}

bool EncounterZoneRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cECZN, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownDATA.isPresent())
  {
    if (!unknownDATA.saveToStream(output, cDATA))
    {
      std::cout << "Error while writing subrecord DATA of ECZN!\n";
      return false;
    }
  }//if ECZN

  return output.good();
}

bool EncounterZoneRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

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
    std::cout <<"Error: sub record EDID of ECZN is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ECZN!\n";
    return false;
  }
  editorID = std::string(buffer);

  if (!unknownDATA.loadFromStream(in_File, cDATA, true))
  {
    std::cout << "Error while reading subrecord DATA of ECZN!\n";
    return false;
  }

  return in_File.good();
}

int32_t EncounterZoneRecord::getRecordType() const
{
  return cECZN;
}

} //namespace