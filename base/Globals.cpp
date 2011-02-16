/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011 Thoronador

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

#include "Globals.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

GlobalRecord::GlobalRecord()
{
  Type = globShort;
  shortVal = 0;
  longVal = 0;
  floatVal = 0.0f;
}

bool GlobalRecord::equals(const GlobalRecord& other) const
{
  if (Type!=other.Type)
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

bool GlobalRecord::saveToStream(std::ofstream& output, const std::string& GlobalID) const
{
  output.write((char*) &cGLOB, 4);
  int32_t Size, HeaderOne, H_Flags;
  HeaderOne = H_Flags = 0;
  Size = 4 /* NAME */ +4 /* 4 bytes for length */
        +GlobalID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* FNAM */ +4 /* 4 bytes for length */ +1 /* length of FNAM */
        +4 /* FLTV */ +4 /* 4 bytes for length */ +4 /* length of FLTV */;
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &H_Flags, 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */

  //write NAME
  output.write((char*) &cNAME, 4);
  int32_t SubLength = GlobalID.length()+1;
  //write NAME's length
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(GlobalID.c_str(), SubLength);
  //write FNAM
  output.write((char*) &cFNAM, 4);
  SubLength = 1;
  //write FNAM's length
  output.write((char*) &SubLength, 4);
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
  output.write((char*) &cFLTV, 4);
  SubLength = 4;
  //write FLTV's length
  output.write((char*) &SubLength, 4);
  //write FLTV (value of global var)
  switch(Type)
  {
    case globFloat:
         output.write((char*) &floatVal, 4);
         break;
    case globLong:
         output.write((char*) &longVal, 4);
         break;
    case globShort:
         SubLength = shortVal;
         output.write((char*) &SubLength, 4);
         break;
  }//swi
  return output.good();
}

Globals::Globals()
{
  //empty
}

Globals::~Globals()
{
  //empty
}

Globals& Globals::getSingleton()
{
  static Globals Instance;
  return Instance;
}

void Globals::addGlobal(const std::string& ID, const GlobalRecord& record)
{
  if (ID!="")
  {
    m_Globals[ID] = record;
  }
}

bool Globals::hasGlobal(const std::string& ID) const
{
  return (m_Globals.find(ID)!=m_Globals.end());
}

unsigned int Globals::getNumberOfGlobals() const
{
  return m_Globals.size();
}

const GlobalRecord& Globals::getGlobal(const std::string& ID) const
{
  std::map<std::string, GlobalRecord>::const_iterator iter = m_Globals.find(ID);
  if (iter!=m_Globals.end())
  {
    return iter->second;
  }
  std::cout << "No apparatus with the ID \""<<ID<<"\" is present.\n";
  throw 42;
}

GlobalListIterator Globals::getBegin() const
{
  return m_Globals.begin();
}

GlobalListIterator Globals::getEnd() const
{
  return m_Globals.end();
}

bool Globals::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Globals::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  GlobalListIterator iter = m_Globals.begin();
  const GlobalListIterator end_iter = m_Globals.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output, iter->first))
    {
      std::cout << "Globalss::saveAllToStream: Error while writing record for \""
                << iter->first <<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

void Globals::clearAll()
{
  m_Globals.clear();
}

int Globals::readRecordGLOB(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Global variable:
    NAME = Global ID
    FNAM = Type of global (1 byte)
        's' = short
        'l' = long
        'f' = float
    FLTV = Float data (4 bytes) */
  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;
  char TypeChar = '\0';

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return -1;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "readRecordGLOB: Error: ID is longer than 255 characters.\n";
    std::cout << "File pos.: "<<in_File.tellg()<<" bytes\n";
    return -1;
  }
  //read name
  char Buffer[256];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "readRecordGLOB: Error while reading ID from stream.\n";
    std::cout << "File position: "<<in_File.tellg()<<" bytes\n";
    return -1;
  }
  const std::string GlobalID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return -1;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=1)
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be one byte.\n";
    return -1;
  }
  in_File.read(&TypeChar, 1);
  if ((TypeChar != 'f') && (TypeChar != 'l') && (TypeChar != 's'))
  {
    std::cout <<"Error: sub record FNAM of GLOB has invalid value ("<<TypeChar
              <<"). Should be 'f', 'l' or 's'.\n";
    return -1;
  }

  //read FLTV
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFLTV)
  {
    UnexpectedRecord(cFLTV, SubRecName);
    return -1;
  }
  //FLTV's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record FLTV of GLOB has invalid length ("<<SubLength
              <<" bytes). Should be four bytes.\n";
    return -1;
  }
  //read value
  GlobalRecord temp;
  switch(TypeChar)
  {
    case 'f':
         in_File.read((char*) &(temp.floatVal), 4);
         temp.Type = globFloat;
         break;
    case 'l':
         in_File.read((char*) &(temp.longVal), 4);
         temp.Type = globLong;
         break;
    case 's':
         in_File.read((char*) &(temp.longVal), 4);
         temp.shortVal = temp.longVal;
         temp.Type = globShort;
         break;
  }//swi

  if (!in_File.good())
  {
    std::cout << "Error while reading data of record GLOB.\n";
    return -1;
  }
  //add it to the list, if not present with same data
  if (hasGlobal(GlobalID))
  {
    if (getGlobal(GlobalID).equals(temp))
    {
      //same record with equal data is already present, return zero
      return 0;
    }
  }//if apparatus present
  addGlobal(GlobalID, temp);
  return 1;
} //readRecordAPPA
