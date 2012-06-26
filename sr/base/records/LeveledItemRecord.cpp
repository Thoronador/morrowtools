/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012  Thoronador

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

#include "LeveledItemRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//flag constants
const uint8_t LeveledItemRecord::cFlagCalcFromAllLevels = 0x01;
const uint8_t LeveledItemRecord::cFlagCalcForEach       = 0x02;
const uint8_t LeveledItemRecord::cFlagUseAll            = 0x04;
const uint8_t LeveledItemRecord::cFlagSpecialLoot       = 0x08;

bool LeveledItemRecord::LeveledListEntry::operator==(const LeveledItemRecord::LeveledListEntry& other) const
{
  return ((level==other.level) and (formID==other.formID) and (count==other.count));
}

LeveledItemRecord::LeveledItemRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  chanceNone = 0; //subrecord LVLD
  flags = 0;
  globalFormID = 0;
  //entryCount = 0; //subrecord LLCT
  entries.clear(); //subrecords LVLO
}

LeveledItemRecord::~LeveledItemRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LeveledItemRecord::equals(const LeveledItemRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (chanceNone==other.chanceNone) and (flags==other.flags)
      and (globalFormID==other.globalFormID) and (entries==other.entries));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LeveledItemRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */
        +4 /* LVLD */ +2 /* 2 bytes for length */ +1 /* fixed size of one byte */
        +4 /* LVLF */ +2 /* 2 bytes for length */ +1 /* fixed size of one byte */
        +4 /* LLCT */ +2 /* 2 bytes for length */ +1 /* fixed size of one byte */
        +entries.size()*(4 /* LVLO */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */);
  if (globalFormID!=0)
  {
    writeSize = writeSize +4 /* LVLG */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */   ;
  }
  return writeSize;
}

bool LeveledItemRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cLVLI, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND stuff
  output.write((const char*) unknownOBND, 12);

  //write LVLD
  output.write((const char*) &cLVLD, 4);
  //LVLD's length
  subLength = 1; //fixed
  output.write((const char*) &subLength, 2);
  //write LVLD's stuff
  output.write((const char*) &chanceNone, 1);

  //write LVLF
  output.write((const char*) &cLVLF, 4);
  //LVLF's length
  subLength = 1; //fixed
  output.write((const char*) &subLength, 2);
  //write LVLF's stuff
  output.write((const char*) &flags, 1);

  if (globalFormID!=0)
  {
    //write LVLG
    output.write((const char*) &cLVLG, 4);
    //LVLG's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write LVLG's stuff
    output.write((const char*) &globalFormID, 4);
  }//if LVLG

  //write LLCT
  output.write((const char*) &cLLCT, 4);
  //LLCT's length
  subLength = 1; //fixed
  output.write((const char*) &subLength, 2);
  //write LLCT's stuff
  const uint8_t len = entries.size();
  output.write((const char*) &len, 1);

  unsigned int i;
  for (i=0; i<len; ++i)
  {
    //write LVLO
    output.write((const char*) &cLVLO, 4);
    //LVLO's length
    subLength = 12; //fixed
    output.write((const char*) &subLength, 2);
    //write LVLO's stuff
    output.write((const char*) &entries[i].level, 4);
    output.write((const char*) &entries[i].formID, 4);
    output.write((const char*) &entries[i].count, 4);
  }//for

  return output.good();
}
#endif

bool LeveledItemRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
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
    std::cout <<"Error: sub record EDID of LVLI is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of LVLI!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: sub record OBND of LVLI has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  memset(unknownOBND, 0, 12);
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of LVLI!\n";
    return false;
  }

  //read LVLD
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cLVLD)
  {
    UnexpectedRecord(cLVLD, subRecName);
    return false;
  }
  //LVLD's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=1)
  {
    std::cout <<"Error: sub record LVLD of LVLI has invalid length ("<<subLength
              <<" bytes). Should be one byte!\n";
    return false;
  }
  //read LVLD's stuff
  in_File.read((char*) &chanceNone, 1);
  bytesRead += 1;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord LVLD of LVLI!\n";
    return false;
  }

  //read LVLF
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cLVLF)
  {
    UnexpectedRecord(cLVLF, subRecName);
    return false;
  }
  //LVLF's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=1)
  {
    std::cout <<"Error: sub record LVLF of LVLI has invalid length ("<<subLength
              <<" bytes). Should be one byte!\n";
    return false;
  }
  //read LVLF's stuff
  in_File.read((char*) &flags, 1);
  bytesRead += 1;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord LVLF of LVLI!\n";
    return false;
  }

  globalFormID = 0;
  bool hasReadLLCT = false;
  uint8_t entryCount = 0;
  entries.clear();
  LeveledListEntry temp;
  unsigned int i;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cLLCT:
           if (hasReadLLCT)
           {
             std::cout << "Error: record LVLI seems to have more than one LLCT subrecord!\n";
             return false;
           }
           //LLCT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record LLCT of LVLI has invalid length ("
                       <<subLength<<" bytes). Should be one byte!\n";
             return false;
           }
           //read LLCT's stuff
           in_File.read((char*) &entryCount, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord LLCT of LVLI!\n";
             return false;
           }
           hasReadLLCT = true;

           //read LVLO records here
           for (i=0; i<entryCount; ++i)
           {
             //read LVLO
             in_File.read((char*) &subRecName, 4);
             bytesRead += 4;
             if (subRecName!=cLVLO)
             {
               UnexpectedRecord(cLVLO, subRecName);
               return false;
             }
             //LVLO's length
             in_File.read((char*) &subLength, 2);
             bytesRead += 2;
             if (subLength!=12)
             {
               std::cout <<"Error: sub record LVLO of LVLI has invalid length ("<<subLength
                         <<" bytes). Should be 12 bytes!\n";
               return false;
             }
             //read LVLO's stuff
             in_File.read((char*) &temp.level, 4);
             in_File.read((char*) &temp.formID, 4);
             in_File.read((char*) &temp.count, 4);
             bytesRead += 12;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord LVLO of LVLI!\n";
               return false;
             }
             entries.push_back(temp);
           }//for
           break;
      case cLVLG:
           if (globalFormID!=0)
           {
             std::cout << "Error: record LVLI seems to have more than one LVLG subrecord!\n";
             return false;
           }
           //LVLG's length
           if (!loadUint32SubRecordFromStream(in_File, cLVLG, globalFormID, false))
             return false;
           bytesRead += 6;
           //check content
           if (globalFormID==0)
           {
             std::cout << "Error: subrecord LVLG of LVLI has invalid value!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only LVLG or LLCT are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

uint32_t LeveledItemRecord::getRecordType() const
{
  return cLVLI;
}

bool LeveledItemRecord::calculateFromAllLevels() const
{
  return ((flags & cFlagCalcFromAllLevels) !=0);
}

bool LeveledItemRecord::calculateForEachItem() const
{
  return ((flags & cFlagCalcForEach) !=0);
}

bool LeveledItemRecord::useAll() const
{
  return ((flags & cFlagUseAll) !=0);
}

bool LeveledItemRecord::specialLoot() const
{
  return ((flags & cFlagSpecialLoot) !=0);
}

} //namespace
