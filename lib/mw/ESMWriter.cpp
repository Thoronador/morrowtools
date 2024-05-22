/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2024  Dirk Stolle

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

#include "ESMWriter.hpp"
#include <fstream>
#include <iostream>
#include "MW_Constants.hpp"
#include "Spells.hpp"

namespace MWTP
{

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

bool ESMWriter::writeESM(const std::string& FileName, const bool IsMasterFile, TES3Record& headerData)
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    std::cerr << "Error: could not open/create file \"" << FileName
              << "\" for writing.\n";
    return false;
  }
  //adjust header data slightly
  // ---- status value (is usually 1 for master files and 0 for plugin files)
  if (IsMasterFile)
  {
    headerData.FileFlag = cMasterFileFlag;
  }
  else
  {
    headerData.FileFlag = cPluginFileFlag;
  }
  // ---- header1 and flags
  headerData.setHeaderOne(0);
  headerData.setHeaderFlags(0);
  // ---- version (1.3 for Tribunal/Bloodmoon, 1.2 for Morrowind)
  if (headerData.Version>1.3f)
  {
    headerData.Version = 1.3f;
  }
  else if (headerData.Version<1.2f)
  {
    headerData.Version = 1.2f;
  }
  // ---- company name/ creator of the master file/ plugin file
  //const char company[33] = "Thoronador\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  // ---- number of records
  headerData.NumRecords = getTotalRecords();

  //write TES3 header data
  if (!headerData.saveToStream(output))
  {
    std::cout << "Error while writing header information for file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }

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

void ESMWriter::setOldStyleHeaderData(TES3Record& headerData, const bool IsMasterFile, const DepFileList& deps, const std::string& Description)
{
  if (IsMasterFile)
  {
    headerData.FileFlag = cMasterFileFlag;
  }
  else
  {
    headerData.FileFlag = cPluginFileFlag;
  }
  headerData.dependencies = deps;
  headerData.description = Description;
  headerData.companyName = "Thoronador";
  headerData.setHeaderOne(0);
  headerData.setHeaderFlags(0);
}

} //namespace
