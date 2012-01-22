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

#include "GrassRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

GrassRecord::GrassRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  modelPath = "";
  unknownMODT.setPresence(false);
  memset(unknownDATA, 0, 32);
}

GrassRecord::~GrassRecord()
{
  //empty
}

bool GrassRecord::equals(const GrassRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (memcmp(unknownDATA, other.unknownDATA, 32)==0));
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t GrassRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +32 /* fixed length of 32 bytes */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /*MODT*/ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }
  return writeSize;
}

bool GrassRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cGRAS, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of GRAS!\n";
      return false;
    }
  }//if has MODT

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 32; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) unknownDATA, 32);

  return output.good();
}
#endif

bool GrassRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
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
    std::cout <<"Error: sub record EDID of GRAS is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of GRAS!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: subrecord OBND of GRAS has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of GRAS!\n";
    return false;
  }

  //read MODL
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cMODL)
  {
    UnexpectedRecord(cMODL, subRecName);
    return false;
  }
  //MODL's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record MODL of GRAS is longer than 511 characters!\n";
    return false;
  }
  //read MODL's stuff
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord MODL of GRAS!\n";
    return false;
  }
  modelPath = std::string(buffer);

  //read MODT
  if (!unknownMODT.loadFromStream(in_File, cMODT, true))
  {
    std::cout << "Error while reading subrecord MODT of GRAS!\n";
    return false;
  }

  //read DATA
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=32)
  {
    std::cout <<"Error: subrecord DATA of GRAS has invalid length ("<<subLength
              <<" bytes). Should be 32 bytes!\n";
    return false;
  }
  //read DATA's stuff
  in_File.read((char*) unknownDATA, 32);
  bytesRead += 32;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of GRAS!\n";
    return false;
  }

  return in_File.good();
}

int32_t GrassRecord::getRecordType() const
{
  return cGRAS;
}

} //namespace
