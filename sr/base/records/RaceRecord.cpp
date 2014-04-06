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

#include "RaceRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* RaceData's functions */

RaceRecord::RaceData::RaceData()
: heightMale(0.0f),
  heightFemale(0.0f),
  weightMale(0.0f),
  weightFemale(0.0f),
  has36(false)

{
  memset(unknown16, 0, 16);
  memset(unknown96, 0, 96);
  memset(unknown36, 0, 36);
}

bool RaceRecord::RaceData::operator==(const RaceRecord::RaceData& other) const
{
  return ((memcmp(unknown16, other.unknown16, 16)==0) and (heightMale==other.heightMale)
      and (heightFemale==other.heightFemale) and (weightMale==other.weightMale)
      and (weightFemale==other.weightFemale)
      and (memcmp(unknown96, other.unknown96, 96)==0)
      and (has36==other.has36) and ((memcmp(unknown36, other.unknown36, 36)==0) or (!has36))
    );
}

void RaceRecord::RaceData::clear()
{
  memset(unknown16, 0, 16);
  heightMale = 0.0f;
  heightFemale = 0.0f;
  weightMale = 0.0f;
  weightFemale = 0.0f;
  memset(unknown96, 0, 96);
  has36 = false;
  memset(unknown36, 0, 36);
}

/* RaceRecord's functions */

RaceRecord::RaceRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  description(LocalizedString()),
  spellFormIDs(std::vector<uint32_t>()),
  hasWNAM(false), unknownWNAM(0),
  unknownBODT(BinarySubRecord()),
  unknownBOD2(BinarySubRecord()),
  keywordArray(std::vector<uint32_t>()),
  data(RaceData()),
  subBlocks(std::vector<SubBlock>())
{
}

RaceRecord::~RaceRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool RaceRecord::equals(const RaceRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (name==other.name)
      and (description==other.description) and (spellFormIDs==other.spellFormIDs)
      and (hasWNAM==other.hasWNAM) and ((unknownWNAM==other.unknownWNAM) or (!hasWNAM))
      and (unknownBODT==other.unknownBODT) and (unknownBOD2==other.unknownBOD2)
      and (keywordArray==other.keywordArray) and (data==other.data)
      and (subBlocks==other.subBlocks));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t RaceRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +description.getWriteSize() /* DESC */
        +4 /* DATA */ +2 /* 2 bytes for length */ +getDataLength() /* fixed length of 128 or 164 bytes */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
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
  if (unknownBODT.isPresent())
  {
    writeSize = writeSize +4 /* BODT */ +2 /* 2 bytes for length */ +unknownBODT.getSize() /* length */;
  }
  if (unknownBOD2.isPresent())
  {
    writeSize = writeSize +4 /* BOD2 */ +2 /* 2 bytes for length */ +unknownBOD2.getSize() /* length */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /*KSIZ*/ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */
                          +4 /*KWDA*/ +2 /* 2 bytes for length */ +4*keywordArray.size() /* size */;
  }
  if (!subBlocks.empty())
  {
    unsigned int i;
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
  output.write((const char*) &cRACE, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if has FULL

  //write DESC
  if (!description.saveToStream(output, cDESC))
    return false;

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

  if (unknownBODT.isPresent())
  {
    //write BODT
    if (!unknownBODT.saveToStream(output, cBODT))
    {
      std::cout << "Error while writing subrecord BODT of RACE!\n";
      return false;
    }
  }//if BODT

  if (unknownBOD2.isPresent())
  {
    //write BOD2
    if (!unknownBOD2.saveToStream(output, cBOD2))
    {
      std::cout << "Error while writing subrecord BOD2 of RACE!\n";
      return false;
    }
  }//if BOD2

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
  subLength = getDataLength(); //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) (data.unknown16), 16);
  output.write((const char*) &(data.heightMale), 4);
  output.write((const char*) &(data.heightFemale), 4);
  output.write((const char*) &(data.weightMale), 4);
  output.write((const char*) &(data.weightFemale), 4);
  output.write((const char*) (data.unknown96), 96);
  if (getDataLength()>128)
  {
    output.write((const char*) (data.unknown36), 36);
  }

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
#endif

bool RaceRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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

  name.reset();
  description.reset();
  spellFormIDs.clear();
  uint32_t spell_count, i, temp_uint32;
  hasWNAM = false;
  unknownBODT.setPresence(false);
  unknownBOD2.setPresence(false);
  keywordArray.clear();
  bool hasReadDATA = false;
  subBlocks.clear();
  while ((bytesRead<readSize) and (!((unknownBODT.isPresent() or unknownBOD2.isPresent()) and hasReadDATA and description.isPresent())))
  {
    //read next subrecord name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: RACE seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
           {
             std::cout << "Error while reading subrecord FULL of RACE!\n";
             return false;
           }
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cout << "Error: RACE seems to have more than one DESC subrecord!\n";
             return false;
           }
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
           {
             std::cout << "Error while reading subrecord DESC of RACE!\n";
             return false;
           }
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
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, unknownWNAM, false))
           {
             std::cout << "Error while reading subrecord WNAM of RACE!\n";
             return false;
           }
           bytesRead += 6;
           hasWNAM = true;
           break;
      case cBODT:
           if (unknownBODT.isPresent())
           {
             std::cout << "Error: RACE seems to have more than one BODT subrecord!\n";
             return false;
           }
           //read BODT
           if (!unknownBODT.loadFromStream(in_File, cBODT, false))
             return false;
           bytesRead += (2+unknownBODT.getSize());
           //check length
           if (unknownBODT.getSize()!=12)
           {
             std::cout <<"Error: sub record BODT of RACE has invalid length("
                       <<unknownBODT.getSize()<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           break;
      case cBOD2:
           if (unknownBOD2.isPresent())
           {
             std::cout << "Error: RACE seems to have more than one BOD2 subrecord!\n";
             return false;
           }
           //read BOD2
           if (!unknownBOD2.loadFromStream(in_File, cBOD2, false))
             return false;
           bytesRead += (2+unknownBOD2.getSize());
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
           if (subLength!=getDataLength())
           {
             std::cout << "Error: sub record DATA of RACE has invalid length("
                       << subLength << " bytes). Should be " << getDataLength()
                       <<" bytes!\n";
             return false;
           }
           //read DATA
           in_File.read((char*) (data.unknown16), 16);
           in_File.read((char*) &(data.heightMale), 4);
           in_File.read((char*) &(data.heightFemale), 4);
           in_File.read((char*) &(data.weightMale), 4);
           in_File.read((char*) &(data.weightFemale), 4);
           in_File.read((char*) (data.unknown96), 96);
           if (getDataLength()>128)
           {
             in_File.read((char*) (data.unknown36), 36);
             data.has36 = true;
           }
           else
           {
             data.has36 = false;
           }
           bytesRead += getDataLength();
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of RACE!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only FULL, DESC, SPCT, WNAM, BODT, BOD2, KSIZ or DATA are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!((unknownBODT.isPresent() or unknownBOD2.isPresent()) and hasReadDATA and description.isPresent()))
  {
    std::cout << "Error: subrecord BODT/BOD2, DATA or DESC of RACE is missing!\n";
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

uint32_t RaceRecord::getRecordType() const
{
  return cRACE;
}

uint32_t RaceRecord::getDataLength() const
{
  if (headerVersion<43) return 128;
  return 164;
}

} //namespace
