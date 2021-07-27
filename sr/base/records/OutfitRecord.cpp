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

#include "OutfitRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

OutfitRecord::OutfitRecord()
: BasicRecord(), editorID(""),
  itemFormIDs(std::vector<uint32_t>())
{

}

OutfitRecord::~OutfitRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool OutfitRecord::equals(const OutfitRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID) and (itemFormIDs==other.itemFormIDs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t OutfitRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* INAM */ +2 /* 2 bytes for length */ +4*itemFormIDs.size());
}

bool OutfitRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cOTFT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write INAM
  output.write((const char*) &cINAM, 4);
  //INAM's length
  subLength = 4*itemFormIDs.size();
  output.write((const char*) &subLength, 2);
  //write form IDs
  unsigned int i;
  for (i=0; i<itemFormIDs.size(); ++i)
  {
    output.write((const char*) &(itemFormIDs[i]), 4);
  }//for

  return output.good();
}
#endif

bool OutfitRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of OTFT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of STAT!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read INAM
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cINAM)
  {
    UnexpectedRecord(cINAM, subRecName);
    return false;
  }
  //INAM's length
  in_File.read((char*) &subLength, 2);
  if ((subLength%4)!=0)
  {
    std::cout <<"Error: sub record OBND of STAT has invalid length ("<<subLength
              <<" bytes)! Should be a integral multiple of four bytes.\n";
    return false;
  }
  //read INAM's stuff
  uint32_t i, temp;
  itemFormIDs.clear();
  const uint32_t count = subLength/4;
  for (i=0; i<count; ++i)
  {
    in_File.read((char*) &temp, 4);
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord INAM of STAT!\n";
      return false;
    }
    itemFormIDs.push_back(temp);
  }//for

  return in_File.good();
}

uint32_t OutfitRecord::getRecordType() const
{
  return cOTFT;
}

} //namespace
