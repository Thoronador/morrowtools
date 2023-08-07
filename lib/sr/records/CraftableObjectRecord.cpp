/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "CraftableObjectRecord.hpp"
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"
#include <iostream>
#include <cstring>

namespace SRTP
{

/* CraftableObjectRecord's functions */

CraftableObjectRecord::CraftableObjectRecord()
: BasicRecord(), editorID(""),
  componentCount(0),
  components(std::vector<ComponentData>()),
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>()),
  unknownCNAM(0),
  unknownBNAM(0),
  resultCount(0)
{

}

CraftableObjectRecord::~CraftableObjectRecord()
{
  //empty
}

uint32_t CraftableObjectRecord::getRecordType() const
{
  return cCOBJ;
}

#ifndef SR_NO_RECORD_EQUALITY
bool CraftableObjectRecord::equals(const CraftableObjectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (componentCount==other.componentCount) and (components==other.components)
    and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s)
    and (unknownBNAM==other.unknownBNAM)
    and (unknownCNAM==other.unknownCNAM) and (resultCount==other.resultCount));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CraftableObjectRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +components.size()*
          (4 /* CNTO */ +2 /* 2 bytes for length */ +8 /* fixed size */)
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* BNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* NAM1 */ +2 /* 2 bytes for length */ +2 /* fixed size */;
  if (componentCount>0)
  {
    writeSize = writeSize +4 /* COCT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  unsigned int i;
  for (i=0; i<unknownCTDA_CIS2s.size(); ++i)
  {
    writeSize = writeSize + unknownCTDA_CIS2s[i].getWriteSize();
  }//for
  return writeSize;
}

bool CraftableObjectRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cCOBJ, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (componentCount>0)
  {
    //write COCT
    output.write((const char*) &cCOCT, 4);
    //COCT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write component count
    output.write((const char*) &componentCount, 4);
  }

  //write components
  unsigned int i;
  for (i=0; i<components.size(); ++i)
  {
    //write CNTO
    output.write((const char*) &cCNTO, 4);
    //CNTO's length
    subLength = 8; //fixed size
    output.write((const char*) &subLength, 2);
    //write formID
    output.write((const char*) &(components[i].formID), 4);
    //write count
    output.write((const char*) &(components[i].count), 4);
  }//for

  //write CTDAs
  for (i=0; i<unknownCTDA_CIS2s.size(); ++i)
  {
    //write CTDA
    if (!unknownCTDA_CIS2s[i].saveToStream(output))
    {
      std::cerr << "Error while writing subrecord CTDA and CIS2 of COBJ!\n";
      return false;
    }
  }//for

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write CNAM stuff
  output.write((const char*) &unknownCNAM, 4);

  //write BNAM
  output.write((const char*) &cBNAM, 4);
  //BNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write BNAM stuff
  output.write((const char*) &unknownBNAM, 4);

  //write NAM1
  output.write((const char*) &cNAM1, 4);
  //NAM1's length
  subLength = 2; //fixed size
  output.write((const char*) &subLength, 2);
  //write resulting quantity
  output.write((const char*) &resultCount, 2);

  return output.good();
}
#endif

bool CraftableObjectRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
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
    std::cerr <<"Error: sub record EDID of COBJ is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of COBJ!\n";
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
      std::cerr <<"Error: sub record COCT of COBJ has invalid length ("<<subLength
                <<" bytes). Should be four bytes.\n";
      return false;
    }
    //read COCT
    in_File.read((char*) &componentCount, 4);
    bytesRead += 4;
    if (!in_File.good())
    {
      std::cerr << "Error while reading subrecord COCT of COBJ!\n";
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
      std::cerr <<"Error: sub record CNTO of COBJ has invalid length ("<<subLength
                <<" bytes). Should be 8 bytes.\n";
      return false;
    }
    //read CNTO data
    in_File.read((char*) &(tempCD.formID), 4);
    in_File.read((char*) &(tempCD.count), 4);
    bytesRead += 8;
    if (!in_File.good())
    {
      std::cerr << "Error while reading subrecord CNTO of COBJ!\n";
      return false;
    }
    components.push_back(tempCD);
  }//for

  //read rest in rotation method
  bool hasReadCNAM = false;
  bool hasReadBNAM = false;
  bool hasReadNAM1 = false;

  CTDAData tempCTDA;
  unknownCTDA_CIS2s.clear();
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;

    switch(subRecName)
    {
      case cCTDA:
           // CTDA's content
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of COBJ!\n";
             return false;
           }
           unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      case cCIS2:
           if (unknownCTDA_CIS2s.empty())
           {
             std::cerr << "Error: found CIS2 without prior CTDA in COBJ!\n";
             return false;
           }
           if (!unknownCTDA_CIS2s.back().unknownCISx.empty())
           {
             std::cerr << "Error: COBJ seems to have more than one CIS2 subrecord per CTDA.\n";
             return false;
           }
           //read CIS2
           if (!loadString512FromStream(in_File, unknownCTDA_CIS2s.back().unknownCISx, buffer, cCIS2, false, bytesRead))
           {
             std::cerr << "Error while reading subrecord CIS2 of COBJ!\n";
             return false;
           }
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cerr << "Error: COBJ seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: sub record CNAM of COBJ has invalid length ("
                       <<subLength<<" bytes). Should be 4 bytes.\n";
             return false;
           }
           //read CNAM's stuff
           in_File.read((char*) &unknownCNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord CNAM of COBJ!\n";
             return false;
           }
           hasReadCNAM = true;
           break;
      case cBNAM:
           if (hasReadBNAM)
           {
             std::cerr << "Error: COBJ seems to have more than one BNAM subrecord.\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cerr <<"Error: sub record BNAM of COBJ has invalid length ("
                       <<subLength<<" bytes). Should be 4 bytes.\n";
             return false;
           }
           //read BNAM's stuff
           in_File.read((char*) &unknownBNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord BNAM of COBJ!\n";
             return false;
           }
           hasReadBNAM = true;
           break;
      case cNAM1:
           if (hasReadNAM1)
           {
             std::cerr << "Error: COBJ seems to have more than one NAM1 subrecord.\n";
             return false;
           }
           //NAM1's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cerr <<"Error: sub record NAM1 of COBJ has invalid length ("
                       <<subLength<<" bytes). Should be 2 bytes.\n";
             return false;
           }
           //read NAM1's stuff
           in_File.read((char*) &resultCount, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord NAM1 of COBJ!\n";
             return false;
           }
           hasReadNAM1 = true;
           break;
      default:
           std::cerr << "Error while reading record COBJ: Found unexpected "
                     << "subrecord type \""<<IntTo4Char(subRecName)
                     <<"\", but only CTDA, CIS2, BNAM, CNAM or NAM1 are allowed!\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
