/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ESMWriter.h"
#include <iostream>

namespace SRTP
{

ESMWriter::ESMWriter()
{
  //empty
}

ESMWriter::~ESMWriter()
{
  //empty
}

bool ESMWriter::writeESM(const std::string& FileName, Tes4HeaderRecord& theHeader)
{
  theHeader.numRecordsAndGroups = 1+ getTotalNumberOfGroups() + getTotalNumberOfRecords();
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    std::cout << "ESMWriter::writeESM: Error: could not open/create file \""
              << FileName<<"\" for writing.\n";
    return false;
  }
  //write TES4 header
  if (!theHeader.saveToStream(output))
  {
    std::cout << "ESMWriter::writeESM: Error while writing TES4 header for file \""
              << FileName<<"\".\n";
    output.close();
    return false;
  }
  if (!writeGroups(output))
  {
    std::cout << "ESMWriter::writeESM: Error while writing groups of file \""
              << FileName <<"\".\n";
    output.close();
    return false;
  }
  const bool success = output.good();
  output.close();
  return success;
}

} //namespace
