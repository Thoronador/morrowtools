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

#include "MiscObjectRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>
#include <cstring>

namespace SRTP
{

MiscObjectRecord::MiscObjectRecord()
: BasicRecord(), editorID(0),
  hasFULL(false), fullNameStringID(0),
  modelPath(""),
  iconPath(""),
  keywordArray(std::vector<uint32_t>()),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  value(0),
  weight(0.0f)
{
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(true);
  unknownMODS.setPresence(false);
}

MiscObjectRecord::~MiscObjectRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MiscObjectRecord::equals(const MiscObjectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (unknownVMAD==other.unknownVMAD) and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
    and (hasFULL==other.hasFULL) and ((fullNameStringID==other.fullNameStringID) or (!hasFULL))
    and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
    and (unknownMODS==other.unknownMODS) and (iconPath==other.iconPath)
    and (keywordArray==other.keywordArray)
    and (pickupSoundFormID==other.pickupSoundFormID)
    and (putdownSoundFormID==other.putdownSoundFormID)
    and (value==other.value) and (weight==other.weight));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MiscObjectRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* length of subrecord */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed size of 8 bytes */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ +2 /* 2 bytes for length */
               +unknownVMAD.getSize() /* length of subrecord */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ +2 /* 2 bytes for length */
               +unknownMODS.getSize() /* length of subrecord */;
  }
  if (!iconPath.empty())
  {
    writeSize = writeSize +4 /* ICON */ +2 /* 2 bytes for length */
               +iconPath.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */
               +4 /* KWDA */ +2 /* 2 bytes for length */+ 4*keywordArray.size();
  }
  if (pickupSoundFormID!=0)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  if (putdownSoundFormID!=0)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of four bytes */;
  }
  return writeSize;
}

bool MiscObjectRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMISC, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write VMAD
  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD)) return false;
  }

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; /* fixed length */
  output.write((const char*) &subLength, 2);
  //write OBND's data
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; /* fixed length */
    output.write((const char*) &subLength, 2);
    //write FULL's data
    output.write((const char*) &fullNameStringID, 4);
  }//if hasFULL

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  //write MODT
  if (!unknownMODT.saveToStream(output, cMODT)) return false;

  //write MODS
  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS)) return false;
  }

  if (!iconPath.empty())
  {
    //write ICON
    output.write((const char*) &cICON, 4);
    //ICON's length
    subLength = iconPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write icon path
    output.write(iconPath.c_str(), subLength);
  }//if icon path present

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; /* fixed length */
    output.write((const char*) &subLength, 2);
    //write KSIZ's data
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size;
    output.write((const char*) &subLength, 2);
    //write KWDA's data
    uint32_t i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }

  if (pickupSoundFormID!=0)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; /* fixed length */
    output.write((const char*) &subLength, 2);
    //write Pickup Sound form ID
    output.write((const char*) &pickupSoundFormID, 4);
  }//if hasYNAM

  if (putdownSoundFormID!=0)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; /* fixed length */
    output.write((const char*) &subLength, 2);
    //write Putdown Sound form ID
    output.write((const char*) &putdownSoundFormID, 4);
  }//if hasZNAM

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 8; /* fixed length */
  output.write((const char*) &subLength, 2);
  //write DATA's data
  output.write((const char*) &value, 4);
  output.write((const char*) &weight, 4);

  return output.good();
}
#endif

bool MiscObjectRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of MISC is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MISC!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  modelPath.clear();
  iconPath.clear();
  hasFULL = false;
  fullNameStringID = 0;
  pickupSoundFormID = 0;
  putdownSoundFormID = 0;
  keywordArray.clear();
  uint32_t k_Size, i, temp;
  bool hasReadOBND = false;
  bool hasReadDATA = false;
  bool hasReadKSIZ = false;
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
             std::cout << "Error: record MISC seems to have more than one VMAD subrecord.\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false)) return false;
           bytesRead = bytesRead + 2 +unknownVMAD.getSize();
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: record MISC seems to have more than one FULL subrecord.\n";
             return false;
           }
           //FULL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record FULL of MISC has invalid length ("
                       <<subLength<<" bytes)! Should be four bytes.\n";
             return false;
           }
           //read FULL's stuff
           in_File.read((char*) &fullNameStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FULL of MISC!\n";
             return false;
           }
           hasFULL = true;
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: record MISC seems to have more than one OBND subrecord.\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record OBND of MISC has invalid length ("
                       <<subLength<<" bytes)! Should be 12 bytes.\n";
             return false;
           }
           //read OBND's stuff
           in_File.read((char*) &unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord OBND of MISC!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: record MISC seems to have more than one MODL subrecord.\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of MISC is longer than 511 characters!\n";
             return false;
           }
           //read model path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of MISC!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: record MISC seems to have more than one MODT subrecord.\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false)) return false;
           bytesRead = bytesRead + 2 +unknownMODT.getSize();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: record MISC seems to have more than one MODS subrecord.\n";
             return false;
           }
           if (!unknownMODS.loadFromStream(in_File, cMODS, false)) return false;
           bytesRead = bytesRead + 2 +unknownMODS.getSize();
           break;
      case cICON:
           if (!iconPath.empty())
           {
             std::cout << "Error: record MISC seems to have more than one ICON subrecord.\n";
             return false;
           }
           //ICON's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record ICON of MISC is longer than 511 characters!\n";
             return false;
           }
           //read icon path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ICON of MISC!\n";
             return false;
           }
           iconPath = std::string(buffer);
           break;
      case cYNAM:
           if (pickupSoundFormID!=0)
           {
             std::cout << "Error: record MISC seems to have more than one YNAM subrecord.\n";
             return false;
           }
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           //check content
           if (pickupSoundFormID==0)
           {
             std::cout << "Error: subrecord YNAM of MISC is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (putdownSoundFormID!=0)
           {
             std::cout << "Error: record MISC seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           //check content
           if (putdownSoundFormID==0)
           {
             std::cout << "Error: subrecord ZNAM of MISC is zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: record MISC seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DATA of MISC has invalid length ("
                       <<subLength<<" bytes)! Should be 8 bytes.\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &value, 4);
           in_File.read((char*) &weight, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of MISC!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cKSIZ:
           if (hasReadKSIZ)
           {
             std::cout << "Error: record MISC seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record KSIZ of MISC has invalid length ("
                       <<subLength<<" bytes)! Should be four bytes.\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of MISC!\n";
             return false;
           }
           hasReadKSIZ = true;

           //keywords follow now
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
           if (subLength!=k_Size*4)
           {
             std::cout <<"Error: sub record KWDA of MISC is has invalid length ("
                       <<subLength<<" bytes)! Should be "<<k_Size*4<<" bytes.\n";
             return false;
           }
           //read keywords
           for (i=0; i<k_Size; ++i)
           {
             temp = 0;
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             keywordArray.push_back(temp);
           }//for
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KWDA of MISC!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, only DATA, FULL, OBND, VMAD, MODL, KSIZ, MODT and MODS are allowed here!\n";
           return false;
    }//swi
  }//while

  //have we read all required subrecords?
  if ((!unknownMODT.isPresent()) or (!hasReadDATA) or (!hasReadOBND)
    or (modelPath.empty()))
  {
    std::cout << "Error: at least one required subrecord of MISC was not found!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MiscObjectRecord::getRecordType() const
{
  return cMISC;
}

} //namespace
