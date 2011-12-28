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

#include "ReferenceRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ReferenceRecord::ReferenceRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  unknownNAME = 0;
  hasXLKR = false;
  unknownXLKR = 0;
  hasXESP = false;
  unknownXESP = 0;
  hasXEMI = false;
  unknownXEMI = 0;
  unknownXLOC.setPresence(false);
  unknownXSCL = 1.0f;
  hasXPRD = false;
  unknownXPRD = 0;
  hasINAM = false;
  unknownINAM = 0;
  hasPDTO = false;
  unknownPDTO = 0;
  memset(unknownDATA, 0, 24);
}

ReferenceRecord::~ReferenceRecord()
{
  //empty
}

bool ReferenceRecord::equals(const ReferenceRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (unknownNAME==other.unknownNAME) and (hasXEMI==other.hasXEMI)
      and (hasXLKR==other.hasXLKR) and ((unknownXLKR==other.unknownXLKR) or (!hasXLKR))
      and (hasXESP==other.hasXESP) and ((unknownXESP==other.unknownXESP) or (!hasXESP))
      and ((unknownXEMI==other.unknownXEMI) or (!hasXEMI))
      and (unknownXLOC==other.unknownXLOC) and (unknownXSCL==other.unknownXSCL)
      and (hasXPRD==other.hasXPRD) and ((unknownXPRD==other.unknownXPRD) or (!hasXPRD))
      and (hasINAM==other.hasINAM) and ((unknownINAM==other.unknownINAM) or (!hasINAM))
      and (hasPDTO==other.hasPDTO) and ((unknownPDTO==other.unknownPDTO) or (!hasPDTO))
      and (memcmp(unknownDATA, other.unknownDATA, 24)==0));
}

uint32_t ReferenceRecord::getWriteSize() const
{
  #warning Not implemented yet!
  return 0;
}

bool ReferenceRecord::saveToStream(std::ofstream& output) const
{
  #warning Not implemented yet!
  return false;
}

bool ReferenceRecord::loadFromStream(std::ifstream& in_File)
{
  #warning Not completely implemented yet!
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  char buffer[512];

  editorID.clear();
  unknownVMAD.setPresence(false);
  bool hasReadNAME = false;
  hasXLKR = false;
  hasXESP = false;
  hasXEMI = false;
  unknownXLOC.setPresence(false);
  unknownXSCL = 1.0f;
  hasXPRD = false;
  hasINAM = false;
  hasPDTO = false;
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
             std::cout << "Error: REFR seems to have more than one EDID subrecord!\n";
             return false;
           }
           //EDID's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record EDID of REFR is longer than 511 characters!\n";
             return false;
           }
           //read EDID's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EDID of REFR!\n";
             return false;
           }
           editorID = std::string(buffer);
           break;
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: REFR seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of REFR!\n!";
             return false;
           }
           bytesRead = bytesRead +2 +unknownVMAD.getSize();
           break;
      case cNAME:
           if (hasReadNAME)
           {
             std::cout << "Error: REFR seems to have more than one NAME subrecord!\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read NAME
           if (!loadUint32SubRecordFromStream(in_File, cNAME, unknownNAME)) return false;
           bytesRead += 6;
           hasReadNAME = true;
           break;
      case cXLKR:
           if (hasXLKR)
           {
             std::cout << "Error: REFR seems to have more than one XLKR subrecord!\n";
             return false;
           }
           //XLKR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record XLKR of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read XLKR
           in_File.read((char*) &unknownXLKR, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XLKR of REFR!\n";
             return false;
           }
           hasXLKR = true;
           break;
      case cXESP:
           if (hasXESP)
           {
             std::cout << "Error: REFR seems to have more than one XESP subrecord!\n";
             return false;
           }
           //XESP's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record XESP of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read XESP
           in_File.read((char*) &unknownXESP, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XESP of REFR!\n";
             return false;
           }
           hasXESP = true;
           break;
      case cXEMI:
           if (hasXEMI)
           {
             std::cout << "Error: REFR seems to have more than one XEMI subrecord!\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read XEMI
           if (!loadUint32SubRecordFromStream(in_File, cXEMI, unknownXEMI)) return false;
           bytesRead += 6;
           hasXEMI = true;
           break;
      case cXLOC:
           if (unknownXLOC.isPresent())
           {
             std::cout << "Error: REFR seems to have more than one XLOC subrecord!\n";
             return false;
           }
           //read XLOC
           if (!unknownXLOC.loadFromStream(in_File, cXLOC, false))
           {
             std::cout << "Error while reading subrecord XLOC of REFR!\n!";
             return false;
           }
           bytesRead = bytesRead +2 +unknownXLOC.getSize();
           break;
      case cXSCL:
           if (unknownXSCL!=1.0f)
           {
             std::cout << "Error: REFR seems to have more than one XSCL subrecord!\n";
             return false;
           }
           //XSCL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record XSCL of REFR has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read XSCL
           in_File.read((char*) &unknownXSCL, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XSCL of REFR!\n";
             return false;
           }
           break;
      case cXPRD:
           if (hasXPRD)
           {
             std::cout << "Error: REFR seems to have more than one XPRD subrecord!\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read XPRD
           if (!loadUint32SubRecordFromStream(in_File, cXPRD, unknownXPRD)) return false;
           bytesRead += 6;
           hasXPRD = true;
           break;
      case cINAM:
           if (hasINAM)
           {
             std::cout << "Error: REFR seems to have more than one INAM subrecord!\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, unknownINAM)) return false;
           bytesRead += 6;
           hasINAM = true;
           break;
      case cPDTO:
           if (hasPDTO)
           {
             std::cout << "Error: REFR seems to have more than one PDTO subrecord!\n";
             return false;
           }
           //PDTO's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record PDTO of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read PDTO
           in_File.read((char*) &unknownPDTO, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord PDTO of REFR!\n";
             return false;
           }
           hasPDTO = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: REFR seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=24)
           {
             std::cout <<"Error: sub record DATA of REFR has invalid length ("<<subLength
                       <<" bytes). Should be 24 bytes.\n";
             return false;
           }
           //read DATA
           in_File.read((char*) unknownDATA, 24);
           bytesRead += 24;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of REFR!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only EDID, VMAD, NAME, XLKR, XESP, XEMI, XLOC,"
                     << " XSCL, XPRD, INAM, PDTO or DATA are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadNAME and hasReadDATA))
  {
    std::cout << "Error: subrecord NAME or DATA of REFR is missing!\n";
    return false;
  }

  #warning Not completely implemented yet!
  return in_File.good();
}

int32_t ReferenceRecord::getRecordType() const
{
  return cREFR;
}

} //namespace
