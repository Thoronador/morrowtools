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

#include "ColourFormRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ColourFormRecord::ColourFormRecord()
: BasicRecord()
{
  editorID = "";
  hasFULL = false;
  nameStringID = 0;
  unknownCNAM = 0;
  unknownFNAM = 0;
}

ColourFormRecord::~ColourFormRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ColourFormRecord::equals(const ColourFormRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (unknownCNAM==other.unknownCNAM) and (unknownFNAM==other.unknownFNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ColourFormRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool ColourFormRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cCLFM, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write FULL's data
    output.write((const char*) &nameStringID, 4);
  }//if has FULL

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write CNAM's data
  output.write((const char*) &unknownCNAM, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write FNAM's data
  output.write((const char*) &unknownFNAM, 4);

  return output.good();
}
#endif

bool ColourFormRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
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
    std::cout <<"Error: sub record EDID of CLFM is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of CLFM!\n";
    return false;
  }
  editorID = std::string(buffer);

  hasFULL = false;
  nameStringID = 0;
  bool hasReadCNAM = false;
  bool hasReadFNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: CLFM seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: CLFM seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, false)) return false;
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: CLFM seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //read FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, unknownFNAM, false)) return false;
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, CNAM or FNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence check
  if (!(hasReadCNAM  and hasReadFNAM))
  {
    std::cout << "Error: while reading CLFM record: at least one required subrecord is missing!\n";
    return false;
  }//if

  return in_File.good();
}

uint32_t ColourFormRecord::getRecordType() const
{
  return cCLFM;
}

} //namespace
