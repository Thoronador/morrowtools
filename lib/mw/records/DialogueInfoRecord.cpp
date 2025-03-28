/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2022, 2025  Dirk Stolle

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
  return (Index == other.Index) && (Type == other.Type)
      && (Function == other.Function) && (CompareOp == other.CompareOp)
      && (Name == other.Name) && (isFloat == other.isFloat)
      && ((isFloat && (fVal == other.fVal)) || (!isFloat && (iVal == other.iVal)));
}

/* DialogueInfoRecord's functions */

DialogueInfoRecord::DialogueInfoRecord()
: BasicRecord(),
  recordID(""), PreviousInfoID(""), NextInfoID(""),
  // info data
  UnknownLong(0),
  Disposition(0),
  Rank(0),
  Gender(0),
  PCRank(0),
  UnknownByte(0),
  // end of info data
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
  return (recordID == other.recordID) && (PreviousInfoID == other.PreviousInfoID)
      && (NextInfoID == other.NextInfoID) && (UnknownLong == other.UnknownLong)
      && (Disposition == other.Disposition) && (Rank == other.Rank)
      && (Gender == other.Gender) && (PCRank == other.PCRank)
      && (UnknownByte == other.UnknownByte) && (ActorID == other.ActorID)
      && (RaceID == other.RaceID) && (ClassID == other.ClassID)
      && (FactionID == other.FactionID) && (CellID == other.CellID)
      && (PCFactionID == other.PCFactionID) && (SoundFile == other.SoundFile)
      && (Response == other.Response) && (Functions == other.Functions)
      && (isQuestName == other.isQuestName) && (isQuestFinished == other.isQuestFinished)
      && (isQuestRestart == other.isQuestRestart) && (ResultString == other.ResultString);
}

#ifndef MW_UNSAVEABLE_RECORDS
uint32_t DialogueInfoRecord::getWriteSize() const
{
  uint32_t Size = 4 /* INAM */ + 4 /* 4 bytes for length */
      + recordID.length() + 1 /* length of ID +1 byte for NUL termination */
      + 4 /* PNAM */ + 4 /* 4 bytes for length */
      + PreviousInfoID.length() + 1 /* length of ID +1 byte for NUL */
      + 4 /* NNAM */ + 4 /* 4 bytes for length */
      + NextInfoID.length() + 1 /* length of ID +1 byte for NUL termination */
      + 4 /* DATA */ + 4 /* 4 bytes for length */ + 12 /* fixed: 12 bytes */;

  if (!ActorID.empty())
  {
    Size += 4 /* ONAM */ + 4 /* 4 bytes for length */
          + ActorID.length() + 1 /* length of ID +1 byte for NUL termination */;
  }
  if (!RaceID.empty())
  {
    Size += 4 /* RNAM */ + 4 /* 4 bytes for length */
          + RaceID.length() + 1 /* length of ID +1 byte for NUL termination */;
  }
  if (!ClassID.empty())
  {
    Size += 4 /* CNAM */ + 4 /* 4 bytes for length */
          + ClassID.length() + 1 /* length of ID +1 byte for NUL termination */;
  }
  if (!FactionID.empty())
  {
    Size += 4 /* FNAM */ + 4 /* 4 bytes for length */
          + FactionID.length() + 1 /* length of ID +1 byte for NUL termination */;
  }
  if (!CellID.empty())
  {
    Size += 4 /* ANAM */ + 4 /* 4 bytes for length */
          + CellID.length() + 1 /* length of ID +1 byte for NUL termination */;
  }
  if (!PCFactionID.empty())
  {
    Size += 4 /* DNAM */ + 4 /* 4 bytes for length */
          + PCFactionID.length() + 1 /* length of ID +1 byte for NUL */;
  }
  if (!SoundFile.empty())
  {
    Size += 4 /* SNAM */ + 4 /* 4 bytes for length */
          + SoundFile.length() + 1 /* length of file path +1 byte for NUL */;
  }
  if (!Response.empty())
  {
    Size += 4 /* NAME */ + 4 /* 4 bytes for length */
          + Response.length() /* length of response (no NUL termination) */;
  }
  if (isQuestName)
  {
    Size += 4 /* QSTN */ + 4 /* 4 bytes for length */ + 1 /* fixed: one byte */;
  }
  if (isQuestFinished)
  {
    Size += 4 /* QSTF */ + 4 /* 4 bytes for length */ + 1 /* fixed: one byte */;
  }
  if (isQuestRestart)
  {
    Size += 4 /* QSTR */ + 4 /* 4 bytes for length */ + 1 /* fixed: one byte */;
  }

  // function/variable-related stuff
  for (const auto& fun: Functions)
  {
    Size += 4 /* QSTR */ + 4 /* 4 bytes for length */ + 5 /* fixed: five bytes */
          + fun.Name.length() /* length of var name (no NUL termination) */
          + 4 /* FLTV/INTV */ +4 /* 4 bytes for length */ + 4 /* fixed: four bytes */;
  }

  if (!ResultString.empty())
  {
    Size += 4 /* BNAM */ + 4 /* 4 bytes for length */
          + ResultString.length() /* length of result (no NUL termination) */;
  }
  return Size;
}
bool DialogueInfoRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cINFO), 4);
  const uint32_t Size = getWriteSize();
  output.write(reinterpret_cast<const char*>(&Size), 4);
  output.write(reinterpret_cast<const char*>(&HeaderOne), 4);
  output.write(reinterpret_cast<const char*>(&HeaderFlags), 4);

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

  // write ID (INAM)
  output.write(reinterpret_cast<const char*>(&cINAM), 4);
  uint32_t SubLength = recordID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(recordID.c_str(), SubLength);

  // write previous ID (PNAM)
  output.write(reinterpret_cast<const char*>(&cPNAM), 4);
  SubLength = PreviousInfoID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(PreviousInfoID.c_str(), SubLength);

  // write next ID (NNAM)
  output.write(reinterpret_cast<const char*>(&cNNAM), 4);
  SubLength = NextInfoID.length() + 1;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(NextInfoID.c_str(), SubLength);

  // write data (DATA)
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  SubLength = 12;
  output.write(reinterpret_cast<const char*>(&SubLength), 4);
  output.write(reinterpret_cast<const char*>(&UnknownLong), 4);
  output.write(reinterpret_cast<const char*>(&Disposition), 4);
  output.write(reinterpret_cast<const char*>(&Rank), 1);
  output.write(reinterpret_cast<const char*>(&Gender), 1);
  output.write(reinterpret_cast<const char*>(&PCRank), 1);
  output.write(reinterpret_cast<const char*>(&UnknownByte), 1);

  if (!ActorID.empty())
  {
    // write actor ID (ONAM)
    output.write(reinterpret_cast<const char*>(&cONAM), 4);
    SubLength = ActorID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ActorID.c_str(), SubLength);
  }

  if (!RaceID.empty())
  {
    // write race ID (RNAM)
    output.write(reinterpret_cast<const char*>(&cRNAM), 4);
    SubLength = RaceID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(RaceID.c_str(), SubLength);
  }

  if (!ClassID.empty())
  {
    // write class ID (CNAM)
    output.write(reinterpret_cast<const char*>(&cCNAM), 4);
    SubLength = ClassID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ClassID.c_str(), SubLength);
  }

  if (!FactionID.empty())
  {
    // write faction ID (FNAM)
    output.write(reinterpret_cast<const char*>(&cFNAM), 4);
    SubLength = FactionID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(FactionID.c_str(), SubLength);
  }

  if (!CellID.empty())
  {
    // write cell name (ANAM)
    output.write(reinterpret_cast<const char*>(&cANAM), 4);
    SubLength = CellID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(CellID.c_str(), SubLength);
  }

  if (!PCFactionID.empty())
  {
    // write PC faction ID (DNAM)
    output.write(reinterpret_cast<const char*>(&cDNAM), 4);
    SubLength = PCFactionID.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(PCFactionID.c_str(), SubLength);
  }

  if (!SoundFile.empty())
  {
    // write sound file path (SNAM)
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    SubLength = SoundFile.length() + 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(SoundFile.c_str(), SubLength);
  }

  if (!Response.empty())
  {
    // write response (NAME)
    output.write(reinterpret_cast<const char*>(&cNAME), 4);
    SubLength = Response.length();
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(Response.c_str(), SubLength);
  }

  if (isQuestName)
  {
    // write quest name flag (QSTN)
    output.write(reinterpret_cast<const char*>(&cQSTN), 4);
    SubLength = 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&SubLength), 1);
  }

  if (isQuestFinished)
  {
    // write quest finished flag (QSTF)
    output.write(reinterpret_cast<const char*>(&cQSTF), 4);
    SubLength = 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&SubLength), 1);
  }

  if (isQuestRestart)
  {
    // write quest restart flag (QSTR)
    output.write(reinterpret_cast<const char*>(&cQSTR), 4);
    SubLength = 1;
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(reinterpret_cast<const char*>(&SubLength), 1);
  }

  for (const auto& fun: Functions)
  {
    // write function-related stuff (SCVR)
    output.write(reinterpret_cast<const char*>(&cSCVR), 4);
    SubLength = 5 + fun.Name.length();
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    //write function related stuff
    output.write(reinterpret_cast<const char*>(&fun.Index), 1);
    output.write(reinterpret_cast<const char*>(&fun.Type), 1);
    output.write(reinterpret_cast<const char*>(&fun.Function), 2);
    output.write(reinterpret_cast<const char*>(&fun.CompareOp), 1);
    // --- write var name, if present
    output.write(fun.Name.c_str(), SubLength - 5);
    // write float / int value
    if (fun.isFloat)
    {
      // write float value (FLTV)
      output.write(reinterpret_cast<const char*>(&cFLTV), 4);
      SubLength = 4;
      output.write(reinterpret_cast<const char*>(&SubLength), 4);
      output.write(reinterpret_cast<const char*>(&fun.fVal), 4);
    }
    else
    {
      // write integer value (INTV)
      output.write(reinterpret_cast<const char*>(&cINTV), 4);
      SubLength = 4;
      output.write(reinterpret_cast<const char*>(&SubLength), 4);
      output.write(reinterpret_cast<const char*>(&fun.iVal), 4);
    }
  }

  if (!ResultString.empty())
  {
    // write result string/script (BNAM)
    output.write(reinterpret_cast<const char*>(&cBNAM), 4);
    SubLength = ResultString.length();
    output.write(reinterpret_cast<const char*>(&SubLength), 4);
    output.write(ResultString.c_str(), SubLength);
  }

  return output.good();
}
#endif

/** \brief Tries to load a byte flag from the stream, without header.
     *
     * \param input       the input stream
     * \param target      the boolean that will be used to store the flag
     * \param buffer      a pre-allocated array of char that can hold at least one byte
     * \param subHeader   the header of that sub record
     * \param bytesRead   the variable that holds the number of bytes read so far
     * \return Returns true on success (data was loaded successfully).
     *         Returns false, if an error occurred.
     */
bool loadByteFlag(std::istream& input, bool& target, char * buffer, const uint32_t subHeader, uint32_t& bytesRead)
{
  if (target)
  {
    std::cerr << "Error: Record INFO seems to have two " << IntTo4Char(subHeader) << " sub records.\n";
    return false;
  }
  // sub record's length
  uint32_t subLength = 0;
  input.read(reinterpret_cast<char*>(&subLength), 4);
  bytesRead += 4;
  if (subLength != 1)
  {
    std::cerr << "Error: Sub record " << IntTo4Char(subHeader)
              << " of INFO has invalid length (" << subLength
              << " bytes). Should be one byte only.\n";
    return false;
  }
  // read flag record
  buffer[0] = '\0';
  input.read(buffer, 1);
  bytesRead += 1;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record " << IntTo4Char(subHeader)
              << " of INFO!\n";
    return false;
  }
  if (buffer[0] != 1)
  {
    std::cerr << "Sub record " << IntTo4Char(subHeader)
              << " of INFO does not have value 01!\n";
    return false;
  }
  target = true;
  return true;
}

bool DialogueInfoRecord::loadFromStream(std::istream& input)
{
  uint32_t Size = 0;
  input.read(reinterpret_cast<char*>(&Size), 4);
  input.read(reinterpret_cast<char*>(&HeaderOne), 4);
  input.read(reinterpret_cast<char*>(&HeaderFlags), 4);
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

  uint32_t BytesRead = 0;

  // read info ID (INAM)
  char Buffer[768];
  if (!loadString256WithHeader(input, recordID, Buffer, cINAM, BytesRead))
  {
    std::cerr << "Error while reading sub record INAM of INFO!\n";
    return false;
  }

  // read previous info's ID (PNAM)
  if (!loadString256WithHeader(input, PreviousInfoID, Buffer, cPNAM, BytesRead))
  {
    std::cerr << "Error while reading sub record PNAM of INFO!\n";
    return false;
  }

  // read next info's ID (NNAM)
  if (!loadString256WithHeader(input, NextInfoID, Buffer, cNNAM, BytesRead))
  {
    std::cerr << "Error while reading sub record NNAM of INFO!\n";
    return false;
  }

  uint32_t SubRecName = 0;
  uint32_t SubLength = 0;
  // read DATA
  input.read(reinterpret_cast<char*>(&SubRecName), 4);
  BytesRead += 4;
  if (SubRecName != cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  // DATA's length
  input.read(reinterpret_cast<char*>(&SubLength), 4);
  BytesRead += 4;
  if (SubLength != 12)
  {
    std::cerr << "Error: Sub record DATA of INFO has invalid length ("
              << SubLength << " bytes). Should be 12 bytes.\n";
    return false;
  }
  // read data
  input.read(reinterpret_cast<char*>(&UnknownLong), 4);
  input.read(reinterpret_cast<char*>(&Disposition), 4);
  input.read(reinterpret_cast<char*>(&Rank), 1);
  input.read(reinterpret_cast<char*>(&Gender), 1);
  input.read(reinterpret_cast<char*>(&PCRank), 1);
  input.read(reinterpret_cast<char*>(&UnknownByte), 1);
  BytesRead += 12;
  if (!input.good())
  {
    std::cerr << "Error while reading sub record DATA of INFO!\n";
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
  bool hasBNAM = false;
  bool hasFNAM = false;
  bool hasANAM = false;
  bool hasDNAM = false;
  bool hasSNAM = false;
  FuncVarRecord temp;
  while (BytesRead < Size)
  {
    // read next sub record
    input.read(reinterpret_cast<char*>(&SubRecName), 4);
    BytesRead += 4;
    switch (SubRecName)
    {
      case cONAM:
           if (hasONAM)
           {
             std::cerr << "Error: Record INFO seems to have two ONAM sub records.\n";
             return false;
           }
           // read actor ID (ONAM)
           if (!loadString256(input, ActorID, Buffer, cONAM, BytesRead))
           {
             std::cerr << "Error while reading sub record ONAM of INFO!\n";
             return false;
           }
           hasONAM = true;
           break;
      case cRNAM:
           if (hasRNAM)
           {
             std::cerr << "Error: Record INFO seems to have two RNAM sub records.\n";
             return false;
           }
           // read race ID (RNAM)
           if (!loadString256(input, RaceID, Buffer, cRNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record RNAM of INFO!\n";
             return false;
           }
           hasRNAM = true;
           break;
      case cCNAM:
           if (hasCNAM)
           {
             std::cerr << "Error: Record INFO seems to have two CNAM sub records.\n";
             return false;
           }
           // read class ID (CNAM)
           if (!loadString256(input, ClassID, Buffer, cCNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record CNAM of INFO!\n";
             return false;
           }
           hasCNAM = true;
           break;
      case cFNAM:
           if (hasFNAM)
           {
             std::cerr << "Error: Record INFO seems to have two FNAM sub records.\n";
             return false;
           }
           // read faction ID (FNAM)
           if (!loadString256(input, FactionID, Buffer, cFNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record FNAM of INFO!\n";
             return false;
           }
           hasFNAM = true;
           break;
      case cANAM:
           if (hasANAM)
           {
             std::cerr << "Error: Record INFO seems to have two ANAM sub records.\n";
             return false;
           }
           // read cell name (ANAM)
           if (!loadString256(input, CellID, Buffer, cANAM, BytesRead))
           {
             std::cerr << "Error while reading sub record ANAM of INFO!\n";
             return false;
           }
           hasANAM = true;
           break;
      case cDNAM:
           if (hasDNAM)
           {
             std::cerr << "Error: Record INFO seems to have two DNAM sub records.\n";
             return false;
           }
           // read PC faction ID (DNAM)
           if (!loadString256(input, PCFactionID, Buffer, cDNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record DNAM of INFO!\n";
             return false;
           }
           hasDNAM = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cerr << "Error: Record INFO seems to have two SNAM sub records.\n";
             return false;
           }
           // read sound file path (SNAM)
           if (!loadString256(input, SoundFile, Buffer, cSNAM, BytesRead))
           {
             std::cerr << "Error while reading sub record SNAM of INFO!\n";
             return false;
           }
           hasSNAM = true;
           break;
      case cNAME:
           if (hasNAME)
           {
             std::cerr << "Error: Record INFO seems to have two NAME sub records.\n";
             return false;
           }
           // NAME's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 512)
           {
             std::cerr << "Error: Subrecord NAME of INFO is longer than 512 characters.\n";
             return false;
           }
           // read dialogue response
           memset(Buffer, '\0', 513);
           input.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record NAME of INFO!\n";
             return false;
           }
           Response = std::string(Buffer);
           hasNAME = true;
           break;
      case cSCVR:
           // SCVR's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength < 5)
           {
             std::cerr << "Error: Sub record SCVR of INFO is shorter than five characters.\n";
             return false;
           }
           if (SubLength > 255 + 5)
           {
             std::cerr << "Error: Sub record SCVR of INFO is longer than 255 characters.\n";
             return false;
           }
           //read function related stuff
           input.read(reinterpret_cast<char*>(&temp.Index), 1);
           input.read(reinterpret_cast<char*>(&temp.Type), 1);
           input.read(reinterpret_cast<char*>(&temp.Function), 2);
           input.read(reinterpret_cast<char*>(&temp.CompareOp), 1);
           BytesRead += 5;
           // --- read var name, if present
           memset(Buffer, '\0', 513);
           input.read(Buffer, SubLength - 5);
           BytesRead += (SubLength - 5);
           if (!input.good())
           {
             std::cerr << "Error while reading sub record SCVR of INFO!\n";
             return false;
           }
           temp.Name = std::string(Buffer);

           // directly after SCVR comes INTV or FLTV
           // read INTV/FLTV
           input.read(reinterpret_cast<char*>(&SubRecName), 4);
           BytesRead += 4;
           if ((SubRecName != cINTV) && (SubRecName != cFLTV))
           {
             std::cerr << "Error while reading INFO: Expected record name INTV "
                       << "or FLTV was not found. Instead, \""
                       << IntTo4Char(SubRecName) << "\" was found.\n";
             return false;
           }
           // INTV's/FLTV's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength != 4)
           {
             std::cerr << "Error: Sub record INTV or FLTV of INFO has invalid length ("
                       << SubLength << " bytes). Should be four bytes.\n";
             return false;
           }
           if (SubRecName == cINTV)
           {
             // It's an integer value.
             temp.isFloat = false;
             input.read(reinterpret_cast<char*>(&temp.iVal), 4);
             BytesRead += 4;
             temp.fVal = 0.0f;
           }
           else
           {
             // It's a float value.
             temp.isFloat = true;
             input.read(reinterpret_cast<char*>(&temp.fVal), 4);
             BytesRead += 4;
             temp.iVal = 0;
           }
           Functions.push_back(temp);
           break;
      case cBNAM:
           if (hasBNAM)
           {
             std::cerr << "Error: Record INFO seems to have two BNAM sub records.\n";
             return false;
           }
           // BNAM's length
           input.read(reinterpret_cast<char*>(&SubLength), 4);
           BytesRead += 4;
           if (SubLength > 767)
           {
             std::cerr << "Error: Sub record BNAM of INFO is longer than 767 characters.\n";
             return false;
           }
           // read result string
           memset(Buffer, '\0', 768);
           input.read(Buffer, SubLength);
           BytesRead += SubLength;
           if (!input.good())
           {
             std::cerr << "Error while reading sub record BNAM of INFO!\n";
             return false;
           }
           ResultString = std::string(Buffer);
           hasBNAM = true;
           break;
      case cQSTN:
           if (!loadByteFlag(input, isQuestName, Buffer, cQSTN, BytesRead))
           {
             return false;
           }
           break;
      case cQSTF:
           if (!loadByteFlag(input, isQuestFinished, Buffer, cQSTF, BytesRead))
           {
             return false;
           }
           break;
      case cQSTR:
           if (!loadByteFlag(input, isQuestRestart, Buffer, cQSTR, BytesRead))
           {
             return false;
           }
           break;
      default:
           std::cerr << "Error while reading INFO: Expected record name ONAM, "
                     << "RNAM, CNAM, FNAM, ANAM, DNAM, SNAM, NAME, SCVR, BNAM,"
                     << " QSTN, QSTF or QSTR not found. Instead, \""
                     << IntTo4Char(SubRecName) << "\" was found.\n";
           return false;
    }
  }

  return input.good();
}

} // namespace
