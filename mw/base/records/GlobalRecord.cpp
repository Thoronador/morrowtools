/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2014  Thoronador

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
#include <cstring>
#include <stdexcept>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"
#include "../../../base/UtilityFunctions.hpp"

namespace MWTP
{

GlobalRecord::GlobalRecord()
: BasicRecord(),
  recordID(""),
  Type(globShort),
  shortVal(0),
  longVal(0),
  floatVal(0.0f)
{}

GlobalRecord::GlobalRecord(const std::string& ID)
: BasicRecord(),
  recordID(ID),
  Type(globShort),
  shortVal(0),
  longVal(0),
  floatVal(0.0f)
{}

bool GlobalRecord::equals(const GlobalRecord& other) const
{
  if ((Type!=other.Type) or (lowerCaseCompare(other.recordID, recordID)!=0))
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
  throw std::logic_error("Unhandled case in GlobalRecord::equals!");
}

#ifndef MW_UNSAVEABLE_RECORDS
bool GlobalRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cGLOB, 4);
  uint32_t Size;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */ +1 /* length of FNAM */
        +4 /* FLTV */ +4 /* 4 bytes for length */ +4 /* length of FLTV */;
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */

  //write NAME
  output.write((const char*) &cNAME, 4);
  uint32_t SubLength = recordID.length()+1;
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);
  //write FNAM
  output.write((const char*) &cFNAM, 4);
  SubLength = 1;
  //write FNAM's length
  output.write((const char*) &SubLength, 4);
  //write FNAM (type of global)
  char var_type = '\0';
  switch (Type)
  {
    case globFloat:
         var_type = 'f';
         break;
    case globShort:
         var_type = 's';
         break;
    case globLong:
         var_type = 'l';
         break;
  }//swi
  output.write(&var_type, 1);
  //write FLTV
  output.write((const char*) &cFLTV, 4);
  SubLength = 4;
  //write FLTV's length
  output.write((const char*) &SubLength, 4);
  //write FLTV (value of global var)
  switch(Type)
  {
    case globFloat:
         output.write((const char*) &floatVal, 4);
         break;
    case globLong:
         output.write((const char*) &longVal, 4);
         break;
    case globShort:
         SubLength = shortVal;
         output.write((const char*) &SubLength, 4);
         break;
  }//swi
  return output.good();
}
#endif


bool GlobalRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */
  uint32_t SubRecName;
  uint32_t SubLength;
  SubRecName = SubLength = 0;
  char TypeChar = '\0';

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
    std::cout << "GlobalRecord::loadFromStream: Error: ID is longer than 255 characters.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes.\n";
    return false;
  }
  //read name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "GlobalRecord::loadFromStream: Error while reading ID from stream.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=1)
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be one byte.\n";
    return false;
  }
  in_File.read(&TypeChar, 1);
  if ((TypeChar != 'f') && (TypeChar != 'l') && (TypeChar != 's'))
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid value ("<<TypeChar
              <<"). Should be 'f', 'l' or 's'.\n";
    return false;
  }

  //read FLTV
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFLTV)
  {
    UnexpectedRecord(cFLTV, SubRecName);
    return false;
  }
  //FLTV's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record FLTV of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be four bytes.\n";
    return false;
  }
  //read value
  switch(TypeChar)
  {
    case 'f':
         in_File.read((char*) &floatVal, 4);
         Type = globFloat;
         break;
    case 'l':
         in_File.read((char*) &longVal, 4);
         Type = globLong;
         break;
    case 's':
         in_File.read((char*) &longVal, 4);
         shortVal = longVal;
         Type = globShort;
         break;
  }//swi

  if (!in_File.good())
  {
    std::cout << "Error while reading data of record GLOB.\n";
    return false;
  }
  return true;
} //loadFromStream

bool operator<(const GlobalRecord& left, const GlobalRecord& right)
{
  return (lowerCaseCompare(left.recordID, right.recordID)<0);
}

} //namespace
