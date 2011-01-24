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
         Success = SkipRecord(in_File); break;
    case cGMST:
         Success = ReadGMST(in_File, FileSize); break;
    case cINFO:
    case cINGR:
    case cLAND:
    case cLEVC:
    case cLEVI:
    case cLIGH:
    case cLOCK:
    case cLTEX:
         Success = SkipRecord(in_File); break;
    case cMGEF:
         Success = ReadMGEF(in_File); break;
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
         Success = SkipRecord(in_File); break;
    case cSPEL:
         Success = ReadSPEL(in_File); break;
    case cSSCR:
    case cSTAT:
    case cWEAP:
         Success = SkipRecord(in_File); break;
    default:
         std::cout << "ProcessRecords: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         Success = false;
         break;
  }//swi
  return Success;
}//ProcessNextRecord

bool ReadGMST(std::ifstream& in_File, const int32_t FileSize)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*GMST: Game Setting
    NAME = Setting ID string
    STRV = String value/ INTV = Integer value (4 btes)/FLTV = Float value (4 bytes)
  */
  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>511)
  {
    std::cout << "Sub record NAME of GMST is longer than 511 characters!\n";
    return false;
  }
  if (SubLength<=0)
  {
    std::cout << "Sub record NAME of GMST is shorter than one character!\n";
    return false;
  }
  char Buffer[512];
  memset(Buffer, '\0', 512);
  in_File.read(Buffer, SubLength);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record NAME of GMST!\n";
    return false;
  }
  const std::string SettingName = std::string(Buffer);
  const char NameFirstChar = SettingName.at(0);

  GMSTRecord temp;
  temp.fVal = 0.0f;
  temp.iVal = 0;
  temp.sVal = "";

  //read value record based on first character of setting name
  switch(NameFirstChar)
  {
    case 'f':
         //read FLTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cFLTV)
         {
           UnexpectedRecord(cFLTV, SubRecName);
           return false;
         }
         //FLTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record FLTV has invalid length ("<<SubLength
                     << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //read FLTV
         in_File.read((char*) &(temp.fVal), 4);
         temp.Type = gtFloat;
         break;
    case 'i':
         //read INTV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName!=cINTV)
         {
           UnexpectedRecord(cINTV, SubRecName);
           return false;
         }
         //INTV's length
         in_File.read((char*) &SubLength, 4);
         if (SubLength != 4)
         {
           std::cout << "Error: sub record INTV has invalid length ("<<SubLength
                     << " bytes). Should be 4 bytes.\n";
           return false;
         }
         //read INTV
         in_File.read((char*) &(temp.iVal), 4);
         temp.Type = gtInteger;
         break;
    default: //'s'
         temp.Type = gtString;
         // if string is empty, STRV sub-record may not be present at all
         if (in_File.tellg()>=FileSize)
         {
           GameSettings::getSingleton().addSetting(SettingName, temp);
           return in_File.good();
         }
         //read STRV
         in_File.read((char*) &SubRecName, 4);
         if (SubRecName==cSTRV)
         {
           //STRV's length
           in_File.read((char*) &SubLength, 4);
           if (SubLength>511)
           {
             std::cout << "Sub record STRV of GMST is longer than 511 characters!\n";
             return false;
           }
           //read STRV
           memset(Buffer, '\0', 512);
           in_File.read(Buffer, SubLength);
           temp.sVal = std::string(Buffer);
         }
         else
         { //seek four bytes towards beginning to be before next record name
           in_File.seekg(-4, std::ios::cur);
         }
         break;
  }//switch
  if (in_File.good())
  {
    GameSettings::getSingleton().addSetting(SettingName, temp);
    return true;
  }
  return false;
}//ReadGMST

bool ReadMGEF(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Magic effects:
    INDX = The Effect ID (4 bytes, long)
    MEDT = Effect Data (36 bytes)
        long  SpellSchool (0 = Alteration,1 = Conjuration,2 = Destruction,
                           3 = Illusion,4 = Mysticism,5 = Restoration)
        float BaseCost
        long  Flags (0x0200 = Spellmaking,0x0400 = Enchanting,0x0800 = Negative)
        long  Red
        long  Blue
        long  Green
        float SpeedX
        float SizeX
        float SizeCap
    ITEX = Effect Icon string
    PTEX = Particle texture string
    CVFX = Casting visual string (optional?)
    BVFX = Bolt visual string (optional?)
    HVFX = Hit visual string (optional?)
    AVFX = Area visual string (optional?)
    DESC = Description text (optional?
    CSND = Cast sound (optional)
    BSND = Bolt sound (optional)
    HSND = Hit sound (optional)
    ASND = Area sound (optional)
      Note (Thoronador):
        sequence of the (at least) last four optional sub records can vary.*/

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read INDX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cINDX)
  {
    UnexpectedRecord(cINDX, SubRecName);
    return false;
  }
  //INDX's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=4)
  {
    std::cout << "Error: sub record INDX of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 4 bytes.\n";
    return false;
  }

  int32_t temp_Index = -1;
  in_File.read((char*) &temp_Index, 4); //read the index

  //read MEDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cMEDT)
  {
    UnexpectedRecord(cMEDT, SubRecName);
    return false;
  }
  //MEDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=36)
  {
    std::cout << "Error: sub record MEDT of MGEF has invalid length ("
              <<SubLength<< " bytes). Should be 36 bytes.\n";
    return false;
  }

  MGEF_Data tempData;

  //in_File.seekg(36, std::ios::cur); //skip the data
  //SpellSchool
  in_File.read((char*) &(tempData.SpellSchool), 4);
  //Cost
  in_File.read((char*) &(tempData.BaseCost), 4);
  //Flags
  in_File.read((char*) &(tempData.Flags), 4);
  //Colour
  in_File.read((char*) &(tempData.Red), 4);
  in_File.read((char*) &(tempData.Blue), 4);
  in_File.read((char*) &(tempData.Green), 4);
  //speed
  in_File.read((char*) &(tempData.SpeedX), 4);
  //size
  in_File.read((char*) &(tempData.SizeX), 4);
  //size cap
  in_File.read((char*) &(tempData.SizeCap), 4);


  //read ITEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cITEX)
  {
    UnexpectedRecord(cITEX, SubRecName);
    return false;
  }
  //ITEX's length
  in_File.read((char*) &SubLength, 4);
  char Buffer [1024];
  memset(Buffer, '\0', 1024);
  //read tex name
  in_File.read(Buffer, SubLength);
  tempData.EffectIcon = std::string(Buffer);

  //read PTEX
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cPTEX)
  {
    UnexpectedRecord(cPTEX, SubRecName);
    return false;
  }
  //PTEX's length
  in_File.read((char*) &SubLength, 4);
  //read particle tex name
  memset(Buffer, '\0', 1024);
  in_File.read(Buffer, SubLength);
  tempData.ParticleTexture = std::string(Buffer);

  //read next subrecord
  in_File.read((char*) &SubRecName, 4);

  //visual strings, partially optional
  tempData.CastingVisual = "";
  tempData.BoltVisual = "";
  tempData.HitVisual = "";
  tempData.AreaVisual = "";
  //description, optional
  tempData.Description = "";
  //effect sounds, all optional
  tempData.CastSound = "";
  tempData.BoltSound = "";
  tempData.HitSound = "";
  tempData.AreaSound = "";
  bool go_on_with_subs = true;
  do
  {
    switch(SubRecName)
    {
      case cCVFX: //read optional CVFX
           //CVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastingVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBVFX: //read optional BVFX
           //BVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHVFX: //read optional HVFX
           //HVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cAVFX: //read optional AVFX
           //AVFX's length
           in_File.read((char*) &SubLength, 4);
           //read effect string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaVisual = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cDESC: //read optional DESC
           //DESC's length
           in_File.read((char*) &SubLength, 4);
           //read effect description
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.Description = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cCSND:
           //CSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.CastSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cBSND:
           //BSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.BoltSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cHSND:
           //HSND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.HitSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      case cASND:
           //ASND's length
           in_File.read((char*) &SubLength, 4);
           //read sound string
           memset(Buffer, '\0', 1024);
           in_File.read(Buffer, SubLength);
           tempData.AreaSound = std::string(Buffer);
           //read next optional header
           in_File.read((char*) &SubRecName, 4);
           break;
      default:
           //unknown sub record, thus stop here, it's beginning of new record
           go_on_with_subs = false;
    }//switch
  } while (go_on_with_subs);

  //seek four bytes into the direction if the beginning to land before the
  // name of the next record
  in_File.seekg(-4, std::ios::cur);
  if (in_File.good())
  {
    MagicEffects::getSingleton().addEffect(temp_Index, tempData);
    return true;
  }
  return false;
}//ReadMGEF

bool ReadSPEL(std::ifstream& in_File)
{
  int32_t Size, HeaderOne, Flags;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &Flags, 4);

  /*Spell:
    NAME = Spell ID
    FNAM = Spell Name
    SPDT = Spell Data (12 bytes)
        long Type (0 = Spell,1 = Ability,2 = Blight,3 = Disease,4 = Curse,5 = Power)
        long SpellCost
        long Flags (0x0001 = AutoCalc,0x0002 = PC Start,0x0004 = Always Succeeds)
    ENAM = Enchantment data (24 bytes, 0 to 8) */

  int32_t SubRecName, SubLength;
  SubRecName = SubLength = 0;

  //read NAME
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cNAME)
  {
    UnexpectedRecord(cNAME, SubRecName);
    return false;
  }
  //NAME's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "ReadSPEL: Error: ID is longer than 255 characters.\n";
    std::cout << "File pos.: "<<in_File.tellg()<<"\n";
    return false;
  }
  char Buffer[256];
  Buffer[0] = Buffer[255] = '\0';
  //read ID
  in_File.read(Buffer, SubLength);
  const std::string SpellID = std::string(Buffer);

  //read FNAM
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cFNAM)
  {
    UnexpectedRecord(cFNAM, SubRecName);
    return false;
  }
  //FNAM's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength>255)
  {
    std::cout << "ReadSPEL: Error: name is longer than 255 characters.\n";
    std::cout << "File pos.: "<<in_File.tellg()<<"\n";
    return false;
  }
  //read spell name
  Buffer[0] = Buffer[255] = '\0';
  in_File.read(Buffer, SubLength);
  SpellRecord temp;
  temp.Name = std::string(Buffer);

  //read SPDT
  in_File.read((char*) &SubRecName, 4);
  if (SubRecName!=cSPDT)
  {
    UnexpectedRecord(cSPDT, SubRecName);
    return false;
  }
  //SPDT's length
  in_File.read((char*) &SubLength, 4);
  if (SubLength!=12)
  {
    std::cout << "Error: sub record SPDT of SPEL has invalid length ("
              <<SubLength<< " bytes). Should be 12 bytes.\n";
    return false;
  }
  // ---- read spell data
  //type
  in_File.read((char*) &(temp.Type), 4);
  //cost
  in_File.read((char*) &(temp.Cost), 4);
  //flags
  in_File.read((char*) &(temp.Flags), 4);
  if (!in_File.good())
  {
    std::cout << "Error while reading sub record SPDT of SPEL.\n";
    return false;
  }

  temp.Enchs.clear();
  //read multiple ENAM records
  do
  {
    in_File.read((char*) &SubRecName, 4);
    if (SubRecName==cENAM)
    {
      //ENAM's length
      in_File.read((char*) &SubLength, 4);
      if (SubLength!=24)
      {
        std::cout << "Error: sub record ENAM of SPEL has invalid length ("
                  <<SubLength<< " bytes). Should be 24 bytes.\n";
        return false;
      }
      //read enchantment data
      EnchantmentData ench;
      in_File.read((char*) &(ench.EffectID), 2);
      in_File.read((char*) &(ench.SkillID), 1);
      in_File.read((char*) &(ench.AttributeID), 1);
      in_File.read((char*) &(ench.RangeType), 4);
      in_File.read((char*) &(ench.Area), 4);
      in_File.read((char*) &(ench.Duration), 4);
      in_File.read((char*) &(ench.MagnitudeMin), 4);
      in_File.read((char*) &(ench.MagnitudeMax), 4);
      if (in_File.good())
      {
        temp.Enchs.push_back(ench);
      }
      else
      {
        std::cout << "Error while reading sub record ENAM of SPEL.\n";
        return false;
      }
    }
  } while (SubRecName == cENAM);
  //seek four bytes towards beginning to land before the name of the next record
  in_File.seekg(-4, std::ios::cur);
  Spells::getSingleton().addSpell(SpellID, temp);
  return in_File.good();
}//ReadSPEL
