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

#include "ClimateRecord.h"
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include <iostream>

namespace SRTP
{

ClimateRecord::ClimateRecord()
: BasicRecord()
{
  editorID = "";
  //subrecord WLST
  unknownFormID = 0;
  unknownWLSTTwo = 0;
  unknownWLSTThree = 0;
  //end of subrecord WLST
  unknownFNAM = "";
  unknownGNAM = "";
  modelPath = "";
  unknownMODT.setPresence(false);
  unknownTNAM.setPresence(false);
}

ClimateRecord::~ClimateRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool ClimateRecord::equals(const ClimateRecord& other) const
{
  return (equalsBasic(other) and (editorID==other.editorID)
      and (unknownFormID==other.unknownFormID) and (unknownWLSTTwo==other.unknownWLSTTwo)
      and (unknownWLSTThree==other.unknownWLSTThree) and (unknownFNAM==other.unknownFNAM)
      and (unknownGNAM==other.unknownGNAM) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (unknownTNAM==other.unknownTNAM));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t ClimateRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* WLST */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* FNAM */ +2 /* 2 bytes for length */
        +unknownFNAM.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* GNAM */ +2 /* 2 bytes for length */
        +unknownGNAM.length()+1 /* length of path +1 byte for NUL termination */
        +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize +4 /* MODT */ +2 /* 2 bytes for length */ +unknownMODT.getSize() /* size */;
  }
  if (unknownTNAM.isPresent())
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +unknownTNAM.getSize() /* fixed size of 6 bytes, usually */;
  }
  return writeSize;
}

bool ClimateRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cCLMT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write WLST
  output.write((const char*) &cWLST, 4);
  //WLST's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write WLST's stuff
  output.write((const char*) &unknownFormID, 4);
  output.write((const char*) &unknownWLSTTwo, 4);
  output.write((const char*) &unknownWLSTThree, 4);

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = unknownFNAM.length()+1;
  output.write((const char*) &subLength, 2);
  //write FNAM
  output.write(unknownFNAM.c_str(), subLength);

  //write GNAM
  output.write((const char*) &cGNAM, 4);
  //GNAM's length
  subLength = unknownGNAM.length()+1;
  output.write((const char*) &subLength, 2);
  //write GNAM
  output.write(unknownGNAM.c_str(), subLength);

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
      std::cout << "Error while writing subrecord MODT of CLMT!\n";
      return false;
    }
  }//if MODT

  if (unknownTNAM.isPresent())
  {
    //write TNAM
    if (!unknownTNAM.saveToStream(output, cTNAM))
    {
      std::cout << "Error while writing subrecord TNAM of CLMT!\n";
      return false;
    }
  }//if TNAM

  return output.good();
}
#endif

bool ClimateRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of CLMT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of CLMT!\n";
    return false;
  }
  editorID = std::string(buffer);

  bool hasReadWLST = false;
  unknownFNAM.clear();
  unknownGNAM.clear();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownTNAM.setPresence(false);

  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cWLST:
           if (hasReadWLST)
           {
             std::cout << "Error: record CLMT seems to have more than one WLST subrecord!\n";
             return false;
           }
           //WLST's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record WLST of CONT has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read WLST
           in_File.read((char*) &unknownFormID, 4);
           in_File.read((char*) &unknownWLSTTwo, 4);
           in_File.read((char*) &unknownWLSTThree, 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord WLST of CLMT!\n";
             return false;
           }
           hasReadWLST = true;
           break;
      case cFNAM:
           if (!unknownFNAM.empty())
           {
             std::cout << "Error: CLMT seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //read FNAM
           if (!loadString512FromStream(in_File, unknownFNAM, buffer, cFNAM, false, bytesRead)) return false;
           //length check
           if (unknownFNAM.empty())
           {
             std::cout << "Error: subrecord FNAM of CLMT is empty.\n";
             return false;
           }
           break;
      case cGNAM:
           if (!unknownGNAM.empty())
           {
             std::cout << "Error: CLMT seems to have more than one GNAM subrecord.\n";
             return false;
           }
           //read GNAM
           if (!loadString512FromStream(in_File, unknownGNAM, buffer, cGNAM, false, bytesRead)) return false;
           //length check
           if (unknownGNAM.empty())
           {
             std::cout << "Error: subrecord GNAM of CLMT is empty.\n";
             return false;
           }
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cout << "Error: CLMT seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead)) return false;
           //length check
           if (modelPath.empty())
           {
             std::cout << "Error: subrecord MODL of CLMT is empty.\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cout << "Error: CLMT seems to have more than one MODT subrecord.\n";
             return false;
           }
           //read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false)) return false;
           bytesRead += (2+unknownMODT.getSize());
           break;
      case cTNAM:
           if (unknownTNAM.isPresent())
           {
             std::cout << "Error: CLMT seems to have more than one TNAM subrecord.\n";
             return false;
           }
           //read TNAM
           if (!unknownTNAM.loadFromStream(in_File, cTNAM, false)) return false;
           bytesRead += (2+unknownTNAM.getSize());
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only WLST, FNAM, GNAM, MODL, MODT, MODS"
                     << " or TNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadWLST and !unknownFNAM.empty() and !unknownGNAM.empty()
      and !modelPath.empty() and unknownTNAM.isPresent()))
  {
    std::cout << "Error: at least one required subrecord of CLMT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t ClimateRecord::getRecordType() const
{
  return cCLMT;
}

} //namespace
