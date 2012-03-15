/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "WaterTypeRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

WaterTypeRecord::WaterTypeRecord()
: BasicRecord()
{
  editorID = "";
  hasFULL = false;
  unknownFULL = 0;
  unknownNNAMs.clear();
  unknownANAM = 0;
  unknownFNAM = 0;
  hasMNAM = false;
  unknownMNAM = 0;
  hasSNAM = false;
  unknownSNAM = 0;
  hasTNAM = false;
  unknownTNAM = 0;
  unknownDATA = 0;
  unknownDNAM.setPresence(false);
  memset(unknownGNAM, 0, 12);
  hasNAM0 = false;
  memset(unknownNAM0, 0, 12);
  hasNAM1 = false;
  memset(unknownNAM1, 0, 12);
}

WaterTypeRecord::~WaterTypeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool WaterTypeRecord::equals(const WaterTypeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasFULL==other.hasFULL) and ((unknownFULL==other.unknownFULL) or (!hasFULL))
      and (unknownNNAMs==other.unknownNNAMs) and (unknownANAM==other.unknownANAM)
      and (unknownFNAM==other.unknownFNAM) and (hasMNAM==other.hasMNAM)
      and ((unknownMNAM==other.unknownMNAM) or (!hasMNAM)) and (hasSNAM==other.hasSNAM)
      and ((unknownSNAM==other.unknownSNAM) or (!hasSNAM))
      and (hasTNAM==other.hasTNAM) and ((unknownTNAM==other.unknownTNAM) or (!hasTNAM))
      and (unknownDATA==other.unknownDATA) and (unknownDNAM==other.unknownDNAM)
      and (memcmp(unknownGNAM, other.unknownGNAM, 12)==0) and (hasNAM0==other.hasNAM0)
      and ((memcmp(unknownNAM0, other.unknownNAM0, 12)==0) or (!hasNAM0))
      and (hasNAM1==other.hasNAM1) and ((memcmp(unknownNAM1, other.unknownNAM1, 12)==0)
      or (!hasNAM1)));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t WaterTypeRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* ANAM */ +2 /* 2 bytes for length */ +1 /* fixed length */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +1 /* fixed length */
        +4 /* DATA */ +2 /* 2 bytes for length */ +2 /* fixed length */
        +4 /* GNAM */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* NAM0 */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* NAM1 */ +2 /* 2 bytes for length */ +12 /* fixed length */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if FULL
  if (hasMNAM)
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +1 /* fixed length */;
  }//if MNAM
  if (hasSNAM)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if SNAM
  if (hasTNAM)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if TNAM
  if (unknownDNAM.isPresent())
  {
    writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */ +unknownDNAM.getSize() /* length */;
  }//if DNAM
  if (hasNAM0)
  {
    writeSize = writeSize +4 /* NAM0 */ +2 /* 2 bytes for length */ +12 /* fixed length */;
  }//if NAM0
  if (hasNAM1)
  {
    writeSize = writeSize +4 /* NAM1 */ +2 /* 2 bytes for length */ +12 /* fixed length */;
  }//if NAM1
  return writeSize;
}

bool WaterTypeRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cWATR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write FULL's stuff
    output.write((const char*) &unknownFULL, 4);
  }

  //write ANAM
  output.write((const char*) &cANAM, 4);
  //ANAM's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write ANAM's stuff
  output.write((const char*) &unknownANAM, 1);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 1;
  output.write((const char*) &subLength, 2);
  //write FNAM's stuff
  output.write((const char*) &unknownFNAM, 1);

  if (hasMNAM)
  {
    //write MNAM
    output.write((const char*) &cMNAM, 4);
    //MNAM's length
    subLength = 1;
    output.write((const char*) &subLength, 2);
    //write MNAM's stuff
    output.write((const char*) &unknownMNAM, 1);
  }//if MNAM

  if (hasSNAM)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write SNAM's stuff
    output.write((const char*) &unknownSNAM, 4);
  }

  if (hasTNAM)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4;
    output.write((const char*) &subLength, 2);
    //write TNAM's stuff
    output.write((const char*) &unknownTNAM, 4);
  }

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 2; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's stuff
  output.write((const char*) &unknownDATA, 2);

  if (unknownDNAM.isPresent())
  {
    //write DNAM
    if (!unknownDNAM.saveToStream(output, cDNAM))
    {
      std::cout << "Error while writing subrecord DNAM of WATR!\n";
      return false;
    }
  }//if DNAM is present

  //write GNAM
  output.write((const char*) &cGNAM, 4);
  //GNAM's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write GNAM's stuff
  output.write((const char*) unknownGNAM, 12);

  if (hasNAM0)
  {
    //write NAM0
    output.write((const char*) &cNAM0, 4);
    //NAM0's length
    subLength = 12; //fixed
    output.write((const char*) &subLength, 2);
    //write NAM0's stuff
    output.write((const char*) unknownNAM0, 12);
  }//if has NAM0

  if (hasNAM1)
  {
    //write NAM1
    output.write((const char*) &cNAM1, 4);
    //NAM1's length
    subLength = 12; //fixed
    output.write((const char*) &subLength, 2);
    //write NAM1's stuff
    output.write((const char*) unknownNAM1, 12);
  }//if has NAM1

  return output.good();
}
#endif

bool WaterTypeRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
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
    std::cout <<"Error: sub record EDID of WATR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of WATR!\n";
    return false;
  }
  editorID = std::string(buffer);

  hasFULL = false;
  unknownNNAMs.clear();
  bool hasReadANAM = false;
  bool hasReadFNAM = false;
  hasMNAM = false;
  hasSNAM = false;
  hasTNAM = false;
  bool hasReadDATA = false;
  unknownDNAM.setPresence(false);
  bool hasReadGNAM = false;
  hasNAM0 = false;
  hasNAM1 = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: WATR seems to have more than one FULL subrecord!\n";
             return false;
           }
           //FULL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord FULL of WATR has invalid length ("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read FULL's stuff
           in_File.read((char*) &unknownFULL, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FULL of WATR!\n";
             return false;
           }//if
           hasFULL = true;
           break;

      case cNNAM:
           if (unknownNNAMs.size()>=3)
           {
             std::cout << "Error: WATR seems to have more than three NNAM subrecords!\n";
             return false;
           }
           //NNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record NNAM of WATR is longer than 511 characters!\n";
             return false;
           }
           //read NNAM's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NNAM of WATR!\n";
             return false;
           }//if
           unknownNNAMs.push_back(std::string(buffer));
           break;
      case cANAM:
           if (hasReadANAM)
           {
             std::cout << "Error: WATR seems to have more than one ANAM subrecord!\n";
             return false;
           }
           //ANAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: subrecord ANAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read ANAM's stuff
           in_File.read((char*) &unknownANAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ANAM of WATR!\n";
             return false;
           }//if
           hasReadANAM = true;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: WATR seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: subrecord FNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read FNAM's stuff
           in_File.read((char*) &unknownFNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of WATR!\n";
             return false;
           }//if
           hasReadFNAM = true;
           break;
      case cMNAM:
           if (hasMNAM)
           {
             std::cout << "Error: WATR seems to have more than one MNAM subrecord!\n";
             return false;
           }
           //MNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: subrecord MNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be one byte!\n";
             return false;
           }
           //read MNAM's stuff
           in_File.read((char*) &unknownMNAM, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MNAM of WATR!\n";
             return false;
           }//if
           hasMNAM = true;
           break;
      case cSNAM:
           if (hasSNAM)
           {
             std::cout << "Error: WATR seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //SNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord SNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read SNAM's stuff
           in_File.read((char*) &unknownSNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SNAM of WATR!\n";
             return false;
           }//if
           hasSNAM = true;
           break;
      case cTNAM:
           if (hasTNAM)
           {
             std::cout << "Error: WATR seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //TNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord TNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be four bytes!\n";
             return false;
           }
           //read TNAM's stuff
           in_File.read((char*) &unknownTNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord TNAM of WATR!\n";
             return false;
           }//if
           hasTNAM = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: WATR seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: subrecord DATA of WATR has invalid length ("<<subLength
                       <<" bytes). Should be two bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) &unknownDATA, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of WATR!\n";
             return false;
           }//if
           hasReadDATA = true;
           break;
      case cDNAM:
           if (unknownDNAM.isPresent())
           {
             std::cout << "Error: WATR seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //read DNAM
           if (!unknownDNAM.loadFromStream(in_File, cDNAM, false))
           {
             std::cout << "Error while reading subrecord DNAM of WATR!\n";
             return false;
           }
           //check length
           if (unknownDNAM.getSize()!=228)
           {
             std::cout <<"Error: subrecord DNAM of WATR has invalid length ("
                       <<unknownDNAM.getSize() <<" bytes). Should be 228 bytes!\n";
             return false;
           }
           bytesRead = bytesRead +2 +228;
           break;
      case cGNAM:
           if (hasReadGNAM)
           {
             std::cout << "Error: WATR seems to have more than one GNAM subrecord!\n";
             return false;
           }
           //GNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: subrecord GNAM of WATR has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read GNAM's stuff
           in_File.read((char*) unknownGNAM, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord GNAM of WATR!\n";
             return false;
           }//if
           hasReadGNAM = true;
           break;
      case cNAM0:
           if (hasNAM0)
           {
             std::cout << "Error: WATR seems to have more than one NAM0 subrecord!\n";
             return false;
           }
           //NAM0's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: subrecord NAM0 of WATR has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read NAM0's stuff
           in_File.read((char*) unknownNAM0, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM0 of WATR!\n";
             return false;
           }//if
           hasNAM0 = true;
           break;
      case cNAM1:
           if (hasNAM1)
           {
             std::cout << "Error: WATR seems to have more than one NAM1 subrecord!\n";
             return false;
           }
           //NAM1's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: subrecord NAM1 of WATR has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read NAM1's stuff
           in_File.read((char*) unknownNAM1, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM1 of WATR!\n";
             return false;
           }//if
           hasNAM1 = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                    << "\" found, but only NNAM, ANAM, FNAM, MNAM, SNAM, DATA,"
                    << " DNAM, GNAM, NAM0 or NAM1 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadANAM and hasReadFNAM and hasReadDATA and unknownDNAM.isPresent()
       and hasReadGNAM))
  {
    std::cout << "Error: at least one subrecord of WATR is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t WaterTypeRecord::getRecordType() const
{
  return cWATR;
}

} //namespace
