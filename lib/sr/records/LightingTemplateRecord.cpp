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

#include "LightingTemplateRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

LightingTemplateRecord::LightingTemplateRecord()
: BasicRecord(), editorID("")
{
  unknownDATA.setPresence(false);
  unknownDALC.setPresence(false);
}

LightingTemplateRecord::~LightingTemplateRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LightingTemplateRecord::equals(const LightingTemplateRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownDATA==other.unknownDATA) and (unknownDALC==other.unknownDALC));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LightingTemplateRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (unknownDATA.isPresent())
  {
    writeSize = writeSize + 4 /* DATA */ + 2 /* 2 bytes for length */ +unknownDATA.size() /* length */;
  }
  if (unknownDALC.isPresent())
  {
    writeSize = writeSize + 4 /* DALC */ + 2 /* 2 bytes for length */ +unknownDALC.size() /* length */;
  }
  return writeSize;
}

bool LightingTemplateRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cLGTM, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownDATA.isPresent())
  {
    //write DATA
    if (!unknownDATA.saveToStream(output, cDATA))
    {
      std::cerr << "Error while writing DATA of LGTM!";
      return false;
    }
  }//if DATA

  if (unknownDALC.isPresent())
  {
    //write DALC
    if (!unknownDALC.saveToStream(output, cDALC))
    {
      std::cerr << "Error while writing DALC of LGTM!";
      return false;
    }
  }//if DALC

  return output.good();
}
#endif

bool LightingTemplateRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
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
    std::cerr <<"Error: sub record EDID of LGTM is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of LGTM!\n";
    return false;
  }
  editorID = std::string(buffer);

  // read DATA
  if (!unknownDATA.loadFromStream(in_File, cDATA, true))
  {
    std::cerr << "Error while reading subrecord DATA of LGTM!\n";
    return false;
  }
  // check length
  if ((unknownDATA.size() != 92) && (unknownDATA.size() != 72) && (unknownDATA.size() != 64))
  {
    std::cerr << "Error: sub record DATA of LGTM has invalid length ("
              << unknownDATA.size()<< " bytes). Should be 92, 72 or 64 bytes!\n";
    return false;
  }

  // read DALC
  if (!unknownDALC.loadFromStream(in_File, cDALC, true))
  {
    std::cerr << "Error while reading subrecord DALC of LGTM!\n";
    return false;
  }
  // check length
  if ((unknownDALC.size() != 32) && (unknownDALC.size() != 24))
  {
    std::cerr << "Error: sub record DALC of LGTM has invalid length ("
              << unknownDALC.size() << " bytes). Should be 32 or 24 bytes!\n";
    return false;
  }

  return in_File.good();
}

uint32_t LightingTemplateRecord::getRecordType() const
{
  return cLGTM;
}

} //namespace
