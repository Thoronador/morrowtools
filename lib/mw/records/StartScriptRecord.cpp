/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "StartScriptRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

StartScriptRecord::StartScriptRecord()
: BasicRecord(),
  Data(std::string()),
  recordID(std::string())
{}

bool StartScriptRecord::equals(const StartScriptRecord& other) const
{
  return (Data == other.Data) && (recordID == other.recordID);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool StartScriptRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSSCR), 4);
  const uint32_t Size = 4 /* DATA */ + 4 /* 4 bytes for length */
        + Data.length() /* length of sequence (no NUL termination) */
        + 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() /* length of name (no NUL termination) */;
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Start Script(?): (no documentation known)
    DATA = ? (a sequence of digits)
    NAME = ID string*/

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  uint32_t SubLength = Data.length();
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(Data.c_str(), SubLength);

  // write script name (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  SubLength = recordID.length();
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  return output.good();
}
#endif

bool StartScriptRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Start Script(?): (no documentation known)
    DATA = ? (a sequence of digits)
    NAME = ID string */

  uint32_t bytesRead = 0;

  // read DATA
  char Buffer[256];
  if (!loadString256WithHeader(input, Data, Buffer, cDATA, bytesRead))
  {
    std::cerr << "StartScriptRecord: Error while reading data sequence from stream.\n";
    return false;
  }

  // read name (NAME)
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, bytesRead))
  {
    std::cerr << "StartScriptRecord: Error while reading name from stream.\n";
    return false;
  }

  return true;
}

} // namespace
