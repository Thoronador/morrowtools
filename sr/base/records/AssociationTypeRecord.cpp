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

#include "AssociationTypeRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

/* flag constants */
const uint32_t AssociationTypeRecord::cFlagFamilyAssociation = 0x00000001;

AssociationTypeRecord::AssociationTypeRecord()
: BasicRecord(), editorID(""),
  maleParentType(""), femaleParentType(""),
  maleChildType(""), femaleChildType(""),
  flags(0)
{

}

AssociationTypeRecord::~AssociationTypeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool AssociationTypeRecord::equals(const AssociationTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (maleParentType==other.maleParentType) and (femaleParentType==other.femaleParentType)
      and (maleChildType==other.maleChildType) and (femaleChildType==other.femaleChildType)
      and (flags==other.flags));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AssociationTypeRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* MPRT */ +2 /* 2 bytes for length */
        +maleParentType.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FPRT */ +2 /* 2 bytes for length */
        +femaleParentType.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (!maleChildType.empty())
  {
    writeSize = writeSize +4 /* MCHT */ +2 /* 2 bytes for length */
        +maleChildType.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!femaleChildType.empty())
  {
    writeSize = writeSize +4 /* FCHT */ +2 /* 2 bytes for length */
        +femaleChildType.length()+1 /* length of name +1 byte for NUL termination */;
  }
  return writeSize;
}

bool AssociationTypeRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cASTP, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write MPRT
  output.write((const char*) &cMPRT, 4);
  //MPRT's length
  subLength = maleParentType.length()+1;
  output.write((const char*) &subLength, 2);
  //write m. parent
  output.write(maleParentType.c_str(), subLength);

  //write FPRT
  output.write((const char*) &cFPRT, 4);
  //FPRT's length
  subLength = femaleParentType.length()+1;
  output.write((const char*) &subLength, 2);
  //write f. parent
  output.write(femaleParentType.c_str(), subLength);

  if (!maleChildType.empty())
  {
    //write MCHT
    output.write((const char*) &cMCHT, 4);
    //MCHT's length
    subLength = maleChildType.length()+1;
    output.write((const char*) &subLength, 2);
    //write m. child
    output.write(maleChildType.c_str(), subLength);
  }//if male child

  if (!femaleChildType.empty())
  {
    //write FCHT
    output.write((const char*) &cFCHT, 4);
    //FCHT's length
    subLength = femaleChildType.length()+1;
    output.write((const char*) &subLength, 2);
    //write f. child
    output.write(femaleChildType.c_str(), subLength);
  }//if female child

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write flags
  output.write((const char*) &flags, 4);

  return output.good();
}
#endif

bool AssociationTypeRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of ASTP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ASTP!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read MPRT
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cMPRT)
  {
    UnexpectedRecord(cMPRT, subRecName);
    return false;
  }
  //MPRT's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record MPRT of ASTP is longer than 511 characters!\n";
    return false;
  }
  //read MPRT's stuff
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MPRT of ASTP!\n";
    return false;
  }
  maleParentType = std::string(buffer);

  //read FPRT
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cFPRT)
  {
    UnexpectedRecord(cFPRT, subRecName);
    return false;
  }
  //FPRT's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record FPRT of ASTP is longer than 511 characters!\n";
    return false;
  }
  //read FPRT's stuff
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FPRT of ASTP!\n";
    return false;
  }
  femaleParentType = std::string(buffer);

  maleChildType.clear();
  femaleChildType.clear();
  bool hasReadDATA = false;

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMCHT:
           if (!maleChildType.empty())
           {
             std::cout << "Error: ASTP seems to have more than one MCHT subrecord!\n";
             return false;
           }
           //MCHT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MCHT of ASTP is longer than 511 characters!\n";
             return false;
           }
           //read MCHT's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MCHT of ASTP!\n";
             return false;
           }
           maleChildType = std::string(buffer);
           break;
      case cFCHT:
           if (!femaleChildType.empty())
           {
             std::cout << "Error: ASTP seems to have more than one FCHT subrecord!\n";
             return false;
           }
           //FCHT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record FCHT of ASTP is longer than 511 characters!\n";
             return false;
           }
           //read FCHT's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FCHT of ASTP!\n";
             return false;
           }
           femaleChildType = std::string(buffer);
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: ASTP seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA
           if (!loadUint32SubRecordFromStream(in_File, cDATA, flags, false)) return false;
           bytesRead += 6;
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only DATA, MCHT or FCHT are allowed here!\n";
           return false;
    }//swi
  }//while

  //check for DATA
  if (!hasReadDATA)
  {
    std::cout << "Error: record ASTP has no DATA subrecord!\n";
    return false;
  }

  return in_File.good();
}

uint32_t AssociationTypeRecord::getRecordType() const
{
  return cASTP;
}

} //namespace
