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

/* TINXstructure's functions */

bool NPCRecord::TINXstructure::operator==(const NPCRecord::TINXstructure& other) const
{
  return ((unknownTINI==other.unknownTINI) and (unknownTINC==other.unknownTINC)
      and (unknownTINV==other.unknownTINV) and (unknownTIAS==other.unknownTIAS));
}

/* CSDXstruct's functions */

bool NPCRecord::CSDXstruct::operator==(const NPCRecord::CSDXstruct& other) const
{
  return ((unknownCSDT==other.unknownCSDT) and (unknownCSDI==other.unknownCSDI)
      and (unknownCSDC==other.unknownCSDC));
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
  hasTPLT = false;
  unknownTPLT = 0;
  unknownRNAM = 0;
  hasDEST = false;
  unknownDEST = 0;
  hasWNAM = false;
  unknownWNAM = 0;
  hasANAM = false;
  unknownANAM = 0;
  hasATKR = false;
  unknownATKR = 0;
  unknownATKD.setPresence(false);
  unknownATKE = "";
  spellFormIDs.clear();
  perkList.clear();
  items.clear();
  hasSPOR = false;
  unknownSPOR = 0;
  hasECOR = false;
  unknownECOR = 0;
  memset(unknownAIDT, 0, 20);
  unknownPKIDs.clear();
  keywordArray.clear();
  unknownCNAM = 0;
  hasFULL = false;
  nameStringID = 0; //subrecord FULL
  hasSHRT = false;
  unknownSHRT = 0;
  memset(unknownDNAM, 0, 52);
  unknownPNAMs.clear();
  hasHCLF = false;
  unknownHCLF = 0;
  hasGNAM = false;
  unknownGNAM = 0;
  hasZNAM = false;
  unknownZNAM = 0;
  unknownNAM5 = 0;
  unknownNAM6 = 0;
  unknownNAM7 = 0;
  unknownNAM8 = 0;
  hasDOFT = false;
  unknownDOFT = 0;
  hasSOFT = false;
  unknownSOFT = 0;
  hasCRIF = false;
  unknownCRIF = 0;
  hasCSCR = false;
  unknownCSCR = 0;
  unknownCSDXs.clear();
  hasDPLT = false;
  unknownDPLT = 0;
  hasFTST = false;
  unknownFTST = 0;
  memset(unknownQNAM, 0, 12);
  unknownNAM9.setPresence(false);
  unknownNAMA.setPresence(false);
  unknownTINXs.clear();
}

NPCRecord::~NPCRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool NPCRecord::equals(const NPCRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (unknownVMAD==other.unknownVMAD)
      and (memcmp(unknownOBND, other.unknownOBND, 12)==0)
      and (memcmp(unknownACBS, other.unknownACBS, 24)==0)
      and (unknownSNAMs==other.unknownSNAMs)
      and (hasINAM==other.hasINAM) and ((unknownINAM==other.unknownINAM) or (!hasINAM))
      and (hasVTCK==other.hasVTCK) and ((unknownVTCK==other.unknownVTCK) or (!hasVTCK))
      and (hasTPLT==other.hasTPLT) and ((unknownTPLT==other.unknownTPLT) or (!hasTPLT))
      and (unknownRNAM==other.unknownRNAM)
      and (hasDEST==other.hasDEST) and ((unknownDEST==other.unknownDEST) or (!hasDEST))
      and (hasWNAM==other.hasWNAM) and ((unknownWNAM==other.unknownWNAM) or (!hasWNAM))
      and (hasANAM==other.hasANAM) and ((unknownANAM==other.unknownANAM) or (!hasANAM))
      and (hasATKR==other.hasATKR) and ((unknownATKR==other.unknownATKR) or (!hasATKR))
      and (unknownATKD==other.unknownATKD) and (unknownATKE==other.unknownATKE)
      and (spellFormIDs==spellFormIDs) and (perkList==other.perkList)
      and (items==other.items)
      and (hasSPOR==other.hasSPOR) and ((unknownSPOR==other.unknownSPOR) or (!hasSPOR))
      and (hasECOR==other.hasECOR) and ((unknownECOR==other.unknownECOR) or (!hasECOR))
      and (memcmp(unknownAIDT, other.unknownAIDT, 20)==0)
      and (unknownPKIDs==other.unknownPKIDs) and (keywordArray==other.keywordArray)
      and (unknownCNAM==other.unknownCNAM)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (hasSHRT==other.hasSHRT) and ((unknownSHRT==other.unknownSHRT) or (!hasSHRT))
      and (memcmp(unknownDNAM, other.unknownDNAM, 52)==0)
      and (unknownPNAMs==other.unknownPNAMs)
      and (hasHCLF==other.hasHCLF) and ((unknownHCLF==other.unknownHCLF) or (!hasHCLF))
      and (hasGNAM==other.hasGNAM) and ((unknownGNAM==other.unknownGNAM) or (!hasGNAM))
      and (hasZNAM==other.hasZNAM) and ((unknownZNAM==other.unknownZNAM) or (!hasZNAM))
      and (unknownNAM5==other.unknownNAM5) and (unknownNAM6==other.unknownNAM6)
      and (unknownNAM7==other.unknownNAM7) and (unknownNAM8==other.unknownNAM8)
      and (hasDOFT==other.hasDOFT) and ((unknownDOFT==other.unknownDOFT) or (!hasDOFT))
      and (hasSOFT==other.hasSOFT) and ((unknownSOFT==other.unknownSOFT) or (!hasSOFT))
      and (hasCRIF==other.hasCRIF) and ((unknownCRIF==other.unknownCRIF) or (!hasCRIF))
      and (hasCSCR==other.hasCSCR) and ((unknownCSCR==other.unknownCSCR) or (!hasCSCR))
      and (unknownCSDXs==unknownCSDXs)
      and (hasDPLT==other.hasDPLT) and ((unknownDPLT==other.unknownDPLT) or (!hasDPLT))
      and (hasFTST==other.hasFTST) and ((unknownFTST==other.unknownFTST) or (!hasFTST))
      and (memcmp(unknownQNAM, other.unknownQNAM, 12)==0)
      and (unknownNAM9==other.unknownNAM9) and (unknownNAMA==other.unknownNAMA)
      and (unknownTINXs==other.unknownTINXs));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t NPCRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = + 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +4 /* OBND */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* ACBS */ +2 /* 2 bytes for length */ +24 /* fixed size */
        +unknownSNAMs.size()*(4 /* SNAM */ +2 /* 2 bytes for length */ +8 /* fixed size */)
        +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* AIDT */ +2 /* 2 bytes for length */ +20 /* fixed size */
        +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */
        +4 /* DATA */ +2 /* 2 bytes for length */ +0 /* fixed size */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +52 /* fixed size */
        +unknownPNAMs.size()*(4 /* PNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */)
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
  if (hasTPLT)
  {
    writeSize = writeSize +4 /* TPLT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasDEST)
  {
    writeSize = writeSize +4 /* DEST */ +2 /* 2 bytes for length */ +8 /* fixed size */;
  }
  if (hasWNAM)
  {
    writeSize = writeSize +4 /* WNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasANAM)
  {
    writeSize = writeSize +4 /* ANAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasATKR)
  {
    writeSize = writeSize +4 /* ATKR */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unknownATKD.isPresent())
  {
    writeSize = writeSize +4 /* ATKD */ +2 /* 2 bytes for length */ +unknownATKD.getSize() /* size */;
  }
  if (!unknownATKE.empty())
  {
    writeSize = writeSize +4 /* ATKE */ +2 /* 2 bytes for length */ +unknownATKE.length()+1 /* length + NUL */;
  }
  if (!spellFormIDs.empty())
  {
    writeSize = writeSize +4 /* SPCT */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +spellFormIDs.size()*(4 /* SPLO */ +2 /* 2 bytes for length */ +4 /* fixed size */);
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
  if (hasECOR)
  {
    writeSize = writeSize +4 /* ECOR */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasSPOR)
  {
    writeSize = writeSize +4 /* SPOR */ +2 /* 2 bytes for length */ +4 /* fixed size */;
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
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasSHRT)
  {
    writeSize = writeSize +4 /* SHRT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasHCLF)
  {
    writeSize = writeSize +4 /* HCLF */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasGNAM)
  {
    writeSize = writeSize +4 /* GNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasZNAM)
  {
    writeSize = writeSize +4 /* ZNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasDOFT)
  {
    writeSize = writeSize +4 /* DOFT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasSOFT)
  {
    writeSize = writeSize +4 /* SOFT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasCRIF)
  {
    writeSize = writeSize +4 /* CRIF */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasCSCR)
  {
    writeSize = writeSize +4 /* CSCR */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unknownCSDXs.empty())
  {
    writeSize = writeSize +unknownCSDXs.size()*(4 /* CSDT */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* CSDI */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* CSDC */ +2 /* 2 bytes for length */ +1 /* fixed size */);
  }
  if (hasDPLT)
  {
    writeSize = writeSize +4 /* DPLT */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasFTST)
  {
    writeSize = writeSize +4 /* FTST */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (unknownNAM9.isPresent())
  {
    writeSize = writeSize +4 /* NAM9 */ +2 /* 2 bytes for length */ +unknownNAM9.getSize() /* size of subrecord */;
  }
  if (unknownNAMA.isPresent())
  {
    writeSize = writeSize +4 /* NAMA */ +2 /* 2 bytes for length */ +unknownNAMA.getSize() /* size of subrecord */;
  }
  if (unknownTINXs.empty())
  {
    writeSize = writeSize +unknownTINXs.size()*(4 /* TINI */ +2 /* 2 bytes for length */ +2 /* fixed size */
               +4 /* TINC */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* TINV */ +2 /* 2 bytes for length */ +4 /* fixed size */
               +4 /* TIAS */ +2 /* 2 bytes for length */ +2 /* fixed size */);
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

  if (hasTPLT)
  {
    //write TPLT
    output.write((const char*) &cTPLT, 4);
    //TPLT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write TPLT
    output.write((const char*) &unknownTPLT, 4);
  } //if TPLT

  //write RNAM
  output.write((const char*) &cRNAM, 4);
  //RNAM's length
  subLength = 4; //fixed size
  output.write((const char*) &subLength, 2);
  //write RNAM
  output.write((const char*) &unknownRNAM, 4);

  if (hasDEST)
  {
    //write DEST
    output.write((const char*) &cDEST, 4);
    //DEST's length
    subLength = 8; //fixed size
    output.write((const char*) &subLength, 2);
    //write DEST
    output.write((const char*) &unknownDEST, 8);
  }//if DEST

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

  if (hasANAM)
  {
    //write ANAM
    output.write((const char*) &cANAM, 4);
    //ANAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write ANAM
    output.write((const char*) &unknownANAM, 4);
  }//if ANAM

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

  if (unknownATKD.isPresent())
  {
    //write ATKD
    if (!unknownATKD.saveToStream(output, cATKD))
    {
      std::cout << "Error while writing subrecord ATKD of NPC_!\n!";
      return false;
    }
  }

  if (!unknownATKE.empty())
  {
    //write ATKE
    output.write((const char*) &cATKE, 4);
    //ATKE's length
    subLength = unknownATKE.length()+1;
    output.write((const char*) &subLength, 2);
    //write ATKE
    output.write(unknownATKE.c_str(), subLength);
  }

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

  if (hasSPOR)
  {
    //write SPOR
    output.write((const char*) &cSPOR, 4);
    //SPOR's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write SPOR
    output.write((const char*) &unknownSPOR, 4);
  }//if SPOR

  if (hasECOR)
  {
    //write ECOR
    output.write((const char*) &cECOR, 4);
    //ECOR's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write ECOR
    output.write((const char*) &unknownECOR, 4);
  }//if ECOR

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

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write FULL
    output.write((const char*) &nameStringID, 4);
  }//if FULL

  if (hasSHRT)
  {
    //write SHRT
    output.write((const char*) &cSHRT, 4);
    //SHRT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write SHRT
    output.write((const char*) &unknownSHRT, 4);
  }//if SHRT

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 0; //fixed size
  output.write((const char*) &subLength, 2);
  //zero size - nothing to write

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 52; //fixed size
  output.write((const char*) &subLength, 2);
  //write DNAM
  output.write((const char*) unknownDNAM, 52);

  if (!unknownPNAMs.empty())
  {
    unsigned int i;
    for (i=0; i<unknownPNAMs.size(); ++i)
    {
      //write PNAM
      output.write((const char*) &cPNAM, 4);
      //PNAM's length
      subLength = 4; //fixed size
      output.write((const char*) &subLength, 2);
      //write PNAM
      output.write((const char*) &(unknownPNAMs[i]), 4);
    }//for
  }

  if (hasHCLF)
  {
    //write HCLF
    output.write((const char*) &cHCLF, 4);
    //HCLF's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write HCLF
    output.write((const char*) &unknownHCLF, 4);
  }//if HCLF

  if (hasGNAM)
  {
    //write GNAM
    output.write((const char*) &cGNAM, 4);
    //GNAM's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write GNAM
    output.write((const char*) &unknownGNAM, 4);
  }//if GNAM

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

  if (hasDOFT)
  {
    //write DOFT
    output.write((const char*) &cDOFT, 4);
    //DOFT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write DOFT
    output.write((const char*) &unknownDOFT, 4);
  }//if DOFT

  if (hasSOFT)
  {
    //write SOFT
    output.write((const char*) &cSOFT, 4);
    //SOFT's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write SOFT
    output.write((const char*) &unknownSOFT, 4);
  }//if SOFT

  if (hasCRIF)
  {
    //write CRIF
    output.write((const char*) &cCRIF, 4);
    //CRIF's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write CRIF
    output.write((const char*) &unknownCRIF, 4);
  }//if CRIF

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

  if (!unknownCSDXs.empty())
  {
    unsigned int i;
    for (i=0; i<unknownCSDXs.size(); ++i)
    {
      //write CSDT
      output.write((const char*) &cCSDT, 4);
      //CSDT's length
      subLength = 4; //fixed size
      output.write((const char*) &subLength, 2);
      //write CSDT
      output.write((const char*) &(unknownCSDXs[i].unknownCSDT), 4);

      //write CSDI
      output.write((const char*) &cCSDI, 4);
      //CSDI's length
      subLength = 4; //fixed size
      output.write((const char*) &subLength, 2);
      //write CSDI
      output.write((const char*) &(unknownCSDXs[i].unknownCSDI), 4);

      //write CSDC
      output.write((const char*) &cCSDC, 4);
      //CSDC's length
      subLength = 1; //fixed size
      output.write((const char*) &subLength, 2);
      //write CSDC
      output.write((const char*) &(unknownCSDXs[i].unknownCSDC), 1);
    }//for
  }//if CSD? stuff

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

  if (hasFTST)
  {
    //write FTST
    output.write((const char*) &cFTST, 4);
    //FTST's length
    subLength = 4; //fixed size
    output.write((const char*) &subLength, 2);
    //write FTST
    output.write((const char*) &unknownFTST, 4);
  }//if FTST

  //write QNAM
  output.write((const char*) &cQNAM, 4);
  //QNAM's length
  subLength = 12; //fixed size
  output.write((const char*) &subLength, 2);
  //write QNAM
  output.write((const char*) unknownQNAM, 12);

  if (unknownNAM9.isPresent())
  {
    //write NAM9
    if (!unknownNAM9.saveToStream(output, cNAM9))
    {
      std::cout << "Error while writing subrecord NAM9 of NPC_!\n!";
      return false;
    }
  }

  if (unknownNAMA.isPresent())
  {
    //write NAMA
    if (!unknownNAMA.saveToStream(output, cNAMA))
    {
      std::cout << "Error while writing subrecord NAMA of NPC_!\n!";
      return false;
    }
  }

  if (!unknownTINXs.empty())
  {
    unsigned int count = unknownTINXs.size();
    unsigned int i;
    for (i=0; i<count; ++i)
    {
      //write TINI
      output.write((const char*) &cTINI, 4);
      //TINI's length
      subLength = 2; //fixed size
      output.write((const char*) &subLength, 2);
      //write TINI
      output.write((const char*) &(unknownTINXs[i].unknownTINI), 2);

      //write TINC
      output.write((const char*) &cTINC, 4);
      //TINC's length
      subLength = 4; //fixed size
      output.write((const char*) &subLength, 2);
      //write TINC
      output.write((const char*) &(unknownTINXs[i].unknownTINC), 4);

      //write TINV
      output.write((const char*) &cTINV, 4);
      //TINV's length
      subLength = 4; //fixed size
      output.write((const char*) &subLength, 2);
      //write TINV
      output.write((const char*) &(unknownTINXs[i].unknownTINV), 4);

      //write TIAS
      output.write((const char*) &cTIAS, 4);
      //TIAS's length
      subLength = 2; //fixed size
      output.write((const char*) &subLength, 2);
      //write TIAS
      output.write((const char*) &(unknownTINXs[i].unknownTIAS), 2);
    }//for
  }//if TIXX stuff

  return output.good();
}
#endif

bool NPCRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  uint32_t subRecName;
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
    std::cout <<"Error: sub record EDID of NPC_ is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  actual_in->read(buffer, subLength);
  bytesRead += subLength;
  if (!actual_in->good())
  {
    std::cout << "Error while reading subrecord EDID of NPC_!\n";
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
  hasTPLT = false; unknownTPLT = 0;
  bool hasReadRNAM = false; unknownRNAM = 0;
  hasDEST = false; unknownDEST = 0;
  hasWNAM = false; unknownWNAM = 0;
  hasANAM = false; unknownANAM = 0;
  hasATKR = false; unknownATKR = 0;
  unknownATKD.setPresence(false);
  unknownATKE.clear();
  spellFormIDs.clear();
  uint32_t tempUint32, size_int;
  perkList.clear();
  PerkElem tempPerk;
  items.clear();
  ComponentData tempComponent;
  unsigned int i;
  hasSPOR = false; unknownSPOR = 0;
  hasECOR = false; unknownECOR = 0;
  bool hasReadAIDT = false;
  unknownPKIDs.clear();
  keywordArray.clear();
  bool hasReadCNAM = false; unknownCNAM = 0;
  hasFULL = false; nameStringID = 0;
  hasSHRT = false; unknownSHRT = 0;
  bool hasReadDNAM = false;
  unknownPNAMs.clear();
  hasHCLF = false; unknownHCLF = 0;
  hasGNAM = false; unknownGNAM = 0;
  hasZNAM = false; unknownZNAM = 0;
  bool hasReadNAM5 = false; unknownNAM5 = 0;
  bool hasReadNAM6 = false; unknownNAM6 = 0;
  bool hasReadNAM7 = false; unknownNAM7 = 0;
  bool hasReadNAM8 = false; unknownNAM8 = 0;
  hasDOFT = false; unknownDOFT = 0;
  hasSOFT = false; unknownSOFT = 0;
  hasCSCR = false; unknownCSCR = 0;
  hasCRIF = false; unknownCRIF = 0;
  unknownCSDXs.clear();
  CSDXstruct tempCSDX;
  hasDPLT = false; unknownDPLT = 0;
  hasFTST = false; unknownFTST = 0;
  bool hasReadQNAM = false;
  unknownNAM9.setPresence(false);
  unknownNAMA.setPresence(false);
  unknownTINXs.clear();
  TINXstructure tempTINX;
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
           //read INAM
           if (!loadUint32SubRecordFromStream(*actual_in, cINAM, unknownINAM, false)) return false;
           bytesRead += 6;
           hasINAM = true;
           break;
      case cVTCK:
           if (hasVTCK)
           {
             std::cout << "Error: NPC_ seems to have more than one VTCK subrecord.\n";
             return false;
           }
           //read VTCK
           if (!loadUint32SubRecordFromStream(*actual_in, cVTCK, unknownVTCK, false)) return false;
           bytesRead += 6;
           hasVTCK = true;
           break;
      case cTPLT:
           if (hasTPLT)
           {
             std::cout << "Error: NPC_ seems to have more than one TPLT subrecord.\n";
             return false;
           }
           //read TPLT
           if (!loadUint32SubRecordFromStream(*actual_in, cTPLT, unknownTPLT, false)) return false;
           bytesRead += 6;
           hasTPLT = true;
           break;
      case cRNAM:
           if (hasReadRNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one RNAM subrecord.\n";
             return false;
           }
           //read RNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cRNAM, unknownRNAM, false)) return false;
           bytesRead += 6;
           hasReadRNAM = true;
           break;
      case cDEST:
           if (hasDEST)
           {
             std::cout << "Error: NPC_ seems to have more than one DEST subrecord.\n";
             return false;
           }
           //DEST's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record DEST of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be eight bytes.\n";
             return false;
           }
           //read DEST
           actual_in->read((char*) &unknownDEST, 8);
           bytesRead += 8;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord DEST of NPC_!\n";
             return false;
           }
           hasDEST = true;
           break;
      case cWNAM:
           if (hasWNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one WNAM subrecord.\n";
             return false;
           }
           //read WNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cWNAM, unknownWNAM, false)) return false;
           bytesRead += 6;
           hasWNAM = true;
           break;
      case cANAM:
           if (hasANAM)
           {
             std::cout << "Error: NPC_ seems to have more than one ANAM subrecord.\n";
             return false;
           }
           //read ANAM
           if (!loadUint32SubRecordFromStream(*actual_in, cANAM, unknownANAM, false)) return false;
           bytesRead += 6;
           hasANAM = true;
           break;
      case cATKR:
           if (hasATKR)
           {
             std::cout << "Error: NPC_ seems to have more than one ATKR subrecord.\n";
             return false;
           }
           //read ATKR
           if (!loadUint32SubRecordFromStream(*actual_in, cATKR, unknownATKR, false)) return false;
           bytesRead += 6;
           hasATKR = true;
           break;
      case cATKD:
           if (unknownATKD.isPresent())
           {
             std::cout << "Error: NPC_ seems to have more than one ATKD subrecord.\n";
             return false;
           }
           //read ATKD
           if (!unknownATKD.loadFromStream(*actual_in, cATKD, false))
           {
             std::cout << "Error while reading subrecord ATKD of NPC_!\n";
             return false;
           }
           bytesRead += (2+unknownATKD.getSize());
           if (unknownATKD.getSize()!=44)
           {
             std::cout <<"Error: sub record ATKD of NPC_ has invalid length ("
                       <<unknownATKD.getSize()<<" bytes). Should be 44 bytes.\n";
             return false;
           }
           break;
      case cATKE:
           if (!unknownATKE.empty())
           {
             std::cout << "Error: NPC_ seems to have more than one ATKE subrecord.\n";
             return false;
           }
           //ATKE's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record ATKE of NPC_ is longer than 511 characters!\n";
             return false;
           }
           //read ATKE's stuff
           memset(buffer, 0, 512);
           actual_in->read(buffer, subLength);
           bytesRead += subLength;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord ATKE of NPC_!\n";
             return false;
           }
           unknownATKE = std::string(buffer);
           if (unknownATKE.empty())
           {
             std::cout << "Error: subrecord ATKE of NPC_ is empty.\n";
             return false;
           }
           break;
      case cSPCT:
           if (!spellFormIDs.empty())
           {
             std::cout << "Error: NPC_ seems to have more than one SPCT subrecord.\n";
             return false;
           }
           //read SPCT
           if (!loadUint32SubRecordFromStream(*actual_in, cSPCT, size_int, false)) return false;
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
           //read PRKZ
           if (!loadUint32SubRecordFromStream(*actual_in, cPRKZ, size_int, false)) return false;
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
           //read COCT
           if (!loadUint32SubRecordFromStream(*actual_in, cCOCT, tempUint32, false)) return false;
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
      case cSPOR:
           if (hasSPOR)
           {
             std::cout << "Error: NPC_ seems to have more than one SPOR subrecord.\n";
             return false;
           }
           //read SPOR
           if (!loadUint32SubRecordFromStream(*actual_in, cSPOR, unknownSPOR, false)) return false;
           bytesRead += 6;
           hasSPOR = true;
           break;
      case cECOR:
           if (hasECOR)
           {
             std::cout << "Error: NPC_ seems to have more than one ECOR subrecord.\n";
             return false;
           }
           //read ECOR
           if (!loadUint32SubRecordFromStream(*actual_in, cECOR, unknownECOR, false)) return false;
           bytesRead += 6;
           hasECOR = true;
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
           //read KSIZ
           if (!loadUint32SubRecordFromStream(*actual_in, cKSIZ, size_int, false)) return false;
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
           //read CNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cCNAM, unknownCNAM, false)) return false;
           bytesRead += 6;
           hasReadCNAM = true;
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: NPC_ seems to have more than one FULL subrecord.\n";
             return false;
           }
           //read FULL
           if (!loadUint32SubRecordFromStream(*actual_in, cFULL, nameStringID, false)) return false;
           bytesRead += 6;
           hasFULL = true;
           break;
      case cSHRT:
           if (hasSHRT)
           {
             std::cout << "Error: NPC_ seems to have more than one SHRT subrecord.\n";
             return false;
           }
           //read SHRT
           if (!loadUint32SubRecordFromStream(*actual_in, cSHRT, unknownSHRT, false)) return false;
           bytesRead += 6;
           hasSHRT = true;
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
      case cPNAM:
           //PNAM's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record PNAM of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be four bytes.\n";
             return false;
           }
           //read PNAM
           tempUint32 = 0;
           actual_in->read((char*) &tempUint32, 4);
           bytesRead += 4;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord PNAM of NPC_!\n";
             return false;
           }
           unknownPNAMs.push_back(tempUint32);
           break;
      case cHCLF:
           if (hasHCLF)
           {
             std::cout << "Error: NPC_ seems to have more than one HCLF subrecord.\n";
             return false;
           }
           //read HCLF
           if (!loadUint32SubRecordFromStream(*actual_in, cHCLF, unknownHCLF, false)) return false;
           bytesRead += 6;
           hasHCLF = true;
           break;
      case cGNAM:
           if (hasGNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one GNAM subrecord.\n";
             return false;
           }
           //read GNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cGNAM, unknownGNAM, false)) return false;
           bytesRead += 6;
           hasGNAM = true;
           break;
      case cZNAM:
           if (hasZNAM)
           {
             std::cout << "Error: NPC_ seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           //read ZNAM
           if (!loadUint32SubRecordFromStream(*actual_in, cZNAM, unknownZNAM, false)) return false;
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
           //read NAM6
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM6, unknownNAM6, false)) return false;
           bytesRead += 6;
           hasReadNAM6 = true;
           break;
      case cNAM7:
           if (hasReadNAM7)
           {
             std::cout << "Error: NPC_ seems to have more than one NAM7 subrecord.\n";
             return false;
           }
           //read NAM7
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM7, unknownNAM7, false)) return false;
           bytesRead += 6;
           hasReadNAM7 = true;
           break;
      case cNAM8:
           if (hasReadNAM8)
           {
             std::cout << "Error: NPC_ seems to have more than one NAM8 subrecord.\n";
             return false;
           }
           //read NAM8
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM8, unknownNAM8, false)) return false;
           bytesRead += 6;
           hasReadNAM8 = true;
           break;
      case cDOFT:
           if (hasDOFT)
           {
             std::cout << "Error: NPC_ seems to have more than one DOFT subrecord.\n";
             return false;
           }
           //read DOFT
           if (!loadUint32SubRecordFromStream(*actual_in, cDOFT, unknownDOFT, false)) return false;
           bytesRead += 6;
           hasDOFT = true;
           break;
      case cSOFT:
           if (hasSOFT)
           {
             std::cout << "Error: NPC_ seems to have more than one SOFT subrecord.\n";
             return false;
           }
           //read SOFT
           if (!loadUint32SubRecordFromStream(*actual_in, cSOFT, unknownSOFT, false)) return false;
           bytesRead += 6;
           hasSOFT = true;
           break;
      case cCRIF:
           if (hasCRIF)
           {
             std::cout << "Error: NPC_ seems to have more than one CRIF subrecord.\n";
             return false;
           }
           //read CRIF
           if (!loadUint32SubRecordFromStream(*actual_in, cCRIF, unknownCRIF, false)) return false;
           bytesRead += 6;
           hasCRIF = true;
           break;
      case cCSCR:
           if (hasCSCR)
           {
             std::cout << "Error: NPC_ seems to have more than one CSCR subrecord.\n";
             return false;
           }
           //read CSCR
           if (!loadUint32SubRecordFromStream(*actual_in, cCSCR, unknownCSCR, false)) return false;
           bytesRead += 6;
           hasCSCR = true;
           break;
      case cCSDT:
           //read CSDT
           if (!loadUint32SubRecordFromStream(*actual_in, cCSDT, tempCSDX.unknownCSDT, false)) return false;
           bytesRead += 6;

           //read TINC
           if (!loadUint32SubRecordFromStream(*actual_in, cCSDI, tempCSDX.unknownCSDI, true)) return false;
           bytesRead += 10;

           //read CSDC
           actual_in->read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cCSDC)
           {
             UnexpectedRecord(cCSDC, subRecName);
             return false;
           }
           //CSDC's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record CSDC of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be one byte.\n";
             return false;
           }
           //read CSDC
           actual_in->read((char*) &(tempCSDX.unknownCSDC), 1);
           bytesRead += 1;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord CSDC of NPC_!\n";
             return false;
           }
           unknownCSDXs.push_back(tempCSDX);
           break;
      case cDPLT:
           if (hasDPLT)
           {
             std::cout << "Error: NPC_ seems to have more than one DPLT subrecord.\n";
             return false;
           }
           //read DPLT
           if (!loadUint32SubRecordFromStream(*actual_in, cDPLT, unknownDPLT, false)) return false;
           bytesRead += 6;
           hasDPLT = true;
           break;
      case cFTST:
           if (hasFTST)
           {
             std::cout << "Error: NPC_ seems to have more than one FTST subrecord.\n";
             return false;
           }
           //read FTST
           if (!loadUint32SubRecordFromStream(*actual_in, cFTST, unknownFTST, false)) return false;
           bytesRead += 6;
           hasFTST = true;
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
      case cNAM9:
           if (unknownNAM9.isPresent())
           {
             std::cout << "Error: NPC_ seems to have more than one NAM9 subrecord.\n";
             return false;
           }
           //read NAM9
           if (!unknownNAM9.loadFromStream(*actual_in, cNAM9, false))
           {
             std::cout << "Error while reading subrecord NAM9 of NPC_!\n";
             return false;
           }
           bytesRead += (2+unknownNAM9.getSize());
           if (unknownNAM9.getSize()!=76)
           {
             std::cout <<"Error: sub record NAM9 of NPC_ has invalid length ("
                       <<unknownNAM9.getSize()<<" bytes). Should be 76 bytes.\n";
             return false;
           }
           break;
      case cNAMA:
           if (unknownNAMA.isPresent())
           {
             std::cout << "Error: NPC_ seems to have more than one NAMA subrecord.\n";
             return false;
           }
           //read NAMA
           if (!unknownNAMA.loadFromStream(*actual_in, cNAMA, false))
           {
             std::cout << "Error while reading subrecord NAMA of NPC_!\n";
             return false;
           }
           bytesRead += (2+unknownNAMA.getSize());
           if (unknownNAMA.getSize()!=16)
           {
             std::cout <<"Error: sub record NAMA of NPC_ has invalid length ("
                       <<unknownNAM9.getSize()<<" bytes). Should be 16 bytes.\n";
             return false;
           }
           break;
      case cTINI:
           //TINI's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record TINI of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be two bytes.\n";
             return false;
           }
           //read TINI
           tempTINX.unknownTINI = 0;
           actual_in->read((char*) &tempTINX.unknownTINI, 2);
           bytesRead += 2;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord TINI of NPC_!\n";
             return false;
           }

           //read TINC
           if (!loadUint32SubRecordFromStream(*actual_in, cTINC, tempTINX.unknownTINC, true)) return false;
           bytesRead += 10;

           //read TINV
           if (!loadUint32SubRecordFromStream(*actual_in, cTINV, tempTINX.unknownTINV, true)) return false;
           bytesRead += 10;

           //read TIAS
           actual_in->read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cTIAS)
           {
             UnexpectedRecord(cTIAS, subRecName);
             return false;
           }
           //TIAS's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record TIAS of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be two bytes.\n";
             return false;
           }
           //read TIAS
           actual_in->read((char*) &(tempTINX.unknownTIAS), 2);
           bytesRead += 2;
           if (!actual_in->good())
           {
             std::cout << "Error while reading subrecord TIAS of NPC_!\n";
             return false;
           }
           unknownTINXs.push_back(tempTINX);
           break;
      case cDSTD:
           /* We just skip all the DSTD/DSTF stuff here. */
           //DSTD's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=20)
           {
             std::cout <<"Error: sub record DSTD of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be 20 bytes.\n";
             return false;
           }
           //skip DSTD
           actual_in->seekg(20, std::ios_base::cur);
           bytesRead += 20;

           //read DSTF
           actual_in->read((char*) &subRecName, 4);
           bytesRead += 4;
           if (subRecName!=cDSTF)
           {
             UnexpectedRecord(cDSTF, subRecName);
             return false;
           }
           //DSTF's length
           actual_in->read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout <<"Error: sub record DSTF of NPC_ has invalid length ("
                       <<subLength<<" bytes). Should be zero bytes.\n";
             return false;
           }
           //nothing to skip, DSTF has length zero
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, ACBS, SNAM, INAM, VTCK, TPLT,"
                     << " RNAM, DEST, WNAM, ANAM, ATKR, ATKD, ATKE, SPCT, PRKZ,"
                     << " CNTO, ECOR, AIDT, KSIZ, CNAM, FULL, SHRT, DATA, PNAM,"
                     << " HCLF, GNAM, ZNAM, NAM5, NAM6, NAM7, NAM8, DPLT, DOFT,"
                     << " SOFT, CRIF, CSCR, CSDT, FTST, QNAM, NAM9, NAMA or TINI are allowed here!\n";
           return false;
    }//swi
  }//while

  //check presence of all required subrecords
  if (!(hasReadOBND and hasReadACBS and hasReadRNAM and hasReadAIDT
        and hasReadCNAM and hasReadDNAM and hasReadNAM5 and hasReadNAM6
        and hasReadNAM7 and hasReadNAM8 and hasReadQNAM))
  {
    std::cout << "Error: At least one required subrecord of NPC_ was not found!\n";
    return false;
  }

  return actual_in->good();
}

uint32_t NPCRecord::getRecordType() const
{
  return cNPC_;
}

} //namespace
