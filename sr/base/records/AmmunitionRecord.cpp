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

#include "AmmunitionRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

AmmunitionRecord::AmmunitionRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  modelPath(""),
  pickupSoundFormID(0),
  putdownSoundFormID(0),
  descriptionStringID(0),
  keywordArray(std::vector<uint32_t>()),
  //DATA
  projectileFormID(0),
  DATAflags(0),
  baseDamage(0.0f),
  value(0)
  //end of DATA
{
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(false);
}

AmmunitionRecord::~AmmunitionRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool AmmunitionRecord::equals(const AmmunitionRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (pickupSoundFormID==other.pickupSoundFormID) and (putdownSoundFormID==other.putdownSoundFormID)
      and (descriptionStringID==other.descriptionStringID)
      and (keywordArray==other.keywordArray) and (projectileFormID==other.projectileFormID)
      and (DATAflags==other.DATAflags) and (baseDamage==other.baseDamage)
      and (value==other.value));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AmmunitionRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of string +1 byte for NUL-termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +16 /* fixed size */;
  if (!modelPath.empty())
  {
    writeSize = writeSize + 4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of string +1 byte for NUL-termination */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ +2 /* 2 bytes for length */
               +unknownMODT.getSize();
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +keywordArray.size()*(4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */);
  }
  return writeSize;
}

bool AmmunitionRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cAMMO, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

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
  subLength = 12; //fixed size
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if hasFULL

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if modelPath

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of AMMO!\n";
      return false;
    }
  }//if MODT

  //write YNAM
  output.write((const char*) &cYNAM, 4);
  //YNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write Pickup Sound form ID
  output.write((const char*) &pickupSoundFormID, 4);

  //write ZNAM
  output.write((const char*) &cZNAM, 4);
  //ZNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write Putdown Sound form ID
  output.write((const char*) &putdownSoundFormID, 4);

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write description string ID
  output.write((const char*) &descriptionStringID, 4);

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write KSIZ's stuff
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size;
    output.write((const char*) &subLength, 2);
    unsigned int i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keywords

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 16; //fixed size
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &projectileFormID, 4);
  output.write((const char*) &DATAflags, 4);
  output.write((const char*) &baseDamage, 4);
  output.write((const char*) &value, 4);

  return output.good();
}
#endif

bool AmmunitionRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of AMMO is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of AMMO!\n";
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
    std::cout <<"Error: sub record OBND of AMMO has invalid length("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of AMMO!\n";
    return false;
  }

  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadYNAM = false;
  bool hasReadZNAM = false;
  bool hasReadDESC = false;
  keywordArray.clear();
  uint32_t k_Size, i, temp;
  bool hasReadDATA = false;

  while (bytesRead<readSize)
  {
    //read next subrecord name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: Record AMMO seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: Record AMMO seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of AMMO is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of AMMO!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: Record AMMO seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of AMMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cYNAM:
           if (hasReadYNAM)
           {
             std::cout << "Error: Record AMMO seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //read YNAM
           if (!loadUint32SubRecordFromStream(in_File, cYNAM, pickupSoundFormID, false))
             return false;
           bytesRead += 6;
           hasReadYNAM = true;
           break;
      case cZNAM:
           if (hasReadZNAM)
           {
             std::cout << "Error: Record AMMO seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(in_File, cZNAM, putdownSoundFormID, false))
             return false;
           bytesRead += 6;
           hasReadZNAM = true;
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: Record AMMO seems to have more than one DESC subrecord!\n";
             return false;
           }
           //read DESC
           if (!loadUint32SubRecordFromStream(in_File, cDESC, descriptionStringID, false))
             return false;
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: Record AMMO seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record KSIZ of AMMO has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of AMMO!\n";
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
             std::cout <<"Error: sub record KWDA of AMMO has invalid length("
                       <<subLength<<" bytes). Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of AMMO!\n";
               return false;
             }
             keywordArray.push_back(temp);
           }//for
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: Record AMMO seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=16)
           {
             std::cout <<"Error: sub record DATA of AMMO has invalid length("
                       <<subLength<<" bytes). Should be 16 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &projectileFormID, 4);
           in_File.read((char*) &DATAflags, 4);
           in_File.read((char*) &baseDamage, 4);
           in_File.read((char*) &value, 4);
           bytesRead += 16;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of AMMO!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, YNAM, ZNAM, DESC, KSIZ or DATA are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  //check
  if ((!hasReadDATA) or (!hasReadYNAM) or (!hasReadZNAM) or (!hasReadDESC))
  {
    std::cout << "Error: at least one required subrecord of AMMO is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t AmmunitionRecord::getRecordType() const
{
  return cAMMO;
}

} //namespace
