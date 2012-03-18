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

#include "EquipmentSlotRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

const uint32_t EquipmentSlotRecord::FlagUseAllParents = 0x00000001;

EquipmentSlotRecord::EquipmentSlotRecord()
: BasicRecord()
{
  editorID = "";
  parentSlots.clear();
  flags = 0;
}

EquipmentSlotRecord::~EquipmentSlotRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool EquipmentSlotRecord::equals(const EquipmentSlotRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (parentSlots==other.parentSlots) and (flags==other.flags));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t EquipmentSlotRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  if (!parentSlots.empty())
  {
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ +parentSlots.size()*4 /* fixed length of four bytes */;
  }
  return writeSize;
}

bool EquipmentSlotRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cEQUP, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (!parentSlots.empty())
  {
    //write PNAM
    output.write((const char*) &cPNAM, 4);
    //PNAM's length
    subLength = 4*parentSlots.size();
    output.write((const char*) &subLength, 2);
    unsigned int i;
    for (i=0; i<parentSlots.size(); ++i)
    {
      //write PNAM's data
      output.write((const char*) &(parentSlots[i]), 4);
    }//for
  }//if has PNAM

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write DATA's data
  output.write((const char*) &flags, 4);

  return output.good();
}
#endif

bool EquipmentSlotRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of EQUP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of EQUP!\n";
    return false;
  }
  editorID = std::string(buffer);

  parentSlots.clear();
  unsigned int i, count;
  uint32_t temp;
  bool hasReadDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: EQUP seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA
           if (!loadUint32SubRecordFromStream(in_File, cDATA, flags, false)) return false;
           bytesRead += 6;
           hasReadDATA = true;
           break;
      case cPNAM:
           if (!parentSlots.empty())
           {
             std::cout << "Error: EQUP seems to have more than one PNAM subrecord!\n";
             return false;
           }
           //read PNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength%4)!=0)
           {
             std::cout <<"Error: sub record PNAM of EQUP has invalid length ("<<subLength
                       <<" bytes). Should be an integral multiple of four bytes.\n";
             return false;
           }
           //read PNAM's stuff
           count = subLength/4;
           for (i=0; i<count; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord PNAM of EQUP!\n";
               return false;
             }
             parentSlots.push_back(temp);
           }//for
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only DATA or PNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  //check for data
  if (!hasReadDATA)
  {
    std::cout << "Error while reading EQUP record: subrecord DATA is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t EquipmentSlotRecord::getRecordType() const
{
  return cEQUP;
}

bool EquipmentSlotRecord::usesAllParents() const
{
  return ((flags& FlagUseAllParents)!=0);
}

} //namespace
