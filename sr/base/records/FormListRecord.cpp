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

#include "FormListRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

FormListRecord::FormListRecord()
: BasicRecord(), editorID(""),
  listFormIDs(std::vector<uint32_t>())
{

}

FormListRecord::~FormListRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool FormListRecord::equals(const FormListRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (listFormIDs==other.listFormIDs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FormListRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
             +editorID.length()+1 /* length of strin +1 byte for NUL-termination */;
  if (!listFormIDs.empty())
  {
    writeSize = writeSize + listFormIDs.size()*
                (4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed size */);
  }
  return writeSize;
}

bool FormListRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cFLST, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (!listFormIDs.empty())
  {
    unsigned int i;
    const unsigned int count = listFormIDs.size();
    for (i=0; i<count; ++i)
    {
      //write LNAM
      output.write((const char*) &cLNAM, 4);
      //LNAM's length
      subLength = 4; //fixed
      //write form ID
      output.write((const char*) &listFormIDs[i], 4);
    }//for
  }//if form IDs present

  return output.good();
}
#endif

bool FormListRecord::loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table)
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
    std::cout <<"Error: sub record EDID of FLST is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of FLST!\n";
    return false;
  }
  editorID = std::string(buffer);

  listFormIDs.clear();
  uint32_t temp_fID;
  while (bytesRead<readSize)
  {
    //read LNAM
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    if (subRecName!=cLNAM)
    {
      UnexpectedRecord(cLNAM, subRecName);
      return false;
    }
    //LNAM's length
    in_File.read((char*) &subLength, 2);
    bytesRead += 2;
    if (subLength!=4)
    {
      std::cout <<"Error: sub record LNAM of RELA has invalid length("<<subLength
                <<" bytes). Should be four bytes!\n";
      return false;
    }
    //read LNAM's stuff
    in_File.read((char*) &temp_fID, 4);
    bytesRead += 4;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord LNAM of FLST!\n";
      return false;
    }//if
    listFormIDs.push_back(temp_fID);
  }//while

  return in_File.good();
}

uint32_t FormListRecord::getRecordType() const
{
  return cFLST;
}

} //namespace
