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

#include "CharacterReferenceRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* EnableParent's functions */
const uint32_t CharacterReferenceRecord::EnableParent::FlagSetEnableStateToOppositeOfParent = 0x00000001;
const uint32_t CharacterReferenceRecord::EnableParent::FlagPopIn                            = 0x00000002;

bool CharacterReferenceRecord::EnableParent::oppositeOfParent() const
{
  return ((flags & FlagSetEnableStateToOppositeOfParent)!=0);
}

bool CharacterReferenceRecord::EnableParent::popsIn() const
{
  return ((flags & FlagPopIn)!=0);
}

bool CharacterReferenceRecord::EnableParent::isPresent() const
{
  return (refID!=0);
}

bool CharacterReferenceRecord::EnableParent::operator==(const EnableParent& other) const
{
  return ((refID==other.refID) and (flags==other.flags));
}

void CharacterReferenceRecord::EnableParent::clear()
{
  refID = 0;
  flags = 0;
}

/* XAPD_XAPR_pair's functions */
/*bool CharacterReferenceRecord::XAPD_XAPR_pair::operator==(const CharacterReferenceRecord::XAPD_XAPR_pair& other) const
{
  return ((unknownXAPD==other.unknownXAPD) and (unknownXAPR==other.unknownXAPR));
}*/

/* CharactereferenceRecord's functions */

CharacterReferenceRecord::CharacterReferenceRecord()
: BasicRecord(), editorID(""),
  baseObjectFormID(0), //subrecord NAME
  hasXLCM(false), unknownXLCM(0),
  hasINAM(false), unknownINAM(0),
  hasPDTO(false), unknownPDTO(0),
  linkedReferences(std::vector<LinkedRef>()),
  XAPDs(std::vector<uint8_t>()),
  XAPRs(std::vector<uint64_t>()),
  persistenceLocationFormID(0), //subrecord XLCN
  locationRefTypeFormID(0), //subrecord XLRT
  encounterZoneFormID(0), //subrecord XEZN
  ownerFormID(0),
  scale(1.0f), //subrecord XSCL
  subBlocks(std::vector<SubBlock>())
{
  unknownVMAD.setPresence(false);
  unknownXRGD.setPresence(false);
  unknownXRGB.setPresence(false);
  subXESP.clear();
  memset(unknownDATA, 0, 24);
}

CharacterReferenceRecord::~CharacterReferenceRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool CharacterReferenceRecord::equals(const CharacterReferenceRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (baseObjectFormID==other.baseObjectFormID) and (unknownXRGD==other.unknownXRGD)
      and (unknownXRGB==other.unknownXRGB)
      and (hasXLCM==other.hasXLCM) and ((unknownXLCM==other.unknownXLCM) or (!hasXLCM))
      and (hasINAM==other.hasINAM) and ((unknownINAM==other.unknownINAM) or (!hasINAM))
      and (hasPDTO==other.hasPDTO) and ((unknownPDTO==other.unknownPDTO) or (!hasPDTO))
      and (linkedReferences==other.linkedReferences) and (XAPDs==other.XAPDs)
      and (XAPRs==other.XAPRs)
      and (persistenceLocationFormID==other.persistenceLocationFormID)
      and (locationRefTypeFormID==other.locationRefTypeFormID)
      and (encounterZoneFormID==other.encounterZoneFormID) and (subXESP==other.subXESP)
      and (ownerFormID==other.ownerFormID) and (scale==other.scale)
      and (subBlocks==other.subBlocks)
      and (memcmp(unknownDATA, other.unknownDATA, 24)==0));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CharacterReferenceRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* NAME */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */
        +linkedReferences.size()*(4 /* XLKR */ +2 /* 2 bytes for length */ +4 /* fixed length of 8 bytes */)
        +4 /* DATA */ +2 /* 2 bytes for length */ +24 /* fixed length of 24 bytes */;
  if (!editorID.empty())
  {
    writeSize = writeSize +4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* size */;
  }//if VMAD
  if (unknownXRGD.isPresent())
  {
    writeSize = writeSize +4 /* XRGD */ +2 /* 2 bytes for length */ +unknownXRGD.getSize() /* size */;
  }//if XRGD
  if (unknownXRGB.isPresent())
  {
    writeSize = writeSize +4 /* XRGB */ +2 /* 2 bytes for length */ +unknownXRGB.getSize() /* size */;
  }//if XRGB
  if (hasXLCM)
  {
    writeSize = writeSize +4 /* XLCM */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if XLCM
  if (hasINAM)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasPDTO)
  {
    writeSize = writeSize +4 /* PDTO */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (!XAPDs.empty())
  {
    writeSize = writeSize +XAPDs.size()*(4 /* XAPD */ +2 /* 2 bytes for length */ +1 /* fixed length of 1 byte */);
  }//if XAPDs
  if (!XAPRs.empty())
  {
    writeSize = writeSize +XAPRs.size()*(4 /* XAPR */ +2 /* 2 bytes for length */ +8 /* fixed length of 8 bytes */);
  }//if XAPRs
  if (persistenceLocationFormID!=0)
  {
    writeSize = writeSize +4 /* XLCN */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if XLCN
  if (locationRefTypeFormID!=0)
  {
    writeSize = writeSize +4 /* XLRT */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if XLRT
  if (encounterZoneFormID!=0)
  {
    writeSize = writeSize +4 /* XEZN */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if XEZN
  if (subXESP.isPresent())
  {
    writeSize = writeSize +4 /* XESP */ +2 /* 2 bytes for length */ +8 /* fixed length of 8 bytes */;
  }//if XESP
  if (ownerFormID!=0)
  {
    writeSize = writeSize +4 /* XOWN */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if XOWN
  if (scale!=1.0f)
  {
    writeSize = writeSize +4 /* XSCL */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 bytes */;
  }//if XSCL
  const unsigned int count = subBlocks.size();
  unsigned int i;
  for (i=0; i<count; ++i)
  {
    if (subBlocks[i].subData.isPresent())
    {
      writeSize = writeSize + 4 /* label */ +2 /* 2 bytes for length */ +subBlocks[i].subData.getSize() /* size of data*/;
    }
  }//for
  return writeSize;
}

bool CharacterReferenceRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cACHR, 4);
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
  }//if editor ID

  if (unknownVMAD.isPresent())
  {
    //write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of ACHR!\n";
      return false;
    }
  }//if VMAD

  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write NAME
  output.write((const char*) &baseObjectFormID, 4);

  if (unknownXRGD.isPresent())
  {
    //write XRGD
    if (!unknownXRGD.saveToStream(output, cXRGD))
    {
      std::cout << "Error while writing subrecord XRGD of ACHR!\n";
      return false;
    }
  }//if XRGD

  if (unknownXRGB.isPresent())
  {
    //write XRGB
    if (!unknownXRGB.saveToStream(output, cXRGB))
    {
      std::cout << "Error while writing subrecord XRGB of ACHR!\n";
      return false;
    }
  }//if XRGB

  if (hasXLCM)
  {
    //write XLCM
    output.write((const char*) &cXLCM, 4);
    //XLCM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XLCM
    output.write((const char*) &unknownXLCM, 4);
  }//if XLCM

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

  if (!linkedReferences.empty())
  {
    const unsigned int len = linkedReferences.size();
    unsigned int i;
    for (i=0; i<len; ++i)
    {
      //write XLKR
      output.write((const char*) &cXLKR, 4);
      //XLKR's length
      subLength = 8; //fixed size
      output.write((const char*) &subLength, 2);
      //write XLKR
      output.write((const char*) &linkedReferences[i].keywordFormID, 4);
      output.write((const char*) &linkedReferences[i].referenceFormID, 4);
    }//for
  }//if linked references

  if (!XAPDs.empty())
  {
    unsigned int i;
    const unsigned int len = XAPDs.size();
    for (i=0; i<len; ++i)
    {
      //write XAPD
      output.write((const char*) &cXAPD, 4);
      //XAPD's length
      subLength = 1; //fixed size
      output.write((const char*) &subLength, 2);
      //write XAPD
      output.write((const char*) &XAPDs[i], 1);
    }//for
  }//if XAPDs

  if (!XAPRs.empty())
  {
    unsigned int i;
    const unsigned int len = XAPRs.size();
    for (i=0; i<len; ++i)
    {
      //write XAPR
      output.write((const char*) &cXAPR, 4);
      //XAPR's length
      subLength = 8; //fixed size
      output.write((const char*) &subLength, 2);
      //write XAPR
      output.write((const char*) &XAPRs[i], 8);
    }//for
  }//if XAPRs

  if (persistenceLocationFormID!=0)
  {
    //write XLCN
    output.write((const char*) &cXLCN, 4);
    //XLCN's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XLCN
    output.write((const char*) &persistenceLocationFormID, 4);
  }//if XLCN

  if (locationRefTypeFormID!=0)
  {
    //write XLRT
    output.write((const char*) &cXLRT, 4);
    //XLRT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XLRT
    output.write((const char*) &locationRefTypeFormID, 4);
  }//if XLRT

  if (encounterZoneFormID!=0)
  {
    //write XEZN
    output.write((const char*) &cXEZN, 4);
    //XEZN's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XEZN
    output.write((const char*) &encounterZoneFormID, 4);
  }//if XEZN

  if (subXESP.isPresent())
  {
    //write XESP
    output.write((const char*) &cXESP, 4);
    //XESP's length
    subLength = 8; //fixed size
    output.write((const char*) &subLength, 2);
    //write XESP
    output.write((const char*) &subXESP.refID, 4);
    output.write((const char*) &subXESP.flags, 4);
  }//if XESP

  if (ownerFormID!=0)
  {
    //write XOWN
    output.write((const char*) &cXOWN, 4);
    //XOWN's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XOWN
    output.write((const char*) &ownerFormID, 4);
  }//if XOWN

  if (scale!=1.0f)
  {
    //write XSCL
    output.write((const char*) &cXSCL, 4);
    //XSCL's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write XSCL
    output.write((const char*) &scale, 4);
  }//if scale

  const unsigned int count = subBlocks.size();
  unsigned int i;
  for (i=0; i<count; ++i)
  {
    if (subBlocks[i].subData.isPresent())
    {
      if (!subBlocks[i].subData.saveToStream(output, subBlocks[i].subType))
      {
        std::cout << "Error while writing subrecord "<<IntTo4Char(subBlocks[i].subType)<<" of ACHR!\n";
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

  return output.good();
}
#endif

bool CharacterReferenceRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  #warning Not completely implemented yet!
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  editorID.clear();
  char buffer[512];
  unknownVMAD.setPresence(false);
  baseObjectFormID = 0;
  unknownXRGD.setPresence(false);
  unknownXRGB.setPresence(false);
  hasXLCM = false; unknownXLCM = 0;
  hasINAM = false; unknownINAM = 0;
  hasPDTO = false; unknownPDTO = 0;
  linkedReferences.clear();
  LinkedRef tempLinkedRef;
  XAPDs.clear();
  uint8_t tempUint8;
  XAPRs.clear();
  uint64_t tempUint64;
  persistenceLocationFormID = 0;
  locationRefTypeFormID = 0;
  encounterZoneFormID = 0;
  subXESP.clear();
  ownerFormID = 0;
  scale = 1.0f;
  subBlocks.clear();
  SubBlock tempSubBlock;
  bool hasReadDATA = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cEDID:
           if (!editorID.empty())
           {
             std::cout << "Error: ACHR seems to have more than one EDID subrecord!\n";
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
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EDID of EXPL!\n";
             return false;
           }
           editorID = std::string(buffer);
           if (editorID.empty())
           {
             std::cout << "Error: subrecord EDID of ACHR is empty!\n";
             return false;
           }
           break;
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: ACHR seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //load VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of ACHR!\n";
             return false;
           }
           bytesRead += (2 + unknownVMAD.getSize());
           break;
      case cNAME:
           if (baseObjectFormID!=0)
           {
             std::cout << "Error: ACHR seems to have more than one NAME subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cNAME, baseObjectFormID, false))
           {
             std::cout << "Error while reading subrecord NAME of ACHR!\n";
             return false;
           }
           bytesRead += 6;
           if (baseObjectFormID==0)
           {
             std::cout << "Error: subrecord NAME of ACHR has value zero!\n";
             return false;
           }
           break;
      case cXRGD:
           if (unknownXRGD.isPresent())
           {
             std::cout << "Error: ACHR seems to have more than one XRGD subrecord!\n";
             return false;
           }
           //load XRGD
           if (!unknownXRGD.loadFromStream(in_File, cXRGD, false))
           {
             std::cout << "Error while reading subrecord XRGD of ACHR!\n";
             return false;
           }
           bytesRead += (2 + unknownXRGD.getSize());
           break;
      case cXRGB:
           if (unknownXRGB.isPresent())
           {
             std::cout << "Error: ACHR seems to have more than one XRGB subrecord!\n";
             return false;
           }
           //load XRGB
           if (!unknownXRGB.loadFromStream(in_File, cXRGB, false))
           {
             std::cout << "Error while reading subrecord XRGB of ACHR!\n";
             return false;
           }
           bytesRead += (2 + unknownXRGB.getSize());
           break;
      case cXLCM:
           if (hasXLCM)
           {
             std::cout << "Error: ACHR seems to have more than one XLCM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXLCM, unknownXLCM, false))
           {
             std::cout << "Error while reading subrecord XLCM of ACHR!\n";
             return false;
           }
           bytesRead += 6;
           hasXLCM = true;
           break;
      case cINAM:
           if (hasINAM)
           {
             std::cout << "Error: ACHR seems to have more than one INAM subrecord!\n";
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
             std::cout << "Error: ACHR seems to have more than one PDTO subrecord!\n";
             return false;
           }
           //PDTO's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record PDTO of ACHR has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read PDTO
           in_File.read((char*) &unknownPDTO, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord PDTO of ACHR!\n";
             return false;
           }
           hasPDTO = true;
           break;
      case cXLKR:
           //XLKR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if ((subLength!=8) and (subLength!=4))
           {
             std::cout <<"Error: subrecord XLKR of ACHR has invalid length ("
                       <<subLength <<" bytes). Should be eight or four bytes!\n";
             return false;
           }
           //read XLKR's stuff
           if (8==subLength)
           {
             in_File.read((char*) &tempLinkedRef.keywordFormID, 4);
           }
           else
           {
             tempLinkedRef.keywordFormID = 0;
           }
           in_File.read((char*) &tempLinkedRef.referenceFormID, 4);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XLKR of ACHR!\n";
             return false;
           }
           linkedReferences.push_back(tempLinkedRef);
           break;
      case cXAPD:
           //XAPD's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: subrecord XAPD of ACHR has invalid length ("
                       <<subLength <<" bytes). Should be one byte!\n";
             return false;
           }
           //read XAPD's stuff
           in_File.read((char*) &tempUint8, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XAPD of ACHR!\n";
             return false;
           }
           XAPDs.push_back(tempUint8);
           break;
      case cXAPR:
           //XAPR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: subrecord XAPR of ACHR has invalid length ("
                       <<subLength <<" bytes). Should be eight bytes!\n";
             return false;
           }
           //read XAPR's stuff
           in_File.read((char*) &tempUint64, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XAPR of ACHR!\n";
             return false;
           }
           XAPRs.push_back(tempUint64);
           break;
      case cXLCN:
           if (persistenceLocationFormID!=0)
           {
             std::cout << "Error: ACHR seems to have more than one XLCN subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXLCN, persistenceLocationFormID, false))
           {
             std::cout << "Error while reading subrecord XLCN of ACHR!\n";
             return false;
           }
           bytesRead += 6;
           if (persistenceLocationFormID==0)
           {
             std::cout << "Error: subrecord XLCN of ACHR has value zero!\n";
             return false;
           }
           break;
      case cXLRT:
           if (locationRefTypeFormID!=0)
           {
             std::cout << "Error: ACHR seems to have more than one XLRT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXLRT, locationRefTypeFormID, false))
           {
             std::cout << "Error while reading subrecord XLRT of ACHR!\n";
             return false;
           }
           bytesRead += 6;
           if (locationRefTypeFormID==0)
           {
             std::cout << "Error: subrecord XLRT of ACHR has value zero!\n";
             return false;
           }
           break;
      case cXEZN:
           if (encounterZoneFormID!=0)
           {
             std::cout << "Error: ACHR seems to have more than one XEZN subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXEZN, encounterZoneFormID, false))
           {
             std::cout << "Error while reading subrecord XEZN of ACHR!\n";
             return false;
           }
           bytesRead += 6;
           if (encounterZoneFormID==0)
           {
             std::cout << "Error: subrecord XEZN of ACHR has value zero!\n";
             return false;
           }
           break;
      case cXESP:
           if (subXESP.isPresent())
           {
             std::cout << "Error: ACHR seems to have more than one XESP subrecord!\n";
             return false;
           }
           //XESP's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: subrecord XESP of ACHR has invalid length ("
                       <<subLength <<" bytes). Should be eight bytes!\n";
             return false;
           }
           //read XESP's stuff
           in_File.read((char*) &subXESP.refID, 4);
           in_File.read((char*) &subXESP.flags, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XESP of ACHR!\n";
             return false;
           }
           if (!subXESP.isPresent())
           {
             std::cout << "Error: subrecord XESP of ACHR has value zero!\n";
             return false;
           }
           break;
      case cXOWN:
           if (ownerFormID!=0)
           {
             std::cout << "Error: ACHR seems to have more than one XOWN subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cXOWN, ownerFormID, false))
           {
             std::cout << "Error while reading subrecord XOWN of ACHR!\n";
             return false;
           }
           bytesRead += 6;
           if (ownerFormID==0)
           {
             std::cout << "Error: subrecord XOWN of ACHR has value zero!\n";
             return false;
           }
           break;
      case cXSCL:
           if (scale!=1.0f)
           {
             std::cout << "Error: ACHR seems to have more than one XSCL subrecord!\n";
             return false;
           }
           //XSCL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord XSCL of ACHR has invalid length ("
                       <<subLength <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read XSCL's stuff
           in_File.read((char*) &scale, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XSCL of ACHR!\n";
             return false;
           }
           if ((scale==1.0f) or (scale<=0.0f))
           {
             std::cout << "Error: subrecord XSCL of ACHR has invalid value "<<scale<<"!\n";
             return false;
           }
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: ACHR seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=24)
           {
             std::cout <<"Error: subrecord DATA of ACHR has invalid length ("
                       <<subLength <<" bytes). Should be 24 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownDATA, 24);
           bytesRead += 24;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of ACHR!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           tempSubBlock.subType = subRecName;
           if (!tempSubBlock.subData.loadFromStream(in_File, subRecName, false))
           {
             std::cout << "Error while loading subrecord "<<IntTo4Char(subRecName)<<" of ACHR into sub-blocks!\n";
             return false;
           }
           bytesRead = bytesRead +2 + tempSubBlock.subData.getSize();
           subBlocks.push_back(tempSubBlock);
           /*
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only EDID, VMAD, NAME, XRGD, XRGB, XLCM,"
                     << " INAM, PDTO, XLKR, XAPD, XAPR, XLCN, XLRT, XEZN, XESP,"
                     << " XOWN, XSCL or DATA are allowed here!\n";
           return false;*/
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadDATA and (baseObjectFormID!=0)))
  {
    std::cout << "Error: at least one required subrecord of ACHR is missing!\n";
    return false;
  }

  return in_File.good();
  #warning Not completely implemented yet!
}

uint32_t CharacterReferenceRecord::getRecordType() const
{
  return cACHR;
}

} //namespace
