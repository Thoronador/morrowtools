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

#include "PerkRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

PerkRecord::PerkRecord()
: BasicRecord(), editorID(""),
  unknownVMAD(BinarySubRecord()),
  name(LocalizedString()),
  description(LocalizedString()),
  subBlocks(std::vector<SubBlock>())
{

}

PerkRecord::~PerkRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool PerkRecord::equals(const PerkRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (name==other.name)
      and (description==other.description) and (subBlocks==other.subBlocks));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t PerkRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +description.getWriteSize() /* DESC */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /*VMAD*/ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (!subBlocks.empty())
  {
    unsigned int i;
    for (i=0; i<subBlocks.size(); ++i)
    {
      writeSize = writeSize +4 /*header*/ +2 /* 2 bytes for length */
                 +subBlocks[i].subData.getSize() /* length */;
    }//for
  }//if subBlocks
  return writeSize;
}

bool PerkRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cPERK, 4);
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
      std::cout << "Error while writing subrecord VMAD of PERK!\n";
      return false;
    }
  }//if VMAD

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if has FULL

  //write DESC
  if (!description.saveToStream(output, cDESC))
    return false;

  if (!subBlocks.empty())
  {
    unsigned int i;
    for (i=0; i<subBlocks.size(); ++i)
    {
      if (!subBlocks[i].subData.saveToStream(output, subBlocks[i].subType))
      {
        std::cout << "Error while writing (binary) subrecord "
                  << IntTo4Char(subBlocks[i].subType)<<" of PERK!\n";
        return false;
      }//if
    }//for
  }//if subBlocks

  return output.good();
}
#endif

bool PerkRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of PERK is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of PERK!\n";
    return false;
  }
  editorID = std::string(buffer);

  //now read the rest
  unknownVMAD.setPresence(false);
  name.reset();
  description.reset();
  subBlocks.clear();
  SubBlock temp;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: record PERK seems to have more than one VMAD subrecord.\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false)) return false;
           bytesRead += (2 +unknownVMAD.getSize());
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: record PERK seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cDESC:
           if (description.isPresent())
           {
             std::cout << "Error: record PERK seems to have more than one DESC subrecord.\n";
             return false;
           }
           //read DESC
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      default:
           temp.subType = subRecName;
           if (!temp.subData.loadFromStream(in_File, subRecName, false))
           {
             std::cout << "Error while reading subrecord "<<IntTo4Char(subRecName)
                       << " of PERK!\n";
             return false;
           }
           bytesRead += (2 +temp.subData.getSize());
           subBlocks.push_back(temp);
           break;
    }//swi
  }//while

  if (!description.isPresent())
  {
    std::cout << "Error: subrecord DESC of PERK is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t PerkRecord::getRecordType() const
{
  return cPERK;
}

} //namespace
