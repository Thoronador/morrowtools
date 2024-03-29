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

#include "DoorRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

DoorRecord::DoorRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  modelPath(""),
  hasSNAM(false), unknownSNAM(0),
  hasANAM(false), unknownANAM(0),
  unknownFNAM(0)
{
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
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
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name) and (modelPath==other.modelPath)
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
  writeSize = 4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +1 /* fixed length of one byte */;
  if (!editorID.empty())
  {
    writeSize = writeSize +4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /*VMAD*/ + 2 /* 2 bytes for length */
               + unknownVMAD.size() /* size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }//if has FULL
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /*MODT*/ + 2 /* 2 bytes for length */
               + unknownMODT.size() /* size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /*MODS*/ + 2 /* 2 bytes for length */
               + unknownMODS.size() /* size */;
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

bool DoorRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cDOOR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  uint16_t subLength;
  if (!editorID.empty())
  {
    //write EDID
    output.write((const char*) &cEDID, 4);
    //EDID's length
    subLength = editorID.length()+1;
    output.write((const char*) &subLength, 2);
    //write editor ID
    output.write(editorID.c_str(), subLength);
  }

  if (unknownVMAD.isPresent())
  {
    //write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing subrecord VMAD of DOOR!\n";
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

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
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
      std::cerr << "Error while writing subrecord MODT of DOOR!\n";
      return false;
    }
  }//if MODT

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing subrecord MODS of DOOR!\n";
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

bool DoorRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //for EDID's stuff
  char buffer[512];

  editorID.clear();
  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasSNAM = false; unknownSNAM = 0;
  hasANAM = false; unknownANAM = 0;
  bool hasReadFNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cEDID:
           if (!editorID.empty())
           {
             std::cerr << "Error: DOOR seems to have more than one EDID subrecord!\n";
             return false;
           }
           //read EDID
           if (!loadString512FromStream(in_File, editorID, buffer, cEDID, false, bytesRead))
           {
             std::cerr << "Error while reading subrecord EDID of DOOR!\n";
             return false;
           }
           if (editorID.empty())
           {
             std::cerr << "Error: subrecord EDID of DOOR is empty!\n";
             return false;
           }
           break;
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: DOOR seems to have more than one VMAD subrecord!\n";
             return false;
           }
           // read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cerr << "Error while reading subrecord VMAD of DOOR!\n";
             return false;
           }
           bytesRead += (2 + unknownVMAD.size());
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: DOOR seems to have more than one OBND subrecord!\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr << "Error: subrecord OBND of DOOR has invalid length ("
                       << subLength << " bytes). Should be 12 bytes!\n";
             return false;
           }
           //read OBND's stuff
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord OBND of DOOR!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: DOOR seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: DOOR seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record MODL of DOOR is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord MODL of DOOR!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: DOOR seems to have more than one MODT subrecord!\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of DOOR!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODT.size();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: DOOR seems to have more than one MODS subrecord!\n";
             return false;
           }
           // read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cerr << "Error while reading subrecord MODS of DOOR!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownMODS.size();
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cerr << "Error: DOOR seems to have more than one SNAM subrecord!\n";
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
             std::cerr << "Error: DOOR seems to have more than one ANAM subrecord!\n";
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
             std::cerr << "Error: DOOR seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cerr <<"Error: subrecord FNAM of DOOR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read FNAM's stuff
           in_File.read((char*) &unknownFNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord FNAM of DOOR!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only EDID, VMAD, OBND, FULL, MODL, MODT,"
                     << " SNAM, ANAM or FNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //check for required records
  if (!(hasReadOBND and hasReadFNAM))
  {
    std::cerr << "Error while reading record DOOR: subrecord OBND or FNAM is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t DoorRecord::getRecordType() const
{
  return cDOOR;
}

} //namespace
