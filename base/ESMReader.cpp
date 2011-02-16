/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#include "ESMReader.h"
#include <iostream>
#include "MagicEffects.h"
#include "Spells.h"
#include "GameSettings.h"
#include "MW_Constants.h"
#include "HelperIO.h"

ESMReader::ESMReader()
{
  //empty
}
ESMReader::~ESMReader()
{
  //empty
}

int ESMReader::skipRecord(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  Size = 0;
  in_File.read((char*) &Size, 4);
  if (Size<0)
  {
    std::cout << "Error: record size is negative.\n";
    return -1;
  }
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);
  /* data does not really matter here */
  in_File.seekg(Size, std::ios::cur);
  if (in_File.good())
  {
    return 0;
  }
  return -1;
}

int ESMReader::readESM(const std::string& FileName, const bool verbose)
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
  int32_t Size, Header_One, Flags, SubSize;
  input.read((char*) &Size, 4);
  input.read((char*) &Header_One, 4);
  input.read((char*) &Flags, 4);
  if (!input.good())
  {
    std::cout << "Error while reading from \""<<FileName<<"\".\n";
    input.close();
    return -1;
  }

  //HEDR
  input.read(Buffer, 4);
  if (Buffer[0]!='H' || Buffer[1]!='E' || Buffer[2]!='D' || Buffer[3]!='R')
  {
    std::cout << "Error: File \""<<FileName<<"\" is not a valid .esp/.esm file."
              << "Record HEDR is not present.\n";
    input.close();
    return -1;
  }

  /*HEDR (300bytes)
    4 bytes, float Version (1.2)
    4 bytes, long FileFlag(?) (seems to be 1 for master and 0 for plugin file)
    32 Bytes, Company/ Author Name string
    256 Bytes, ESM file description
    4 bytes, long NumRecords (48227)
  */
  SubSize = 0;
  input.read((char*) &SubSize, 4);//size of HEDR record
  if (SubSize!=300)
  {
    std::cout << "Error: record HEDR of file \""<<FileName<<"\" has wrong size."
              << " Actual size: "<<SubSize<<" bytes. Must-have size: 300 bytes.\n";
    input.close();
    return -1;
  }

  float Version = 0.0f;
  int32_t FileFlag, NumRecords;
  input.read((char*) &Version, 4);//version of file
  input.read((char*) &FileFlag, 4);//unknown value (file flag?)

  char CName[33];
  char Description[257];
  memset(CName, '\0', 33);
  memset(Description, '\0', 257);
  input.read(CName, 32);//company name
  input.read(Description, 256);//file description
  input.read((char*) &NumRecords, 4);//total number of records

  if (!input.good())
  {
    std::cout << "Error while reading HEDR data from file \""<<FileName<<"\".\n";
    input.close();
    return -1;
  }

  if (verbose)
  {
    std::cout << "Info for file \""<<FileName<<"\":"
              << "\n Version: "<<Version
              << "\n File flag: "<<FileFlag;
    if ((FileFlag & 1)!=0)
    {
      std::cout << " (master file)";
    }
    else
    {
      std::cout << " (plugin file)";
    }
    std::cout << "\n Company Name: "<<CName
              << "\n Description: "<<Description
              << "\n Number of records: "<< NumRecords<< "\n\n";

    std::cout << "Master files:\n";
  }//verbose

  do
  {
    memset(Buffer, '\0', 4);
    input.read(Buffer, 4);
    if (Buffer[0]=='M' && Buffer[1]=='A' && Buffer[2]=='S' && Buffer[3]=='T')
    {
      //read the data
      SubSize = 0;
      input.read((char*) &SubSize, 4);
      if (SubSize<=256)
      {
        memset(Description, '\0', 256);
        input.read(Description, SubSize);
        if (verbose)
        {
          std::cout << " Master File: "<<Description<<"\n";
        }//if verbose
      }
      else
      {
        //skip the data
        input.seekg(SubSize, std::ios::cur);
        std::cout << " Skipping one master file due to long ("<<SubSize
                  << " bytes) file name.\n";
      }
      //reading DATA record
      memset(Buffer, '\0', 4);
      input.read(Buffer, 4);
      if (Buffer[0]!='D' || Buffer[1]!='A' || Buffer[2]!='T' || Buffer[3]!='A')
      {
        std::cout << "Error while reading file \""<<FileName<<"\": DATA "
                  << "record expected, but got: \""<<Buffer[0]<<Buffer[1]
                  <<Buffer[2]<<Buffer[3]<<"\".\n";
        input.close();
        return -1;
      }
      //reading size of DATA record
      input.read((char*) &SubSize, 4);
      if (SubSize!=8)
      {
        std::cout << "Error: Record DATA has invalid size ("<<SubSize
                  <<" bytes). Should be 8 bytes.\n";
        input.close();
        return -1;
      }
      //skip 8 bytes of DATA
      input.seekg(8, std::ios::cur);
    }//read master file info
    else
    {
      if (verbose)
      {
        std::cout <<"End of Master File list.\n\n First record is \""<<Buffer[0]
                  <<Buffer[1]<<Buffer[2]<<Buffer[3]<<"\".\n";
      }//if verbose
    }//else

  } while //(RecordType == Master / Data);
    (Buffer[0]=='D' && Buffer[1]=='A' && Buffer[2]=='T' && Buffer[3]=='A');

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  input.seekg(-4, std::ios::cur);

  if (!input.good())
  {
    std::cout << "Error while reading file \""<<FileName<<"\".\n";
    input.close();
    return -1;
  }
  //bool Go_on_processing = true;
  uint32_t Processed_Records = 0;
  int relevantRecords = 0;
  int lastResult = 0;
  int32_t lastGoodPosition = input.tellg();
  //now read all the records
  while ((input.tellg()<FileSize) and (lastResult!=-1))
  {
    lastResult = processNextRecord(input, FileSize);
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

int ESMReader::processNextRecord(std::ifstream& in_File, const int32_t FileSize)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
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
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }//swi
  return lastResult;
}//processNextRecord of ESMReader class
