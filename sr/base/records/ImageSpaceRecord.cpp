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

#include "ImageSpaceRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ImageSpaceRecord::ImageSpaceRecord()
: BasicRecord()
{
  editorID = "";
  unsigned int i;
  hasENAM = false;
  for (i=0; i<14; ++i)
  {
    unknownENAM[i] = 0.0f;
  }
  unknownHNAM.setPresence(false);
  unknownCNAM.setPresence(false);
  unknownTNAM.setPresence(false);
  unknownDNAM.setPresence(false);
}

ImageSpaceRecord::~ImageSpaceRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ImageSpaceRecord::equals(const ImageSpaceRecord& other) const
{
  if ((equalsBasic(other)) and (editorID==other.editorID)
      and (hasENAM==other.hasENAM))
  {
    unsigned int i;
    if (hasENAM)
    {
      for (i=0; i<14; ++i)
      {
        if (unknownENAM[i]!=other.unknownENAM[i]) return false;
      }
    }//if ENAM
    return ((unknownHNAM==other.unknownHNAM) and (unknownCNAM==other.unknownCNAM)
        and (unknownTNAM==other.unknownTNAM) and (unknownDNAM==other.unknownDNAM));
  }
  return false;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ImageSpaceRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */;
  if (hasENAM)
  {
    writeSize = writeSize +4 /* ENAM */ +2 /* 2 bytes for length */ +56 /* fixed length of 56 bytes */;
  }
  if (unknownHNAM.isPresent())
  {
    writeSize = writeSize +4 /* HNAM */ +2 /* 2 bytes for length */ +unknownHNAM.getSize();
  }
  if (unknownCNAM.isPresent())
  {
    writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */ +unknownCNAM.getSize();
  }
  if (unknownTNAM.isPresent())
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +unknownTNAM.getSize();
  }
  if (unknownDNAM.isPresent())
  {
    writeSize = writeSize +4 /* DNAM */ +2 /* 2 bytes for length */ +unknownDNAM.getSize() /* fixed length of 16 bytes */;
  }
  return writeSize;
}

bool ImageSpaceRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cIMGS, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  unsigned int i;

  if (hasENAM)
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    //ENAM's length
    subLength = 56; //fixed at 56
    output.write((const char*) &subLength, 2);
    //write ENAM's content
    for (i=0; i<14; ++i)
    {
      output.write((const char*) &(unknownENAM[i]), 4);
    }
  }//if

  if (unknownHNAM.isPresent())
  {
    //write HNAM
    if (!unknownHNAM.saveToStream(output, cHNAM))
    {
      std::cout << "Error while writing subrecord HNAM of IMGS!\n";
      return false;
    }
  }

  if (unknownCNAM.isPresent())
  {
    //write CNAM
    if (!unknownCNAM.saveToStream(output, cCNAM))
    {
      std::cout << "Error while writing subrecord CNAM of IMGS!\n";
      return false;
    }
  }

  if (unknownTNAM.isPresent())
  {
    //write TNAM
    if (!unknownTNAM.saveToStream(output, cTNAM))
    {
      std::cout << "Error while writing subrecord TNAM of IMGS!\n";
      return false;
    }
  }

  if (unknownDNAM.isPresent())
  {
    //write DNAM
    if (!unknownDNAM.saveToStream(output, cDNAM))
    {
      std::cout << "Error while writing subrecord DNAM of IMGS!\n";
      return false;
    }
  }

  return output.good();
}
#endif

bool ImageSpaceRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of IMGS is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of IMGS!\n";
    return false;
  }
  editorID = std::string(buffer);

  unsigned int i;

  hasENAM = false;
  unknownHNAM.setPresence(false);
  unknownCNAM.setPresence(false);
  unknownTNAM.setPresence(false);
  unknownDNAM.setPresence(false);
  while (bytesRead<readSize)
  {
    //read next subrec
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cENAM:
           if (hasENAM)
           {
             std::cout << "Error: IMGS seems to have more than one ENAM subrecord!\n";
             return false;
           }
           //ENAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=56)
           {
             std::cout <<"Error: subrecord ENAM of IMGS has invalid length ("<<subLength
                       <<" bytes). Should be 56 bytes!\n";
             return false;
           }
           //read ENAM's stuff
           for (i=0; i<14; ++i)
           {
             in_File.read((char*) &(unknownENAM[i]), 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord ENAM of IMGS!\n";
               return false;
             }//if
           }//for
           hasENAM = true;
           break;
      case cHNAM:
           if (unknownHNAM.isPresent())
           {
             std::cout << "Error: IMGS seems to have more than one HNAM subrecord!\n";
             return false;
           }
           //read HNAM
           if (!unknownHNAM.loadFromStream(in_File, cHNAM, false))
           {
             std::cout << "Error while reading subrecord HNAM of IMGS!\n";
             return false;
           }
           //check length
           if (unknownHNAM.getSize()!=36)
           {
             std::cout <<"Error: subrecord HNAM of IMGS has invalid length ("
                       <<unknownHNAM.getSize()<<" bytes). Should be 36 bytes!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + 36;
           break;
      case cCNAM:
           if (unknownCNAM.isPresent())
           {
             std::cout << "Error: IMGS seems to have more than one CNAM subrecord!\n";
             return false;
           }
           //read CNAM
           if (!unknownCNAM.loadFromStream(in_File, cCNAM, false))
           {
             std::cout << "Error while reading subrecord CNAM of IMGS!\n";
             return false;
           }
           //check length
           if (unknownCNAM.getSize()!=12)
           {
             std::cout <<"Error: subrecord CNAM of IMGS has invalid length ("
                       <<unknownCNAM.getSize()<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + 12;
           break;
      case cTNAM:
           if (unknownTNAM.isPresent())
           {
             std::cout << "Error: IMGS seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //read TNAM
           if (!unknownTNAM.loadFromStream(in_File, cTNAM, false))
           {
             std::cout << "Error while reading subrecord TNAM of IMGS!\n";
             return false;
           }
           //check length
           if (unknownTNAM.getSize()!=16)
           {
             std::cout <<"Error: subrecord TNAM of IMGS has invalid length ("
                       <<unknownTNAM.getSize()<<" bytes). Should be 16 bytes!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + 16;
           break;
      case cDNAM:
           if (unknownDNAM.isPresent())
           {
             std::cout << "Error: IMGS seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //read DNAM
           if (!unknownDNAM.loadFromStream(in_File, cDNAM, false))
           {
             std::cout << "Error while reading subrecord DNAM of IMGS!\n";
             return false;
           }
           //check length
           if ((unknownDNAM.getSize()!=16) and (unknownDNAM.getSize()!=12))
           {
             std::cout <<"Error: subrecord DNAM of IMGS has invalid length ("
                       <<unknownDNAM.getSize()<<" bytes). Should be 12 or 16 bytes!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownDNAM.getSize();
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only HNAM, CNAM, TNAM or DNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

uint32_t ImageSpaceRecord::getRecordType() const
{
  return cIMGS;
}

} //namespace
