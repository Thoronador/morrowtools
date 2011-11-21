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

#include "CraftableObjectRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>

namespace SRTP
{

CraftableObjectRecord::CraftableObjectRecord()
: BasicRecord()
{
  editorID = "";
  componentCount = 0;
  components.clear();
  unknownCTDAs.clear();
  unknownCNAM = 0;
  unknownBNAM = 0;
  resultCount = 0;
}

CraftableObjectRecord::~CraftableObjectRecord()
{
  //empty
}

int32_t CraftableObjectRecord::getRecordType() const
{
  return cCOBJ;
}

bool CraftableObjectRecord::equals(const CraftableObjectRecord& other) const
{
  if ((equalsBasic(other)) and (editorID==other.editorID)
    and (componentCount==other.componentCount) and (components.size()==other.components.size())
    and (unknownCTDAs.size()==other.unknownCTDAs.size()) and (unknownBNAM==other.unknownBNAM)
    and (unknownCNAM==other.unknownCNAM) and (resultCount==other.resultCount))
  {
    unsigned int i;
    for (i=0; i<components.size(); ++i)
    {
      if ((components[i].formID!=other.components[i].formID)
        or (components[i].count!=other.components[i].count))
      {
        return false;
      }
    }//for
    for (i=0; i<unknownCTDAs.size(); ++i)
    {
      if (memcmp(unknownCTDAs[i].content, other.unknownCTDAs[i].content, 32)!=0) return false;
    }
    return true;
  }
  return false;
}

bool CraftableObjectRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cCOBJ, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +components.size()*
          (4 /* CNTO */ +2 /* 2 bytes for length */ +8 /* fixed size */)
        +unknownCTDAs.size()*
          (4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed size */)
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* BNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* NAM1 */ +2 /* 2 bytes for length */ +2 /* fixed size */;
  if (componentCount>0)
  {
    writeSize = writeSize +4 /* COCT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (componentCount>0)
  {
    //write COCT
    output.write((char*) &cCOCT, 4);
    //COCT's length
    subLength = 4; //fixed size
    output.write((char*) &subLength, 2);
    //write component count
    output.write((char*) &componentCount, 4);
  }

  //write components
  unsigned int i;
  for (i=0; i<components.size(); ++i)
  {
    //write CNTO
    output.write((char*) &cCNTO, 4);
    //CNTO's length
    subLength = 8; //fixed size
    output.write((char*) &subLength, 2);
    //write formID
    output.write((char*) &(components[i].formID), 4);
    //write count
    output.write((char*) &(components[i].count), 4);
  }//for

  //write CTDAs
  for (i=0; i<unknownCTDAs.size(); ++i)
  {
    //write CTDA
    output.write((char*) &cCTDA, 4);
    //CTDA's length
    subLength = 32; //fixed size
    output.write((char*) &subLength, 2);
    //write content
    output.write((const char*) (unknownCTDAs[i].content), 32);
  }//for

  //write CNAM
  output.write((char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write CNAM stuff
  output.write((char*) &unknownCNAM, 4);

  //write BNAM
  output.write((char*) &cBNAM, 4);
  //BNAM's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write BNAM stuff
  output.write((char*) &unknownBNAM, 4);

  //write NAM1
  output.write((char*) &cNAM1, 4);
  //NAM1's length
  subLength = 2; //fixed size
  output.write((char*) &subLength, 2);
  //write resulting quantity
  output.write((char*) &resultCount, 2);

  return output.good();
}

bool CraftableObjectRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  uint32_t bytesRead;
  subRecName = subLength = 0;

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
    std::cout <<"Error: sub record EDID of COBJ is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of COBJ!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read COCT
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cCOCT)
  {
    //no component count?
    componentCount = 0;
    in_File.seekg(-4, std::ios_base::cur);
    bytesRead -= 4;
  }
  else
  {
    //COCT's length
    in_File.read((char*) &subLength, 2);
    bytesRead += 2;
    if (subLength!=4)
    {
      std::cout <<"Error: sub record COCT of COBJ has invalid length ("<<subLength
                <<" bytes). Should be four bytes.\n";
      return false;
    }
    //read COCT
    in_File.read((char*) &componentCount, 4);
    bytesRead += 4;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord COCT of COBJ!\n";
      return false;
    }
  }//else

  //now read the components
  uint32_t i;
  ComponentData tempCD;
  components.clear();
  for (i=0; i<componentCount; ++i)
  {
    //read CNTO
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    if (subRecName!=cCNTO)
    {
      UnexpectedRecord(cCNTO, subRecName);
      return false;
    }
    //CNTO's length
    in_File.read((char*) &subLength, 2);
    bytesRead += 2;
    if (subLength!=8)
    {
      std::cout <<"Error: sub record CNTO of COBJ has invalid length ("<<subLength
                <<" bytes). Should be 8 bytes.\n";
      return false;
    }
    //read CNTO data
    in_File.read((char*) &(tempCD.formID), 4);
    in_File.read((char*) &(tempCD.count), 4);
    bytesRead += 8;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord CNTO of COBJ!\n";
      return false;
    }
    components.push_back(tempCD);
  }//for

  //read rest in rotation method
  bool hasReadCNAM = false;
  bool hasReadBNAM = false;
  bool hasReadNAM1 = false;

  CTDAData tempCTDA;
  unknownCTDAs.clear();
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;

    switch(subRecName)
    {
      case cCTDA:
           //CTDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=32)
           {
             std::cout <<"Error: sub record CTDA of COBJ has invalid length ("<<subLength
                       <<" bytes). Should be 32 bytes.\n";
             return false;
           }
           //read CTDA data
           in_File.read((char*) &(tempCTDA.content), 32);
           bytesRead += 32;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CTDA of COBJ!\n";
             return false;
           }
           unknownCTDAs.push_back(tempCTDA);
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: COBJ seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record CNAM of COBJ has invalid length ("
                       <<subLength<<" bytes). Should be 4 bytes.\n";
             return false;
           }
           //read CNAM's stuff
           in_File.read((char*) &unknownCNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of COBJ!\n";
             return false;
           }
           hasReadCNAM = true;
           break;
      case cBNAM:
           if (hasReadBNAM)
           {
             std::cout << "Error: COBJ seems to have more than one BNAM subrecord.\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record BNAM of COBJ has invalid length ("
                       <<subLength<<" bytes). Should be 4 bytes.\n";
             return false;
           }
           //read BNAM's stuff
           in_File.read((char*) &unknownBNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of COBJ!\n";
             return false;
           }
           hasReadBNAM = true;
           break;
      case cNAM1:
           if (hasReadNAM1)
           {
             std::cout << "Error: COBJ seems to have more than one NAM1 subrecord.\n";
             return false;
           }
           //NAM1's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record NAM1 of COBJ has invalid length ("
                       <<subLength<<" bytes). Should be 2 bytes.\n";
             return false;
           }
           //read NAM1's stuff
           in_File.read((char*) &resultCount, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM1 of COBJ!\n";
             return false;
           }
           hasReadNAM1 = true;
           break;
      default:
           std::cout << "Error while reading record COBJ: Found unexpected "
                     << "subrecord type \""<<IntTo4Char(subRecName)
                     <<"\", but only BNAM, CNAM, NAM1 or CTDA are allowed!\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
