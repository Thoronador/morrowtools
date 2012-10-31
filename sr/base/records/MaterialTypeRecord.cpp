/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "MaterialTypeRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//constants for flags
const uint32_t MaterialTypeRecord::cFlagStairMaterial = 0x00000001;
const uint32_t MaterialTypeRecord::cFlagArrowsStick   = 0x00000002;

MaterialTypeRecord::MaterialTypeRecord()
: BasicRecord()
{
  editorID = "";
  materialName = "";
  parentMaterialFormID = 0;
  //subrecord CNAM
  havokDisplayColorR = 0.0f;
  havokDisplayColorG = 0.0f;
  havokDisplayColorB = 0.0f;
  //end of subrecord CNAM
  bouyancy = 0.0f; //subrecord BNAM
  flags = 0; //subrecord FNAM
  havokImpactDataSetID = 0; //subrecord HNAM
}

MaterialTypeRecord::~MaterialTypeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool MaterialTypeRecord::equals(const MaterialTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (materialName==other.materialName) and (parentMaterialFormID==other.parentMaterialFormID)
      and (havokDisplayColorR==other.havokDisplayColorR)
      and (havokDisplayColorG==other.havokDisplayColorG)
      and (havokDisplayColorB==other.havokDisplayColorB)
      and (bouyancy==other.bouyancy)
      and (flags==other.flags)
      and (havokImpactDataSetID==other.havokImpactDataSetID));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t MaterialTypeRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
             +editorID.length()+1 /* length of name +1 byte for NUL termination */
             +4 /* MNAM */ +2 /* 2 bytes for length */
             +materialName.length()+1 /* length of name +1 byte for NUL termination */
             +4 /* CNAM */ +2 /* 2 bytes for length */ +12 /* fixed size of 12 bytes */
             +4 /* BNAM */ +2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */
             +4 /* FNAM */ +2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */;
  if (parentMaterialFormID!=0)
    writeSize = writeSize +4 /* PNAM */ +2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */;
  if (havokImpactDataSetID!=0)
    writeSize = writeSize +4 /* HNAM */ +2 /* 2 bytes for length */ + 4 /* fixed size of four bytes */;

  return writeSize;
}

bool MaterialTypeRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cMATT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (parentMaterialFormID!=0)
  {
    //write PNAM
    output.write((const char*) &cPNAM, 4);
    //PNAM's length
    subLength = 4; /* fixed */
    output.write((const char*) &subLength, 2);
    //write parent material's form ID
    output.write((const char*) &parentMaterialFormID, 4);
  }//if PNAM

  //write MNAM
  output.write((const char*) &cMNAM, 4);
  //MNAM's length
  subLength = materialName.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(materialName.c_str(), subLength);

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 12; /* fixed */
  output.write((const char*) &subLength, 2);
  //write havok display colour components
  output.write((const char*) &havokDisplayColorR, 4);
  output.write((const char*) &havokDisplayColorG, 4);
  output.write((const char*) &havokDisplayColorB, 4);

  //write BNAM
  output.write((const char*) &cBNAM, 4);
  //BNAM's length
  subLength = 4; /* fixed */
  output.write((const char*) &subLength, 2);
  //write bouyancy
  output.write((const char*) &bouyancy, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 4; /* fixed */
  output.write((const char*) &subLength, 2);
  //write flags
  output.write((const char*) &flags, 4);

  if (havokImpactDataSetID!=0)
  {
    //write HNAM
    output.write((const char*) &cHNAM, 4);
    //HNAM's length
    subLength = 4; /* fixed */
    output.write((const char*) &subLength, 2);
    //write havok impact data set's form ID
    output.write((const char*) &havokImpactDataSetID, 4);
  }//if HNAM

  return output.good();
}
#endif

bool MaterialTypeRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  if (isDeleted()) return true;
  uint32_t subRecName;
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
    std::cout <<"Error: sub record EDID of MATT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of MATT!\n";
    return false;
  }
  editorID = std::string(buffer);

  parentMaterialFormID = 0;
  materialName.clear();
  bool hasReadCNAM = false;
  bool hasReadBNAM = false; bouyancy = 0.0f;
  bool hasReadFNAM = false;
  havokImpactDataSetID = 0;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cPNAM:
           if (parentMaterialFormID!=0)
           {
             std::cout << "Error: MATT seems to have more than one PNAM subrecord!\n";
             return false;
           }
           //load PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, parentMaterialFormID, false))
           {
             return false;
           }
           bytesRead += 6;
           //check value
           if (parentMaterialFormID==0)
           {
             std::cout << "Error: subrecord PNAM of MATT has value zero!\n";
             return false;
           }
           break;
      case cMNAM:
           if (!materialName.empty())
           {
             std::cout << "Error: MATT seems to have more than one MNAM subrecord!\n";
             return false;
           }
           //read MNAM
           if (!loadString512FromStream(in_File, materialName, buffer, cMNAM, false, bytesRead))
           {
             return false;
           }
           if (materialName.empty())
           {
             std::cout << "Error: subrecord MNAM of MATT is empty!\n";
             return false;
           }
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: MATT seems to have more than one CNAM subrecord!\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout << "Error: sub record CNAM of MATT has invalid length ("
                       << subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //now load CNAM
           in_File.read((char*) &havokDisplayColorR, 4);
           in_File.read((char*) &havokDisplayColorG, 4);
           in_File.read((char*) &havokDisplayColorB, 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of MATT!\n";
             return false;
           }
           //integrity check
           if ((havokDisplayColorR>1.0f) or (havokDisplayColorR<0.0f))
           {
             std::cout << "Error while reading subrecord CNAM of MATT: red "
                       << "havok display colour component is not in range "
                       << "[0;1]! Its current value is "<<havokDisplayColorR<<".\n";
             return false;
           }
           if ((havokDisplayColorG>1.0f) or (havokDisplayColorG<0.0f))
           {
             std::cout << "Error while reading subrecord CNAM of MATT: green "
                       << "havok display colour component is not in range "
                       << "[0;1]! Its current value is "<<havokDisplayColorG<<".\n";
             return false;
           }
           if ((havokDisplayColorB>1.0f) or (havokDisplayColorB<0.0f))
           {
             std::cout << "Error while reading subrecord CNAM of MATT: blue "
                       << "havok display colour component is not in range "
                       << "[0;1]! Its current value is "<<havokDisplayColorB<<".\n";
             return false;
           }
           hasReadCNAM = true;
           break;
      case cBNAM:
           if (hasReadBNAM)
           {
             std::cout << "Error: MATT seems to have more than one BNAM subrecord!\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout << "Error: sub record BNAM of MATT has invalid length ("
                       << subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //now load BNAM
           in_File.read((char*) &bouyancy, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of MATT!\n";
             return false;
           }
           hasReadBNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: MATT seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //load FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, flags, false))
           {
             return false;
           }
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      case cHNAM:
           if (havokImpactDataSetID!=0)
           {
             std::cout << "Error: MATT seems to have more than one HNAM subrecord!\n";
             return false;
           }
           //HNAM's length
           if (!loadUint32SubRecordFromStream(in_File, cHNAM, havokImpactDataSetID, false))
           {
             return false;
           }
           bytesRead += 6;
           //check value
           if (havokImpactDataSetID==0)
           {
             std::cout << "Error: subrecord HNAM of MATT has value zero!\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MNAM, CNAM, BNAM or HNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(!materialName.empty() and hasReadCNAM and hasReadBNAM and hasReadFNAM))
  {
    std::cout << "Error: At least one required subrecord of MATT is missing!\n";
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

}//namespace
