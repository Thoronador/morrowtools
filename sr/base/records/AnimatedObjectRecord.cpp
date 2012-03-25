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

#include "AnimatedObjectRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

AnimatedObjectRecord::AnimatedObjectRecord()
: BasicRecord()
{
  editorID = "";
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownBNAM = "";
}

AnimatedObjectRecord::~AnimatedObjectRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool AnimatedObjectRecord::equals(const AnimatedObjectRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (modelPath==other.modelPath) and (unknownMODT==other.unknownMODT)
      and (unknownBNAM==other.unknownBNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AnimatedObjectRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of string +1 byte for NUL-termination */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of string +1 byte for NUL-termination */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize();
  }
  if (!unknownBNAM.empty())
  {
    writeSize = writeSize +4 /* BNAM */ +2 /* 2 bytes for length */
        +unknownBNAM.length()+1 /* length of string +1 byte for NUL-termination */;
  }
  return writeSize;
}

bool AnimatedObjectRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cANIO, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write MODL
  output.write((const char*) &cMODL, 4);
  //MODL's length
  subLength = modelPath.length()+1;
  output.write((const char*) &subLength, 2);
  //write model path
  output.write(modelPath.c_str(), subLength);

  //write MODT
  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of ACTI!\n";
      return false;
    }
  }//if MODT

  if (!unknownBNAM.empty())
  {
    //write BNAM
    output.write((const char*) &cBNAM, 4);
    //BNAM's length
    subLength = unknownBNAM.length()+1;
    output.write((const char*) &subLength, 2);
    //write BNAM
    output.write(unknownBNAM.c_str(), subLength);
  }//if BNAM

  return output.good();
}
#endif

bool AnimatedObjectRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of ANIO is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ANIO!\n";
    return false;
  }
  editorID = std::string(buffer);

  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownBNAM.clear();
  while (bytesRead<readSize)
  {
    //read next record
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: ANIO seems to have more than one MODL subrecord.\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of ANIO is longer than 511 characters!\n";
             return false;
           }
           //read model path
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of ANIO!\n";
             return false;
           }
           modelPath = std::string(buffer);
           if (modelPath.empty())
           {
             std::cout << "Error: ANIO seems to have empty MODL subrecord.\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: ANIO seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false)) return false;
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cBNAM:
           if (!unknownBNAM.empty())
           {
             std::cout << "Error: ANIO seems to have more than one BNAM subrecord.\n";
             return false;
           }
           //BNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record BNAM of ANIO is longer than 511 characters!\n";
             return false;
           }
           //read BNAM
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BNAM of ANIO!\n";
             return false;
           }
           unknownBNAM = std::string(buffer);
           if (unknownBNAM.empty())
           {
             std::cout << "Error: ANIO seems to have empty BNAM subrecord.\n";
             return false;
           }
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only MODL, MODT or BNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (modelPath.empty())
  {
    std::cout << "Error: At least one required subrecord of ANIO was not found!\n";
    return false;
  }

  return in_File.good();
}

uint32_t AnimatedObjectRecord::getRecordType() const
{
  return cANIO;
}

} //namespace
