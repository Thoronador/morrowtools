/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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
#include <iostream>

#ifndef SR_UNSAVEABLE_RECORDS
namespace SRTP
{

ESMWriter::~ESMWriter()
{
  //empty
}

bool ESMWriter::writeESM(const std::string& FileName, Tes4HeaderRecord& theHeader)
{
  theHeader.numRecordsAndGroups = getTotalNumberOfGroups() + getTotalNumberOfRecords();
  std::ofstream output;
  output.open(FileName, std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    std::cerr << "ESMWriter::writeESM: Error: could not open/create file \""
              << FileName << "\" for writing.\n";
    return false;
  }
  //write TES4 header
  if (!theHeader.saveToStream(output))
  {
    std::cerr << "ESMWriter::writeESM: Error while writing TES4 header for file \""
              << FileName << "\".\n";
    output.close();
    return false;
  }
  if (!writeGroups(output))
  {
    std::cerr << "ESMWriter::writeESM: Error while writing groups of file \""
              << FileName << "\".\n";
    output.close();
    return false;
  }
  const bool success = output.good();
  output.close();
  return success;
}

} //namespace
#endif // SR_UNSAVEABLE_RECORDS
