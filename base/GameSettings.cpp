/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#include "GameSettings.h"
#include <iostream>
#include "MW_Constants.h"
#include "HelperIO.h"

bool GMSTRecord::equals(const GMSTRecord& other) const
{
  if (Type!=other.Type)
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
  throw 42;
}

bool GMSTRecord::saveToStream(std::ofstream& output, const std::string& GMSTID) const
{
  //write GMST
  output.write((char*) &cGMST, 4);
  int32_t Size, HeaderOne, HeaderFlags;
  HeaderOne = HeaderFlags = 0;
  Size = 4 /* NAME */ +4 /* four bytes for length */
        +GMSTID.length() /* length of ID */
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
  output.write((char*) &Size, 4);
  #warning HeaderOne and H_Flags might not be initialized properly!
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  //write NAME
  output.write((char*) &cNAME, 4);
  //NAME's length
  int32_t SubLength;
  SubLength = GMSTID.length(); //length of string
  output.write((char*) &SubLength, 4);
  //write ID
  output.write(GMSTID.c_str(), SubLength);
  //write STRV/ FLTV/ INTV
  switch(Type)
  {
    case gtInteger:
         //write INTV
         output.write((char*) &cINTV, 4);
         //write length
         SubLength = 4;
         output.write((char*) &SubLength, 4);
         //write integer
         output.write((char*) &iVal, 4);
         break;
    case gtFloat:
         //write FLTV
         output.write((char*) &cFLTV, 4);
         //write length
         SubLength = 4;
         output.write((char*) &SubLength, 4);
         //write float
         output.write((char*) &fVal, 4);
         break;
    case gtString:
         //write STRV
         output.write((char*) &cSTRV, 4);
         //write length
         SubLength = sVal.length();
         output.write((char*) &SubLength, 4);
         //write string
         output.write(sVal.c_str(), SubLength);
         break;
  }//swi
  return output.good();
}

GameSettings::GameSettings()
{
  //empty
}

GameSettings::~GameSettings()
{
  m_Settings.clear();
}

GameSettings& GameSettings::getSingleton()
{
  static GameSettings Instance;
  return Instance;
}

void GameSettings::addSetting(const std::string& Name, const GMSTRecord& value)
{
  m_Settings[Name] = value;
}

bool GameSettings::hasSetting(const std::string& Name) const
{
  return m_Settings.find(Name)!=m_Settings.end();
}

const GMSTRecord& GameSettings::getSetting(const std::string& Name) const
{
  std::map<std::string, GMSTRecord>::const_iterator iter;
  iter = m_Settings.find(Name);
  if (iter!=m_Settings.end())
  {
    return iter->second;
  }
  std::cout << "Error: setting \""<<Name<<"\" not found.\n";
  throw 42;
}

unsigned int GameSettings::getNumberOfSettings() const
{
  return m_Settings.size();
}

bool GameSettings::readGMST(std::ifstream& in_File, const int32_t FileSize)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 bytes)/FLTV = Float value (4 bytes)
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
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record NAME of GMST!\n";
    return false;
  }
  const std::string SettingName = std::string(Buffer);
  const char NameFirstChar = SettingName.at(0);

  GMSTRecord temp;
  temp.fVal = 0.0f;
  temp.iVal = 0;
  temp.sVal = "";

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
         in_File.read((char*) &(temp.fVal), 4);
         temp.Type = gtFloat;
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
         in_File.read((char*) &(temp.iVal), 4);
         temp.Type = gtInteger;
         break;
    default: //'s'
         temp.Type = gtString;
         // if string is empty, STRV sub-record may not be present at all
         if (in_File.tellg()>=FileSize)
         {
           addSetting(SettingName, temp);
           return in_File.good();
         }
         //read STRV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName==cSTRV)
         {
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
           temp.sVal = std::string(Buffer);
         }
         else
         { //seek four bytes towards beginning to be before next record name
           in_File.seekg(-4, std::ios::cur);
         }
         break;
  }//switch
  if (in_File.good())
  {
    addSetting(SettingName, temp);
    return true;
  }
  return false;
}//readGMST

int GameSettings::readRecordGMST(std::ifstream& in_File, const int32_t FileSize)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return -1;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>511)
  {
    std::cout << "Sub record NAME of GMST is longer than 511 characters!\n";
    return -1;
  }
  if (SubLength<=0)
  {
    std::cout << "Sub record NAME of GMST is shorter than one character!\n";
    return -1;
  }
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record NAME of GMST!\n";
    return -1;
  }
  const std::string SettingName = std::string(Buffer);
  const char NameFirstChar = SettingName.at(0);

  GMSTRecord temp;
  temp.fVal = 0.0f;
  temp.iVal = 0;
  temp.sVal = "";

  //read value record based on first character of setting name
  switch(NameFirstChar)
  {
    case 'f':
         //read FLTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cFLTV)
         {
           UnexpectedRecord(cFLTV, SubRecName);
           return -1;
         }
         //FLTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record FLTV of GMST has invalid length ("
                     << SubLength << " bytes). Should be 4 bytes.\n";
           return -1;
         }
         //read FLTV
         in_File.read((char*) &(temp.fVal), 4);
         temp.Type = gtFloat;
         break;
    case 'i':
         //read INTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cINTV)
         {
           UnexpectedRecord(cINTV, SubRecName);
           return -1;
         }
         //INTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record INTV of GMST has invalid length ("
                     << SubLength << " bytes). Should be 4 bytes.\n";
           return -1;
         }
         //read INTV
         in_File.read((char*) &(temp.iVal), 4);
         temp.Type = gtInteger;
         break;
    default: //'s'
         temp.Type = gtString;
         // if string is empty, STRV sub-record may not be present at all
         if (in_File.tellg()<FileSize)
         {
           //read STRV
           in_File.read((char*) &SubRecName, 4);
           if (SubRecName==cSTRV)
           {
             //STRV's length
             in_File.read((char*) &SubLength, 4);
             if (SubLength>511)
             {
               std::cout << "Sub record STRV of GMST is longer than 511 characters!\n";
               return -1;
             }
             //read STRV
             memset(Buffer, '\0', 512);
             in_File.read(Buffer, SubLength);
             temp.sVal = std::string(Buffer);
           }
           else
           { //seek four bytes towards beginning to be before next record name
             in_File.seekg(-4, std::ios::cur);
           }
         }
         break;
  }//switch
  if (in_File.good())
  {
    //check for presence of setting
    if (hasSetting(SettingName))
    {
      if (getSetting(SettingName).equals(temp))
      {
        return 0; //return zero, nothing was changed
      }
    }//if
    addSetting(SettingName, temp);
    return 1;
  }
  std::cout << "Error while reading data subrecord of GMST.\n";
  return -1;
}//readRecordGMST (integer version of readGMST)


bool GameSettings::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "GameSettings::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  GMSTListIterator iter = m_Settings.begin();
  const GMSTListIterator end_iter = m_Settings.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output, iter->first))
    {
      std::cout << "GameSettings::saveAllToStream: Error while writing record.\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}

GMSTListIterator GameSettings::getBegin() const
{
  return m_Settings.begin();
}

GMSTListIterator GameSettings::getEnd() const
{
  return m_Settings.end();
}

void GameSettings::clearAll()
{
  m_Settings.clear();
}
