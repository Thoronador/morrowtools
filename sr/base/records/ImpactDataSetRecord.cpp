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

#include "ImpactDataSetRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

bool ImpactDataSetRecord::PNAMSubrecord::operator==(const ImpactDataSetRecord::PNAMSubrecord& other) const
{
  return ((unknownOne==other.unknownOne) and (unknownTwo==other.unknownTwo));
}

ImpactDataSetRecord::ImpactDataSetRecord()
: BasicRecord(), editorID(""),
  unknownPNAMs(std::vector<PNAMSubrecord>())
{

}

ImpactDataSetRecord::~ImpactDataSetRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ImpactDataSetRecord::equals(const ImpactDataSetRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownPNAMs==other.unknownPNAMs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ImpactDataSetRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +unknownPNAMs.size()* (4 /* PNAM */ +2 /* 2 bytes for length */ +8 /* fixed length */));
}

bool ImpactDataSetRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cIPDS, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  unsigned int i;
  for (i=0; i<unknownPNAMs.size(); ++i)
  {
    //write PNAM
    output.write((const char*) &cPNAM, 4);
    //PNAM's length
    subLength = 8;
    output.write((const char*) &subLength, 2);
    //write PNAM's stuff
    output.write((const char*) &(unknownPNAMs[i].unknownOne), 4);
    output.write((const char*) &(unknownPNAMs[i].unknownTwo), 4);
  }//for

  return output.good();
}
#endif

bool ImpactDataSetRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
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
    std::cout <<"Error: sub record EDID of IPDS is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of IPDS!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownPNAMs.clear();
  PNAMSubrecord temp;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cPNAM:
           //PNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: subrecord PNAM of IPDS has invalid length ("<<subLength
                       <<" bytes). Should be 8 bytes!\n";
             return false;
           }
           //read PNAM's stuff
           in_File.read((char*) &temp.unknownOne, 4);
           in_File.read((char*) &temp.unknownTwo, 4);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord PNAM of IPDS!\n";
             return false;
           }//if
           unknownPNAMs.push_back(temp);
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                    << "\" found, but only PNAM is allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

uint32_t ImpactDataSetRecord::getRecordType() const
{
  return cIPDS;
}

} //namespace
