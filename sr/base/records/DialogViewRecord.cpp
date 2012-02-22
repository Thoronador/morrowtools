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

#include "DialogViewRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

DialogViewRecord::DialogViewRecord()
: BasicRecord()
{
  editorID = "";
  unknownQNAM = 0;
  unknownBNAMs.clear();
  unknownTNAMs.clear();
  hasENAM = false;
  unknownENAM = 0;
  hasDNAM = false;
  unknownDNAM = 0;
}

DialogViewRecord::~DialogViewRecord()
{
  //empty
}

bool DialogViewRecord::equals(const DialogViewRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownQNAM==other.unknownQNAM) and (unknownBNAMs==other.unknownBNAMs)
      and (unknownTNAMs==other.unknownTNAMs)
      and (hasENAM==other.hasENAM) and ((unknownENAM==other.unknownENAM) or (!hasENAM))
      and (hasDNAM==other.hasDNAM) and ((unknownDNAM==other.unknownDNAM) or (!hasDNAM)));
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DialogViewRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* QNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +unknownBNAMs.size()*(4 /* BNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */)
        +unknownTNAMs.size()*(4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */);
  if (hasENAM)
  {
    writeSize = writeSize +4 /* ENAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }
  if (hasDNAM)
  {
    writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */ +1 /* fixed length of 1 byte */;
  }
  return writeSize;
}

bool DialogViewRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cDLVW, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write QNAM
  output.write((const char*) &cQNAM, 4);
  //QNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write QNAM's stuff
  output.write((const char*) &unknownQNAM, 4);

  unsigned int i;
  const unsigned int count = unknownBNAMs.size();
  for (i=0; i<count; ++i)
  {
    //write BNAM
    output.write((const char*) &cBNAM, 4);
    //BNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write BNAM's stuff
    output.write((const char*) &(unknownBNAMs[i]), 4);
  }//for

  const unsigned int c = unknownTNAMs.size();
  for (i=0; i<c; ++i)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write TNAM's stuff
    output.write((const char*) &(unknownTNAMs[i]), 4);
  }//for

  if (hasENAM)
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    //ENAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write ENAM's stuff
    output.write((const char*) &unknownENAM, 4);
  }//if has ENAM

  if (hasDNAM)
  {
    //write DNAM
    output.write((const char*) &cDNAM, 4);
    //DNAM's length
    subLength = 1; //fixed
    output.write((const char*) &subLength, 2);
    //write DNAM's stuff
    output.write((const char*) &unknownDNAM, 1);
  }//if DNAM

  return output.good();
}
#endif

bool DialogViewRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of DLVW is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of DLVW!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read QNAM
  if (!loadUint32SubRecordFromStream(in_File, cQNAM, unknownQNAM))
  {
    std::cout << "Error while reading subrecord QNAM of DLVW!\n";
    return false;
  }
  bytesRead += 10;

  unknownBNAMs.clear();
  unknownTNAMs.clear();
  uint32_t tempUint32;
  hasENAM = false; unknownENAM = 0;
  hasDNAM = false; unknownDNAM = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cBNAM:
           //BNAM's length
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cBNAM, tempUint32))
           {
             std::cout << "Error while reading subrecord BNAM of DLVW!\n";
             return false;
           }
           bytesRead += 6;
           unknownBNAMs.push_back(tempUint32);
           break;
      case cTNAM:
           //TNAM's length
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, tempUint32))
           {
             std::cout << "Error while reading subrecord TNAM of DLVW!\n";
             return false;
           }
           bytesRead += 6;
           unknownTNAMs.push_back(tempUint32);
           break;
      case cENAM:
           if (hasENAM)
           {
             std::cout << "Error: record DLVW seems to have more than one ENAM subrecord!\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cENAM, unknownENAM))
           {
             std::cout << "Error while reading subrecord ENAM of DLVW!\n";
             return false;
           }
           bytesRead += 6;
           hasENAM = true;
           break;
      case cDNAM:
           if (hasDNAM)
           {
             std::cout << "Error: record DLVW seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record DNAM of DLVW has invalid length ("
                       <<subLength<<" bytes). Should be one byte!\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) &unknownDNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of DLVW!\n";
             return false;
           }
           hasDNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only BNAM, TNAM, ENAM or DNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if ((!unknownBNAMs.empty()) and (!unknownTNAMs.empty()))
  {
    std::cout << "Error while reading DLVW: only BNAM or TNAM can be present!\n";
    return false;
  }

  return in_File.good();
}

int32_t DialogViewRecord::getRecordType() const
{
  return cDLVW;
}

} //namespace
