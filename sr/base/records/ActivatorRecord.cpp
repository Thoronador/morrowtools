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

#include "ActivatorRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

bool ActivatorRecord::destStruct::operator==(const ActivatorRecord::destStruct& other) const
{
  return ((hasDSTD==other.hasDSTD) and ((memcmp(unknownDSTD, other.unknownDSTD, 20)==0) or (!hasDSTD))
      and (destroyedModelPath==other.destroyedModelPath) and (unknownDMDT==other.unknownDMDT)
      and (unknownDMDS==other.unknownDMDS));
}

void ActivatorRecord::destStruct::reset()
{
  hasDSTD = false;
  memset(unknownDSTD, 0, 20);
  destroyedModelPath = "";
  unknownDMDT.setPresence(false);
  unknownDMDS.setPresence(false);
}

/* ActivatorRecord's functions */

ActivatorRecord::ActivatorRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false),
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasDEST = false;
  memset(unknownDEST, 0, 8);
  destructionStructures.clear();
  keywordArray.clear();
  hasPNAM = false;
  unknownPNAM = 0;
  hasSNAM = false;
  unknownSNAM = 0;
  hasVNAM = false;
  unknownVNAM = 0;
  hasWNAM = false;
  unknownWNAM = 0;
  hasRNAM = false;
  unknownRNAM = 0;
  hasFNAM = false;
  unknownFNAM = 0;
  hasKNAM = false;
  unknownKNAM = 0;
}

ActivatorRecord::~ActivatorRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ActivatorRecord::equals(const ActivatorRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (unknownVMAD==other.unknownVMAD)
    and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
    and (hasFULL==other.hasFULL) and ((unknownFNAM==other.unknownFNAM) or (!hasFULL))
    and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
    and (unknownMODS==other.unknownMODS) and (hasDEST==other.hasDEST)
    and (memcmp(unknownDEST, other.unknownDEST, 8)==0)
    and (destructionStructures==other.destructionStructures)
    and (keywordArray==other.keywordArray)
    and (hasPNAM==other.hasPNAM) and ((unknownPNAM==other.unknownPNAM) or (!hasPNAM))
    and (hasFNAM==other.hasFNAM) and ((unknownFNAM==other.unknownFNAM) or (!hasFNAM))
    and (hasSNAM==other.hasSNAM) and ((unknownSNAM==other.unknownSNAM) or (!hasSNAM))
    and (hasVNAM==other.hasVNAM) and ((unknownVNAM==other.unknownVNAM) or (!hasVNAM))
    and (hasWNAM==other.hasWNAM) and ((unknownWNAM==other.unknownWNAM) or (!hasWNAM))
    and (hasRNAM==other.hasRNAM) and ((unknownRNAM==other.unknownRNAM) or (!hasRNAM))
    and (hasKNAM==other.hasKNAM) and ((unknownKNAM==other.unknownKNAM) or (!hasKNAM)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ActivatorRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of strin +1 byte for NUL-termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */;
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
        +modelPath.length()+1 /* length of string +1 byte for NUL-termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize();
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ +2 /* 2 bytes for length */ +unknownMODS.getSize();
  }
  if (hasDEST)
  {
    writeSize = writeSize +4 /* DEST */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (!destructionStructures.empty())
  {
    unsigned int i;
    for (i=0; i<destructionStructures.size(); ++i)
    {
      if (destructionStructures[i].hasDSTD)
      {
        writeSize = writeSize +4 /* DSTD */ +2 /* 2 bytes for length */ +20 /* fixed size */;
      }
      if (!destructionStructures[i].destroyedModelPath.empty())
      {
        writeSize = writeSize +4 /* DMDL */ +2 /* 2 bytes for length */
            +destructionStructures[i].destroyedModelPath.length()+1 /* length of string +1 byte for NUL-termination */;
      }
      if (destructionStructures[i].unknownDMDT.isPresent())
      {
        writeSize = writeSize + 4 /* DMDT */ +2 /* 2 bytes for length */ +destructionStructures[i].unknownDMDT.getSize();
      }
      if (destructionStructures[i].unknownDMDS.isPresent())
      {
        writeSize = writeSize + 4 /* DMDS */ +2 /* 2 bytes for length */ +destructionStructures[i].unknownDMDS.getSize();
      }
      writeSize = writeSize + 4 /* DSTF */ +2 /* 2 bytes for length */ +0 /* zero size */;
    }//for
  }//if not empty
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size();
  }
  if (hasPNAM)
  {
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasSNAM)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasVNAM)
  {
    writeSize = writeSize +4 /* VNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasWNAM)
  {
    writeSize = writeSize +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasRNAM)
  {
    writeSize = writeSize +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasFNAM)
  {
    writeSize = writeSize +4 /* FNAM */ +2 /* 2 bytes for length */ +2 /* fixed size */;
  }
  if (hasKNAM)
  {
    writeSize = writeSize +4 /* KNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool ActivatorRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cACTI, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
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
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed size
  output.write((const char*) &subLength, 2);
  //write OBND stuff
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write FULL stuff
    output.write((const char*) &nameStringID, 4);
  }//if FULL

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
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of ACTI!\n";
      return false;
    }
  }//if MODT

  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing subrecord MODS of ACTI!\n";
      return false;
    }
  }//if MODS

  if (hasDEST)
  {
    //write DEST
    output.write((const char*) &cDEST, 4);
    //DEST's length
    subLength = 8; //fixed
    output.write((const char*) &subLength, 2);
    //write DEST's data
    output.write((const char*) &unknownDEST, 8);
  }//if DEST

  if (!destructionStructures.empty())
  {
    unsigned int i;
    unsigned int count = destructionStructures.size();
    for (i=0; i<count; ++i)
    {
      if (destructionStructures[i].hasDSTD)
      {
        //write DSTD
        output.write((const char*) &cDSTD, 4);
        //DSTD's length
        subLength = 20; //fixed
        output.write((const char*) &subLength, 2);
        //write DSTD's data
        output.write((const char*) &destructionStructures[i].unknownDSTD, 20);
      }//if DSTD

      if (!destructionStructures[i].destroyedModelPath.empty())
      {
        //write DMDL
        output.write((const char*) &cDMDL, 4);
        //DMDL's length
        subLength = destructionStructures[i].destroyedModelPath.length()+1;
        output.write((const char*) &subLength, 2);
        //write destroyed model path
        output.write(destructionStructures[i].destroyedModelPath.c_str(), subLength);
      }//if dest. model path

      if (destructionStructures[i].unknownDMDT.isPresent())
      {
        if (!destructionStructures[i].unknownDMDT.saveToStream(output, cDMDT))
        {
          std::cout << "Error while writing subrecord DMDT of ACTI!\n";
          return false;
        }
      }//if DMDT

      if (destructionStructures[i].unknownDMDS.isPresent())
      {
        if (!destructionStructures[i].unknownDMDS.saveToStream(output, cDMDS))
        {
          std::cout << "Error while writing subrecord DMDS of ACTI!\n";
          return false;
        }
      }//if DMDS

      //write DSTF
      output.write((const char*) &cDSTF, 4);
      //DSTF's length
      subLength = 0; //fixed size
      output.write((const char*) &subLength, 2);
    }//for
  }//if not empty

  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write keyword size
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*keywordArray.size(); //fixed size
    output.write((const char*) &subLength, 2);
    //write keywords
    uint32_t i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keyword array

  if (hasPNAM)
  {
    //write PNAM
    output.write((const char*) &cPNAM, 4);
    //PNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write PNAM stuff
    output.write((const char*) &unknownPNAM, 4);
  }//if PNAM

  if (hasSNAM)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write SNAM stuff
    output.write((const char*) &unknownSNAM, 4);
  }//if SNAM

  if (hasVNAM)
  {
    //write VNAM
    output.write((const char*) &cVNAM, 4);
    //VNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write VNAM stuff
    output.write((const char*) &unknownVNAM, 4);
  }//if VNAM

  if (hasWNAM)
  {
    //write WNAM
    output.write((const char*) &cWNAM, 4);
    //WNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write WNAM stuff
    output.write((const char*) &unknownWNAM, 4);
  }//if WNAM

  if (hasRNAM)
  {
    //write RNAM
    output.write((const char*) &cRNAM, 4);
    //RNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write RNAM stuff
    output.write((const char*) &unknownRNAM, 4);
  }//if RNAM

  if (hasFNAM)
  {
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    //FNAM's length
    subLength = 2; //fixed size
    output.write((const char*) &subLength, 2);
    //write FNAM stuff
    output.write((const char*) &unknownFNAM, 2);
  }//if FNAM

  if (hasKNAM)
  {
    //write KNAM
    output.write((const char*) &cKNAM, 4);
    //KNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write KNAM stuff
    output.write((const char*) &unknownKNAM, 4);
  }//if KNAM

  return output.good();
}
#endif

bool ActivatorRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
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
  hasFULL = false; nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  destStruct tempDestStruct;
  bool hasBegunDestStruct = false;
  destructionStructures.clear();
  hasDEST = false;
  keywordArray.clear();
  hasKNAM = false; unknownKNAM = 0;
  hasSNAM = false; unknownSNAM = 0;
  hasVNAM = false; unknownVNAM = 0;
  hasWNAM = false; unknownWNAM = 0;
  hasRNAM = false; unknownRNAM = 0;
  bool hasReadOBND = false;
  hasPNAM = false; unknownPNAM = 0;
  hasFNAM = false; unknownFNAM = 0;

  uint32_t k_Size, i, temp;
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
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID, false)) return false;
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
           bytesRead = bytesRead + 2 + unknownMODT.getSize();
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
           bytesRead = bytesRead + 2 + unknownMODS.getSize();
           break;
      case cDEST:
           if (hasDEST)
           {
             std::cout << "Error: ACTI seems to have more than one DEST subrecord.\n";
             return false;
           }
           //DEST's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DEST of ACTI has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read DEST
           in_File.read((char*) unknownDEST, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DEST of ACTI!\n";
             return false;
           }
           hasDEST = true;
           break;
      case cDSTD:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (tempDestStruct.hasDSTD)
           {
             std::cout << "Error: ACTI seems to have more than one DSTD subrecord.\n";
             return false;
           }
           //DSTD's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=20)
           {
             std::cout <<"Error: sub record DSTD of ACTI has invalid length ("
                       <<subLength<<" bytes). Should be 20 bytes.\n";
             return false;
           }
           //read DSTD
           in_File.read((char*) (tempDestStruct.unknownDSTD), 20);
           bytesRead += 20;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DSTD of ACTI!\n";
             return false;
           }
           tempDestStruct.hasDSTD = true;
           hasBegunDestStruct = true;
           break;
      case cDMDL:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (!tempDestStruct.destroyedModelPath.empty())
           {
             std::cout << "Error: ACTI seems to have more than one DMDL subrecord.\n";
             return false;
           }
           //DMDL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record DMDL of ACTI is longer than 511 characters!\n";
             return false;
           }
           //read dest. model path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DMDL of ACTI!\n";
             return false;
           }
           tempDestStruct.destroyedModelPath = std::string(buffer);
           hasBegunDestStruct = true;
           break;
      case cDMDT:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (tempDestStruct.unknownDMDT.isPresent())
           {
             std::cout << "Error: ACTI seems to have more than one DMDT subrecord.\n";
             return false;
           }
           if (!tempDestStruct.unknownDMDT.loadFromStream(in_File, cDMDT, false))
           {
             std::cout << "Error while reading subrecord DMDT of ACTI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempDestStruct.unknownDMDT.getSize();
           hasBegunDestStruct = true;
           break;
      case cDMDS:
           if (!hasBegunDestStruct)
           {
             tempDestStruct.reset();
           }
           if (tempDestStruct.unknownDMDS.isPresent())
           {
             std::cout << "Error: ACTI seems to have more than one DMDS subrecord.\n";
             return false;
           }
           if (!tempDestStruct.unknownDMDS.loadFromStream(in_File, cDMDS, false))
           {
             std::cout << "Error while reading subrecord DMDS of ACTI!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempDestStruct.unknownDMDS.getSize();
           hasBegunDestStruct = true;
           break;
      case cDSTF:
           if (!hasBegunDestStruct)
           {
             std::cout << "Error while reading record of type ACTI: no data prior to DSTF.\n";
             return false;
           }
           //DSTF's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout <<"Error: sub record DSTF of ACTI has invalid length ("
                       <<subLength<<" bytes). Should be zero bytes!\n";
             return false;
           }
           destructionStructures.push_back(tempDestStruct);
           hasBegunDestStruct = false;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: ACTI seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size, false)) return false;
           bytesRead += 6;

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*k_Size)
           {
             std::cout <<"Error: sub record KWDA of ACTI has invalid length ("
                       <<subLength<<" bytes). Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read keywords
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of ACTI!\n";
               return false;
             }
             keywordArray.push_back(temp);
           }//for
           break;
      case cPNAM:
           if (hasPNAM)
           {
             std::cout << "Error: ACTI seems to have more than one PNAM subrecord.\n";
             return false;
           }
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false)) return false;
           bytesRead += 6;
           hasPNAM = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: ACTI seems to have more than one SNAM subrecord.\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM, false)) return false;
           bytesRead += 6;
           hasSNAM = true;
           break;
      case cVNAM:
           if (hasVNAM)
           {
             std::cout << "Error: ACTI seems to have more than one VNAM subrecord.\n";
             return false;
           }
           //read VNAM
           if (!loadUint32SubRecordFromStream(in_File, cVNAM, unknownVNAM, false)) return false;
           bytesRead += 6;
           hasVNAM = true;
           break;
      case cWNAM:
           if (hasWNAM)
           {
             std::cout << "Error: ACTI seems to have more than one WNAM subrecord.\n";
             return false;
           }
           //read WNAM
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, unknownWNAM, false)) return false;
           bytesRead += 6;
           hasWNAM = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cout << "Error: ACTI seems to have more than one RNAM subrecord.\n";
             return false;
           }
           //read RNAM
           if (!loadUint32SubRecordFromStream(in_File, cRNAM, unknownRNAM, false)) return false;
           bytesRead += 6;
           hasRNAM = true;
           break;
      case cFNAM:
           if (hasFNAM)
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
           hasFNAM = true;
           break;
      case cKNAM:
           if (hasKNAM)
           {
             std::cout << "Error: ACTI seems to have more than one KNAM subrecord.\n";
             return false;
           }
           //read KNAM
           if (!loadUint32SubRecordFromStream(in_File, cKNAM, unknownKNAM, false)) return false;
           bytesRead += 6;
           hasKNAM = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, FULL, PNAM, FNAM, SNAM or KNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //check presence of all required subrecords
  if (!hasReadOBND)
  {
    std::cout << "Error: At least one required subrecord of ACTI was not found!\n";
    return false;
  }
  if (hasBegunDestStruct)
  {
    std::cout << "Error: Unfinished DSTF stuff in ACTI!\n";
    return false;
  }
  return in_File.good();
}

uint32_t ActivatorRecord::getRecordType() const
{
  return cACTI;
}

} //namespace
