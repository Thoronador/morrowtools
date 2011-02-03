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

bool ReadESM(const std::string& FileName, const bool verbose)
{
  std::ifstream input;
  char Buffer[4];

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << "Error: could not open file \""<<FileName<<"\".\n";
    return false;
  }

  //set file pointer to end
  input.seekg(0, std::ios::end);
  const int32_t FileSize = input.tellg();
  //reset file pointer to start of file
  input.seekg(0, std::ios::beg);

  //read the header
  //TES3
  input.read(Buffer, 4);
  if (Buffer[0]!='T' || Buffer[1]!='E' || Buffer[2]!='S' || Buffer[3]!='3')
  {
    std::cout << "Error: File \""<<FileName<<"\" is not a valid .esp/.esm file.\n";
    input.close();
    return false;
  }
  int32_t Size, Header_One, Flags, SubSize;
  input.read((char*) &Size, 4);
  input.read((char*) &Header_One, 4);
  input.read((char*) &Flags, 4);
  if (!input.good())
  {
    std::cout << "Error while reading from \""<<FileName<<"\".\n";
    input.close();
    return false;
  }

  //HEDR
  input.read(Buffer, 4);
  if (Buffer[0]!='H' || Buffer[1]!='E' || Buffer[2]!='D' || Buffer[3]!='R')
  {
    std::cout << "Error: File \""<<FileName<<"\" is not a valid .esp/.esm file."
              << "Record HEDR is not present.\n";
    input.close();
    return false;
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
    return false;
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
    return false;
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
        return false;
      }
      //reading size of DATA record
      input.read((char*) &SubSize, 4);
      if (SubSize!=8)
      {
        std::cout << "Error: Record DATA has invalid size ("<<SubSize
                  <<" bytes). Should be 8 bytes.\n";
        input.close();
        return false;
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

  bool Go_on_processing = input.good();
  uint32_t Processed_Records = 0;
  //now read all the records
  while (Go_on_processing)
  {
    Go_on_processing = ProcessNextRecord(input, FileSize);
    ++Processed_Records;
    if (input.tellg()>=FileSize)
    {
      Go_on_processing = false;
    }
  }

  if (verbose)
  {
    std::cout << "Number of processed and/or skipped records (including "
              << "possible failures): "<<Processed_Records<<"\n"
              << "Current file position: "<<input.tellg()<< " bytes.\n";
  }//if verbose
  //end
  const bool Result = input.good();
  input.close();
  //save data and return
  return Result;
}  //ReadESM

bool ProcessNextRecord(std::ifstream& in_File, const int32_t FileSize)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  bool Success = false;

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
         Success = (ESMReader::skipRecord(in_File)==0);
         break;
    case cGMST:
         Success = GameSettings::getSingleton().readGMST(in_File, FileSize);
         break;
    case cINFO:
    case cINGR:
    case cLAND:
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
         Success = (ESMReader::skipRecord(in_File)==0);
         break;
    case cMGEF:
         Success = MagicEffects::getSingleton().readMGEF(in_File);
         break;
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
         Success = (ESMReader::skipRecord(in_File)==0);
         break;
    case cSPEL:
         Success = Spells::getSingleton().readSPEL(in_File);
         break;
    case cSSCR:
    case cSTAT:
    case cWEAP:
         Success = (ESMReader::skipRecord(in_File)==0);
         break;
    default:
         std::cout << "ProcessRecords: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         Success = false;
         break;
  }//swi
  return Success;
}//ProcessNextRecord

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

