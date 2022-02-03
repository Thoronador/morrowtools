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

#include "CellRecord.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

/* AmbientLight's functions */

AmbientLight::AmbientLight()
: isPresent(false),
  AmbientColour(0),
  SunlightColour(0),
  FogColour(0),
  FogDensity(0.0f)
{
}

bool AmbientLight::operator==(const AmbientLight& other) const
{
  if (isPresent ^ other.isPresent)
    return false;
  if (isPresent)
  {
    return (AmbientColour == other.AmbientColour)
        && (SunlightColour == other.SunlightColour)
        && (FogColour == other.FogColour) && (FogDensity == other.FogDensity);
  }
  return true;
}

/* **** CellRecord functions ****/

const uint32_t cfInterior = 1;
const uint32_t cfWater = 2;

CellRecord::CellRecord()
: BasicRecord(),
  CellID(""),
  //cell data
  CellFlags(0),
  GridX(0),
  GridY(0),
  //end of cell data
  RegionID(""),
  NumReferences(0),
  ColourRef(0),
  hasWHGT(false),
  WaterHeight(0.0f),
  Ambience(AmbientLight()),
  References(std::vector<ReferencedObject>())
{
  Ambience.isPresent = false;
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

#ifndef MW_UNSAVEABLE_RECORDS
bool CellRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cCELL, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +CellID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* DATA */ +4 /* 4 bytes for length */ +12 /* fixed length of 12 bytes */;
  if (!RegionID.empty())
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

  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

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
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = CellID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write cell name
  output.write(CellID.c_str(), SubLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  SubLength = 12; //fixed length of 12 bytes
  //write DATA's length
  output.write((const char*) &SubLength, 4);
  //write cell data
  output.write((const char*) &CellFlags, 4);
  output.write((const char*) &GridX, 4);
  output.write((const char*) &GridY, 4);

  if (!RegionID.empty())
  {
    //write RGNN
    output.write((const char*) &cRGNN, 4);
    SubLength = RegionID.length()+1;
    //write RGNN's length
    output.write((const char*) &SubLength, 4);
    //write region ID
    output.write(RegionID.c_str(), SubLength);
  }

  if (!isInterior())
  {
    //write NAM5
    output.write((const char*) &cNAM5, 4);
    SubLength = 4; //fixed length of four bytes
    //write NAM5's length
    output.write((const char*) &SubLength, 4);
    //write colour ref
    output.write((const char*) &ColourRef, 4);
  }
  else
  {
    if (hasWHGT)
    {
      //write WHGT
      output.write((const char*) &cWHGT, 4);
      SubLength = 4; //fixed length of four bytes
      //write WHGT's length
      output.write((const char*) &SubLength, 4);
      //write water level
      output.write((const char*) &WaterHeight, 4);
    }
    if (Ambience.isPresent)
    {
      //write AMBI
      output.write((const char*) &cAMBI, 4);
      SubLength = 16; //fixed length of 16 bytes
      //write AMBI's length
      output.write((const char*) &SubLength, 4);
      //write ambience data
      output.write((const char*) &Ambience.AmbientColour, 4);
      output.write((const char*) &Ambience.SunlightColour, 4);
      output.write((const char*) &Ambience.FogColour, 4);
      output.write((const char*) &Ambience.FogDensity, 4);
    }
  }

  if (NumReferences!=0)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    SubLength = 4; //fixed length of four bytes
    //write NAM0's length
    output.write((const char*) &SubLength, 4);
    //write number of references
    output.write((const char*) &NumReferences, 4);
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
#endif

bool CellRecord::loadFromStream(std::istream& in_File)
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

  uint32_t SubRecName;
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
  bool hasUnknownINTV = false;
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
      case cINTV: //Somehow some of the english plugin files seem to have an
                  //  INTV record after DATA and before AMBI, but I can't see
                  //  what value they represent in game, so we just skip it.
           if (hasUnknownINTV)
           {
             std::cout << "Error: record CELL seems to have two INTV subrecords.\n";
             return false;
           }
           //INTV's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: subrecord INTV of CELL has invalid length ("<<SubLength
                       << " bytes). Should be four bytes.\n";
             return false;
           }
           //read int value
           in_File.read((char*) &SubLength, 4);//we don't actually use this value
           BytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord INTV of CELL!\n";
             return false;
           }
           hasUnknownINTV = true;
           break;
      default:
           std::cout << "Error while reading CELL: expected record name FRMR, "
                     << "RGNN, WHGT, AMBI, NAM0, NAM5 or INTV not found. Instead, \""
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

} //namespace
