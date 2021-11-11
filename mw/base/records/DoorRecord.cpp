/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2013, 2021  Thoronador

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

#include "DoorRecord.hpp"
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

DoorRecord::DoorRecord()
: BasicRecord(),
  recordID(std::string()),
  ModelPath(std::string()),
  Name(std::string()),
  Script(std::string()),
  SoundOpen(std::string()),
  SoundClose(std::string())
{}

bool DoorRecord::equals(const DoorRecord& other) const
{
  return (recordID == other.recordID) && (ModelPath == other.ModelPath)
      && (Name == other.Name) && (Script == other.Script)
      && (SoundOpen == other.SoundOpen) && (SoundClose == other.SoundClose);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool DoorRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cDOOR), 4);
  uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* MODL */ + 4 /* 4 bytes for length */
        + ModelPath.length() + 1 /* length of path +1 byte for NUL termination */;
  if (!Name.empty())
  {
    Size = Size + 4 /* FNAM */ + 4 /* 4 bytes for length */
        + Name.length() + 1 /* length of name +1 byte for NUL termination */;
  }
  if (!Script.empty())
  {
    Size = Size + 4 /* SCRI */ + 4 /* 4 bytes for length */
          + Script.length() + 1; /* length of script ID +1 byte for NUL termination */;
  }
  if (!SoundOpen.empty())
  {
    Size = Size + 4 /* SNAM */ + 4 /* 4 bytes for length */
          + SoundOpen.length() + 1; /* length of sound ID +1 byte for NUL termination */;
  }
  if (!SoundClose.empty())
  {
    Size = Size + 4 /* ANAM */ + 4 /* 4 bytes for length */
          + SoundClose.length() + 1; /* length of sound ID +1 byte for NUL termination */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Door:
    NAME = door ID
    MODL = NIF model filename
    FNAM = door name (optional in some rare cases)
    SCRI = Script (optional)
    SNAM = Sound name open (optional)
    ANAM = Sound name close (optional)
  */

  // write ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write model path (MODL)
  output.write(reinterpret_cast<const char*>(&cMODL), 4);
  SubLength = ModelPath.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(ModelPath.c_str(), SubLength);

  if (!Name.empty())
  {
    // write FNAM
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = Name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Name.c_str(), SubLength);
  }

  if (!Script.empty())
  {
    // write script ID (SCRI)
    output.write(reinterpret_cast<const char*>(&cSCRI), 4);
    SubLength = Script.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Script.c_str(), SubLength);
  }

  if (!SoundOpen.empty())
  {
    // write sound ID (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    SubLength = SoundOpen.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(SoundOpen.c_str(), SubLength);
  }

  if (!SoundClose.empty())
  {
    // write sound ID (ANAM)
    output.write(reinterpret_cast<const char*>(&cANAM), 4);
    SubLength = SoundClose.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(SoundClose.c_str(), SubLength);
  }
  return output.good();
}
#endif

bool DoorRecord::loadFromStream(std::istream& in_File)
{
  uint32_t Size = 0;
  in_File.read(reinterpret_cast<char*>(&Size), 4);
  in_File.read(reinterpret_cast<char*>(&HeaderOne), 4);
  in_File.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Door:
    NAME = door ID
    FNAM = door name (optional in some rare cases)
    MODL = NIF model filename
    SCRI = Script (optional)
    SNAM = Sound name open (optional)
    ANAM = Sound name close (optional)
  */

  uint32_t SubRecName = 0;
  uint32_t BytesRead = 0;

  // read door ID (NAME)
  char Buffer[256];
  if (!loadString256WithHeader(in_File, recordID, Buffer, cNAME, BytesRead))
    return false;

  // read optional part and records with varying order
  Name.clear();
  ModelPath.clear();
  Script.clear();
  SoundOpen.clear();
  SoundClose.clear();
  bool hasFNAM = false;
  bool hasMODL = false;
  bool hasSCRI = false;
  bool hasSNAM = false;
  bool hasANAM = false;
  while (BytesRead < Size)
  {
    // read next optional sub record name
    in_File.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cMODL:
           if (hasMODL)
           {
             std::cerr << "Error: Record DOOR seems to have two MODL subrecords.\n";
             return false;
           }
           if (!loadString256(in_File, ModelPath, Buffer, cMODL, BytesRead))
             return false;
           hasMODL = true;
           break;
      case cFNAM:
           if (hasFNAM)
           {
             std::cerr << "Error: Record DOOR seems to have two FNAM subrecords.\n";
             return false;
           }
           if (!loadString256(in_File, Name, Buffer, cFNAM, BytesRead))
             return false;
           hasFNAM = true;
           break;
      case cSCRI:
           if (hasSCRI)
           {
             std::cerr << "Error: Record DOOR seems to have two SCRI subrecords.\n";
             return false;
           }
           if (!loadString256(in_File, Script, Buffer, cSCRI, BytesRead))
             return false;
           hasSCRI = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cerr << "Error: Record DOOR seems to have two SNAM subrecords.\n";
             return false;
           }
           if (!loadString256(in_File, SoundOpen, Buffer, cSNAM, BytesRead))
             return false;
           hasSNAM = true;
           break;
      case cANAM:
           if (hasANAM)
           {
             std::cerr << "Error: Record DOOR seems to have two ANAM subrecords.\n";
             return false;
           }
           if (!loadString256(in_File, SoundClose, Buffer, cANAM, BytesRead))
             return false;
           hasANAM = true;
           break;
      default:
           std::cerr << "Error while reading DOOR: Expected record name SCRI, "
                     << "SNAM or ANAM was not found. Instead, \""
                     << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
           break;
    }
  }
  return in_File.good();
}

} // namespace
