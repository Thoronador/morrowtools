/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012 Thoronador

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
#include <cstring>
#include "MW_Constants.hpp"
#include "HelperIO.hpp"

namespace MWTP
{

ESMReader::ESMReader()
{
  //empty
}
ESMReader::~ESMReader()
{
  //empty
}

int ESMReader::skipRecord(std::istream& input)
{
  int32_t Size, HeaderOne, Flags;
  Size = 0;
  input.read((char*) &Size, 4);
  if (Size<0)
  {
    std::cout << "Error: record size is negative.\n";
    return -1;
  }
  input.read((char*) &HeaderOne, 4);
  input.read((char*) &Flags, 4);
  /* data does not really matter here */
  input.seekg(Size, std::ios::cur);
  if (input.good())
  {
    return 0;
  }
  return -1;
}

int ESMReader::readESM(const std::string& FileName, TES3Record& theHead, const bool verbose)
{
  std::ifstream input;
  char Buffer[4];

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "Error: could not open file \""<<FileName<<"\".\n";
    return -1;
  }

  //set file pointer to end
  input.seekg(0, std::ios::end);
  const int32_t FileSize = input.tellg();
  //reset file pointer to start of file
  input.seekg(0, std::ios::beg);

  //read the header
  //TES3
  Buffer[0] = Buffer[1] = Buffer[2] = Buffer[3] = '\0';
  input.read(Buffer, 4);
  if (Buffer[0]!='T' || Buffer[1]!='E' || Buffer[2]!='S' || Buffer[3]!='3')
  {
    std::cout << "Error: File \""<<FileName<<"\" is not a valid .esp/.esm file.\n";
    input.close();
    return -1;
  }

  //read TES3 header record
  if (!theHead.loadFromStream(input))
  {
    std::cout << "Error while reading TES3 header from \""<<FileName<<"\".\n";
    input.close();
    return -1;
  }

  if (verbose)
  {
    std::cout << "Info for file \""<<FileName<<"\":"
              << "\n Version: "<<theHead.Version
              << "\n File flag: "<<theHead.FileFlag;
    if ((theHead.FileFlag & 1)!=0)
    {
      std::cout << " (master file)";
    }
    else
    {
      std::cout << " (plugin file)";
    }
    std::cout << "\n Company Name: "<<theHead.companyName
              << "\n Description: "<<theHead.description
              << "\n Number of records: "<< theHead.NumRecords<< "\n\n";

    std::cout << "Master files:\n";
    unsigned int i;
    for (i=0; i<theHead.dependencies.getSize(); ++i)
    {
      std::cout << " Master File: "<<theHead.dependencies.at(i).name<<"\n";
    }//for i
    std::cout <<"End of Master File list.\n\n";
  }//verbose

  if (!input.good())
  {
    std::cout << "Error while reading header of file \""<<FileName<<"\".\n";
    input.close();
    return -1;
  }

  //Now we will read the real data records that follow after the header.
  uint32_t Processed_Records = 0;
  int relevantRecords = 0;
  int lastResult = 0;
  int32_t lastGoodPosition = input.tellg();
  //now read all the records
  while ((input.tellg()<FileSize) and (lastResult!=-1))
  {
    lastResult = processNextRecord(input);
    ++Processed_Records;
    if (lastResult!=-1)
    {
      relevantRecords += lastResult;
      lastGoodPosition = input.tellg();
    }
  }//while

  if (verbose)
  {
    std::cout << "Number of processed and/or skipped records (including "
              << "possible failures): "<<Processed_Records<<"\n"
              << "Current file position: "<<input.tellg()<< " bytes.\n";
  }//if verbose
  //end
  const bool good_result = (input.good() and (lastResult!=-1));
  input.close();
  if (!good_result)
  {
    std::cout << "Error: readESM of file \""<<FileName<<"\" failed. Last known "
              << "good position was "<<lastGoodPosition<<".\n";
    return -1;
  }
  //save data and return
  return relevantRecords;
}//readESM of ESMReader class

bool ESMReader::peekESMHeader(const std::string& FileName, TES3Record& theHead)
{
  std::ifstream input;
  char Buffer[4];

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "ESMReader::peekESMHeader: Error: could not open file \""<<FileName<<"\".\n";
    return false;
  }

  //read the header
  //TES3
  Buffer[0] = Buffer[1] = Buffer[2] = Buffer[3] = '\0';
  input.read(Buffer, 4);
  if (Buffer[0]!='T' || Buffer[1]!='E' || Buffer[2]!='S' || Buffer[3]!='3')
  {
    std::cout << "ESMReader::peekESMHeader: Error: File \""<<FileName<<"\" is not a valid .esp/.esm file.\n";
    input.close();
    return false;
  }

  //read TES3 header record
  if (!theHead.loadFromStream(input))
  {
    std::cout << "ESMReader::peekESMHeader: Error while reading TES3 header from \""<<FileName<<"\".\n";
    input.close();
    return false;
  }

  input.close();
  return input.good();
}

int ESMReader::processNextRecord(std::istream& in_File)
{
  uint32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  //read record name
  in_File.read((char*) &RecordName, 4);
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
         lastResult = ESMReader::skipRecord(in_File);
         break;
    default:
         std::cout << "ESMReader::processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }//swi
  return lastResult;
}//processNextRecord of ESMReader class

} //namespace
