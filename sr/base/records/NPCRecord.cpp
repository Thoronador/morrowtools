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

#include "NPCRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"
#include "../../../base/CompressionFunctions.h"
#include "../../../base/BufferStream.h"

namespace SRTP
{

/* PerkElem's functions */

bool NPCRecord::PerkElem::operator==(const NPCRecord::PerkElem& other) const
{
  return ((formID==other.formID) and (valueTwo==other.valueTwo));
}

/* NPCRecord's functions */

NPCRecord::NPCRecord()
: BasicRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  memset(unknownOBND, 0, 12);
  memset(unknownACBS, 0, 24);
  unknownSNAMs.clear();
  hasINAM = false;
  unknownINAM = 0;
  hasVTCK = false;
  unknownVTCK = 0;
  unknownTPLT = 0;
  unknownRNAM = 0;
  hasWNAM = false;
  unknownWNAM = 0;
  hasATKR = false;
  unknownATKR = 0;
  spellFormIDs.clear();
  perkList.clear();
  items.clear();
  memset(unknownAIDT, 0, 20);
  unknownPKIDs.clear();
  keywordArray.clear();
  unknownCNAM = 0;
  memset(unknownDNAM, 0, 52);
  hasZNAM = false;
  unknownZNAM = 0;
  unknownNAM5 = 0;
  unknownNAM6 = 0;
  unknownNAM7 = 0;
  unknownNAM8 = 0;
  hasCSCR = false;
  unknownCSCR = 0;
  hasDPLT = false;
  unknownDPLT = 0;
  memset(unknownQNAM, 0, 12);
}

NPCRecord::~NPCRecord()
{
  //empty
}

bool NPCRecord::equals(const NPCRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (memcmp(unknownACBS, other.unknownACBS, 24)==0)
      and (unknownSNAMs==other.unknownSNAMs)
      and (hasINAM==other.hasINAM) and ((unknownINAM==other.unknownINAM) or (!hasINAM))
      and (hasVTCK==other.hasVTCK) and ((unknownVTCK==other.unknownVTCK) or (!hasVTCK))
      and (unknownTPLT==other.unknownTPLT) and (unknownRNAM==other.unknownRNAM)
      and (hasWNAM==other.hasWNAM) and ((unknownWNAM==other.unknownWNAM) or (!hasWNAM))
      and (hasATKR==other.hasATKR) and ((unknownATKR==other.unknownATKR) or (!hasATKR))
      and (spellFormIDs==spellFormIDs) and (perkList==other.perkList)
      and (items==other.items)
      and (memcmp(unknownAIDT, other.unknownAIDT, 20)==0)
      and (unknownPKIDs==other.unknownPKIDs) and (keywordArray==other.keywordArray)
      and (unknownCNAM==other.unknownCNAM)
      and (memcmp(unknownDNAM, other.unknownDNAM, 52)==0)
      and (hasZNAM==other.hasZNAM) and ((unknownZNAM==other.unknownZNAM) or (!hasZNAM))
      and (unknownNAM5==other.unknownNAM5) and (unknownNAM6==other.unknownNAM6)
      and (unknownNAM7==other.unknownNAM7) and (unknownNAM8==other.unknownNAM8)
      and (hasCSCR==other.hasCSCR) and ((unknownCSCR==other.unknownCSCR) or (!hasCSCR))
      and (hasDPLT==other.hasDPLT) and ((unknownDPLT==other.unknownDPLT) or (!hasDPLT))
      and (memcmp(unknownQNAM, other.unknownQNAM, 12)==0));
}

uint32_t NPCRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = + 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* ACBS */ +2 /* 2 bytes for length */ +24 /* fixed size */
        +unknownSNAMs.size()*(4 /* SNAM */ +2 /* 2 bytes for length */ +8 /* fixed size */)
        +4 /* TPLT */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* AIDT */ +2 /* 2 bytes for length */ +20 /* fixed size */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +0 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +52 /* fixed size */
        +4 /* NAM5 */ +2 /* 2 bytes for length */ +2 /* fixed size */
        +4 /* NAM6 */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* NAM7 */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* NAM8 */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* QNAM */ +2 /* 2 bytes for length */ +12 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize() /* size of subrecord */;
  }
  if (hasINAM)
  {
    writeSize = writeSize +4 /* INAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasVTCK)
  {
    writeSize = writeSize +4 /* VTCK */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasWNAM)
  {
    writeSize = writeSize +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasATKR)
  {
    writeSize = writeSize +4 /* ATKR */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!spellFormIDs.empty())
  {
    writeSize = writeSize +4 /* SPCT */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +items.size()*(4 /* SPLO */ +2 /* 2 bytes for length */ +4 /* fixed size */);
  }
  if (!perkList.empty())
  {
    writeSize = writeSize +4 /* PRKZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +perkList.size()*(4 /* PRKR */ +2 /* 2 bytes for length */ +8 /* fixed size */);
  }
  if (!items.empty())
  {
    writeSize = writeSize +4 /* COCT */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +items.size()*(4 /* CNTO */ +2 /* 2 bytes for length */ +8 /* fixed size */);
  }
  if (!unknownPKIDs.empty())
  {
    writeSize = writeSize +unknownPKIDs.size()*(4 /* PKID */ +2 /* 2 bytes for length */ +4 /* fixed size */);
  }
  if (!keywordArray.empty())
  {
    writeSize = writeSize +4 /* KSIZ */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* KWDA */ +2 /* 2 bytes for length */ +4*keywordArray.size() /* fixed size */;
  }
  if (hasZNAM)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasCSCR)
  {
    writeSize = writeSize +4 /* CSCR */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasDPLT)
  {
    writeSize = writeSize +4 /* DPLT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  return writeSize;
}

bool NPCRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cNPC_, 4);
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
      std::cout << "Error while writing subrecord VMAD of NPC_!\n!";
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

  //write ACBS
  output.write((const char*) &cACBS, 4);
  //ACBS's length
  subLength = 24; //fixed size
  output.write((const char*) &subLength, 2);
  //write ACBS
  output.write((const char*) unknownACBS, 24);

  if (!unknownSNAMs.empty())
  {
    unsigned int i;
    for (i=0; i<unknownSNAMs.size(); ++i)
    {
      //write SNAM
      output.write((const char*) &cSNAM, 4);
      //SNAM's length
      subLength = 8; //fixed size
      output.write((const char*) &subLength, 2);
      //write SNAM
      output.write((const char*) &(unknownSNAMs[i]), 8);
    }//for
  }

  if (hasINAM)
  {
    //write INAM
    output.write((const char*) &cINAM, 4);
    //INAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write INAM
    output.write((const char*) &unknownINAM, 4);
  }//if INAM

  if (hasVTCK)
  {
    //write VTCK
    output.write((const char*) &cVTCK, 4);
    //VTCK's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write VTCK
    output.write((const char*) &unknownVTCK, 4);
  }//if VTCK

  //write TPLT
  output.write((const char*) &cTPLT, 4);
  //TPLT's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write TPLT
  output.write((const char*) &unknownTPLT, 4);

  //write RNAM
  output.write((const char*) &cRNAM, 4);
  //RNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write RNAM
  output.write((const char*) &unknownRNAM, 4);

  if (hasWNAM)
  {
    //write WNAM
    output.write((const char*) &cWNAM, 4);
    //WNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write WNAM
    output.write((const char*) &unknownWNAM, 4);
  }//if WNAM

  if (hasATKR)
  {
    //write ATKR
    output.write((const char*) &cATKR, 4);
    //ATKR's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write ATKR
    output.write((const char*) &unknownATKR, 4);
  }//if ATKR

  if (!spellFormIDs.empty())
  {
    const uint32_t count = spellFormIDs.size();
    //write SPCT
    output.write((const char*) &cSPCT, 4);
    //SPCT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write SPCT
    output.write((const char*) &count, 4);

    unsigned int i;
    for (i=0; i<count; ++i)
    {
      //write SPLO
      output.write((const char*) &cSPLO, 4);
      //SPLO's length
      subLength = 4; //fixed size
      output.write((const char*) &subLength, 2);
      //write SPLO
      output.write((const char*) &(spellFormIDs[i]), 4);
    }//for
  }//spells

  if (!perkList.empty())
  {
    const uint32_t count = perkList.size();
    //write PRKZ
    output.write((const char*) &cPRKZ, 4);
    //PRKZ's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write PRKZ
    output.write((const char*) &count, 4);

    unsigned int i;
    for (i=0; i<count; ++i)
    {
      //write PRKR
      output.write((const char*) &cPRKR, 4);
      //PRKR's length
      subLength = 8; //fixed size
      output.write((const char*) &subLength, 2);
      //write PRKR
      output.write((const char*) &(perkList[i].formID), 4);
      output.write((const char*) &(perkList[i].valueTwo), 4);
    }//for
  }//perks

  if (!items.empty())
  {
    const uint32_t count = items.size();
    //write COCT
    output.write((const char*) &cCOCT, 4);
    //COCT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write COCT
    output.write((const char*) &count, 4);

    unsigned int i;
    for (i=0; i<count; ++i)
    {
      //write CNTO
      output.write((const char*) &cCNTO, 4);
      //CNTO's length
      subLength = 8; //fixed size
      output.write((const char*) &subLength, 2);
      //write CNTO
      output.write((const char*) &(items[i].formID), 4);
      output.write((const char*) &(items[i].count), 4);
    }//for
  }//items

  //write AIDT
  output.write((const char*) &cAIDT, 4);
  //AIDT's length
  subLength = 20; //fixed size
  output.write((const char*) &subLength, 2);
  //write AIDT
  output.write((const char*) unknownACBS, 20);

  if (!unknownPKIDs.empty())
  {
    unsigned int i;
    unsigned int count = unknownPKIDs.size();
    for (i=0; i<count; ++i)
    {
      //write PKID
      output.write((const char*) &cPKID, 4);
      //PKID's length
      subLength = 4; //fixed size
      output.write((const char*) &subLength, 2);
      //write PKID
      output.write((const char*) &(unknownPKIDs[i]), 4);
    }//for
  }

  if (!keywordArray.empty())
  {
    unsigned int k_Size = keywordArray.size();
    //write KSIZ
    output.write((const char*) &cKSIZ, 4);
    //KSIZ's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write KSIZ
    output.write((const char*) &k_Size, 4);

    //write KWDA
    output.write((const char*) &cKWDA, 4);
    //KWDA's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    unsigned int i;
    for (i=0; i<k_Size; ++i)
    {
      //write KWDA
      output.write((const char*) &(keywordArray[i]), 4);
    }//for
  }//if keywords

  //write CNAM
  output.write((const char*) &cCNAM, 4);
  //CNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write CNAM
  output.write((const char*) &unknownCNAM, 4);

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 52; //fixed size
  output.write((const char*) &subLength, 2);
  //write DNAM
  output.write((const char*) unknownDNAM, 52);

  if (hasZNAM)
  {
    //write ZNAM
    output.write((const char*) &cZNAM, 4);
    //ZNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write ZNAM
    output.write((const char*) &unknownZNAM, 4);
  }//if ZNAM

  //write NAM5
  output.write((const char*) &cNAM5, 4);
  //NAM5's length
  subLength = 2; //fixed size
  output.write((const char*) &subLength, 2);
  //write NAM5
  output.write((const char*) &unknownNAM5, 2);

  //write NAM6
  output.write((const char*) &cNAM6, 4);
  //NAM6's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write NAM6
  output.write((const char*) &unknownNAM6, 4);

  //write NAM7
  output.write((const char*) &cNAM7, 4);
  //NAM7's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write NAM7
  output.write((const char*) &unknownNAM7, 4);

  //write NAM8
  output.write((const char*) &cNAM8, 4);
  //NAM8's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write NAM8
  output.write((const char*) &unknownNAM8, 4);

  if (hasCSCR)
  {
    //write CSCR
    output.write((const char*) &cCSCR, 4);
    //CSCR's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write CSCR
    output.write((const char*) &unknownCSCR, 4);
  }//if CSCR

  if (hasDPLT)
  {
    //write DPLT
    output.write((const char*) &cDPLT, 4);
    //DPLT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write DPLT
    output.write((const char*) &unknownDPLT, 4);
  }//if DPLT

  //write QNAM
  output.write((const char*) &cQNAM, 4);
  //QNAM's length
  subLength = 12; //fixed size
  output.write((const char*) &subLength, 2);
  //write QNAM
  output.write((const char*) unknownQNAM, 12);

  return output.good();
}

bool NPCRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName;
  uint16_t subLength;
  subRecName = subLength = 0;
  uint32_t bytesRead = 0;

  MWTP::BufferStream decompStream(NULL, 0);
  std::basic_istream<char>* actual_in = &in_File;

  if (isCompressed())
  {
    uint32_t decompressedSize = 0;
    //read size of decompressed data
    in_File.read((char*) &decompressedSize, 4);
    if (!in_File.good())
    {
      std::cout << "Error while reading decompression size of NPC_!\n";
      return false;
    }
    if (readSize<=4)
    {
      std::cout << "Error: size of compressed NPC_ record is too small to contain any compressed data!\n";
      return false;
    }
    //buffer to read compressed data
    uint8_t * stream_buffer = new uint8_t[readSize-4];
    in_File.read((char*) stream_buffer, readSize-4);
    if (!in_File.good())
    {
      std::cout << "Error while reading compressed data of NPC_!\n";
      delete[] stream_buffer;
      return false;
    }
    //now decompress here
    uint8_t * decompBuffer = new uint8_t [decompressedSize];
    if (!MWTP::zlibDecompress(stream_buffer, readSize-4, decompBuffer, decompressedSize))
    {
      std::cout << "Error while executing decompression function!\n";
      delete[] stream_buffer;
      stream_buffer = NULL;
      delete[] decompBuffer;
      decompBuffer = NULL;
      return false;
    }
    delete[] stream_buffer;
    stream_buffer = NULL;

    //DBG-dev
    std::ofstream tmpOut;
    tmpOut.open("C:\\Temp\\decomp_stuff.dat", std::ios_base::out | std::ios_base::binary);
    if (!tmpOut)
    {
      std::cout << "Error: could not open temp file!\n";
      return false;
    }
    tmpOut.write((const char*) decompBuffer, decompressedSize);
    tmpOut.close();

    //end of DBG-dev

    //set underlying buffer for decompressed stream
    decompStream.buffer((char*) decompBuffer, decompressedSize);
    actual_in = &decompStream;
    //update read size
    readSize = decompressedSize;
  }

  //read EDID
  actual_in->read((char*) &subRecName, 4);
  bytesRead = 4;
  if (subRecName!=cEDID)
  {
    UnexpectedRecord(cEDID, subRecName);
    return false;
  }
  //EDID's length
  actual_in->read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of BOOK is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  actual_in->read(buffer, subLength);
  bytesRead += subLength;
  if (!actual_in->good())
  {
    std::cout << "Error while reading subrecord EDID of BOOK!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  bool hasReadACBS = false;
  unknownSNAMs.clear();
  uint64_t tempUint64;
  hasINAM = false; unknownINAM = 0;
  hasVTCK = false; unknownVTCK = 0;
  bool hasReadTPLT = false; unknownTPLT = 0;
  bool hasReadRNAM = false; unknownRNAM = 0;
  hasATKR = false; unknownATKR = 0;
  spellFormIDs.clear();
  uint32_t tempUint32, size_int;
  perkList.clear();
  PerkElem tempPerk;
  items.clear();
  ComponentData tempComponent;
  unsigned int i;
  bool hasReadAIDT = false;
  unknownPKIDs.clear();
  keywordArray.clear();
  bool hasReadCNAM = false; unknownCNAM = 0;
  bool hasReadDNAM = false;
  hasZNAM = false; unknownZNAM = 0;
  bool hasReadNAM5 = false; unknownNAM5 = 0;
  bool hasReadNAM6 = false; unknownNAM6 = 0;
  bool hasReadNAM7 = false; unknownNAM7 = 0;
  bool hasReadNAM8 = false; unknownNAM8 = 0;
  hasCSCR = false; unknownCSCR = 0;
  hasDPLT = false; unknownDPLT = 0;
  bool hasReadQNAM = false;
  while (bytesRead<readSize)
  {
    //read next header
    actual_in->read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cout << "Error: NPC_ seems to have more than one VMAD subrecord.\n";
             return false;
           }
           //read VMAD
           if (!unknownVMAD.loadFromStream(*actual_in, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of NPC_!\n";
             return false;
           }
           bytesRead += (2+unknownVMAD.getSize());
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cout << "Error: NPC_ seems to have more than one OBND subrecord.\n";
             return false;
           }
           //OBND's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record OBND of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read OBND
           actual_in->read((char*) unknownOBND, 12);
           bytesRead += 12;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord OBND of NPC_!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cACBS:
           if (hasReadACBS)
           {
             std::cout << "Error: NPC_ seems to have more than one ACBS subrecord.\n";
             return false;
           }
           //ACBS's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=24)
           {
             std::cout <<"Error: sub record ACBS of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be 24 bytes.\n";
             return false;
           }
           //read ACBS
           actual_in->read((char*) unknownACBS, 24);
           bytesRead += 24;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord ACBS of NPC_!\n";
             return false;
           }
           hasReadACBS = true;
           break;
      case cSNAM:
           //SNAM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record SNAM of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be 8 bytes.\n";
             return false;
           }
           //read SNAM
           tempUint64 = 0;
           actual_in->read((char*) &tempUint64, 8);
           bytesRead += 8;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord SNAM of NPC_!\n";
             return false;
           }
           unknownSNAMs.push_back(tempUint64);
           break;
      case cINAM:
           if (hasINAM)
           {
             std::cout << "Error: NPC_ seems to have more than one INAM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read INAM
           if (!loadUint32SubRecordFromStream(*actual_in, cINAM, unknownINAM)) return false;
           bytesRead += 6;
           hasINAM = true;
           break;
      case cVTCK:
           if (hasVTCK)
           {
             std::cout << "Error: NPC_ seems to have more than one VTCK subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read VTCK
           if (!loadUint32SubRecordFromStream(*actual_in, cVTCK, unknownVTCK)) return false;
           bytesRead += 6;
           hasVTCK = true;
           break;
      case cTPLT:
           if (hasReadTPLT)
           {
             std::cout << "Error: NPC_ seems to have more than one TPLT subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read TPLT
           if (!loadUint32SubRecordFromStream(*actual_in, cTPLT, unknownTPLT)) return false;
           bytesRead += 6;
           hasReadTPLT = true;
           break;
      case cRNAM:
           if (hasReadRNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one RNAM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read RNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cRNAM, unknownRNAM)) return false;
           bytesRead += 6;
           hasReadRNAM = true;
           break;
      case cWNAM:
           if (hasWNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one WNAM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read WNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cWNAM, unknownWNAM)) return false;
           bytesRead += 6;
           hasWNAM = true;
           break;
      case cATKR:
           if (hasATKR)
           {
             std::cout << "Error: NPC_ seems to have more than one ATKR subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read ATKR
           if (!loadUint32SubRecordFromStream(*actual_in, cATKR, unknownATKR)) return false;
           bytesRead += 6;
           hasATKR = true;
           break;
      case cSPCT:
           if (!spellFormIDs.empty())
           {
             std::cout << "Error: NPC_ seems to have more than one SPCT subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read SPCT
           if (!loadUint32SubRecordFromStream(*actual_in, cSPCT, size_int)) return false;
           bytesRead += 6;
           if (size_int==0)
           {
             std::cout << "Error: Value of subrecord SPCT of NPC_ is zero!\n";
             return false;
           }

           for (i=0; i<size_int; ++i)
           {
             //read SPLO
             actual_in->read((char*) &subRecName, 4);
             bytesRead += 4;
             if (subRecName!=cSPLO)
             {
               UnexpectedRecord(cSPLO, subRecName);
               return false;
             }
             //SPLO's length
             actual_in->read((char*) &subLength, 2);
             bytesRead += 2;
             if (subLength!=4)
             {
               std::cout <<"Error: sub record SPLO of NPC_ has invalid length ("
                         <<subLength<<" bytes). Should be four bytes.\n";
               return false;
             }
             //read SPLO
             actual_in->read((char*) &tempUint32, 4);
             bytesRead += 4;
             if (!actual_in->good())
             {
               std::cout << "Error while reading subrecord SPLO of NPC_!\n";
               return false;
             }
             spellFormIDs.push_back(tempUint32);
           }//for
           break;
      case cPRKZ:
           if (!perkList.empty())
           {
             std::cout << "Error: NPC_ seems to have more than one PRKZ subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read PRKZ
           if (!loadUint32SubRecordFromStream(*actual_in, cPRKZ, size_int)) return false;
           bytesRead += 6;
           if (size_int==0)
           {
             std::cout << "Error: Value of subrecord PRKZ of NPC_ is zero!\n";
             return false;
           }

           for (i=0; i<size_int; ++i)
           {
             //read PRKR
             actual_in->read((char*) &subRecName, 4);
             bytesRead += 4;
             if (subRecName!=cPRKR)
             {
               UnexpectedRecord(cPRKR, subRecName);
               return false;
             }
             //PRKR's length
             actual_in->read((char*) &subLength, 2);
             bytesRead += 2;
             if (subLength!=8)
             {
               std::cout <<"Error: sub record PRKR of NPC_ has invalid length ("
                         <<subLength<<" bytes). Should be 8 bytes.\n";
               return false;
             }
             //read PRKR
             actual_in->read((char*) &(tempPerk.formID), 4);
             actual_in->read((char*) &(tempPerk.valueTwo), 4);
             bytesRead += 8;
             if (!actual_in->good())
             {
               std::cout << "Error while reading subrecord PRKR of NPC_!\n";
               return false;
             }
             perkList.push_back(tempPerk);
           }//for
           break;
      case cCOCT:
           if (!items.empty())
           {
             std::cout << "Error: NPC_ seems to have more than one COCT subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read COCT
           if (!loadUint32SubRecordFromStream(*actual_in, cCOCT, tempUint32)) return false;
           bytesRead += 6;
           if (tempUint32==0)
           {
             std::cout << "Error: Value of subrecord COCT of NPC_ is zero!\n";
             return false;
           }

           for (i=0; i<tempUint32; ++i)
           {
             //read CNTO
             actual_in->read((char*) &subRecName, 4);
             bytesRead += 4;
             if (subRecName!=cCNTO)
             {
               UnexpectedRecord(cCNTO, subRecName);
               return false;
             }
             //CNTO's length
             actual_in->read((char*) &subLength, 2);
             bytesRead += 2;
             if (subLength!=8)
             {
               std::cout <<"Error: sub record CNTO of NPC_ has invalid length ("
                         <<subLength<<" bytes). Should be 8 bytes.\n";
               return false;
             }
             //read CNTO
             actual_in->read((char*) &(tempComponent.formID), 4);
             actual_in->read((char*) &(tempComponent.count), 4);
             bytesRead += 8;
             if (!actual_in->good())
             {
               std::cout << "Error while reading subrecord CNTO of NPC_!\n";
               return false;
             }
             items.push_back(tempComponent);
           }//for
           break;
      case cAIDT:
           if (hasReadAIDT)
           {
             std::cout << "Error: NPC_ seems to have more than one AIDT subrecord.\n";
             return false;
           }
           //AIDT's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=20)
           {
             std::cout <<"Error: sub record AIDT of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be 20 bytes.\n";
             return false;
           }
           //read AIDT
           actual_in->read((char*) unknownAIDT, 20);
           bytesRead += 20;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord AIDT of NPC_!\n";
             return false;
           }
           hasReadAIDT = true;
           break;
      case cPKID:
           //PKID's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record PKID of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read PKID
           tempUint32 = 0;
           actual_in->read((char*) &tempUint32, 4);
           bytesRead += 4;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord PKID of NPC_!\n";
             return false;
           }
           unknownPKIDs.push_back(tempUint32);
           break;
      case cKSIZ:
           if (!keywordArray.empty())
           {
             std::cout << "Error: NPC_ seems to have more than one KSIZ subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read KSIZ
           if (!loadUint32SubRecordFromStream(*actual_in, cKSIZ, size_int)) return false;
           bytesRead += 6;
           if (size_int==0)
           {
             std::cout << "Error: Value of subrecord KSIZ of NPC_ is zero!\n";
             return false;
           }

           //read KWDA
           actual_in->read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cKWDA)
           {
             UnexpectedRecord(cKWDA, subRecName);
             return false;
           }
           //KWDA's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4*size_int)
           {
             std::cout <<"Error: sub record KWDA of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be "<<4*size_int<<" bytes.\n";
             return false;
           }
           for (i=0; i<size_int; ++i)
           {
             //read content
             actual_in->read((char*) &tempUint32, 4);
             bytesRead += 4;
             if (!actual_in->good())
             {
               std::cout << "Error while reading subrecord KWDA of NPC_!\n";
               return false;
             }
             keywordArray.push_back(tempUint32);
           }//for
           break;
      case cCNAM:
           if (hasReadCNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one CNAM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read CNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cCNAM, unknownCNAM)) return false;
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cDATA:
           if (hasReadDNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one DNAM subrecord.\n";
             return false;
           }
           //DATA's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout <<"Error: sub record DATA of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be zero bytes.\n";
             return false;
           }
           //zero bytes - no need to read

           //read DNAM
           actual_in->read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cDNAM)
           {
             UnexpectedRecord(cDNAM, subRecName);
             return false;
           }
           //DNAM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=52)
           {
             std::cout <<"Error: sub record DNAM of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be 52 bytes.\n";
             return false;
           }
           //read DNAM
           actual_in->read((char*) unknownDNAM, 52);
           bytesRead += 52;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord DNAM of NPC_!\n";
             return false;
           }
           hasReadDNAM = true;
           break;
      case cZNAM:
           if (hasZNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read ZNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cZNAM, unknownZNAM)) return false;
           bytesRead += 6;
           hasZNAM = true;
           break;
      case cNAM5:
           if (hasReadNAM5)
           {
             std::cout << "Error: NPC_ seems to have more than one NAM5 subrecord.\n";
             return false;
           }
           //NAM5's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record NAM5 of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be two bytes.\n";
             return false;
           }
           //read NAM5
           actual_in->read((char*) &unknownNAM5, 2);
           bytesRead += 2;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord NAM5 of NPC_!\n";
             return false;
           }
           hasReadNAM5 = true;
           break;
      case cNAM6:
           if (hasReadNAM6)
           {
             std::cout << "Error: NPC_ seems to have more than one NAM6 subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read NAM6
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM6, unknownNAM6)) return false;
           bytesRead += 6;
           hasReadNAM6 = true;
           break;
      case cNAM7:
           if (hasReadNAM7)
           {
             std::cout << "Error: NPC_ seems to have more than one NAM7 subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read NAM7
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM7, unknownNAM7)) return false;
           bytesRead += 6;
           hasReadNAM7 = true;
           break;
      case cNAM8:
           if (hasReadNAM8)
           {
             std::cout << "Error: NPC_ seems to have more than one NAM8 subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read NAM8
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM8, unknownNAM8)) return false;
           bytesRead += 6;
           hasReadNAM8 = true;
           break;
      case cCSCR:
           if (hasCSCR)
           {
             std::cout << "Error: NPC_ seems to have more than one CSCR subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read CSCR
           if (!loadUint32SubRecordFromStream(*actual_in, cCSCR, unknownCSCR)) return false;
           bytesRead += 6;
           hasCSCR = true;
           break;
      case cDPLT:
           if (hasDPLT)
           {
             std::cout << "Error: NPC_ seems to have more than one DPLT subrecord.\n";
             return false;
           }
           actual_in->seekg(-4, std::ios_base::cur);
           //read DPLT
           if (!loadUint32SubRecordFromStream(*actual_in, cDPLT, unknownDPLT)) return false;
           bytesRead += 6;
           hasDPLT = true;
           break;
      case cQNAM:
           if (hasReadQNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one QNAM subrecord.\n";
             return false;
           }
           //QNAM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record QNAM of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read QNAM
           actual_in->read((char*) unknownQNAM, 12);
           bytesRead += 12;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord QNAM of NPC_!\n";
             return false;
           }
           hasReadQNAM = true;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, ACBS, SNAM, INAM, VTCK, TPLT,"
                     << " RNAM, WNAM, ATKR, SPCT, PRKZ, CNTO, AIDT, KSIZ, CNAM,"
                     << " DATA, ZNAM, NAM5, NAM6, NAM7, NAM8, DPLT, CSCR or QNAM are allowed here!\n";
           return false;
    }//swi
  }//while

  //check presence of all required subrecords
  if (!(hasReadOBND and hasReadACBS and hasReadTPLT and hasReadRNAM
        and hasReadAIDT and hasReadCNAM and hasReadDNAM and hasReadDNAM
        and hasReadNAM5 and hasReadNAM6 and hasReadNAM7 and hasReadNAM8
        and hasReadQNAM))
  {
    std::cout << "Error: At least one required subrecord of NPC_ was not found!\n";
    return false;
  }

  return actual_in->good();
}

int32_t NPCRecord::getRecordType() const
{
  return cNPC_;
}

} //namespace
