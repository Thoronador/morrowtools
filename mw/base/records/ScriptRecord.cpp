/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013  Thoronador

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

#include "ScriptRecord.h"
#include <iostream>
#include <cstring>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

ScriptRecord::ScriptRecord()
: BasicRecord(),
  //script header
  recordID(""),
  NumShorts(0),
  NumLongs(0),
  NumFloats(0),
  ScriptDataSize(0),
  LocalVarSize(0),
  //end of script header
  LocalVars(std::vector<std::string>()),
  ScriptData(NULL),
  ScriptText("")
{}

ScriptRecord::ScriptRecord(const ScriptRecord& source)
: BasicRecord(source),
  //script header
  recordID(source.recordID),
  NumShorts(source.NumShorts),
  NumLongs(source.NumLongs),
  NumFloats(source.NumFloats),
  ScriptDataSize(source.ScriptDataSize),
  LocalVarSize(source.LocalVarSize),
  //end of script header
  LocalVars(source.LocalVars),
  ScriptData(NULL),
  ScriptText(source.ScriptText)
{
  //copy the script data
  if (source.ScriptData!=NULL)
  {
    ScriptData = new char[ScriptDataSize];
    memcpy(ScriptData, source.ScriptData, ScriptDataSize);
  }
}

ScriptRecord& ScriptRecord::operator=(const ScriptRecord& rhs)
{
  if (this==&rhs) return *this;
  //script header
  recordID = rhs.recordID;
  NumShorts = rhs.NumShorts;
  NumLongs = rhs.NumLongs;
  NumFloats = rhs.NumFloats;
  ScriptDataSize = rhs.ScriptDataSize;
  LocalVarSize = rhs.LocalVarSize;
  //end of script header
  LocalVars = rhs.LocalVars;
  //copy the script data
  if (rhs.ScriptData==NULL)
  {
    /** I know there's a possible memory leak here in the next line, but
      changing that will crash the programme that uses lots of ScriptRecords
      for no obvious reason.
    */
    #warning Memory leak possibility!
    ScriptData = NULL;
  }
  else
  {
    /** I know there's a possible memory leak here in the next line, but
      changing that will crash the programme that uses lots of ScriptRecords
      for no obvious reason.
    */
    #warning Memory leak possibility!
    ScriptData = new char[ScriptDataSize];
    memcpy(ScriptData, rhs.ScriptData, ScriptDataSize);
  }
  ScriptText = rhs.ScriptText;
  return *this;
}

ScriptRecord::~ScriptRecord()
{
  //make sure the allocated memory will be freed
  if (ScriptData!=NULL)
  {
    delete[] ScriptData;
    ScriptData = NULL;
    ScriptDataSize = 0;
  }
}

bool ScriptRecord::equals(const ScriptRecord& other) const
{
  if ((recordID==other.recordID) and (NumShorts==other.NumShorts)
    and (NumLongs==other.NumLongs) and (NumFloats==other.NumFloats)
    and (ScriptDataSize==other.ScriptDataSize) and (LocalVarSize==other.LocalVarSize)
    and (LocalVars==other.LocalVars) and (ScriptText==other.ScriptText))
  {
    //check script data
    //Is one NULL, the other not?
    if ((ScriptData==NULL) xor (other.ScriptData==NULL)) return false;
    //So either both are NULL or none is NULL - if NULL, return true.
    if (ScriptData==NULL) return true;
    //compare memory
    return (0==memcmp(ScriptData, other.ScriptData, ScriptDataSize));
  }
  return false;
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ScriptRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cSCPT, 4);
  uint32_t Size;
  Size = 4 /* SCHD */ +4 /* 4 bytes for length */ +52 /* fixed length is 52 bytes */
        +4 /* SCDT */ +4 /* 4 bytes for length */ +ScriptDataSize
        +4 /* SCTX */ +4 /* 4 bytes for length */
        +ScriptText.length() /* length of text (no NUL termination) */;
  if (!LocalVars.empty())
  {
    Size = Size + 4 /* SCVR */ +4 /* 4 bytes for length */;
    unsigned int i;
    for (i=0; i<LocalVars.size(); ++i)
    {
      Size = Size + LocalVars.at(i).length()+1;/*length of string +1 for NUL termination*/
    }
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /*Script:
    SCHD = Script Header (52 bytes)
        char Name[32]
        long NumShorts
        long NumLongs
        long NumFloats
        long ScriptDataSize
        long LocalVarSize
    SCVR = List of all the local script variables seperated by '\0' NULL characters.
    SCDT = The compiled script data
    SCTX = Script text
      Note (thoronador): SCVR may not be present at all, if there are no local
          vars. Moreover, SCVR, SCDT and SCTX can occur in any order.*/

  //write SCHD
  output.write((const char*) &cSCHD, 4);
  uint32_t SubLength = 52; /* length is fixed at 52 bytes */
  //write SCHD's length
  output.write((const char*) &SubLength, 4);
  //write script header
  // ---- write ID
  uint_fast32_t ID_len = recordID.length()+1;
  if (ID_len>32) ID_len = 32;
  output.write(recordID.c_str(), ID_len);
  //Do we need to fill the rest to reach 32 characters?
  if (ID_len<32)
  {
    output.write(NULof32, 32 - ID_len);
  }
  // ---- write local var counts
  output.write((const char*) &NumShorts, 4);
  output.write((const char*) &NumLongs, 4);
  output.write((const char*) &NumFloats, 4);
  // ---- write sizes
  output.write((const char*) &ScriptDataSize, 4);
  output.write((const char*) &LocalVarSize, 4);

  if (!LocalVars.empty())
  {
    //write SCVR
    output.write((const char*) &cSCVR, 4);
    //length is sum of lengths of individual strings
    SubLength = 0;
    unsigned int i;
    for (i=0; i<LocalVars.size(); ++i)
    {
      SubLength += LocalVars.at(i).length()+1;/*length of string +1 for NUL termination*/
    }
    //write SCVR's length
    output.write((const char*) &SubLength, 4);
    //write script's variable list
    for (i=0; i<LocalVars.size(); ++i)
    {
      output.write(LocalVars.at(i).c_str(), LocalVars.at(i).length()+1);/*length of string +1 for NUL termination*/
    }
  }

  //check if data is present
  if ((ScriptDataSize!=0) and (ScriptData==NULL))
  {
    std::cout << "Error while writing script record: script data is NULL, "
              << "but size is not zero.\n";
    return false;
  }

  //write SCDT
  output.write((const char*) &cSCDT, 4);
  SubLength = ScriptDataSize;
  //write SCDT's length
  output.write((const char*) &SubLength, 4);
  //write script compiled data
  output.write(ScriptData, SubLength);

  //write SCTX
  output.write((const char*) &cSCTX, 4);
  SubLength = ScriptText.length(); /* length of text, no NUL-termination */
  //write SCTX's length
  output.write((const char*) &SubLength, 4);
  //write script compiled data
  output.write(ScriptText.c_str(), SubLength);

  return output.good();
}
#endif

bool ScriptRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Script:
    SCHD = Script Header (52 bytes)
        char Name[32]
        long NumShorts
        long NumLongs
        long NumFloats
        long ScriptDataSize
        long LocalVarSize
    SCVR = List of all the local script variables seperated by '\0' NULL characters.
    SCDT = The compiled script data
    SCTX = Script text
      Note (thoronador): SCVR may not be present at all, if there are no local
          vars. Moreover, SCVR, SCDT and SCTX can occur in any order.*/

  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read SCHD
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cSCHD)
  {
    UnexpectedRecord(cSCHD, SubRecName);
    return false;
  }
  //SCHD's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=52)
  {
    std::cout <<"Error: sub record SCHD of SCPT has invalid length ("<<SubLength
              <<" bytes). Should be 52 bytes.\n";
    return false;
  }
  //read script header
  char Buffer[256];
  memset(Buffer, '\0', 256);
  // ---- read script name
  in_File.read(Buffer, 32);
  BytesRead += 32;
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record SCHD of SCPT.\n";
    return false;
  }
  recordID = std::string(Buffer);
  // ---- read local var counts
  in_File.read((char*) &NumShorts, 4);
  in_File.read((char*) &NumLongs, 4);
  in_File.read((char*) &NumFloats, 4);
  // ---- read sizes
  in_File.read((char*) &ScriptDataSize, 4);
  in_File.read((char*) &LocalVarSize, 4);
  BytesRead += 20;
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record SCHD of SCPT (2nd part).\n";
    return false;
  }
  /* //won't occur with unsigned type any more
  if (ScriptDataSize<0)
  {
    std::cout << "Error while reading sub record SCHD of SCPT: data size is "
              << "negative!\n";
    return false;
  }*/

  uint32_t currentPos;
  //preset data
  LocalVars.clear();
  ScriptText.clear();
  if (ScriptData!=NULL)
  {
    delete[] ScriptData;
    ScriptData = NULL;
  }

  //dynamic buffer pointer for script data and vars
  char * dynamicBuffer = NULL;
  uint32_t dynamicBufferSize = 0;
  //optional records/ records with unknown sequence
  //read script sub records
  while (BytesRead<Size)
  {
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cSCVR:
           //SCVR's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           //Do we have to resize the buffer?
           if (dynamicBufferSize<=SubLength)
           {
             delete[] dynamicBuffer;
             dynamicBuffer = new char[SubLength+1];
             dynamicBufferSize = SubLength+1;
           }//if
           memset(dynamicBuffer, '\0', SubLength+1);
           //read the var list
           in_File.read(dynamicBuffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading sub record SCVR of SCPT.\n";
             delete[] dynamicBuffer;
             return false;
           }
           //add local vars
           currentPos = 0;
           do
           {
             //push next var
             LocalVars.push_back(&dynamicBuffer[currentPos]);
             //find end of previous var name in buffer
             while (dynamicBuffer[currentPos]!='\0')
             {
               ++currentPos;
             }//while
             ++currentPos; //puts currentPos one after NUL, i.e. to the position
                           // where the next string starts
           } while (currentPos<SubLength);
           //LocalVars = std::string(dynamicBuffer);
           break;
      case cSCDT:
           //SCDT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=ScriptDataSize)
           {
             std::cout << "Error: sub record SCDT of SCPT has not the proper "
                       << "size as given in header. Actual size is "<<SubLength
                       << " bytes, but it should be "<<ScriptDataSize<<"bytes.\n";
             delete[] dynamicBuffer;
             return false;
           }//if
           //Do we have to resize the buffer?
           if (dynamicBufferSize<=SubLength)
           {
             delete[] dynamicBuffer;
             dynamicBuffer = new char[SubLength+1];
             dynamicBufferSize = SubLength+1;
           }//if
           memset(dynamicBuffer, '\0', SubLength+1);
           //read the script data
           in_File.read(dynamicBuffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading sub record SCDT of SCPT.\n";
             delete[] dynamicBuffer;
             return false;
           }
           //check script data
           if (ScriptData!=NULL)
           {
             std::cout << "Error: sub record SCDT of SCPT was read twice!\n";
             delete[] dynamicBuffer;
             return false;
           }
           //set script data
           // ---- allocate memory
           ScriptData = new char[ScriptDataSize];
           // ---- copy data from read buffer
           memcpy(ScriptData, dynamicBuffer, ScriptDataSize);
           break;
      case cSCTX:
           //SCTX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           //Do we have to resize the buffer?
           if (dynamicBufferSize<=SubLength)
           {
             delete[] dynamicBuffer;
             dynamicBuffer = new char[SubLength+1];
             dynamicBufferSize = SubLength+1;
           }//if
           memset(dynamicBuffer, '\0', SubLength+1);
           //read script text
           in_File.read(dynamicBuffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading sub record SCTX of SCPT.\n";
             delete[] dynamicBuffer;
             return false;
           }
           ScriptText = std::string(dynamicBuffer);
           break;
      default: //unknown record header, finish
           std::cout << "Unexpected sub record found. Expected SCDT, SCTX or "
                     << "SCVR, but "<<IntTo4Char(SubRecName)<<" was found.\n";
           delete[] dynamicBuffer;
           return false;
           break;
    }//switch
  }//while

  delete[] dynamicBuffer;
  return in_File.good();
}

} //namespace
