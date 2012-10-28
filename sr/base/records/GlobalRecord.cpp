/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "GlobalRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

GlobalRecord::GlobalRecord()
: BasicRecord()
{
  editorID = "";
  Type = globShort;
  shortVal = 0;
  longVal = 0;
  floatVal = 0.0f;
}

uint32_t GlobalRecord::getRecordType() const
{
  return cGLOB;
}

#ifndef SR_NO_RECORD_EQUALITY
bool GlobalRecord::equals(const GlobalRecord& other) const
{
  if ((Type!=other.Type) or (editorID!=other.editorID) or (!equalsBasic(other)))
  {
    return false;
  }
  switch (Type)
  {
    case globShort:
         return (shortVal==other.shortVal);
         break;
    case globLong:
         return (longVal==other.longVal);
         break;
    case globFloat:
         return (floatVal==other.floatVal);
         break;
  }//swi
  std::cout << "Unhandled case in GlobalRecord::equals!\n";
  throw 42;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t GlobalRecord::getWriteSize() const
{
  return (4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +1 /* fixed length of 1 byte */
        +4 /* FLTV */ +2 /* 2 bytes for length */ +4 /* fixed length of 4 byte2 */);
}

bool GlobalRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cGLOB, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write EDID's stuff
  output.write(editorID.c_str(), subLength);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 1; /* fixed size */
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  char outChar = '\0';
  float outFloat = 0.0f;
  switch(Type)
  {
    case globShort:
         outChar = 's';
         outFloat = shortVal;
         break;
    case globLong:
         outChar = 'l';
         outFloat = longVal;
         break;
    case globFloat:
         outChar = 'f';
         outFloat = floatVal;
         break;
  }//swi
  output.write(&outChar, 1);

  //write FLTV
  output.write((const char*) &cFLTV, 4);
  //FLTV's length
  subLength = 4; /* fixed size */
  output.write((const char*) &subLength, 2);
  //write FLTV's value
  output.write((const char*) &outFloat, 4);

  return output.good();
}
#endif

bool GlobalRecord::loadFromStream(std::ifstream& in_File)
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
  //read EDID's size
  subLength = 0;
  in_File.read((char*) &subLength, 2);
  if (subLength>511)
  {
    std::cout << "Sub record EDID of GLOB is longer than 511 characters!\n";
    return false;
  }
  if (subLength<=0)
  {
    std::cout << "Sub record EDID of GLOB is shorter than one character!\n";
    return false;
  }
  char buffer[512];
  memset(buffer, 0, 512);
  //read EDID
  in_File.read(buffer, subLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record EDID of GLOB!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read FNAM
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, subRecName);
    return false;
  }
  //read FNAM's size
  subLength = 0;
  in_File.read((char*) &subLength, 2);
  if (subLength!=1)
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid length ("<<subLength
              <<" bytes). Should be one byte.\n";
    return false;
  }
  char typeChar = '\0';
  //read FNAM's content
  in_File.read(&typeChar, 1);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record FNAM of GLOB!\n";
    return false;
  }
  if ((typeChar!='s') and (typeChar!='l') and (typeChar!='f'))
  {
    std::cout << "Error: sub record FNAM of GLOB has invalid content!\n";
    return false;
  }

  //read FLTV
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cFLTV)
  {
    UnexpectedRecord(cFLTV, subRecName);
    return false;
  }
  //read FLTV's size
  subLength = 0;
  in_File.read((char*) &subLength, 2);
  if (subLength!=4)
  {
    std::cout <<"Error: sub record FLTV of GLOB has invalid length ("<<subLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  //read FLTV's content
  float tempFloat = 0.0f;
  in_File.read((char*) &tempFloat, 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record FLTV of GLOB!\n";
    return false;
  }
  switch (typeChar)
  {
    case 's':
         shortVal = tempFloat;
         Type = globShort;
         break;
    case 'l':
         longVal = tempFloat;
         Type = globLong;
         break;
    case 'f':
         floatVal = tempFloat;
         Type = globFloat;
         break;
  }//swi

  return in_File.good();
}

} //namespace
