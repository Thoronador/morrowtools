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

#include "GMSTRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>

namespace SRTP
{

GMSTRecord::GMSTRecord()
: BasicRecord()
{
  m_SettingName = "";
  std::string m_SettingName;
  m_IntValue = 0;
  m_FloatValue = 0.0f;
  m_StringValue = "";
  m_Type = gtInteger;
}

GMSTRecord::~GMSTRecord()
{
  //empty
}

int32_t GMSTRecord::getRecordType() const
{
  return cGMST;
}

#ifndef SR_NO_RECORD_EQUALITY
bool GMSTRecord::equals(const GMSTRecord& other) const
{
  if ((m_SettingName==other.getSettingName()) and (m_Type==other.getSettingType())
    and (equalsBasic(other)))
  {
    switch (m_Type)
    {
      case gtInteger:
           return (m_IntValue==other.getInteger());
      case gtFloat:
           return (m_FloatValue==other.getFloat());
      case gtString:
           return (m_StringValue==other.getString());
    }//swi
  }
  return false;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t GMSTRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +m_SettingName.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */;
  switch (m_Type)
  {
    case gtInteger:
    case gtFloat:
         writeSize += 4;
         break;
    case gtString:
         writeSize = writeSize + m_StringValue.length()+1 /* length of content +1 byte for NUL termination */;
         break;
  }//swi
  return writeSize;
}

bool GMSTRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cGMST, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t SubLength = m_SettingName.length()+1;
  output.write((char*) &SubLength, 2);
  //write EDID's stuff
  output.write(m_SettingName.c_str(), SubLength);
  //write DATA
  output.write((char*) &cEDID, 4);

  switch (m_Type)
  {
    case gtInteger:
         //DATA's length
         SubLength = 4; /* fixed length */
         output.write((char*) &SubLength, 2);
         //write DATA's stuff
         output.write((char*)&m_IntValue, 4);
         break;
    case gtFloat:
         //DATA's length
         SubLength = 4; /* fixed length */
         output.write((char*) &SubLength, 2);
         //write DATA's stuff
         output.write((char*)&m_FloatValue, 4);
         break;
    case gtString:
         //DATA's length
         SubLength = m_StringValue.length()+1;
         output.write((char*) &SubLength, 2);
         //write DATA's stuff
         output.write(m_SettingName.c_str(), SubLength);
         break;
  }//swi
  return output.good();
}
#endif

bool GMSTRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  if (!loadSizeAndUnknownValues(in_File, Size)) return false;

  uint16_t subRecSize;
  int32_t subRecName;
  //read EDID
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //read EDID's size
  subRecSize = 0;
  in_File.read((char*) &subRecSize, 2);
  if (subRecSize>511)
  {
    std::cout << "Sub record EDID of GMST is longer than 511 characters!\n";
    return false;
  }
  if (subRecSize<=0)
  {
    std::cout << "Sub record EDID of GMST is shorter than one character!\n";
    return false;
  }
  char buffer[512];
  memset(buffer, 0, 512);
  //read EDID
  in_File.read(buffer, subRecSize);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record EDID of GMST!\n";
    return false;
  }
  m_SettingName = std::string(buffer);
  const char NameFirstChar = m_SettingName.at(0);

  m_FloatValue = 0.0f;
  m_IntValue = 0;
  m_StringValue = "";

  //read DATA
  in_File.read((char*) &subRecName, 4);
  if (subRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, subRecName);
    return false;
  }
  //read DATA's length
  in_File.read((char*) &subRecSize, 2);

  //read value record based on first character of setting name
  switch(NameFirstChar)
  {
    case 'f':
         //read float
         //check length
         if (subRecSize != 4)
         {
           std::cout << "Error: sub record DATA of GMST has invalid length ("
                     << subRecSize << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //read the float value
         in_File.read((char*) &m_FloatValue, 4);
         m_Type = gtFloat;
         break;
    case 'i':
         //read integer
         //interger's length
         if (subRecSize != 4)
         {
           std::cout << "Error: sub record DATA of GMST has invalid length ("
                     << subRecSize << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //read the integer value
         in_File.read((char*) &m_IntValue, 4);
         m_Type = gtInteger;
         break;
    default: //'s'
         // if string is empty, STRV sub-record may not be present at all
         //check  length
         if (subRecSize>511)
         {
           std::cout << "Sub record DATA of GMST is longer than 511 characters!\n";
           return false;
         }
         //read string value
         memset(buffer, '\0', 512);
         in_File.read(buffer, subRecSize);
         if (!in_File.good())
         {
           std::cout << "GMSTRecord::loadFromStream: Error while reading string data!\n";
           return false;
         }
         m_StringValue = std::string(buffer);
         m_Type = gtString;
         break;
  }//switch
  return in_File.good();
}

const std::string& GMSTRecord::getSettingName() const
{
  return m_SettingName;
}

GMSTType GMSTRecord::getSettingType() const
{
  return m_Type;
}

const std::string& GMSTRecord::getString() const
{
  return m_StringValue;
}

float GMSTRecord::getFloat() const
{
  return m_FloatValue;
}

int32_t GMSTRecord::getInteger() const
{
  return m_IntValue;
}

} //namespace
