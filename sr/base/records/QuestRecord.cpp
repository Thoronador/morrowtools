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

#include "QuestRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* functions of index entry structure */

QuestRecord::IndexEntry::IndexEntry()
{
  index = 0;
  isFinisher = false;
  CNAM = 0;
}

bool QuestRecord::IndexEntry::operator==(const QuestRecord::IndexEntry& other) const
{
  return ((index==other.index) and (isFinisher==other.isFinisher) and (CNAM==other.CNAM));
}

/* quest record's functions */

QuestRecord::QuestRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  unknownFULL = 0;
  memset(unknownDNAM, 0, 12);
  filter = "";
  indices.clear();
}

QuestRecord::~QuestRecord()
{
  //empty
}

bool QuestRecord::equals(const QuestRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (unknownVMAD==other.unknownVMAD) and (unknownFULL==other.unknownFULL)
    and (unknownDNAM==other.unknownDNAM) and (filter==other.filter)
    and (indices==other.indices));
}

bool QuestRecord::saveToStream(std::ofstream& output) const
{
  #warning Not completely implemented yet!
  output.write((char*) &cQUST, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of strin +1 byte for NUL-termination */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +12 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize();
  }
  if (!filter.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +filter.length()+1 /* length of string +1 byte for NUL-termination */;
  }
  /// ... more to come
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write VMAD
  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of QUST!\n";
      return false;
    }
  }//if VMAD

  //write FULL
  output.write((char*) &cFULL, 4);
  //FULL's length
  subLength = 4; // fixed
  output.write((char*) &subLength, 2);
  //write FULL
  output.write((const char*) &unknownFULL, 4);

  //write DNAM
  output.write((char*) &cDNAM, 4);
  //DNAM's length
  subLength = 12; //fixed size
  output.write((char*) &subLength, 2);
  //write DNAM stuff
  output.write((const char*) unknownDNAM, 12);

  if (!filter.empty())
  {
    //write FLTR
    output.write((char*) &cFLTR, 4);
    //FLTR's length
    subLength = filter.length()+1;
    output.write((char*) &subLength, 2);
    //write filter string
    output.write(filter.c_str(), subLength);
  }//if FLTR

  #warning Code more!
  return false;
}

bool QuestRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName, lastReadRec;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of QUST is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of QUST!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  unknownFULL = 0;
  bool hasReadFULL = false;
  bool hasReadDNAM = false;
  memset(unknownDNAM, 0, 12);
  filter.clear();
  indices.clear();
  IndexEntry i_entry;
  lastReadRec = cEDID;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: QUST seems to have more than one VMAD subrecord.\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of QUST!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownVMAD.getSize();
           lastReadRec = cVMAD;
           break;
      case cFULL:
           if (hasReadFULL)
           {
             std::cout << "Error: QUST seems to have more than one FULL subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, unknownFULL)) return false;
           bytesRead += 6;
           hasReadFULL = true;
           lastReadRec = cFULL;
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: QUST seems to have more than one DNAM subrecord.\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record DNAM of QUST has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read DNAM
           in_File.read((char*) unknownDNAM, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of QUST!\n";
             return false;
           }
           hasReadDNAM = true;
           lastReadRec = cDNAM;
           break;
      case cFLTR:
           if (!filter.empty())
           {
             std::cout << "Error: QUST seems to have more than one FLTR subrecord.\n";
             return false;
           }
           //FLTR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record FLTR of QUST is longer than 511 characters!\n";
             return false;
           }
           //read EDID's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FLTR of QUST!\n";
             return false;
           }
           filter = std::string(buffer);
           lastReadRec = cFLTR;
           break;
      case cNEXT:
           //NEXT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout <<"Error: sub record NEXT of QUST has invalid length ("<<subLength
                       <<" bytes). Should be zero bytes.\n";
             return false;
           }
           lastReadRec = cNEXT;
           break;
      case cINDX:
           if ((lastReadRec==cINDX) or (lastReadRec==cQSDT))
           {
             /* Last entry seems to have a index only, or index and finishing
                 bit only, so push it!*/
             indices.push_back(i_entry);
           }
           else if ((lastReadRec!=cCNAM) and (lastReadRec!=cNEXT))
           {
             std::cout << "Error in subrecord sequence of QUST! INDX should "
                       << "follow after INDX or CNAM, but previous record was \""
                       << IntTo4Char(lastReadRec) <<"\".\n";
             return false;
           }
           //INDX's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record INDX of QUST has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read index
           in_File.read((char*) &(i_entry.index), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord INDX of QUST!\n";
             return false;
           }
           //preset for other fields of index entry
           i_entry.isFinisher = false;
           i_entry.CNAM = 0;
           lastReadRec = cINDX;
           break;
      case cQSDT:
           if (lastReadRec!=cINDX)
           {
             std::cout << "Error in subrecord sequence of QUST! QSDT should "
                       << "follow after INDX, but previous record was \""
                       << IntTo4Char(lastReadRec) <<"\".\n";
             return false;
           }
           //QSDT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record QSDT of QUST has invalid length ("<<subLength
                       <<" bytes). Should be one byte.\n";
             return false;
           }
           //read QSDT
           memset(buffer, 0, 1);
           in_File.read(buffer, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord QSDT of QUST!\n";
             return false;
           }
           i_entry.isFinisher = (buffer[0]!=0);
           lastReadRec = cQSDT;
           break;
      case cCNAM:
           if (lastReadRec!=cQSDT)
           {
             std::cout << "Error in subrecord sequence of QUST! CNAM should "
                       << "follow after QSDT, but previous record was \""
                       << IntTo4Char(lastReadRec) <<"\".\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record CNAM of QUST has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read CNAM
           in_File.read((char*) &(i_entry.CNAM), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of QUST!\n";
             return false;
           }
           //we have all three fields, push entry!
           indices.push_back(i_entry);
           lastReadRec = cCNAM;
           break;
      default:
          /* To implement the rest now would be A LOT of work, so I'll do this
             later and we just skip the rest until then.
          */
          #warning Skipping content! Function is not completely implemented yet!
          in_File.seekg(readSize-bytesRead, std::ios_base::cur);
          return in_File.good();
    }//swi
  }//while

  return in_File.good();
}

int32_t QuestRecord::getRecordType() const
{
  return cQUST;
}

} //namespace
