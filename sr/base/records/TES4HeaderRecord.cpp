/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "TES4HeaderRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

bool Tes4HeaderRecord::MasterFile::operator==(const Tes4HeaderRecord::MasterFile& other) const
{
  return ((data==other.data) and (fileName==other.fileName));
}

Tes4HeaderRecord::Tes4HeaderRecord()
: BasicRecord()
{
  version = 0.0f;
  numRecordsAndGroups = 0;
  nextObjectID = 0;
  authorName = "";
  dependencies.clear();
  unknownIntValue = 0;
}

Tes4HeaderRecord::~Tes4HeaderRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool Tes4HeaderRecord::equals(const Tes4HeaderRecord& other) const
{
  return ((equalsBasic(other)) and (version==other.version)
    and (numRecordsAndGroups==other.numRecordsAndGroups)
    and (nextObjectID==other.nextObjectID)
    and (authorName==other.authorName) and (dependencies==other.dependencies)
    and (unknownIntValue==other.unknownIntValue));
}
#endif

uint32_t Tes4HeaderRecord::getRecordType() const
{
  return cTES4;
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t Tes4HeaderRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* HEDR */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* CNAM */ +2 /* 2 bytes for length */
        +authorName.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* INTV */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  unsigned int i;
  for (i=0; i<dependencies.size(); ++i)
  {
    writeSize =  writeSize +4 /* MAST */ +2 /* 2 bytes for length */
        +dependencies[i].fileName.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length of 8 bytes */;
  }//for
  return writeSize;
}

bool Tes4HeaderRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cTES4, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write HEDR
  output.write((char*) &cHEDR, 4);
  //HEDR's length
  uint16_t SubLength = 12;
  output.write((char*) &SubLength, 2);
  //write HEDR's stuff
  // ---- write version
  output.write((char*) &version, 4);
  // ---- write unknown stuff
  output.write((char*) &numRecordsAndGroups, 4);
  output.write((char*) &nextObjectID, 4);
  if (!output.good())
  {
    std::cout << "Error while writing subrecord HEDR of TES4!\n";
    return false;
  }

  //write CNAM
  output.write((char*) &cCNAM, 4);
  //CNAM's length
  SubLength = authorName.length()+1;
  output.write((char*) &SubLength, 2);
  //write author's name
  output.write(authorName.c_str(), SubLength);

  unsigned int i;
  for (i=0; i<dependencies.size(); ++i)
  {
    //write MAST
    output.write((char*) &cMAST, 4);
    //MAST's length
    SubLength = dependencies[i].fileName.length()+1;
    output.write((char*) &SubLength, 2);
    //write file name
    output.write(dependencies[i].fileName.c_str(), SubLength);

    //write DATA
    output.write((char*) &cDATA, 4);
    //DATA's length
    SubLength = 8; //fixed length
    output.write((char*) &SubLength, 2);
    //write data value
    output.write((const char*) &(dependencies[i].data), 8);
  }//for

  //write INTV
  output.write((char*) &cINTV, 4);
  //INTV's length
  SubLength = 4; //fixed size
  output.write((char*) &SubLength, 2);
  //write integer valie
  output.write((const char*) &unknownIntValue, 4);

  return output.good();
}
#endif

bool Tes4HeaderRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t SubRecName;
  uint16_t SubLength;
  uint32_t BytesRead;
  SubRecName = SubLength = 0;

  //read HEDR
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cHEDR)
  {
    UnexpectedRecord(cHEDR, SubRecName);
    return false;
  }
  //HEDR's length
  in_File.read((char*) &SubLength, 2);
  BytesRead += 2;
  if (SubLength!=12)
  {
    std::cout <<"Error: sub record HEDR of TES4 has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read HEDR's stuff
  // ---- read version number
  in_File.read((char*) &version, 4);
  // ---- read unknown values
  in_File.read((char*) &numRecordsAndGroups, 4);
  in_File.read((char*) &nextObjectID, 4);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord HEDR of TES4!\n";
    return false;
  }

  //read CNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cCNAM)
  {
    UnexpectedRecord(cCNAM, SubRecName);
    return false;
  }
  //CNAM's length
  in_File.read((char*) &SubLength, 2);
  BytesRead += 2;
  if (SubLength>511)
  {
    std::cout << "Error: subrecord CNAM of TES4 is longer than 511 characters.\n";
    return false;
  }
  //read author's name
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CNAM of TES4!\n";
    return false;
  }
  authorName = std::string(Buffer);

  dependencies.clear();
  MasterFile depEntry;
  bool hasDoneINTV = false;
  while (BytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cMAST:
           //master file coming
           //MAST's length
           in_File.read((char*) &SubLength, 2);
           BytesRead += 2;
           if (SubLength>511)
           {
             std::cout << "Error: subrecord MAST of TES4 is longer than 511 characters.\n";
             return false;
           }
           //read master file name
           memset(Buffer, '\0', 512);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MAST of TES4!\n";
             return false;
           }
           depEntry.fileName = std::string(Buffer);

           //data coming next...
           //read DATA
           in_File.read((char*) &SubRecName, 4);
           BytesRead += 4;
           if (SubRecName!=cDATA)
           {
             UnexpectedRecord(cDATA, SubRecName);
             return false;
           }
           //DATA's length
           in_File.read((char*) &SubLength, 2);
           BytesRead += 2;
           if (SubLength!=8)
           {
             std::cout <<"Error: sub record DATA of TES4 has invalid length ("
                       <<SubLength <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &(depEntry.data), 8);
           BytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of TES4!\n";
             return false;
           }
           dependencies.push_back(depEntry);
           break;
      case cINTV:
           if (hasDoneINTV)
           {
             std::cout << "Error: Record TES4 seems to have more than one INTV subrecord!\n";
             return false;
           }
           //INTV's length
           in_File.read((char*) &SubLength, 2);
           BytesRead += 2;
           if (SubLength!=4)
           {
             std::cout <<"Error: sub record INTV of TES4 has invalid length ("
                       <<SubLength<<" bytes). Should be 4 bytes.\n";
             return false;
           }
           //read value
           in_File.read((char*) &unknownIntValue, 4);
           BytesRead += 4;
           hasDoneINTV = true;
           break;
    }//swi
  }//while

  //check for required field
  if (!hasDoneINTV)
  {
    std::cout << "Error: Record TES4 does not have a INTV field!\n";
    return false;
  }

  return in_File.good();
}

bool Tes4HeaderRecord::isMasterFile() const
{
  return ((headerFlags & cMasterFlag) != 0);
}

bool Tes4HeaderRecord::isLocalized() const
{
  return ((headerFlags & cLocalizedFlag) != 0);
}

} //namespace
