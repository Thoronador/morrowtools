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
#include <limits>
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

const uint32_t cfInterior = 0x00000001;
const uint32_t cfWater = 0x00000002;

CellRecord::CellRecord()
: BasicRecord(),
  CellID(""),
  // cell data
  CellFlags(0),
  GridX(0),
  GridY(0),
  // end of cell data
  RegionID(""),
  NumReferences(0),
  ColourRef(0),
  WaterHeight(std::nullopt),
  Ambience(AmbientLight()),
  ReferencesPersistent(std::vector<ReferencedObject>()),
  ReferencesOther(std::vector<ReferencedObject>())
{
  Ambience.isPresent = false;
}

bool CellRecord::equals(const CellRecord& other) const
{
  return (CellID == other.CellID) && (CellFlags == other.CellFlags)
      && (GridX == other.GridX) && (GridY == other.GridY)
      && (RegionID == other.RegionID) && (ReferencesPersistent == other.ReferencesPersistent)
      && (NumReferences == other.NumReferences)
      && (ReferencesOther == other.ReferencesOther)
      && (ColourRef == other.ColourRef) && (Ambience == other.Ambience)
      && (WaterHeight == other.WaterHeight);
}

#ifndef MW_UNSAVEABLE_RECORDS
uint32_t CellRecord::getWriteSize() const
{
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + CellID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* DATA */ + 4 /* 4 bytes for length */ + 12 /* fixed length of 12 bytes */;
  if (!RegionID.empty())
  {
    Size += 4 /* RGNN */ + 4 /* 4 bytes for length */
         + RegionID.length() + 1 /* length of model path +1 byte for NUL */;
  }
  if (!isInterior())
  {
    Size += 4 /* NAM5 */ + 4 /* 4 bytes for length */
          + 4 /* fixed length of 4 bytes */;
  }
  else
  {
    // WHGT and AMBI is for interior cells only
    if (WaterHeight.has_value())
    {
      Size += 4 /* WHGT */ + 4 /* 4 bytes for length */
            + 4 /* fixed length of 4 bytes */;
    }
    if (Ambience.isPresent)
    {
      Size += 4 /* AMBI */ + 4 /* 4 bytes for length */
            + 16 /* fixed length of 16 bytes */;
    }
  }

  for (const auto& ref_elem: ReferencesPersistent)
  {
    Size += ref_elem.getWrittenSize();
  }
  if (NumReferences != 0)
  {
    Size += 4 /* NAM0 */ + 4 /* 4 bytes for length */
          + 4 /* fixed length of 4 bytes */;
  }
  for (const auto& ref_elem: ReferencesOther)
  {
    Size += ref_elem.getWrittenSize();
  }
  return Size;
}

bool CellRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cCELL), 4);
  const uint32_t Size = getWriteSize();
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write cell name (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = CellID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(CellID.c_str(), SubLength);

  // write cell data (DATA)
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  SubLength = 12;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&CellFlags), 4);
  output.write(reinterpret_cast<const char*>(&GridX), 4);
  output.write(reinterpret_cast<const char*>(&GridY), 4);

  if (!RegionID.empty())
  {
    // write region ID (RGNN)
    output.write(reinterpret_cast<const char*>(&cRGNN), 4);
    SubLength = RegionID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(RegionID.c_str(), SubLength);
  }

  if (!isInterior())
  {
    // write colour reference (NAM5)
    output.write(reinterpret_cast<const char*>(&cNAM5), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&ColourRef), 4);
  }
  else
  {
    if (WaterHeight.has_value())
    {
      // write water level (WHGT)
      output.write(reinterpret_cast<const char*>(&cWHGT), 4);
      SubLength = 4;
      output.write(reinterpret_cast<const char*>(&SubLength), 4);
      output.write(reinterpret_cast<const char*>(&WaterHeight.value()), 4);
    }
    if (Ambience.isPresent)
    {
      // write ambience data (AMBI)
      output.write(reinterpret_cast<const char*>(&cAMBI), 4);
      SubLength = 16;
      output.write(reinterpret_cast<const char*>(&SubLength), 4);
      output.write(reinterpret_cast<const char*>(&Ambience.AmbientColour), 4);
      output.write(reinterpret_cast<const char*>(&Ambience.SunlightColour), 4);
      output.write(reinterpret_cast<const char*>(&Ambience.FogColour), 4);
      output.write(reinterpret_cast<const char*>(&Ambience.FogDensity), 4);
    }
  }

  for (const auto& ref_elem: ReferencesPersistent)
  {
    if (!ref_elem.saveToStream(output))
    {
      std::cerr << "Error while writing reference data of CELL record.\n";
      return false;
    }
  }

  if (NumReferences != 0)
  {
    // write next reference count / number of references (NAM0)
    output.write(reinterpret_cast<const char*>(&cNAM0), 4);
    SubLength = 4;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&NumReferences), 4);
  }

  for (const auto& ref_elem: ReferencesOther)
  {
    if (!ref_elem.saveToStream(output))
    {
      std::cerr << "Error while writing reference data of CELL record.\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool CellRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  uint32_t BytesRead = 0;

  // read cell name (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, CellID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading subrecord NAME of CELL!\n";
    return false;
  }

  // read DATA
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  // DATA's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 12)
  {
    std::cerr << "Error: Sub record DATA of CELL has invalid length ("
              << SubLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read cell data
  input.read(reinterpret_cast<char*>(&CellFlags), 4);
  input.read(reinterpret_cast<char*>(&GridX), 4);
  input.read(reinterpret_cast<char*>(&GridY), 4);
  BytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord DATA of CELL!\n";
    return false;
  }

  RegionID = "";
  NumReferences = 0;
  ReferencesPersistent.clear();
  ReferencesOther.clear();
  ColourRef = 0;
  ReferencedObject newRef;

  bool hasNAM0 = false;
  bool hasNAM5 = false;
  bool hasRGNN = false;
  bool hasUnknownINTV = false;
  Ambience.isPresent = false;
  WaterHeight.reset();
  bool refIsPersistent = true;
  while (BytesRead < Size)
  {
    // read next subrecord
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cFRMR:
           if (!newRef.loadFromStream(input, BytesRead, Buffer))
           {
             std::cerr << "Error while reading reference data of CELL!\n";
             return false;
           }
           if (refIsPersistent)
             ReferencesPersistent.emplace_back(newRef);
           else
             ReferencesOther.emplace_back(newRef);
           break;
      case cRGNN:
           if (hasRGNN)
           {
             std::cerr << "Error: Record CELL seems to have two RGNN subrecords.\n";
             return false;
           }
           // read region name (RGNN)
           if (!loadString256(input, RegionID, Buffer, cRGNN, BytesRead))
           {
             std::cerr << "Error while reading subrecord RGNN of CELL!\n";
             return false;
           }
           hasRGNN = true;
           break;
      case cWHGT:
           if (WaterHeight.has_value())
           {
             std::cerr << "Error: Record CELL seems to have two WHGT subrecords.\n";
             return false;
           }
           // WHGT's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord WHGT of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read water height
           WaterHeight = std::numeric_limits<float>::quiet_NaN();
           input.read(reinterpret_cast<char*>(&WaterHeight), 4);
           BytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord WHGT of CELL!\n";
             return false;
           }
           break;
      case cAMBI:
           if (Ambience.isPresent)
           {
             std::cerr << "Error: Record CELL seems to have two AMBI sub records.\n";
             return false;
           }
           // AMBI's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 16)
           {
             std::cerr << "Error: Subrecord AMBI of CELL has invalid length ("
                       << SubLength << " bytes). Should be 16 bytes.\n";
             return false;
           }
           // read ambient light data
           input.read(reinterpret_cast<char*>(&Ambience.AmbientColour), 4);
           input.read(reinterpret_cast<char*>(&Ambience.SunlightColour), 4);
           input.read(reinterpret_cast<char*>(&Ambience.FogColour), 4);
           input.read(reinterpret_cast<char*>(&Ambience.FogDensity), 4);
           BytesRead += 16;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord AMBI of CELL!\n";
             return false;
           }
           Ambience.isPresent = true;
           break;
      case cNAM0:
           if (hasNAM0)
           {
             std::cerr << "Error: Record CELL seems to have two NAM0 subrecords.\n";
             return false;
           }
           // NAM0's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord NAM0 of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           //read number of objects in cell
           input.read(reinterpret_cast<char*>(&NumReferences), 4);
           BytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord NAM0 of CELL!\n";
             return false;
           }
           hasNAM0 = true;
           refIsPersistent = false;
           break;
      case cNAM5:
           if (hasNAM5)
           {
             std::cerr << "Error: Record CELL seems to have two NAM5 subrecords.\n";
             return false;
           }
           // NAM5's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord NAM5 of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read map color reference
           input.read(reinterpret_cast<char*>(&ColourRef), 4);
           BytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord NAM5 of CELL!\n";
             return false;
           }
           hasNAM5 = true;
           break;
      case cINTV: //Somehow some of the English plugin files seem to have an
                  //  INTV record after DATA and before AMBI, but I can't see
                  //  what value they represent in game, so we just skip it.
           if (hasUnknownINTV)
           {
             std::cerr << "Error: Record CELL seems to have two INTV subrecords.\n";
             return false;
           }
           //INTV's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Subrecord INTV of CELL has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           // read int value
           input.read(reinterpret_cast<char*>(&SubLength), 4); // We don't actually use this value.
           BytesRead += 4;
           if (!input.good())
           {
             std::cerr << "Error while reading subrecord INTV of CELL!\n";
             return false;
           }
           hasUnknownINTV = true;
           break;
      default:
           std::cerr << "Error while reading CELL: Expected record name FRMR, "
                     << "RGNN, WHGT, AMBI, NAM0, NAM5 or INTV not found. Instead, \""
                     << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
           break;
    }
  }

  return input.good();
}

bool CellRecord::isInterior() const
{
  return (CellFlags & cfInterior) != 0;
}

bool CellRecord::hasWater() const
{
  return (CellFlags & cfWater) != 0;
}

} // namespace
