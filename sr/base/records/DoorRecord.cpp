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

#include "DoorRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

DoorRecord::DoorRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0; //subrecord FULL
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasSNAM = false;
  unknownSNAM = 0;
  hasANAM = false;
  unknownANAM = 0;
  unknownFNAM = 0;
}

DoorRecord::~DoorRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool DoorRecord::equals(const DoorRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0) and (hasFULL==other.hasFULL)
      and ((nameStringID==other.nameStringID) or (!hasFULL)) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (unknownMODS==other.unknownMODS)
      and (hasSNAM==other.hasSNAM) and ((unknownSNAM==other.unknownSNAM) or (!hasSNAM))
      and (hasANAM==other.hasANAM) and ((unknownANAM==other.unknownANAM) or (!hasANAM))
      and (unknownFNAM==other.unknownFNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t DoorRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +1 /* fixed length of one byte */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /*VMAD*/ +2 /* 2 bytes for length */
               +unknownVMAD.getSize() /* size */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if has SNAM
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /*MODT*/ +2 /* 2 bytes for length */
               +unknownMODT.getSize() /* size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize +4 /*MODS*/ +2 /* 2 bytes for length */
               +unknownMODS.getSize() /* size */;
  }
  if (hasSNAM)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if has SNAM
  if (hasANAM)
  {
    writeSize = writeSize +4 /* ANAM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if has ANAM
  return writeSize;
}

bool DoorRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cDOOR, 4);
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
    //write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of DOOR!\n";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

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

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if model path

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of DOOR!\n";
      return false;
    }
  }//if MODT

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing subrecord MODS of DOOR!\n";
      return false;
    }
  }//if MODS

  if (hasSNAM)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write SNAM's stuff
    output.write((const char*) &unknownSNAM, 4);
  }//if has SNAM

  if (hasANAM)
  {
    //write ANAM
    output.write((const char*) &cANAM, 4);
    //ANAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write ANAM's stuff
    output.write((const char*) &unknownANAM, 4);
  }//if has ANAM

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 1; //fixed
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &unknownFNAM, 1);

  return output.good();
}
#endif

bool DoorRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of DOOR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of DOOR!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read (VMAD or) OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName==cVMAD)
  {
    //has VMAD
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cout << "Error while reading subrecord VMAD of DOOR!\n";
      return false;
    }
    bytesRead = bytesRead +2 +unknownVMAD.getSize();

    //read next subrecord (OBND)
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }
  else
  {
    //no VMAD here!
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
    std::cout <<"Error: subrecord OBND of DOOR has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of DOOR!\n";
    return false;
  }

  hasFULL = false;
  nameStringID = 0;
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasSNAM = false;
  hasANAM = false;
  bool hasReadFNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: DOOR seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: DOOR seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of DOOR is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of DOOR!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: DOOR seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of DOOR!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: DOOR seems to have more than one MODS subrecord!\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cout << "Error while reading subrecord MODS of DOOR!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODS.getSize();
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: DOOR seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM, false)) return false;
           bytesRead += 6;
           hasSNAM = true;
           break;
      case cANAM:
           if (hasANAM)
           {
             std::cout << "Error: DOOR seems to have more than one ANAM subrecord!\n";
             return false;
           }
           //read ANAM
           if (!loadUint32SubRecordFromStream(in_File, cANAM, unknownANAM, false)) return false;
           bytesRead += 6;
           hasANAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: DOOR seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: subrecord FNAM of DOOR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read FNAM's stuff
           in_File.read((char*) &unknownFNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of DOOR!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, SNAM, ANAM"
                     << " or FNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //check for required records
  if (!hasReadFNAM)
  {
    std::cout << "Error while reading record DOOR: subrecord FNAM is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t DoorRecord::getRecordType() const
{
  return cDOOR;
}

} //namespace
