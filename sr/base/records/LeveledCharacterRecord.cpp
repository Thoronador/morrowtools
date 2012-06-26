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

#include "LeveledCharacterRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

LeveledCharacterRecord::LeveledCharacterRecord()
: LeveledListBaseRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  chanceNone = 0; //subrecord LVLD
  flags = 0;
  //entryCount = 0; //subrecord LLCT
  entries.clear(); //subrecords LVLO
  modelPath = "";
  unknownMODT.setPresence(false);
}

LeveledCharacterRecord::~LeveledCharacterRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LeveledCharacterRecord::equals(const LeveledCharacterRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (chanceNone==other.chanceNone) and (flags==other.flags)
      and (entries==other.entries) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LeveledCharacterRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */
        +4 /* LVLD */ +2 /* 2 bytes for length */ +1 /* fixed size of one byte */
        +4 /* LVLF */ +2 /* 2 bytes for length */ +1 /* fixed size of one byte */
        +4 /* LLCT */ +2 /* 2 bytes for length */ +1 /* fixed size of one byte */
        +entries.size()*(4 /* LVLO */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */);
  unsigned int i;
  for (i=0; i<entries.size(); ++i)
  {
    if (!entries[i].coed.isInit())
    {
      writeSize = writeSize +4 /* COED */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */;
    }//if
  }//for
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */
        +unknownMODT.getSize() /* length of data */;
  }
  return writeSize;
}

bool LeveledCharacterRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cLVLN, 4);
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
    output.write((const char*) &entries[i].entry.level, 4);
    output.write((const char*) &entries[i].entry.formID, 4);
    output.write((const char*) &entries[i].entry.count, 4);

    if (!entries[i].coed.isInit())
    {
      //write COED
      output.write((const char*) &cCOED, 4);
      //COED's length
      subLength = 12; //fixed
      output.write((const char*) &subLength, 2);
      //write COED's stuff
      output.write((const char*) &entries[i].coed.factionFormID, 4);
      output.write((const char*) &entries[i].coed.requiredRank, 4);
      output.write((const char*) &entries[i].coed.unknownFloat, 4);
    }//if COED
  }//for

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of LVLN!\n";
      return false;
    }
  }//if MODT

  return output.good();
}
#endif

bool LeveledCharacterRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of LVLN is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of LVLN!\n";
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
    std::cout <<"Error: sub record OBND of LVLN has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  memset(unknownOBND, 0, 12);
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of LVLN!\n";
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
    std::cout <<"Error: sub record LVLD of LVLN has invalid length ("<<subLength
              <<" bytes). Should be one byte!\n";
    return false;
  }
  //read LVLD's stuff
  in_File.read((char*) &chanceNone, 1);
  bytesRead += 1;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord LVLD of LVLN!\n";
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
    std::cout <<"Error: sub record LVLF of LVLN has invalid length ("<<subLength
              <<" bytes). Should be one byte!\n";
    return false;
  }
  //read LVLF's stuff
  in_File.read((char*) &flags, 1);
  bytesRead += 1;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord LVLF of LVLN!\n";
    return false;
  }

  bool hasReadLLCT = false;
  uint8_t entryCount = 0;
  entries.clear();
  EntryWithCOED temp;
  modelPath.clear();
  unknownMODT.setPresence(false);
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
             std::cout << "Error: record LVLN seems to have more than one LLCT subrecord!\n";
             return false;
           }
           //LLCT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record LLCT of LVLN has invalid length ("
                       <<subLength<<" bytes). Should be one byte!\n";
             return false;
           }
           //read LLCT's stuff
           in_File.read((char*) &entryCount, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord LLCT of LVLN!\n";
             return false;
           }
           hasReadLLCT = true;
           break;
      case cLVLO:
           if (entries.size()>=entryCount)
           {
             std::cout << "Error: record LVLN seems to have more LVLO subrecords than the LLCT value indicates!\n";
             return false;
           }
           //LVLO's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record LVLO of LVLN has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read LVLO's stuff
           in_File.read((char*) &temp.entry.level, 4);
           in_File.read((char*) &temp.entry.formID, 4);
           in_File.read((char*) &temp.entry.count, 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord LVLO of LVLN!\n";
             return false;
           }
           temp.coed.init(); //sets default values for COED part
           entries.push_back(temp);
           break;
      case cCOED:
           if (entries.empty())
           {
             std::cout << "Error: COED subrecord not allowed without previous LVLO subrecord!\n";
             return false;
           }
           if ((entries.back().coed.factionFormID!=0)
               or (entries.back().coed.requiredRank!=0xFFFFFFFF)
               or (entries.back().coed.unknownFloat!=1.0f))
           {
             std::cout << "Error: record LVLN seems to have more than one COED subrecord per LVLO!\n";
             return false;
           }
           //COED's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record COED of LVLN has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read LVLO's stuff
           in_File.read((char*) &entries.back().coed.factionFormID, 4);
           in_File.read((char*) &entries.back().coed.requiredRank, 4);
           in_File.read((char*) &entries.back().coed.unknownFloat, 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord COED of LVLN!\n";
             return false;
           }
           //status check
           if (entries.back().coed.isInit())
           {
             std::cout << "Error: subrecord COED of LVLN contains invalid value!\n";
             return false;
           }
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: record LVLN seems to have more than one MODL subrecord!\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           //content check
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of LVLN is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: record LVLN seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of LVLN!\n";
             return false;
           }
           bytesRead += (2+unknownMODT.getSize());
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only LLCT, MODL or MODT are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence check
  if (!hasReadLLCT)
  {
    std::cout << "Error: at least one required subrecord of LVLN is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t LeveledCharacterRecord::getRecordType() const
{
  return cLVLN;
}

    std::string editorID;
    uint8_t unknownOBND[12];
    uint8_t chanceNone; //subrecord LVLD
    uint8_t flags; //subrecord LVLF
    //uint8_t entryCount; //subrecord LLCT
    //std::vector<LeveledListEntry> entries; //subrecords LVLO
    std::string modelPath;
    BinarySubRecord unknownMODT;

} //namespace
