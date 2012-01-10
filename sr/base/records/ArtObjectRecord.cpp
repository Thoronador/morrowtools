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

#include "ArtObjectRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ArtObjectRecord::ArtObjectRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownDNAM = 0;
}

ArtObjectRecord::~ArtObjectRecord()
{
  //empty
}

bool ArtObjectRecord::equals(const ArtObjectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownDNAM==other.unknownDNAM));
}

uint32_t ArtObjectRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size of subrecord */;
  }
  return writeSize;
}

bool ArtObjectRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cARTO, 4);
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
  subLength = 12;
  output.write((char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  //write MODL
  output.write((char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of ARTO!\n";
      return false;
    }
  }//if MODT

  //write DNAM
  output.write((char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write DNAM
  output.write((const char*) &unknownDNAM, 4);

  return output.good();
}

bool ArtObjectRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of ARTO is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ARTO!\n";
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
    std::cout <<"Error: sub record OBND of ARTO has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of ARTO!\n";
    return false;
  }

  modelPath.clear();
  unknownMODT.setPresence(false);
  bool hasReadDNAM = false;
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: ARTO seems to have more than one MODL subrecord.\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of ARTO is longer than 511 characters!\n";
             return false;
           }
           //read model path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of ARTO!\n";
             return false;
           }
           modelPath = std::string(buffer);
           //check right here
           if (modelPath.empty())
           {
             std::cout << "Error while reading subrecord MODL of ARTO: empty path!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: ARTO seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of ARTO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: ARTO seems to have more than one DNAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read DNAM
           if (!loadUint32SubRecordFromStream(in_File, cDNAM, unknownDNAM)) return false;
           bytesRead += 6;
           hasReadDNAM = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only MODL, MODT or DNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!hasReadDNAM)
  {
    std::cout << "Error: Subrecord DNAM of ARTO was not found!\n";
    return false;
  }

  return in_File.good();
}

int32_t ArtObjectRecord::getRecordType() const
{
  return cARTO;
}

} //namespace
