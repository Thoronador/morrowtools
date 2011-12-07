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

#include "StaticRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>

namespace SRTP
{

StaticRecord::StaticRecord()
: BasicRecord()
{
  editorID = "";
  memset(unknownOBND, 0, 12);
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  memset(unknownDNAM, 0, 8);
  unknownMNAM.setPresence(false);
}

StaticRecord::~StaticRecord()
{
  //empty
}

bool StaticRecord::equals(const StaticRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (unknownMODS==other.unknownMODS)
      and (memcmp(unknownDNAM, other.unknownDNAM, 8)==0)
      and (unknownMNAM==other.unknownMNAM));
}

bool StaticRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cSTAT, 4);
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
               +modelPath.length()+1 /* length of name +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */
                          +unknownMODT.getSize();
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize +4 /* MODS */ +2 /* 2 bytes for length */
                          +unknownMODS.getSize();
  }
  if (unknownMNAM.isPresent())
  {
    writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */
                          +unknownMNAM.getSize();
  }
  if (!saveSizeAndUnknownValues(output, writeSize)) return false;

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
  subLength = 12; //fixed
  output.write((char*) &subLength, 2);
  //write OBND's data
  output.write((const char*) unknownOBND, 12);

  if (!modelPath.empty())
  {
    //write MODL
    output.write((char*) &cMODL, 4);
    //path's length
    subLength = modelPath.length()+1;
    output.write((char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }//if MODL

  //write MODT
  if (unknownMODT.isPresent())
  {
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cout << "Error while writing subrecord MODT of STAT!\n";
      return false;
    }
  }//if MODT

  //write MODS
  if (unknownMODS.isPresent())
  {
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cout << "Error while writing subrecord MODS of STAT!\n";
      return false;
    }
  }//if MODS

  //write DNAM
  output.write((char*) &cDNAM, 4);
  //DNAM's length
  subLength = 8; //fixed
  output.write((char*) &subLength, 2);
  //write DNAM's data
  output.write((const char*) unknownDNAM, 8);

  //write MNAM
  if (unknownMNAM.isPresent())
  {
    if (!unknownMNAM.saveToStream(output, cMNAM))
    {
      std::cout << "Error while writing subrecord MNAM of STAT!\n";
      return false;
    }
  }//if MNAM

  return output.good();
}

bool StaticRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of STAT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of STAT!\n";
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
    std::cout <<"Error: sub record OBND of STAT has invalid length ("<<subLength
              <<" bytes)! Should be 12 bytes.\n";
    return false;
  }
  //read OBND's stuff
  in_File.read((char*) unknownOBND, 12);
  bytesRead += 12;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord OBND of STAT!\n";
    return false;
  }

  modelPath.clear();
  unknownMODS.setPresence(false);
  unknownMODT.setPresence(false);
  bool hasReadDNAM = false;
  unknownMNAM.setPresence(false);
  while (bytesRead<readSize)
  {
    //read next subrecord name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: STAT seems to have more than one MODL subrecord!\n";
             return false;
           }
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MODL of STAT is longer than 511 characters!\n";
             return false;
           }
           //read MODL's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of STAT!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: STAT seems to have more than one MODT subrecord!\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cout << "Error while reading subrecord MODT of STAT!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODT.getSize();
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cout << "Error: STAT seems to have more than one MODS subrecord!\n";
             return false;
           }
           //read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cout << "Error while reading subrecord MODS of STAT!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMODS.getSize();
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: STAT seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DNAM of STAT has invalid length ("
                       <<subLength<<" bytes)! Should be 8 bytes.\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) unknownDNAM, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of STAT!\n";
             return false;
           }
           hasReadDNAM = true;
           break;
      case cMNAM:
           if (unknownMNAM.isPresent())
           {
             std::cout << "Error: STAT seems to have more than one MNAM subrecord!\n";
             return false;
           }
           //read MNAM
           if (!unknownMNAM.loadFromStream(in_File, cMNAM, false))
           {
             std::cout << "Error while reading subrecord MNAM of STAT!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMNAM.getSize();
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only MODT, DNAM or MNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //check presence of DNAM (required subrecord)
  if (!hasReadDNAM)
  {
    std::cout << "Error: STAT seems to have no DNAM subrecord!\n";
    return false;
  }

  return in_File.good();
}

int32_t StaticRecord::getRecordType() const
{
  return cSTAT;
}

} //namespace
