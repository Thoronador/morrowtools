/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "ESMWriter.h"
#include <fstream>
#include <iostream>
#include "MW_Constants.h"
#include "Spells.h"

const int32_t cMasterFileFlag = 1;
const int32_t cPluginFileFlag = 0;

ESMWriter::ESMWriter()
{
  //empty
}

ESMWriter::~ESMWriter()
{
  //empty
}

bool ESMWriter::writeESM(const std::string& FileName, const bool IsMasterFile, const DepFileList& deps, const std::string& Description)
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    std::cout << "Error: could not open/create file \""<<FileName
              <<"\" for writing.\n";
    return false;
  }
  //write TES3
  output.write((char*) &cTES3, 4);
  //write size (308 bytes for fixed header without any master file dependencies)
  int32_t Size = 308;
  if (!deps.isEmpty())
  {
    unsigned int j;
    for (j=0; j<deps.getSize(); ++j)
    {
      Size = Size +4 /* MAST */ +4 /* size */
             +1 +deps.at(j).name.length() /* space for filename + NUL-termination */
             + 4 /* DATA */ +4 /* size */ +8 /* file size of master file */;
    }//for
  }//if deps available
  output.write((char*) &Size, 4);
  int32_t Header1, Flags;
  Header1 = Flags = 0;
  output.write((char*) &Header1, 4);
  output.write((char*) &Flags, 4);
  //Subrecord HEDR
  //write HEDR
  output.write((char*) &cHEDR, 4);
  //write size (300 bytes for fixed header without any master file dependencies
  Size = 300;
  output.write((char*) &Size, 4);
  //write version (1.3 for Tribunal/Bloodmoon, 1.2 for Morrowind)
  float Version = 1.2;
  output.write((char*) &Version, 4);
  //status value (is usually 1 for master files and 0 for plugin files)
  if (IsMasterFile)
  {
    Size = cMasterFileFlag;
  }
  else
  {
    Size = cPluginFileFlag;
  }
  output.write((char*) &Size, 4);
  //company name/ creator of the master file/ plugin file
  const char company[33] = "Thoronador\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  output.write(company, 32);
  //description (256 Bytes)
  std::string real_description = Description;
  if (Description.length()>255)
  {
    real_description = Description.substr(0,255);
    std::cout << "Info: Description was too long and was shortened to 255 characters.\n";
  }
  real_description = real_description + std::string(256-real_description.length(), '\0');
  output.write(real_description.c_str(), 256);
  //number of records
  Size = getTotalRecords();
  output.write((char*) &Size, 4);
  if (!output.good())
  {
    std::cout << "Error while writing header information for file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  //write dependencies
  unsigned int i;
  for (i=0; i<deps.getSize(); ++i)
  {
    // write MAST subrecord
    // -- MAST header
    output.write((char*) &cMAST, 4);
    // -- size of file name
    Size = deps.at(i).name.length()+1; //length of string + one byte for NUL-character
    output.write((char*) &Size, 4);
    // -- file name itself
    output.write(deps.at(i).name.c_str(), Size);
    // write DATA subrecord
    // -- DATA header
    output.write((char*) &cDATA, 4);
    // -- size (is always 8 bytes for an int64)
    Size = 8;
    output.write((char*) &Size, 4);
    // -- write file size (currently always zero)
    output.write((char*) &(deps.at(i).size), 8);
  }//for
  //now write the data
  if (!writeRecords(output))
  {
    std::cout << "Error while writing data records for file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  output.close();
  return true;
}//writeESM
