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

#include "WeaponRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <cstring>

namespace SRTP
{

WeaponRecord::WeaponRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  unknownFULL = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  hasEITM = false;
  unknownEITM = 0;
  hasEAMT = false;
  unknownEAMT = 0;
  unknownETYP = 0;
  hasBIDS = false;
  unknownBIDS = 0;
  hasBAMT = false;
  unknownBAMT = 0;
  keywordSize = 0;
  keywordArray.clear();
  unknownDESC = 0;
  hasINAM = false;
  unknownINAM = 0;
  unknownWNAM = 0;
  unknownTNAM = 0;
  unknownNAM9 = 0;
  unknownNAM8 = 0;
  memset(unknownDATA, 0, 10);
  memset(unknownDNAM, 0, 100);
  memset(unknownCRDT, 0, 16);
  unknownVNAM = 0;
  hasCNAM = false;
  unknownCNAM = 0;
}

WeaponRecord::WeaponRecord(const WeaponRecord& other)
{
  copyBasicMembers(other);
  editorID = other.editorID;
  unknownVMAD = other.unknownVMAD;
  memcpy(unknownOBND, other.unknownOBND, 12);
  unknownFULL = other.unknownFULL;
  modelPath = other.modelPath;
  unknownMODT = other.unknownMODT;
  hasEITM = other.hasEITM;
  unknownEITM = other.unknownEITM;
  hasEAMT = other.hasEAMT;
  unknownEAMT = other.unknownEAMT;
  unknownETYP = other.unknownETYP;
  hasBIDS = other.hasBIDS;
  unknownBIDS = other.unknownBIDS;
  hasBAMT = other.hasBAMT;
  unknownBAMT = other.unknownBAMT;
  keywordSize = other.keywordSize;
  keywordArray = other.keywordArray;
  unknownDESC = other.unknownDESC;
  hasINAM = other.hasINAM;
  unknownINAM = other.unknownINAM;
  unknownWNAM = other.unknownWNAM;
  unknownTNAM = other.unknownTNAM;
  unknownNAM9 = other.unknownNAM9;
  unknownNAM8 = other.unknownNAM8;
  memcpy(unknownDATA, other.unknownDATA, 10);
  memcpy(unknownDNAM, other.unknownDNAM, 100);
  memcpy(unknownCRDT, other.unknownCRDT, 16);
  unknownVNAM = other.unknownVNAM;
  hasCNAM = other.hasCNAM;
  unknownCNAM = other.unknownCNAM;
}

WeaponRecord& WeaponRecord::operator=(const WeaponRecord& other)
{
  if (this == &other) return *this;

  copyBasicMembers(other);
  editorID = other.editorID;
  unknownVMAD = other.unknownVMAD;
  memcpy(unknownOBND, other.unknownOBND, 12);
  unknownFULL = other.unknownFULL;
  modelPath = other.modelPath;
  unknownMODT = other.unknownMODT;
  hasEITM = other.hasEITM;
  unknownEITM = other.unknownEITM;
  hasEAMT = other.hasEAMT;
  unknownEAMT = other.unknownEAMT;
  unknownETYP = other.unknownETYP;
  hasBIDS = other.hasBIDS;
  unknownBIDS = other.unknownBIDS;
  hasBAMT = other.hasBAMT;
  unknownBAMT = other.unknownBAMT;
  keywordSize = other.keywordSize;
  keywordArray = other.keywordArray;
  unknownDESC = other.unknownDESC;
  hasINAM = other.hasINAM;
  unknownINAM = other.unknownINAM;
  unknownWNAM = other.unknownWNAM;
  unknownTNAM = other.unknownTNAM;
  unknownNAM9 = other.unknownNAM9;
  unknownNAM8 = other.unknownNAM8;
  memcpy(unknownDATA, other.unknownDATA, 10);
  memcpy(unknownDNAM, other.unknownDNAM, 100);
  memcpy(unknownCRDT, other.unknownCRDT, 16);
  unknownVNAM = other.unknownVNAM;
  hasCNAM = other.hasCNAM;
  unknownCNAM = other.unknownCNAM;
  return *this;
}

WeaponRecord::~WeaponRecord()
{
  //empty
}

int32_t WeaponRecord::getRecordType() const
{
  return cWEAP;
}

bool WeaponRecord::equals(const WeaponRecord& other) const
{
  if ((editorID!=other.editorID) or (unknownVMAD!=other.unknownVMAD)
      or (memcmp(unknownOBND, other.unknownOBND, 12)!=0)
      or (unknownFULL!=other.unknownFULL) or (modelPath!=other.modelPath)
      or (unknownMODT!=other.unknownMODT) or (!equalsBasic(other)))
  {
    return false;
  }
  if (((unknownEITM!=other.unknownEITM) and hasEITM)
    or ((unknownEAMT!=other.unknownEAMT) and hasEAMT)
    or (unknownETYP!=other.unknownETYP) or (hasBIDS!=other.hasBIDS)
    or ((unknownBIDS!=other.unknownBIDS) and hasBIDS)
    or ((hasBAMT!=other.hasBAMT) and hasBAMT) or (unknownBAMT!=other.unknownBAMT)
    or (keywordSize!=other.keywordSize))
  {
    return false;
  }
  if ((keywordArray!=other.keywordArray) or (unknownDESC!=other.unknownDESC)
    or (hasINAM!=other.hasINAM) or ((unknownINAM!=other.unknownINAM) and hasINAM)
    or (unknownWNAM!=other.unknownWNAM) or (unknownTNAM!=other.unknownTNAM)
    or (unknownNAM9!=other.unknownNAM9) or (unknownNAM8!=other.unknownNAM8)
    or (hasCNAM!=other.hasCNAM) or ((unknownCNAM!=other.unknownCNAM) and hasCNAM))
  {
    return false;
  }
  return ((memcmp(unknownDATA, other.unknownDATA, 10)==0)
      and (memcmp(unknownDNAM, other.unknownDATA, 100)==0)
      and (memcmp(unknownCRDT, other.unknownCRDT, 16)==0)
      and (unknownVNAM==other.unknownVNAM));
}

bool WeaponRecord::saveToStream(std::ofstream& output) const
{
  #warning Not implemented yet!
  std::cout << "Error: call to unimplemented function WeaponRecord::saveToStream!\n";
  return false;
}

bool WeaponRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of WEAP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of WEAP!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  modelPath.clear();
  bool hasReadETYP = false;
  hasBIDS = false;
  hasBAMT = false;
  keywordSize = 0;
  keywordArray.clear();
  uint32_t i, fid;
  bool hasReadDESC = false;
  hasINAM = false;
  bool hasReadWNAM = false;
  hasCNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: WEAP seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead +2+unknownVMAD.getSize();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: WEAP seems to have more than one OBND subrecord!\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record OBND of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read OBND
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord OBND of WEAP!\n";
             return false;
           }
           hasReadOBND = true;

           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, unknownFULL)) return false;
           bytesRead += 10;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: WEAP seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of WEAP is longer than 511 characters!\n";
             return false;
           }
           //read MODL path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of WEAP!\n";
             return false;
           }
           modelPath = std::string(buffer);

           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, true))
           {
             std::cout << "Error while reading subrecord MODT of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead + 4+2 +unknownMODT.getSize();
           /*
           //check MODT's length
           if ((unknownMODT.getSize()!=12) and (unknownMODT.getSize()!=24) and (unknownMODT.getSize()!=36))
           {
             std::cout <<"Error: sub record MODT of WEAP has invalid length ("<<unknownMODT.getSize()
                       <<" bytes). Should be 12 or 24 or 36 bytes.\n";
             return false;
           }*/
           break;
      case cEITM:
           if (hasEITM)
           {
             std::cout << "Error: WEAP seems to have more than one EITM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read EITM
           if (!loadUint32SubRecordFromStream(in_File, cEITM, unknownEITM)) return false;
           bytesRead += 6;
           hasEITM = true;
           break;
      case cEAMT:
           if (hasEAMT)
           {
             std::cout << "Error: WEAP seems to have more than one EAMT subrecord!\n";
             return false;
           }
           //EAMT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record EAMT of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 2 bytes.\n";
             return false;
           }
           //read EAMT
           in_File.read((char*) &unknownEAMT, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EAMT of WEAP!\n";
             return false;
           }
           hasEAMT = true;
           break;
      case cETYP:
           if (hasReadETYP)
           {
             std::cout << "Error: WEAP seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, unknownETYP)) return false;
           bytesRead += 6;
           hasReadETYP = true;
           break;
      case cBIDS:
           if (hasBIDS)
           {
             std::cout << "Error: WEAP seems to have more than one BIDS subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read BIDS
           if (!loadUint32SubRecordFromStream(in_File, cBIDS, unknownBIDS)) return false;
           bytesRead += 6;
           hasBIDS = true;
           break;
      case cBAMT:
           if (hasBAMT)
           {
             std::cout << "Error: WEAP seems to have more than one BAMT subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read BAMT
           if (!loadUint32SubRecordFromStream(in_File, cBAMT, unknownBAMT)) return false;
           bytesRead += 6;
           hasBAMT = true;
           break;
      case cKSIZ:
           if (keywordSize!=0)
           {
             std::cout << "Error: WEAP seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read KSIZ
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, keywordSize)) return false;
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
           if (subLength!=4*keywordSize)
           {
             std::cout <<"Error: sub record KWDA of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be "<<4*keywordSize<<" bytes.\n";
             return false;
           }
           //read KWDA
           keywordArray.clear();
           for (i=0; i<keywordSize; ++i)
           {
             fid = 0;
             in_File.read((char*) &fid, 4);
             bytesRead += 4;
             keywordArray.push_back(subRecName);
           }
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KWDA of WEAP!\n";
             return false;
           }
           break;
      case cDESC:
           if (hasReadDESC)
           {
             std::cout << "Error: WEAP seems to have more than one DESC subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read DESC
           if (!loadUint32SubRecordFromStream(in_File, cDESC, unknownDESC)) return false;
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cINAM:
           if (hasINAM)
           {
             std::cout << "Error: WEAP seems to have more than one INAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read INAM
           if (!loadUint32SubRecordFromStream(in_File, cINAM, unknownINAM)) return false;
           hasINAM = true;
           bytesRead += 6;
           break;
      case cWNAM:
           if (hasReadWNAM)
           {
             std::cout << "Error: WEAP seems to have more than one WNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read WNAM
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, unknownWNAM)) return false;
           hasReadWNAM = true;
           bytesRead += 6;

           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM)) return false;
           bytesRead += 10;

           //read NAM9
           if (!loadUint32SubRecordFromStream(in_File, cNAM9, unknownNAM9)) return false;
           bytesRead += 10;

           //read NAM8
           if (!loadUint32SubRecordFromStream(in_File, cNAM8, unknownNAM8)) return false;
           bytesRead += 10;

           //read DATA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cDATA)
           {
             UnexpectedRecord(cDATA, subRecName);
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=10)
           {
             std::cout <<"Error: sub record DATA of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 10 bytes.\n";
             return false;
           }
           //read DATA's content
           in_File.read((char*) unknownDATA, 10);
           bytesRead += 10;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of WEAP!\n";
             return false;
           }

           //read DNAM
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cDNAM)
           {
             UnexpectedRecord(cDNAM, subRecName);
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=100)
           {
             std::cout <<"Error: sub record DNAM of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 100 bytes.\n";
             return false;
           }
           //read DNAM's content
           in_File.read((char*) unknownDNAM, 100);
           bytesRead += 100;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of WEAP!\n";
             return false;
           }

           //read CRDT
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cCRDT)
           {
             UnexpectedRecord(cCRDT, subRecName);
             return false;
           }
           //CRDT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=16)
           {
             std::cout <<"Error: sub record CRDT of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be 16 bytes.\n";
             return false;
           }
           //read CRDT's content
           in_File.read((char*) unknownCRDT, 16);
           bytesRead += 16;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CRDT of WEAP!\n";
             return false;
           }

           //read VNAM
           if (!loadUint32SubRecordFromStream(in_File, cVNAM, unknownVNAM)) return false;
           bytesRead += 10;

           break;
      case cCNAM:
           if (hasCNAM)
           {
             std::cout << "Error: WEAP seems to have more than one CNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM)) return false;
           hasCNAM = true;
           bytesRead += 6;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only KSIZ, DESC, INAM, WNAM or CNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  if (!(hasReadOBND and hasReadWNAM and hasReadETYP and hasReadDESC))
  {
    std::cout << "Error: WEAP's OBND or WNAM or ETYP or DESC subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

} //namespace
