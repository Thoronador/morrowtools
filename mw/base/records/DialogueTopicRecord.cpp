/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2022  Dirk Stolle

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

#include "DialogueTopicRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

DialogueTopicRecord::DialogueTopicRecord()
: BasicRecord(),
  recordID(""),
  Type(DialogueTopicType::Regular)
{}

bool DialogueTopicRecord::equals(const DialogueTopicRecord& other) const
{
  return (recordID == other.recordID) && (Type == other.Type);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool DialogueTopicRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cDIAL), 4);
  const uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* DATA */ + 4 /* 4 bytes for length */ + 1 /* data is one byte */;
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write type (DATA)
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  SubLength = 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Type), 1);

  return output.good();
}
#endif

bool DialogueTopicRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

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

  uint32_t BytesRead = 0;

  // read dialogue topic ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of DIAL!\n";
    return false;
  }

  // read DATA
  uint32_t SubRecName = 0;
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  // DATA's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 1)
  {
    std::cerr << "Error: Sub record DATA of DIAL has wrong size (" << SubLength
              << " bytes), should be one byte.\n";
    return false;
  }
  // read dialogue data
  input.read(reinterpret_cast<char*>(&Type), 1);
  if (!input.good())
  {
    std::cerr << "Error while reading sub record DATA of DIAL!\n";
    return false;
  }
  return true;
}

} // namespace
