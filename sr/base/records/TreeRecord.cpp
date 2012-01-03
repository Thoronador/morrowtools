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

#include "TreeRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

TreeRecord::TreeRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  modelPath = "";
  unknownMODT.setPresence(false);
  hasPFIG = false;
  unknownPFIG = 0;
  hasSNAM = false;
  unknownSNAM = 0;
  unknownPFPC = 0;
  hasFULL = false;
  nameStringID = 0; //subrecord FULL
  memset(unknownCNAM, 0, 48);
}

TreeRecord::~TreeRecord()
{
  //empty
}

bool TreeRecord::equals(const TreeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (hasPFIG==other.hasPFIG) and ((unknownPFIG==other.unknownPFIG) or (!hasPFIG))
      and (hasSNAM==other.hasSNAM) and ((unknownSNAM==other.unknownSNAM) or (!hasSNAM))
      and (unknownPFPC==other.unknownPFPC) and (hasFULL==other.hasFULL)
      and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (memcmp(unknownCNAM, other.unknownCNAM, 48)==0));
}

uint32_t TreeRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* PFPC */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +48 /* fixed size */;
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }
  if (hasPFIG)
  {
    writeSize = writeSize +4 /* PFIG */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasSNAM)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool TreeRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cTREE, 4);
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
      std::cout << "Error while writing subrecord MODT of TREE!\n";
      return false;
    }
  }//if MODT

  if (hasPFIG)
  {
    //write PFIG
    output.write((const char*) &cPFIG, 4);
    //PFIG's length
    subLength = 4; // fixed
    output.write((char*) &subLength, 2);
    //write PFIG
    output.write((const char*) &unknownPFIG, 4);
  }//if PFIG

  if (hasSNAM)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; // fixed
    output.write((char*) &subLength, 2);
    //write SNAM
    output.write((const char*) &unknownSNAM, 4);
  }

  //write PFPC
  output.write((const char*) &cPFPC, 4);
  //PFPC's length
  subLength = 4; // fixed
  output.write((char*) &subLength, 2);
  //write PFPC
  output.write((const char*) &unknownPFPC, 4);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; // fixed
    output.write((char*) &subLength, 2);
    //write FULL
    output.write((const char*) &nameStringID, 4);
  }//if FULL

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 48; //fixed
  output.write((char*) &subLength, 2);
  //write CNAM's stuff
  output.write((const char*) unknownCNAM, 48);

  return output.good();
}

bool TreeRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of TREE is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of TREE!\n";
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
    std::cout <<"Error: sub record OBND of TREE has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of TREE!\n";
    return false;
  }

  modelPath.clear();
  unknownMODT.setPresence(false);
  hasPFIG = false;
  hasSNAM = false;
  bool hasReadPFPC = false;
  hasFULL = false;
  bool hasReadCNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: TREE seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of TREE is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of TREE!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: TREE seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of TREE!\n";
             return false;
           }
           bytesRead = bytesRead +2 /*length value*/ +unknownMODT.getSize() /*data size*/;
           break;
      case cPFIG:
           if (hasPFIG)
           {
             std::cout << "Error: TREE seems to have more than one PFIG subrecord.\n";
             return false;
           }
           //read PFIG
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cPFIG, unknownPFIG)) return false;
           bytesRead += 6;
           hasPFIG = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: TREE seems to have more than one SNAM subrecord.\n";
             return false;
           }
           //read SNAM
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM)) return false;
           bytesRead += 6;
           hasSNAM = true;
           break;
      case cPFPC:
           if (hasReadPFPC)
           {
             std::cout << "Error: TREE seems to have more than one PFCP subrecord.\n";
             return false;
           }
           //read PFCP
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cPFPC, unknownPFPC)) return false;
           bytesRead += 6;
           hasReadPFPC = true;
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: TREE seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           in_File.seekg(-4, std::ios_base::cur);
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: TREE seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=48)
           {
             std::cout <<"Error: sub record CNAM of TREE has invalid length ("<<subLength
                       <<" bytes). Should be 48 bytes.\n";
             return false;
           }
           //read CNAM's stuff
           in_File.read((char*) unknownCNAM, 48);
           bytesRead += 48;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of TREE!\n";
             return false;
           }
           hasReadCNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODL, MODT, PFIG, SNAM, PFPC, "
                     << " or FULL are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!((!modelPath.empty()) and unknownMODT.isPresent() and hasReadPFPC and hasReadCNAM))
  {
    std::cout << "Error: while reading TREE record: at least one required subrecord is missing!\n";
    return false;
  }

  //check triplet consistency
  if (hasPFIG or hasSNAM or hasFULL)
  {
    if (!(hasPFIG and hasSNAM and hasFULL))
    {
      std::cout << "Error: while reading TREE record: at least one of the "
                << "subrecords PFIG, SNAM or FULL is missing!\n";
      return false;
    }
  }

  return in_File.good();
}

int32_t TreeRecord::getRecordType() const
{
  return cTREE;
}

} //namespace
