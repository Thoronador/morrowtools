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

#include "ReferenceRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

ReferenceRecord::ReferenceRecord()
: BasicRecord(), editorID(""),
  baseObjectFormID(0),
  hasXNDP(false), unknownXNDP(0),
  unknownXLKRs(std::vector<std::vector<uint32_t> >()),
  hasXESP(false), unknownXESP(0),
  hasXEMI(false), unknownXEMI(0),
  unknownXSCL(1.0f),
  hasXPRD(false), unknownXPRD(0),
  hasINAM(false), unknownINAM(0),
  hasPDTO(false), unknownPDTO(0),
  subBlocks(std::vector<SubBlock>())
{
  unknownVMAD.setPresence(false);
  unknownXTEL.setPresence(false);
  unknownXPRM.setPresence(false);
  unknownXLOC.setPresence(false);
  memset(unknownDATA, 0, 24);
}

ReferenceRecord::~ReferenceRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ReferenceRecord::equals(const ReferenceRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (baseObjectFormID==other.baseObjectFormID) and (unknownXTEL==other.unknownXTEL)
      and (hasXNDP==other.hasXNDP) and ((unknownXNDP==other.unknownXNDP) or (!hasXNDP))
      and (unknownXLKRs==other.unknownXLKRs)
      and (hasXESP==other.hasXESP) and ((unknownXESP==other.unknownXESP) or (!hasXESP))
      and (hasXEMI==other.hasXEMI) and ((unknownXEMI==other.unknownXEMI) or (!hasXEMI))
      and (unknownXLOC==other.unknownXLOC) and (unknownXPRM==other.unknownXPRM)
      and (unknownXSCL==other.unknownXSCL)
      and (hasXPRD==other.hasXPRD) and ((unknownXPRD==other.unknownXPRD) or (!hasXPRD))
      and (hasINAM==other.hasINAM) and ((unknownINAM==other.unknownINAM) or (!hasINAM))
      and (hasPDTO==other.hasPDTO) and ((unknownPDTO==other.unknownPDTO) or (!hasPDTO))
      and (subBlocks==other.subBlocks)
      and (memcmp(unknownDATA, other.unknownDATA, 24)==0));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ReferenceRecord::getWriteSize() const
{
  #warning Not completely implemented yet!
  uint32_t writeSize;
  writeSize = 4 /* NAME */ +2 /* 2 bytes for length */ +4 /* fixed size */
             +4 /* DATA */ +2 /* 2 bytes for length */ +24 /* fixed size */;
  if (!editorID.empty())
  {
    writeSize = writeSize + 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* size of record data */;
  }
  if (unknownXTEL.isPresent())
  {
    writeSize = writeSize + 4 /* XTEL */ + 2 /* 2 bytes for length */ + unknownXTEL.size() /* size of record data */;
  }
  if (hasXNDP)
  {
    writeSize = writeSize +4 /* XNDP */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (!unknownXLKRs.empty())
  {
    const unsigned int count = unknownXLKRs.size();
    unsigned int i;
    for (i=0; i<count; ++i)
    {
      writeSize = writeSize +4 /* XLKR */ +2 /* 2 bytes for length */ +4*unknownXLKRs[i].size() /* fixed size */;
    }
  }
  if (hasXESP)
  {
    writeSize = writeSize +4 /* XESP */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (hasXEMI)
  {
    writeSize = writeSize +4 /* XEMI */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unknownXPRM.isPresent())
  {
    writeSize = writeSize + 4 /* XPRM */ + 2 /* 2 bytes for length */ + unknownXPRM.size() /* size of record data */;
  }
  if (unknownXLOC.isPresent())
  {
    writeSize = writeSize + 4 /* XLOC */ + 2 /* 2 bytes for length */ + unknownXLOC.size() /* size of record data */;
  }
  if (unknownXSCL != 1.0f)
  {
    writeSize = writeSize + 4 /* XSCL */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (hasXPRD)
  {
    writeSize = writeSize +4 /* XPRD */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasINAM)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasPDTO)
  {
    writeSize = writeSize +4 /* PDTO */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  const unsigned int count = subBlocks.size();
  unsigned int i;
  for (i=0; i<count; ++i)
  {
    if (subBlocks[i].subData.isPresent())
    {
      writeSize = writeSize + 4 /* label */ + 2 /* 2 bytes for length */ + subBlocks[i].subData.size() /* size of data*/;
    }
  }
  return writeSize;
}

bool ReferenceRecord::saveToStream(std::ostream& output) const
{
  #warning Not completely implemented yet!
  output.write((const char*) &cREFR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  uint16_t subLength;
  if (!editorID.empty())
  {
    //write EDID
    output.write((const char*) &cEDID, 4);
    //EDID's length
    subLength = editorID.length()+1;
    output.write((const char*) &subLength, 2);
    //write editor ID
    output.write(editorID.c_str(), subLength);
  }//if editorID

  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing subrecord VMAD of REFR!\n";
      return false;
    }
  }//if VMAD

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write base object's form ID
  output.write((const char*) &baseObjectFormID, 4);

  if (unknownXTEL.isPresent())
  {
    if (!unknownXTEL.saveToStream(output, cXTEL))
    {
      std::cerr << "Error while writing subrecord XTEL of REFR!\n";
      return false;
    }
  }//if XTEL

  if (hasXNDP)
  {
    //write XNDP
    output.write((const char*) &cXNDP, 4);
    //XNDP's length
    subLength = 8; //fixed size
    output.write((const char*) &subLength, 2);
    //write XNDP
    output.write((const char*) &unknownXNDP, 8);
  }//if XNDP

  if (!unknownXLKRs.empty())
  {
    unsigned int i, j;
    const unsigned int count = unknownXLKRs.size();
    for (i=0; i<count; ++i)
    {
      //write XLKR
      output.write((const char*) &cXLKR, 4);
      //XLKR's length
      subLength = 4*unknownXLKRs[i].size(); //fixed size
      output.write((const char*) &subLength, 2);
      //write XLKR
      for (j=0; j<unknownXLKRs[i].size(); ++j)
      {
        output.write((const char*) &(unknownXLKRs[i][j]), 4);
      }//for j
    }//for i
  }//if XLKR

  if (hasXESP)
  {
    //write XESP
    output.write((const char*) &cXESP, 4);
    //XESP's length
    subLength = 8; //fixed size
    output.write((const char*) &subLength, 2);
    //write XESP
    output.write((const char*) &unknownXESP, 8);
  }//if XESP

  if (hasXEMI)
  {
    //write XEMI
    output.write((const char*) &cXEMI, 4);
    //XEMI's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XEMI
    output.write((const char*) &unknownXEMI, 4);
  }//if XEMI

  if (unknownXPRM.isPresent())
  {
    //write XPRM
    if (!unknownXPRM.saveToStream(output, cXPRM))
    {
      std::cerr << "Error while writing subrecord XPRM of REFR!\n";
      return false;
    }
  }//if XPRM

  if (unknownXLOC.isPresent())
  {
    //write XLOC
    if (!unknownXLOC.saveToStream(output, cXLOC))
    {
      std::cerr << "Error while writing subrecord XLOC of REFR!\n";
      return false;
    }
  }//if XLOC

  if (unknownXSCL!=1.0f)
  {
    //write XSCL
    output.write((const char*) &cXSCL, 4);
    //XSCL's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XSCL
    output.write((const char*) &unknownXSCL, 4);
  }//if XSCL

  if (hasXPRD)
  {
    //write XPRD
    output.write((const char*) &cXPRD, 4);
    //XPRD's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XPRD
    output.write((const char*) &unknownXPRD, 4);
  }//if XPRD

  if (hasINAM)
  {
    //write INAM
    output.write((const char*) &cINAM, 4);
    //INAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write INAM
    output.write((const char*) &unknownINAM, 4);
  }//if INAM

  if (hasPDTO)
  {
    //write PDTO
    output.write((const char*) &cPDTO, 4);
    //PDTO's length
    subLength = 8; //fixed size
    output.write((const char*) &subLength, 2);
    //write PDTO
    output.write((const char*) &unknownPDTO, 8);
  }//if PDTO

  const unsigned int count = subBlocks.size();
  unsigned int i;
  for (i=0; i<count; ++i)
  {
    if (subBlocks[i].subData.isPresent())
    {
      if (!subBlocks[i].subData.saveToStream(output, subBlocks[i].subType))
      {
        std::cerr << "Error while writing subrecord "<<IntTo4Char(subBlocks[i].subType)<<" of REFR!\n";
        return false;
      }
    }
  }//for

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 24; //fixed size
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) unknownDATA, 24);

  return false;
}
#endif

bool ReferenceRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  #warning Not completely implemented yet!
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  char buffer[512];

  editorID.clear();
  unknownVMAD.setPresence(false);
  bool hasReadNAME = false; baseObjectFormID = 0;
  unknownXTEL.setPresence(false);
  hasXNDP = false; unknownXNDP = 0;
  unknownXLKRs.clear();
  std::vector<uint32_t> tempVec32;
  uint32_t i, tempUint32;
  hasXESP = false; unknownXESP = 0;
  hasXEMI = false; unknownXEMI = 0;
  unknownXPRM.setPresence(false);
  unknownXLOC.setPresence(false);
  unknownXSCL = 1.0f;
  hasXPRD = false; unknownXPRD = 0;
  hasINAM = false; unknownINAM = 0;
  hasPDTO = false; unknownPDTO = 0;
  subBlocks.clear();
  SubBlock tempSubBlock;
  bool hasReadDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cEDID:
           if (!editorID.empty())
           {
             std::cerr << "Error: REFR seems to have more than one EDID subrecord!\n";
             return false;
           }
           //EDID's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record EDID of REFR is longer than 511 characters!\n";
             return false;
           }
           //read EDID's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord EDID of REFR!\n";
             return false;
           }
           editorID = std::string(buffer);
           break;
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: REFR seems to have more than one VMAD subrecord!\n";
             return false;
           }
           // read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cerr << "Error while reading subrecord VMAD of REFR!\n!";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownVMAD.size();
           break;
      case cNAME:
           if (hasReadNAME)
           {
             std::cerr << "Error: REFR seems to have more than one NAME subrecord!\n";
             return false;
           }
           //read NAME
           if (!loadUint32SubRecordFromStream(in_File, cNAME, baseObjectFormID, false)) return false;
           bytesRead += 6;
           hasReadNAME = true;
           break;
      case cXTEL:
           if (unknownXTEL.isPresent())
           {
             std::cerr << "Error: REFR seems to have more than one XTEL subrecord!\n";
             return false;
           }
           // read XTEL
           if (!unknownXTEL.loadFromStream(in_File, cXTEL, false))
           {
             std::cerr << "Error while reading subrecord XTEL of REFR!\n!";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownXTEL.size();
           if (unknownXTEL.size() != 32)
           {
             std::cerr << "Error: sub record XTEL of REFR has invalid length ("
                       << unknownXTEL.size()<< " bytes). Should be 32 bytes.\n";
             return false;
           }
           break;
      case cXNDP:
           if (hasXNDP)
           {
             std::cerr << "Error: REFR seems to have more than one XNDP subrecord!\n";
             return false;
           }
           // XLKR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: sub record XNDP of REFR has invalid length ("
                       << subLength << " bytes). Should be 8 bytes.\n";
             return false;
           }
           // read XNDP
           in_File.read((char*) &unknownXNDP, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XNDP of REFR!\n";
             return false;
           }
           hasXNDP = true;
           break;
      case cXLKR:
           //XLKR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength!=8) and (subLength!=4))
           {
             std::cerr <<"Error: sub record XLKR of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 8 or 4 bytes.\n";
             return false;
           }
           //read XLKR
           tempVec32.clear();
           for (i=0; i<subLength; i=i+4)
           {
             in_File.read((char*) &tempUint32, 4);
             tempVec32.push_back(tempUint32);
           }//for
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XLKR of REFR!\n";
             return false;
           }
           unknownXLKRs.push_back(tempVec32);
           break;
      case cXESP:
           if (hasXESP)
           {
             std::cerr << "Error: REFR seems to have more than one XESP subrecord!\n";
             return false;
           }
           //XESP's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cerr <<"Error: sub record XESP of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read XESP
           in_File.read((char*) &unknownXESP, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XESP of REFR!\n";
             return false;
           }
           hasXESP = true;
           break;
      case cXEMI:
           if (hasXEMI)
           {
             std::cerr << "Error: REFR seems to have more than one XEMI subrecord!\n";
             return false;
           }
           //read XEMI
           if (!loadUint32SubRecordFromStream(in_File, cXEMI, unknownXEMI, false)) return false;
           bytesRead += 6;
           hasXEMI = true;
           break;
      case cXPRM:
           if (unknownXPRM.isPresent())
           {
             std::cerr << "Error: REFR seems to have more than one XPRM subrecord!\n";
             return false;
           }
           // read XPRM
           if (!unknownXPRM.loadFromStream(in_File, cXPRM, false))
           {
             std::cerr << "Error while reading subrecord XPRM of REFR!\n!";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownXPRM.size();
           if (unknownXPRM.size() != 32)
           {
             std::cerr << "Error: sub record XPRM of REFR has invalid length ("
                       << unknownXPRM.size() << " bytes). Should be 32 bytes.\n";
             return false;
           }
           break;
      case cXLOC:
           if (unknownXLOC.isPresent())
           {
             std::cerr << "Error: REFR seems to have more than one XLOC subrecord!\n";
             return false;
           }
           // read XLOC
           if (!unknownXLOC.loadFromStream(in_File, cXLOC, false))
           {
             std::cerr << "Error while reading subrecord XLOC of REFR!\n!";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownXLOC.size();
           break;
      case cXSCL:
           if (unknownXSCL!=1.0f)
           {
             std::cerr << "Error: REFR seems to have more than one XSCL subrecord!\n";
             return false;
           }
           //XSCL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: sub record XSCL of REFR has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read XSCL
           in_File.read((char*) &unknownXSCL, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XSCL of REFR!\n";
             return false;
           }
           break;
      case cXPRD:
           if (hasXPRD)
           {
             std::cerr << "Error: REFR seems to have more than one XPRD subrecord!\n";
             return false;
           }
           //read XPRD
           if (!loadUint32SubRecordFromStream(in_File, cXPRD, unknownXPRD, false)) return false;
           bytesRead += 6;
           hasXPRD = true;
           break;
      case cINAM:
           if (hasINAM)
           {
             std::cerr << "Error: REFR seems to have more than one INAM subrecord!\n";
             return false;
           }
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, unknownINAM, false)) return false;
           bytesRead += 6;
           hasINAM = true;
           break;
      case cPDTO:
           if (hasPDTO)
           {
             std::cerr << "Error: REFR seems to have more than one PDTO subrecord!\n";
             return false;
           }
           //PDTO's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cerr <<"Error: sub record PDTO of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read PDTO
           in_File.read((char*) &unknownPDTO, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord PDTO of REFR!\n";
             return false;
           }
           hasPDTO = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: REFR seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=24)
           {
             std::cerr <<"Error: sub record DATA of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 24 bytes.\n";
             return false;
           }
           //read DATA
           in_File.read((char*) unknownDATA, 24);
           bytesRead += 24;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of REFR!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           tempSubBlock.subType = subRecName;
           if (!tempSubBlock.subData.loadFromStream(in_File, subRecName, false))
           {
             std::cerr << "Error while loading subrecord " << IntTo4Char(subRecName) << " of REFR into sub-blocks!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + tempSubBlock.subData.size();
           subBlocks.push_back(tempSubBlock);
           /* std::cerr << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only EDID, VMAD, NAME, XTEL, XNDP, XLKR, XESP,"
                     << " XEMI, XPRM, XLOC, XSCL, XPRD, INAM, PDTO or DATA are allowed here!\n";
           return false; */
           break;
    }//swi
  }//while

  //presence checks
  // - NAME required for all
  // - DATA only required for non-deleted records
  if (!(hasReadNAME and (hasReadDATA or isDeleted())))
  {
    std::cerr << "Error: subrecord NAME or DATA of REFR is missing!\n";
    return false;
  }

  #warning Not completely implemented yet!
  return in_File.good();
}

uint32_t ReferenceRecord::getRecordType() const
{
  return cREFR;
}

} //namespace
