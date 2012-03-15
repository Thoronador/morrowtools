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

#include "BookRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//flag constants
const uint32_t BookRecord::cSkillBookFlag = 0x00000001;
const uint32_t BookRecord::cSpellTomeFlag = 0x00000004;
const uint32_t BookRecord::cNoteOrScrollTypeFlag = 0x0000FF00;

BookRecord::BookRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  titleStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(true);
  textStringID = 0;
  hasYNAM = false;
  unknownYNAM = 0;
  keywordArray.clear();
  //DATA
  bookFlags = 0;
  spellOrSkillID = 0;
  bookValue = 0;
  weight = 0.0f;
  //end of DATA
  hasINAM = false;
  unknownINAM = 0;
  unknownCNAM = 0;
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
    and (hasFULL==other.hasFULL) and ((titleStringID==other.titleStringID) or (!hasFULL))
    and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
    and (textStringID==other.textStringID) and (unknownCNAM==other.unknownCNAM)
    and (hasYNAM==other.hasYNAM) and ((unknownYNAM==other.unknownYNAM) or (!hasYNAM))
    and (keywordArray==other.keywordArray)
    and (bookFlags==other.bookFlags) and (spellOrSkillID==other.spellOrSkillID)
    and (bookValue==other.bookValue) and (weight==other.weight)
    and (hasINAM==other.hasINAM) and ((unknownINAM==other.unknownINAM) or (!hasINAM)));
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
        +4 /* DATA */ +2 /* 2 bytes for length */ +16 /* fixed size */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +keywordArray.size()*4 /* n*fixed size */;
  }
  if (hasINAM)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasYNAM)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }//if YNAM is present
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
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
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
  output.write((char*) &cOBND, 4);
  //OBND's length
  subLength = 12;
  output.write((char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write FULL
    output.write((const char*) &titleStringID, 4);
  }//if has FULL subrecord

  //write MODL
  output.write((char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  //write MODT
  if (!unknownMODT.saveToStream(output, cMODT))
  {
    std::cout << "Error while writing subrecord MODT of BOOK!\n";
    return false;
  }

  if (hasYNAM)
  {
    //write YNAM
    output.write((char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write YNAM
    output.write((const char*) &unknownYNAM, 4);
  }

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write keyword size
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size; //fixed size
    output.write((char*) &subLength, 2);
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

  if (hasINAM)
  {
    //write INAM
    output.write((const char*) &cINAM, 4);
    //INAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write INAM
    output.write((const char*) &unknownINAM, 4);
  }//if has INAM

  //write CNAM
  output.write((char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write CNAM
  output.write((const char*) &unknownCNAM, 4);

  return output.good();
}
#endif

bool BookRecord::loadFromStream(std::ifstream& in_File)
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

  hasFULL = false;
  hasYNAM = false;
  hasINAM = false;
  keywordArray.clear();

  bool hasReadMODL = false;
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
           if (hasFULL)
           {
             std::cout << "Error: BOOK seems to have more than one FULL subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, titleStringID)) return false;
           bytesRead += 6;
           hasFULL = true;
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
           if (!loadUint32SubRecordFromStream(in_File, cDESC, textStringID)) return false;
           bytesRead += 10;
           hasReadMODL = true;
           break;
      case cKSIZ:
           if (hasReadKSIZ)
           {
             std::cout << "Error: BOOK seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size)) return false;
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
           if (hasINAM)
           {
             std::cout << "Error: BOOK seems to have more than one INAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, unknownINAM)) return false;
           bytesRead += 6;
           hasINAM = true;
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: BOOK seems to have more than one CNAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM)) return false;
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cYNAM:
           if (hasYNAM)
           {
             std::cout << "Error: BOOK seems to have more than one YNAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, unknownYNAM)) return false;
           bytesRead += 6;
           hasYNAM = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only MODL, KSIZ, DATA, INAM, CNAM or YNAM are allowed here!\n";
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

int32_t BookRecord::getRecordType() const
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
