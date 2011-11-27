/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "ActivatorRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ActivatorRecord::ActivatorRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false),
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  unknownFULL = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  unknownPNAM = 0;
  hasRNAM = false;
  unknownRNAM = 0;
  unknownFNAM = 0;
  hasKNAM = false;
  unknownKNAM = 0;
}

ActivatorRecord::~ActivatorRecord()
{
  //empty
}

bool ActivatorRecord::equals(const ActivatorRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (unknownVMAD==other.unknownVMAD)
    and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
    and (hasFULL==other.hasFULL) and ((unknownFNAM==other.unknownFNAM) or (!hasFULL))
    and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
    and (unknownMODS==other.unknownMODS)
    and (unknownPNAM==other.unknownPNAM) and (unknownFNAM==other.unknownFNAM)
    and (hasRNAM==other.hasRNAM) and ((unknownRNAM==other.unknownRNAM) or (!hasRNAM))
    and (hasKNAM==other.hasKNAM) and ((unknownKNAM==other.unknownKNAM) or (!hasKNAM)));
}

bool ActivatorRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cCOBJ, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of strin +1 byte for NUL-termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +2 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize();
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of strin +1 byte for NUL-termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize();
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ +2 /* 2 bytes for length */ +unknownMODS.getSize();
  }
  if (hasRNAM)
  {
    writeSize = writeSize +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasKNAM)
  {
    writeSize = writeSize +4 /* KNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write VMAD
  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of ACTI!\n";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed size
  output.write((char*) &subLength, 2);
  //write OBND stuff
  output.write((char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write FULL stuff
    output.write((char*) &unknownFULL, 4);
  }//if FULL

  if (!modelPath.empty())
  {
    //write MODL
    output.write((char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if model path

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of ACTI!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing subrecord MODS of ACTI!\n";
      return false;
    }
  }

  //write PNAM
  output.write((char*) &cPNAM, 4);
  //PNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write PNAM stuff
  output.write((char*) &unknownPNAM, 4);

  if (hasRNAM)
  {
    //write RNAM
    output.write((char*) &cRNAM, 4);
    //RNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write RNAM stuff
    output.write((char*) &unknownRNAM, 4);
  }//if RNAM

  //write FNAM
  output.write((char*) &cFNAM, 4);
  //FNAM's length
  subLength = 2; //fixed size
  output.write((char*) &subLength, 2);
  //write FNAM stuff
  output.write((char*) &unknownFNAM, 2);

  if (hasKNAM)
  {
    //write KNAM
    output.write((char*) &cKNAM, 4);
    //KNAM's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write KNAM stuff
    output.write((char*) &unknownKNAM, 4);
  }//if KNAM

  return output.good();
}

bool ActivatorRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  uint32_t bytesRead;
  subRecName = subLength = 0;

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
    std::cout <<"Error: sub record EDID of ACTI is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ACTI!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  hasFULL = false;
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasKNAM = false;
  hasRNAM = false;
  bool hasReadOBND = false;
  bool hasReadPNAM = false;
  bool hasReadFNAM = false;
  while (bytesRead<readSize)
  {
    //read next record
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: ACTI seems to have more than one VMAD subrecord.\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false)) return false;
           bytesRead += (2+unknownVMAD.getSize());
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: ACTI seems to have more than one OBND subrecord.\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record OBND of ACTI has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read OBND
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord OBND of ACTI!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: ACTI seems to have more than one FULL subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, unknownFULL)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: ACTI seems to have more than one MODL subrecord.\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of ACTI is longer than 511 characters!\n";
             return false;
           }
           //read model path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of ACTI!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: ACTI seems to have more than one MODT subrecord.\n";
             return false;
           }
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of ACTI!\n";
             return false;
           }
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: ACTI seems to have more than one MODS subrecord.\n";
             return false;
           }
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cout << "Error while reading subrecord MODS of ACTI!\n";
             return false;
           }
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cout << "Error: ACTI seems to have more than one PNAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM)) return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cout << "Error: ACTI seems to have more than one RNAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read RNAM
           if (!loadUint32SubRecordFromStream(in_File, cRNAM, unknownRNAM)) return false;
           bytesRead += 6;
           hasRNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: ACTI seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record FNAM of ACTI has invalid length ("<<subLength
                       <<" bytes). Should be two bytes.\n";
             return false;
           }
           //read FNAM
           in_File.read((char*) &unknownFNAM, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of ACTI!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      case cKNAM:
           if (hasKNAM)
           {
             std::cout << "Error: ACTI seems to have more than one KNAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read KNAM
           if (!loadUint32SubRecordFromStream(in_File, cKNAM, unknownKNAM)) return false;
           bytesRead += 6;
           hasKNAM = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, FULL, PNAM, FNAM or KNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //check presence of all required subrecords
  if (!(hasReadOBND and hasReadPNAM and hasReadFNAM))
  {
    std::cout << "Error: At least one required subrecord of ACTI was not found!\n";
    return false;
  }
  return in_File.good();
}

int32_t ActivatorRecord::getRecordType() const
{
  return cACTI;
}

} //namespace
