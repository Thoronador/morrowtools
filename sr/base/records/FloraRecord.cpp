/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Thoronador

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

#include "FloraRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../../mw/base/HelperIO.hpp"

namespace SRTP
{

FloraRecord::FloraRecord()
: BasicRecord(), editorID(""),
  unknownVMAD(BinarySubRecord()),
  name(LocalizedString()),
  modelPath(""),
  unknownMODT(BinarySubRecord()),
  unknownMODS(BinarySubRecord()),
  unknownPNAM(0),
  activateTextOverride(LocalizedString()),
  unknownFNAM(0),
  ingredientFormID(0),
  harvestSoundFormID(0),
  unknownPFPC(0)
{
  memset(unknownOBND, 0, 12);
}

FloraRecord::~FloraRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool FloraRecord::equals(const FloraRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (name==other.name) and (modelPath==other.modelPath)
      and (unknownMODT==other.unknownMODT) and (unknownMODS==other.unknownMODS)
      and (unknownPNAM==other.unknownPNAM)
      and (activateTextOverride==other.activateTextOverride)
      and (unknownFNAM==other.unknownFNAM)
      and (ingredientFormID==other.ingredientFormID)
      and (harvestSoundFormID==other.harvestSoundFormID)
      and (unknownPFPC==other.unknownPFPC));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FloraRecord::getWriteSize() const
{
  if (isDeleted())
    return 0;
  uint32_t writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +name.getWriteSize() /* FULL */
        +4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* FNAM */ +2 /* 2 bytes for length */ +2 /* fixed size */
        +4 /* PFPC */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* size */;
  }
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MODL */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMODT.isPresent())
  {
    writeSize = writeSize + 4 /* MODT */ + 2 /* 2 bytes for length */ + unknownMODT.size() /* size */;
  }
  if (unknownMODS.isPresent())
  {
    writeSize = writeSize + 4 /* MODS */ + 2 /* 2 bytes for length */ + unknownMODS.size() /* size */;
  }
  if (activateTextOverride.isPresent())
  {
    writeSize += activateTextOverride.getWriteSize();
  }
  if (ingredientFormID!=0)
  {
    writeSize = writeSize +4 /* PFIG */ +2 /* 2 bytes for length */ +4 /* size */;
  }
  if (harvestSoundFormID!=0)
  {
    writeSize = writeSize +4 /* SNAM */ +2 /* 2 bytes for length */ +4 /* size */;
  }
  return writeSize;
}

bool FloraRecord::saveToStream(std::ostream& output) const
{
  output.write((const char*) &cFLOR, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;
  if (isDeleted())
    return true;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    //write VMAD
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing subrecord VMAD of FLOR!\n";
      return false;
    }
  }

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed size
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  //write FULL
  if (!name.saveToStream(output, cFULL))
    return false;

  if (!modelPath.empty())
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path
    output.write(modelPath.c_str(), subLength);
  }

  if (unknownMODT.isPresent())
  {
    //write MODT
    if (!unknownMODT.saveToStream(output, cMODT))
    {
      std::cerr << "Error while writing subrecord MODT of FLOR!\n";
      return false;
    }
  }

  if (unknownMODS.isPresent())
  {
    //write MODS
    if (!unknownMODS.saveToStream(output, cMODS))
    {
      std::cerr << "Error while writing subrecord MODS of FLOR!\n";
      return false;
    }
  }

  //write PNAM
  output.write((const char*) &cPNAM, 4);
  //PNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write PNAM
  output.write((const char*) &unknownPNAM, 4);

  if (activateTextOverride.isPresent())
  {
    //write RNAM
    if (!activateTextOverride.saveToStream(output, cRNAM))
      return false;
  }//if has RNAM

  //write FNAM
  output.write((const char*) &cFNAM, 4);
  //FNAM's length
  subLength = 2; //fixed size
  output.write((const char*) &subLength, 2);
  //write FNAM
  output.write((const char*) &unknownFNAM, 2);

  if (ingredientFormID!=0)
  {
    //write PFIG
    output.write((const char*) &cPFIG, 4);
    //PFIG's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Ingredient form ID
    output.write((const char*) &ingredientFormID, 4);
  }//if has PFIG

  if (harvestSoundFormID!=0)
  {
    //write SNAM
    output.write((const char*) &cSNAM, 4);
    //SNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write Harvest Sound's form ID
    output.write((const char*) &harvestSoundFormID, 4);
  }//if has SNAM

  //write PFPC
  output.write((const char*) &cPFPC, 4);
  //PFPC's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write PFPC
  output.write((const char*) &unknownPFPC, 4);

  return output.good();
}
#endif

bool FloraRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  if (isDeleted())
    return true;
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
    std::cerr <<"Error: sub record EDID of FLOR is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cerr << "Error while reading subrecord EDID of FLOR!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  name.reset();
  modelPath.clear();
  unknownMODT.setPresence(false);
  unknownMODS.setPresence(false);
  bool hasReadPNAM = false; unknownPNAM = 0;
  activateTextOverride.reset();
  bool hasReadFNAM = false; unknownFNAM = 0;
  ingredientFormID = 0;
  harvestSoundFormID = 0;
  bool hasReadPFPC = false; unknownPFPC = 0;
  while (bytesRead<readSize)
  {
    //read next header
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one VMAD subrecord.\n";
             return false;
           }
           // read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cerr << "Error while reading subrecord VMAD of FLOR!\n";
             return false;
           }
           bytesRead = bytesRead + 2 + unknownVMAD.size();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: FLOR seems to have more than one OBND subrecord.\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cerr <<"Error: sub record OBND of FLOR has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read OBND
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord OBND of FLOR!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cMODL:
           if (!modelPath.empty())
           {
             std::cerr << "Error: FLOR seems to have more than one MODL subrecord.\n";
             return false;
           }
           //read MODL
           if (!loadString512FromStream(in_File, modelPath, buffer, cMODL, false, bytesRead))
             return false;
           if (modelPath.empty())
           {
             std::cerr << "Error: subrecord MODL of FLOR is empty!\n";
             return false;
           }
           break;
      case cMODT:
           if (unknownMODT.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one MODT subrecord.\n";
             return false;
           }
           // read MODT
           if (!unknownMODT.loadFromStream(in_File, cMODT, false))
           {
             std::cerr << "Error while reading subrecord MODT of FLOR!\n";
             return false;
           }
           bytesRead += (2 + unknownMODT.size());
           break;
      case cMODS:
           if (unknownMODS.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one MODS subrecord.\n";
             return false;
           }
           // read MODS
           if (!unknownMODS.loadFromStream(in_File, cMODS, false))
           {
             std::cerr << "Error while reading subrecord MODS of FLOR!\n";
             return false;
           }
           bytesRead += (2 + unknownMODS.size());
           break;
      case cPNAM:
           if (hasReadPNAM)
           {
             std::cerr << "Error: FLOR seems to have more than one PNAM subrecord.\n";
             return false;
           }
           //read PNAM
           if (!loadUint32SubRecordFromStream(in_File, cPNAM, unknownPNAM, false)) return false;
           bytesRead += 6;
           hasReadPNAM = true;
           break;
      case cRNAM:
           if (activateTextOverride.isPresent())
           {
             std::cerr << "Error: FLOR seems to have more than one RNAM subrecord.\n";
             return false;
           }
           //read RNAM
           if (!activateTextOverride.loadFromStream(in_File, cRNAM, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: FLOR seems to have more than one FNAM subrecord.\n";
             return false;
           }
           //FNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cerr <<"Error: sub record FNAM of FLOR has invalid length ("
                       <<subLength<<" bytes). Should be two bytes.\n";
             return false;
           }
           //read FNAM
           in_File.read((char*) &unknownFNAM, 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord FNAM of FLOR!\n";
             return false;
           }
           hasReadFNAM = true;
           break;
      case cPFIG:
           if (ingredientFormID!=0)
           {
             std::cerr << "Error: FLOR seems to have more than one PFIG subrecord.\n";
             return false;
           }
           //read PFIG
           if (!loadUint32SubRecordFromStream(in_File, cPFIG, ingredientFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (ingredientFormID==0)
           {
             std::cerr << "Error: subrecord PFIG of FLOR is zero!\n";
             return false;
           }
           break;
      case cSNAM:
           if (harvestSoundFormID!=0)
           {
             std::cerr << "Error: FLOR seems to have more than one SNAM subrecord.\n";
             return false;
           }
           //read SNAM
           if (!loadUint32SubRecordFromStream(in_File, cSNAM, harvestSoundFormID, false)) return false;
           bytesRead += 6;
           //check content
           if (harvestSoundFormID==0)
           {
             std::cerr << "Error: subrecord SNAM of FLOR is zero!\n";
             return false;
           }
           break;
      case cPFPC:
           if (hasReadPFPC)
           {
             std::cerr << "Error: FLOR seems to have more than one PFPC subrecord.\n";
             return false;
           }
           //read PFPC
           if (!loadUint32SubRecordFromStream(in_File, cPFPC, unknownPFPC, false)) return false;
           bytesRead += 6;
           hasReadPFPC = true;
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, MODL, MODT, MODS, PNAM, RNAM,"
                     << " FNAM, PFIG, SNAM or PFPC are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadOBND and name.isPresent()
      and hasReadPNAM and hasReadFNAM and hasReadPFPC))
  {
    std::cerr << "Error: at least one required subrecord of FLOR is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t FloraRecord::getRecordType() const
{
  return cFLOR;
}

} //namespace
