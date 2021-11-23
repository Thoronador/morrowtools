/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013, 2014, 2021  Thoronador

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

#include "GMSTRecord.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

GMSTRecord::GMSTRecord()
: BasicRecord(),
  SettingName(std::string()),
  Type(GMSTType::Integer),
  iVal(0),
  fVal(0.0f),
  sVal(std::string())
{
}

bool GMSTRecord::equals(const GMSTRecord& other) const
{
  if ((Type != other.Type) || (SettingName != other.SettingName))
  {
    return false;
  }
  switch (Type)
  {
    case GMSTType::Integer:
         return iVal == other.iVal;
         break;
    case GMSTType::Float:
         // Is it really a good idea to use direct equality tests for floats here?
         // Things like 4.0000000001 and 3.999999998 can basically be the same,
         // but have a different bit representation.
         return fVal == other.fVal;
         break;
    case GMSTType::String:
         return sVal == other.sVal;
         break;
  }
  // should never reach this point here
  throw std::logic_error("MWTP::GMSTRecord::equals(): We should never reach this point!");
}

#ifndef MW_UNSAVEABLE_RECORDS
bool GMSTRecord::saveToStream(std::ostream& output) const
{
  // write GMST
  output.write(reinterpret_cast<const char*>(&cGMST), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* four bytes for length */
        + SettingName.length() /* length of ID */
        + 4 /* STRV/ INTV/ FLTV */ + 4 /* four bytes for length of data */;
  switch(Type)
  {
    case GMSTType::Integer:
    case GMSTType::Float:
         Size = Size + 4 /* four bytes for integer/float value */;
         break;
    case GMSTType::String:
         Size = Size + sVal.length();
         break;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */

  // write settings's ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = SettingName.length();
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(SettingName.c_str(), SubLength);

  // write STRV / FLTV / INTV
  switch(Type)
  {
    case GMSTType::Integer:
         // write integer value (INTV)
         output.write(reinterpret_cast<const char*>(&cINTV), 4);
         SubLength = 4;
         output.write(reinterpret_cast<const char*>(&SubLength), 4);
         output.write(reinterpret_cast<const char*>(&iVal), 4);
         break;
    case GMSTType::Float:
         // write floating point value (FLTV)
         output.write(reinterpret_cast<const char*>(&cFLTV), 4);
         SubLength = 4;
         output.write(reinterpret_cast<const char*>(&SubLength), 4);
         output.write(reinterpret_cast<const char*>(&fVal), 4);
         break;
    case GMSTType::String:
         // write string value (STRV)
         output.write(reinterpret_cast<const char*>(&cSTRV), 4);
         SubLength = sVal.length();
         output.write(reinterpret_cast<const char*>(&SubLength), 4);
         output.write(sVal.c_str(), SubLength);
         break;
  }
  return output.good();
}
#endif

bool GMSTRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;

  // read NAME
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  uint32_t BytesRead = 4;
  if (SubRecName != cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength > 511)
  {
    std::cerr << "Sub record NAME of GMST is longer than 511 characters!\n";
    return false;
  }
  if (SubLength <= 0)
  {
    std::cerr << "Sub record NAME of GMST is shorter than one character!\n";
    return false;
  }
  char Buffer[512];
  memset(Buffer, '\0', 512);
  input.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record NAME of GMST!\n";
    return false;
  }
  SettingName = std::string(Buffer);
  const char NameFirstChar = SettingName.at(0);

  fVal = 0.0f;
  iVal = 0;
  sVal.clear();

  // read value record based on first character of setting name
  switch(NameFirstChar)
  {
    case 'f':
         // read FLTV
         input.read(reinterpret_cast<char*>(&SubRecName), 4);
         if (SubRecName != cFLTV)
         {
           UnexpectedRecord(cFLTV, SubRecName);
           return false;
         }
         input.read(reinterpret_cast<char*>(&SubLength), 4);
         if (SubLength != 4)
         {
           std::cerr << "Error: sub record FLTV of GMST has invalid length ("
                     << SubLength << " bytes). Should be 4 bytes.\n";
           return false;
         }
         input.read(reinterpret_cast<char*>(&fVal), 4);
         Type = GMSTType::Float;
         break;
    case 'i':
         // read INTV
         input.read(reinterpret_cast<char*>(&SubRecName), 4);
         if (SubRecName != cINTV)
         {
           UnexpectedRecord(cINTV, SubRecName);
           return false;
         }
         input.read(reinterpret_cast<char*>(&SubLength), 4);
         if (SubLength != 4)
         {
           std::cerr << "Error: sub record INTV of GMST has invalid length ("
                     << SubLength << " bytes). Should be 4 bytes.\n";
           return false;
         }
         input.read(reinterpret_cast<char*>(&iVal), 4);
         Type = GMSTType::Integer;
         break;
    default: //'s'
         Type = GMSTType::String;
         // if string is empty, STRV sub-record may not be present at all
         if (BytesRead < Size)
         {
           // read STRV
           input.read(reinterpret_cast<char*>(&SubRecName), 4);
           if (SubRecName != cSTRV)
           {
             UnexpectedRecord(cSTRV, SubRecName);
             return false;
           }
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           if (SubLength > 511)
           {
             std::cerr << "Sub record STRV of GMST is longer than 511 characters!\n";
             return false;
           }
           memset(Buffer, '\0', 512);
           input.read(Buffer, SubLength);
           sVal = std::string(Buffer);
         }
         break;
  }
  return input.good();
}

} // namespace
