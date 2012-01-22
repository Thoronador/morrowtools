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

#include "AmmunitionRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

AmmunitionRecord::AmmunitionRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownYNAM = 0;
  unknownZNAM = 0;
  unknownDESC = 0;
  keywordArray.clear();
  unknownDATA[0] = unknownDATA[1] = unknownDATA[2] = unknownDATA[3] = 0;
}

AmmunitionRecord::~AmmunitionRecord()
{
  //empty
}

bool AmmunitionRecord::equals(const AmmunitionRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownYNAM==other.unknownYNAM) and (unknownZNAM==other.unknownZNAM)
      and (unknownDESC==other.unknownDESC)
      and (keywordArray==other.keywordArray) and (unknownDATA[0]==other.unknownDATA[0])
      and (unknownDATA[1]==other.unknownDATA[1]) and (unknownDATA[2]==other.unknownDATA[2])
      and (unknownDATA[3]==other.unknownDATA[3]));
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AmmunitionRecord::getWriteSize() const
{
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
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
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
  output.write((char*) &cAMMO, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed size
  output.write((char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write FULL's stuff
    output.write((const char*) &nameStringID, 4);
  }//if hasFULL

  if (!modelPath.empty())
  {
    //write MODL
    output.write((char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((char*) &subLength, 2);
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
  output.write((char*) &cYNAM, 4);
  //YNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write YNAM's stuff
  output.write((const char*) &unknownYNAM, 4);

  //write ZNAM
  output.write((char*) &cZNAM, 4);
  //ZNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write ZNAM's stuff
  output.write((const char*) &unknownZNAM, 4);

  //write DESC
  output.write((char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write DESC's stuff
  output.write((const char*) &unknownDESC, 4);

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write KSIZ's stuff
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size;
    output.write((char*) &subLength, 2);
    unsigned int i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((char*) &(keywordArray[i]), 4);
    }//for
  }//if keywords

  //write DATA
  output.write((char*) &cDATA, 4);
  //DATA's length
  subLength = 16; //fixed size
  output.write((char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &unknownDATA[0], 4);
  output.write((const char*) &unknownDATA[1], 4);
  output.write((const char*) &unknownDATA[2], 4);
  output.write((const char*) &unknownDATA[3], 4);

  return output.good();
}
#endif

bool AmmunitionRecord::loadFromStream(std::ifstream& in_File)
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

  hasFULL = false;
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
           if (hasFULL)
           {
             std::cout << "Error: Record AMMO seems to have more than one FULL subrecord!\n";
             return false;
           }
           //FULL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record FULL of AMMO has invalid length("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read FULL's stuff
           in_File.read((char*) &nameStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FULL of AMMO!\n";
             return false;
           }
           hasFULL = true;
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
           //YNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record YNAM of AMMO has invalid length("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read YNAM's stuff
           in_File.read((char*) &unknownYNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord YNAM of AMMO!\n";
             return false;
           }
           hasReadYNAM = true;
           break;
      case cZNAM:
           if (hasReadZNAM)
           {
             std::cout << "Error: Record AMMO seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //ZNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record ZNAM of AMMO has invalid length("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read ZNAM's stuff
           in_File.read((char*) &unknownZNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ZNAM of AMMO!\n";
             return false;
           }
           hasReadZNAM = true;
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: Record AMMO seems to have more than one DESC subrecord!\n";
             return false;
           }
           //DESC's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record DESC of AMMO has invalid length("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DESC's stuff
           in_File.read((char*) &unknownDESC, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DESC of AMMO!\n";
             return false;
           }
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
             std::cout <<"Error: sub record KSIZ of AMMO has invalid length("<<subLength
                       <<" bytes). Should be four bytes!\n";
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
             std::cout <<"Error: sub record KWDA of AMMO has invalid length("<<subLength
                       <<" bytes). Should be "<<4*k_Size<<" bytes!\n";
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
             std::cout <<"Error: sub record DATA of AMMO has invalid length("<<subLength
                       <<" bytes). Should be 16 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &unknownDATA[0], 4);
           in_File.read((char*) &unknownDATA[1], 4);
           in_File.read((char*) &unknownDATA[2], 4);
           in_File.read((char*) &unknownDATA[3], 4);
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

int32_t AmmunitionRecord::getRecordType() const
{
  return cAMMO;
}

} //namespace
