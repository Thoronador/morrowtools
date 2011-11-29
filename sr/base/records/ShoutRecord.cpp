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

#include "ShoutRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

bool ShoutRecord::SNAMentry::operator==(const ShoutRecord::SNAMentry& other) const
{
  return (memcmp(data, other.data, 12)==0);
}

ShoutRecord::ShoutRecord()
{
  editorID = "";
  hasFULL = false;
  fullNameStringID = 0;
  hasMDOB = false;
  unknownMDOB = 0;
  descriptionStringID = 0;
  unknownSNAMs.clear();
}

ShoutRecord::~ShoutRecord()
{
  //empty
}

bool ShoutRecord::equals(const ShoutRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasFULL==other.hasFULL) and ((fullNameStringID==other.fullNameStringID) or (!hasFULL))
      and (hasMDOB==other.hasMDOB) and ((unknownMDOB==other.unknownMDOB) or (!hasMDOB))
      and (descriptionStringID==other.descriptionStringID) and (unknownSNAMs==other.unknownSNAMs));
}

bool ShoutRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cSHOU, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of strin +1 byte for NUL-termination */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +unknownSNAMs.size()
        *(4 /* SNAM */ +2 /* 2 bytes for length */ +12 /* fixed size */);
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasMDOB)
  {
    writeSize = writeSize +4 /* MDOB */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (hasFULL)
  {
    //write FULL
    output.write((char*) &cFULL, 4);
    //FULL's length
    subLength = 4; // fixed
    output.write((char*) &subLength, 2);
    //write FULL
    output.write((const char*) &fullNameStringID, 4);
  }

  if (hasMDOB)
  {
    //write MDOB
    output.write((char*) &cMDOB, 4);
    //MDOB's length
    subLength = 4; // fixed
     output.write((char*) &subLength, 2);
    //write MDOB's data
    output.write((const char*) &unknownMDOB, 4);
  }//if has MDOB

  //write DESC
  output.write((char*) &cDESC, 4);
  //DESC's length
  subLength = 4; // fixed
  output.write((char*) &subLength, 2);
  //write DESC
  output.write((const char*) &descriptionStringID, 4);

  unsigned int i;
  for (i=0; i<unknownSNAMs.size(); ++i)
  {
    //write SNAM
    output.write((char*) &cSNAM, 4);
    //SNAM's length
    subLength = 12; //fixed size of 12 bytes
    output.write((char*) &subLength, 2);
    //write SNAM's data
    output.write((const char*) (unknownSNAMs[i].data), subLength);
  }//for

  return output.good();
}

bool ShoutRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of SHOU is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SHOU!\n";
    return false;
  }
  editorID = std::string(buffer);

  hasFULL = false;
  hasMDOB = false;
  bool hasReadDESC = false;
  unknownSNAMs.clear();
  SNAMentry temp;

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
             std::cout << "Error: SHOU seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cFULL, fullNameStringID)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMDOB:
           if (hasMDOB)
           {
             std::cout << "Error: SHOU seems to have more than one MDOB subrecord.\n";
             return false;
           }
           //read MDOB
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, unknownMDOB)) return false;
           bytesRead += 6;
           hasMDOB = true;
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: SHOU seems to have more than one DESC subrecord.\n";
             return false;
           }
           //read DESC
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cDESC, descriptionStringID)) return false;
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cSNAM:
           //SNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record SNAM of SHOU has invalid length("
                       <<subLength<<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read SNAM's stuff
           memset(temp.data, 0, 12);
           in_File.read((char*) (temp.data), 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SNAM of SHOU!\n";
             return false;
           }
           unknownSNAMs.push_back(temp);
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MDOB, DESC or SNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  //check for presence of required elements
  if ((!hasReadDESC) or (unknownSNAMs.size()!=3))
  {
    std::cout << "Error: at least one subrecord of SHOU is not present!\n"
              << "hasDESC: "<<hasReadDESC<<", SNAM.size="<<unknownSNAMs.size()<<"\n";
    return false;
  }

  return in_File.good();
}

int32_t ShoutRecord::getRecordType() const
{
  return cSHOU;
}

} //namespace
