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

#include "MovementTypeRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

MovementTypeRecord::MovementTypeRecord()
: BasicRecord()
{
  editorID = "";
  nameString = "";
  unknownSPED.setPresence(false);
  unknownINAM.setPresence(false);
}

MovementTypeRecord::~MovementTypeRecord()
{
  //empty
}

bool MovementTypeRecord::equals(const MovementTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (nameString==other.nameString) and (unknownSPED==other.unknownSPED)
      and (unknownINAM==other.unknownINAM));
}

uint32_t MovementTypeRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* NNAM */ +2 /* 2 bytes for length */
        +nameString.length()+1 /* length of name +1 byte for NUL termination */;
  if (unknownSPED.isPresent())
  {
    writeSize = writeSize +4 /* SPED */ +2 /* 2 bytes for length */ +unknownSPED.getSize() /* size */;
  }
  if (unknownINAM.isPresent())
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +unknownINAM.getSize() /* size */;
  }
  return writeSize;
}

bool MovementTypeRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cMOVT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write NNAM
  output.write((const char*) &cNNAM, 4);
  //NNAM's length
  subLength = nameString.length()+1;
  output.write((char*) &subLength, 2);
  //write name string
  output.write(nameString.c_str(), subLength);

  //write SPED
  if (!unknownSPED.saveToStream(output, cSPED))
  {
    std::cout << "Error while writing subrecord SPED of MOVT!\n";
    return false;
  }

  //write INAM
  if (!unknownINAM.saveToStream(output, cINAM))
  {
    std::cout << "Error while writing subrecord INAM of MOVT!\n";
    return false;
  }

  return output.good();
}

bool MovementTypeRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of MOVT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MOVT!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read MNAM
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cMNAM)
  {
    UnexpectedRecord(cMNAM, subRecName);
    return false;
  }
  //MNAM's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record MNAM of MOVT is longer than 511 characters!\n";
    return false;
  }
  //read MNAM's stuff
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MNAM of MOVT!\n";
    return false;
  }
  nameString = std::string(buffer);

  //read SPED
  if (!unknownSPED.loadFromStream(in_File, cSPED, true))
  {
    std::cout << "Error while reading subrecord SPED of MOVT!\n";
    return false;
  }
  subLength = unknownSPED.getSize();
  bytesRead = bytesRead +4 +2 +subLength;
  //length check
  if ((subLength!=44) and (subLength!=40))
  {
    std::cout <<"Error: sub record SPED of MOVT has invalid length ("<<subLength
              <<" bytes). Should be 44 or 40 bytes.\n";
    return false;
  }

  //read INAM
  unknownINAM.setPresence(false);
  if (bytesRead<readSize)
  {
    if (!unknownINAM.loadFromStream(in_File, cINAM, true))
    {
      std::cout << "Error while reading subrecord INAM of MOVT!\n";
      return false;
    }
    bytesRead = bytesRead +4 +2 +unknownINAM.getSize();
    //INAM's length check
    if (unknownINAM.getSize()!=12)
    {
      std::cout <<"Error: sub record INAM of MOVT has invalid length ("<<subLength
                <<" bytes). Should be 12 bytes.\n";
      return false;
    }
  }

  return in_File.good();
}

int32_t MovementTypeRecord::getRecordType() const
{
  return cMOVT;
}

} //namespace
