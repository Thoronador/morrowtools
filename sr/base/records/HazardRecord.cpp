/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "HazardRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

HazardRecord::HazardRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  hasMNAM = false;
  unknownMNAM = 0;
  memset(unknownDATA, 0, 40);
}

HazardRecord::~HazardRecord()
{
  //empty
}

bool HazardRecord::equals(const HazardRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (hasMNAM==other.hasMNAM) and ((unknownMNAM==other.unknownMNAM) or (!hasMNAM))
      and (memcmp(unknownDATA, other.unknownDATA, 40)==0));
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t HazardRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +40 /* fixed size */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
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
  if (hasMNAM)
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool HazardRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cHAZD, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; // fixed
    output.write((char*) &subLength, 2);
    //write FULL
    output.write((const char*) &nameStringID, 4);
  }

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //EDID's length
    subLength = modelPath.length()+1;
    output.write((char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of HAZD!\n";
      return false;
    }
  }//if MODT

  if (hasMNAM)
  {
    //write MNAM
    output.write((const char*) &cMNAM, 4);
    //MNAM's length
    subLength = 4; // fixed
    output.write((char*) &subLength, 2);
    //write MNAM
    output.write((const char*) &unknownMNAM, 4);
  }

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 40; //fixed
  output.write((char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) unknownDATA, 40);

  return output.good();
}
#endif

bool HazardRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
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
    std::cout <<"Error: sub record EDID of HAZD is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of HAZD!\n";
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
    std::cout <<"Error: sub record OBND of HAZD has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of HAZD!\n";
    return false;
  }

  hasFULL = false; nameStringID = 0;
  modelPath.clear();
  unknownMODT.setPresence(false);
  hasMNAM = false; unknownMNAM = 0;
  bool hasReadDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: HAZD seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: HAZD seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of HAZD is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of HAZD!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: HAZD seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of HAZD!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownMODT.getSize() /*data size*/;
           break;
      case cMNAM:
           if (hasMNAM)
           {
             std::cout << "Error: HAZD seems to have more than one MNAM subrecord.\n";
             return false;
           }
           //read MNAM
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, unknownMNAM)) return false;
           bytesRead += 6;
           hasMNAM = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: HAZD seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=40)
           {
             std::cout <<"Error: sub record DATA of HAZD has invalid length ("<<subLength
                       <<" bytes). Should be 40 bytes.\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownDATA, 40);
           bytesRead += 40;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of HAZD!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, MNAM or DATA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!( (!modelPath.empty()) and hasReadDATA))
  {
    std::cout << "Error: while reading HAZD record: at least one required subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t HazardRecord::getRecordType() const
{
  return cHAZD;
}

} //namespace
