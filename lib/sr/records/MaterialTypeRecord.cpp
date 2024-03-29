/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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

#include "MaterialTypeRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

// constants for flags
const uint32_t MaterialTypeRecord::cFlagStairMaterial = 0x00000001;
const uint32_t MaterialTypeRecord::cFlagArrowsStick   = 0x00000002;

MaterialTypeRecord::MaterialTypeRecord()
: BasicRecord(), editorID(""),
  materialName(""),
  parentMaterialFormID(0),
  // subrecord CNAM
  havokDisplayColorR(0.0f),
  havokDisplayColorG(0.0f),
  havokDisplayColorB(0.0f),
  // end of subrecord CNAM
  buoyancy(0.0f), // subrecord BNAM
  flags(0), // subrecord FNAM
  havokImpactDataSetID(0) // subrecord HNAM
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool MaterialTypeRecord::equals(const MaterialTypeRecord& other) const
{
  return ((equalsBasic(other)) && (editorID == other.editorID)
      && (materialName == other.materialName) && (parentMaterialFormID == other.parentMaterialFormID)
      && (havokDisplayColorR == other.havokDisplayColorR)
      && (havokDisplayColorG == other.havokDisplayColorG)
      && (havokDisplayColorB == other.havokDisplayColorB)
      && (buoyancy == other.buoyancy)
      && (flags == other.flags)
      && (havokImpactDataSetID == other.havokImpactDataSetID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MaterialTypeRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
             + editorID.length() + 1 /* length of name +1 byte for NUL termination */
             + 4 /* MNAM */ + 2 /* 2 bytes for length */
             + materialName.length() + 1 /* length of name +1 byte for NUL termination */
             + 4 /* CNAM */ + 2 /* 2 bytes for length */ + 12 /* fixed size of 12 bytes */
             + 4 /* BNAM */ + 2 /* 2 bytes for length */ +  4 /* fixed size of four bytes */
             + 4 /* FNAM */ + 2 /* 2 bytes for length */ +  4 /* fixed size of four bytes */;
  if (parentMaterialFormID != 0)
    writeSize = writeSize + 4 /* PNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */;
  if (havokImpactDataSetID != 0)
    writeSize = writeSize + 4 /* HNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */;

  return writeSize;
}

bool MaterialTypeRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cMATT), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  // write EDID
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  // EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write editor ID
  output.write(editorID.c_str(), subLength);

  if (parentMaterialFormID != 0)
  {
    // write PNAM
    output.write(reinterpret_cast<const char*>(&cPNAM), 4);
    // PNAM's length
    subLength = 4; /* fixed */
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write parent material's form ID
    output.write(reinterpret_cast<const char*>(&parentMaterialFormID), 4);
  } // if PNAM

  // write MNAM
  output.write(reinterpret_cast<const char*>(&cMNAM), 4);
  // MNAM's length
  subLength = materialName.length()+1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write model path
  output.write(materialName.c_str(), subLength);

  // write CNAM
  output.write(reinterpret_cast<const char*>(&cCNAM), 4);
  // CNAM's length
  subLength = 12; /* fixed */
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write havok display colour components
  output.write(reinterpret_cast<const char*>(&havokDisplayColorR), 4);
  output.write(reinterpret_cast<const char*>(&havokDisplayColorG), 4);
  output.write(reinterpret_cast<const char*>(&havokDisplayColorB), 4);

  // write BNAM
  output.write(reinterpret_cast<const char*>(&cBNAM), 4);
  // BNAM's length
  subLength = 4; /* fixed */
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write buoyancy
  output.write(reinterpret_cast<const char*>(&buoyancy), 4);

  // write FNAM
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  // FNAM's length
  subLength = 4; /* fixed */
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write flags
  output.write(reinterpret_cast<const char*>(&flags), 4);

  if (havokImpactDataSetID != 0)
  {
    // write HNAM
    output.write(reinterpret_cast<const char*>(&cHNAM), 4);
    // HNAM's length
    subLength = 4; /* fixed */
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write havok impact data set's form ID
    output.write(reinterpret_cast<const char*>(&havokImpactDataSetID), 4);
  } // if HNAM

  return output.good();
}
#endif

bool MaterialTypeRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;

  // read EDID
  in_File.read(reinterpret_cast<char*>(&subRecName), 4);
  uint32_t bytesRead = 4;
  if (subRecName != cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  // EDID's length
  in_File.read(reinterpret_cast<char*>(&subLength), 2);
  bytesRead += 2;
  if (subLength > 511)
  {
    std::cerr << "Error: sub record EDID of MATT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of MATT!\n";
    return false;
  }
  editorID = std::string(buffer);

  parentMaterialFormID = 0;
  materialName.clear();
  bool hasReadCNAM = false;
  bool hasReadBNAM = false;
  buoyancy = 0.0f;
  bool hasReadFNAM = false;
  havokImpactDataSetID = 0;
  while (bytesRead < readSize)
  {
    // read next subrecord's name
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cPNAM:
           if (parentMaterialFormID != 0)
           {
             std::cerr << "Error: MATT seems to have more than one PNAM subrecord!\n";
             return false;
           }
           // load PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, parentMaterialFormID, false))
           {
             return false;
           }
           bytesRead += 6;
           // check value
           if (parentMaterialFormID == 0)
           {
             std::cerr << "Error: subrecord PNAM of MATT has value zero!\n";
             return false;
           }
           break;
      case cMNAM:
           if (!materialName.empty())
           {
             std::cerr << "Error: MATT seems to have more than one MNAM subrecord!\n";
             return false;
           }
           // read MNAM
           if (!loadString512FromStream(in_File, materialName, buffer, cMNAM, false, bytesRead))
           {
             return false;
           }
           if (materialName.empty())
           {
             std::cerr << "Error: subrecord MNAM of MATT is empty!\n";
             return false;
           }
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cerr << "Error: MATT seems to have more than one CNAM subrecord!\n";
             return false;
           }
           // CNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: sub record CNAM of MATT has invalid length ("
                       << subLength << " bytes). Should be 12 bytes!\n";
             return false;
           }
           // now load CNAM
           in_File.read(reinterpret_cast<char*>(&havokDisplayColorR), 4);
           in_File.read(reinterpret_cast<char*>(&havokDisplayColorG), 4);
           in_File.read(reinterpret_cast<char*>(&havokDisplayColorB), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord CNAM of MATT!\n";
             return false;
           }
           // integrity check
           if ((havokDisplayColorR > 1.0f) || (havokDisplayColorR < 0.0f))
           {
             std::cerr << "Error while reading subrecord CNAM of MATT: red "
                       << "havok display colour component is not in range "
                       << "[0;1]! Its current value is " << havokDisplayColorR
                       << ".\n";
             return false;
           }
           if ((havokDisplayColorG > 1.0f) || (havokDisplayColorG < 0.0f))
           {
             std::cerr << "Error while reading subrecord CNAM of MATT: green "
                       << "havok display colour component is not in range "
                       << "[0;1]! Its current value is " << havokDisplayColorG
                       << ".\n";
             return false;
           }
           if ((havokDisplayColorB > 1.0f) || (havokDisplayColorB < 0.0f))
           {
             std::cerr << "Error while reading subrecord CNAM of MATT: blue "
                       << "havok display colour component is not in range "
                       << "[0;1]! Its current value is " << havokDisplayColorB
                       << ".\n";
             return false;
           }
           hasReadCNAM = true;
           break;
      case cBNAM:
           if (hasReadBNAM)
           {
             std::cerr << "Error: MATT seems to have more than one BNAM subrecord!\n";
             return false;
           }
           // BNAM's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 4)
           {
             std::cerr << "Error: sub record BNAM of MATT has invalid length ("
                       << subLength << " bytes). Should be four bytes!\n";
             return false;
           }
           // now load BNAM
           in_File.read(reinterpret_cast<char*>(&buoyancy), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord BNAM of MATT!\n";
             return false;
           }
           hasReadBNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: MATT seems to have more than one FNAM subrecord!\n";
             return false;
           }
           // load FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, flags, false))
           {
             return false;
           }
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      case cHNAM:
           if (havokImpactDataSetID != 0)
           {
             std::cerr << "Error: MATT seems to have more than one HNAM subrecord!\n";
             return false;
           }
           // HNAM's length
           if (!loadUint32SubRecordFromStream(in_File, cHNAM, havokImpactDataSetID, false))
           {
             return false;
           }
           bytesRead += 6;
           // check value
           if (havokImpactDataSetID == 0)
           {
             std::cerr << "Error: subrecord HNAM of MATT has value zero!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: unexpected record type \"" << IntTo4Char(subRecName)
                     << "\" found, but only MNAM, CNAM, BNAM or HNAM are allowed here!\n";
           return false;
           break;
    } // swi
  } // while

  // presence checks
  if (!(!materialName.empty() && hasReadCNAM && hasReadBNAM && hasReadFNAM))
  {
    std::cerr << "Error: At least one required subrecord of MATT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t MaterialTypeRecord::getRecordType() const
{
  return cMATT;
}

bool MaterialTypeRecord::arrowsStick() const
{
  return ((cFlagArrowsStick & flags) != 0);
}

bool MaterialTypeRecord::isStairMaterial() const
{
  return ((cFlagStairMaterial & flags) != 0);
}

} // namespace
