/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011, 2012, 2023  Dirk Stolle

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

#include "SoundRecord.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

SoundRecord::SoundRecord()
: BasicRecord(),
  recordID(""),
  FileName(""),
  Volume(0),
  MinRange(0),
  MaxRange(0)
{}

bool SoundRecord::equals(const SoundRecord& other) const
{
  return (recordID == other.recordID) && (FileName == other.FileName)
      && (Volume == other.Volume) && (MinRange == other.MinRange)
      && (MaxRange == other.MaxRange);
}

#ifndef MW_UNSAVEABLE_RECORDS
bool SoundRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cSOUN), 4);
  const uint32_t Size = 4 /* NAME */ + 4 /* 4 bytes for length */
        + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
        + 4 /* FNAM */ + 4 /* 4 bytes for FNAM's length */
        + FileName.length() + 1 /*length of name plus one for NUL-termination */
        + 4 /* DATA */ + 4 /* DATA's length */ + 3 /*size of sound data (DATA)*/;
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*Sound:
    NAME = Sound ID
    FNAM = Sound Filename (relative to Sounds\)
    DATA = Sound Data (3 bytes)
        byte Volume (0=0.00, 255=1.00)
        byte MinRange
        byte MaxRange*/

  // write sound name / ID (NAME)
  output.write(reinterpret_cast<const char*>(&cNAME), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write file name (FNAM)
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  SubLength = FileName.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(FileName.c_str(), SubLength);

  // write sound data (DATA)
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  SubLength = 3; // length of DATA (sound data) is always three bytes
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&Volume), 1);
  output.write(reinterpret_cast<const char*>(&MinRange), 1);
  output.write(reinterpret_cast<const char*>(&MaxRange), 1);

  return output.good();
}
#endif

bool SoundRecord::loadFromStream(std::istream& input)
{
  uint32_t Size { 0 };
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  /*Sound:
    NAME = Sound ID
    FNAM = Sound Filename (relative to Sounds\)
    DATA = Sound Data (3 bytes)
        byte Volume (0=0.00, 255=1.00)
        byte MinRange
        byte MaxRange*/

  // read name / id (NAME)
  char Buffer[256];
  uint32_t BytesRead { 0 };
  if (!loadString256WithHeader(input, recordID, Buffer, cNAME, BytesRead))
  {
    std::cerr << "Error while reading sub record NAME of SOUN.\n";
    return false;
  }

  // read file name (FNAM)
  if (!loadString256WithHeader(input, FileName, Buffer, cFNAM, BytesRead))
  {
    std::cerr << "Error while reading sub record FNAM of SOUN.\n";
    return false;
  }

  // read sound data (DATA)
  uint32_t SubRecName = 0;
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  if (SubRecName != cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  // DATA's length
  uint32_t SubLength = 0;
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  if (SubLength != 3)
  {
    std::cerr << "Error: Sub record DATA of SOUN has invalid length ("
              << SubLength << " bytes). Should be 3 bytes.\n";
    return false;
  }
  // read sound data
  input.read(reinterpret_cast<char*>(&Volume), 1);
  input.read(reinterpret_cast<char*>(&MinRange), 1);
  input.read(reinterpret_cast<char*>(&MaxRange), 1);
  if (!input.good())
  {
    std::cerr << "Error while reading sub record DATA of SOUN.\n";
    return false;
  }
  return true;
}

double SoundRecord::normalizedVolume() const
{
  return static_cast<double>(Volume) / 255.0;
}

} // namespace
