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

#include "FurnitureRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

FurnitureRecord::FurnitureRecord()
: BasicRecord(), editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND(BinarySubRecord()),
  name(LocalizedString()), modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  keywords(std::vector<uint32_t>()),
  unknownPNAM(0), unknownFNAM(0),
  interactionKeywordFormID(0),
  unknownMNAM(0), unknownWBDT(0),
  unknownENAMs(std::vector<uint32_t>()),
  unknownNAM0s(std::vector<uint32_t>()),
  unknownFNMKs(std::vector<uint32_t>()),
  unknownFNPRs(std::vector<uint32_t>()), markerModelPath("")
{
  destruction.clear();
}

FurnitureRecord::~FurnitureRecord()
{

}

#ifndef SR_NO_RECORD_EQUALITY
bool FurnitureRecord::equals(const FurnitureRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD) and (unknownOBND==other.unknownOBND)
      and (name==other.name)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownMODS==other.unknownMODS)
      and (destruction==other.destruction)
      and (keywords==other.keywords)
      and (unknownPNAM==other.unknownPNAM) and (unknownFNAM==other.unknownFNAM)
      and (interactionKeywordFormID==other.interactionKeywordFormID)
      and (unknownMNAM==other.unknownMNAM) and (unknownWBDT==other.unknownWBDT)
      and (unknownENAMs==other.unknownENAMs) and (unknownNAM0s==other.unknownNAM0s)
      and (unknownFNMKs==other.unknownFNMKs)
      and (unknownFNPRs==other.unknownFNPRs) and (markerModelPath==other.markerModelPath)
    );
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FurnitureRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
             +editorID.length()+1 /* length of name +1 byte for NUL termination */
             +destruction.getWriteSize()
             +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */
             +4 /* FNAM */ +2 /* 2 bytes for length */ +2 /* fixed length */
             +4 /* MNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */
             +4 /* WBDT */ +2 /* 2 bytes for length */ +2 /* fixed length */
             +unknownENAMs.size()*(4 /* ENAM */ +2 /* 2 bytes for length */ +4 /* fixed size */)
             +unknownNAM0s.size()*(4 /* NAM0 */ +2 /* 2 bytes for length */ +4 /* fixed size */)
             +unknownFNMKs.size()*(4 /* FNMK */ +2 /* 2 bytes for length */ +4 /* fixed size */)
             +unknownFNPRs.size()*(4 /* FNPR */ +2 /* 2 bytes for length */ +4 /* fixed length */);
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ + unknownVMAD.getSize() /* size */;
  }
  if (unknownOBND.isPresent())
  {
    writeSize = writeSize +4 /* OBND */ +2 /* 2 bytes for length */ + unknownOBND.getSize() /* size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
             +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ + unknownMODT.getSize() /* size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize +4 /* MODS */ +2 /* 2 bytes for length */ + unknownMODS.getSize() /* size */;
  }
  if (!keywords.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ + 4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +keywords.size()*4 /* 4 bytes per element */;
  }
  if (interactionKeywordFormID!=0)
  {
    writeSize = writeSize +4 /* KNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }
  if (!markerModelPath.empty())
  {
    writeSize = writeSize +4 /* XMKR */ +2 /* 2 bytes for length */
               +markerModelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  return writeSize;
}

bool FurnitureRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cFURN, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

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
      std::cout << "Error while writing subrecord VMAD of FURN!\n";
      return false;
    }
  }//if VMAD

  if (unknownOBND.isPresent())
  {
    //write OBND
    if (!unknownOBND.saveToStream(output, cOBND))
    {
      std::cout << "Error while writing subrecord OBND of FURN!\n";
      return false;
    }
  }//if OBND

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
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
  }

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of FURN!\n";
      return false;
    }
  }//if MODT

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing subrecord MODS of FURN!\n";
      return false;
    }
  }//if MODS

  if (!destruction.saveToStream(output))
  {
    std::cout << "Error while writing destruction data of FURN!\n";
    return false;
  }

  if (!keywords.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write KSIZ's stuff
    const uint32_t k_Size = keywords.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size;
    output.write((const char*) &subLength, 2);
    //write keyword array
    unsigned int i;
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &keywords[i], 4);
    }//for
  }//if keywords

  //write PNAM
  output.write((const char*) &cPNAM, 4);
  //PNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write PNAM's stuff
  output.write((const char*) &unknownPNAM, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 2;
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &unknownFNAM, 2);

  if (interactionKeywordFormID!=0)
  {
    //write KNAM
    output.write((const char*) &cKNAM, 4);
    //KNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write KNAM's stuff
    output.write((const char*) &interactionKeywordFormID, 4);
  }//if KNAM

  //write MNAM
  output.write((const char*) &cMNAM, 4);
  //MNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write MNAM's stuff
  output.write((const char*) &unknownMNAM, 4);

  //write WBDT
  output.write((const char*) &cWBDT, 4);
  //WBDT's length
  subLength = 2;
  output.write((const char*) &subLength, 2);
  //write WBDT's stuff
  output.write((const char*) &unknownWBDT, 2);

  unsigned int i;
  for (i=0; i<unknownENAMs.size(); ++i)
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    //ENAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write ENAM's stuff
    output.write((const char*) &unknownENAMs[i], 4);
  }//for

  for (i=0; i<unknownNAM0s.size(); ++i)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    //NAM0's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write NAM0's stuff
    output.write((const char*) &unknownNAM0s[i], 4);
  }//for NAM0

  for (i=0; i<unknownFNMKs.size(); ++i)
  {
    //write FNMK
    output.write((const char*) &cFNMK, 4);
    //FNMK's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write FNMK's stuff
    output.write((const char*) &unknownFNMKs[i], 4);
  }//for FNMK

  for (i=0; i<unknownFNPRs.size(); ++i)
  {
    //write FNPR
    output.write((const char*) &cFNPR, 4);
    //FNPR's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write FNPR's stuff
    output.write((const char*) &unknownFNPRs[i], 4);
  }//for FNPR

  if (!markerModelPath.empty())
  {
    //write XMRK
    output.write((const char*) &cXMRK, 4);
    //XMRK's length
    subLength = markerModelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write marker model path
    output.write(markerModelPath.c_str(), subLength);
  }

  return output.good();
}
#endif

bool FurnitureRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;

  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  unknownVMAD.setPresence(false);
  unknownOBND.setPresence(false);
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  destruction.clear();
  keywords.clear();
  uint32_t tempUint32, k_Size, i;
  bool hasReadPNAM = false; unknownPNAM = 0;
  bool hasReadFNAM = false; unknownFNAM = 0;
  interactionKeywordFormID = 0;
  bool hasReadMNAM = false; unknownMNAM = 0;
  bool hasReadWBDT = false; unknownWBDT = 0;
  unknownENAMs.clear();
  unknownNAM0s.clear();
  unknownFNMKs.clear();
  unknownFNPRs.clear();
  markerModelPath.clear();

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
             std::cout << "Error: FURN seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
             return false;
           bytesRead += (2+unknownVMAD.getSize());
           break;
      case cOBND:
           if (unknownOBND.isPresent())
           {
             std::cout << "Error: FURN seems to have more than one OBND subrecord!\n";
             return false;
           }
           //read OBND
           if (!unknownOBND.loadFromStream(in_File, cOBND, false))
             return false;
           bytesRead += (2+unknownOBND.getSize());
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cout << "Error: FURN seems to have more than one FULL subrecord!\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: FURN seems to have more than one MODL subrecord!\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           //check content
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of FURN is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: FURN seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
             return false;
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: FURN seems to have more than one MODS subrecord!\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
             return false;
           bytesRead += (2+unknownMODS.getSize());
           break;
      case cDEST:
           if (destruction.isPresent)
           {
             std::cout << "Error: FURN seems to have more than one DEST subrecord.\n";
             return false;
           }
           //read DEST and possible DSTD, DMDL, DMDT, DSTF subrecords
           if (!destruction.loadFromStream(in_File, cFURN, buffer, bytesRead))
             return false;
           break;
      case cKSIZ:
           if (!keywords.empty())
           {
             std::cout << "Error: FURN seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //read KSIZ
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size, false))
             return false;
           bytesRead += 6;

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cKWDA)
           {
             UnexpectedRecord(cKWDA, subRecName);
             return false;
           }
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*k_Size)
           {
             std::cout << "Error: subrecord KWDA of FURN has invalid length ("
                       << subLength << " bytes). Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &tempUint32, 4);
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of FURN!\n";
               return false;
             }
             keywords.push_back(tempUint32);
           }//for
           bytesRead += subLength;
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cout << "Error: FURN seems to have more than one PNAM subrecord!\n";
             return false;
           }
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false))
             return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: FURN seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout << "Error: subrecord FNAM of FURN has invalid length ("
                       << subLength << " bytes). Should be two bytes!\n";
             return false;
           }
           //read FNAM's stuff
           in_File.read((char*) &unknownFNAM, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of FURN!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      case cKNAM:
           if (interactionKeywordFormID!=0)
           {
             std::cout << "Error: FURN seems to have more than one KNAM subrecord!\n";
             return false;
           }
           //read KNAM
           if (!loadUint32SubRecordFromStream(in_File, cKNAM, interactionKeywordFormID, false))
             return false;
           bytesRead += 6;
           //check content
           if (interactionKeywordFormID==0)
           {
             std::cout << "Error: subrecord KNAM of FURN is zero!\n";
             return false;
           }
           break;
      case cMNAM:
           if (hasReadMNAM)
           {
             std::cout << "Error: FURN seems to have more than one MNAM subrecord!\n";
             return false;
           }
           //read MNAM
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, unknownMNAM, false))
             return false;
           bytesRead += 6;
           hasReadMNAM = true;
           break;
      case cWBDT:
           if (hasReadWBDT)
           {
             std::cout << "Error: FURN seems to have more than one WBDT subrecord!\n";
             return false;
           }
           //WBDT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout << "Error: subrecord WBDT of FURN has invalid length ("
                       << subLength << " bytes). Should be two bytes!\n";
             return false;
           }
           //read WBDT's stuff
           in_File.read((char*) &unknownWBDT, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord WBDT of FURN!\n";
             return false;
           }
           hasReadWBDT = true;
           break;
      case cENAM:
           //read ENAM
           if (!loadUint32SubRecordFromStream(in_File, cENAM, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownENAMs.push_back(tempUint32);
           break;
      case cNAM0:
           //read NAM0
           if (!loadUint32SubRecordFromStream(in_File, cNAM0, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownNAM0s.push_back(tempUint32);
           break;
      case cFNMK:
           //read FNMK
           if (!loadUint32SubRecordFromStream(in_File, cFNMK, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownFNMKs.push_back(tempUint32);
           break;
      case cFNPR:
           //read FNPR
           if (!loadUint32SubRecordFromStream(in_File, cFNPR, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownFNPRs.push_back(tempUint32);
           break;
      case cXMRK:
           if (!markerModelPath.empty())
           {
             std::cout << "Error: FURN seems to have more than one XMRK subrecord!\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, markerModelPath, buffer, cXMRK, false, bytesRead))
             return false;
           //check content
           if (markerModelPath.empty())
           {
             std::cout << "Error: subrecord XMRK of FURN is empty!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, FULL, MODL, MODT, MODS, DEST,"
                     << " KSIZ, PNAM, FNAM, KNAM, MNAM, WBDT, ENAM, NAM0, FNMK,"
                     << " FNPR or XMRK are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadPNAM and hasReadFNAM
        and hasReadMNAM and hasReadWBDT))
  {
    std::cout << "Error: At least one required subrecord of FURN is missing!\n";
    std::cout << "PNAM: "<<hasReadPNAM << ", FNAM: "<<hasReadFNAM
              << ", MNAM: "<<hasReadMNAM<<", WBDT: "<<hasReadWBDT<<".\n";
    return false;
  }

  return in_File.good();
}

uint32_t FurnitureRecord::getRecordType() const
{
  return cFURN;
}

} //namespace
