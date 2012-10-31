/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#include "AddOnNodeRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

//flag constants
const uint16_t AddOnNodeRecord::cFlagUnknown      = 0x01;
const uint16_t AddOnNodeRecord::cFlagAlwaysLoaded = 0x02;

AddOnNodeRecord::AddOnNodeRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownDATA = 0;
  soundDescriptorFormID = 0;
  MasterParticleSystemCap = 0;
  flags = 1; //yes, one
}

AddOnNodeRecord::~AddOnNodeRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool AddOnNodeRecord::equals(const AddOnNodeRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownDATA==other.unknownDATA) and (soundDescriptorFormID==other.soundDescriptorFormID)
      and (MasterParticleSystemCap==other.MasterParticleSystemCap)
      and (flags==other.flags));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AddOnNodeRecord::getWriteSize() const
{
  if (isDeleted()) return 0;
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }
  if (soundDescriptorFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* fixed size of 4 bytes */;
  }
  return writeSize;
}

bool AddOnNodeRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cADDN, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;
  if (isDeleted()) return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of ADDN!\n";
      return false;
    }
  }

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) &unknownDATA, 4);

  if (soundDescriptorFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write SNAM
    output.write((const char*) &soundDescriptorFormID, 4);
  }//if

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DNAM's stuff
  output.write((const char*) &MasterParticleSystemCap, 2);
  output.write((const char*) &flags, 2);

  return output.good();
}
#endif

bool AddOnNodeRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of ADDN is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ADDN!\n";
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
    std::cout <<"Error: sub record OBND of ADDN has invalid length ("<<subLength
              << " bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  memset(unknownOBND, 0, 12);
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of ADDN!\n";
    return false;
  }

  //read MODL
  if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, true, bytesRead)) return false;

  unknownMODT.setPresence(false);
  bool hasReadDATA = false;
  soundDescriptorFormID = 0;
  bool hasReadDNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: record ADDN seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of ADDN!";
             return false;
           }
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: record ADDN seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA
           if (!loadUint32SubRecordFromStream(in_File, cDATA, unknownDATA, false))
             return false;
           bytesRead += 6;
           hasReadDATA = true;
           break;
      case cSNAM:
           if (soundDescriptorFormID!=0)
           {
             std::cout << "Error: record ADDN seems to have more than one SNAM subrecord!\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, soundDescriptorFormID, false))
             return false;
           bytesRead += 6;
           if (soundDescriptorFormID==0)
           {
             std::cout << "Error: subrecord SNAM of ADDN has value zero!\n";
             return false;
           }
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: record ADDN seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record DNAM of ADDN has invalid length ("<<subLength
                       << " bytes). Should be two bytes!\n";
             return false;
           }
           //read DNAM
           in_File.read((char*) &MasterParticleSystemCap, 2);
           in_File.read((char*) &flags, 2);
           bytesRead += 4;
           hasReadDNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only MODT, DATA, SNAM or DNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadDATA and hasReadDNAM))
  {
    std::cout << "Error: At least one required subrecord of ADDN is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t AddOnNodeRecord::getRecordType() const
{
  return cADDN;
}

bool AddOnNodeRecord::alwaysLoaded() const
{
  return ((flags & cFlagAlwaysLoaded)!=0);
}

} //namespace
