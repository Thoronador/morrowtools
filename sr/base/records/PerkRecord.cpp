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

#include "PerkRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

bool PerkRecord::SubBlock::operator==(const PerkRecord::SubBlock& other) const
{
  return ((subType==other.subType) and (subData==other.subData));
}

PerkRecord::PerkRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  hasFULL = false;
  nameStringID = 0;
  descriptionStringID = 0;
  subBlocks.clear();
}

PerkRecord::~PerkRecord()
{
  //empty
}

bool PerkRecord::equals(const PerkRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or !hasFULL)
      and (descriptionStringID==other.descriptionStringID) and (subBlocks==other.subBlocks));
}

uint32_t PerkRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /*VMAD*/ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* size */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /*FULL*/ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }
  unsigned int i;
  if (!subBlocks.empty())
  {
    for (i=0; i<subBlocks.size(); ++i)
    {
      writeSize = writeSize +4 /*header*/ +2 /* 2 bytes for length */
                 +subBlocks[i].subData.getSize() /* length */;
    }//for
  }//if subBlocks
  return writeSize;
}

bool PerkRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cPERK, 4);
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

  //write DESC
  output.write((const char*) &cDESC, 4);
  //DESC's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DESC's stuff
  output.write((const char*) &descriptionStringID, 4);

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

bool PerkRecord::loadFromStream(std::ifstream& in_File)
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

  //read (optional) VMAD or FULL
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName==cVMAD)
  {
    //read VMAD
    if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
    {
      std::cout << "Error while reading subrecord VMAD of PERK!\n";
      return false;
    }
    bytesRead = bytesRead + 2 + unknownVMAD.getSize();
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }//if VMAD
  else
  {
    unknownVMAD.setPresence(false);
  }

  if (subRecName==cFULL)
  {
    //FULL's length
    in_File.read((char*) &subLength, 2);
    bytesRead += 2;
    if (subLength!=4)
    {
      std::cout <<"Error: subrecord FULL of PERK has invalid length ("<<subLength
                <<" bytes). Should be four bytes!\n";
      return false;
    }
    //read FULL's stuff
    in_File.read((char*) &nameStringID, 4);
    bytesRead += 4;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord FULL of PERK!\n";
      return false;
    }
    hasFULL = true;

    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
  }//if FULL
  else
  {
    hasFULL = false;
    nameStringID = 0;
  }//else (no FULL)

  //read DESC -> already read above
  if (subRecName!=cDESC)
  {
    UnexpectedRecord(cDESC, subRecName);
    return false;
  }
  //DESC's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=4)
  {
    std::cout <<"Error: subrecord DESC of PERK has invalid length ("<<subLength
              <<" bytes). Should be four bytes!\n";
    return false;
  }
  //read DESC's stuff
  in_File.read((char*) &descriptionStringID, 4);
  bytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DESC of PERK!\n";
    return false;
  }

  //now read the rest
  subBlocks.clear();
  SubBlock temp;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    temp.subType = subRecName;
    if (!temp.subData.loadFromStream(in_File, subRecName, false))
    {
      std::cout << "Error while reading subrecord "<<IntTo4Char(subRecName)
                << " of PERK!\n";
      return false;
    }
    bytesRead = bytesRead +2 +temp.subData.getSize();
    subBlocks.push_back(temp);
  }//while

  return in_File.good();
}

int32_t PerkRecord::getRecordType() const
{
  return cPERK;
}

} //namespace
