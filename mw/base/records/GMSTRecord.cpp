/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013, 2014  Thoronador

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
  SettingName(""),
  Type(gtInteger),
  iVal(0),
  fVal(0.0f),
  sVal("")
{
}

bool GMSTRecord::equals(const GMSTRecord& other) const
{
  if ((Type!=other.Type) or (SettingName!=other.SettingName))
  {
    return false;
  }
  switch (Type)
  {
    case gtInteger:
         return (iVal==other.iVal);
         break;
    case gtFloat:
         //Is it really a good idea to use direct equality tests for floats here?
         //Things like 4.0000000001 and 3.999999998 can basically be the same,
         //but have a different bit representation.
         return (fVal==other.fVal);
         break;
    case gtString:
         return (sVal==other.sVal);
         break;
  }//swi
  //should never reach this point here
  throw std::logic_error("MWTP::GMSTRecord::equals(): We should never reach this point!");
}

#ifndef MW_UNSAVEABLE_RECORDS
bool GMSTRecord::saveToStream(std::ofstream& output) const
{
  //write GMST
  output.write((const char*) &cGMST, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* four bytes for length */
        +SettingName.length() /* length of ID */
        +4 /* STRV/ INTV/ FLTV */ + 4 /* four bytes for length of data */;
  switch(Type)
  {
    case gtInteger:
    case gtFloat:
         Size = Size + 4 /* four bytes for integer/float value */;
         break;
    case gtString:
         Size = Size + sVal.length();
         break;
  }//swi
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  //write NAME
  output.write((const char*) &cNAME, 4);
  //NAME's length
  uint32_t SubLength;
  SubLength = SettingName.length(); //length of string
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(SettingName.c_str(), SubLength);
  //write STRV/ FLTV/ INTV
  switch(Type)
  {
    case gtInteger:
         //write INTV
         output.write((const char*) &cINTV, 4);
         //write length
         SubLength = 4;
         output.write((const char*) &SubLength, 4);
         //write integer
         output.write((const char*) &iVal, 4);
         break;
    case gtFloat:
         //write FLTV
         output.write((const char*) &cFLTV, 4);
         //write length
         SubLength = 4;
         output.write((const char*) &SubLength, 4);
         //write float
         output.write((const char*) &fVal, 4);
         break;
    case gtString:
         //write STRV
         output.write((const char*) &cSTRV, 4);
         //write length
         SubLength = sVal.length();
         output.write((const char*) &SubLength, 4);
         //write string
         output.write(sVal.c_str(), SubLength);
         break;
  }//swi
  return output.good();
}
#endif

bool GMSTRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>511)
  {
    std::cout << "Sub record NAME of GMST is longer than 511 characters!\n";
    return false;
  }
  if (SubLength<=0)
  {
    std::cout << "Sub record NAME of GMST is shorter than one character!\n";
    return false;
  }
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record NAME of GMST!\n";
    return false;
  }
  SettingName = std::string(Buffer);
  const char NameFirstChar = SettingName.at(0);

  fVal = 0.0f;
  iVal = 0;
  sVal = "";

  //read value record based on first character of setting name
  switch(NameFirstChar)
  {
    case 'f':
         //read FLTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cFLTV)
         {
           UnexpectedRecord(cFLTV, SubRecName);
           return false;
         }
         //FLTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record FLTV of GMST has invalid length ("
                     << SubLength << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //read FLTV
         in_File.read((char*) &fVal, 4);
         Type = gtFloat;
         break;
    case 'i':
         //read INTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cINTV)
         {
           UnexpectedRecord(cINTV, SubRecName);
           return false;
         }
         //INTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record INTV of GMST has invalid length ("
                     << SubLength << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //read INTV
         in_File.read((char*) &iVal, 4);
         Type = gtInteger;
         break;
    default: //'s'
         Type = gtString;
         // if string is empty, STRV sub-record may not be present at all
         if (BytesRead<Size)
         {
           //read STRV
           in_File.read((char*) &SubRecName, 4);
           if (SubRecName!=cSTRV)
           {
             UnexpectedRecord(cSTRV, SubRecName);
             return false;
           }
           //STRV's length
           in_File.read((char*) &SubLength, 4);
           if (SubLength>511)
           {
             std::cout << "Sub record STRV of GMST is longer than 511 characters!\n";
             return false;
           }
           //read STRV
           memset(Buffer, '\0', 512);
           in_File.read(Buffer, SubLength);
           sVal = std::string(Buffer);
         }
         break;
  }//switch
  return in_File.good();
}

} //namespace
