/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "SoulGemRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* constants for capacity / filled stuff */
const uint8_t SoulGemRecord::cCapacityEmpty = 0;
const uint8_t SoulGemRecord::cCapacityPetty = 1;
const uint8_t SoulGemRecord::cCapacityLesser = 2;
const uint8_t SoulGemRecord::cCapacityCommon = 3;
const uint8_t SoulGemRecord::cCapacityGreater = 4;
const uint8_t SoulGemRecord::cCapacityAzura = 5;

SoulGemRecord::SoulGemRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  modelPath(""),
  keywordArray(std::vector<uint32_t>()),
  //subrecord DATA
  value(0),
  weight(0.0f),
  //end of DATA
  soulInside(0), //subrecord SOUL
  capacity(0), //subrecord SLCP
  linkedToFormID(0)
{
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(false);
}

SoulGemRecord::~SoulGemRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool SoulGemRecord::equals(const SoulGemRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (keywordArray==other.keywordArray) and (value==other.value)
      and (weight==other.weight) and (soulInside==other.soulInside)
      and (capacity==other.capacity)
      and (linkedToFormID==other.linkedToFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SoulGemRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed size */
        +4 /* SOUL */ +2 /* 2 bytes for length */ +1 /* fixed size */
        +4 /* SLCP */ +2 /* 2 bytes for length */ +1 /* fixed size */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /* 4 bytes per element */;
  }
  if (linkedToFormID!=0)
  {
    writeSize = writeSize +4 /* NAM0 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool SoulGemRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cSLGM, 4);
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
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if FULL

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //EDID's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of SLGM!\n";
      return false;
    }
  }//if MODT

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write keyword count
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size;
    output.write((const char*) &subLength, 2);
    //write keywords
    uint32_t i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keywords

  //write SOUL
  output.write((const char*) &cSOUL, 4);
  //SOUL's length
  subLength = 1; //fixed
  output.write((const char*) &subLength, 2);
  //write SOUL's stuff
  output.write((const char*) &soulInside, 1);

  //write SLCP
  output.write((const char*) &cSLCP, 4);
  //SLCP's length
  subLength = 1; //fixed
  output.write((const char*) &subLength, 2);
  //write SLCP's stuff
  output.write((const char*) &capacity, 1);

  if (linkedToFormID!=0)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    //NAM0's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write "Linked to" form ID
    output.write((const char*) &linkedToFormID, 4);
  }//if NAM0

  return output.good();
}
#endif

bool SoulGemRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of SLGM is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SLGM!\n";
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
    std::cout <<"Error: sub record OBND of SLGM has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of SLGM!\n";
    return false;
  }

  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  keywordArray.clear();
  uint32_t i, k_Size, tempUint32;
  bool hasReadDATA = false;
  bool hasReadSOUL = false;
  bool hasReadSLCP = false;
  linkedToFormID = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: SLGM seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: SLGM seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of SLGM is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of SLGM!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: SLGM seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of SLGM!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownMODT.getSize() /*data size*/;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: SLGM seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size, false)) return false;
           bytesRead += 6;
           if (k_Size==0)
           {
             std::cout << "Error: SLGM's KSIZ value is zero, but that's not allowed!\n";
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
           if (subLength!=4*k_Size)
           {
             std::cout <<"Error: sub record KWDA of SLGM has invalid length ("
                       <<subLength<<" bytes). Should be "<<4*k_Size<<" bytes.\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &tempUint32, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of SLGM!\n";
               return false;
             }
             keywordArray.push_back(tempUint32);
           }//for
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: SLGM seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DATA of SLGM has invalid length ("
                       <<subLength<<" bytes). Should be eight bytes.\n";
             return false;
           }
           //read DATA
           in_File.read((char*) &value, 4);
           in_File.read((char*) &weight, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of SLGM!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cSOUL:
           if (hasReadSOUL)
           {
             std::cout << "Error: SLGM seems to have more than one SOUL subrecord.\n";
             return false;
           }
           //SOUL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record SOUL of SLGM has invalid length ("
                       <<subLength<<" bytes). Should be one byte.\n";
             return false;
           }
           //read SOUL
           in_File.read((char*) &soulInside, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SOUL of SLGM!\n";
             return false;
           }
           hasReadSOUL = true;
           break;
      case cSLCP:
           if (hasReadSLCP)
           {
             std::cout << "Error: SLGM seems to have more than one SLCP subrecord.\n";
             return false;
           }
           //SLCP's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record SLCP of SLGM has invalid length ("
                       <<subLength<<" bytes). Should be one byte.\n";
             return false;
           }
           //read SLCP
           in_File.read((char*) &capacity, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SLCP of SLGM!\n";
             return false;
           }
           hasReadSLCP = true;
           break;
      case cNAM0:
           if (linkedToFormID!=0)
           {
             std::cout << "Error: SLGM seems to have more than one NAM0 subrecord.\n";
             return false;
           }
           //read NAM0
           if (!loadUint32SubRecordFromStream(in_File, cNAM0, linkedToFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (linkedToFormID==0)
           {
             std::cout << "Error: subrecord NAM0 of SLGM is zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, KSIZ, DATA, SOUL, "
                     << "SLCP or NAM0 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!((!modelPath.empty()) and hasReadDATA and hasReadSOUL and hasReadSLCP))
  {
    std::cout << "Error: while reading SLGM record: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t SoulGemRecord::getRecordType() const
{
  return cSLGM;
}

} //namespace
