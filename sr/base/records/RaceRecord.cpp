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

#include "RaceRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* RaceData's functions */

bool RaceRecord::RaceData::operator==(const RaceRecord::RaceData& other) const
{
  return ((memcmp(unknown16, other.unknown16, 16)==0) and (heightMale==other.heightMale)
      and (heightFemale==other.heightFemale) and (weightMale==other.weightMale)
      and (weightFemale==other.weightFemale)
      and (memcmp(unknown96, other.unknown96, 96)==0));
}

void RaceRecord::RaceData::clear()
{
  memset(unknown16, 0, 16);
  heightMale = 0.0f;
  heightFemale = 0.0f;
  weightMale = 0.0f;
  weightFemale = 0.0f;
  memset(unknown96, 0, 96);
}

/* SubBlock's functions */

bool RaceRecord::SubBlock::operator==(const RaceRecord::SubBlock& other) const
{
  return ((subType==other.subType) and (subData==other.subData));
}

/* RaceRecord's functions */

RaceRecord::RaceRecord()
: BasicRecord()
{
  editorID = "";
  hasFULL = false;
  nameStringID = 0;
  descriptionStringID = 0;
  spellFormIDs.clear();
  hasWNAM = false;
  unknownWNAM = 0;
  memset(unknownBODT, 0, 12);
  keywordArray.clear();
  data.clear();
  subBlocks.clear();
}

RaceRecord::~RaceRecord()
{
  //empty
}

bool RaceRecord::equals(const RaceRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (descriptionStringID==other.descriptionStringID) and (spellFormIDs==other.spellFormIDs)
      and (hasWNAM==other.hasWNAM) and ((unknownWNAM==other.unknownWNAM) or (!hasWNAM))
      and (memcmp(unknownBODT, other.unknownBODT, 12)==0)
      and (keywordArray==other.keywordArray) and (data==other.data)
      and (subBlocks==other.subBlocks));
}

uint32_t RaceRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +4 /* BODT */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* DATA */ +2 /* 2 bytes for length */ +128 /* fixed length of 128 bytes */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if has FULL
  if (!spellFormIDs.empty())
  {
    writeSize = writeSize +4 /*SPCT*/ +2 /* 2 bytes for length */ +4 /* size */
                          +spellFormIDs.size()*(4 /*SPLO*/ +2 /* 2 bytes for length */ +4 /* size */);
  }
  if (hasWNAM)
  {
    writeSize = writeSize +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if has WNAM
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /*KSIZ*/ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
                          +4 /*KWDA*/ +2 /* 2 bytes for length */ +4*keywordArray.size() /* size */;
  }
  unsigned int i;
  if (!subBlocks.empty())
  {
    for (i=0; i<subBlocks.size(); ++i)
    {
      writeSize = writeSize +4 /*header*/ +2 /* 2 bytes for length */
                 +subBlocks[i].subData.getSize() /* length */;
    }//for
  }//if subBlocks
  return writeSize;
}

bool RaceRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cRACE, 4);
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
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FULL's stuff
    output.write((const char*) &nameStringID, 4);
  }//if has FULL

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DESC's stuff
  output.write((const char*) &descriptionStringID, 4);

  unsigned int i;
  if (!spellFormIDs.empty())
  {
    //write SPCT
    output.write((const char*) &cSPCT, 4);
    //SCPT's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write SCPT's stuff
    const uint32_t spell_count = spellFormIDs.size();
    output.write((const char*) &spell_count, 4);

    for (i=0; i<spell_count; ++i)
    {
      //write SPLO
      output.write((const char*) &cSPLO, 4);
      //SCPT's length
      subLength = 4; //fixed
      output.write((const char*) &subLength, 2);
      //write SCLO's stuff
      output.write((const char*) &(spellFormIDs[i]), 4);
    }//for
  }//if spell form IDs

  if (hasWNAM)
  {
    //write WNAM
    output.write((const char*) &cWNAM, 4);
    //WNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write WNAM's stuff
    output.write((const char*) &unknownWNAM, 4);
  }//if has WNAM

  //write BODT
  output.write((const char*) &cBODT, 4);
  //BODT's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write BODT's stuff
  output.write((const char*) unknownBODT, 12);

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write KSIZ's stuff
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size; //fixed
    output.write((const char*) &subLength, 2);
    //write KWDA's stuff
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 128; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) (data.unknown16), 16);
  output.write((const char*) &(data.heightMale), 4);
  output.write((const char*) &(data.heightFemale), 4);
  output.write((const char*) &(data.weightMale), 4);
  output.write((const char*) &(data.weightFemale), 4);
  output.write((const char*) (data.unknown96), 96);

  if (!subBlocks.empty())
  {
    for (i=0; i<subBlocks.size(); ++i)
    {
      if (!subBlocks[i].subData.saveToStream(output, subBlocks[i].subType))
      {
        std::cout << "Error while writing (binary) subrecord "
                  << IntTo4Char(subBlocks[i].subType)<<" of RACE!\n";
        return false;
      }//if
    }//for
  }//if subBlocks

  return output.good();
}

bool RaceRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of RACE is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of RACE!\n";
    return false;
  }
  editorID = std::string(buffer);

  hasFULL = false;
  bool hasReadDESC = false;
  spellFormIDs.clear();
  uint32_t spell_count, i, temp_uint32;
  hasWNAM = false;
  bool hasReadBODT = false;
  keywordArray.clear();
  bool hasReadDATA = false;
  subBlocks.clear();
  while ((bytesRead<readSize) and (!(hasReadBODT and hasReadDATA and hasReadDESC)))
  {
    //read next subrecord name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: RACE seems to have more than one FULL subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID))
           {
             std::cout << "Error while reading subrecord FULL of RACE!\n";
             return false;
           }
           bytesRead += 6;
           hasFULL = true;
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: RACE seems to have more than one DESC subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cDESC, descriptionStringID))
           {
             std::cout << "Error while reading subrecord DESC of RACE!\n";
             return false;
           }
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cSPCT:
           if (!spellFormIDs.empty())
           {
             std::cout << "Error: RACE seems to have more than one SPCT subrecord!\n";
             return false;
           }
           //SPCT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record SPCT of RACE has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read SPCT
           spell_count = 0;
           in_File.read((char*) &spell_count, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SPCT of RACE!\n";
             return false;
           }
           if (spell_count==0)
           {
             std::cout << "Error: subrecord SPCT of RACE is zero!\n";
             return false;
           }

           for (i=0; i<spell_count; ++i)
           {
             //read SPLO
             in_File.read((char*) &subRecName, 4);
             bytesRead += 4;
             if (subRecName!=cSPLO)
             {
               UnexpectedRecord(cSPLO, subRecName);
               return false;
             }
             //SPLO's length
             in_File.read((char*) &subLength, 2);
             bytesRead += 2;
             if (subLength!=4)
             {
               std::cout <<"Error: sub record SPLO of RACE has invalid length("
                         <<subLength<<" bytes). Should be four bytes!\n";
               return false;
             }
             //read SPLO's content
             in_File.read((char*) &temp_uint32, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord SPLO of RACE!\n";
               return false;
             }
             spellFormIDs.push_back(temp_uint32);
           }//for
           break;
      case cWNAM:
           if (hasWNAM)
           {
             std::cout << "Error: RACE seems to have more than one WNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, unknownWNAM))
           {
             std::cout << "Error while reading subrecord WNAM of RACE!\n";
             return false;
           }
           bytesRead += 6;
           hasWNAM = true;
           break;
      case cBODT:
           if (hasReadBODT)
           {
             std::cout << "Error: RACE seems to have more than one BODT subrecord!\n";
             return false;
           }
           //BODT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record BODT of RACE has invalid length("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read BODT
           spell_count = 0;
           in_File.read((char*) unknownBODT, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BODT of RACE!\n";
             return false;
           }
           hasReadBODT = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: RACE seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record KSIZ of RACE has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ
           spell_count = 0;
           in_File.read((char*) &spell_count, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of RACE!\n";
             return false;
           }
           if (spell_count==0)
           {
             std::cout << "Error: subrecord KSIZ of RACE is zero!\n";
             return false;
           }

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cKWDA)
           {
             UnexpectedRecord(cKWDA, subRecName);
             return false;
           }
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*spell_count)
           {
             std::cout <<"Error: sub record KWDA of RACE has invalid length("
                       <<subLength<<" bytes). Should be "<<4*spell_count<<" bytes!\n";
             return false;
           }

           for (i=0; i<spell_count; ++i)
           {
             //read KWDA's content
             in_File.read((char*) &temp_uint32, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of RACE!\n";
               return false;
             }
             keywordArray.push_back(temp_uint32);
           }//for
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: RACE seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=128)
           {
             std::cout <<"Error: sub record DATA of RACE has invalid length("
                       <<subLength<<" bytes). Should be 128 bytes!\n";
             return false;
           }
           //read DATA
           in_File.read((char*) (data.unknown16), 16);
           in_File.read((char*) &(data.heightMale), 4);
           in_File.read((char*) &(data.heightFemale), 4);
           in_File.read((char*) &(data.weightMale), 4);
           in_File.read((char*) &(data.weightFemale), 4);
           in_File.read((char*) (data.unknown96), 96);
           bytesRead += 128;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of RACE!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only SPCT, WNAM, BODT, KSIZ or DATA are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadBODT and hasReadDATA and hasReadDESC))
  {
    std::cout << "Error: subrecord BODT, DATA or DESC of RACE is missing!\n";
    return false;
  }

  //read all other sub blocks
  //now read the rest
  subBlocks.clear();
  SubBlock tempBlock;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    tempBlock.subType = subRecName;
    if (!tempBlock.subData.loadFromStream(in_File, subRecName, false))
    {
      std::cout << "Error while reading subrecord "<<IntTo4Char(subRecName)
                << " of RACE!\n";
      std::cout << "Read size: "<<readSize<<",    bytes read: "<<bytesRead<<"\n";
      return false;
    }
    bytesRead = bytesRead +2 +tempBlock.subData.getSize();
    subBlocks.push_back(tempBlock);
  }//while

  return in_File.good();
}

int32_t RaceRecord::getRecordType() const
{
  return cRACE;
}

} //namespace
