/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2013, 2021  Dirk Stolle

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

#include "TES3Record.hpp"
#include <cstring>
#include <iostream>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

TES3Record::TES3Record()
: BasicRecord(),
  Version(1.2f),
  FileFlag(0),
  companyName(std::string()),
  description(std::string()),
  NumRecords(0),
  dependencies(DepFileList())
{
}

bool TES3Record::equals(const TES3Record& other) const
{
  return (Version == other.Version) && (FileFlag == other.FileFlag)
      && (companyName == other.companyName) && (description == other.description)
      && (NumRecords == other.NumRecords) && (dependencies == other.dependencies);
}

bool TES3Record::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cTES3), 4);
  uint32_t Size = 4 /* HEDR */ + 4 /* 4 bytes for length */ + 300 /* fixed length is 300 bytes */;
  for (size_t i = 0; i < dependencies.getSize(); ++i)
  {
    Size = Size + 4 /* MAST */ + 4 /* 4 bytes for length */
           + dependencies.at(i).name.length() + 1 /* length of string + NUL */
           + 4 /* MAST */ + 4 /* 4 bytes for length */ + 8 /* size of DATA */;
  }
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

  /*TES3:
    HEDR (300 bytes)
        4 bytes, float Version (1.2)
        4 bytes, long FileFlag(?) (seems to be 1 for master and 0 for plugin file)
        32 Bytes, Company/ Author Name string
        256 Bytes, ESM file description
        4 bytes, long NumRecords (48227 for Morrowind.esm)
    MAST = string, variable length
        Only found in ESP plugins and specifies a master file that the plugin
        requires.  Can occur multiple times.  Usually found just after the TES3
        record.
    DATA = 8 Bytes  long64 MasterSize
        Size of the previous master file in bytes (used for version tracking of plugin).
        The MAST and DATA records are always found together, the DATA following the MAST record
        that it refers to. */

  // write header data (HEDR)
  output.write(reinterpret_cast<const char*>(&cHEDR), 4);
  // HEDR's length
  uint32_t SubLength = 300;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  // ---- write version (usually 1.3 for Tribunal/Bloodmoon, 1.2 for Morrowind)
  output.write(reinterpret_cast<const char*>(&Version), 4);
  // ---- write file flag
  output.write(reinterpret_cast<const char*>(&FileFlag), 4);
  // ---- write company name/ creator of the master file/ plugin file
  const auto comp_len = companyName.length();
  if (comp_len > 31)
  {
    output.write(companyName.c_str(), 31);
    output.write("\0", 1);
    std::cout << "Info: Company/author name of TES3 was too long and was shortened to 31 characters.\n";
  }
  else
  {
    output.write(companyName.c_str(), comp_len);
    output.write(NULof32, 32 - comp_len);
  }
  // ---- write description (256 Bytes)
  std::string real_description = description;
  if (description.length() > 255)
  {
    real_description = description.substr(0,255);
    std::cout << "Info: Description of TES3 was too long and was shortened to 255 characters.\n";
  }
  else
  {
    real_description = real_description + std::string(256 - real_description.length(), '\0');
  }
  output.write(real_description.c_str(), 256);
  // ---- write number of records
  output.write(reinterpret_cast<const char*>(&NumRecords), 4);
  if (!output.good())
  {
    std::cerr << "Error while writing subrecord HEDR of TES3!\n";
    return false;
  }
  // ---- write dependencies
  for (size_t i = 0; i < dependencies.getSize(); ++i)
  {
    // write MAST subrecord
    output.write(reinterpret_cast<const char*>(&cMAST), 4);
    SubLength = dependencies.at(i).name.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(dependencies.at(i).name.c_str(), SubLength);
    // write DATA subrecord
    output.write(reinterpret_cast<const char*>(&cDATA), 4);
    SubLength = 8;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&dependencies.at(i).size), 8);
  }

  return output.good();
}

bool TES3Record::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);

  if (Size < 308)
  {
    std::cerr << "Error while reading TES3: size of TES3 record is less than "
              << "308 bytes, it cannot contain a valid .esp/.esm file header.\n";
    return false;
  }

  /*TES3:
    HEDR (300 bytes)
        4 bytes, float Version (1.2)
        4 bytes, long FileFlag(?) (seems to be 1 for master and 0 for plugin file)
        32 Bytes, Company/ Author Name string
        256 Bytes, ESM file description
        4 bytes, long NumRecords (48227 for Morrowind.esm)
    MAST = string, variable length
        Only found in ESP plugins and specifies a master file that the plugin
        requires.  Can occur multiple times.  Usually found just after the TES3
        record.
    DATA = 8 Bytes  long64 MasterSize
        Size of the previous master file in bytes (used for version tracking of plugin).
        The MAST and DATA records are always found together, the DATA following the MAST record
        that it refers to. */

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;

  // read HEDR
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  uint32_t BytesRead = 4;
  if (SubRecName != cHEDR)
  {
    UnexpectedRecord(cHEDR, SubRecName);
    return false;
  }
  // HEDR's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 300)
  {
    std::cerr << "Error: Subrecord HEDR of TES3 has wrong size."
              << " Actual size: " << SubLength << " bytes. Must-have size: 300 bytes.\n";
    return false;
  }
  //read HEDR's stuff
  // ---- read version
  input.read(reinterpret_cast<char*>(&Version), 4);
  // ---- read flags
  input.read(reinterpret_cast<char*>(&FileFlag), 4);
  BytesRead += 8;
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }
  // ---- read company name
  char Buffer[257];
  memset(Buffer, '\0', 33);
  input.read(Buffer, 32);
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }
  companyName = std::string(Buffer);
  // ---- read description
  memset(Buffer, '\0', 257);
  input.read(Buffer, 256);
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }
  description = std::string(Buffer);
  // ---- read total number of records
  input.read(reinterpret_cast<char*>(&NumRecords), 4);
  BytesRead += (32 + 256 + 4);
  if (!input.good())
  {
    std::cerr << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }

  // read the master files
  dependencies.clear();
  while (BytesRead < Size)
  {
    DepFile temp_file_info;
    // read MAST
    if (!loadString256WithHeader(input, temp_file_info.name, Buffer, cMAST, BytesRead))
    {
      std::cerr << "Error while reading subrecord MAST of TES3!\n";
      return false;
    }

    // reading DATA record
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    if (SubRecName != cDATA)
    {
      UnexpectedRecord(cDATA, SubRecName);
      return false;
    }
    // reading size of DATA record
    input.read(reinterpret_cast<char*>(&SubLength), 4);
    BytesRead += 4;
    if (SubLength != 8)
    {
      std::cerr << "Error: Subrecord DATA of TES3 has invalid size ("
                << SubLength << " bytes). Should be 8 bytes.\n";
      return false;
    }
    // read 8 bytes of DATA
    input.read(reinterpret_cast<char*>(&temp_file_info.size), 8);
    BytesRead += 8;
    if (!input.good())
    {
      std::cerr << "Error while reading subrecord DATA of TES3.\n";
      return false;
    }
    temp_file_info.modified = -1; // guess / preset
    dependencies.push_back(temp_file_info);
  }

  return input.good();
}

} // namespace
