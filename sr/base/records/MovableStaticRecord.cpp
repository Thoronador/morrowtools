/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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

#include "MovableStaticRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

MovableStaticRecord::MovableStaticRecord()
: BasicRecord(), editorID(""),
  modelPath(""),
  unknownDATA(0),
  loopingSoundFormID(0)
{
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  destruction.clear();
}

MovableStaticRecord::~MovableStaticRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MovableStaticRecord::equals(const MovableStaticRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownMODS==other.unknownMODS)
      and (unknownDATA==other.unknownDATA) and (loopingSoundFormID==other.loopingSoundFormID)
      and (destruction==other.destruction));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MovableStaticRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +1 /* fixed length */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }//if MODT
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize +4 /* MODS */ +2 /* 2 bytes for length */ +unknownMODS.getSize() /* size */;
  }//if MODS
  if (loopingSoundFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize+destruction.getWriteSize();
}

bool MovableStaticRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cMSTT, 4);
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
  subLength = 12;
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of MSTT!\n";
      return false;
    }
  }//if MODT

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing subrecord MODS of MSTT!\n";
      return false;
    }
  }//if MODS

  if (!destruction.saveToStream(output))
  {
    std::cout << "Error while writing destruction data of MSTT!\n";
    return false;
  }//if

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &unknownDATA, 1);

  if (loopingSoundFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write Looping Sound form ID
    output.write((const char*) &loopingSoundFormID, 4);
  }//if SNAM

  return output.good();
}
#endif

bool MovableStaticRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
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
    std::cout <<"Error: sub record EDID of MSTT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MSTT!\n";
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
    std::cout <<"Error: sub record OBND of MSTT has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of MSTT!\n";
    return false;
  }

  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  bool hasReadDATA = false;
  loopingSoundFormID = 0;
  destruction.clear();
  DestructionStage tempStage;
  while (bytesRead<readSize)
  {
    //read next record
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: MSTT seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           //check content
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of MSTT is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: MSTT seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false)) return false;
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: MSTT seems to have more than one MODS subrecord.\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false)) return false;
           bytesRead += (2+unknownMODS.getSize());
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: MSTT seems to have more than one DATA subrecord.\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout << "Error: sub record DATA of MSTT has invalid length ("
                       << subLength << " bytes). Should be one byte.\n";
             return false;
           }
           //read DATA
           in_File.read((char*) &unknownDATA, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of MSTT!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cSNAM:
           if (loopingSoundFormID!=0)
           {
             std::cout << "Error: MSTT seems to have more than one SNAM subrecord.\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, loopingSoundFormID, false)) return false;
           bytesRead += 6;
           if (loopingSoundFormID==0)
           {
             std::cout << "Error: subrecord SNAM of MSTT has value zero!\n";
             return false;
           }
           break;
      case cDEST:
           if (destruction.isPresent)
           {
             std::cout << "Error: MSTT seems to have more than one DEST subrecord.\n";
             return false;
           }
           //read DEST and possible DSTD, DMDL, DMDT, DSTF subrecords
           if (!destruction.loadFromStream(in_File, cMSTT, buffer, bytesRead))
             return false;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only MODL, MODT, MODS, DATA, SNAM or DEST are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadDATA and !modelPath.empty()))
  {
    std::cout << "Error: at least one required subrecord of MSTT is missing!\n"
              << "DATA: "<<hasReadDATA<<", MODL: "<<!modelPath.empty()<<"\n";
    return false;
  }

  return in_File.good();
}

uint32_t MovableStaticRecord::getRecordType() const
{
  return cMSTT;
}

} //namespace
