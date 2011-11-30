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

#include "MagicEffectRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>

namespace SRTP
{

MagicEffectRecord::MagicEffectRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  hasFULL = false;
  fullNameStringID = 0;
  hasMDOB = false;
  unknownMDOB = 0;
  keywordSize = 0;
  keywordArray.clear();
  unknownDATA.setPresence(false);
  unknownSNDD.setPresence(false);
  descriptionStringID = 0;
  unknownCTDAs.clear();
}

MagicEffectRecord::~MagicEffectRecord()
{
  //empty
}

bool MagicEffectRecord::equals(const MagicEffectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD) and (hasFULL==other.hasFULL)
      and ((fullNameStringID==other.fullNameStringID) or (!hasFULL))
      and (hasMDOB==other.hasMDOB) and ((unknownMDOB==other.unknownMDOB) or (!hasMDOB))
      and (keywordSize==other.keywordSize) and (keywordArray==other.keywordArray)
      and (unknownDATA==other.unknownDATA) and (unknownSNDD==other.unknownSNDD)
      and (descriptionStringID==other.descriptionStringID) and (unknownCTDAs==other.unknownCTDAs));
}

bool MagicEffectRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMGEF, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +152 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +unknownCTDAs.size()*
        (4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed size */);
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* size */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasMDOB)
  {
    writeSize = writeSize +4 /* MDOB */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +keywordArray.size()*4 /* n*fixed size */;
  }
  if (unknownSNDD.isPresent())
  {
    writeSize = writeSize +4 /* SNDD */ +2 /* 2 bytes for length */ +unknownSNDD.getSize() /* size */;
  }
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    //write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of MGEF!\n";
      return false;
    }
  }//if VMAD

  if (hasFULL)
  {
    //write FULL
    output.write((char*) &cFULL, 4);
    //FULL's length
    subLength = 4;
    output.write((char*) &subLength, 2);
    //write FULL
    output.write((const char*) &fullNameStringID, 4);
  }//if has FULL subrecord

  if (hasMDOB)
  {
    //write MDOB
    output.write((char*) &cMDOB, 4);
    //MDOB's length
    subLength = 4;
    output.write((char*) &subLength, 2);
    //write MDOB
    output.write((const char*) &unknownMDOB, 4);
  }//if MDOB present

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write keyword size
    output.write((const char*) &keywordSize, 4);

    //write KWDA
    output.write((char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*keywordArray.size(); //fixed size
    output.write((char*) &subLength, 2);
    //write actual data
    for (writeSize=0; writeSize<keywordArray.size(); ++writeSize)
    {
      output.write((const char*) &(keywordArray[writeSize]), 4);
    }//for
  }//if keywords

  //write DATA
  if ((unknownDATA.getSize()!=152) or (!unknownDATA.isPresent()))
  {
    std::cout << "Error: will not write DATA subrecord of MGEF, because it has invalid size!\n";
    return false;
  }
  if (!unknownDATA.saveToStream(output, cDATA))
  {
    std::cout << "Error while writing subrecord DATA of MGEF!\n";
    return false;
  }

  if (unknownSNDD.isPresent())
  {
    //write SNDD
    if (!unknownSNDD.saveToStream(output, cSNDD))
    {
      std::cout << "Error while writing subrecord SNDD of MGEF!\n";
      return false;
    }
  }

  //write DNAM
  output.write((char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4;
  output.write((char*) &subLength, 2);
  //write DNAM
  output.write((const char*) &descriptionStringID, 4);

  //write CTDAs
  for (writeSize=0; writeSize<unknownCTDAs.size(); ++writeSize)
  {
    //write CTDA
    output.write((char*) &cCTDA, 4);
    //CTDA's length
    subLength = 32;
    output.write((char*) &subLength, 2);
    //write CTDA
    output.write((const char*) (unknownCTDAs[writeSize].content), 32);
  }//for

  return output.good();
}

bool MagicEffectRecord::loadFromStream(std::ifstream& in_File)
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

  unknownVMAD.setPresence(false);
  hasFULL = false;
  hasMDOB = false;
  unknownDATA.setPresence(false);
  keywordSize = 0;
  keywordArray.clear();
  uint32_t i, helper;
  unknownSNDD.setPresence(false);
  bool hasReadDNAM = false;
  unknownCTDAs.clear();
  CTDAData tempCTDA;

  while (bytesRead<readSize)
  {
    //read next subrecord header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: MGEF seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD's stuff
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of MGEF!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownVMAD.getSize();
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: MGEF seems to have more than one FULL subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, fullNameStringID)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMDOB:
           if (hasMDOB)
           {
             std::cout << "Error: MGEF seems to have more than one MDOB subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read MDOB
           if (!loadUint32SubRecordFromStream(in_File, cMDOB, unknownMDOB)) return false;
           bytesRead += 6;
           hasMDOB = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: MGEF seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record KSIZ of MGEF has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           in_File.read((char*) &keywordSize, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of MGEF!\n";
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
           if (subLength!=4*keywordSize)
           {
             std::cout <<"Error: sub record KWDA of MGEF has invalid length("
                       <<subLength<<" bytes). Should be "<<4*keywordSize<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<keywordSize; ++i)
           {
             in_File.read((char*) &helper, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of MGEF!\n";
               return false;
             }
             keywordArray.push_back(helper);
           }//for
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cout << "Error: MGEF seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA's stuff
           if (!unknownDATA.loadFromStream(in_File, cDATA, false))
           {
             std::cout << "Error while reading subrecord DATA of MGEF!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownDATA.getSize();
           //check length
           if (unknownDATA.getSize()!=152)
           {
             std::cout <<"Error: sub record DATA of MGEF has invalid length("
                       <<unknownDATA.getSize()<<" bytes). Should be 152 bytes!\n";
             return false;
           }
           break;
      case cSNDD:
           if (unknownSNDD.isPresent())
           {
             std::cout << "Error: MGEF seems to have more than one SNDD subrecord!\n";
             return false;
           }
           //read SNDD's stuff
           if (!unknownSNDD.loadFromStream(in_File, cSNDD, false))
           {
             std::cout << "Error while reading subrecord SNDD of MGEF!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownSNDD.getSize();
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: MGEF seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record DNAM of MGEF has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) &descriptionStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of MGEF!\n";
             return false;
           }
           hasReadDNAM = true;
           break;
      case cCTDA:
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cout <<"Error: sub record CTDA of MGEF has invalid length("
                       <<subLength<<" bytes). Should be 32 bytes!\n";
             return false;
           }
           //read CTDA's stuff
           memset(tempCTDA.content, 0, 32);
           in_File.read((char*) tempCTDA.content, 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CTDA of MGEF!\n";
             return false;
           }
           unknownCTDAs.push_back(tempCTDA);
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only VMAD, FULL, MDOB, KSIZ, SNDD, DATA or DNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  //check for required DNAM
  if (!hasReadDNAM)
  {
    std::cout << "Error: DNAM subrecord of MGEF is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t MagicEffectRecord::getRecordType() const
{
  return cMGEF;
}

} //namespace
