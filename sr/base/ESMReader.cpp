/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021, 2022  Dirk Stolle

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

#include "ESMReader.hpp"
#include <iostream>
#include "SR_Constants.hpp"
#include "TableUtilities.hpp"
#include "../../mw/base/HelperIO.hpp"

namespace SRTP
{

ESMReader::ESMReader()
: currentHead(Tes4HeaderRecord())
{
}

int ESMReader::skipRecord(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.seekg(Size + 16, std::ios::cur);
  if (input.good())
  {
    return 0;
  }
  return -1;
}

int ESMReader::skipGroup(std::istream& input, const GroupData& g_data)
{
  if (g_data.size() < 24)
  {
    std::cerr << "ESMReader::skipGroup: Error: Group size is below 24 bytes!\n";
    return -1;
  }
  // skip it
  input.seekg(g_data.size() - 24, std::ios::cur);
  if (input.good())
  {
    return 0;
  }
  return -1;
}

int ESMReader::readESM(const std::string& FileName, Tes4HeaderRecord& head, const std::optional<Localization>& l10n)
{
  std::ifstream input;
  input.open(FileName, std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cerr << "Error: Could not open file \"" << FileName << "\".\n";
    return -1;
  }

  // set file pointer to end
  input.seekg(0, std::ios::end);
  const uint32_t FileSize = input.tellg();
  // reset file pointer to start of file
  input.seekg(0, std::ios::beg);

  // read the header name
  // -> TES4
  uint32_t recordName = 0;
  input.read(reinterpret_cast<char*>(&recordName), 4);
  if (recordName != cTES4)
  {
    std::cerr << "Error: File \"" << FileName << "\" is not a valid .esp/.esm file."
              << " Expected TES4, but \"" << IntTo4Char(recordName)
              << "\" was found instead.\n";
    input.close();
    return -1;
  }
  // now read the actual header
  if (!head.loadFromStream(input, true, StringTable()))
  {
    std::cerr << "Error: Could not read header from \"" << FileName << "\".\n";
    input.close();
    return -1;
  }

  const bool localized = head.isLocalized();
  StringTable table;
  if (localized)
  {
    if (!loadStringTables(FileName, table, l10n))
    {
      input.close();
      std::cerr << "Error while reading string tables for " << FileName << "!\n";
      return -1;
    }
  } // if localized

  // save header for subsequent functions that might need it
  currentHead = head;

  // read the groups/ records
  uint32_t processedGroups = 0;
  int lastResult = 0;
  uint32_t lastGoodPosition = input.tellg();
  while ((input.tellg() < FileSize) && (lastResult >= 0))
  {
    // try to read or skip a group - possibly that won't always work
    lastResult = processGroup(input, true, localized, table);
    if (lastResult >= 0)
    {
      processedGroups += lastResult;
      lastGoodPosition = input.tellg();
    }
  }
  // finished here
  const bool good_result = (input.good() && (lastResult >= 0));
  input.close();
  if (!good_result)
  {
    std::cerr << "Error: readESM of file \"" << FileName << "\" failed. Last known "
              << "good position was " << lastGoodPosition << ".\n";
    return -1;
  }
  // save data and return
  return processedGroups;
}

bool ESMReader::peekESMHeader(const std::string& FileName, Tes4HeaderRecord& head)
{
  std::ifstream input;
  input.open(FileName, std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cerr << "ESMReader::peekESMHeader: Error: Could not open file \""
              << FileName << "\".\n";
    return false;
  }

  // read the header name
  // TES4
  uint32_t recordName = 0;
  input.read(reinterpret_cast<char*>(&recordName), 4);
  if (recordName != cTES4)
  {
    std::cerr << "ESMReader::peekESMHeader: Error: File \"" << FileName
              << "\" is not a valid .esp/.esm file. Expected TES4, but \""
              << IntTo4Char(recordName) << "\" was found instead.\n";
    input.close();
    return false;
  }
  // now read the actual header
  if (!head.loadFromStream(input, true, StringTable()))
  {
    std::cerr << "ESMReader::peekESMHeader: Error: Could not read header from \""
              << FileName << "\".\n";
    input.close();
    return false;
  }

  input.close();
  return true;
}

int ESMReader::processGroup(std::istream& input, const bool withHeader, const bool localized, const StringTable& table)
{
  if (withHeader)
  {
    uint32_t recordHeader = 0;
    // read "GRUP"
    input.read(reinterpret_cast<char*>(&recordHeader), 4);
    if (!input.good())
    {
      std::cerr << "ESMReader::processGroup: Error: Could not read group header!\n";
      return -1;
    }
    if (recordHeader != cGRUP)
    {
      UnexpectedRecord(cGRUP, recordHeader);
      return -1;
    }
  }
  // now read the group data header
  GroupData gd;
  if (!gd.loadFromStream(input))
  {
    std::cerr << "ESMReader::processGroup: Error: Could not read group data!\n";
    return -1;
  }
  if (needGroup(gd))
  {
    if (!nextGroupStarted(gd, !withHeader))
    {
      std::cerr << "ESMReader::processGroup: Error: nextGroupStarted returned false!\n";
      return -1;
    }
    int result = readGroup(input, gd, localized, table);
    if (result >= 0)
    {
      if (!groupFinished(gd))
      {
        std::cerr << "ESMReader::processGroup: Error: groupFinished returned false!\n";
        return -1;
      }
    }
    return result;
  }
  // group not needed, skip it
  return skipGroup(input, gd);
}

int ESMReader::readGroup(std::istream& input, const GroupData& g_data, const bool localized, const StringTable& table)
{
  // actually read the group
  const auto endPosition = input.tellg() + static_cast<std::ifstream::pos_type>(g_data.size() - 24);
  int recordsRead = 0;
  int lastResult = 0;
  uint32_t recName;
  while ((input.tellg() < endPosition) && (lastResult >= 0))
  {
    // read next header
    recName = 0;
    input.read(reinterpret_cast<char*>(&recName), 4);
    if (!input.good())
    {
      std::cerr << "ESMReader::readGroup: Error while reading next record "
                << "header!\nCurrent position is " << input.tellg() << " bytes.\n";
      return -1;
    }
    // Is it a record or a group?
    if (recName != cGRUP)
    {
      // read next record
      lastResult = readNextRecord(input, recName, localized, table);
    }
    else
    {
      // next group
      lastResult = processGroup(input, false, localized, table);
    }
    if (lastResult > 0)
    {
      recordsRead += lastResult;
    }
  }
  if (lastResult >= 0)
  {
    if (recordsRead > 0)
      return 1;
    return 0;
  }
  std::cerr << "ESMReader::readGroup: Error while reading record of type "
            << IntTo4Char(recName) << "!\nCurrent position is " << input.tellg()
            << " bytes.\n";
  return -1;
}

} // namespace
