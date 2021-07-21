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

#include "BookRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

//flag constants
const uint32_t BookRecord::cSkillBookFlag = 0x00000001;
const uint32_t BookRecord::cSpellTomeFlag = 0x00000004;
const uint32_t BookRecord::cNoteOrScrollTypeFlag = 0x0000FF00;

BookRecord::BookRecord()
: BasicRecord(), editorID(""),
  unknownVMAD(BinarySubRecord()),
  title(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  text(LocalizedString()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  keywordArray(std::vector<uint32_t>()),
  //DATA
  bookFlags(0),
  spellOrSkillID(0),
  bookValue(0),
  weight(0.0f),
  //end of DATA
  inventoryArtFormID(0),
  unknownCNAM(0)
{
  memset(unknownOBND, 0, 12);
}

BookRecord::~BookRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool BookRecord::equals(const BookRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (unknownVMAD==other.unknownVMAD)
    and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
    and (title==other.title)
    and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
    and (text==other.text) and (unknownCNAM==other.unknownCNAM)
    and (pickupSoundFormID==other.pickupSoundFormID)
    and (putdownSoundFormID==other.putdownSoundFormID)
    and (keywordArray==other.keywordArray)
    and (bookFlags==other.bookFlags) and (spellOrSkillID==other.spellOrSkillID)
    and (bookValue==other.bookValue) and (weight==other.weight)
    and (inventoryArtFormID==other.inventoryArtFormID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t BookRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size of subrecord */
        +text.getWriteSize() /* DESC */
        +4 /* DATA */ +2 /* 2 bytes for length */ +16 /* fixed size */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (title.isPresent())
  {
    writeSize += title.getWriteSize();
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +keywordArray.size()*4 /* n*fixed size */;
  }
  if (inventoryArtFormID!=0)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (pickupSoundFormID!=0)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }//if YNAM is present
  if (putdownSoundFormID!=0)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }//if ZNAM
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */
               +unknownVMAD.getSize() /* subrecord size */;
  }
  return writeSize;
}

bool BookRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cBOOK, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of BOOK!\n";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12;
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  if (title.isPresent())
  {
    //write FULL
    if (!title.saveToStream(output, cFULL))
      return false;
  }//if has FULL subrecord

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  //write MODT
  if (!unknownMODT.saveToStream(output, cMODT))
  {
    std::cout << "Error while writing subrecord MODT of BOOK!\n";
    return false;
  }

  //write DESC
  if (!text.saveToStream(output, cDESC))
    return false;

  if (pickupSoundFormID!=0)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Pickup Sound form ID
    output.write((const char*) &pickupSoundFormID, 4);
  }

  if (putdownSoundFormID!=0)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Putdown Sound form ID
    output.write((const char*) &putdownSoundFormID, 4);
  }

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write keyword size
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size; //fixed size
    output.write((const char*) &subLength, 2);
    //write actual data
    uint32_t i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keyword array not empty

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 10;
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &bookFlags, 4);
  output.write((const char*) &spellOrSkillID, 4);
  output.write((const char*) &bookValue, 4);
  output.write((const char*) &weight, 4);

  if (inventoryArtFormID!=0)
  {
    //write INAM
    output.write((const char*) &cINAM, 4);
    //INAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Inventory Art form ID
    output.write((const char*) &inventoryArtFormID, 4);
  }//if has INAM

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write CNAM
  output.write((const char*) &unknownCNAM, 4);

  return output.good();
}
#endif

bool BookRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of BOOK is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of BOOK!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND or VMAD
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName==cVMAD)
  {
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cout << "Error while reading subrecord VMAD of BOOK!\n";
      return false;
    }
    bytesRead = bytesRead +2+unknownVMAD.getSize();
    //read OBND
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }
  else
  {
    //no VMAD subrecord
    unknownVMAD.setPresence(false);
  }

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
    std::cout <<"Error: sub record OBND of BOOK has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of BOOK!\n";
    return false;
  }

  title.reset();
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  inventoryArtFormID = 0;
  keywordArray.clear();

  bool hasReadMODL = false;
  text.reset();
  bool hasReadKSIZ = false;
  bool hasReadDATA = false;
  bool hasReadCNAM = false;

  uint32_t k_Size, i, temp;
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (title.isPresent())
           {
             std::cout << "Error: BOOK seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!title.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (hasReadMODL)
           {
             std::cout << "Error: BOOK seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of BOOK is longer than 511 characters!\n";
             return false;
           }
           //read model path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of BOOK!\n";
             return false;
           }
           modelPath = std::string(buffer);

           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, true)) return false;
           bytesRead += (4+2+unknownMODT.getSize());

           //read DESC
           if (!text.loadFromStream(in_File, cDESC, true, bytesRead, localized, table, buffer))
             return false;
           hasReadMODL = true;
           break;
      case cKSIZ:
           if (hasReadKSIZ)
           {
             std::cout << "Error: BOOK seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size, false)) return false;
           bytesRead += 6;
           //keyword array follows, always
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
             std::cout <<"Error: sub record KWDA of BOOK has invalid length ("<<subLength
                       <<" bytes). Should be "<<4*k_Size<<" bytes.\n";
             return false;
           }
           //read KWDA
           keywordArray.clear();
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of BOOK!\n";
               return false;
             }
             keywordArray.push_back(temp);
           }//for
           hasReadKSIZ = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: BOOK seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=16)
           {
             std::cout <<"Error: sub record DATA of BOOK has invalid length ("<<subLength
                       <<" bytes). Should be 16 bytes.\n";
             return false;
           }
           //read DATA
           in_File.read((char*) &bookFlags, 4);
           in_File.read((char*) &spellOrSkillID, 4);
           in_File.read((char*) &bookValue, 4);
           in_File.read((char*) &weight, 4);
           bytesRead += 16;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of BOOK!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cINAM:
           if (inventoryArtFormID!=0)
           {
             std::cout << "Error: BOOK seems to have more than one INAM subrecord.\n";
             return false;
           }
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, inventoryArtFormID, false)) return false;
           bytesRead += 6;
           if (inventoryArtFormID==0)
           {
             std::cout << "Error: subrecord INAM of BOOK is zero!\n";
             return false;
           }
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: BOOK seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, false)) return false;
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cYNAM:
           if (pickupSoundFormID!=0)
           {
             std::cout << "Error: BOOK seems to have more than one YNAM subrecord.\n";
             return false;
           }
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false)) return false;
           bytesRead += 6;
           if (pickupSoundFormID==0)
           {
             std::cout << "Error: subrecord YNAM of BOOK is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID!=0)
           {
             std::cout << "Error: BOOK seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false)) return false;
           bytesRead += 6;
           if (putdownSoundFormID==0)
           {
             std::cout << "Error: subrecord ZNAM of BOOK is zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only MODL, KSIZ, DATA, INAM, CNAM, YNAM or ZNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //check presence of all required subrecords
  if (!(hasReadMODL and hasReadDATA and hasReadCNAM))
  {
    std::cout << "Error: At least one required subrecord of BOOK was not found!\n";
    return false;
  }
  return in_File.good();
}

uint32_t BookRecord::getRecordType() const
{
  return cBOOK;
}

bool BookRecord::isSkillBook() const
{
  return ((bookFlags bitand cSkillBookFlag)==cSkillBookFlag);
}

bool BookRecord::isSpellTome() const
{
  return ((bookFlags bitand cSpellTomeFlag)==cSpellTomeFlag);
}

} //namespace
