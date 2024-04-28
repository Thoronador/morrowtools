/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2021, 2024  Dirk Stolle

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
#include "MW_Constants.hpp"
#include "HelperIO.hpp"

namespace MWTP
{

int ESMReader::skipRecord(std::istream& input)
{
  uint32_t Size = 0;
  uint32_t HeaderOne = 0, Flags = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&Flags), 4);
  /* data does not really matter here */
  input.seekg(Size, std::ios::cur);
  if (input.good())
  {
    return 0;
  }
  return -1;
}

int ESMReader::readESM(const std::string& FileName, TES3Record& theHead)
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
  const auto FileSize = input.tellg();
  // reset file pointer to start of file
  input.seekg(0, std::ios::beg);

  // read the header
  // TES3
  uint32_t Buffer = 0;
  input.read(reinterpret_cast<char*>(&Buffer), 4);
  if (Buffer != cTES3)
  {
    std::cerr << "Error: File \"" << FileName << "\" is not a valid .esp/.esm file.\n";
    input.close();
    return -1;
  }

  // read TES3 header record
  if (!theHead.loadFromStream(input))
  {
    std::cerr << "Error while reading TES3 header from \"" << FileName
              << "\".\n";
    input.close();
    return -1;
  }

  if (!input.good())
  {
    std::cerr << "Error while reading header of file \"" << FileName << "\".\n";
    input.close();
    return -1;
  }

  // Now we will read the real data records that follow after the header.
  int relevantRecords = 0;
  int lastResult = 0;
  auto lastGoodPosition = input.tellg();
  // now read all the records
  while ((input.tellg() < FileSize) && (lastResult != -1))
  {
    lastResult = processNextRecord(input);
    if (lastResult != -1)
    {
      relevantRecords += lastResult;
      lastGoodPosition = input.tellg();
    }
  }

  const bool good_result = input.good() && (lastResult != -1);
  input.close();
  if (!good_result)
  {
    std::cerr << "Error: readESM of file \"" << FileName << "\" failed. Last "
              << "known good position was " << lastGoodPosition << ".\n";
    return -1;
  }
  // save data and return
  return relevantRecords;
}

bool ESMReader::peekESMHeader(const std::string& FileName, TES3Record& theHead)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cerr << "ESMReader::peekESMHeader: Error: Could not open file \""
              << FileName << "\".\n";
    return false;
  }

  // read the header
  // TES3
  uint32_t Buffer = 0;
  input.read(reinterpret_cast<char*>(&Buffer), 4);
  if (Buffer != cTES3)
  {
    std::cerr << "ESMReader::peekESMHeader: Error: File \"" << FileName
              << "\" is not a valid .esp/.esm file.\n";
    input.close();
    return false;
  }

  // read TES3 header record
  if (!theHead.loadFromStream(input))
  {
    std::cerr << "ESMReader::peekESMHeader: Error while reading TES3 header from \""
              << FileName << "\".\n";
    input.close();
    return false;
  }

  input.close();
  return input.good();
}

int ESMReader::processNextRecord(std::istream& input)
{
  // Normally should be 4 chars, but char array is not eligible for switch.
  uint32_t RecordName = 0;
  // read record name
  input.read(reinterpret_cast<char*>(&RecordName), 4);
  switch(RecordName)
  {
    case cACTI:
    case cALCH:
    case cAPPA:
    case cARMO:
    case cBODY:
    case cBOOK:
    case cBSGN:
    case cCELL:
    case cCLAS:
    case cCLOT:
    case cCONT:
    case cCREA:
    case cDIAL:
    case cDOOR:
    case cENCH:
    case cFACT:
    case cGLOB:
    case cGMST:
    case cINFO:
    case cINGR:
    case cLAND:
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
    case cMGEF:
    case cMISC:
    case cNPC_:
    case cPGRD:
    case cPROB:
    case cRACE:
    case cREGN:
    case cREPA:
    case cSCPT:
    case cSKIL:
    case cSNDG:
    case cSOUN:
    case cSPEL:
    case cSSCR:
    case cSTAT:
    case cWEAP:
         return ESMReader::skipRecord(input);
    default:
         std::cerr << "ESMReader::processNextRecord: ERROR: Unknown record type found: \""
                   << IntTo4Char(RecordName) << "\".\n"
                   << "Current file position: " << input.tellg() << " bytes.\n";
         return -1;
  }
}

} // namespace
