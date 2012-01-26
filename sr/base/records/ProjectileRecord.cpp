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

#include "ProjectileRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* equality operator for DSTD_DSTF_record */
bool ProjectileRecord::DSTD_DSTF_record::operator==(const ProjectileRecord::DSTD_DSTF_record& other) const
{
  return (memcmp(unknownDSTD, other.unknownDSTD, 20)==0);
}

/* ProjectileRecord's functions */

ProjectileRecord::ProjectileRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  modelPath = "";
  unknownMODT.setPresence(false);
  hasDEST = false;
  unknownDEST = 0;
  unknownDSTD_DSTFs.clear();;
  unknownDATA.setPresence(false);
  unknownNAM1 = "";
  unknownNAM2.setPresence(false);
  unknownVNAM = 0;
}

ProjectileRecord::~ProjectileRecord()
{
  //empty
}

bool ProjectileRecord::equals(const ProjectileRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (hasDEST==other.hasDEST) and ((unknownDEST==other.unknownDEST) or (!hasDEST))
      and (unknownDSTD_DSTFs==other.unknownDSTD_DSTFs) and (unknownDATA==other.unknownDATA)
      and (unknownNAM1==other.unknownNAM1) and (unknownNAM2==other.unknownNAM2)
      and (unknownVNAM==other.unknownVNAM));
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ProjectileRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of string +1 byte for NUL-termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +unknownDATA.getSize() /* size */
        +4 /* NAM1 */ +2 /* 2 bytes for length */
        +unknownNAM1.length()+1 /* length of string +1 byte for NUL-termination */
        +4 /* VNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of string +1 byte for NUL-termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ +2 /* 2 bytes for length */
               +unknownMODT.getSize();
  }
  if (hasDEST)
  {
    writeSize = writeSize +4 /* DEST */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (!unknownDSTD_DSTFs.empty())
  {
    writeSize = writeSize +unknownDSTD_DSTFs.size()*(4 /* DSTD */ +2 /* 2 bytes for length */ +20 /* fixed size */
               +4 /* DSTF */ +2 /* 2 bytes for length */ +0 /* fixed size */);
  }
  if (unknownNAM2.isPresent())
  {
    writeSize = writeSize + 4 /* NAM2 */ +2 /* 2 bytes for length */
               +unknownNAM2.getSize();
  }
  return writeSize;
}

bool ProjectileRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cPROJ, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

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
  subLength = 12; //fixed size
  output.write((const char*) &subLength, 2);
  //write OBND's stuff
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write FULL's stuff
    output.write((const char*) &nameStringID, 4);
  }//if hasFULL

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if model path

  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of PROJ!\n";
      return false;
    }
  }//if MODT

  if (hasDEST)
  {
    //write DEST
    output.write((const char*) &cDEST, 4);
    //DEST's length
    subLength = 8; //fixed size
    output.write((const char*) &subLength, 2);
    //write DEST's stuff
    output.write((const char*) &unknownDEST, 4);
  }//if hasDEST

  if (!unknownDSTD_DSTFs.empty())
  {
    const uint32_t count = unknownDSTD_DSTFs.size();
    unsigned int i;
    for (i=0; i<count; ++i)
    {
      //write DSTD
      output.write((const char*) &cDSTD, 4);
      //DSTD's length
      subLength = 20; //fixed size
      output.write((const char*) &subLength, 2);
      //write DSTD's stuff
      output.write((const char*) (unknownDSTD_DSTFs[i].unknownDSTD), 20);

      //write DSTF
      output.write((const char*) &cDSTF, 4);
      //DSTF's length
      subLength = 0;
      output.write((const char*) &subLength, 2);
      //zero length, nothing to write
    }//for
  }//if DSTD_DSTF

  //write DATA
  if (unknownDATA.isPresent())
  {
    if (!unknownDATA.saveToStream(output, cDATA))
    {
      std::cout << "Error while writing subrecord DATA of PROJ!\n";
      return false;
    }
  }//if DATA
  else
  {
    std::cout << "Error while writing subrecord DATA of PROJ: no DATA subrecord present!\n";
    return false;
  }

  //write NAM1
  output.write((const char*) &cNAM1, 4);
  //NAM1's length
  subLength = unknownNAM1.length()+1;
  output.write((const char*) &subLength, 2);
  //write NAM1 path
  output.write(unknownNAM1.c_str(), subLength);

  if (unknownNAM2.isPresent())
  {
    if (!unknownNAM2.saveToStream(output, cNAM2))
    {
      std::cout << "Error while writing subrecord NAM2 of PROJ!\n";
      return false;
    }
  }//if NAM2

  //write VNAM
  output.write((const char*) &cVNAM, 4);
  //VNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write VNAM's stuff
  output.write((const char*) &unknownVNAM, 4);

  return output.good();
}
#endif

bool ProjectileRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of PROJ is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord PROJ of AMMO!\n";
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
    std::cout <<"Error: sub record OBND of PROJ has invalid length("<<subLength
              <<" bytes). Should be 12 bytes!\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of PROJ!\n";
    return false;
  }

  hasFULL = false; nameStringID = 0;
  modelPath.clear();
  unknownMODT.setPresence(false);
  hasDEST = false; unknownDEST = 0;
  unknownDSTD_DSTFs.clear();
  DSTD_DSTF_record tempDSTD_DSTF;
  bool hasReadNAM1 = false;
  unknownNAM1.clear();
  unknownNAM2.setPresence(false);
  bool hasReadVNAM = false; unknownVNAM = 0;

  while (bytesRead<readSize)
  {
    //read next subrecord name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: Record PROJ seems to have more than one FULL subrecord!\n";
             return false;
           }
           //FULL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record FULL of PROJ has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read FULL's stuff
           in_File.read((char*) &nameStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FULL of PROJ!\n";
             return false;
           }
           hasFULL = true;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: Record PROJ seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of PROJ is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of PROJ!\n";
             return false;
           }
           modelPath = std::string(buffer);
           if (modelPath.empty())
           {
             std::cout << "Error: MODL subrecord of PROJ is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: Record PROJ seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of PROJ!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cDEST:
           if (hasDEST)
           {
             std::cout << "Error: Record PROJ seems to have more than one DEST subrecord!\n";
             return false;
           }
           //DEST's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DEST of PROJ has invalid length("
                       <<subLength <<" bytes). Should be eight bytes!\n";
             return false;
           }
           //read DEST's stuff
           in_File.read((char*) &unknownDEST, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DEST of PROJ!\n";
             return false;
           }
           hasDEST = true;
           break;
      case cDSTD:
           if (!hasDEST)
           {
             std::cout << "Error: Record PROJ seems to have a DSTF subrecord but no DEST subrecord!\n";
             return false;
           }
           //DSTD's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=20)
           {
             std::cout <<"Error: sub record DSTD of PROJ has invalid length("
                       <<subLength <<" bytes). Should be 20 bytes!\n";
             return false;
           }
           //read DSTD's stuff
           in_File.read((char*) tempDSTD_DSTF.unknownDSTD, 20);
           bytesRead += 20;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DSTD of PROJ!\n";
             return false;
           }

           //read DSTF
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cDSTF)
           {
             UnexpectedRecord(cDSTF, subRecName);
             return false;
           }
           //DSTF's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout <<"Error: sub record DSTF of PROJ has invalid length("
                       <<subLength <<" bytes). Should be zero bytes!\n";
             return false;
           }
           unknownDSTD_DSTFs.push_back(tempDSTD_DSTF);
           break;
      case cDATA:
           if (unknownDATA.isPresent())
           {
             std::cout << "Error: Record PROJ seems to have more than one DATA subrecord!\n";
             return false;
           }
           //read DATA
           if (!unknownDATA.loadFromStream(in_File, cDATA, false))
           {
             std::cout << "Error while reading subrecord DATA of PROJ!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownDATA.getSize();
           //check DATA's length
           subLength = unknownDATA.getSize();
           if ((subLength!=92) and (subLength!=88) and (subLength!=84))
           {
             std::cout <<"Error: sub record DATA of PROJ has invalid length("
                       <<subLength <<" bytes). Should be 92 or 88 or 84 bytes!\n";
             return false;
           }
           break;
      case cNAM1:
           if (hasReadNAM1)
           {
             std::cout << "Error: Record PROJ seems to have more than one NAM1 subrecord!\n";
             return false;
           }
           //NAM1's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record NAM1 of PROJ is longer than 511 characters!\n";
             return false;
           }
           //read NAM1's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord NAM1 of PROJ!\n";
             return false;
           }
           unknownNAM1 = std::string(buffer);
           hasReadNAM1 = true;
           break;
      case cNAM2:
           if (!hasReadNAM1)
           {
             std::cout << "Error: Record PROJ seems to have a NAM2 subrecord but no NAM1 subrecord!\n";
             return false;
           }
           if (unknownNAM2.isPresent())
           {
             std::cout << "Error: Record PROJ seems to have more than one NAM2 subrecord!\n";
             return false;
           }
           //read NAM2
           if (!unknownNAM2.loadFromStream(in_File, cNAM2, false))
           {
             std::cout << "Error while reading subrecord NAM2 of PROJ!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownNAM2.getSize();
           break;
      case cVNAM:
           if (hasReadVNAM)
           {
             std::cout << "Error: Record PROJ seems to have more than one VNAM subrecord!\n";
             return false;
           }
           //VNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record VNAM of PROJ has invalid length("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read VNAM's stuff
           in_File.read((char*) &unknownVNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord VNAM of PROJ!\n";
             return false;
           }
           hasReadVNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only FULL, MODL, MODT, DEST, DSTD, DATA, NAM1, NAM2 or VNAM are allowed!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadNAM1 and unknownDATA.isPresent() and hasReadVNAM))
  {
    std::cout << "Error: At least one of the required subrecords of PROJ is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t ProjectileRecord::getRecordType() const
{
  return cPROJ;
}

    std::string editorID;
    uint8_t unknownOBND[12];
    std::string modelPath;
    BinarySubRecord unknownMODT;
    bool hasDEST;
    uint64_t unknownDEST;
    std::vector<ProjectileRecord::DSTD_DSTF_record> unknownDSTD_DSTFs;
    uint32_t unknownDATA[92];//subrecord DATA
    std::string unknownNAM1;
    BinarySubRecord unknownNAM2;
    uint32_t unknownVNAM;

} //namespace
