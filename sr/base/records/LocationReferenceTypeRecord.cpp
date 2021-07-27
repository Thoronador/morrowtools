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

#include "LocationReferenceTypeRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

LocationReferenceTypeRecord::LocationReferenceTypeRecord()
: BasicRecord(), editorID(""),
  hasCNAM(false),
  colourRed(0),
  colourGreen(0),
  colourBlue(0),
  unused(0)
{

}

LocationReferenceTypeRecord::~LocationReferenceTypeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LocationReferenceTypeRecord::equals(const LocationReferenceTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasCNAM==other.hasCNAM) and (((colourRed==other.colourRed)
        and (colourGreen==other.colourGreen) and (colourBlue==other.colourBlue)
        and (unused==other.unused)) or (!hasCNAM)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LocationReferenceTypeRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (hasCNAM)
  {
    writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if CNAM
  return writeSize;
}

bool LocationReferenceTypeRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cLCRT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (hasCNAM)
  {
    //write CNAM
    output.write((const char*) &cCNAM, 4);
    //CNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write CNAM's stuff
    output.write((const char*) &colourRed, 1);
    output.write((const char*) &colourGreen, 1);
    output.write((const char*) &colourBlue, 1);
    output.write((const char*) &unused, 1);
  }//if has CNAM

  return output.good();
}
#endif

bool LocationReferenceTypeRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of LCRT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of LCRT!\n";
    return false;
  }
  editorID = std::string(buffer);

  if (bytesRead<readSize)
  {
    //read CNAM
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    if (subRecName!=cCNAM)
    {
      UnexpectedRecord(cCNAM, subRecName);
      return false;
    }
    //CNAM's length
    in_File.read((char*) &subLength, 2);
    bytesRead += 2;
    if (subLength!=4)
    {
      std::cout <<"Error: sub record CNAM of LCRT has invalid length ("
                <<subLength<<" bytes). Should be four bytes!\n";
      return false;
    }
    //read EDID's stuff
    in_File.read(buffer, subLength);
    bytesRead += subLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord CNAM of LCRT!\n";
      return false;
    }
    hasCNAM = true;
  }
  else
  {
    hasCNAM = false;
    colourRed = 0;
    colourGreen = 0;
    colourBlue = 0;
    unused = 0;
  }

  return in_File.good();
}

uint32_t LocationReferenceTypeRecord::getRecordType() const
{
  return cLCRT;
}

} //namespace
