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

#include "ShoutRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* WordEntry's functions */

bool ShoutRecord::WordEntry::operator==(const ShoutRecord::WordEntry& other) const
{
  return ((wordFormID==other.wordFormID) and (spellFormID==other.spellFormID)
      and (recharge==other.recharge));
}

/* ShoutRecord's funtions */

ShoutRecord::ShoutRecord()
: BasicRecord(), editorID(""),
  hasFULL(false), fullNameStringID(0),
  menuDisplayObjectFormID(0),
  descriptionStringID(0),
  words(std::vector<WordEntry>())
{

}

ShoutRecord::~ShoutRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ShoutRecord::equals(const ShoutRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasFULL==other.hasFULL) and ((fullNameStringID==other.fullNameStringID) or (!hasFULL))
      and (menuDisplayObjectFormID==other.menuDisplayObjectFormID)
      and (descriptionStringID==other.descriptionStringID) and (words==other.words));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ShoutRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of strin +1 byte for NUL-termination */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +words.size()
        *(4 /* SNAM */ +2 /* 2 bytes for length */ +12 /* fixed size */);
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (menuDisplayObjectFormID!=0)
  {
    writeSize = writeSize +4 /* MDOB */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool ShoutRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cSHOU, 4);
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
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write FULL
    output.write((const char*) &fullNameStringID, 4);
  }

  if (menuDisplayObjectFormID!=0)
  {
    //write MDOB
    output.write((const char*) &cMDOB, 4);
    //MDOB's length
    subLength = 4; // fixed
     output.write((const char*) &subLength, 2);
    //write Menu Display Object's form ID
    output.write((const char*) &menuDisplayObjectFormID, 4);
  }//if has MDOB

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; // fixed
  output.write((const char*) &subLength, 2);
  //write DESC
  output.write((const char*) &descriptionStringID, 4);

  unsigned int i;
  for (i=0; i<words.size(); ++i)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 12; //fixed size of 12 bytes
    output.write((const char*) &subLength, 2);
    //write SNAM's data
    output.write((const char*) &(words[i].wordFormID), 4);
    output.write((const char*) &(words[i].spellFormID), 4);
    output.write((const char*) &(words[i].recharge), 4);
  }//for

  return output.good();
}
#endif

bool ShoutRecord::loadFromStream(std::ifstream& in_File)
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

  hasFULL = false; fullNameStringID = 0;
  menuDisplayObjectFormID = 0;
  bool hasReadDESC = false;
  words.clear();
  WordEntry temp;

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
           if (!loadUint32SubRecordFromStream(in_File, cFULL, fullNameStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMDOB:
           if (menuDisplayObjectFormID!=0)
           {
             std::cout << "Error: SHOU seems to have more than one MDOB subrecord.\n";
             return false;
           }
           //read MDOB
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, menuDisplayObjectFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (menuDisplayObjectFormID==0)
           {
             std::cout << "Error: subrecord MDOB of SHOU is zero!\n";
             return false;
           }
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: SHOU seems to have more than one DESC subrecord.\n";
             return false;
           }
           //read DESC
           if (!loadUint32SubRecordFromStream(in_File, cDESC, descriptionStringID, false)) return false;
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
           temp.wordFormID = 0;
           temp.spellFormID = 0;
           temp.recharge = 0.0f;
           in_File.read((char*) &(temp.wordFormID), 4);
           in_File.read((char*) &(temp.spellFormID), 4);
           in_File.read((char*) &(temp.recharge), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SNAM of SHOU!\n";
             return false;
           }
           words.push_back(temp);
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MDOB, DESC or SNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  //check for presence of required elements
  if ((!hasReadDESC) or (words.size()!=3))
  {
    std::cout << "Error: at least one subrecord of SHOU is not present!\n"
              << "hasDESC: "<<hasReadDESC<<", SNAM.size="<<words.size()<<"\n";
    return false;
  }

  return in_File.good();
}

uint32_t ShoutRecord::getRecordType() const
{
  return cSHOU;
}

} //namespace
