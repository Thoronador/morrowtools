/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "ColourFormRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

ColourFormRecord::ColourFormRecord()
: BasicRecord(), editorID(""),
  name(LocalizedString()),
  unknownCNAM(0),
  unknownFNAM(0)
{

}

ColourFormRecord::~ColourFormRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ColourFormRecord::equals(const ColourFormRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (name==other.name)
      and (unknownCNAM==other.unknownCNAM) and (unknownFNAM==other.unknownFNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ColourFormRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize() /* FULL */;
  }
  return writeSize;
}

bool ColourFormRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cCLFM, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (name.isPresent())
  {
    //write FULL
    if (!name.saveToStream(output, cFULL))
      return false;
  }//if has FULL

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write CNAM's data
  output.write((const char*) &unknownCNAM, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 4;
  output.write((const char*) &subLength, 2);
  //write FNAM's data
  output.write((const char*) &unknownFNAM, 4);

  return output.good();
}
#endif

bool ColourFormRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

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
    std::cerr <<"Error: sub record EDID of CLFM is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of CLFM!\n";
    return false;
  }
  editorID = std::string(buffer);

  name.reset();
  bool hasReadCNAM = false;
  bool hasReadFNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: CLFM seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cerr << "Error: CLFM seems to have more than one CNAM subrecord.\n";
             return false;
           }
           //read CNAM
           if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, false)) return false;
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: CLFM seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //read FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, unknownFNAM, false)) return false;
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      default:
           std::cerr << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, CNAM or FNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence check
  if (!(hasReadCNAM  and hasReadFNAM))
  {
    std::cerr << "Error: while reading CLFM record: at least one required subrecord is missing!\n";
    return false;
  }//if

  return in_File.good();
}

uint32_t ColourFormRecord::getRecordType() const
{
  return cCLFM;
}

} //namespace
