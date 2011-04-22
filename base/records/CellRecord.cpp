/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "CellRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

/* RefDoorData's functions */

bool RefDoorData::operator==(const RefDoorData& other) const
{
  return ((PosX==other.PosX) and (PosY==other.PosY) and (PosZ==other.PosZ)
      and (RotX==other.RotX) and (RotY==other.RotY) and (RotZ==other.RotZ));
}

/* AmbientLight's functions */

bool AmbientLight::operator==(const AmbientLight& other) const
{
  if (isPresent xor other.isPresent) return false;
  if (isPresent)
  {
    return ((AmbientColour==other.AmbientColour)
        and (SunlightColour==other.SunlightColour)
        and (FogColour==other.FogColour) and (FogDensity==other.FogDensity));
  }
  return true;
}

/* ReferencedObject functions */

ReferencedObject::ReferencedObject()
{
  ObjectIndex = 0;
  ObjectID = "";
  Scale = 1.0f;
  //data
  PosX = PosY = PosZ = 0.0f;
  RotX = RotY = RotZ = 0.0f;
  //end of data
  hasDoorData = false;
  hasFLTV = false;
  LockLevel = 0;
  KeyID = "";
  TrapID = "";
  OwnerID = "";
  OwnerFactionID = "";
  FactionRank = 0;
  GlobalVarID = "";
  SoulCreatureID = "";
  hasXCHG = false;
  EnchantCharge = 0.0f;
  NumberOfUses = 0;
  hasNAM9 = false;
  UnknownNAM9 = 0;
  hasUNAM = false;
  ReferenceBlockedByte = 0;
  isDeleted = false;
  DeletionLong = 0;
}

bool ReferencedObject::operator==(const ReferencedObject& other) const
{
  if ((ObjectIndex==other.ObjectIndex) and (ObjectID==other.ObjectID)
      and (isDeleted==other.isDeleted))
  {
    //the part for deleted stuff is short
    if (isDeleted)
    {
      return (DeletionLong==other.DeletionLong);
    }
    //now the part for the references that aren't deleted
    if ((Scale==other.Scale) and (PosX==other.PosX) and (PosY==other.PosY)
      and (PosZ==other.PosZ) and (RotX==other.RotX) and (RotY==other.RotY)
      and (RotZ==other.RotZ) and (hasFLTV==other.hasFLTV) and (KeyID==other.KeyID)
      and (TrapID==other.TrapID) and (OwnerID==other.OwnerID)
      and (OwnerFactionID==other.OwnerFactionID) and (FactionRank==other.FactionRank)
      and (SoulCreatureID==other.SoulCreatureID) and (hasXCHG==other.hasXCHG)
      and (NumberOfUses==NumberOfUses) and (hasNAM9==other.hasNAM9)
      and (GlobalVarID==other.GlobalVarID) and (hasUNAM==other.hasUNAM))
    {
      if (hasDoorData!=other.hasDoorData) return false;
      if (hasDoorData)
      {
        if (!(DoorData == other.DoorData))
          return false;
      }
      if (hasFLTV)
      {
        if (LockLevel!=other.LockLevel)
          return false;
      }
      if (hasNAM9)
      {
        if (UnknownNAM9!=other.UnknownNAM9)
          return false;
      }
      if (hasUNAM)
      {
        if (ReferenceBlockedByte!=other.ReferenceBlockedByte)
          return false;
      }
      if (hasXCHG)
      {
        return (EnchantCharge==other.EnchantCharge);
      }
      return true;
    }
  }//if
  return false;
}

bool ReferencedObject::loadFromStream(std::ifstream& in_File, uint32_t& BytesRead, char* Buffer)
{
  int32_t SubRecName;
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

  //FRMR's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout << "Error: subrecord FRMR of CELL has invalid length ("<<SubLength
              << " bytes). Should be four bytes.\n";
    return false;
  }
  //read object index
  in_File.read((char*) &ObjectIndex, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord FRMR of CELL!\n";
    return false;
  }

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME (FRMR) of CELL is longer than 255 characters.\n";
    return false;
  }
  //read object ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME (FRMR) of CELL!\n";
    return false;
  }
  ObjectID = std::string(Buffer);

  Scale = 1.0f;
  hasDoorData = false;
  hasFLTV = false;
  LockLevel = 0;
  KeyID = "";
  TrapID = "";
  OwnerID = "";
  OwnerFactionID = "";
  FactionRank = -1;
  GlobalVarID = "";
  SoulCreatureID = "";
  hasXCHG = false;
  NumberOfUses = 0;
  hasNAM9 = false;
  UnknownNAM9 = 0;
  hasUNAM = false;
  ReferenceBlockedByte = 0;
  DeletionLong = 0;
  isDeleted = false;
  //let's assume that DATA is the last record
  while (SubRecName!=cDATA)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cXSCL:
           if (Scale!=1.0f)
           {
             std::cout << "Error: reference of record CELL seems to have two XSCL subrecords.\n";
             return false;
           }
           //XSCL's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout <<"Error: sub record XSCL of CELL has invalid length ("
                       <<SubLength <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read object scale
           in_File.read((char*) &Scale, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XSCL of CELL!\n";
             return false;
           }
           break;
      case cDATA:
           //DATA's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=24)
           {
             std::cout << "Error: subrecord DATA of CELL has invalid length ("<<SubLength
                       << " bytes). Should be 24 bytes.\n";
             return false;
           }
           //read object position data
           in_File.read((char*) &PosX, 4);
           in_File.read((char*) &PosY, 4);
           in_File.read((char*) &PosZ, 4);
           in_File.read((char*) &RotX, 4);
           in_File.read((char*) &RotY, 4);
           in_File.read((char*) &RotZ, 4);
           BytesRead += 24;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of CELL!\n";
             return false;
           }
           break;
      case cDODT:
           if (hasDoorData)
           {
             std::cout << "Error: reference of record CELL seems to have two DODT subrecords.\n";
             return false;
           }
           //DODT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=24)
           {
             std::cout << "Error: subrecord DODT of CELL has invalid length ("<<SubLength
                       << " bytes). Should be 24 bytes.\n";
             return false;
           }
           //read object position data
           in_File.read((char*) &(DoorData.PosX), 4);
           in_File.read((char*) &(DoorData.PosY), 4);
           in_File.read((char*) &(DoorData.PosZ), 4);
           in_File.read((char*) &(DoorData.RotX), 4);
           in_File.read((char*) &(DoorData.RotY), 4);
           in_File.read((char*) &(DoorData.RotZ), 4);
           BytesRead += 24;
           DoorData.ExitName = "";
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DODT of CELL!\n";
             return false;
           }
           hasDoorData = true;
           break;
      case cDNAM:
           if (!hasDoorData)
           {
             std::cout << "Error while reading CELL: there can't be a DNAM "
                       << "without a previous DODT subrecord.\n";
             return false;
           }
           if (DoorData.ExitName!="")
           {
             std::cout << "Error: reference of record CELL seems to have two DNAM subrecords.\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord DNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           //read exit name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM (FRMR) of CELL!\n";
             return false;
           }
           DoorData.ExitName = std::string(Buffer);
           break;
      case cFLTV:
           if (hasFLTV)
           {
             std::cout << "Error: reference of record CELL seems to have two FLTV subrecords.\n";
             return false;
           }
           //FLTV's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord FLTV (FRMR) of CELL has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read lock level
           in_File.read((char*) &LockLevel, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FLTV (FRMR) of CELL!\n";
             return false;
           }
           hasFLTV = true;
           break;
      case cINTV:
           if (NumberOfUses!=0)
           {
             std::cout << "Error: reference of record CELL seems to have two INTV subrecords.\n";
             return false;
           }
           //INTV's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord INTV (FRMR) of CELL has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read number of uses
           in_File.read((char*) &NumberOfUses, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord INTV (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cKNAM:
           if (not hasFLTV)
           {
             std::cout << "Error while reading CELL: there can't be a KNAM "
                       << "without a previous FLTV subrecord.\n";
             return false;
           }
           if (KeyID!="")
           {
             std::cout << "Error: reference of record CELL seems to have two KNAM subrecords.\n";
             return false;
           }
           //KNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord KNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           //read key ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KNAM (FRMR) of CELL!\n";
             return false;
           }
           KeyID = std::string(Buffer);
           break;
      case cTNAM:
           if (TrapID!="")
           {
             std::cout << "Error: reference of record CELL seems to have two TNAM subrecords.\n";
             return false;
           }
           //TNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord TNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           //read trap ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord TNAM (FRMR) of CELL!\n";
             return false;
           }
           TrapID = std::string(Buffer);
           break;
      case cANAM:
           if (OwnerID!="")
           {
             std::cout << "Error: reference of record CELL seems to have two ANAM subrecords.\n";
             return false;
           }
           if (OwnerFactionID!="")
           {
             std::cout << "Error: reference of record CELL seems to have both "
                       << "ANAM and CNAM subrecords, but it should have only "
                       << "one of them.\n";
             return false;
           }
           //ANAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ANAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           //read owner ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ANAM (FRMR) of CELL!\n";
             return false;
           }
           OwnerID = std::string(Buffer);
           break;
      case cCNAM:
           if (OwnerFactionID!="")
           {
             std::cout << "Error: reference of record CELL seems to have two CNAM subrecords.\n";
             return false;
           }
           if (OwnerID!="")
           {
             std::cout << "Error: reference of record CELL seems to have both "
                       << "ANAM and CNAM subrecords, but it should have only "
                       << "one of them.\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord CNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           //read owner faction ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM (FRMR) of CELL!\n";
             return false;
           }
           OwnerFactionID = std::string(Buffer);

           //INDX should follow right now
           //read INDX
           in_File.read((char*) &SubRecName, 4);
           BytesRead += 4;
           if (SubRecName!=cINDX)
           {
             UnexpectedRecord(cINDX, SubRecName);
             return false;
           }
           //read INDX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord INDX (FRMR) of CELL has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read faction rank number
           in_File.read((char*) &FactionRank, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord INDX (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cBNAM:
           if (GlobalVarID!="")
           {
             std::cout << "Error: reference of record CELL seems to have two BNAM subrecords.\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord BNAM (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           //read global var ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM (FRMR) of CELL!\n";
             return false;
           }
           GlobalVarID = std::string(Buffer);
           break;
      case cXSOL:
           if (SoulCreatureID!="")
           {
             std::cout << "Error: reference of record CELL seems to have two XSOL subrecords.\n";
             return false;
           }
           //XSOL's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord XSOL (FRMR) of CELL is longer than 255 characters.\n";
             return false;
           }
           //read soul creature's ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XSOL (FRMR) of CELL!\n";
             return false;
           }
           SoulCreatureID = std::string(Buffer);
           break;
      case cXCHG:
           if (hasXCHG)
           {
             std::cout << "Error: reference of record CELL seems to have two XCHG subrecords.\n";
             return false;
           }
           //XCHG's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord XCHG (FRMR) of CELL has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read charged enchantment points
           in_File.read((char*) &EnchantCharge, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XCHG (FRMR) of CELL!\n";
             return false;
           }
           break;
      case cNAM9:
           if (hasNAM9)
           {
             std::cout << "Error: reference of record CELL seems to have two NAM9 subrecords.\n";
             return false;
           }
           //NAM9's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord NAM9 (FRMR) of CELL has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read unknown value
           in_File.read((char*) &UnknownNAM9, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM9 (FRMR) of CELL!\n";
             return false;
           }
           hasNAM9 = true;
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
           break;
      default:
           std::cout << "Error while reading CELL: expected record name XSCL, "
                     << "DODT, DNAM, FLTV, INTV, KNAM, TNAM, ANAM, BNAM, CNAM, "
                     << "XSOL, XCHG, NAM9, UNAM, DELE or DATA not found. Instead, \""
                     << IntTo4Char(SubRecName)<<"\" was found.\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

bool ReferencedObject::saveToStream(std::ofstream& output) const
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

  //write FRMR
  output.write((char*) &cFRMR, 4);
  uint32_t SubLength = 4; //fixed length of four bytes
  //write FRMR's length
  output.write((char*) &SubLength, 4);
  //write object index
  output.write((char*) &ObjectIndex, 4);

  //write NAME
  output.write((char*) &cNAME, 4);
  SubLength = ObjectID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write object ID
  output.write(ObjectID.c_str(), SubLength);

  if (isDeleted)
  {
    //write DELE
    output.write((char*) &cDELE, 4);
    SubLength = 4; //fixed length of four bytes
    //write DELE's length
    output.write((char*) &SubLength, 4);
    //write content of DELE (kind of useless, I guess)
    output.write((char*) &DeletionLong, 4);

    //return here, because we are done after that
    return output.good();
  }

  if (Scale!=1.0f)
  {
    //write XSCL
    output.write((char*) &cXSCL, 4);
    SubLength = 4; //fixed length of four bytes
    //write XSCL's length
    output.write((char*) &SubLength, 4);
    //write object scale
    output.write((char*) &Scale, 4);
  }

  if (hasDoorData)
  {
    //write DODT
    output.write((char*) &cDODT, 4);
    SubLength = 24; //fixed length of 24 bytes
    //write DODT's length
    output.write((char*) &SubLength, 4);
    //write door position data
    output.write((char*) &(DoorData.PosX), 4);
    output.write((char*) &(DoorData.PosY), 4);
    output.write((char*) &(DoorData.PosZ), 4);
    output.write((char*) &(DoorData.RotX), 4);
    output.write((char*) &(DoorData.RotY), 4);
    output.write((char*) &(DoorData.RotZ), 4);

    if (DoorData.ExitName!="")
    {
      //write DNAM
      output.write((char*) &cDNAM, 4);
      SubLength = DoorData.ExitName.length()+1; //length of string +1 for NUL
      //write DNAM's length
      output.write((char*) &SubLength, 4);
      //write door's exit name
      output.write(DoorData.ExitName.c_str(), SubLength);
    }
  }

  if (hasFLTV)
  {
    //write FLTV
    output.write((char*) &cFLTV, 4);
    SubLength = 4; //fixed length of four bytes
    //write FLTV's length
    output.write((char*) &SubLength, 4);
    //write lock level
    output.write((char*) &LockLevel, 4);

    if (KeyID!="")
    {
      //write KNAM
      output.write((char*) &cKNAM, 4);
      SubLength = KeyID.length()+1; //length of string +1 for NUL
      //write KNAM's length
      output.write((char*) &SubLength, 4);
      //write key ID
      output.write(KeyID.c_str(), SubLength);
    }
  }

  if (TrapID!="")
  {
    //write TNAM
    output.write((char*) &cTNAM, 4);
    SubLength = TrapID.length()+1; //length of string +1 for NUL
    //write TNAM's length
    output.write((char*) &SubLength, 4);
    //write trap ID
    output.write(TrapID.c_str(), SubLength);
  }

  if (OwnerID!="")
  {
    //write ANAM
    output.write((char*) &cANAM, 4);
    SubLength = OwnerID.length()+1; //length of string +1 for NUL
    //write ANAM's length
    output.write((char*) &SubLength, 4);
    //write owner ID
    output.write(OwnerID.c_str(), SubLength);
  }
  else if (OwnerFactionID!="")
  {
    //write CNAM
    output.write((char*) &cCNAM, 4);
    SubLength = OwnerFactionID.length()+1; //length of string +1 for NUL
    //write CNAM's length
    output.write((char*) &SubLength, 4);
    //write owner faction ID
    output.write(OwnerFactionID.c_str(), SubLength);

    //write INDX
    output.write((char*) &cINDX, 4);
    SubLength = 4; //fixed length of four bytes
    //write INDX's length
    output.write((char*) &SubLength, 4);
    //write owner faction rank
    output.write((char*) &FactionRank, 4);
  }

  if (GlobalVarID!="")
  {
    //write BNAM
    output.write((char*) &cBNAM, 4);
    SubLength = GlobalVarID.length()+1; //length of string +1 for NUL
    //write BNAM's length
    output.write((char*) &SubLength, 4);
    //write global var ID
    output.write(GlobalVarID.c_str(), SubLength);
  }

  if (SoulCreatureID!="")
  {
    //write XSOL
    output.write((char*) &cXSOL, 4);
    SubLength = SoulCreatureID.length()+1; //length of string +1 for NUL
    //write XSOL's length
    output.write((char*) &SubLength, 4);
    //write ID of creature of captured soul
    output.write(SoulCreatureID.c_str(), SubLength);
  }

  if (hasXCHG)
  {
    //write XCHG
    output.write((char*) &cXCHG, 4);
    SubLength = 4; //fixed length of four bytes
    //write XCHG's length
    output.write((char*) &SubLength, 4);
    //write charges
    output.write((char*) &EnchantCharge, 4);
  }

  if (hasNAM9)
  {
    //write NAM9
    output.write((char*) &cNAM9, 4);
    SubLength = 4; //fixed length of four bytes
    //write NAM9's length
    output.write((char*) &SubLength, 4);
    //write NAM9
    output.write((char*) &UnknownNAM9, 4);
  }

  if (hasUNAM)
  {
    //write UNAM
    output.write((char*) &cUNAM, 4);
    SubLength = 1; //fixed length of one byte
    //write UNAM's length
    output.write((char*) &SubLength, 4);
    //write UNAM
    output.write((char*) &ReferenceBlockedByte, 1);
  }

  //write DATA
  output.write((char*) &cDATA, 4);
  SubLength = 24; //fixed length of 24 bytes
  //write DATA's length
  output.write((char*) &SubLength, 4);
  //write object position data
  output.write((char*) &PosX, 4);
  output.write((char*) &PosY, 4);
  output.write((char*) &PosZ, 4);
  output.write((char*) &RotX, 4);
  output.write((char*) &RotY, 4);
  output.write((char*) &RotZ, 4);

  return output.good();
}

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
    if (DoorData.ExitName!="")
    {
      Result = Result +4 /* DNAM */ +4 /* 4 bytes for length */
              +DoorData.ExitName.length()+1 /* length of string +1 byte for NUL */;
    }
  }

  if (hasFLTV)
  {
    Result = Result +4 /* FLTV */ +4 /* 4 bytes for length */ +4 /* fixed size of 4 bytes */;
    if (KeyID!="")
    {
      Result = Result +4 /* KNAM */ +4 /* 4 bytes for length */
              +KeyID.length()+1 /* length of string +1 byte for NUL */;
    }
  }

  if (TrapID!="")
  {
    Result = Result +4 /* TNAM */ +4 /* 4 bytes for length */
            +TrapID.length()+1 /* length of string +1 byte for NUL */;
  }

  if (OwnerID!="")
  {
    Result = Result +4 /* ANAM */ +4 /* 4 bytes for length */
            +OwnerID.length()+1 /* length of string +1 byte for NUL */;
  }
  else if (OwnerFactionID!="")
  {
    Result = Result +4 /* CNAM */ +4 /* 4 bytes for length */
            +OwnerFactionID.length()+1 /* length of string +1 byte for NUL */
            +4 /* INDX */ +4 /* 4 bytes for length */ +4 /* fixed length of four bytes*/;
  }

  if (GlobalVarID!="")
  {
    Result = Result +4 /* BNAM */ +4 /* 4 bytes for length */
            +GlobalVarID.length()+1 /* length of string +1 byte for NUL */;
  }

  if (SoulCreatureID!="")
  {
    Result = Result +4 /* XSOL */ +4 /* 4 bytes for length */
            +SoulCreatureID.length()+1 /* length of string +1 byte for NUL */;
  }

  if (hasXCHG)
  {
    Result = Result +4 /* XCHG */ +4 /* 4 bytes for length */ +4 /* fixed size of 4 bytes */;
  }

  if (hasNAM9)
  {
    Result = Result +4 /* NAM9 */ +4 /* 4 bytes for length */ +4 /* fixed size of 4 bytes */;
  }

  if (hasUNAM)
  {
    Result = Result +4 /* UNAM */ +4 /* 4 bytes for length */ +1 /* fixed size of one byte */;
  }

  return Result;
}

/* **** CellRecord functions ****/

const uint32_t cfInterior = 1;
const uint32_t cfWater = 2;

CellRecord::CellRecord()
{
  CellID = "";
  //cell data
  CellFlags = 0;
  GridX = 0;
  GridY = 0;
  //end of cell data
  RegionID = "";
  NumReferences = 0;
  References.clear();
  ColourRef = 0;
  Ambience.isPresent = false;
  hasWHGT = false;
  WaterHeight = 0.0f;
}

bool CellRecord::equals(const CellRecord& other) const
{
  return ((CellID==other.CellID) and (CellFlags==other.CellFlags)
      and (GridX==other.GridX) and (GridY==other.GridY)
      and (RegionID==other.RegionID) and (References==other.References)
      and (NumReferences==other.NumReferences) and (ColourRef==other.ColourRef)
      and (Ambience==other.Ambience) and (hasWHGT==other.hasWHGT)
      and ((WaterHeight==other.WaterHeight) or (not hasWHGT)));
}

bool CellRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cCELL, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +CellID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* DATA */ +4 /* 4 bytes for length */ +12 /* fixed length of 12 bytes */;
  if (RegionID!="")
  {
    Size = Size +4 /* RGNN */ +4 /* 4 bytes for length */
          +RegionID.length()+1 /* length of model path +1 byte for NUL termination */;
  }
  if (!isInterior())
  {
    Size = Size +4 /* NAM5 */ +4 /* 4 bytes for length */
                +4 /* fixed length of 4 bytes */;
  }
  else
  {
    //WHGT and AMBI is for interior cells only
    if (hasWHGT)
    {
      Size = Size +4 /* WHGT */ +4 /* 4 bytes for length */
                  +4 /* fixed length of 4 bytes */;
    }
    if (Ambience.isPresent)
    {
      Size = Size +4 /* AMBI */ +4 /* 4 bytes for length */
                  +16 /* fixed length of 16 bytes */;
    }
  }

  if (NumReferences!=0)
  {
    Size = Size +4 /* NAM0 */ +4 /* 4 bytes for length */
                +4 /* fixed length of 4 bytes */;
  }
  unsigned int i;
  for (i=0; i<References.size(); ++i)
  {
    Size = Size + References[i].getWrittenSize();
  }//for

  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*Cells:
    NAME = Cell ID string. Can be an empty string for exterior cells in which case
        the region name is used instead.
    DATA = Cell Data
        long Flags
            0x01 = Interior?
            0x02 = Has Water
            0x04 = Illegal to Sleep here
            0x80 = Behave like exterior (Tribunal)
        long GridX
        long GridY
    RGNN = Region name string
    NAM0 = Number of objects in cell in current file? (4 byte, long), Optional

    Exterior Cell Sub-Records
        NAM5 = Map Color (4 bytes, long, COLORREF)

    Interior Cell Sub-Records
        WHGT = Water Height (4 bytes, float)
        AMBI = Ambient Light Level (16 bytes)
            long AmbientColor
            long SunlightColor
            long FogColor
            float FogDensity

    Referenced Object Data Grouping
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

  //write NAME
  output.write((char*) &cNAME, 4);
  uint32_t SubLength = CellID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write cell name
  output.write(CellID.c_str(), SubLength);

  //write DATA
  output.write((char*) &cDATA, 4);
  SubLength = 12; //fixed length of 12 bytes
  //write DATA's length
  output.write((char*) &SubLength, 4);
  //write cell data
  output.write((char*) &CellFlags, 4);
  output.write((char*) &GridX, 4);
  output.write((char*) &GridY, 4);

  if (RegionID!="")
  {
    //write RGNN
    output.write((char*) &cRGNN, 4);
    SubLength = RegionID.length()+1;
    //write RGNN's length
    output.write((char*) &SubLength, 4);
    //write region ID
    output.write(RegionID.c_str(), SubLength);
  }

  if (!isInterior())
  {
    //write NAM5
    output.write((char*) &cNAM5, 4);
    SubLength = 4; //fixed length of four bytes
    //write NAM5's length
    output.write((char*) &SubLength, 4);
    //write colour ref
    output.write((char*) &ColourRef, 4);
  }
  else
  {
    if (hasWHGT)
    {
      //write WHGT
      output.write((char*) &cWHGT, 4);
      SubLength = 4; //fixed length of four bytes
      //write WHGT's length
      output.write((char*) &SubLength, 4);
      //write water level
      output.write((char*) &WaterHeight, 4);
    }
    if (Ambience.isPresent)
    {
      //write AMBI
      output.write((char*) &cAMBI, 4);
      SubLength = 16; //fixed length of 16 bytes
      //write AMBI's length
      output.write((char*) &SubLength, 4);
      //write ambience data
      output.write((char*) &Ambience.AmbientColour, 4);
      output.write((char*) &Ambience.SunlightColour, 4);
      output.write((char*) &Ambience.FogColour, 4);
      output.write((char*) &Ambience.FogDensity, 4);
    }
  }

  if (NumReferences!=0)
  {
    //write NAM0
    output.write((char*) &cNAM0, 4);
    SubLength = 4; //fixed length of four bytes
    //write NAM0's length
    output.write((char*) &SubLength, 4);
    //write number of references
    output.write((char*) &NumReferences, 4);
  }

  for (i=0; i<References.size(); ++i)
  {
    if (!References[i].saveToStream(output))
    {
      std::cout << "Error while writing reference data of CELL record.\n";
      return false;
    }
  }//for

  return output.good();
}

bool CellRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Cells:
    NAME = Cell ID string. Can be an empty string for exterior cells in which case
        the region name is used instead.
    DATA = Cell Data
        long Flags
            0x01 = Interior?
            0x02 = Has Water
            0x04 = Illegal to Sleep here
            0x80 = Behave like exterior (Tribunal)
        long GridX
        long GridY
    RGNN = Region name string
    NAM0 = Number of objects in cell in current file? (4 byte, long), Optional

    Exterior Cell Sub-Records
        NAM5 = Map Color (4 bytes, long, COLORREF)

    Interior Cell Sub-Records
        WHGT = Water Height (4 bytes, float)
        AMBI = Ambient Light Level (16 bytes)
            long AmbientColor
            long SunlightColor
            long FogColor
            float FogDensity

    Referenced Object Data Grouping
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
		INTV = Number of uses ( 4 bytes, long, 1 default), occurs even for objects that don't use it
		NAM9 = ? (4 bytes, long, 0x00000001)
		XSOL = Soul Extra Data (ID string of creature)
		DATA = Ref Position Data (24 bytes)
			float XPos
			float YPos
			float ZPos
			float XRotate
			float YRotate
			float ZRotate
  */

  int32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: subrecord NAME of CELL is longer than 255 characters.\n";
    return false;
  }
  //read cell name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of CELL!\n";
    return false;
  }
  CellID = std::string(Buffer);

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=12)
  {
    std::cout <<"Error: sub record DATA of CELL has invalid length ("<<SubLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read cell data
  in_File.read((char*) &CellFlags, 4);
  in_File.read((char*) &GridX, 4);
  in_File.read((char*) &GridY, 4);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of CELL!\n";
    return false;
  }

  RegionID = "";
  NumReferences = 0;
  References.clear();
  ColourRef = 0;
  ReferencedObject newRef;

  bool hasNAM0 = false;
  bool hasNAM5 = false;
  bool hasRGNN = false;
  Ambience.isPresent = false;
  hasWHGT = false;
  while (BytesRead<Size)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cFRMR:
           if (!newRef.loadFromStream(in_File, BytesRead, Buffer))
           {
             std::cout << "Error while reading reference data of CELL!\n";
             return false;
           }
           References.push_back(newRef);
           break;
      case cRGNN:
           if (hasRGNN)
           {
             std::cout << "Error: record CELL seems to have two RGNN subrecords.\n";
             return false;
           }
           //RGNN's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord RGNN of CELL is longer than 255 characters.\n";
             return false;
           }
           //read region name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord RGNN of CELL!\n";
             return false;
           }
           RegionID = std::string(Buffer);
           hasRGNN = true;
           break;
      case cWHGT:
           if (hasWHGT)
           {
             std::cout << "Error: record CELL seems to have two WHGT subrecords.\n";
             return false;
           }
           //WHGT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord WHGT of CELL has invalid length ("<<SubLength
                       << " bytes). Should be four bytes.\n";
             return false;
           }
           //read water height
           in_File.read((char*) &WaterHeight, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord WHGT of CELL!\n";
             return false;
           }
           hasWHGT = true;
           break;
      case cAMBI:
           if (Ambience.isPresent)
           {
             std::cout << "Error: record CELL seems to have two AMBI subrecords.\n";
             return false;
           }
           //AMBI's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=16)
           {
             std::cout << "Error: subrecord AMBI of CELL has invalid length ("<<SubLength
                       << " bytes). Should be 16 bytes.\n";
             return false;
           }
           //read ambient light data
           in_File.read((char*) &Ambience.AmbientColour, 4);
           in_File.read((char*) &Ambience.SunlightColour, 4);
           in_File.read((char*) &Ambience.FogColour, 4);
           in_File.read((char*) &Ambience.FogDensity, 4);
           BytesRead += 16;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord AMBI of CELL!\n";
             return false;
           }
           Ambience.isPresent = true;
           break;
      case cNAM0:
           if (hasNAM0)
           {
             std::cout << "Error: record CELL seems to have two NAM0 subrecords.\n";
             return false;
           }
           //NAM0's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord NAM0 of CELL has invalid length ("<<SubLength
                       << " bytes). Should be four bytes.\n";
             return false;
           }
           //read number of objects in cell
           in_File.read((char*) &NumReferences, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM0 of CELL!\n";
             return false;
           }
           hasNAM0 = true;
           break;
      case cNAM5:
           if (hasNAM5)
           {
             std::cout << "Error: record CELL seems to have two NAM5 subrecords.\n";
             return false;
           }
           //NAM5's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord NAM5 of CELL has invalid length ("<<SubLength
                       << " bytes). Should be four bytes.\n";
             return false;
           }
           //read map color reference
           in_File.read((char*) &ColourRef, 4);
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM5 of CELL!\n";
             return false;
           }
           hasNAM5 = true;
           break;
      default:
           std::cout << "Error while reading CELL: expected record name FRMR, "
                     << "RGNN, WHGT, AMBI, NAM0 or NAM5 not found. Instead, \""
                     << IntTo4Char(SubRecName)<<"\" was found.\n";
           return false;
           break;
    }//swi
  }//while

  while (BytesRead<Size)
  {
    ReferencedObject newRef;
    if (!newRef.loadFromStream(in_File, BytesRead, Buffer))
    {
      std::cout << "Error while reading reference data of CELL!\n";
      return false;
    }
    References.push_back(newRef);
  }//while

  return (in_File.good() and (BytesRead>=Size));
}

bool CellRecord::isInterior() const
{
  return ((CellFlags & cfInterior)!=0);
}

bool CellRecord::hasWater() const
{
  return ((CellFlags & cfWater)!=0);
}
