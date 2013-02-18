/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "ESMReader.h"
#include <iostream>
#include "SR_Constants.h"
#include "PathFunctions.h"
#include "../../mw/base/HelperIO.h"

namespace SRTP
{

ESMReader::ESMReader()
{
  //empty
}

ESMReader::~ESMReader()
{
  //empty
}

/* skips a record and returns 0 on success, and -1 on error */
int ESMReader::skipRecord(std::ifstream& in_File)
{
  uint32_t Size = 0;
  in_File.read((char*) &Size, 4);
  in_File.seekg(Size+16, std::ios::cur);
  if (in_File.good())
  {
    return 0;
  }
  return -1;
}

/* skips a group and returns 0 on success, and -1 on error */
int ESMReader::skipGroup(std::ifstream& in_File, const GroupData& g_data)
{
  if (g_data.getGroupSize()<24)
  {
    std::cout << "ESMReader::skipGroup: Error: group size is below 24 bytes!\n";
    return -1;
  }
  //skip it
  in_File.seekg(g_data.getGroupSize()-24, std::ios::cur);
  if (in_File.good())
  {
    return 0;
  }
  return -1;
}

int ESMReader::readESM(const std::string& FileName, Tes4HeaderRecord& head)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "Error: could not open file \""<<FileName<<"\".\n";
    return -1;
  }

  //set file pointer to end
  input.seekg(0, std::ios::end);
  const uint32_t FileSize = input.tellg();
  //reset file pointer to start of file
  input.seekg(0, std::ios::beg);

  //read the header name
  //TES4
  uint32_t recordName = 0;
  input.read((char*) &recordName, 4);
  if (recordName!=cTES4)
  {
    std::cout << "Error: File \""<<FileName<<"\" is not a valid .esp/.esm file."
              << " Expected "<<IntTo4Char(cTES4)<<", but \""<<IntTo4Char(recordName)
              << "\" was found instead.\n";
    input.close();
    return -1;
  }
  //now read the actual header
  if (!head.loadFromStream(input))
  {
    std::cout << "Error: Could not read header from \""<<FileName<<"\".\n";
    input.close();
    return -1;
  }

  const bool localized = head.isLocalized();
  //TODO: read string tables
  StringTable table;
  if (localized)
  {
    std::vector<std::string> files;
    const int rc = getAssociatedTableFiles(FileName, files);
    if (rc!=0)
    {
      input.close();
      return -1;
    }
    unsigned int i;
    for (i=0; i<files.size(); ++i)
    {
      if (!table.readTable(files[i], StringTable::sdUnknown))
      {
        input.close();
        std::cout << "Error while reading string tables for "<<FileName<<"!\n";
        return -1;
      }
    }//for
  }//if localized

  //read the groups/ records
  uint32_t processedGroups = 0;
  int lastResult = 0;
  uint32_t lastGoodPosition = input.tellg();
  while ((input.tellg()<FileSize) and (lastResult>=0))
  {
    //try to read or skip a group - possibly that won't always work
    lastResult = processGroup(input, true, localized, table);
    if (lastResult>=0)
    {
      processedGroups += lastResult;
      lastGoodPosition = input.tellg();
    }
  }//while
  //finished here
  const bool good_result = (input.good() and (lastResult>=0));
  input.close();
  if (!good_result)
  {
    std::cout << "Error: readESM of file \""<<FileName<<"\" failed. Last known "
              << "good position was "<<lastGoodPosition<<".\n";
    return -1;
  }
  //save data and return
  return processedGroups;
}

bool ESMReader::peekESMHeader(const std::string& FileName, Tes4HeaderRecord& head)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "ESMReader::peekESMHeader: Error: could not open file \""<<FileName<<"\".\n";
    return false;
  }

  //read the header name
  //TES4
  uint32_t recordName = 0;
  input.read((char*) &recordName, 4);
  if (recordName!=cTES4)
  {
    std::cout << "ESMReader::peekESMHeader: Error: File \""<<FileName
              <<"\" is not a valid .esp/.esm file. Expected "<<IntTo4Char(cTES4)
              <<", but \""<<IntTo4Char(recordName)<< "\" was found instead.\n";
    input.close();
    return false;
  }
  //now read the actual header
  if (!head.loadFromStream(input))
  {
    std::cout << "ESMReader::peekESMHeader: Error: Could not read header from \""<<FileName<<"\".\n";
    input.close();
    return false;
  }

  input.close();
  return true;
}

int ESMReader::processGroup(std::ifstream& in_File, const bool withHeader, const bool localized, const StringTable& table)
{
  if (withHeader)
  {
    uint32_t recordHeader = 0;
    //read "GRUP"
    in_File.read((char*) &recordHeader, 4);
    if (!in_File.good())
    {
      std::cout << "ESMReader::processGroup: Error: could not read group header!\n";
      return -1;
    }
    if (recordHeader!=cGRUP)
    {
      UnexpectedRecord(cGRUP, recordHeader);
      return -1;
    }
  }//if with header
  //now read the group data header
  GroupData gd;
  if (!gd.loadFromStream(in_File))
  {
    std::cout << "ESMReader::processGroup: Error: could not read group data!\n";
    return -1;
  }
  if (needGroup(gd))
  {
    nextGroupStarted(gd, !withHeader);
    int result = readGroup(in_File, gd, localized, table);
    if (result>=0)
    {
      groupFinished(gd);
    }
    return result;
  }
  //group not needed, skip it
  return skipGroup(in_File, gd);
}

int ESMReader::readGroup(std::ifstream& in_File, const GroupData& g_data, const bool localized, const StringTable& table)
{
  //actually read the group
  const std::ifstream::pos_type endPosition = in_File.tellg()+static_cast<std::ifstream::pos_type>(g_data.getGroupSize()-24);
  int recordsRead = 0;
  int lastResult = 0;
  uint32_t recName;
  while ((in_File.tellg()<endPosition) and (lastResult>=0))
  {
    //read next header
    recName = 0;
    in_File.read((char*) &recName, 4);
    if (!in_File.good())
    {
      std::cout << "ESMReader::readGroup: Error while reading next record "
                << "header!\nCurrent position is "<<in_File.tellg() <<" bytes.\n";
      return -1;
    }
    //is it a record or a group?
    if (recName!=cGRUP)
    {
      //read next record
      lastResult = readNextRecord(in_File, recName, localized, table);
      if (lastResult>0)
      {
        recordsRead += lastResult;
      }
    }
    else
    {
      //next group
      lastResult = processGroup(in_File, false, localized, table);
    }
  }//while
  if (lastResult>=0)
  {
    if (recordsRead>0) return 1;
    return 0;
  }
  std::cout << "ESMReader::readGroup: Error while reading record of type "
            <<IntTo4Char(recName)<<"!\nCurrent position is "<<in_File.tellg()
            <<" bytes.\n";
  return -1;
}

} //namespace
