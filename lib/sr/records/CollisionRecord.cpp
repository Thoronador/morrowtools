/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#include "CollisionRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

CollisionRecord::CollisionRecord()
: BasicRecord(), editorID(""),
  description(LocalizedString()),
  unknownBNAM(0),
  unknownFNAM(0),
  unknownGNAM(0),
  unknownMNAM(""),
  unknownCNAMs(std::vector<uint32_t>())
{

}

CollisionRecord::~CollisionRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool CollisionRecord::equals(const CollisionRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (description==other.description) and (unknownBNAM==other.unknownBNAM)
      and (unknownFNAM==other.unknownFNAM) and (unknownGNAM==other.unknownGNAM)
      and (unknownMNAM==other.unknownMNAM) and (unknownCNAMs==other.unknownCNAMs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t CollisionRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +description.getWriteSize() /* DESC */
        +4 /* BNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* GNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* MNAM */ +2 /* 2 bytes for length */
        +unknownMNAM.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* INTV */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (!unknownCNAMs.empty())
  {
    writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */ +4*unknownCNAMs.size() /* fixed size */;
  }
  return writeSize;
}

bool CollisionRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cCOLL, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DESC
  if (!description.saveToStream(output, cDESC))
    return false;

  //write BNAM
  output.write((const char*) &cBNAM, 4);
  //BNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write BNAM's stuff
  output.write((const char*) &unknownBNAM, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &unknownFNAM, 4);

  //write GNAM
  output.write((const char*) &cGNAM, 4);
  //GNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write GNAM's stuff
  output.write((const char*) &unknownGNAM, 4);

  //write MNAM
  output.write((const char*) &cMNAM, 4);
  //MNAM's length
  subLength = unknownMNAM.length()+1;
  output.write((const char*) &subLength, 2);
  //write MNAM
  output.write(unknownMNAM.c_str(), subLength);

  //write INTV
  output.write((const char*) &cINTV, 4);
  //INTV's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write INTV's stuff
  const uint32_t count = unknownCNAMs.size();
  output.write((const char*) &count, 4);

  if (count>0)
  {
    //write CNAM
    output.write((const char*) &cCNAM, 4);
    //CNAM's length
    subLength = 4*count; //fixed size
    output.write((const char*) &subLength, 2);
    //write INTV's stuff
    unsigned int i;
    for (i=0; i<count; ++i)
    {
      output.write((const char*) &(unknownCNAMs[i]), 4);
    }//for
  }//if count

  return output.good();
}
#endif

bool CollisionRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
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
    std::cerr <<"Error: sub record EDID of COLL is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of COLL!\n";
    return false;
  }
  editorID = std::string(buffer);

  description.reset();
  bool hasReadBNAM = false;
  bool hasReadFNAM = false;
  bool hasReadGNAM = false;
  unknownMNAM.clear();
  bool hasReadINTV = false;
  unknownCNAMs.clear();
  uint32_t i, size_int, tempUint32;
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cDESC:
           if (description.isPresent())
           {
             std::cerr << "Error: COLL seems to have more than one DESC subrecord.\n";
             return false;
           }
           //read DESC
           if (!description.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cBNAM:
           if (hasReadBNAM)
           {
             std::cerr << "Error: COLL seems to have more than one BNAM subrecord.\n";
             return false;
           }
           //read BNAM
           if (!loadUint32SubRecordFromStream(in_File, cBNAM, unknownBNAM, false)) return false;
           bytesRead += 6;
           hasReadBNAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: COLL seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //read FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, unknownFNAM, false)) return false;
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      case cGNAM:
           if (hasReadGNAM)
           {
             std::cerr << "Error: COLL seems to have more than one GNAM subrecord.\n";
             return false;
           }
           //read GNAM
           if (!loadUint32SubRecordFromStream(in_File, cGNAM, unknownGNAM, false)) return false;
           bytesRead += 6;
           hasReadGNAM = true;
           break;
      case cMNAM:
           if (!unknownMNAM.empty())
           {
             std::cerr << "Error: COLL seems to have more than one MNAM subrecord.\n";
             return false;
           }
           //MNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: sub record MNAM of COLL is longer than 511 characters!\n";
             return false;
           }
           //read MNAM's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord MNAM of COLL!\n";
             return false;
           }
           unknownMNAM = std::string(buffer);
           break;
      case cINTV:
           if (hasReadINTV)
           {
             std::cerr << "Error: COLL seems to have more than one INTV subrecord.\n";
             return false;
           }
           //read INTV
           if (!loadUint32SubRecordFromStream(in_File, cINTV, size_int, false)) return false;
           bytesRead += 6;
           hasReadINTV = true;
           if (size_int>0)
           {
             //read CNAM
             in_File.read((char*) &subRecName, 4);
             bytesRead += 4;
             if (subRecName!=cCNAM)
             {
               UnexpectedRecord(cCNAM, subRecName);
               return false;
             }
             //CNAM's length
             in_File.read((char*) &subLength, 2);
             bytesRead += 2;
             if (subLength!=4*size_int)
             {
               std::cerr <<"Error: sub record CNAM of COLL has invalid length ("
                         <<subLength<<" bytes). Should be "<<4*size_int<<" bytes.\n";
               return false;
             }
             //read CNAM
             for (i=0; i<size_int; ++i)
             {
               tempUint32 = 0;
               in_File.read((char*) &tempUint32, 4);
               bytesRead += 4;
               if (!in_File.good())
               {
                 std::cerr << "Error while reading subrecord CNAM of COLL!\n";
                 return false;
               }
               unknownCNAMs.push_back(tempUint32);
             }//for
           }//if size>0
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only DESC, BNAM, FNAM, GNAM, MNAM or INTV are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(description.isPresent() and hasReadBNAM and hasReadFNAM and hasReadGNAM
      and (!unknownMNAM.empty()) and hasReadINTV))
  {
    std::cerr << "Error: at least one subrecord of COLL is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t CollisionRecord::getRecordType() const
{
  return cCOLL;
}

} //namespace
