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

#include "SoundRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>

namespace SRTP
{

SoundRecord::SoundRecord()
: BasicRecord()
{
  editorID = "";
  unknownSDSC = 0;
  memset(unknownOBND, 0, 12);
  soundFileName = "";
  hasSNDD = false;
  memset(unknownSNDD, 0, 36);
}

SoundRecord::~SoundRecord()
{
  //empty
}

uint32_t SoundRecord::getRecordType() const
{
  return cSOUN;
}

#ifndef SR_NO_RECORD_EQUALITY
bool SoundRecord::equals(const SoundRecord& other) const
{
  if ((editorID==other.editorID) and (unknownSDSC==other.unknownSDSC)
    and (memcmp(unknownOBND, other.unknownOBND, 12)==0) and (equalsBasic(other))
    and (soundFileName==other.soundFileName) and (hasSNDD==other.hasSNDD))
  {
    if (hasSNDD) return (memcmp(unknownSNDD, other.unknownSNDD, 36)==0);
    return true;
  }
  return false;
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SoundRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* SDSC */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (hasSNDD)
  {
    writeSize = writeSize +4 /* SNDD */ +2 /* 2 bytes for length */ +36 /* fixed size */;
  }
  if (soundFileName!="")
  {
    writeSize = writeSize + 4 /* FNAM */ +2 /* 2 bytes for length */
               +soundFileName.length()+1 /* length of name +1 byte for NUL termination */;
  }
  return writeSize;
}

bool SoundRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cSOUN, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed size
  output.write((char*) &subLength, 2);
  //write OBND data
  output.write((char*) unknownOBND, 12);

  if (soundFileName!="")
  {
    //write FNAM
    output.write((char*) &cFNAM, 4);
    //FNAM's length
    subLength = soundFileName.length()+1;
    output.write((char*) &subLength, 2);
    //write file name
    output.write(soundFileName.c_str(), subLength);
  }

  if (hasSNDD)
  {
    //write SNDD
    output.write((char*) &cSNDD, 4);
    //SNDD's length
    subLength = 36; //fixed size
    output.write((char*) &subLength, 2);
    //write SNDD data
    output.write((char*) unknownSNDD, 36);
  }//if

  //write SDSC
  output.write((char*) &cSDSC, 4);
  //SDSC's length
  subLength = 4; //fixed size
  output.write((char*) &subLength, 2);
  //write SDSC data
  output.write((char*) &unknownSDSC, 4);

  return output.good();
}
#endif

bool SoundRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  uint32_t bytesRead;
  subRecName = subLength = 0;

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
    std::cout <<"Error: sub record EDID of SOUN is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SOUN!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read OBND
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
  if (subRecName!=cOBND)
  {
    UnexpectedRecord(cOBND, subRecName);
    return false;
  }
  //OBND's length
  in_File.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=12)
  {
    std::cout <<"Error: sub record OBND of SOUN has invalid length ("<<subLength
              <<" bytes). Should be 12 bytes.\n";
    return false;
  }
  //read OBND
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of SOUN!\n";
    return false;
  }

  //optional stuff and rest
  hasSNDD = false;
  bool hasDoneSNDD = false;
  bool hasDoneSDSC = false;
  bool hasDoneFNAM = false;
  soundFileName = "";
  while (bytesRead<readSize)
  {
    //read next subrec header
    subRecName = 0;
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cSNDD:
           if (hasDoneSNDD)
           {
             std::cout << "Error: SOUN seems to have more than one SNDD subrecord.\n";
             return false;
           }
           //SNDD's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=36)
           {
             std::cout <<"Error: sub record SNDD of SOUN has invalid length ("<<subLength
                       <<" bytes). Should be 36 bytes.\n";
             return false;
           }
           //read SNDD
           in_File.read((char*) unknownSNDD, 36);
           bytesRead += 36;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SNDD of SOUN!\n";
             return false;
           }
           hasSNDD = true;
           hasDoneSNDD = true;
           break;
      case cFNAM:
           if (hasDoneFNAM)
           {
             std::cout << "Error: SOUN seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record FNAM of SOUN is longer than 511 characters!\n";
             return false;
           }
           //read EDID's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FNAM of SOUN!\n";
             return false;
           }
           soundFileName = std::string(buffer);
           hasDoneFNAM = true;
           break;
      case cSDSC:
           if (hasDoneSDSC)
           {
             std::cout << "Error: SOUN seems to have more than one SDSC subrecord.\n";
             return false;
           }
           //SDSC's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record SDSC of SOUN has invalid length ("
                       <<subLength<<" bytes). Should be 4 bytes.\n";
             return false;
           }
           //read SDSC's stuff
           in_File.read((char*) &unknownSDSC, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SDSC of SOUN!\n";
             return false;
           }
           hasDoneSDSC = true;
           break;
      default:
           std::cout << "Error while reading record SOUN: Found unexpected "
                     << "subrecord type \""<<IntTo4Char(subRecName)
                     <<"\", but only FNAM, SNDD or SDSC are allowed!\n";
           return false;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
