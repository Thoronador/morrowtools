/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012 Thoronador

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

#include "TES3Record.h"
#include <cstring>
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

TES3Record::TES3Record()
: BasicRecord()
{
  Version = 1.2f;
  FileFlag = 0;
  companyName = "";
  description = "";
  NumRecords = 0;
  dependencies.clear();
}

bool TES3Record::equals(const TES3Record& other) const
{
  return ((Version==other.Version) and (FileFlag==other.FileFlag)
      and (companyName==other.companyName) and (description==other.description)
      and (NumRecords==other.NumRecords) and (dependencies==other.dependencies));
}

bool TES3Record::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cTES3, 4);
  uint32_t Size;
  Size = 4 /* HEDR */ +4 /* 4 bytes for length */ +300 /* fixed length is 300 bytes */;
  unsigned int i;
  for (i=0; i<dependencies.getSize(); ++i)
  {
    Size = Size + 4 /* MAST */ +4 /* 4 bytes for length */
           + dependencies.at(i).name.length()+1 /*length of string +1 byte for NUL-termination */
           + 4 /* MAST */ +4 /* 4 bytes for length */ +8 /* size of DATA (fixed at 8 bytes) */;
  }//for
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);

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

  //write HEDR
  output.write((char*) &cHEDR, 4);
  //HEDR's length
  uint32_t SubLength;
  SubLength = 300;//fixed length of 300 bytes
  output.write((char*) &SubLength, 4);
  //write Header data
  // ---- write version (usually 1.3 for Tribunal/Bloodmoon, 1.2 for Morrowind)
  output.write((char*) &Version, 4);
  // ---- write file flag
  output.write((char*) &FileFlag, 4);
  // ---- write company name/ creator of the master file/ plugin file
  const std::string::size_type comp_len = companyName.length();
  if (comp_len>31)
  {
    output.write(companyName.c_str(), 31);
    output.write('\0', 1);
    std::cout << "Info: Company/author name of TES3 was too long and was shortened to 31 characters.\n";
  }
  else
  {
    output.write(companyName.c_str(), comp_len);
    output.write(NULof32, 32 - comp_len);
  }
  // ---- write description (256 Bytes)
  std::string real_description = description;
  if (description.length()>255)
  {
    real_description = description.substr(0,255);
    std::cout << "Info: Description of TES3 was too long and was shortened to 255 characters.\n";
  }
  else
  {
    real_description = real_description + std::string(256-real_description.length(), '\0');
  }
  output.write(real_description.c_str(), 256);
  // ---- write number of records
  output.write((char*) &NumRecords, 4);
  if (!output.good())
  {
    std::cout << "Error while writing subrecord HEDR of TES3!\n";
    return false;
  }
  // ---- write dependencies
  for (i=0; i<dependencies.getSize(); ++i)
  {
    // write MAST subrecord
    // -- MAST header
    output.write((char*) &cMAST, 4);
    // -- size of file name
    Size = dependencies.at(i).name.length()+1; //length of string + one byte for NUL-character
    output.write((char*) &Size, 4);
    // -- file name itself
    output.write(dependencies.at(i).name.c_str(), Size);
    // write DATA subrecord
    // -- DATA header
    output.write((char*) &cDATA, 4);
    // -- size (is always 8 bytes for an int64)
    Size = 8;
    output.write((char*) &Size, 4);
    // -- write file size
    output.write((char*) &(dependencies.at(i).size), 8);
  }//for

  return output.good();
}

bool TES3Record::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size = 0;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  if (Size<308)
  {
    std::cout << "Error while reading TES3: size of TES3 record is less than "
              <<"308 bytes, it cannot contain a valid .esp/.esm file header.\n";
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

  int32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read HEDR
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cHEDR)
  {
    UnexpectedRecord(cHEDR, SubRecName);
    return false;
  }
  //HEDR's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=300)
  {
    std::cout << "Error: subrecord HEDR of TES3 has wrong size."
              << " Actual size: "<<SubLength<<" bytes. Must-have size: 300 bytes.\n";
    return false;
  }
  //read HEDR's stuff
  // ---- red version
  in_File.read((char*) &Version, 4);//version of file
  // ---- red flags
  in_File.read((char*) &FileFlag, 4);//file flag
  BytesRead += 8;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }
  // ---- read company name
  char Buffer[257];
  memset(Buffer, '\0', 33);
  in_File.read(Buffer, 32);//company name
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }
  companyName = std::string(Buffer);
  // ---- read description
  memset(Buffer, '\0', 257);
  in_File.read(Buffer, 256);//file description
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }
  description = std::string(Buffer);
  // ---- read total number of records
  in_File.read((char*) &NumRecords, 4);//total number of records
  BytesRead += (32+256+4);
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord HEDR of TES3!\n";
    return false;
  }

  //read the master files
  dependencies.clear();
  while (BytesRead<Size)
  {
    DepFile temp_file_info;
    //read MAST
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cMAST)
    {
      UnexpectedRecord(cMAST, SubRecName);
      return false;
    }
    //MAST's length
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength>256)
    {
      std::cout << "Error: subrecord MAST of TES3 is longer than 256 characters.\n";
      return false;
    }
    //read MAST file name
    memset(Buffer, '\0', 256);
    in_File.read(Buffer, SubLength);
    BytesRead += SubLength;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord MAST of TES3!\n";
      return false;
    }
    temp_file_info.name = std::string(Buffer);

    //reading DATA record
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    if (SubRecName!=cDATA)
    {
      UnexpectedRecord(cDATA, SubRecName);
      return false;
    }
    //reading size of DATA record
    in_File.read((char*) &SubLength, 4);
    BytesRead += 4;
    if (SubLength!=8)
    {
      std::cout << "Error: Subrecord DATA of TES3 has invalid size ("
                << SubLength <<" bytes). Should be 8 bytes.\n";
      return false;
    }
    //read 8 bytes of DATA
    in_File.read((char*) &(temp_file_info.size), 8);
    BytesRead += 8;
    if (!in_File.good())
    {
      std::cout << "Error while reading subrecord DATA of TES3.\n";
      return false;
    }
    temp_file_info.modified = -1; //guess/preset
    dependencies.push_back(temp_file_info);
  }//while

  return in_File.good();
}

} //namespace
