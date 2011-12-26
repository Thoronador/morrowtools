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
  hasFULL = false;
  nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  hasEITM = false;
  unknownEITM = 0;
  hasEAMT = false;
  unknownEAMT = 0;
  hasETYP = false;
  unknownETYP = 0;
  hasBIDS = false;
  unknownBIDS = 0;
  hasBAMT = false;
  unknownBAMT = 0;
  keywordArray.clear();
  descriptionStringID = 0;
  unknownNNAM = "";
  hasINAM = false;
  unknownINAM = 0;
  hasWNAM = false;
  unknownWNAM = 0;
  hasTNAM = false;
  unknownTNAM = 0;
  hasUNAM = false;
  unknownUNAM = 0;
  hasNAM9 = false;
  unknownNAM9 = 0;
  hasNAM8 = false;
  unknownNAM8 = 0;
  hasSNAM = false;
  unknownSNAM = 0;
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
  hasFULL = other.hasFULL;
  nameStringID = other.nameStringID;
  modelPath = other.modelPath;
  unknownMODT = other.unknownMODT;
  unknownMODS = other.unknownMODS;
  hasEITM = other.hasEITM;
  unknownEITM = other.unknownEITM;
  hasEAMT = other.hasEAMT;
  unknownEAMT = other.unknownEAMT;
  hasETYP = other.hasETYP;
  unknownETYP = other.unknownETYP;
  hasBIDS = other.hasBIDS;
  unknownBIDS = other.unknownBIDS;
  hasBAMT = other.hasBAMT;
  unknownBAMT = other.unknownBAMT;
  keywordArray = other.keywordArray;
  descriptionStringID = other.descriptionStringID;
  unknownNNAM = other.unknownNNAM;
  hasINAM = other.hasINAM;
  unknownINAM = other.unknownINAM;
  hasWNAM = other.hasWNAM;
  unknownWNAM = other.unknownWNAM;
  hasTNAM = other.hasTNAM;
  unknownTNAM = other.unknownTNAM;
  hasUNAM = other.hasUNAM;
  unknownUNAM = other.unknownUNAM;
  hasNAM9 = other.hasNAM9;
  unknownNAM9 = other.unknownNAM9;
  hasNAM8 = other.hasNAM8;
  unknownNAM8 = other.unknownNAM8;
  hasSNAM = other.hasSNAM;
  unknownSNAM = other.unknownSNAM;
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
  hasFULL = other.hasFULL;
  nameStringID = other.nameStringID;
  modelPath = other.modelPath;
  unknownMODT = other.unknownMODT;
  unknownMODS = other.unknownMODS;
  hasEITM = other.hasEITM;
  unknownEITM = other.unknownEITM;
  hasEAMT = other.hasEAMT;
  unknownEAMT = other.unknownEAMT;
  hasETYP = other.hasETYP;
  unknownETYP = other.unknownETYP;
  hasBIDS = other.hasBIDS;
  unknownBIDS = other.unknownBIDS;
  hasBAMT = other.hasBAMT;
  unknownBAMT = other.unknownBAMT;
  keywordArray = other.keywordArray;
  descriptionStringID = other.descriptionStringID;
  unknownNNAM = other.unknownNNAM;
  hasINAM = other.hasINAM;
  unknownINAM = other.unknownINAM;
  hasWNAM = other.hasWNAM;
  unknownWNAM = other.unknownWNAM;
  hasTNAM = other.hasTNAM;
  unknownTNAM = other.unknownTNAM;
  hasUNAM = other.hasUNAM;
  unknownUNAM = other.unknownUNAM;
  hasNAM9 = other.hasNAM9;
  unknownNAM9 = other.unknownNAM9;
  hasNAM8 = other.hasNAM8;
  unknownNAM8 = other.unknownNAM8;
  hasSNAM = other.hasSNAM;
  unknownSNAM = other.unknownSNAM;
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
      or (hasFULL!=other.hasFULL) or ((nameStringID!=other.nameStringID) and hasFULL)
      or (modelPath!=other.modelPath) or (unknownMODT!=other.unknownMODT)
      or (unknownMODS!=other.unknownMODS)or (!equalsBasic(other)))
  {
    return false;
  }
  if ((hasEITM!=other.hasEITM) or ((unknownEITM!=other.unknownEITM) and hasEITM)
    or (hasEAMT!=other.hasEAMT) or ((unknownEAMT!=other.unknownEAMT) and hasEAMT)
    or (hasETYP!=other.hasETYP) or ((unknownETYP!=other.unknownETYP) and hasETYP)
    or (hasBIDS!=other.hasBIDS) or ((unknownBIDS!=other.unknownBIDS) and hasBIDS)
    or ((hasBAMT!=other.hasBAMT) and hasBAMT) or (unknownBAMT!=other.unknownBAMT))
  {
    return false;
  }
  if ((keywordArray!=other.keywordArray) or (descriptionStringID!=other.descriptionStringID)
    or (unknownNNAM!=other.unknownNNAM)
    or (hasINAM!=other.hasINAM) or ((unknownINAM!=other.unknownINAM) and hasINAM)
    or (hasWNAM!=other.hasWNAM) or ((unknownWNAM!=other.unknownWNAM) and hasWNAM)
    or (hasTNAM!=other.hasTNAM) or ((unknownTNAM!=other.unknownTNAM) and hasTNAM)
    or (hasUNAM!=other.hasUNAM) or ((unknownUNAM!=other.unknownUNAM) and hasUNAM)
    or (hasNAM9!=other.hasNAM9) or ((unknownNAM9!=other.unknownNAM9) and hasNAM9)
    or (hasNAM8!=other.hasNAM8) or ((unknownNAM8!=other.unknownNAM8) and hasNAM8)
    or (hasCNAM!=other.hasCNAM) or ((unknownCNAM!=other.unknownCNAM) and hasCNAM)
    or (hasSNAM!=other.hasSNAM) or ((unknownSNAM!=other.unknownSNAM) and hasSNAM))
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
  hasFULL = false;
  nameStringID = 0;
  modelPath.clear();
  unknownMODS.setPresence(false);
  hasETYP = false;
  hasBIDS = false;
  hasBAMT = false;
  keywordArray.clear();
  uint32_t k_Size, i, fid;
  bool hasReadDESC = false;
  unknownNNAM.clear();
  hasSNAM = false;
  bool hasReadDATA = false;
  hasINAM = false;
  hasWNAM = false;
  hasTNAM = false;
  hasUNAM = false;
  hasNAM9 = false;
  hasNAM8 = false;
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
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: WEAP seems to have more than one FULL subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID)) return false;
           bytesRead += 6;
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
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: WEAP seems to have more than one MODS subrecord!\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cout << "Error while reading subrecord MODS of WEAP!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODS.getSize();
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
           if (hasETYP)
           {
             std::cout << "Error: WEAP seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read ETYP
           if (!loadUint32SubRecordFromStream(in_File, cETYP, unknownETYP)) return false;
           bytesRead += 6;
           hasETYP = true;
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
           if (!keywordArray.empty())
           {
             std::cout << "Error: WEAP seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read KSIZ
           k_Size = 0;
           if (!loadUint32SubRecordFromStream(in_File, cKSIZ, k_Size)) return false;
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
             std::cout <<"Error: sub record KWDA of WEAP has invalid length ("<<subLength
                       <<" bytes). Should be "<<4*k_Size<<" bytes.\n";
             return false;
           }
           //read KWDA
           keywordArray.clear();
           for (i=0; i<k_Size; ++i)
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
           if (!loadUint32SubRecordFromStream(in_File, cDESC, descriptionStringID)) return false;
           bytesRead += 6;
           hasReadDESC = true;
           break;
      case cNNAM:
           if (!unknownNNAM.empty())
           {
             std::cout << "Error: WEAP seems to have more than one NNAM subrecord!\n";
             return false;
           }
           //NNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record NNAM of WEAP is longer than 511 characters!\n";
             return false;
           }
           //read NNAM (node name?)
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NNAM of WEAP!\n";
             return false;
           }
           unknownNNAM = std::string(buffer);
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
           if (hasWNAM)
           {
             std::cout << "Error: WEAP seems to have more than one WNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read WNAM
           if (!loadUint32SubRecordFromStream(in_File, cWNAM, unknownWNAM)) return false;
           hasWNAM = true;
           bytesRead += 6;
           break;
      case cTNAM:
           if (hasTNAM)
           {
             std::cout << "Error: WEAP seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read TNAM
           if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM)) return false;
           bytesRead += 6;
           hasTNAM = true;
           break;
      case cUNAM:
           if (hasUNAM)
           {
             std::cout << "Error: WEAP seems to have more than one UNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read UNAM
           if (!loadUint32SubRecordFromStream(in_File, cUNAM, unknownUNAM)) return false;
           bytesRead += 6;
           hasUNAM = true;
           break;
      case cNAM9:
           if (hasNAM9)
           {
             std::cout << "Error: WEAP seems to have more than one NAM9 subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read NAM9
           if (!loadUint32SubRecordFromStream(in_File, cNAM9, unknownNAM9)) return false;
           bytesRead += 6;
           hasNAM9 = true;
           break;
      case cNAM8:
           if (hasNAM8)
           {
             std::cout << "Error: WEAP seems to have more than one NAM8 subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read NAM8
           if (!loadUint32SubRecordFromStream(in_File, cNAM8, unknownNAM8)) return false;
           bytesRead += 6;
           hasNAM8 = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: WEAP seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //skip back
           in_File.seekg(-4, std::ios_base::cur);
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM)) return false;
           hasSNAM = true;
           bytesRead += 6;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: WEAP seems to have more than one DATA subrecord!\n";
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
           hasReadDATA = true;

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

  if (!(hasReadOBND and hasReadDESC and hasReadDATA))
  {
    std::cout << "Error: WEAP's OBND or DESC or DATA subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

} //namespace
