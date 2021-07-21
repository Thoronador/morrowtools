/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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

#include "DialogueInfoRecord.hpp"
#include <iostream>
#include <cstring>
#include "../MW_Constants.hpp"
#include "../HelperIO.hpp"

namespace MWTP
{

/* FuncVarRecord's functions */

FuncVarRecord::FuncVarRecord()
: Index(0),
  Type(0),
  Function(0),
  CompareOp(0),
  Name(""),
  isFloat(false),
  fVal(0.0f),
  iVal(0)
{ }

bool FuncVarRecord::operator==(const FuncVarRecord& other) const
{
  if ((Index==other.Index) and (Type==other.Type)
      and (Function==other.Function) and (CompareOp==other.CompareOp)
      and (Name==other.Name) and (isFloat==other.isFloat))
  {
    if (isFloat) return (fVal == other.fVal);
    return (iVal == other.iVal);
  }//if
  return false;
}

/* DialogueInfoRecord's functions */

DialogueInfoRecord::DialogueInfoRecord()
: BasicRecord(),
  recordID(""), PreviousInfoID(""), NextInfoID(""),
  //info data
  UnknownLong(0),
  Disposition(0),
  Rank(0),
  Gender(0),
  PCRank(0),
  UnknownByte(0),
  //end of info data
  ActorID(""), RaceID(""), ClassID(""), FactionID(""), CellID(""),
  PCFactionID(""), SoundFile(""),
  isQuestName(false),
  isQuestFinished(false),
  isQuestRestart(false),
  Response(""),
  Functions(std::vector<FuncVarRecord>()),
  ResultString("")
{ }

bool DialogueInfoRecord::equals(const DialogueInfoRecord& other) const
{
  return ((recordID==other.recordID) and (PreviousInfoID==other.PreviousInfoID)
      and (NextInfoID==other.NextInfoID) and (UnknownLong==other.UnknownLong)
      and (Disposition==other.Disposition) and (Rank==other.Rank)
      and (Gender==other.Gender) and (PCRank==other.PCRank)
      and (UnknownByte==other.UnknownByte) and (ActorID==other.ActorID)
      and (RaceID==other.RaceID) and (ClassID==other.ClassID)
      and (FactionID==other.FactionID) and (CellID==other.CellID)
      and (PCFactionID==other.PCFactionID) and (SoundFile==other.SoundFile)
      and (Response==other.Response) and (Functions==other.Functions)
      and (isQuestName==other.isQuestName) and (isQuestFinished==other.isQuestFinished)
      and (isQuestRestart==other.isQuestRestart) and (ResultString==other.ResultString));
}

#ifndef MW_UNSAVEABLE_RECORDS
bool DialogueInfoRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cINFO, 4);
  uint32_t Size;
  Size = 4 /* INAM */ +4 /* 4 bytes for length */
        +recordID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* PNAM */ +4 /* 4 bytes for length */
        +PreviousInfoID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* NNAM */ +4 /* 4 bytes for length */
        +NextInfoID.length()+1 /* length of ID +1 byte for NUL termination */
        +4 /* DATA */ +4 /* 4 bytes for length */ +12 /* fixed length of 12 bytes */
        +4 /* NAME */ +4 /* 4 bytes for length */
        +Response.length() /* length of response (no NUL termination) */;

  if (!ActorID.empty())
  {
    Size = Size +4 /* ONAM */ +4 /* 4 bytes for length */
          +ActorID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!RaceID.empty())
  {
    Size = Size +4 /* RNAM */ +4 /* 4 bytes for length */
          +RaceID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!ClassID.empty())
  {
    Size = Size +4 /* CNAM */ +4 /* 4 bytes for length */
          +ClassID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!FactionID.empty())
  {
    Size = Size +4 /* FNAM */ +4 /* 4 bytes for length */
          +FactionID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!CellID.empty())
  {
    Size = Size +4 /* ANAM */ +4 /* 4 bytes for length */
          +CellID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!PCFactionID.empty())
  {
    Size = Size +4 /* DNAM */ +4 /* 4 bytes for length */
          +PCFactionID.length()+1 /* length of ID +1 byte for NUL termination */;
  }
  if (!SoundFile.empty())
  {
    Size = Size +4 /* SNAM */ +4 /* 4 bytes for length */
          +SoundFile.length()+1 /* length of file path +1 byte for NUL termination */;
  }
  if (isQuestName)
  {
    Size = Size +4 /* QSTN */ +4 /* 4 bytes for length */ +1 /* fixed length of one byte */;
  }
  if (isQuestFinished)
  {
    Size = Size +4 /* QSTF */ +4 /* 4 bytes for length */ +1 /* fixed length of one byte */;
  }
  if (isQuestRestart)
  {
    Size = Size +4 /* QSTR */ +4 /* 4 bytes for length */ +1 /* fixed length of one byte */;
  }

  //func/var related stuff
  unsigned int i;
  for (i=0; i<Functions.size(); ++i)
  {
    Size = Size + +4 /* QSTR */ +4 /* 4 bytes for length */ +5 /* fixed length of five bytes */
          +Functions[i].Name.length() /* length of var name (no NUL ermination) */
          +4 /* FLTV/INTV */ +4 /* 4 bytes for length */ +4 /* fixed length of four bytes */;
  }//for

  if (!ResultString.empty())
  {
    Size = Size +4 /* BNAM */ +4 /* 4 bytes for length */
          +ResultString.length()+1 /* length of result +1 byte for NUL termination */;
  }
  output.write((const char*) &Size, 4);
  output.write((const char*) &HeaderOne, 4);
  output.write((const char*) &HeaderFlags, 4);

  /* Dialogue info record:
    Dialogue response record that belongs to previous DIAL record.
	INAM = Info name string (unique sequence of #'s), ID
	PNAM = Previous info ID
	NNAM = Next info ID (form a linked list of INFOs for the DIAL). First
		INFO has an empty PNAM, last has an empty NNAM.
	DATA = Info data (12 bytes)
		long Unknown1
		long Disposition
		byte Rank (0-10)
		byte Gender
			0xFF = None
			0x00 = Male
			0x01 = Female
		byte PCRank (0-10)
		byte Unknown2
	ONAM = Actor string
	RNAM = Race string
	CNAM = Class string
	FNAM = Faction string
	ANAM = Cell string
	DNAM = PC Faction string
	NAME = The info response string (512 max)
	SNAM = Sound filename
	QSTN = Journal Name (1 byte, 0x01)
	QSTF = Journal Finished (1 byte, 0x01)
	QSTR = Journal Restart (1 byte, 0x01)
	SCVR = String for the function/variable choice (5+ bytes)
		byte  Index
			'0' to '5'
		byte  Type
			'0' = Nothing?
			'1' = Function
			'2' = Global
			'3' = Local
			'4' = Journal
			'5' = Item
			'6' = Dead
			'7' = Not ID
			'8' = Not Faction
			'9' = Not Class
			'A' = Not Race
			'B' = Not Cell
			'C' = Not Local
		short Function (2-byte string, '00' to '71')
			'sX' = Global/Local/Not Local types
			'JX' = Journal type
			'IX' = Item Type
			'DX' = Dead Type
			'XX' = Not ID Type
			'FX' = Not Faction
			'CX' = Not Class
			'RX' = Not Race
			'LX' = Not Cell
		byte CompareOp
			'0' = '='
			'1' = '!='
			'2' = '>'
			'3' = '>='
			'4' = '<'
			'5' = '<='
		byte Name[]
			Except for the function type, this is the ID for the global/local/etc...
			Is not necessarily NULL terminated. The function type SCVR sub-record has
			no name string.

	INTV =
	FLTV = The function/variable result for the previous SCVR
	BNAM = Result text (not compiled)
  */

  //write INAM
  output.write((const char*) &cINAM, 4);
  uint32_t SubLength = recordID.length()+1;
  //write INAM's length
  output.write((const char*) &SubLength, 4);
  //write ID
  output.write(recordID.c_str(), SubLength);

  //write PNAM
  output.write((const char*) &cPNAM, 4);
  SubLength = PreviousInfoID.length()+1;
  //write PNAM's length
  output.write((const char*) &SubLength, 4);
  //write previous ID
  output.write(PreviousInfoID.c_str(), SubLength);

  //write NNAM
  output.write((const char*) &cNNAM, 4);
  SubLength = NextInfoID.length()+1;
  //write NNAM's length
  output.write((const char*) &SubLength, 4);
  //write next ID
  output.write(NextInfoID.c_str(), SubLength);

  //write DATA
  output.write((const char*) &cDATA, 4);
  SubLength = 12; //fixed length of 12 bytes
  //write DATA's length
  output.write((const char*) &SubLength, 4);
  //write data
  output.write((const char*) &UnknownLong, 4);
  output.write((const char*) &Disposition, 4);
  output.write((const char*) &Rank, 1);
  output.write((const char*) &Gender, 1);
  output.write((const char*) &PCRank, 1);
  output.write((const char*) &UnknownByte, 1);

  if (!ActorID.empty())
  {
    //write ONAM
    output.write((const char*) &cONAM, 4);
    SubLength = ActorID.length()+1;
    //write ONAM's length
    output.write((const char*) &SubLength, 4);
    //write actor ID
    output.write(ActorID.c_str(), SubLength);
  }

  if (!RaceID.empty())
  {
    //write RNAM
    output.write((const char*) &cRNAM, 4);
    SubLength = RaceID.length()+1;
    //write RNAM's length
    output.write((const char*) &SubLength, 4);
    //write race ID
    output.write(RaceID.c_str(), SubLength);
  }

  if (!ClassID.empty())
  {
    //write CNAM
    output.write((const char*) &cCNAM, 4);
    SubLength = ClassID.length()+1;
    //write CNAM's length
    output.write((const char*) &SubLength, 4);
    //write class ID
    output.write(ClassID.c_str(), SubLength);
  }

  if (!FactionID.empty())
  {
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    SubLength = FactionID.length()+1;
    //write FNAM's length
    output.write((const char*) &SubLength, 4);
    //write faction ID
    output.write(FactionID.c_str(), SubLength);
  }

  if (!CellID.empty())
  {
    //write ANAM
    output.write((const char*) &cANAM, 4);
    SubLength = CellID.length()+1;
    //write ANAM's length
    output.write((const char*) &SubLength, 4);
    //write cell name
    output.write(CellID.c_str(), SubLength);
  }

  if (!PCFactionID.empty())
  {
    //write DNAM
    output.write((const char*) &cDNAM, 4);
    SubLength = PCFactionID.length()+1;
    //write DNAM's length
    output.write((const char*) &SubLength, 4);
    //write PC faction ID
    output.write(PCFactionID.c_str(), SubLength);
  }

  //write NAME
  output.write((const char*) &cNAME, 4);
  SubLength = Response.length();
  //write NAME's length
  output.write((const char*) &SubLength, 4);
  //write response
  output.write(Response.c_str(), SubLength);

  if (!SoundFile.empty())
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    SubLength = SoundFile.length()+1;
    //write SNAM's length
    output.write((const char*) &SubLength, 4);
    //write sound file path
    output.write(SoundFile.c_str(), SubLength);
  }

  if (isQuestName)
  {
    //write QSTN
    output.write((const char*) &cQSTN, 4);
    SubLength = 1; //fixed length
    //write QSTN's length
    output.write((const char*) &SubLength, 4);
    //write quest name flag
    output.write((const char*) &SubLength, 1);
  }

  if (isQuestFinished)
  {
    //write QSTF
    output.write((const char*) &cQSTF, 4);
    SubLength = 1; //fixed length
    //write QSTF's length
    output.write((const char*) &SubLength, 4);
    //write quest finished flag
    output.write((const char*) &SubLength, 1);
  }

  if (isQuestRestart)
  {
    //write QSTN
    output.write((const char*) &cQSTR, 4);
    SubLength = 1; //fixed length of one byte
    //write QSTR's length
    output.write((const char*) &SubLength, 4);
    //write quest restart flag
    output.write((const char*) &SubLength, 1);
  }

  for (i=0; i<Functions.size(); ++i)
  {
    //write SCVR
    output.write((const char*) &cSCVR, 4);
    SubLength = 5 + Functions[i].Name.length();
    //write SCVR's length
    output.write((const char*) &SubLength, 4);
    //write function related stuff
    output.write((const char*) &Functions[i].Index, 1);
    output.write((const char*) &Functions[i].Type, 1);
    output.write((const char*) &Functions[i].Function, 2);
    output.write((const char*) &Functions[i].CompareOp, 1);
    // --- write var name, if present
    output.write(Functions[i].Name.c_str(), SubLength-5);
    //write float/ int value
    if (Functions[i].isFloat)
    {
      //write FLTV
      output.write((const char*) &cFLTV, 4);
      SubLength = 4; //fixed size of 4 bytes
      //write FLTV's length
      output.write((const char*) &SubLength, 4);
      //write float itself
      output.write((const char*) &(Functions[i].fVal), 4);
    }
    else
    {
      //write INTV
      output.write((const char*) &cINTV, 4);
      SubLength = 4; //fixed size of 4 bytes
      //write INTV's length
      output.write((const char*) &SubLength, 4);
      //write integer itself
      output.write((const char*) &(Functions[i].iVal), 4);
    }
  }//for

  if (!ResultString.empty())
  {
    //write BNAM
    output.write((const char*) &cBNAM, 4);
    SubLength = ResultString.length()+1;
    //write BNAM's length
    output.write((const char*) &SubLength, 4);
    //write result string/script
    output.write(ResultString.c_str(), SubLength);
  }

  return output.good();
}
#endif

bool DialogueInfoRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);
  /* Dialogue info record:
    Dialogue response record that belongs to previous DIAL record.
	INAM = Info name string (unique sequence of #'s), ID
	PNAM = Previous info ID
	NNAM = Next info ID (form a linked list of INFOs for the DIAL). First
		INFO has an empty PNAM, last has an empty NNAM.
	DATA = Info data (12 bytes)
		long Unknown1
		long Disposition
		byte Rank (0-10)
		byte Gender
			0xFF = None
			0x00 = Male
			0x01 = Female
		byte PCRank (0-10)
		byte Unknown2
	ONAM = Actor string
	RNAM = Race string
	CNAM = Class string
	FNAM = Faction string
	ANAM = Cell string
	DNAM = PC Faction string
	NAME = The info response string (512 max)
	SNAM = Sound filename
	QSTN = Journal Name (1 byte, 0x01)
	QSTF = Journal Finished (1 byte, 0x01)
	QSTR = Journal Restart (1 byte, 0x01)
	SCVR = String for the function/variable choice (5+ bytes)
		byte  Index
			'0' to '5'
		byte  Type
			'0' = Nothing?
			'1' = Function
			'2' = Global
			'3' = Local
			'4' = Journal
			'5' = Item
			'6' = Dead
			'7' = Not ID
			'8' = Not Faction
			'9' = Not Class
			'A' = Not Race
			'B' = Not Cell
			'C' = Not Local
		short Function (2-byte string, '00' to '71')
			'sX' = Global/Local/Not Local types
			'JX' = Journal type
			'IX' = Item Type
			'DX' = Dead Type
			'XX' = Not ID Type
			'FX' = Not Faction
			'CX' = Not Class
			'RX' = Not Race
			'LX' = Not Cell
		byte CompareOp
			'0' = '='
			'1' = '!='
			'2' = '>'
			'3' = '>='
			'4' = '<'
			'5' = '<='
		byte Name[]
			Except for the function type, this is the ID for the global/local/etc...
			Is not necessarily NULL terminated. The function type SCVR sub-record has
			no name string.

	INTV =
	FLTV = The function/variable result for the previous SCVR
	BNAM = Result text (not compiled)
  */
  uint32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read INAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cINAM)
  {
    UnexpectedRecord(cINAM, SubRecName);
    return false;
  }
  //INAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord INAM of INFO is longer than 255 characters.\n";
    return false;
  }
  //read info ID
  char Buffer[768];
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord INAM of DIAL!\n";
    return false;
  }
  recordID = std::string(Buffer);

  //read PNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cPNAM)
  {
    UnexpectedRecord(cPNAM, SubRecName);
    return false;
  }
  //PNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord PNAM of INFO is longer than 255 characters.\n";
    return false;
  }
  //read previous info's ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord PNAM of DIAL!\n";
    return false;
  }
  PreviousInfoID = std::string(Buffer);

  //read NNAM
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cNNAM)
  {
    UnexpectedRecord(cNNAM, SubRecName);
    return false;
  }
  //NNAM's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength>255)
  {
    std::cout << "Error: Subrecord NNAM of INFO is longer than 255 characters.\n";
    return false;
  }
  //read next info's ID
  memset(Buffer, '\0', 256);
  in_File.read(Buffer, SubLength);
  BytesRead += SubLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord NNAM of DIAL!\n";
    return false;
  }
  NextInfoID = std::string(Buffer);

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=12)
  {
    std::cout << "Error: Subrecord DATA of INFO has invalid length ("<<SubLength
              << " bytes). Should be 12 bytes.\n";
    return false;
  }
  //read dialogue response
  in_File.read((char*) &UnknownLong, 4);
  in_File.read((char*) &Disposition, 4);
  in_File.read((char*) &Rank, 1);
  in_File.read((char*) &Gender, 1);
  in_File.read((char*) &PCRank, 1);
  in_File.read((char*) &UnknownByte, 1);
  BytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of DIAL!\n";
    return false;
  }


  ActorID.clear();
  RaceID.clear();
  ClassID.clear();
  FactionID.clear();
  CellID.clear();
  PCFactionID.clear();
  SoundFile.clear();
  Response.clear();
  ResultString.clear();
  Functions.clear();
  isQuestName = false;
  isQuestFinished = false;
  isQuestRestart = false;
  bool hasONAM = false;
  bool hasRNAM = false;
  bool hasCNAM = false;
  bool hasNAME = false;
  //bool hasSCVR = false;
  bool hasBNAM = false;
  bool hasFNAM = false;
  bool hasANAM = false;
  bool hasDNAM = false;
  bool hasSNAM = false;
  FuncVarRecord temp;
  while (BytesRead<Size)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cONAM:
           if (hasONAM)
           {
             std::cout << "Error: record INFO seems to have two ONAM subrecords.\n";
             return false;
           }
           //ONAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ONAM of INFO is longer than 255 characters.\n";
             return false;
           }
           //read actor ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ONAM of INFO!\n";
             return false;
           }
           ActorID = std::string(Buffer);
           hasONAM = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cout << "Error: record INFO seems to have two RNAM subrecords.\n";
             return false;
           }
           //RNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord RNAM of INFO is longer than 255 characters.\n";
             return false;
           }
           //read race ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord RNAM of INFO!\n";
             return false;
           }
           RaceID = std::string(Buffer);
           hasRNAM = true;
           break;
      case cCNAM:
           if (hasCNAM)
           {
             std::cout << "Error: record INFO seems to have two CNAM subrecords.\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord CNAM of INFO is longer than 255 characters.\n";
             return false;
           }
           //read class ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of INFO!\n";
             return false;
           }
           ClassID = std::string(Buffer);
           hasCNAM = true;
           break;
      case cFNAM:
           if (hasFNAM)
           {
             std::cout << "Error: record INFO seems to have two FNAM subrecords.\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord FNAM of INFO is longer than 255 characters.\n";
             return false;
           }
           //read faction ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of INFO!\n";
             return false;
           }
           FactionID = std::string(Buffer);
           hasFNAM = true;
           break;
      case cANAM:
           if (hasANAM)
           {
             std::cout << "Error: record INFO seems to have two ANAM subrecords.\n";
             return false;
           }
           //ANAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord ANAM of INFO is longer than 255 characters.\n";
             return false;
           }
           //read cell name
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ANAM of INFO!\n";
             return false;
           }
           CellID = std::string(Buffer);
           hasANAM = true;
           break;
      case cDNAM:
           if (hasDNAM)
           {
             std::cout << "Error: record INFO seems to have two DNAM subrecords.\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord DNAM of INFO is longer than 255 characters.\n";
             return false;
           }
           //read pc faction ID
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of INFO!\n";
             return false;
           }
           PCFactionID = std::string(Buffer);
           hasDNAM = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: record INFO seems to have two SNAM subrecords.\n";
             return false;
           }
           //SNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>255)
           {
             std::cout << "Error: subrecord SNAM of INFO is longer than 255 characters.\n";
             return false;
           }
           //read sound file path
           memset(Buffer, '\0', 256);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SNAM of INFO!\n";
             return false;
           }
           SoundFile = std::string(Buffer);
           hasSNAM = true;
           break;
      case cNAME:
           if (hasNAME)
           {
             std::cout << "Error: record INFO seems to have two NAME subrecords.\n";
             return false;
           }
           //NAME's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>512)
           {
             std::cout << "Error: Subrecord NAME of INFO is longer than 512 characters.\n";
             return false;
           }
           //read dialogue response
           memset(Buffer, '\0', 513);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAME of DIAL!\n";
             return false;
           }
           Response = std::string(Buffer);
           hasNAME = true;
           break;
      case cSCVR:
           //SCVR's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength<5)
           {
             std::cout << "Error: Subrecord SCVR of INFO is shorter than five characters.\n";
             return false;
           }
           if (SubLength>255+5)
           {
             std::cout << "Error: Subrecord SCVR of INFO is longer than 255 characters.\n";
             return false;
           }
           //read function related stuff
           in_File.read((char*) &temp.Index, 1);
           in_File.read((char*) &temp.Type, 1);
           in_File.read((char*) &temp.Function, 2);
           in_File.read((char*) &temp.CompareOp, 1);
           BytesRead += 5;
           // --- read var name, if present
           memset(Buffer, '\0', 513);
           in_File.read(Buffer, SubLength-5);
           BytesRead += (SubLength-5);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCVR of DIAL!\n";
             return false;
           }
           temp.Name = std::string(Buffer);

           //directly after SCVR comes INTV or FLTV
           //read INTV/FLTV
           in_File.read((char*) &SubRecName, 4);
           BytesRead += 4;
           if ((SubRecName!=cINTV) and (SubRecName!=cFLTV))
           {
             std::cout << "Error while reading INFO: expected record name INTV "
                       << "or FLTV was not found. Instead, \""
                       << IntTo4Char(SubRecName)<<"\" was found.\n";
             return false;
           }
           //INTV's/FLTV's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4)
           {
             std::cout << "Error: Subrecord INTV of INFO has invalid length ("
                       << SubLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           if (SubRecName==cINTV)
           {
             //it's an integer value
             temp.isFloat = false;
             in_File.read((char*) &temp.iVal, 4);
             BytesRead += 4;
             temp.fVal = 0.0f;
           }
           else
           {
             //it's a float value
             temp.isFloat = true;
             in_File.read((char*) &temp.fVal, 4);
             BytesRead += 4;
             temp.iVal = 0;
           }
           Functions.push_back(temp);
           break;
      case cBNAM:
           if (hasBNAM)
           {
             std::cout << "Error: record INFO seems to have two BNAM subrecords.\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength>767)
           {
             std::cout << "Error: subrecord BNAM of INFO is longer than 767 characters.\n";
             return false;
           }
           //read result string
           memset(Buffer, '\0', 768);
           in_File.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of INFO!\n";
             return false;
           }
           ResultString = std::string(Buffer);
           hasBNAM = true;
           break;
      case cQSTN:
           if (isQuestName)
           {
             std::cout << "Error: record INFO seems to have two QSTN subrecords.\n";
             return false;
           }
           //QSTN's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=1)
           {
             std::cout << "Error: subrecord QSTN of INFO has invalid length ("
                       << SubLength<<" bytes). Should be one byte only.\n";
             return false;
           }
           //read QSTN record
           Buffer[0] = '\0';
           in_File.read(Buffer, 1);
           BytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord QSTN of INFO!\n";
             return false;
           }
           if (Buffer[0]!=1)
           {
             std::cout << "Subrecord QSTN of INFO does not have value 01!\n";
             return false;
           }
           isQuestName = true;
           break;
      case cQSTF:
           if (isQuestFinished)
           {
             std::cout << "Error: record INFO seems to have two QSTF subrecords.\n";
             return false;
           }
           //QSTF's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=1)
           {
             std::cout << "Error: subrecord QSTF of INFO has invalid length ("
                       << SubLength<<" bytes). Should be one byte only.\n";
             return false;
           }
           //read QSTF record
           Buffer[0] = '\0';
           in_File.read(Buffer, 1);
           BytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord QSTF of INFO!\n";
             return false;
           }
           if (Buffer[0]!=1)
           {
             std::cout << "Subrecord QSTF of INFO does not have value 01!\n";
             return false;
           }
           isQuestFinished = true;
           break;
      case cQSTR:
           if (isQuestRestart)
           {
             std::cout << "Error: record INFO seems to have two QSTR subrecords.\n";
             return false;
           }
           //QSTR's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=1)
           {
             std::cout << "Error: subrecord QSTR of INFO has invalid length ("
                       << SubLength<<" bytes). Should be one byte only.\n";
             return false;
           }
           //read QSTR record
           Buffer[0] = '\0';
           in_File.read(Buffer, 1);
           BytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord QSTR of INFO!\n";
             return false;
           }
           if (Buffer[0]!=1)
           {
             std::cout << "Subrecord QSTR of INFO does not have value 01!\n";
             return false;
           }
           isQuestRestart = true;
           break;
      default:
           std::cout << "Error while reading INFO: expected record name ONAM, "
                     << "RNAM, CNAM, FNAM, ANAM, DNAM, SNAM, NAME, SCVR, BNAM,"
                     << " QSTN, QSTF or QSTR not found. Instead, \""
                     << IntTo4Char(SubRecName)<<"\" was found.\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
