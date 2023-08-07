/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2014, 2021  Dirk Stolle

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

#include "GlobalRecord.hpp"
#include <iostream>
#include <stdexcept>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"
#include "../../base/UtilityFunctions.hpp"

namespace MWTP
{

GlobalRecord::GlobalRecord(const std::string& ID)
: BasicRecord(),
  recordID(ID),
  Type(GlobalType::Short),
  shortVal(0),
  longVal(0),
  floatVal(0.0f)
{}

bool GlobalRecord::equals(const GlobalRecord& other) const
{
  if ((Type != other.Type) || (lowerCaseCompare(other.recordID, recordID) != 0))
  {
    return false;
  }
  switch (Type)
  {
    case GlobalType::Short:
         return shortVal == other.shortVal;
         break;
    case GlobalType::Long:
         return longVal == other.longVal;
         break;
    case GlobalType::Float:
         return floatVal == other.floatVal;
         break;
  }
  std::cout << "Unhandled case in GlobalRecord::equals!\n";
  throw std::logic_error("Unhandled case in GlobalRecord::equals!");
}

#ifndef MW_UNSAVEABLE_RECORDS
bool GlobalRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cGLOB), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* FNAM */ + 4 /* 4 bytes for length */ + 1 /* length of FNAM */
        + 4 /* FLTV */ + 4 /* 4 bytes for length */ + 4 /* length of FLTV */;
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */

  // write record ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write variable type (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = 1;
  output.write((const char*) &SubLength, 4);
  char var_type = '\0';
  switch (Type)
  {
    case GlobalType::Float:
         var_type = 'f';
         break;
    case GlobalType::Short:
         var_type = 's';
         break;
    case GlobalType::Long:
         var_type = 'l';
         break;
  }
  output.write(&var_type, 1);

  // write variable's value (FLTV)
  output.write(reinterpret_cast<const char*>(&cFLTV), 4);
  SubLength = 4;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // Value is always written as float, even if it is a short or a long.
  float data = floatVal;
  switch(Type)
  {
    case GlobalType::Float:
         data = floatVal;
         break;
    case GlobalType::Long:
         data = longVal;
         break;
    case GlobalType::Short:
         data = shortVal;
         break;
  }
  output.write(reinterpret_cast<const char*>(&data), 4);
  return output.good();
}
#endif


bool GlobalRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */

  // read record ID (NAME)
  uint32_t bytesRead = 0;
  char Buffer[256];
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, bytesRead))
  {
    std::cerr << "GlobalRecord::loadFromStream: Error while reading ID from stream.\n";
    return false;
  }

  // read type (FNAM)
  uint32_t SubRecName = 0;
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  // FNAM's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 1)
  {
    std::cerr << "Error: Sub record FNAM of GLOB has invalid length ("
              << SubLength << " bytes). Should be one byte.\n";
    return false;
  }
  char TypeChar = '\0';
  input.read(&TypeChar, 1);
  if ((TypeChar != 'f') && (TypeChar != 'l') && (TypeChar != 's'))
  {
    std::cerr << "Error: Sub record FNAM of GLOB has invalid value ("
              << TypeChar << "). Should be 'f', 'l' or 's'.\n";
    return false;
  }

  // read value (FLTV)
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cFLTV)
  {
    UnexpectedRecord(cFLTV, SubRecName);
    return false;
  }
  // FLTV's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 4)
  {
    std::cerr << "Error: Sub record FLTV of GLOB has invalid length ("
              << SubLength << " bytes). Should be four bytes.\n";
    return false;
  }
  // read value - it's always stored as float in the file
  input.read(reinterpret_cast<char*>(&floatVal), 4);
  switch(TypeChar)
  {
    case 'f':
         Type = GlobalType::Float;
         break;
    case 'l':
         longVal = static_cast<int32_t>(floatVal);
         Type = GlobalType::Long;
         break;
    case 's':
         shortVal = static_cast<int16_t>(floatVal);;
         Type = GlobalType::Short;
         break;
  }

  if (!input.good())
  {
    std::cerr << "Error while reading data of record GLOB.\n";
    return false;
  }
  return true;
}

bool operator<(const GlobalRecord& left, const GlobalRecord& right)
{
  return lowerCaseCompare(left.recordID, right.recordID) < 0;
}

} // namespace
