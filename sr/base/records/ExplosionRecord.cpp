/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "ExplosionRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

ExplosionRecord::ExplosionRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  modelPath(""),
  enchantmentFormID(0), //subrecord EITM
  imageSpaceModFormID(0) //subrecord MNAM
{
  memset(unknownOBND, 0, 12);
  unknownMODT.setPresence(false);
  unknownDATA.setPresence(false);
}

ExplosionRecord::~ExplosionRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ExplosionRecord::equals(const ExplosionRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (enchantmentFormID==other.enchantmentFormID)
      and (imageSpaceModFormID==other.imageSpaceModFormID) and (unknownDATA==other.unknownDATA));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ExplosionRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* DATA */ +2 /* 2 bytes for length */ +unknownDATA.getSize() /* fixed length of 48 or 52 bytes */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }//if FULL
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }//if MODL
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }//if MODT
  if (enchantmentFormID!=0)
  {
    writeSize = writeSize +4 /* EITM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if EITM
  if (imageSpaceModFormID!=0)
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if MNAM
  return writeSize;
}

bool ExplosionRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cEXPL, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

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
  //write OBND
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
  }//if MODL

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of EXPL!\n";
      return false;
    }
  }//if MODT

  if (enchantmentFormID!=0)
  {
    //write EITM
    output.write((const char*) &cEITM, 4);
    //EITM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write EITM's data
    output.write((const char*) &enchantmentFormID, 4);
  }//if has EITM

  if (imageSpaceModFormID!=0)
  {
    //write MNAM
    output.write((const char*) &cMNAM, 4);
    //MNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write MNAM's data
    output.write((const char*) &imageSpaceModFormID, 4);
  }//if has MNAM

  if (!unknownDATA.isPresent())
  {
    std::cout << "Error while writing subrecord DATA of EXPL: subrecord DATA is missing!\n";
    return false;
  }

  //write DATA
  if (!unknownDATA.saveToStream(output, cDATA))
  {
    std::cout << "Error while writing subrecord DATA of EXPL!\n";
    return false;
  }

  return output.good();
}
#endif

bool ExplosionRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of EXPL is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of EXPL!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadOBND = false;
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  enchantmentFormID = 0;
  imageSpaceModFormID = 0;
  unknownDATA.setPresence(false);
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: EXPL seems to have more than one OBND subrecord!\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: subrecord OBND of EXPL has invalid length ("
                       <<subLength <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read OBND's stuff
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord OBND of EXPL!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: EXPL seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           if ((name.getType()==LocalizedString::lsIndex) and (name.getIndex()==0))
           {
             std::cout << "Error: subrecord FULL of EXPL has value zero!\n";
             return false;
           }
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: EXPL seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of EXPL is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of EXPL!\n";
             return false;
           }
           modelPath = std::string(buffer);
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of EXPL is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: EXPL seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of EXPL!\n";
             return false;
           }
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cEITM:
           if (enchantmentFormID!=0)
           {
             std::cout << "Error: EXPL seems to have more than one EITM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cEITM, enchantmentFormID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (enchantmentFormID==0)
           {
             std::cout << "Error: subrecord EITM of EXPL has value zero!\n";
             return false;
           }
           break;
      case cMNAM:
           if (imageSpaceModFormID!=0)
           {
             std::cout << "Error: EXPL seems to have more than one MNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, imageSpaceModFormID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (imageSpaceModFormID==0)
           {
             std::cout << "Error: subrecord MNAM of EXPL has value zero!\n";
             return false;
           }
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cout << "Error: EXPL seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA
           if (!unknownDATA.loadFromStream(in_File, cDATA, false))
           {
             std::cout << "Error while reading subrecord DATA of EXPL!\n";
             return false;
           }
           subLength = unknownDATA.getSize();
           bytesRead += (2+subLength);
           //check DATA's length
           if ((subLength!=52) and (subLength!=48) and (subLength!=44) and (subLength!=40))
           {
             std::cout <<"Error: subrecord DATA of EXPL has invalid length ("
                       <<subLength <<" bytes). Should be 52 bytes or 48 bytes or 44 bytes or 40 bytes!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only OBND, MODL, MODT, EITM, MNAM or DATA are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadOBND and unknownDATA.isPresent()))
  {
    std::cout << "Error: at least one required subrecord of EXPL is missing!\n";
    return false;
  }//if

  return in_File.good();
}

uint32_t ExplosionRecord::getRecordType() const
{
  return cEXPL;
}

} //namespace
