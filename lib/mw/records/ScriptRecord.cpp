/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021, 2025  Dirk Stolle

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

#include "ScriptRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

ScriptRecord::ScriptRecord()
: BasicRecord(),
  recordID(""),
  NumShorts(0),
  NumLongs(0),
  NumFloats(0),
  LocalVars(std::vector<std::string>()),
  ScriptData(ByteBuffer()),
  ScriptText("")
{}

uint32_t ScriptRecord::getLocalVarSize() const
{
  uint32_t varSize = 0;
  for (const auto& name: LocalVars)
  {
    varSize += name.size() + 1;
  }
  return varSize;
}

bool ScriptRecord::equals(const ScriptRecord& other) const
{
  return (recordID == other.recordID) && (NumShorts == other.NumShorts)
    && (NumLongs == other.NumLongs) && (NumFloats == other.NumFloats)
    && (LocalVars == other.LocalVars) && (ScriptData == other.ScriptData)
    && (ScriptText == other.ScriptText);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool ScriptRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSCPT), 4);
  const uint32_t ScriptDataSize = ScriptData.size();
  uint32_t Size = 4 /* SCHD */ + 4 /* 4 bytes for length */ + 52 /* data size */
        + 4 /* SCDT */ + 4 /* 4 bytes for length */ + ScriptDataSize
        + 4 /* SCTX */ + 4 /* 4 bytes for length */
        + ScriptText.length() /* length of text (no NUL termination) */;
  const uint32_t LocalVarSize = getLocalVarSize();
  if (!LocalVars.empty())
  {
    Size = Size + 4 /* SCVR */ + 4 /* 4 bytes for length */
         + LocalVarSize;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Script:
    SCHD = Script Header (52 bytes)
        char Name[32]
        long NumShorts
        long NumLongs
        long NumFloats
        long ScriptDataSize
        long LocalVarSize
    SCVR = List of all the local script variables separated by '\0' NULL characters.
    SCDT = The compiled script data
    SCTX = Script text
      Note (thoronador): SCVR may not be present at all, if there are no local
          vars. Moreover, SCVR, SCDT and SCTX can occur in any order.*/

  // write script header (SCHD)
  output.write(reinterpret_cast<const char*>(&cSCHD), 4);
  uint32_t SubLength = 52; /* length is fixed at 52 bytes */
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // ---- write ID
  uint_fast32_t ID_len = recordID.length() + 1;
  if (ID_len > 32)
    ID_len = 32;
  output.write(recordID.c_str(), ID_len);
  // Do we need to fill the rest to reach 32 characters?
  if (ID_len < 32)
  {
    output.write(NULof32, 32 - ID_len);
  }
  // ---- write local var counts
  output.write(reinterpret_cast<const char*>(&NumShorts), 4);
  output.write(reinterpret_cast<const char*>(&NumLongs), 4);
  output.write(reinterpret_cast<const char*>(&NumFloats), 4);
  // ---- write sizes
  output.write(reinterpret_cast<const char*>(&ScriptDataSize), 4);
  output.write(reinterpret_cast<const char*>(&LocalVarSize), 4);

  if (!LocalVars.empty())
  {
    // write script's variable names (SCVR)
    output.write(reinterpret_cast<const char*>(&cSCVR), 4);
    SubLength = LocalVarSize;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    // write script's variable list
    for (const auto& name: LocalVars)
    {
      // length of string +1 for NUL termination
      output.write(name.c_str(), name.length() + 1);
    }
  }

  // write compiled script data (SCDT)
  output.write(reinterpret_cast<const char*>(&cSCDT), 4);
  SubLength = ScriptDataSize;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(ScriptData.data()), SubLength);

  // write script text (SCTX)
  output.write(reinterpret_cast<const char*>(&cSCTX), 4);
  SubLength = ScriptText.length(); /* length of text, no NUL-termination */
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ScriptText.c_str(), SubLength);

  return output.good();
}
#endif

bool ScriptRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Script:
    SCHD = Script Header (52 bytes)
        char Name[32]
        long NumShorts
        long NumLongs
        long NumFloats
        long ScriptDataSize
        long LocalVarSize
    SCVR = List of all the local script variables separated by '\0' NULL characters.
    SCDT = The compiled script data
    SCTX = Script text
      Note (thoronador): SCVR may not be present at all, if there are no local
          vars. Moreover, SCVR, SCDT and SCTX can occur in any order.*/

  uint32_t SubRecName = 0;

  // read SCHD
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  uint32_t BytesRead = 4;
  if (SubRecName != cSCHD)
  {
    UnexpectedRecord(cSCHD, SubRecName);
    return false;
  }
  // SCHD's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 52)
  {
    std::cerr << "Error: Sub record SCHD of SCPT has invalid length ("
              << SubLength << " bytes). Should be 52 bytes.\n";
    return false;
  }
  // read script header
  char Buffer[256];
  memset(Buffer, '\0', 256);
  // ---- read script name
  input.read(Buffer, 32);
  BytesRead += 32;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record SCHD of SCPT.\n";
    return false;
  }
  recordID = std::string(Buffer);
  // ---- read local var counts
  input.read(reinterpret_cast<char*>(&NumShorts), 4);
  input.read(reinterpret_cast<char*>(&NumLongs), 4);
  input.read(reinterpret_cast<char*>(&NumFloats), 4);
  // ---- read sizes
  uint32_t ScriptDataSize = 0;
  input.read(reinterpret_cast<char*>(&ScriptDataSize), 4);
  uint32_t LocalVarSize = 0;
  input.read(reinterpret_cast<char*>(&LocalVarSize), 4);
  BytesRead += 20;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record SCHD of SCPT!\n";
    return false;
  }

  uint32_t currentPos;
  // clear possibly existing data
  LocalVars.clear();
  ScriptText.clear();
  ScriptData.reset();

  // dynamic buffer pointer for script data and vars
  char * dynamicBuffer = nullptr;
  uint32_t dynamicBufferSize = 0;
  while (BytesRead < Size)
  {
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cSCVR:
           // SCVR's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (LocalVarSize != SubLength)
           {
             std::cerr << "Error: Local variable size of script " << recordID
                       << " should be " << LocalVarSize << " byte(s) "
                       << "according to header data, but it is " << SubLength
                       << " bytes instead!\n";
             delete[] dynamicBuffer;
             return false;
           }
           // More than 64 KB for variable names is unlikely.
           // Check this to avoid huge memory allocations / out of memory.
           if (SubLength > 65536)
           {
             std::cerr << "Error: Local variable size of script " << recordID
                       << " exceeds 64 KB, it's " << SubLength << " bytes!\n";
             delete[] dynamicBuffer;
             return false;
           }
           // Do we have to resize the buffer?
           if (dynamicBufferSize <= SubLength)
           {
             delete[] dynamicBuffer;
             dynamicBuffer = new char[SubLength + 1];
             dynamicBufferSize = SubLength + 1;
           }
           memset(dynamicBuffer, '\0', SubLength + 1);
           // read the var list
           input.read(dynamicBuffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record SCVR of SCPT.\n";
             delete[] dynamicBuffer;
             return false;
           }
           // add local vars
           currentPos = 0;
           do
           {
             // push next var
             LocalVars.push_back(&dynamicBuffer[currentPos]);
             // find end of previous var name in buffer
             while (dynamicBuffer[currentPos] != '\0')
             {
               ++currentPos;
             }
             ++currentPos; //puts currentPos one after NUL, i.e. to the position
                           // where the next string starts
           } while (currentPos < SubLength);
           break;
      case cSCDT:
           // check for existing script data
           if (ScriptData.data() != nullptr)
           {
             std::cerr << "Error: Sub record SCDT of SCPT was read twice!\n";
             delete[] dynamicBuffer;
             return false;
           }
           // SCDT's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != ScriptDataSize)
           {
             std::cerr << "Error: Sub record SCDT of SCPT has not the proper "
                       << "size as given in header. Actual size is " << SubLength
                       << " bytes, but it should be " << ScriptDataSize << " bytes.\n";
             delete[] dynamicBuffer;
             return false;
           }
           // Do we have to resize the buffer?
           if (dynamicBufferSize <= SubLength)
           {
             delete[] dynamicBuffer;
             dynamicBuffer = new char[SubLength + 1];
             dynamicBufferSize = SubLength + 1;
           }
           memset(dynamicBuffer, '\0', SubLength + 1);
           // read the script data
           input.read(dynamicBuffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record SCDT of SCPT.\n";
             delete[] dynamicBuffer;
             return false;
           }
           // set script data
           ScriptData.copy_from(reinterpret_cast<uint8_t*>(dynamicBuffer), ScriptDataSize);
           break;
      case cSCTX:
           // SCTX's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           // Do we have to resize the buffer?
           if (dynamicBufferSize <= SubLength)
           {
             delete[] dynamicBuffer;
             dynamicBuffer = new char[SubLength + 1];
             dynamicBufferSize = SubLength + 1;
           }
           memset(dynamicBuffer, '\0', SubLength + 1);
           // read script text
           input.read(dynamicBuffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record SCTX of SCPT.\n";
             delete[] dynamicBuffer;
             return false;
           }
           ScriptText = std::string(dynamicBuffer);
           break;
      default: // unknown record header, failure
           std::cerr << "Unexpected sub record found. Expected SCDT, SCTX or "
                     << "SCVR, but " << IntTo4Char(SubRecName) << " was found.\n";
           delete[] dynamicBuffer;
           return false;
    }
  }

  delete[] dynamicBuffer;
  return input.good();
}

} // namespace
