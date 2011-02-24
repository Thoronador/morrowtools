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

#include "ScriptRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

const char NULof32[32] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

ScriptRecord::ScriptRecord()
{
  //script header
  ScriptID = "";
  NumShorts = NumLongs = NumFloats = ScriptDataSize = LocalVarSize = 0;
  //end of script header
  LocalVars = "";
  ScriptData = NULL;
  ScriptText = "";
}

ScriptRecord::ScriptRecord(const ScriptRecord& source)
{
  //script header
  ScriptID = source.ScriptID;
  NumShorts = source.NumShorts;
  NumLongs = source.NumLongs;
  NumFloats = source.NumFloats;
  ScriptDataSize = source.ScriptDataSize;
  LocalVarSize = source.LocalVarSize;
  //end of script header
  LocalVars = source.LocalVars;
  //copy the script data
  if (source.ScriptData==NULL)
  {
    ScriptData = NULL;
  }
  else
  {
    ScriptData = new char[ScriptDataSize];
    memcpy(ScriptData, source.ScriptData, ScriptDataSize);
  }
  ScriptText = source.ScriptText;
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
  if ((ScriptID==other.ScriptID) and (NumShorts==other.NumShorts)
    and (NumLongs==other.NumFloats) and (NumFloats==other.NumFloats)
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

bool ScriptRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cSCPT, 4);
  int32_t Size;
  Size = 4 /* SCHD */ +4 /* 4 bytes for length */ +52 /* fixed length is 52 bytes */
        +4 /* SCDT */ +4 /* 4 bytes for length */ +ScriptDataSize
        +4 /* SCTX */ +4 /* 4 bytes for length */
        +ScriptText.length() /* length of text (no NUL termination) */;
  if (LocalVars!="")
  {
    Size = Size + 4 /* SCVR */ +4 /* 4 bytes for length */
          +LocalVars.length()+1; /* length of list +1 for NUL termination */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

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
  output.write((char*) &cSCHD, 4);
  int32_t SubLength = 52; /* length is fixed at 52 bytes */
  //write SCHD's length
  output.write((char*) &SubLength, 4);
  //write script header
  // ---- write ID
  output.write(ScriptID.c_str(), ScriptID.length()+1);
  //Do we need to fill the rest to reach 32 characters?
  if (ScriptID.length()+1<32)
  {
    output.write(NULof32, 32 - (ScriptID.length()+1));
  }
  // ---- write local var counts
  output.write((char*) &NumShorts, 4);
  output.write((char*) &NumLongs, 4);
  output.write((char*) &NumFloats, 4);
  // ---- write sizes
  output.write((char*) &ScriptDataSize, 4);
  output.write((char*) &LocalVarSize, 4);

  if (LocalVars!="")
  {
    //write SCVR
    output.write((char*) &cSCVR, 4);
    SubLength = LocalVars.length()+1; /* length of list +1 for NUL termination */
    //write SCVR's length
    output.write((char*) &SubLength, 4);
    //write script's variable list
    output.write(LocalVars.c_str(), SubLength);
  }

  //check if data is present
  if ((ScriptDataSize!=0) and (ScriptData==NULL))
  {
    std::cout << "Error while writing script record: script data is NULL, "
              << "but size is not zero.\n";
    return false;
  }

  //write SCDT
  output.write((char*) &cSCDT, 4);
  SubLength = ScriptDataSize;
  //write SCDT's length
  output.write((char*) &SubLength, 4);
  //write script compiled data
  output.write(ScriptData, SubLength);

  //write SCTX
  output.write((char*) &cSCTX, 4);
  SubLength = ScriptText.length(); /* length of text, no NUL-termination */
  //write SCTX's length
  output.write((char*) &SubLength, 4);
  //write script compiled data
  output.write(ScriptText.c_str(), SubLength);

  return output.good();
}

bool ScriptRecord::loadFromStream(std::ifstream& in_File)
{
  int32_t Size;
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

  int32_t SubRecName, SubLength, BytesRead;
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
  char * Buffer = NULL;
  Buffer = new char[256];
  memset(Buffer, '\0', 256);
  int32_t BufferSize = 256;
  // ---- read script name
  in_File.read(Buffer, 32);
  BytesRead += 32;
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record SCHD of SCPT.\n";
    delete[] Buffer;
    return false;
  }
  ScriptID = std::string(Buffer);
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
    delete[] Buffer;
    return false;
  }
  if (ScriptDataSize<0)
  {
    std::cout << "Error while reading sub record SCHD of SCPT: data size is "
              << "negative!\n";
    delete[] Buffer;
    return false;
  }


  //preset data
  LocalVars = "";
  ScriptText = "";
  if (ScriptData!=NULL)
  {
    delete[] ScriptData;
    ScriptData = NULL;
  }

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
           if (BufferSize<=SubLength)
           {
             delete[] Buffer;
             Buffer = new char[SubLength+1];
             BufferSize = SubLength+1;
           }//if
           memset(Buffer, '\0', SubLength+1);
           //read the var list
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading sub record SCVR of SCPT.\n";
             delete[] Buffer;
             return false;
           }
           LocalVars = std::string(Buffer);
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
             delete[] Buffer;
             return false;
           }//if
           //Do we have to resize the buffer?
           if (BufferSize<=SubLength)
           {
             delete[] Buffer;
             Buffer = new char[SubLength+1];
             BufferSize = SubLength+1;
           }//if
           memset(Buffer, '\0', SubLength+1);
           //read the script data
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading sub record SCDT of SCPT.\n";
             delete[] Buffer;
             return false;
           }
           //check script data
           if (ScriptData!=NULL)
           {
             std::cout << "Error: sub record SCDT of SCPT was read twice!\n";
             delete[] Buffer;
             return false;
           }
           //set script data
           // ---- allocate memory
           ScriptData = new char[ScriptDataSize];
           // ---- copy data from read buffer
           memcpy(ScriptData, Buffer, ScriptDataSize);
           break;
      case cSCTX:
           //SCTX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           //Do we have to resize the buffer?
           if (BufferSize<=SubLength)
           {
             delete[] Buffer;
             Buffer = new char[SubLength+1];
             BufferSize = SubLength+1;
           }//if
           memset(Buffer, '\0', SubLength+1);
           //read script text
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading sub record SCTX of SCPT.\n";
             delete[] Buffer;
             return false;
           }
           ScriptText = std::string(Buffer);
           break;
      default: //unknown record header, finish
           std::cout << "Unexpected sub record found. Expected SCDT, SCTX or "
                     << "SCVR, but "<<IntTo4Char(SubRecName)<<" was found.\n";
           delete[] Buffer;
           return false;
           break;
    }//switch
  }//while

  delete[] Buffer;
  return in_File.good();
}
