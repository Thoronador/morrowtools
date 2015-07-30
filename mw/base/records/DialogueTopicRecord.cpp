/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "DialogueTopicRecord.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

const uint8_t DialogueTopicRecord::dttRegular = 0;
const uint8_t DialogueTopicRecord::dttVoice = 1;
const uint8_t DialogueTopicRecord::dttGreeting = 2;
const uint8_t DialogueTopicRecord::dttPersuasion = 3;
const uint8_t DialogueTopicRecord::dttJournal = 4;

DialogueTopicRecord::DialogueTopicRecord()
: BasicRecord(),
  DialogueID(""),
  Type(dttRegular)
{}

bool DialogueTopicRecord::equals(const DialogueTopicRecord& other) const
{
  return ((DialogueID==other.DialogueID) and (Type==other.Type));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool DialogueTopicRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cDIAL, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +DialogueID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* DATA */ +4 /* 4 bytes for length */ +1 /* fixed length of one byte */;
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Dialogue topic (including journals)
	NAME = Dialogue ID string
	DATA = Dialogue Type? (1 byte, 4 bytes for deleted?)
		0 = Regular Topic
		1 = Voice?
		2 = Greeting?
		3 = Persuasion?
		4 = Journal
	What follows in the ESP/ESM are all the INFO records that belong to the
	DIAL record (one of the few cases where order is important).
  */

  //write NAME
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = DialogueID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(DialogueID.c_str(), SubLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  SubLength = 1;
  //write DATA's length
  output.write((const char*) &SubLength, 4);
  //write data
  output.write((const char*) &Type, 1);

  return output.good();
}
#endif

bool DialogueTopicRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Dialogue topic (including journals)
	NAME = Dialogue ID string
	DATA = Dialogue Type? (1 byte, 4 bytes for deleted?)
		0 = Regular Topic
		1 = Voice?
		2 = Greeting?
		3 = Persuasion?
		4 = Journal
	What follows in the ESP/ESM are all the INFO records that belong to the
	DIAL record (one of the few cases where order is important).
  */

  uint32_t SubRecName;
  uint32_t SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord NAME of DIAL is longer than 255 characters.\n";
    return false;
  }
  //read dialogue topic ID
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NAME of DIAL!\n";
    return false;
  }
  DialogueID = std::string(Buffer);

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=1)
  {
    std::cout << "Error: Subrecord DATA of DIAL has wrong size ("<<SubLength
              << " bytes), should be one byte.\n";
    return false;
  }
  //read dialogue data
  in_File.read((char*) &Type, 1);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of DIAL!\n";
    return false;
  }
  return true;
}

bool DialogueTopicRecord::isRegularTopic() const
{
  return (Type==dttRegular);
}

bool DialogueTopicRecord::isGreeting() const
{
  return (Type==dttGreeting);
}

bool DialogueTopicRecord::isJournal() const
{
  return (Type==dttJournal);
}

} //namespace
