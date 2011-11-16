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
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  unknownFULL = 0;
  modelPath = "";
  m_sizeOfMODT = 0;
  m_pointerToMODT = NULL;
  unknownEITM = 0;
  unknownEAMT = 0;
  unknownETYP = 0;
  unknownBIDS = 0;
  unknownBAMT = 0;
  unknownKSIZ = 0;
  memset(unknownKWDA, 0, 12);
  unknownDESC = 0;
  unknownINAM = 0;
  unknownWNAM = 0;
  unknownTNAM = 0;
  unknownNAM9 = 0;
  unknownNAM8 = 0;
  memset(unknownDATA, 0, 10);
  memset(unknownDNAM, 0, 100);
  memset(unknownCRDT, 0, 16);
  unknownVNAM = 0;
}

WeaponRecord::WeaponRecord(const WeaponRecord& other)
{
  editorID = other.editorID;
  memcpy(unknownOBND, other.unknownOBND, 12);
  unknownFULL = other.unknownFULL;
  modelPath = other.modelPath;
  if (other.m_pointerToMODT != NULL)
  {
    m_sizeOfMODT = other.m_sizeOfMODT;
    m_pointerToMODT = new uint8_t[m_sizeOfMODT];
    memcpy(m_pointerToMODT, other.m_pointerToMODT, m_sizeOfMODT);
  }
  else
  {
    m_sizeOfMODT = 0;
    m_pointerToMODT = NULL;
  }
  unknownEITM = other.unknownEITM;
  unknownEAMT = other.unknownEAMT;
  unknownETYP = other.unknownETYP;
  unknownBIDS = other.unknownBIDS;
  unknownBAMT = other.unknownBAMT;
  unknownKSIZ = other.unknownKSIZ;
  memcpy(unknownKWDA, other.unknownKWDA, 12);
  unknownDESC = other.unknownDESC;
  unknownINAM = other.unknownINAM;
  unknownWNAM = other.unknownWNAM;
  unknownTNAM = other.unknownTNAM;
  unknownNAM9 = other.unknownNAM9;
  unknownNAM8 = other.unknownNAM8;
  memcpy(unknownDATA, other.unknownDATA, 10);
  memcpy(unknownDNAM, other.unknownDNAM, 100);
  memcpy(unknownCRDT, other.unknownCRDT, 16);
  unknownVNAM = other.unknownVNAM;
}

WeaponRecord& WeaponRecord::operator=(const WeaponRecord& other)
{
  if (this == &other) return *this;

  editorID = other.editorID;
  memcpy(unknownOBND, other.unknownOBND, 12);
  unknownFULL = other.unknownFULL;
  modelPath = other.modelPath;
  delete m_pointerToMODT;
  if (other.m_pointerToMODT != NULL)
  {
    m_sizeOfMODT = other.m_sizeOfMODT;
    m_pointerToMODT = new uint8_t[m_sizeOfMODT];
    memcpy(m_pointerToMODT, other.m_pointerToMODT, m_sizeOfMODT);
  }
  else
  {
    m_sizeOfMODT = 0;
    m_pointerToMODT = NULL;
  }
  unknownEITM = other.unknownEITM;
  unknownEAMT = other.unknownEAMT;
  unknownETYP = other.unknownETYP;
  unknownBIDS = other.unknownBIDS;
  unknownBAMT = other.unknownBAMT;
  unknownKSIZ = other.unknownKSIZ;
  memcpy(unknownKWDA, other.unknownKWDA, 12);
  unknownDESC = other.unknownDESC;
  unknownINAM = other.unknownINAM;
  unknownWNAM = other.unknownWNAM;
  unknownTNAM = other.unknownTNAM;
  unknownNAM9 = other.unknownNAM9;
  unknownNAM8 = other.unknownNAM8;
  memcpy(unknownDATA, other.unknownDATA, 10);
  memcpy(unknownDNAM, other.unknownDNAM, 100);
  memcpy(unknownCRDT, other.unknownCRDT, 16);
  unknownVNAM = other.unknownVNAM;
  return *this;
}

WeaponRecord::~WeaponRecord()
{
  delete[] m_pointerToMODT;
  m_pointerToMODT = NULL;
  m_sizeOfMODT = 0;
}

bool WeaponRecord::equals(const WeaponRecord& other) const
{
  if ((editorID!=other.editorID) or (memcmp(unknownOBND, other.unknownOBND, 12)!=0)
      or (unknownFULL!=other.unknownFULL) or (modelPath!=other.modelPath)
      or (m_sizeOfMODT!=other.m_sizeOfMODT))
  {
    return false;
  }
  if (memcmp(m_pointerToMODT, other.getMODTPointer(), m_sizeOfMODT)!=0)
  {
    return false;
  }
  if ((unknownEITM!=other.unknownEITM) or (unknownEAMT!=other.unknownEAMT)
    or (unknownETYP!=other.unknownETYP) or (unknownBIDS!=other.unknownBIDS)
    or (unknownBAMT!=other.unknownBAMT) or (unknownKSIZ!=other.unknownKSIZ))
  {
    return false;
  }
  if ((memcmp(unknownKWDA, other.unknownKWDA, 12)!=0) or (unknownDESC!=other.unknownDESC)
    or (unknownINAM!=other.unknownINAM) or (unknownWNAM!=other.unknownWNAM)
    or (unknownTNAM!=other.unknownTNAM) or (unknownNAM9!=other.unknownNAM9)
    or (unknownNAM8!=other.unknownNAM8))
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
  return false;
}

bool WeaponRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of WEAP is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of WEAP!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=12)
  {
    std::cout <<"Error: sub record OBND of WEAP has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND
  in_File.read((char*) unknownOBND, 12);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of WEAP!\n";
    return false;
  }

  //read FULL
  if (!loadUint32SubRecordFromStream(in_File, cFULL, unknownFULL)) return false;

  //read MODL
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, subRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout <<"Error: sub record MODL of WEAP is longer than 511 characters!\n";
    return false;
  }
  //read MODL path
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of WEAP!\n";
    return false;
  }
  modelPath = std::string(buffer);

  //read MODT
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cMODT)
  {
    UnexpectedRecord(cMODT, subRecName);
    return false;
  }
  //MODT's length
  in_File.read((char*) &subLength, 2);
  if ((subLength!=12) and (subLength!=24))
  {
    std::cout <<"Error: sub record MODT of WEAP has invalid length ("<<subLength
              <<" bytes). Should be 12 or 24 bytes.\n";
    return false;
  }
  //read MODL path
  delete [] m_pointerToMODT;
  m_pointerToMODT = new uint8_t[subLength];
  m_sizeOfMODT = subLength;
  memset(m_pointerToMODT, 0, subLength);
  in_File.read((char*) m_pointerToMODT, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODT of WEAP!\n";
    return false;
  }

  //read EITM
  if (!loadUint32SubRecordFromStream(in_File, cEITM, unknownEITM)) return false;

  //read EAMT
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cEAMT)
  {
    UnexpectedRecord(cEAMT, subRecName);
    return false;
  }
  //EAMT's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=2)
  {
    std::cout <<"Error: sub record EAMT of WEAP has invalid length ("<<subLength
              <<" bytes). Should be 2 bytes.\n";
    return false;
  }
  //read EAMT
  in_File.read((char*) &unknownEAMT, 2);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EAMT of WEAP!\n";
    return false;
  }

  //read ETYP
  if (!loadUint32SubRecordFromStream(in_File, cETYP, unknownETYP)) return false;

  //read BIDS
  if (!loadUint32SubRecordFromStream(in_File, cBIDS, unknownBIDS)) return false;

  //read BAMT
  if (!loadUint32SubRecordFromStream(in_File, cBAMT, unknownBAMT)) return false;

  //read KSIZ
  if (!loadUint32SubRecordFromStream(in_File, cKSIZ, unknownKSIZ)) return false;

  //read KWDA
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cKWDA)
  {
    UnexpectedRecord(cKWDA, subRecName);
    return false;
  }
  //KWDA's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=12)
  {
    std::cout <<"Error: sub record KWDA of WEAP has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read KWDA
  in_File.read((char*) unknownKWDA, 12);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord KWDA of WEAP!\n";
    return false;
  }

  //read DESC
  if (!loadUint32SubRecordFromStream(in_File, cDESC, unknownDESC)) return false;

  //read INAM
  if (!loadUint32SubRecordFromStream(in_File, cINAM, unknownINAM)) return false;

  //read WNAM
  if (!loadUint32SubRecordFromStream(in_File, cWNAM, unknownWNAM)) return false;

  //read TNAM
  if (!loadUint32SubRecordFromStream(in_File, cTNAM, unknownTNAM)) return false;

  //read NAM9
  if (!loadUint32SubRecordFromStream(in_File, cNAM9, unknownNAM9)) return false;

  //read NAM8
  if (!loadUint32SubRecordFromStream(in_File, cNAM9, unknownNAM8)) return false;

  //read DATA
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=10)
  {
    std::cout <<"Error: sub record DATA of WEAP has invalid length ("<<subLength
              <<" bytes). Should be 10 bytes.\n";
    return false;
  }
  //read DATA's content
  in_File.read((char*) unknownDATA, 10);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of WEAP!\n";
    return false;
  }

  //read DNAM
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cDNAM)
  {
    UnexpectedRecord(cDNAM, subRecName);
    return false;
  }
  //DNAM's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=100)
  {
    std::cout <<"Error: sub record DNAM of WEAP has invalid length ("<<subLength
              <<" bytes). Should be 100 bytes.\n";
    return false;
  }
  //read DNAM's content
  in_File.read((char*) unknownDNAM, 100);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DNAM of WEAP!\n";
    return false;
  }

  //read CRDT
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cCRDT)
  {
    UnexpectedRecord(cCRDT, subRecName);
    return false;
  }
  //CRDT's length
  in_File.read((char*) &subLength, 2);
  if (subLength!=16)
  {
    std::cout <<"Error: sub record CRDT of WEAP has invalid length ("<<subLength
              <<" bytes). Should be 16 bytes.\n";
    return false;
  }
  //read CRDT's content
  in_File.read((char*) unknownCRDT, 16);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord CRDT of WEAP!\n";
    return false;
  }

  //read VNAM
  if (!loadUint32SubRecordFromStream(in_File, cVNAM, unknownVNAM)) return false;

  return in_File.good();
}

uint8_t WeaponRecord::getSizeOfMODT() const
{
  return m_sizeOfMODT;
}

const uint8_t * WeaponRecord::getMODTPointer() const
{
  return m_pointerToMODT;
}

} //namespace
