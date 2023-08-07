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

#include "LoadScreenRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

LoadScreenRecord::LoadScreenRecord()
: BasicRecord(), editorID(""),
  text(LocalizedString()),
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>()),
  unknownNNAM(0),
  unknownSNAM(0),
  unknownONAM(0),
  unknownMOD2("")
{
  memset(unknownRNAM, 0, 6);
  memset(unknownXNAM, 0, 12);
}

LoadScreenRecord::~LoadScreenRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool LoadScreenRecord::equals(const LoadScreenRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (text==other.text) and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s)
      and (unknownNNAM==other.unknownNNAM)
      and (unknownSNAM==other.unknownSNAM)
      and (memcmp(unknownRNAM, other.unknownRNAM, 6)==0)
      and (unknownONAM==other.unknownONAM)
      and (memcmp(unknownXNAM, other.unknownXNAM, 12)==0)
      and (unknownMOD2==other.unknownMOD2));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t LoadScreenRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +text.getWriteSize() /* DESC */
        +4 /* NNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* RNAM */ +2 /* 2 bytes for length */ +6 /* fixed size */
        +4 /* ONAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* XNAM */ +2 /* 2 bytes for length */ +12 /* fixed size */;
  if (!unknownCTDA_CIS2s.empty())
  {
    std::vector<CTDA_CIS2_compound>::size_type i;
    for (i=0; i<unknownCTDA_CIS2s.size(); ++i)
    {
      writeSize += unknownCTDA_CIS2s[i].getWriteSize();
    }//for
  }
  if (!unknownMOD2.empty())
  {
    writeSize = writeSize +4 /* MOD2 */ +2 /* 2 bytes for length */
        +unknownMOD2.length()+1 /* length of name +1 byte for NUL termination */;
  }
  return writeSize;
}

bool LoadScreenRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cLSCR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write DESC
  if (!text.saveToStream(output, cDESC))
    return false;

  std::vector<CTDA_CIS2_compound>::size_type i;
  for (i=0; i<unknownCTDA_CIS2s.size(); ++i)
  {
    //write CTDA and CIS2
    if (!unknownCTDA_CIS2s[i].saveToStream(output))
    {
      std::cerr << "Error while writing CTDA and CIS2 of LSCR!\n";
      return false;
    }
  }//for

  //write NNAM
  output.write((const char*) &cNNAM, 4);
  //NNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write NNAM
  output.write((const char*) &unknownNNAM, 4);

  //write SNAM
  output.write((const char*) &cSNAM, 4);
  //SNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write SNAM
  output.write((const char*) &unknownSNAM, 4);

  //write RNAM
  output.write((const char*) &cRNAM, 4);
  //RNAM's length
  subLength = 6;
  output.write((const char*) &subLength, 2);
  //write RNAM's stuff
  output.write((const char*) unknownRNAM, 6);

  //write ONAM
  output.write((const char*) &cONAM, 4);
  //ONAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write ONAM
  output.write((const char*) &unknownONAM, 4);

  //write XNAM
  output.write((const char*) &cXNAM, 4);
  //XNAM's length
  subLength = 12;
  output.write((const char*) &subLength, 2);
  //write XNAM's stuff
  output.write((const char*) unknownXNAM, 12);

  if (!unknownMOD2.empty())
  {
    //write MOD2
    output.write((const char*) &cMOD2, 4);
    //MOD2's length
    subLength = unknownMOD2.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(unknownMOD2.c_str(), subLength);
  }

  return output.good();
}
#endif

bool LoadScreenRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
  {
    std::cerr << "Error while reading subrecord EDID of LSCR!\n";
    return false;
  }

  text.reset();
  unknownCTDA_CIS2s.clear();
  CTDAData tempCTDA;
  bool hasReadNNAM = false;
  bool hasReadSNAM = false;
  bool hasReadRNAM = false;
  bool hasReadONAM = false;
  bool hasReadXNAM = false;
  unknownMOD2.clear();
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cDESC:
           if (text.isPresent())
           {
             std::cerr << "Error: LSCR seems to have more than one DESC subrecord.\n";
             return false;
           }
           //read DESC
           if (!text.loadFromStream(in_File, cDESC, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cCTDA:
           // read CTDA
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of LSCR!\n";
             return false;
           }
           unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           break;
      case cCIS2:
           if (unknownCTDA_CIS2s.empty())
           {
             std::cerr << "Error: found CIS2 without prior CTDA in LSCR!\n";
             return false;
           }
           if (!unknownCTDA_CIS2s.back().unknownCISx.empty())
           {
             std::cerr << "Error: LSCR seems to have more than one CIS2 subrecord per CTDA.\n";
             return false;
           }
           //read CIS2
           if (!loadString512FromStream(in_File, unknownCTDA_CIS2s.back().unknownCISx, buffer, cCIS2, false, bytesRead))
           {
             std::cerr << "Error while reading subrecord CIS2 of LSCR!\n";
             return false;
           }
           break;
      case cNNAM:
           if (hasReadNNAM)
           {
             std::cerr << "Error: LSCR seems to have more than one NNAM subrecord.\n";
             return false;
           }
           //read NNAM
           if (!loadUint32SubRecordFromStream(in_File, cNNAM, unknownNNAM, false)) return false;
           bytesRead += 6;
           hasReadNNAM = true;
           break;
      case cSNAM:
           if (hasReadSNAM)
           {
             std::cerr << "Error: LSCR seems to have more than one SNAM subrecord.\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, unknownSNAM, false)) return false;
           bytesRead += 6;
           hasReadSNAM = true;
           break;
      case cRNAM:
           if (hasReadRNAM)
           {
             std::cerr << "Error: LSCR seems to have more than one RNAM subrecord.\n";
             return false;
           }
           //RNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=6)
           {
             std::cerr <<"Error: sub record RNAM of LSCR has invalid length ("<<subLength
                       <<" bytes). Should be six bytes.\n";
             return false;
           }
           //read RNAM
           in_File.read((char*) unknownRNAM, 6);
           bytesRead += 6;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord RNAM of LSCR!\n";
             return false;
           }
           hasReadRNAM = true;
           break;
      case cONAM:
           if (hasReadONAM)
           {
             std::cerr << "Error: LSCR seems to have more than one ONAM subrecord.\n";
             return false;
           }
           //read ONAM
           if (!loadUint32SubRecordFromStream(in_File, cONAM, unknownONAM, false)) return false;
           bytesRead += 6;
           hasReadONAM = true;
           break;
      case cXNAM:
           if (hasReadXNAM)
           {
             std::cerr << "Error: LSCR seems to have more than one XNAM subrecord.\n";
             return false;
           }
           //XNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr <<"Error: sub record XNAM of LSCR has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read XNAM
           in_File.read((char*) unknownXNAM, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XNAM of LSCR!\n";
             return false;
           }
           hasReadXNAM = true;
           break;
      case cMOD2:
           if (!unknownMOD2.empty())
           {
             std::cerr << "Error: LSCR seems to have more than one MOD2 subrecord.\n";
             return false;
           }
           //read MOD2
           if (!loadString512FromStream(in_File, unknownMOD2, buffer, cMOD2, false, bytesRead))
           {
             std::cerr << "Error while reading subrecord MOD2 of LSCR!\n";
             return false;
           }
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only DESC, CTDA, NNAM, SNAM, RNAM, ONAM, XNAM or MOD2 are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(text.isPresent() and hasReadNNAM and hasReadSNAM and hasReadRNAM
      and hasReadONAM and hasReadXNAM))
  {
    std::cerr << "Error while reading LSCR record: at least one required "
              << "subrecord is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t LoadScreenRecord::getRecordType() const
{
  return cLSCR;
}

} //namespace
