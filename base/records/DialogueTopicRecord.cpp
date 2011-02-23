/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "DialogueTopicRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

DialogueTopicRecord::DialogueTopicRecord()
{
  DialogueID = "";
  Type = 0;
}

bool DialogueTopicRecord::equals(const DialogueTopicRecord& other) const
{
  return ((DialogueID==other.DialogueID) and (Type==other.Type));
}

bool DialogueTopicRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cDIAL, 4);
  int32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +DialogueID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* DATA */ +4 /* 4 bytes for length */ +1 /* fixed length of one byte */;
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

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
  output.write((char*) &cNAME, 4);
  int32_t SubLength = DialogueID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(DialogueID.c_str(), SubLength);

  //write DATA
  output.write((char*) &cDATA, 4);
  SubLength = 1;
  //write DATA's length
  output.write((char*) &SubLength, 4);
  //write data
  output.write((char*) &Type, 1);

  return output.good();
}

bool DialogueTopicRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size;
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

  int32_t SubRecName, SubLength;
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
  //read book ID
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
    std::cout << "Error: Subrecord DATA of BOOK has wrong size ("<<SubLength
              << " bytes), should be one byte.\n";
    return false;
  }
  //read book data
  in_File.read((char*) &Type, 1);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of DIAL!\n";
    return false;
  }
  return true;
}
