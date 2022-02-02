/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2022  Dirk Stolle

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

#include "ReferencedObject.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

/* RefDoorData's functions */

RefDoorData::RefDoorData()
: PosX(0.0f), PosY(0.0f), PosZ(0.0f),
  RotX(0.0f), RotY(0.0f), RotZ(0.0f),
  ExitName("")
{ }

bool RefDoorData::operator==(const RefDoorData& other) const
{
  return (PosX == other.PosX) && (PosY == other.PosY) && (PosZ == other.PosZ)
      && (RotX == other.RotX) && (RotY == other.RotY) && (RotZ == other.RotZ)
      && (ExitName == other.ExitName);
}

/* ReferencedObject functions */

ReferencedObject::ReferencedObject()
: ObjectIndex(0),
  ObjectID(""),
  Scale(1.0f),
  // data
  PosX(0.0f), PosY(0.0f), PosZ(0.0f),
  RotX(0.0f), RotY(0.0f), RotZ(0.0f),
  // end of data
  hasDoorData(false),
  DoorData(RefDoorData()),
  LockLevel(std::nullopt),
  KeyID(""),
  TrapID(""),
  OwnerID(""),
  OwnerFactionID(""),
  FactionRank(0),
  GlobalVarID(""),
  SoulCreatureID(""),
  EnchantCharge(std::nullopt),
  NumberOfUses(std::nullopt),
  UnknownNAM9(std::nullopt),
  hasUNAM(false),
  ReferenceBlockedByte(0),
  isDeleted(false),
  DeletionLong(0)
{}

bool ReferencedObject::operator==(const ReferencedObject& other) const
{
  if ((ObjectIndex == other.ObjectIndex) && (ObjectID == other.ObjectID)
      && (isDeleted == other.isDeleted))
  {
    // the part for deleted stuff is short
    if (isDeleted)
    {
      return (DeletionLong == other.DeletionLong);
    }
    // now the part for the references that aren't deleted
    if ((Scale == other.Scale) && (PosX == other.PosX) && (PosY == other.PosY)
      && (PosZ == other.PosZ) && (RotX == other.RotX) && (RotY == other.RotY)
      && (RotZ == other.RotZ) && (LockLevel == other.LockLevel) && (KeyID == other.KeyID)
      && (TrapID == other.TrapID) && (OwnerID == other.OwnerID)
      && (OwnerFactionID == other.OwnerFactionID) && (FactionRank == other.FactionRank)
      && (SoulCreatureID == other.SoulCreatureID) && (EnchantCharge == other.EnchantCharge)
      && (NumberOfUses == other.NumberOfUses)
      && (GlobalVarID == other.GlobalVarID) && (hasUNAM == other.hasUNAM))
    {
      if (hasDoorData != other.hasDoorData)
        return false;
      if (hasDoorData)
      {
        if (!(DoorData == other.DoorData))
          return false;
      }
      if (UnknownNAM9 != other.UnknownNAM9)
        return false;
      if (hasUNAM)
      {
        if (ReferenceBlockedByte != other.ReferenceBlockedByte)
          return false;
      }
      return true;
    }
  }
  return false;
}

bool ReferencedObject::loadFromStream(std::istream& in_File, uint32_t& BytesRead, char* Buffer)
{
  uint32_t SubRecName;
  uint32_t SubLength;
  SubRecName = SubLength = 0;

  /*Referenced Object Data Grouping
        FRMR = Object Index (starts at 1) (4 bytes, long)
			This is used to uniquely identify objects in the cell.  For new files the
			index starts at 1 and is incremented for each new object added.  For modified
			objects the index is kept the same.
		NAME = Object ID string
		XSCL = Scale (4 bytes, float) Static
		DELE = (4 byte long) Indicates that the reference is deleted.
		DODT = XYZ Pos, XYZ Rotation of exit (24 bytes, Door objects)
			float XPos
			float YPos
			float ZPos
			float XRotate
			float YRotate
			float ZRotate
		DNAM = Door exit name (Door objects)
		FLTV = Follows the DNAM optionally, lock level (long)
		KNAM = Door key
		TNAM = Trap name
		UNAM = Reference Blocked (1 byte, 00?), only occurs once in MORROWIND.ESM
		ANAM = Owner ID string
		BNAM = Global variable/rank ID string
        CNAM = ID of faction that owns the object (like ANAM, but belongs to a faction)
        INDX = rank level in faction (4 bytes, signed int32) (-1 means no rank, 0 is first rank)
		INTV = Number of uses ( 4 bytes, long, 1 default), occurs even for objects that don't use it
		NAM9 = ? (4 bytes, long, 0x00000001)
		XSOL = Soul Extra Data (ID string of creature)
		XCHG = Enchanting Charges (4 bytes, float)
		DATA = Ref Position Data (24 bytes)
			float XPos
			float YPos
			float ZPos
			float XRotate
			float YRotate
			float ZRotate
  */

  // FRMR's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 4)
  {
    std::cerr << "Error: Subrecord FRMR of CELL has invalid length ("
              << SubLength << " bytes). Should be four bytes.\n";
    return false;
  }
  // read object index
  in_File.read(reinterpret_cast<char*>(&ObjectIndex), 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord FRMR of CELL!\n";
    return false;
  }

  // read NAME
  in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  // NAME's length
  in_File.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength > 255)
  {
    std::cerr << "Error: Subrecord NAME (FRMR) of CELL is longer than 255 characters.\n";
    return false;
  }
  // read object ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord NAME (FRMR) of CELL!\n";
    return false;
  }
  ObjectID = std::string(Buffer);

  Scale = 1.0f;
  hasDoorData = false;
  LockLevel.reset();
  KeyID = "";
  TrapID = "";
  OwnerID = "";
  OwnerFactionID = "";
  FactionRank = -1;
  GlobalVarID = "";
  SoulCreatureID = "";
  EnchantCharge.reset();
  NumberOfUses.reset();
  UnknownNAM9.reset();
  hasUNAM = false;
  ReferenceBlockedByte = 0;
  DeletionLong = 0;
  isDeleted = false;
  // Let's assume that DATA is the last record.
  while (SubRecName != cDATA)
  {
    // read next header
    in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cXSCL:
           if (Scale != 1.0f)
           {
             std::cerr << "Error: Reference of record CELL seems to have two XSCL subrecords.\n";
             return false;
           }
           // XSCL's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr <<"Error: Sub record XSCL of CELL has invalid length ("
                       << SubLength <<" bytes). Should be four bytes.\n";
             return false;
           }
           // read object scale
           in_File.read(reinterpret_cast<char*>(&Scale), 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XSCL of CELL!\n";
             return false;
           }
           break;
      case cDATA:
           // DATA's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 24)
           {
             std::cerr << "Error: Subrecord DATA of CELL has invalid length ("
                       << SubLength << " bytes). Should be 24 bytes.\n";
             return false;
           }
           // read object position data
           in_File.read(reinterpret_cast<char*>(&PosX), 4);
           in_File.read(reinterpret_cast<char*>(&PosY), 4);
           in_File.read(reinterpret_cast<char*>(&PosZ), 4);
           in_File.read(reinterpret_cast<char*>(&RotX), 4);
           in_File.read(reinterpret_cast<char*>(&RotY), 4);
           in_File.read(reinterpret_cast<char*>(&RotZ), 4);
           BytesRead += 24;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DATA of CELL!\n";
             return false;
           }
           break;
      case cDODT:
           if (hasDoorData)
           {
             std::cerr << "Error: Reference of record CELL seems to have two DODT subrecords.\n";
             return false;
           }
           // DODT's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 24)
           {
             std::cerr << "Error: Subrecord DODT of CELL has invalid length ("
                       << SubLength << " bytes). Should be 24 bytes.\n";
             return false;
           }
           // read object position data
           in_File.read(reinterpret_cast<char*>(&DoorData.PosX), 4);
           in_File.read(reinterpret_cast<char*>(&DoorData.PosY), 4);
           in_File.read(reinterpret_cast<char*>(&DoorData.PosZ), 4);
           in_File.read(reinterpret_cast<char*>(&DoorData.RotX), 4);
           in_File.read(reinterpret_cast<char*>(&DoorData.RotY), 4);
           in_File.read(reinterpret_cast<char*>(&DoorData.RotZ), 4);
           BytesRead += 24;
           DoorData.ExitName.clear();
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DODT of CELL!\n";
             return false;
           }
           hasDoorData = true;
           break;
      case cDNAM:
           if (!hasDoorData)
           {
             std::cerr << "Error while reading CELL: There can't be a DNAM "
                       << "without a previous DODT subrecord.\n";
             return false;
           }
           if (!DoorData.ExitName.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have two DNAM subrecords.\n";
             return false;
           }
           // DNAM's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 255)
           {
             std::cerr << "Error: Subrecord DNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           // read exit name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord DNAM (FRMR) of CELL!\n";
             return false;
           }
           DoorData.ExitName = std::string(Buffer);
           break;
      case cFLTV:
           if (LockLevel.has_value())
           {
             std::cerr << "Error: Reference of record CELL seems to have two FLTV subrecords.\n";
             return false;
           }
           // FLTV's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord FLTV (FRMR) of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read lock level
           LockLevel = 0;
           in_File.read(reinterpret_cast<char*>(&LockLevel.value()), 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord FLTV (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cINTV:
           if (NumberOfUses.has_value())
           {
             std::cerr << "Error: Reference of record CELL seems to have two INTV subrecords.\n";
             return false;
           }
           // INTV's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord INTV (FRMR) of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read number of uses
           NumberOfUses = 0;
           in_File.read(reinterpret_cast<char*>(&NumberOfUses.value()), 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord INTV (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cKNAM:
           if (!LockLevel.has_value())
           {
             std::cerr << "Error while reading CELL: There can't be a KNAM "
                       << "without a previous FLTV subrecord.\n";
             return false;
           }
           if (!KeyID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have two KNAM subrecords.\n";
             return false;
           }
           // KNAM's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 255)
           {
             std::cerr << "Error: Subrecord KNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           // read key ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord KNAM (FRMR) of CELL!\n";
             return false;
           }
           KeyID = std::string(Buffer);
           break;
      case cTNAM:
           if (!TrapID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have two TNAM subrecords.\n";
             return false;
           }
           // TNAM's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 255)
           {
             std::cerr << "Error: Subrecord TNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           // read trap ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord TNAM (FRMR) of CELL!\n";
             return false;
           }
           TrapID = std::string(Buffer);
           break;
      case cANAM:
           if (!OwnerID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have two ANAM subrecords.\n";
             return false;
           }
           if (!OwnerFactionID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have both "
                       << "ANAM and CNAM subrecords, but it should have only "
                       << "one of them.\n";
             return false;
           }
           // ANAM's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 255)
           {
             std::cerr << "Error: Subrecord ANAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           // read owner ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord ANAM (FRMR) of CELL!\n";
             return false;
           }
           OwnerID = std::string(Buffer);
           break;
      case cCNAM:
           if (!OwnerFactionID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have two CNAM subrecords.\n";
             return false;
           }
           if (!OwnerID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have both "
                       << "ANAM and CNAM subrecords, but it should have only "
                       << "one of them.\n";
             return false;
           }
           // CNAM's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 255)
           {
             std::cerr << "Error: Subrecord CNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           // read owner faction ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord CNAM (FRMR) of CELL!\n";
             return false;
           }
           OwnerFactionID = std::string(Buffer);

           // INDX should follow right now.
           // read INDX
           in_File.read((char*) &SubRecName, 4);
           BytesRead += 4;
           if (SubRecName != cINDX)
           {
             UnexpectedRecord(cINDX, SubRecName);
             return false;
           }
           // read INDX's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord INDX (FRMR) of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read faction rank number
           in_File.read(reinterpret_cast<char*>(&FactionRank), 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord INDX (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cBNAM:
           if (!GlobalVarID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have two BNAM subrecords.\n";
             return false;
           }
           // BNAM's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 255)
           {
             std::cerr << "Error: Subrecord BNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           // read global var ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord BNAM (FRMR) of CELL!\n";
             return false;
           }
           GlobalVarID = std::string(Buffer);
           break;
      case cXSOL:
           if (!SoulCreatureID.empty())
           {
             std::cerr << "Error: Reference of record CELL seems to have two XSOL subrecords.\n";
             return false;
           }
           // XSOL's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 255)
           {
             std::cerr << "Error: Subrecord XSOL (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           // read soul creature's ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XSOL (FRMR) of CELL!\n";
             return false;
           }
           SoulCreatureID = std::string(Buffer);
           break;
      case cXCHG:
           if (EnchantCharge.has_value())
           {
             std::cerr << "Error: Reference of record CELL seems to have two XCHG subrecords.\n";
             return false;
           }
           // XCHG's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord XCHG (FRMR) of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read charged enchantment points
           EnchantCharge = 0.0f;
           in_File.read(reinterpret_cast<char*>(&EnchantCharge.value()), 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XCHG (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cNAM9:
           if (UnknownNAM9.has_value())
           {
             std::cerr << "Error: Reference of record CELL seems to have two NAM9 subrecords.\n";
             return false;
           }
           // NAM9's length
           in_File.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord NAM9 (FRMR) of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read unknown value
           UnknownNAM9 = 0;
           in_File.read(reinterpret_cast<char*>(&UnknownNAM9.value()), 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM9 (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cUNAM:
           if (hasUNAM)
           {
             std::cout << "Error: reference of record CELL seems to have two UNAM subrecords.\n";
             return false;
           }
           //UNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=1)
           {
             std::cout <<"Error: sub record UNAM of CELL has invalid length ("
                       <<SubLength <<" bytes). Should be one byte.\n";
             return false;
           }
           //read reference blocked
           in_File.read((char*) &ReferenceBlockedByte, 1);
           BytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord UNAM of CELL!\n";
             return false;
           }
           hasUNAM = true;
           break;
      case cDELE:
           //DELE's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord DELE (FRMR) of CELL has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read deletion value
           in_File.read((char*) &DeletionLong, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DELE (FRMR) of CELL!\n";
             return false;
           }
           isDeleted = true;
           //DELE is the last subrecord of a reference, so we can return here
           return in_File.good();
      default:
           std::cerr << "Error while reading CELL: Expected record name XSCL, "
                     << "DODT, DNAM, FLTV, INTV, KNAM, TNAM, ANAM, BNAM, CNAM, "
                     << "XSOL, XCHG, NAM9, UNAM, DELE or DATA not found. Instead, \""
                     << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
    }
  }

  return in_File.good();
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ReferencedObject::saveToStream(std::ostream& output) const
{
  /*Referenced Object Data Grouping
        FRMR = Object Index (starts at 1) (4 bytes, long)
			This is used to uniquely identify objects in the cell.  For new files the
			index starts at 1 and is incremented for each new object added.  For modified
			objects the index is kept the same.
		NAME = Object ID string
		XSCL = Scale (4 bytes, float) Static
		DELE = (4 byte long) Indicates that the reference is deleted.
		DODT = XYZ Pos, XYZ Rotation of exit (24 bytes, Door objects)
			float XPos
			float YPos
			float ZPos
			float XRotate
			float YRotate
			float ZRotate
		DNAM = Door exit name (Door objects)
		FLTV = Follows the DNAM optionally, lock level (long)
		KNAM = Door key
		TNAM = Trap name
		UNAM = Reference Blocked (1 byte, 00?), only occurs once in MORROWIND.ESM
		ANAM = Owner ID string
		BNAM = Global variable/rank ID string
        CNAM = ID of faction that owns the object (like ANAM, but belongs to a faction)
        INDX = rank level in faction (4 bytes, signed int32) (-1 means no rank, 0 is first rank)
		INTV = Number of uses ( 4 bytes, long, 1 default), occurs even for objects that don't use it
		NAM9 = ? (4 bytes, long, 0x00000001)
		XSOL = Soul Extra Data (ID string of creature)
		XCHG = Enchanting Charges (4 bytes, float)
		DATA = Ref Position Data (24 bytes)
			float XPos
			float YPos
			float ZPos
			float XRotate
			float YRotate
			float ZRotate
  */

  // write object index (FRMR)
  output.write(reinterpret_cast<const char*>(&cFRMR), 4);
  uint32_t SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&ObjectIndex), 4);

  // write object ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  SubLength = ObjectID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ObjectID.c_str(), SubLength);

  if (isDeleted)
  {
    // write deletion status (DELE)
    output.write(reinterpret_cast<const char*>(&cDELE), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    // write content of DELE (kind of useless, I guess)
    output.write(reinterpret_cast<const char*>(&DeletionLong), 4);

    // Return here, because we are done after that.
    return output.good();
  }

  if (Scale != 1.0f)
  {
    // write object scale (XSCL)
    output.write(reinterpret_cast<const char*>(&cXSCL), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&Scale), 4);
  }

  if (hasDoorData)
  {
    // write door position data (DODT)
    output.write(reinterpret_cast<const char*>(&cDODT), 4);
    SubLength = 24;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&DoorData.PosX), 4);
    output.write(reinterpret_cast<const char*>(&DoorData.PosY), 4);
    output.write(reinterpret_cast<const char*>(&DoorData.PosZ), 4);
    output.write(reinterpret_cast<const char*>(&DoorData.RotX), 4);
    output.write(reinterpret_cast<const char*>(&DoorData.RotY), 4);
    output.write(reinterpret_cast<const char*>(&DoorData.RotZ), 4);

    if (!DoorData.ExitName.empty())
    {
      // write door's exit name (DNAM)
      output.write(reinterpret_cast<const char*>(&cDNAM), 4);
      SubLength = DoorData.ExitName.length() + 1;
      output.write(reinterpret_cast<const char*>(&SubLength), 4);
      output.write(DoorData.ExitName.c_str(), SubLength);
    }
  }

  if (LockLevel.has_value())
  {
    // write lock level (FLTV)
    output.write(reinterpret_cast<const char*>(&cFLTV), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&LockLevel.value()), 4);

    if (!KeyID.empty())
    {
      // write key ID (KNAM)
      output.write(reinterpret_cast<const char*>(&cKNAM), 4);
      SubLength = KeyID.length() + 1;
      output.write(reinterpret_cast<const char*>(&SubLength), 4);
      output.write(KeyID.c_str(), SubLength);
    }
  }

  if (!TrapID.empty())
  {
    // write trap ID (TNAM)
    output.write(reinterpret_cast<const char*>(&cTNAM), 4);
    SubLength = TrapID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(TrapID.c_str(), SubLength);
  }

  if (!OwnerID.empty())
  {
    // write owner ID (ANAM)
    output.write(reinterpret_cast<const char*>(&cANAM), 4);
    SubLength = OwnerID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(OwnerID.c_str(), SubLength);
  }
  else if (!OwnerFactionID.empty())
  {
    // write owner faction ID (CNAM)
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    SubLength = OwnerFactionID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(OwnerFactionID.c_str(), SubLength);

    // write owner faction rank (INDX)
    output.write(reinterpret_cast<const char*>(&cINDX), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&FactionRank), 4);
  }

  if (!GlobalVarID.empty())
  {
    // write global variable name (BNAM)
    output.write(reinterpret_cast<const char*>(&cBNAM), 4);
    SubLength = GlobalVarID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(GlobalVarID.c_str(), SubLength);
  }

  if (!SoulCreatureID.empty())
  {
    // write ID of creature of captured soul (XSOL)
    output.write(reinterpret_cast<const char*>(&cXSOL), 4);
    SubLength = SoulCreatureID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(SoulCreatureID.c_str(), SubLength);
  }

  if (EnchantCharge.has_value())
  {
    // write charges (XCHG)
    output.write(reinterpret_cast<const char*>(&cXCHG), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&EnchantCharge.value()), 4);
  }

  if (NumberOfUses.has_value())
  {
    // write number of uses (INTV)
    output.write(reinterpret_cast<const char*>(&cINTV), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&NumberOfUses.value()), 4);
  }

  if (UnknownNAM9.has_value())
  {
    // write NAM9
    output.write(reinterpret_cast<const char*>(&cNAM9), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&UnknownNAM9.value()), 4);
  }

  if (hasUNAM)
  {
    // write UNAM
    output.write(reinterpret_cast<const char*>(&cUNAM), 4);
    SubLength = 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&ReferenceBlockedByte), 1);
  }

  // write object position data (DATA)
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  SubLength = 24;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // write object position data
  output.write(reinterpret_cast<const char*>(&PosX), 4);
  output.write(reinterpret_cast<const char*>(&PosY), 4);
  output.write(reinterpret_cast<const char*>(&PosZ), 4);
  output.write(reinterpret_cast<const char*>(&RotX), 4);
  output.write(reinterpret_cast<const char*>(&RotY), 4);
  output.write(reinterpret_cast<const char*>(&RotZ), 4);

  return output.good();
}
#endif

uint32_t ReferencedObject::getWrittenSize() const
{
  uint32_t Result = 4 /* FRMR */ +4 /* 4 bytes for length */ +4 /* fixed length of 4 bytes */
                   +4 /* NAME */ +4 /* 4 bytes for length */
                   +ObjectID.length()+1 /* length of string +1 byte for NUL */;
  if (isDeleted)
  {
    Result = Result + 4 /* DELE */ +4 /* 4 bytes for length */ +4 /* fixed length of 4 bytes */;
    return Result;
  }

  Result = Result +4 /* DATA */ +4 /* 4 bytes for length */ +24 /* fixed length of 24 bytes */;

  if (Scale!=1.0f)
  {
    Result = Result +4 /* XSCL */ +4 /* 4 bytes for length */ +4 /* fixed length of 4 bytes */;
  }

  if (hasDoorData)
  {
    Result = Result +4 /* DODT */ +4 /* 4 bytes for length */ +24 /* fixed length of 24 bytes */;
    if (!DoorData.ExitName.empty())
    {
      Result = Result +4 /* DNAM */ +4 /* 4 bytes for length */
              +DoorData.ExitName.length()+1 /* length of string +1 byte for NUL */;
    }
  }

  if (LockLevel.has_value())
  {
    Result = Result + 4 /* FLTV */ + 4 /* 4 bytes for length */ + 4 /* fixed size of 4 bytes */;
    if (!KeyID.empty())
    {
      Result = Result + 4 /* KNAM */ + 4 /* 4 bytes for length */
             + KeyID.length() + 1 /* length of string +1 byte for NUL */;
    }
  }

  if (!TrapID.empty())
  {
    Result = Result +4 /* TNAM */ +4 /* 4 bytes for length */
            +TrapID.length()+1 /* length of string +1 byte for NUL */;
  }

  if (!OwnerID.empty())
  {
    Result = Result +4 /* ANAM */ +4 /* 4 bytes for length */
            +OwnerID.length()+1 /* length of string +1 byte for NUL */;
  }
  else if (!OwnerFactionID.empty())
  {
    Result = Result +4 /* CNAM */ +4 /* 4 bytes for length */
            +OwnerFactionID.length()+1 /* length of string +1 byte for NUL */
            +4 /* INDX */ +4 /* 4 bytes for length */ +4 /* fixed length of four bytes*/;
  }

  if (!GlobalVarID.empty())
  {
    Result = Result + 4 /* BNAM */ + 4 /* 4 bytes for length */
           + GlobalVarID.length() + 1 /* length of string +1 byte for NUL */;
  }

  if (!SoulCreatureID.empty())
  {
    Result = Result + 4 /* XSOL */ + 4 /* 4 bytes for length */
           + SoulCreatureID.length() + 1 /* length of string +1 byte for NUL */;
  }

  if (EnchantCharge.has_value())
  {
    Result = Result + 4 /* XCHG */ + 4 /* 4 bytes for length */ + 4 /* fixed size of 4 bytes */;
  }

  if (UnknownNAM9.has_value())
  {
    Result = Result +4 /* NAM9 */ +4 /* 4 bytes for length */ +4 /* fixed size of 4 bytes */;
  }

  if (hasUNAM)
  {
    Result = Result +4 /* UNAM */ +4 /* 4 bytes for length */ +1 /* fixed size of one byte */;
  }

  return Result;
}

} // namespace
