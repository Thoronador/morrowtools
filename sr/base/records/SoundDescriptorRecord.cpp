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

#include "SoundDescriptorRecord.hpp"
#include <iostream>
#include <cstring>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

SoundDescriptorRecord::SoundDescriptorRecord()
: BasicRecord(), editorID(""),
  unknownCNAM(0),
  categoryFormID(0), //subrecord GNAM
  alternateFormID(0), //subrecord SNAM
  waveFiles(std::vector<std::string>()), //subrecords ANAM
  outputModelFormID(0), //subrecord ONAM
  hasFNAM(false), unknownFNAM(0),
  conditions(std::vector<CTDAData>()),
  hasLNAM(false), unknownLNAM(0),
  //subrecord BNAM
  frequencyShiftPercentage(0),
  frequencyVariancePercentage(0),
  priority(0),
  dB_Variance(0),
  staticAttenuation_x100(0) //real value is that divided by 100
  //end of subrecord BNAM
{

}

SoundDescriptorRecord::~SoundDescriptorRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool SoundDescriptorRecord::equals(const SoundDescriptorRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownCNAM==other.unknownCNAM) and (categoryFormID==other.categoryFormID)
      and (alternateFormID==other.alternateFormID) and (waveFiles==other.waveFiles)
      and (outputModelFormID==other.outputModelFormID) and (conditions==other.conditions)
      and (hasFNAM==other.hasFNAM) and ((unknownFNAM==other.unknownFNAM) or (!hasFNAM))
      and (hasLNAM==other.hasLNAM) and ((unknownLNAM==other.unknownLNAM) or (!hasLNAM))
      and (frequencyShiftPercentage==other.frequencyShiftPercentage)
      and (frequencyVariancePercentage==other.frequencyVariancePercentage)
      and (priority==other.priority) and (dB_Variance==other.dB_Variance)
      and (staticAttenuation_x100==other.staticAttenuation_x100));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t SoundDescriptorRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* BNAM */ +2 /* 2 bytes for length */ +6 /* fixed size of 6 bytes */
        +conditions.size()*(4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed size of 32 bytes */);
  if (categoryFormID!=0)
  {
    writeSize = writeSize +4 /* GNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  if (alternateFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  unsigned int i;
  for (i=0; i<waveFiles.size(); ++i)
  {
    writeSize = writeSize +4 /* ANAM */ +2 /* 2 bytes for length */
        +waveFiles[i].length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (outputModelFormID!=0)
  {
    writeSize = writeSize +4 /* ONAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  if (hasFNAM)
  {
    writeSize = writeSize +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  if (hasLNAM)
  {
    writeSize = writeSize+4 /* LNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool SoundDescriptorRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cSNDR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write CNAM's stuff
  output.write((const char*) &unknownCNAM, 4);

  if (categoryFormID!=0)
  {
    //write GNAM
    output.write((const char*) &cGNAM, 4);
    //GNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write category form ID
    output.write((const char*) &categoryFormID, 4);
  }//if GNAM

  if (alternateFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write alternate form ID
    output.write((const char*) &alternateFormID, 4);
  }//if SNAM

  unsigned int i;
  for (i=0; i<waveFiles.size(); ++i)
  {
    //write ANAM
    output.write((const char*) &cANAM, 4);
    //ANAM's length
    subLength = waveFiles[i].length()+1;
    output.write((const char*) &subLength, 2);
    //write file path
    output.write(waveFiles[i].c_str(), subLength);
  }

  if (outputModelFormID!=0)
  {
    //write ONAM
    output.write((const char*) &cONAM, 4);
    //ONAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write output model form ID
    output.write((const char*) &outputModelFormID, 4);
  }//if ONAM

  if (hasFNAM)
  {
    //write FNAM
    output.write((const char*) &cFNAM, 4);
    //FNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FNAM
    output.write((const char*) &unknownFNAM, 4);
  }//if FNAM

  for (i=0; i<conditions.size(); ++i)
  {
    //write CTDA
    if (!conditions[i].saveToStream(output))
    {
      std::cout << "Error while writing subrecord CTDA of SNDR!\n";
      return false;
    }
  }//for

  if (hasLNAM)
  {
    //write LNAM
    output.write((const char*) &cLNAM, 4);
    //LNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write LNAM's stuff
    output.write((const char*) &unknownLNAM, 4);
  }//if LNAM

  //write BNAM
  output.write((const char*) &cBNAM, 4);
  //BNAM's length
  subLength = 6; //fixed
  output.write((const char*) &subLength, 2);
  //write BNAM's stuff
  output.write((const char*) &frequencyShiftPercentage, 1);
  output.write((const char*) &frequencyVariancePercentage, 1);
  output.write((const char*) &priority, 1);
  output.write((const char*) &dB_Variance, 1);
  output.write((const char*) &staticAttenuation_x100, 2);

  return output.good();
}
#endif

bool SoundDescriptorRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  in_File.read((char*) &subRecName, 4);
  bytesRead += 4;
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
    std::cout <<"Error: sub record EDID of SNDR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of SNDR!\n";
    return false;
  }
  editorID = std::string(buffer);

  //read CNAM
  if (!loadUint32SubRecordFromStream(in_File, cCNAM, unknownCNAM, true)) return false;
  bytesRead += 10;

  categoryFormID = 0;
  alternateFormID = 0;
  waveFiles.clear();
  outputModelFormID = 0;
  hasFNAM = false; unknownFNAM = 0;
  conditions.clear();
  CTDAData tempCTDA;
  hasLNAM = false; unknownLNAM = 0;
  bool hasReadBNAM = false;

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cGNAM:
           if (categoryFormID!=0)
           {
             std::cout << "Error: record SNDR seems to have more than one GNAM subrecord!\n";
             return false;
           }
           //read GNAM
           if (!loadUint32SubRecordFromStream(in_File, cGNAM, categoryFormID, false)) return false;
           bytesRead += 6;
           //sanity check
           if (categoryFormID==0)
           {
             std::cout << "Error: subrecord GNAM of SNDR has invalid value zero!\n";
             return false;
           }
           break;
      case cSNAM:
           if (alternateFormID!=0)
           {
             std::cout << "Error: record SNDR seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, alternateFormID, false)) return false;
           bytesRead += 6;
           //sanity check
           if (alternateFormID==0)
           {
             std::cout << "Error: subrecord SNAM of SNDR has invalid value zero!\n";
             return false;
           }
           break;
      case cANAM:
           waveFiles.push_back("");
           //read MODL
           if (!loadString512FromStream(in_File, waveFiles.back(), buffer, cANAM, false, bytesRead)) return false;
           //sanity check
           if (waveFiles.back().empty())
           {
             std::cout << "Error: subrecord ANAM of SNDR is empty!\n";
             return false;
           }
           break;
      case cONAM:
           if (outputModelFormID!=0)
           {
             std::cout << "Error: record SNDR seems to have more than one ONAM subrecord!\n";
             return false;
           }
           //read ONAM
           if (!loadUint32SubRecordFromStream(in_File, cONAM, outputModelFormID, false)) return false;
           bytesRead += 6;
           //sanity check
           if (outputModelFormID==0)
           {
             std::cout << "Error: subrecord ONAM of SNDR has invalid value zero!\n";
             return false;
           }
           break;
      case cFNAM:
           if (hasFNAM)
           {
             std::cout << "Error: record SNDR seems to have more than one FNAM subrecord!\n";
             return false;
           }
           //read FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, unknownFNAM, false)) return false;
           bytesRead += 6;
           hasFNAM = true;
           break;
      case cCTDA:
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cout << "Error while reading subrecord CTDA of SNDR!\n";
             return false;
           }
           conditions.push_back(tempCTDA);
           break;
      case cLNAM:
           if (hasLNAM)
           {
             std::cout << "Error: record SNDR seems to have more than one LNAM subrecord!\n";
             return false;
           }
           //read LNAM
           if (!loadUint32SubRecordFromStream(in_File, cLNAM, unknownLNAM, false)) return false;
           bytesRead += 6;
           hasLNAM = true;
           break;
      case cBNAM:
           if (hasReadBNAM)
           {
             std::cout << "Error: record SNDR seems to have more than one BNAM subrecord!\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=6)
           {
             std::cout <<"Error: sub record BNAM of SNDR has invalid length ("
                       <<subLength << " bytes). Should be six bytes!\n";
             return false;
           }
           //read BNAM's stuff
           in_File.read((char*) &frequencyShiftPercentage, 1);
           in_File.read((char*) &frequencyVariancePercentage, 1);
           in_File.read((char*) &priority, 1);
           in_File.read((char*) &dB_Variance, 1);
           in_File.read((char*) &staticAttenuation_x100, 2);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of SNDR!\n";
            return false;
           }
           bytesRead += 6;
           hasReadBNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only GNAM, SNAM, ANAM, ONAM, FNAM, CTDA, LNAM or BNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!((outputModelFormID!=0) and (!waveFiles.empty()) and hasReadBNAM))
  {
    std::cout << "Error: At least one required subrecord of SNDR is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t SoundDescriptorRecord::getRecordType() const
{
  return cSNDR;
}

float SoundDescriptorRecord::getStaticAttenuation() const
{
  return (staticAttenuation_x100 * 0.01f);
}

} //namespace
