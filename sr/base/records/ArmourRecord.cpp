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

#include "ArmourRecord.h"
#include <iostream>
#include <cstring>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

ArmourRecord::ArmourRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  hasFULL = false;
  nameStringID = 0;
  hasEITM = false;
  unknownEITM = 0;
  modelPath = "";
  unknownMO2T.setPresence(false);
  unknownMO2S.setPresence(false);
  mod4Path = "";
  unknownMO4T.setPresence(false);
  unknownMO4S.setPresence(false);
  unknownBODT.setPresence(false);
  hasETYP = false;
  unknownETYP = 0;
  hasBIDS = false;
  unknownBIDS = 0;
  hasBAMT = false;
  unknownBAMT = 0;
  hasYNAM = false;
  unknownYNAM = 0;
  hasZNAM = false;
  unknownZNAM = 0;
  unknownRNAM = 0;
  keywordArray.clear();
  hasDESC = false;
  descriptionStringID = 0;
  unknownMODLs.clear();
  memset(unknownDATA, 0, 8);
  unknownDNAM = 0;
  hasTNAM = false;
  unknownTNAM = 0;
}

ArmourRecord::~ArmourRecord()
{
  //empty
}

bool ArmourRecord::equals(const ArmourRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (hasEITM==other.hasEITM) and ((unknownEITM==other.unknownEITM) or (!hasEITM))
      and (modelPath==other.modelPath) and (unknownMO2T==other.unknownMO2T)
      and (unknownMO2S==other.unknownMO2S)
      and (mod4Path==other.mod4Path) and (unknownMO4T==other.unknownMO4T)
      and (unknownMO4S==other.unknownMO4S) and (unknownBODT==other.unknownBODT)
      and (hasETYP==other.hasETYP) and ((unknownETYP==other.unknownETYP) or (!hasETYP))
      and (hasBIDS==other.hasBIDS) and ((unknownBIDS==other.unknownBIDS) or (!hasBIDS))
      and (hasBAMT==other.hasBAMT) and ((unknownBAMT==other.unknownBAMT) or (!hasBAMT))
      and (hasYNAM==other.hasYNAM) and ((unknownYNAM==other.unknownYNAM) or (!hasYNAM))
      and (hasZNAM==other.hasZNAM) and ((unknownZNAM==other.unknownZNAM) or (!hasZNAM))
      and (unknownRNAM==other.unknownRNAM) and (keywordArray==other.keywordArray)
      and (hasDESC==other.hasDESC) and ((descriptionStringID==other.descriptionStringID) or (!hasDESC))
      and (unknownMODLs==other.unknownMODLs)
      and (memcmp(unknownDATA, other.unknownDATA, 8)==0)
      and (unknownDNAM==other.unknownDNAM)
      and (hasTNAM==other.hasTNAM) and ((unknownTNAM==other.unknownTNAM) or (!hasTNAM)));
}

uint32_t ArmourRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed length */
        +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */
        +4 /* DATA */ +2 /* 2 bytes for length */ +8 /* fixed length */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* length */;
  }//if VMAD
  if (hasEITM)
  {
    writeSize = writeSize +4 /* EITM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has EITM
  if (!modelPath.empty())
  {
    writeSize = writeSize +4 /* MOD2 */ +2 /* 2 bytes for length */
        +modelPath.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMO2T.isPresent())
  {
    writeSize = writeSize +4 /* MO2T */ +2 /* 2 bytes for length */ +unknownMO2T.getSize() /* length */;
  }//if MO2T
  if (unknownMO2S.isPresent())
  {
    writeSize = writeSize +4 /* MO2S */ +2 /* 2 bytes for length */ +unknownMO2S.getSize() /* length */;
  }//if MO2S
  if (!mod4Path.empty())
  {
    writeSize = writeSize +4 /* MOD4 */ +2 /* 2 bytes for length */
        +mod4Path.length()+1 /* length of path +1 byte for NUL termination */;
  }
  if (unknownMO4T.isPresent())
  {
    writeSize = writeSize +4 /* MO4T */ +2 /* 2 bytes for length */ +unknownMO4T.getSize() /* length */;
  }//if MO4T
  if (unknownMO4S.isPresent())
  {
    writeSize = writeSize +4 /* MO4S */ +2 /* 2 bytes for length */ +unknownMO4S.getSize() /* length */;
  }//if MO4S
  if (unknownBODT.isPresent())
  {
    writeSize = writeSize +4 /* BODT */ +2 /* 2 bytes for length */ +unknownBODT.getSize() /* length */;
  }//if BODT
  if (hasETYP)
  {
    writeSize = writeSize +4 /* ETYP */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ETYP
  if (hasBIDS)
  {
    writeSize = writeSize +4 /* BIDS */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has BIDS
  if (hasBAMT)
  {
    writeSize = writeSize +4 /* BAMT */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has BAMT
  if (hasYNAM)
  {
    writeSize = writeSize +4 /* YNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has YNAM
  if (hasZNAM)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has ZNAM
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed length */
                          +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /*data length*/ ;
  }//if keywords
  if (hasDESC)
  {
    writeSize = writeSize +4 /* DESC */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has DESC
  if (!unknownMODLs.empty())
  {
    writeSize = writeSize +unknownMODLs.size()*(4 /* MODL */ +2 /* 2 bytes for length */ +4 /* fixed length */);
  }//if keywords
  if (hasTNAM)
  {
    writeSize = writeSize +4 /* TNAM */ +2 /* 2 bytes for length */ +4 /* fixed length */;
  }//if has TNAM
  return writeSize;
}

bool ArmourRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cARMO, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

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
      std::cout << "Error while writing VMAD of ARMO!";
      return false;
    }
  }//if VMAD

  //write OBND
  output.write((const char*) &cOBND, 4);
  //OBND's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write OBND
  output.write((const char*) unknownOBND, 12);

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FULL
    output.write((const char*) &nameStringID, 4);
  }//if has FULL

  if (hasEITM)
  {
    //write EITM
    output.write((const char*) &cEITM, 4);
    //EITM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write EITM's data
    output.write((const char*) &unknownEITM, 4);
  }//if has EITM

  if (!modelPath.empty())
  {
    //write MOD2
    output.write((const char*) &cMOD2, 4);
    //MOD2's length
    subLength = modelPath.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path 2
    output.write(modelPath.c_str(), subLength);
  }//if model path not empty

  if (unknownMO2T.isPresent())
  {
    //write MO2T
    if (!unknownMO2T.saveToStream(output, cMO2T))
    {
      std::cout << "Error while writing MO2T of ARMO!";
      return false;
    }
  }//if MO2T

  if (unknownMO2S.isPresent())
  {
    //write MO2S
    if (!unknownMO2S.saveToStream(output, cMO2S))
    {
      std::cout << "Error while writing MO2S of ARMO!";
      return false;
    }
  }//if MO2S

  if (!mod4Path.empty())
  {
    //write MOD4
    output.write((const char*) &cMOD4, 4);
    //MOD4's length
    subLength = mod4Path.length()+1;
    output.write((const char*) &subLength, 2);
    //write model path 4
    output.write(mod4Path.c_str(), subLength);
  }//if mod4

  if (unknownMO4T.isPresent())
  {
    //write MO4T
    if (!unknownMO4T.saveToStream(output, cMO4T))
    {
      std::cout << "Error while writing MO4T of ARMO!";
      return false;
    }
  }//if MO4T

  if (unknownMO4S.isPresent())
  {
    //write MO4S
    if (!unknownMO4S.saveToStream(output, cMO4S))
    {
      std::cout << "Error while writing MO4S of ARMO!";
      return false;
    }
  }//if MO4S

  if (unknownBODT.isPresent())
  {
    //write BODT
    if (!unknownBODT.saveToStream(output, cBODT))
    {
      std::cout << "Error while writing BODT of ARMO!";
      return false;
    }
  }//if BODT

  if (hasETYP)
  {
    //write ETYP
    output.write((const char*) &cETYP, 4);
    //ETYP's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write ETYP's data
    output.write((const char*) &unknownETYP, 4);
  }//if has ETYP

  if (hasBIDS)
  {
    //write BIDS
    output.write((const char*) &cBIDS, 4);
    //BIDS's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write BIDS's data
    output.write((const char*) &unknownBIDS, 4);
  }//if has BIDS

  if (hasBAMT)
  {
    //write BAMT
    output.write((const char*) &cBAMT, 4);
    //BAMT's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write BAMT's data
    output.write((const char*) &unknownBAMT, 4);
  }//if has BAMT

  if (hasYNAM)
  {
    //write YNAM
    output.write((const char*) &cYNAM, 4);
    //YNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write YNAM's data
    output.write((const char*) &unknownYNAM, 4);
  }//if has YNAM

  if (hasZNAM)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write ZNAM's data
    output.write((const char*) &unknownZNAM, 4);
  }//if has ZNAM

  //write RNAM
  output.write((const char*) &cRNAM, 4);
  //RNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write RNAM's data
  output.write((const char*) &unknownRNAM, 4);

  uint32_t i;
  if (!keywordArray.empty())
  {
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write KSIZ's data
    const uint32_t k_Size = keywordArray.size();
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4*k_Size; //fixed
    output.write((const char*) &subLength, 2);
    //write KWDA's data
    for (i=0; i<k_Size; ++i)
    {
      output.write((const char*) &(keywordArray.at(i)), 4);
    }//for
  }//if keyword array

  if (hasDESC)
  {
    //write DESC
    output.write((const char*) &cDESC, 4);
    //DESC's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write DESC's data
    output.write((const char*) &descriptionStringID, 4);
  }//if has DESC

  for (i=0; i<unknownMODLs.size(); ++i)
  {
    //write MODL
    output.write((const char*) &cMODL, 4);
    //MODL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write MODL's data
    output.write((const char*) &(unknownMODLs[i]), 4);
  }//for MODL

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 8; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA
  output.write((const char*) unknownDATA, 8);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DNAM's data
  output.write((const char*) &unknownDNAM, 4);

  if (hasTNAM)
  {
    //write TNAM
    output.write((const char*) &cTNAM, 4);
    //TNAM's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write TNAM's data
    output.write((const char*) &unknownTNAM, 4);
  }//if has TNAM

  return output.good();
}

bool ArmourRecord::loadFromStream(std::ifstream& in_File)
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
    std::cout <<"Error: sub record EDID of ARMO is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of ARMO!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  hasFULL = false;
  hasEITM = false;
  modelPath.clear();
  unknownMO2T.setPresence(false);
  unknownMO2S.setPresence(false);
  mod4Path.clear();
  unknownMO4T.setPresence(false);
  unknownBODT.setPresence(false);
  hasETYP = false;
  hasBIDS = false;
  hasBAMT = false;
  hasYNAM = false;
  hasZNAM = false;
  bool hasReadRNAM = false;
  keywordArray.clear();
  uint32_t k_Size, i, temp;
  hasDESC = false;
  unknownMODLs.clear();
  bool hasReadDATA = false;
  bool hasReadDNAM = false;
  hasTNAM = false;
  while (bytesRead<readSize)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one VMAD subrecord!\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownVMAD.getSize();
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: ARMO seems to have more than one OBND subrecord!\n";
             return false;
           }
           //OBND's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record OBND of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read OBND's stuff
           in_File.read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord OBND of ARMO!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: record ARMO seems to have more than one FULL subrecord!\n";
             return false;
           }
           //FULL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record FULL of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read FULL's stuff
           in_File.read((char*) &nameStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FULL of ARMO!\n";
             return false;
           }
           hasFULL = true;
           break;
      case cEITM:
           if (hasEITM)
           {
             std::cout << "Error: record ARMO seems to have more than one EITM subrecord!\n";
             return false;
           }
           //EITM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record EITM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read EITM's stuff
           in_File.read((char*) &unknownEITM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord EITM of ARMO!\n";
             return false;
           }
           hasEITM = true;
           break;
      case cMOD2:
           if (!modelPath.empty())
           {
             std::cout << "Error: record ARMO seems to have more than one MOD2 subrecord!\n";
             return false;
           }
           //MOD2's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MOD2 of ARMO is longer than 511 characters!\n";
             return false;
           }
           //read MOD2's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MOD2 of ARMO!\n";
             return false;
           }
           modelPath = std::string(buffer);
           break;
      case cMO2T:
           if (unknownMO2T.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO2T subrecord!\n";
             return false;
           }
           //read MO2T
           if (!unknownMO2T.loadFromStream(in_File, cMO2T, false))
           {
             std::cout << "Error while reading subrecord MO2T of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO2T.getSize();
           break;
      case cMO2S:
           if (unknownMO2S.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO2S subrecord!\n";
             return false;
           }
           //read MO2S
           if (!unknownMO2S.loadFromStream(in_File, cMO2S, false))
           {
             std::cout << "Error while reading subrecord MO2S of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO2S.getSize();
           break;
      case cMOD4:
           if (!mod4Path.empty())
           {
             std::cout << "Error: record ARMO seems to have more than one MOD4 subrecord!\n";
             return false;
           }
           //MOD4's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record MOD4 of ARMO is longer than 511 characters!\n";
             return false;
           }
           //read MOD4's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MOD4 of ARMO!\n";
             return false;
           }
           mod4Path = std::string(buffer);
           break;
      case cMO4T:
           if (unknownMO4T.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO4T subrecord!\n";
             return false;
           }
           //read MO4T
           if (!unknownMO4T.loadFromStream(in_File, cMO4T, false))
           {
             std::cout << "Error while reading subrecord MO4T of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO4T.getSize();
           break;
      case cMO4S:
           if (unknownMO4S.isPresent())
           {
             std::cout << "Error: ARMO seems to have more than one MO4S subrecord!\n";
             return false;
           }
           //read MO4S
           if (!unknownMO4S.loadFromStream(in_File, cMO4S, false))
           {
             std::cout << "Error while reading subrecord MO4S of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownMO4S.getSize();
           break;
      case cBODT:
           if (unknownBODT.isPresent())
           {
             std::cout << "Error: record ARMO seems to have more than one BODT subrecord!\n";
             return false;
           }
           //read BODT
           if (!unknownBODT.loadFromStream(in_File, cBODT, false))
           {
             std::cout << "Error while reading subrecord BODT of ARMO!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownBODT.getSize();
           //check BODT's length
           if ((unknownBODT.getSize()!=12) and (unknownBODT.getSize()!=8))
           {
             std::cout <<"Error: sub record BODT of ARMO has invalid length ("
                       <<unknownBODT.getSize()<<" bytes). Should be 8 or 12 bytes!\n";
             return false;
           }
           break;
      case cETYP:
           if (hasETYP)
           {
             std::cout << "Error: record ARMO seems to have more than one ETYP subrecord!\n";
             return false;
           }
           //ETYP's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record ETYP of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read ETYP's stuff
           in_File.read((char*) &unknownETYP, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ETYP of ARMO!\n";
             return false;
           }
           hasETYP = true;
           break;
      case cBIDS:
           if (hasBIDS)
           {
             std::cout << "Error: record ARMO seems to have more than one BIDS subrecord!\n";
             return false;
           }
           //BIDS's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record BIDS of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read BIDS's stuff
           in_File.read((char*) &unknownBIDS, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BIDS of ARMO!\n";
             return false;
           }
           hasBIDS = true;
           break;
      case cBAMT:
           if (hasBAMT)
           {
             std::cout << "Error: record ARMO seems to have more than one BAMT subrecord!\n";
             return false;
           }
           //BAMT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record BAMT of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read BAMT's stuff
           in_File.read((char*) &unknownBAMT, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord BAMT of ARMO!\n";
             return false;
           }
           hasBAMT = true;
           break;
      case cYNAM:
           if (hasYNAM)
           {
             std::cout << "Error: ARMO seems to have more than one YNAM subrecord!\n";
             return false;
           }
           //YNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord YNAM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read YNAM's stuff
           in_File.read((char*) &unknownYNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord YNAM of ARMO!\n";
             return false;
           }//if
           hasYNAM = true;
           break;
      case cZNAM:
           if (hasZNAM)
           {
             std::cout << "Error: ARMO seems to have more than one ZNAM subrecord!\n";
             return false;
           }
           //ZNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord ZNAM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read ZNAM's stuff
           in_File.read((char*) &unknownZNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ZNAM of ARMO!\n";
             return false;
           }//if
           hasZNAM = true;
           break;
      case cRNAM:
           if (hasReadRNAM)
           {
             std::cout << "Error: ARMO seems to have more than one RNAM subrecord!\n";
             return false;
           }
           //RNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord RNAM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read RNAM's stuff
           in_File.read((char*) &unknownRNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord RNAM of ARMO!\n";
             return false;
           }//if
           hasReadRNAM = true;
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: recurd ARMO seems to have more than one KSIZ subrecord!\n";
             return false;
           }
           //KSIZ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord KSIZ of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read KSIZ's stuff
           k_Size = 0;
           in_File.read((char*) &k_Size, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord KSIZ of ARMO!\n";
             return false;
           }//if

           //read KWDA
           in_File.read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cKWDA)
           {
             UnexpectedRecord(cKWDA, subRecName);
             return false;
           }
           //KWDA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*k_Size)
           {
             std::cout <<"Error: sub record KWDA of ARMO has invalid length ("
                       <<subLength<<" bytes. Should be "<<4*k_Size<<" bytes!\n";
             return false;
           }
           //read KWDA's stuff
           for (i=0; i<k_Size; ++i)
           {
             in_File.read((char*) &temp, 4);
             bytesRead += 4;
             if (!in_File.good())
             {
               std::cout << "Error while reading subrecord KWDA of ARMO!\n";
               return false;
             }//if
             keywordArray.push_back(temp);
           }//for
           break;
      case cDESC:
           if (hasDESC)
           {
             std::cout << "Error: ARMO seems to have more than one DESC subrecord!\n";
             return false;
           }
           //DESC's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord DESC of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DESC's stuff
           in_File.read((char*) &descriptionStringID, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DESC of ARMO!\n";
             return false;
           }//if
           hasDESC = true;
           break;
      case cMODL:
           //MODL's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord MODL of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read MODL's stuff
           temp = 0;
           in_File.read((char*) &temp, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord MODL of ARMO!\n";
             return false;
           }//if
           unknownMODLs.push_back(temp);
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: record ARMO seems to have more than one DATA subrecord!\n";
             return false;
           }
           //DATA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DATA of ARMO has invalid length ("
                       <<subLength<<" bytes. Should be 8 bytes!\n";
             return false;
           }
           //read DATA's stuff
           in_File.read((char*) unknownDATA, 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DATA of ARMO!\n";
             return false;
           }
           hasReadDATA = true;
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: ARMO seems to have more than one DNAM subrecord!\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord DNAM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read DNAM's stuff
           in_File.read((char*) &unknownDNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of ARMO!\n";
             return false;
           }//if
           hasReadDNAM = true;
           break;
      case cTNAM:
           if (hasTNAM)
           {
             std::cout << "Error: ARMO seems to have more than one TNAM subrecord!\n";
             return false;
           }
           //TNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: subrecord TNAM of ARMO has invalid length ("
                       <<subLength<<" bytes). Should be four bytes!\n";
             return false;
           }
           //read TNAM's stuff
           in_File.read((char*) &unknownTNAM, 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord TNAM of ARMO!\n";
             return false;
           }//if
           hasTNAM = true;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only VMAD, OBND, FULL, EITM, MOD2, MO2T,"
                     << " MOD4, MO4T, BODT, ETYP, BIDS, BAMT, YNAM, ZNAM, RNAM,"
                     << " KSIZ, DESC, MODL, DATA, DNAM, or TNAM are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //presence checks
  if (!(hasReadOBND and unknownBODT.isPresent() and hasReadRNAM and hasReadDATA and hasReadDNAM))
  {
    std::cout << "Error: subrecord OBND, BODT, DATA, DNAM or RNAM of ARMO is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t ArmourRecord::getRecordType() const
{
  return cARMO;
}

} //namespace
