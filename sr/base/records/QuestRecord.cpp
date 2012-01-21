/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "QuestRecord.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* functions of index entry structure */

QuestRecord::IndexEntry::IndexEntry()
{
  index = 0;
  theQSDTs.clear();
}

bool QuestRecord::IndexEntry::operator==(const QuestRecord::IndexEntry& other) const
{
  return ((index==other.index) and (indexUnknownPart==other.indexUnknownPart)
      and (theQSDTs==other.theQSDTs));
}

/* QSDTRecord functions */
QuestRecord::IndexEntry::QSDTRecord::QSDTRecord()
{
  isFinisher = false;
  hasNAM0 = false;
  unknownNAM0 = 0;
  unknownSCHR.setPresence(false);
  unknownSCTX = "";
  hasQNAM = false;
  unknownQNAM = 0;
  unknownCTDA_CIS2s.clear();
  hasCNAM = false;
  unknownCNAM = 0;
}

bool QuestRecord::IndexEntry::QSDTRecord::operator==(const QuestRecord::IndexEntry::QSDTRecord& other) const
{
  return ((isFinisher==other.isFinisher) and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s)
      and (hasNAM0==other.hasNAM0) and ((unknownNAM0==other.unknownNAM0) or (!hasNAM0))
      and (hasQNAM==other.hasQNAM) and ((unknownQNAM==other.unknownQNAM) or (!hasQNAM))
      and (hasCNAM==other.hasCNAM) and ((unknownCNAM==other.unknownCNAM) or (!hasCNAM))
      and (unknownSCHR==other.unknownSCHR) and (unknownSCTX==other.unknownSCTX));
}

/* AliasEntry's functions */

QuestRecord::AliasEntry::AliasEntry()
{
  unknownALST = 0;
  aliasID = ""; //subrecord ALID
  clear();
}

bool QuestRecord::AliasEntry::operator==(const QuestRecord::AliasEntry& other) const
{
  return ((hasALLS==other.hasALLS) and ((unknownALLS==other.unknownALLS) or (!hasALLS))
      and (hasALST==other.hasALST) and ((unknownALST==other.unknownALST) or (!hasALST))
      and (aliasID==other.aliasID) and (unknownFNAM==other.unknownFNAM)
      and (hasALFA==other.hasALFA) and ((unknownALFA==other.unknownALFA) or (!hasALFA))
      and (hasALRT==other.hasALRT) and ((unknownALRT==other.unknownALRT) or (!hasALRT))
      and (hasALCO==other.hasALCO) and ((unknownALCO==other.unknownALCO) or (!hasALCO))
      and (hasALCA==other.hasALCA) and ((unknownALCA==other.unknownALCA) or (!hasALCA))
      and (hasALCL==other.hasALCL) and ((unknownALCL==other.unknownALCL) or (!hasALCL))
      and (hasALDN==other.hasALDN) and ((unknownALDN==other.unknownALDN) or (!hasALDN))
      and (components==other.components) and (keywordArray==other.keywordArray)
      and (hasALFE==other.hasALFE) and ((unknownALFE==other.unknownALFE) or (!hasALFE))
      and (hasALFD==other.hasALFD) and ((unknownALFD==other.unknownALFD) or (!hasALFD))
      and (hasALFI==other.hasALFI) and ((unknownALFI==other.unknownALFI) or (!hasALFI))
      and (hasALFL==other.hasALFL) and ((unknownALFL==other.unknownALFL) or (!hasALFL))
      and (hasALFR==other.hasALFR) and ((unknownALFR==other.unknownALFR) or (!hasALFR))
      and (hasALNA==other.hasALNA) and ((unknownALNA==other.unknownALNA) or (!hasALNA))
      and (hasALNT==other.hasALNT) and ((unknownALNT==other.unknownALNT) or (!hasALNT))
      and (hasALUA==other.hasALUA) and ((unknownALUA==other.unknownALUA) or (!hasALUA))
      and (hasALEQ==other.hasALEQ) and ((unknownALEQ==other.unknownALEQ) or (!hasALEQ))
      and (hasALEA==other.hasALEA) and ((unknownALEA==other.unknownALEA) or (!hasALEA))
      and (unknownALSPs==other.unknownALSPs)
      and (hasKNAM==other.hasKNAM) and ((unknownKNAM==other.unknownKNAM) or (!hasKNAM))
      and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s)
      and (hasSPOR==other.hasSPOR) and ((unknownSPOR==other.unknownSPOR) or (!hasSPOR))
      and (hasECOR==other.hasECOR) and ((unknownECOR==other.unknownECOR) or (!hasECOR))
      and (unknownALFCs==other.unknownALFCs)
      and (unknownALPCs==other.unknownALPCs)
      and (hasVTCK==other.hasVTCK) and ((unknownVTCK==other.unknownVTCK) or (!hasVTCK)));
}

void QuestRecord::AliasEntry::clear()
{
  hasALST = false;
  unknownALST = 0;
  hasALLS = false;
  unknownALLS = 0;
  aliasID.clear(); //subrecord ALID
  unknownFNAM = 0;
  hasALFA = false;
  unknownALFA = 0;
  hasALRT = false;
  unknownALRT = 0;
  hasALCO = false;
  unknownALCO = 0;
  hasALCA = false;
  unknownALCA = 0;
  hasALCL = false;
  unknownALCL = 0;
  hasALDN = false;
  unknownALDN = 0;
  components.clear();
  keywordArray.clear();
  hasALFE = false;
  unknownALFE = 0;
  hasALFD = false;
  unknownALFD = 0;
  hasALFI = false;
  unknownALFI = 0;
  hasALFL = false;
  unknownALFL = 0;
  hasALFR = false;
  unknownALFR = 0;
  hasALNA = false;
  unknownALNA = 0;
  hasALNT = false;
  unknownALNT = 0;
  hasALUA = false;
  unknownALUA = 0;
  hasALEQ = false;
  unknownALEQ = 0;
  hasALEA = false;
  unknownALEA = 0;
  hasKNAM = false;
  unknownKNAM = 0;
  unknownCTDA_CIS2s.clear();
  hasSPOR = false;
  unknownSPOR = 0;
  hasECOR = false;
  unknownECOR = 0;
  unknownALSPs.clear();
  unknownALFCs.clear();
  unknownALPCs.clear();
  hasVTCK = false;
  unknownVTCK = 0;
}

/*bool QuestRecord::AliasEntry::loadFromStream(std::ifstream& in_File, uint32_t& bytesRead, char * buffer)
{
  //ALST is always first
  in_File.seekg(-4, std::ios_base::cur);
  //read ALST
  if (!loadUint32SubRecordFromStream(in_File, cALST, unknownALST)) return false;
  bytesRead += 6;

  bool toDo = false;
  int32_t subRecName = 0;
  uint16_t subLength = 0;

  aliasID.clear();
  bool hasReadFNAM = false;
  hasALFE = false; unknownALFE = 0;
  hasALFD = false; unknownALFD = 0;
  hasALFR = false; unknownALFR = 0;
  unknownCTDAs.clear();
  CTDAData tempCTDA;
  bool hasVTCK = false; unknownVTCK = 0;
  while (toDo)
  {
    //read next subrecord
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cALID:
           if (!aliasID.empty())
           {
             std::cout << "Error: QUST seems to have more than one ALID subrecord per alias structure.\n";
             return false;
           }
           //ALID's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record ALID of QUST is longer than 511 characters!\n";
             return false;
           }
           //read ALID's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord ALID of QUST!\n";
             return false;
           }
           aliasID = std::string(buffer);
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cout << "Error: QUST seems to have more than one FNAM subrecord per alias structure.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, unknownFNAM)) return false;
           bytesRead += 6;
           hasReadFNAM = true;
           break;
      case cALFE:
           if (hasALFE)
           {
             std::cout << "Error: QUST seems to have more than one ALFE subrecord per alias structure.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read ALFE
           if (!loadUint32SubRecordFromStream(in_File, cALFE, unknownALFE)) return false;
           bytesRead += 6;
           hasALFE = true;
           break;
      case cALFD:
           if (hasALFD)
           {
             std::cout << "Error: QUST seems to have more than one ALFD subrecord per alias structure.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read ALFD
           if (!loadUint32SubRecordFromStream(in_File, cALFD, unknownALFD)) return false;
           bytesRead += 6;
           hasALFD = true;
           break;
      case cALFR:
           if (hasALFR)
           {
             std::cout << "Error: QUST seems to have more than one ALFR subrecord per alias structure.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read ALFR
           if (!loadUint32SubRecordFromStream(in_File, cALFR, unknownALFR)) return false;
           bytesRead += 6;
           hasALFR = true;
           break;
      case cCTDA:
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cout << "Error while reading subrecord CTDA of QUST!\n";
             return false;
           }
           unknownCTDAs.push_back(tempCTDA);
           break;
      case cVTCK:
           if (hasReadVTCK)
           {
             std::cout << "Error: QUST seems to have more than one VTCK subrecord per alias structure.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read VTCK
           if (!loadUint32SubRecordFromStream(in_File, cVTCK, unknownVTCK)) return false;
           bytesRead += 6;
           hasReadVTCK = true;
           break;
      case cALED:
           //end marker
           //ALED's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout <<"Error: sub record ALED of QUST has invalid length ("
                       <<subLength<<" bytes). Should be zero bytes!\n";
             return false;
           }
           toDo = false;
           break;
      default:
           std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only ALID, FNAM, ALFE, ALFD, ALFR, CTDA, VTCK or ALED are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadFNAM and hasReadVTCK))
  {
    std::cout << "Error: at least one required subrecord of alias structure in QUST is missing!\n";
    return false;
  }

  return in_File.good();
} */

/* QOBJEntry's functions */

QuestRecord::QOBJEntry::QOBJEntry()
{
  unknownQOBJ = 0;
  unknownFNAM = 0;
  unknownNNAM = 0;
  theQSTAs.clear();
}

void QuestRecord::QOBJEntry::clear()
{
  unknownQOBJ = 0;
  unknownFNAM = 0;
  unknownNNAM = 0;
  theQSTAs.clear();
}

bool QuestRecord::QOBJEntry::operator==(const QuestRecord::QOBJEntry& other) const
{
  return ((unknownQOBJ==other.unknownQOBJ) and (unknownFNAM==other.unknownFNAM)
      and (unknownNNAM==other.unknownNNAM) and (theQSTAs==other.theQSTAs));
}

/* QSTAEntry's functions */

QuestRecord::QOBJEntry::QSTAEntry::QSTAEntry()
{
  unknownQSTA = 0;
  unknownCTDA_CIS2s.clear();
}

void QuestRecord::QOBJEntry::QSTAEntry::clear()
{
  unknownQSTA = 0;
  unknownCTDA_CIS2s.clear();
}

bool QuestRecord::QOBJEntry::QSTAEntry::operator==(const QuestRecord::QOBJEntry::QSTAEntry& other) const
{
  return ((unknownQSTA==other.unknownQSTA) and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s));
}

/* quest record's functions */

QuestRecord::QuestRecord()
{
  editorID = "";
  unknownVMAD.setPresence(false);
  unknownFULL = 0;
  memset(unknownDNAM, 0, 12);
  hasENAM = false;
  unknownENAM = 0;
  unknownQTGLs.clear();
  unknownCTDA_CIS2s.clear();
  filter = "";
  indices.clear();
  theQOBJs.clear();
  unknownANAM = 0;
  aliases.clear();
}

QuestRecord::~QuestRecord()
{
  //empty
}

bool QuestRecord::equals(const QuestRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
    and (unknownVMAD==other.unknownVMAD)
    and (hasFULL==other.hasFULL) and ((unknownFULL==other.unknownFULL) or (!hasFULL))
    and (memcmp(unknownDNAM, other.unknownDNAM, 12)==0)
    and (hasENAM==other.hasENAM) and ((unknownENAM==other.unknownENAM) or (!hasENAM))
    and (unknownQTGLs==other.unknownQTGLs) and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s)
    and (filter==other.filter) and (indices==other.indices) and (theQOBJs==other.theQOBJs)
    and (unknownANAM==other.unknownANAM) and (aliases==other.aliases));
}

uint32_t QuestRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of string +1 byte for NUL-termination */
        +4 /* DNAM */ +2 /* 2 bytes for length */ +12 /* fixed size */
        +4 /* NEXT */ +2 /* 2 bytes for length */ +0 /* fixed size */
        +4 /* ANAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize +4 /* VMAD */ +2 /* 2 bytes for length */ +unknownVMAD.getSize();
  }
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (hasENAM)
  {
    writeSize = writeSize +4 /* ENAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
  }
  if (!unknownQTGLs.empty())
  {
    writeSize = writeSize +unknownQTGLs.size()*(4 /* QTGL */ +2 /* 2 bytes for length */ +4 /* fixed size */);
  }
  if (!unknownCTDA_CIS2s.empty())
  {
    unsigned int i;
    const unsigned int count = unknownCTDA_CIS2s.size();
    for (i=0; i<count; ++i)
    {
      writeSize = writeSize +4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed size */;
      if (!unknownCTDA_CIS2s[i].unknownCIS2.empty())
      {
        writeSize = writeSize +4 /* CIS2 */ +2 /* 2 bytes for length */
            +unknownCTDA_CIS2s[i].unknownCIS2.length()+1 /* length of string +1 byte for NUL-termination */;
      }
    }//for
  }
  if (!filter.empty())
  {
    writeSize = writeSize +4 /* FLTR */ +2 /* 2 bytes for length */
        +filter.length()+1 /* length of string +1 byte for NUL-termination */;
  }
  if (!indices.empty())
  {
    unsigned int i, j, k;
    const unsigned int idx_count = indices.size();
    for (i=0; i<idx_count; ++i)
    {
      writeSize = writeSize +4 /* INDX */ +2 /* 2 bytes for length */ +4 /* fixed size */;
      const unsigned int q_size = indices[i].theQSDTs.size();
      for (j=0; j<q_size; ++j)
      {
        writeSize = writeSize +4 /* QSDT */ +2 /* 2 bytes for length */ +1 /* fixed size */;
        if (indices[i].theQSDTs[j].hasNAM0)
        {
          writeSize = writeSize +4 /* NAM0 */ +2 /* 2 bytes for length */ +4 /* fixed size */;
        }
        if (indices[i].theQSDTs[j].unknownSCHR.isPresent())
        {
          writeSize = writeSize +4 /* SCHR */ +2 /* 2 bytes for length */ +indices[i].theQSDTs[j].unknownSCHR.getSize() /* size */;
        }
        if (!indices[i].theQSDTs[j].unknownSCTX.empty())
        {
          writeSize = writeSize +4 /* SCTX */ +2 /* 2 bytes for length */ +indices[i].theQSDTs[j].unknownSCTX.length() /* length */;
        }
        if (indices[i].theQSDTs[j].hasQNAM)
        {
          writeSize = writeSize +4 /* QNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
        }
        const unsigned int compound_count = indices[i].theQSDTs[j].unknownCTDA_CIS2s.size();
        for (k=0; k<compound_count; ++k)
        {
          writeSize = writeSize +4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed size */;
          if (!indices[i].theQSDTs[j].unknownCTDA_CIS2s[k].unknownCIS2.empty())
          {
            writeSize = writeSize +4 /* CIS2 */ +2 /* 2 bytes for length */
                       +indices[i].theQSDTs[j].unknownCTDA_CIS2s[k].unknownCIS2.length()+1 /* length */;
          }
        }//for k
        if (indices[i].theQSDTs[j].hasCNAM)
        {
          writeSize = writeSize +4 /* CNAM */ +2 /* 2 bytes for length */ +4 /* fixed size */;
        }
      }//for j
    }//for i
  }//indices
  /// ... more to come
  #warning Size of QOBJ and alias are not properly calculated here!
  return writeSize;
}

bool QuestRecord::saveToStream(std::ofstream& output) const
{
  #warning Not completely implemented yet!
  output.write((const char*) &cQUST, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  //write VMAD
  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cout << "Error while writing subrecord VMAD of QUST!\n";
      return false;
    }
  }//if VMAD

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write FULL
    output.write((const char*) &unknownFULL, 4);
  }//if FULL

  //write DNAM
  output.write((const char*) &cDNAM, 4);
  //DNAM's length
  subLength = 12; //fixed size
  output.write((const char*) &subLength, 2);
  //write DNAM stuff
  output.write((const char*) unknownDNAM, 12);

  if (hasENAM)
  {
    //write ENAM
    output.write((const char*) &cENAM, 4);
    //ENAM's length
    subLength = 4; // fixed
    output.write((const char*) &subLength, 2);
    //write ENAM
    output.write((const char*) &unknownENAM, 4);
  }//if ENAM

  if (!unknownQTGLs.empty())
  {
    unsigned int i;
    const unsigned int count = unknownCTDA_CIS2s.size();
    for (i=0; i<count; ++i)
    {
      if (!unknownCTDA_CIS2s[i].unknownCTDA.saveToStream(output))
      {
        std::cout << "Error while writing subrecord CTDA of QUST!\n";
        return false;
      }
      if (!unknownCTDA_CIS2s[i].unknownCIS2.empty())
      {
        //write CIS2
        output.write((const char*) &cCIS2, 4);
        //CIS2's length
        subLength = unknownCTDA_CIS2s[i].unknownCIS2.length()+1;
        output.write((const char*) &subLength, 2);
        //write CIS2
        output.write(unknownCTDA_CIS2s[i].unknownCIS2.c_str(), subLength);
      }
    }//for
  }//if QTGL

  if (!filter.empty())
  {
    //write FLTR
    output.write((const char*) &cFLTR, 4);
    //FLTR's length
    subLength = filter.length()+1;
    output.write((const char*) &subLength, 2);
    //write filter string
    output.write(filter.c_str(), subLength);
  }//if FLTR

  //write NEXT
  output.write((const char*) &cNEXT, 4);
  //NEXT's length
  subLength = 0; //fixed
  output.write((const char*) &subLength, 2);

  if (!indices.empty())
  {
    unsigned int i, j, k;
    const unsigned int idx_count = indices.size();
    for (i=0; i<idx_count; ++i)
    {
      //write INDX
      output.write((const char*) &cINDX, 4);
      //INDX's length
      subLength = 4;
      output.write((const char*) &subLength, 2);
      //write index
      output.write((const char*) &(indices[i].index), 2);
      output.write((const char*) &(indices[i].indexUnknownPart), 2);

      const unsigned int q_size = indices[i].theQSDTs.size();
      for (j=0; j<q_size; ++j)
      {
        //write QSDT
        output.write((const char*) &cQSDT, 4);
        //QSDT's length
        subLength = 1; //fixed
        output.write((const char*) &subLength, 2);
        //write QSTD
        output.write((const char*) &(indices[i].theQSDTs[j].isFinisher), 1);

        if (indices[i].theQSDTs[j].hasNAM0)
        {
          //write NAM0
          output.write((const char*) &cNAM0, 4);
          //NAM0's length
          subLength = 4;
          output.write((const char*) &subLength, 2);
          //write NAM0
          output.write((const char*) &(indices[i].theQSDTs[j].unknownNAM0), 4);
        }
        if (indices[i].theQSDTs[j].unknownSCHR.isPresent())
        {
          if (!indices[i].theQSDTs[j].unknownSCHR.saveToStream(output, cSCHR))
          {
            std::cout << "Error while writing subrecord SCHR of QUST!\n";
            return false;
          }
        }
        if (!indices[i].theQSDTs[j].unknownSCTX.empty())
        {
          //write SCTX
          output.write((const char*) &cSCTX, 4);
          //SCTX's length
          subLength = indices[i].theQSDTs[j].unknownSCTX.length(); //not NUL-terminated
          output.write((const char*) &subLength, 2);
          //write SCTX
          output.write(indices[i].theQSDTs[j].unknownSCTX.c_str(), subLength);
        }
        if (indices[i].theQSDTs[j].hasQNAM)
        {
          //write QNAM
          output.write((const char*) &cQNAM, 4);
          //QNAM's length
          subLength = 4;
          output.write((const char*) &subLength, 2);
          //write QNAM
          output.write((const char*) &(indices[i].theQSDTs[j].unknownQNAM), 4);
        }
        const unsigned int compound_count = indices[i].theQSDTs[j].unknownCTDA_CIS2s.size();
        for (k=0; k<compound_count; ++k)
        {
          if (!indices[i].theQSDTs[j].unknownCTDA_CIS2s[k].unknownCTDA.saveToStream(output))
          {
            std::cout << "Error while writing subrecord CTDA of QUST!\n";
            return false;
          }
          if (!indices[i].theQSDTs[j].unknownCTDA_CIS2s[k].unknownCIS2.empty())
          {
            //write CIS2
            output.write((const char*) &cCIS2, 4);
            //CIS2's length
            subLength = indices[i].theQSDTs[j].unknownCTDA_CIS2s[k].unknownCIS2.length()+1; //length
            output.write((const char*) &subLength, 2);
            //write CIS2
            output.write(indices[i].theQSDTs[j].unknownCTDA_CIS2s[k].unknownCIS2.c_str(), subLength);
          }
        }//for k
        if (indices[i].theQSDTs[j].hasCNAM)
        {
          //write CNAM
          output.write((const char*) &cCNAM, 4);
          //CNAM's length
          subLength = 4;
          output.write((const char*) &subLength, 2);
          //write CNAM
          output.write((const char*) &(indices[i].theQSDTs[j].unknownCNAM), 4);
        }
      }//for j
    }//for i
  }//indices

  //write ANAM
  output.write((const char*) &cANAM, 4);
  //ANAM's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write ANAM
  output.write((const char*) &unknownANAM, 4);

  #warning Code more! (QOBJ and aliases are still missing.)
  return false;
}

bool QuestRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize)) return false;
  int32_t subRecName, lastReadRec;
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
    std::cout <<"Error: sub record EDID of QUST is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of QUST!\n";
    return false;
  }
  editorID = std::string(buffer);

  unknownVMAD.setPresence(false);
  hasFULL = false; unknownFULL = 0;
  bool hasReadDNAM = false;
  memset(unknownDNAM, 0, 12);
  hasENAM = false; unknownENAM = 0;
  unknownQTGLs.clear();
  CTDAData tempCTDA;
  filter.clear();
  indices.clear();
  IndexEntry i_entry;
  bool hasUnpushedIndexEntry = false;
  bool indexPartStarted = false;
  IndexEntry::QSDTRecord tempQSDT;
  bool hasUnpushedQSDTRecord = false;
  theQOBJs.clear();
  QOBJEntry tempQOBJ;
  bool hasUnpushedQOBJEntry = false;
  QOBJEntry::QSTAEntry tempQSTA;
  bool hasUnpushedQSTAEntry = false;
  bool hasReadANAM = false;
  aliases.clear();
  AliasEntry al_entry;
    bool toDo = false;
    bool hasReadFNAM = false;
    uint32_t tempUint32, size_int, i;
    ComponentData tempComp;
  lastReadRec = cEDID;
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
             std::cout << "Error: QUST seems to have more than one VMAD subrecord.\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(in_File, cVMAD, false))
           {
             std::cout << "Error while reading subrecord VMAD of QUST!\n";
             return false;
           }
           bytesRead = bytesRead +2 +unknownVMAD.getSize();
           lastReadRec = cVMAD;
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: QUST seems to have more than one FULL subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read FULL
           if (!loadUint32SubRecordFromStream(in_File, cFULL, unknownFULL)) return false;
           bytesRead += 6;
           hasFULL = true;
           lastReadRec = cFULL;
           break;
      case cDNAM:
           if (hasReadDNAM)
           {
             std::cout << "Error: QUST seems to have more than one DNAM subrecord.\n";
             return false;
           }
           //DNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: sub record DNAM of QUST has invalid length ("<<subLength
                       <<" bytes). Should be 12 bytes.\n";
             return false;
           }
           //read DNAM
           in_File.read((char*) unknownDNAM, 12);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord DNAM of QUST!\n";
             return false;
           }
           hasReadDNAM = true;
           lastReadRec = cDNAM;
           break;
      case cENAM:
           if (hasENAM)
           {
             std::cout << "Error: QUST seems to have more than one ENAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read ENAM
           if (!loadUint32SubRecordFromStream(in_File, cENAM, unknownENAM)) return false;
           bytesRead += 6;
           hasENAM = true;
           lastReadRec = cENAM;
           break;
      case cQTGL:
           in_File.seekg(-4, std::ios_base::cur);
           //read QTGL
           if (!loadUint32SubRecordFromStream(in_File, cQTGL, tempUint32)) return false;
           bytesRead += 6;
           unknownQTGLs.push_back(tempUint32);
           lastReadRec = cQTGL;
           break;
      case cCTDA:
           if (!tempCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cout << "Error while reading subrecord CTDA of QUST!\n";
             return false;
           }
           if (hasUnpushedQSTAEntry)
           {
             //if we are in the middle of a QSTA sequence, push it there
             tempQSTA.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           }
           else if (indexPartStarted)
           {
             if (hasUnpushedQSDTRecord)
             {
               tempQSDT.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
             }
             else
             {
               std::cout << "Error in subrecord sequence of QUST! CTDA after "
                         << "first INDX cannot be processed, if there was no QSDT before it!\n";
               return false;
             }
           }
           else
           {
             unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
           }
           lastReadRec = cCTDA;
           break;
      case cFLTR:
           if (!filter.empty())
           {
             std::cout << "Error: QUST seems to have more than one FLTR subrecord.\n";
             return false;
           }
           //FLTR's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record FLTR of QUST is longer than 511 characters!\n";
             return false;
           }
           //read FLTR's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord FLTR of QUST!\n";
             return false;
           }
           filter = std::string(buffer);
           lastReadRec = cFLTR;
           break;
      case cNEXT:
           //NEXT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=0)
           {
             std::cout <<"Error: sub record NEXT of QUST has invalid length ("<<subLength
                       <<" bytes). Should be zero bytes.\n";
             return false;
           }
           lastReadRec = cNEXT;
           break;
      case cINDX:
           if (hasUnpushedIndexEntry)
           {
             /* Last entry seems to have a index only, or index and finishing
                 bit only, so push it!*/
             indices.push_back(i_entry);
           }
           indexPartStarted = true;
           //INDX's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record INDX of QUST has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read index
           in_File.read((char*) &(i_entry.index), 2);
           in_File.read((char*) &(i_entry.indexUnknownPart), 2);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord INDX of QUST!\n";
             return false;
           }
           //preset for other fields of index entry
           i_entry.theQSDTs.clear();
           hasUnpushedIndexEntry = true;
           lastReadRec = cINDX;
           break;
      case cQSDT:
           if (!hasUnpushedIndexEntry)
           {
             std::cout << "Error in subrecord sequence of QUST! QSDT should "
                       << "follow after INDX, but previous record was \""
                       << IntTo4Char(lastReadRec) <<"\".\n";
             return false;
           }
           if (hasUnpushedQSDTRecord)
           {
             i_entry.theQSDTs.push_back(tempQSDT);
             hasUnpushedQSDTRecord = false;
           }
           tempQSDT.isFinisher = false;
           tempQSDT.hasNAM0 = false;
           tempQSDT.unknownNAM0 = 0;
           tempQSDT.hasQNAM = false;
           tempQSDT.unknownQNAM = 0;
           tempQSDT.unknownSCHR.setPresence(false);
           tempQSDT.unknownCTDA_CIS2s.clear();
           tempQSDT.hasCNAM = false;
           tempQSDT.unknownCNAM = 0;
           //QSDT's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=1)
           {
             std::cout <<"Error: sub record QSDT of QUST has invalid length ("<<subLength
                       <<" bytes). Should be one byte.\n";
             return false;
           }
           //read QSDT
           memset(buffer, 0, 1);
           in_File.read(buffer, 1);
           bytesRead += 1;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord QSDT of QUST!\n";
             return false;
           }
           tempQSDT.isFinisher = (buffer[0]!=0);
           hasUnpushedQSDTRecord = true;
           lastReadRec = cQSDT;
           break;
      case cNAM0:
           if (!hasUnpushedQSDTRecord)
           {
             std::cout << "Error in subrecord sequence of QUST! NAM0 outside "
                       << "of QSDT encountered!\n";
             return false;
           }
           if (tempQSDT.hasNAM0)
           {
             std::cout << "Error: QUST seems to have more than one NAM0 subrecord per QSDT.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read NAM0
           if (!loadUint32SubRecordFromStream(in_File, cNAM0, tempQSDT.unknownNAM0)) return false;
           bytesRead += 6;
           tempQSDT.hasNAM0 = true;
           lastReadRec = cNAM0;
           break;
      case cQNAM:
           if (!hasUnpushedQSDTRecord)
           {
             std::cout << "Error in subrecord sequence of QUST! QNAM outside "
                       << "of QSDT encountered!\n";
             return false;
           }
           if (tempQSDT.hasQNAM)
           {
             std::cout << "Error: QUST seems to have more than one QNAM subrecord per QSDT.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read QNAM
           if (!loadUint32SubRecordFromStream(in_File, cQNAM, tempQSDT.unknownQNAM)) return false;
           bytesRead += 6;
           tempQSDT.hasQNAM = true;
           lastReadRec = cQNAM;
           break;
      case cSCHR:
           if (!hasUnpushedQSDTRecord)
           {
             std::cout << "Error in subrecord sequence of QUST! SCHR outside "
                       << "of QSDT encountered!\n";
             return false;
           }
           if (tempQSDT.unknownSCHR.isPresent())
           {
             std::cout << "Error: QUST seems to have more than one SCHR subrecord per QSDT.\n";
             return false;
           }
           if (!tempQSDT.unknownSCHR.loadFromStream(in_File, cSCHR, false))
           {
             std::cout << "Error while loading subrecord SCHR of QUST!\n";
             return false;
           }
           bytesRead = bytesRead +2 +tempQSDT.unknownSCHR.getSize();
           //check length
           if (tempQSDT.unknownSCHR.getSize()!=20)
           {
             std::cout <<"Error: sub record SCHR of QUST has invalid length ("
                       <<tempQSDT.unknownSCHR.getSize()<<" bytes). Should be 20 bytes.\n";
             return false;
           }
           lastReadRec = cSCHR;
           break;
      case cSCTX:
           if (!hasUnpushedQSDTRecord)
           {
             std::cout << "Error in subrecord sequence of QUST! SCTX outside "
                       << "of QSDT encountered!\n";
             return false;
           }
           if (!tempQSDT.unknownSCTX.empty())
           {
             std::cout << "Error: QUST seems to have more than one SCTX subrecord per QSDT.\n";
             return false;
           }
           //SCTX's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record SCTX of QUST is longer than 511 characters!\n";
             return false;
           }
           //read SCTX's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord SCTX of QUST!\n";
             return false;
           }
           tempQSDT.unknownSCTX = std::string(buffer);
           lastReadRec = cSCTX;
           break;
      case cCNAM:
           if (!hasUnpushedIndexEntry)
           {
             std::cout << "Error in subrecord sequence of QUST! CNAM should "
                       << "follow in index part after QSDT, but previous record was \""
                       << IntTo4Char(lastReadRec) <<"\" and no index was "
                       << "started yet.\n";
             return false;
           }
           if (!hasUnpushedQSDTRecord)
           {
             std::cout << "Error in subrecord sequence of QUST! CNAM should "
                       << "follow in index part after QSDT, but previous record was \""
                       << IntTo4Char(lastReadRec) <<"\" and no QSDT was "
                       << "started yet.\n";
             return false;
           }
           if (tempQSDT.hasCNAM)
           {
             std::cout << "Error: QUST seems to have more than one CNAM subrecord per QSDT.\n";
             return false;
           }
           //CNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=4)
           {
             std::cout <<"Error: sub record CNAM of QUST has invalid length ("<<subLength
                       <<" bytes). Should be four bytes.\n";
             return false;
           }
           //read CNAM
           in_File.read((char*) &(tempQSDT.unknownCNAM), 4);
           bytesRead += 4;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CNAM of QUST!\n";
             return false;
           }
           tempQSDT.hasCNAM = true;
           lastReadRec = cCNAM;
           break;
      case cQOBJ:
           if (hasUnpushedQOBJEntry)
           {
             //push previous entry
             //... and check for QSTA first
             if (hasUnpushedQSTAEntry)
             {
               tempQOBJ.theQSTAs.push_back(tempQSTA);
               hasUnpushedQSTAEntry = false;
             }
             //now push the real record
             theQOBJs.push_back(tempQOBJ);
           }
           tempQOBJ.clear();
           //QOBJ's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=2)
           {
             std::cout <<"Error: sub record QOBJ of QUST has invalid length ("
                       <<subLength <<" bytes). Should be two bytes.\n";
             return false;
           }
           //read index
           in_File.read((char*) &(tempQOBJ.unknownQOBJ), 2);
           bytesRead += 2;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord QOBJ of QUST!\n";
             return false;
           }

           //read FNAM
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, tempQOBJ.unknownFNAM)) return false;
           bytesRead += 10;

           //read NNAM
           if (!loadUint32SubRecordFromStream(in_File, cNNAM, tempQOBJ.unknownNNAM)) return false;
           bytesRead += 10;

           hasUnpushedQOBJEntry = true;
           lastReadRec = cNNAM;
           break;
      case cQSTA:
           if (!hasUnpushedQOBJEntry)
           {
             std::cout << "Error while reading record of type QUST: encountered"
                       << " QSTA subrecord without previous QOBJ subrecord.\n";
             return false;
           }
           if (hasUnpushedQSTAEntry)
           {
             tempQOBJ.theQSTAs.push_back(tempQSTA);
           }
           tempQSTA.clear();

           //QSTA's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=8)
           {
             std::cout <<"Error: sub record QOBJ of QUST has invalid length ("
                       <<subLength <<" bytes). Should be eight bytes.\n";
             return false;
           }
           //read QSTA
           in_File.read((char*) &(tempQSTA.unknownQSTA), 8);
           bytesRead += 8;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord QSTA of QUST!\n";
             return false;
           }
           hasUnpushedQSTAEntry = true;
           lastReadRec = cQSTA;
           break;
      case cCIS2:
           //CIS2's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cout <<"Error: sub record CIS2 of QUST is longer than 511 characters!\n";
             return false;
           }
           //read CIS2's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CIS2 of QUST!\n";
             return false;
           }
           //check where we put that stuff
           if (hasUnpushedQSTAEntry)
           {
             if (tempQSTA.unknownCTDA_CIS2s.empty())
             {
               std::cout << "Error while reading record of type QUST: encountered"
                         << " CIS2 subrecord without previous CTDA subrecord.\n";
               return false;
             }
             if (!tempQSTA.unknownCTDA_CIS2s.back().unknownCIS2.empty())
             {
               std::cout << "Error: QUST seems to have more than one CIS2 subrecord.\n";
               return false;
             }
             tempQSTA.unknownCTDA_CIS2s.back().unknownCIS2 = std::string(buffer);
           }
           else if (hasUnpushedQSDTRecord)
           {
             if (tempQSDT.unknownCTDA_CIS2s.empty())
             {
               std::cout << "Error while reading record of type QUST: encountered"
                         << " CIS2 subrecord without previous CTDA subrecord.\n";
               return false;
             }
             if (!tempQSDT.unknownCTDA_CIS2s.back().unknownCIS2.empty())
             {
               std::cout << "Error: QUST seems to have more than one CIS2 subrecord per CTDA!\n";
               return false;
             }
             tempQSDT.unknownCTDA_CIS2s.back().unknownCIS2 = std::string(buffer);
           }
           else
           {
             //normal record
             if (indexPartStarted)
             {
               std::cout << "Error while reading record of type QUST: encountered"
                         << " CIS2 subrecord in index part!\n";
               return false;
             }
             if (unknownCTDA_CIS2s.empty())
             {
               std::cout << "Error while reading record of type QUST: encountered"
                         << " CIS2 subrecord without previous CTDA subrecord.\n";
               return false;
             }
             if (!unknownCTDA_CIS2s.back().unknownCIS2.empty())
             {
               std::cout << "Error: QUST seems to have more than one CIS2 subrecord per CTDA!\n";
               return false;
             }
             unknownCTDA_CIS2s.back().unknownCIS2 = std::string(buffer);
           }
           lastReadRec = cCIS2;
           break;
      case cANAM:
           if (hasUnpushedQSDTRecord)
           {
             i_entry.theQSDTs.push_back(tempQSDT);
             hasUnpushedQSDTRecord = false;
           }
           if (hasUnpushedIndexEntry)
           {
             indices.push_back(i_entry);
             hasUnpushedIndexEntry = false;
           }
           if (hasUnpushedQOBJEntry)
           {
             //push previous QOBJ entry
             //... and check for QSTA first
             if (hasUnpushedQSTAEntry)
             {
               tempQOBJ.theQSTAs.push_back(tempQSTA);
               hasUnpushedQSTAEntry = false;
             }
             //now push the real record
             theQOBJs.push_back(tempQOBJ);
             hasUnpushedQOBJEntry = false;
           }
           if (hasReadANAM)
           {
             std::cout << "Error: QUST seems to have more than one ANAM subrecord.\n";
             return false;
           }
           in_File.seekg(-4, std::ios_base::cur);
           //read ANAM
           if (!loadUint32SubRecordFromStream(in_File, cANAM, unknownANAM)) return false;
           bytesRead += 6;
           hasReadANAM = true;
           lastReadRec = cANAM;
           break;
      case cALST:
      case cALLS:
           //alias started
           al_entry.clear();
           //ALST is always first
           in_File.seekg(-4, std::ios_base::cur);
           //read ALST/ALLS
           if (!loadUint32SubRecordFromStream(in_File, subRecName, al_entry.unknownALST)) return false;
           bytesRead += 6;

           if (subRecName==cALST)
           {
             al_entry.hasALLS = false;
             al_entry.unknownALLS = 0;
             al_entry.hasALST = true;
           }
           else
           {
             al_entry.hasALLS = true;
             al_entry.unknownALLS = al_entry.unknownALST;
             al_entry.hasALST = false;
             al_entry.unknownALST = 0;
           }

           toDo = true;
           hasReadFNAM = false;
           while (toDo)
           {
             //read next subrecord
             in_File.read((char*) &subRecName, 4);
             bytesRead += 4;
             switch (subRecName)
             {
               case cALID:
                    if (!al_entry.aliasID.empty())
                    {
                      std::cout << "Error: QUST seems to have more than one ALID subrecord per alias structure.\n";
                      return false;
                    }
                    //ALID's length
                    in_File.read((char*) &subLength, 2);
                    bytesRead += 2;
                    if (subLength>511)
                    {
                      std::cout <<"Error: sub record ALID of QUST is longer than 511 characters!\n";
                      return false;
                    }
                    //read ALID's stuff
                    memset(buffer, 0, 512);
                    in_File.read(buffer, subLength);
                    bytesRead += subLength;
                    if (!in_File.good())
                    {
                      std::cout << "Error while reading subrecord ALID of QUST!\n";
                      return false;
                    }
                    al_entry.aliasID = std::string(buffer);
                    break;
               case cFNAM:
                    if (hasReadFNAM)
                    {
                      std::cout << "Error: QUST seems to have more than one FNAM subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read FNAM
                    if (!loadUint32SubRecordFromStream(in_File, cFNAM, al_entry.unknownFNAM)) return false;
                    bytesRead += 6;
                    hasReadFNAM = true;
                    break;
               case cALFA:
                    if (al_entry.hasALFA)
                    {
                      std::cout << "Error: QUST seems to have more than one ALFA subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALFA
                    if (!loadUint32SubRecordFromStream(in_File, cALFA, al_entry.unknownALFA)) return false;
                    bytesRead += 6;
                    al_entry.hasALFA = true;
                    break;
               case cALRT:
                    if (al_entry.hasALRT)
                    {
                      std::cout << "Error: QUST seems to have more than one ALRT subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALRT
                    if (!loadUint32SubRecordFromStream(in_File, cALRT, al_entry.unknownALRT)) return false;
                    bytesRead += 6;
                    al_entry.hasALRT = true;
                    break;
               case cALCO:
                    if (al_entry.hasALCO)
                    {
                      std::cout << "Error: QUST seems to have more than one ALCO subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALCO
                    if (!loadUint32SubRecordFromStream(in_File, cALCO, al_entry.unknownALCO)) return false;
                    bytesRead += 6;
                    al_entry.hasALCO = true;
                    break;
               case cALCA:
                    if (al_entry.hasALCA)
                    {
                      std::cout << "Error: QUST seems to have more than one ALCA subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALCA
                    if (!loadUint32SubRecordFromStream(in_File, cALCA, al_entry.unknownALCA)) return false;
                    bytesRead += 6;
                    al_entry.hasALCA = true;
                    break;
               case cALCL:
                    if (al_entry.hasALCL)
                    {
                      std::cout << "Error: QUST seems to have more than one ALCL subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALCL
                    if (!loadUint32SubRecordFromStream(in_File, cALCL, al_entry.unknownALCL)) return false;
                    bytesRead += 6;
                    al_entry.hasALCL = true;
                    break;
               case cALDN:
                    if (al_entry.hasALDN)
                    {
                      std::cout << "Error: QUST seems to have more than one ALDN subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALDN
                    if (!loadUint32SubRecordFromStream(in_File, cALDN, al_entry.unknownALDN)) return false;
                    bytesRead += 6;
                    al_entry.hasALDN = true;
                    break;
               case cCOCT:
                    if (!al_entry.components.empty())
                    {
                      std::cout << "Error: QUST seems to have more than one COCT subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read COCT
                    size_int = 0;
                    if (!loadUint32SubRecordFromStream(in_File, cCOCT, size_int)) return false;
                    bytesRead += 6;
                    if (size_int==0)
                    {
                      std::cout << "Error: subrecord COCT of QUST has invalid value!\n";
                      return false;
                    }

                    for (i=0; i<size_int; ++i)
                    {
                      //read CNTO
                      in_File.read((char*) &subRecName, 4);
                      bytesRead += 4;
                      if (subRecName!=cCNTO)
                      {
                        UnexpectedRecord(cCNTO, subRecName);
                        return false;
                      }
                      //CNTO's length
                      in_File.read((char*) &subLength, 2);
                      bytesRead += 2;
                      if (subLength!=8)
                      {
                        std::cout <<"Error: sub record CNTO of QUST has invlaid length ("
                                  <<subLength<<" bytes). Should be 8 bytes!\n";
                        return false;
                      }
                      //read CNTO's stuff
                      in_File.read((char*) &tempComp.formID, 4);
                      in_File.read((char*) &tempComp.count, 4);
                      bytesRead += 8;
                      if (!in_File.good())
                      {
                        std::cout << "Error while reading subrecord CNTO of QUST!\n";
                        return false;
                      }
                      al_entry.components.push_back(tempComp);
                    }//for
                    break;
               case cCNTO:
                    /*Sometimes a single CNTO record appears without a previous
                      COCT record. I'm not sure why this would happen, but let's
                      assume this is one single component entry. */
                    //CNTO's length
                    in_File.read((char*) &subLength, 2);
                    bytesRead += 2;
                    if (subLength!=8)
                    {
                      std::cout <<"Error: sub record CNTO of QUST has invlaid length ("
                                <<subLength<<" bytes). Should be 8 bytes!\n";
                      return false;
                    }
                    //read CNTO's stuff
                    in_File.read((char*) &tempComp.formID, 4);
                    in_File.read((char*) &tempComp.count, 4);
                    bytesRead += 8;
                    if (!in_File.good())
                    {
                      std::cout << "Error while reading subrecord CNTO of QUST!\n";
                      return false;
                    }
                    al_entry.components.push_back(tempComp);
                    break;
               case cKSIZ:
                    if (!al_entry.keywordArray.empty())
                    {
                      std::cout << "Error: QUST seems to have more than one KSIZ subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read KSIZ
                    size_int = 0;
                    if (!loadUint32SubRecordFromStream(in_File, cKSIZ, size_int)) return false;
                    bytesRead += 6;
                    if (size_int==0)
                    {
                      std::cout << "Error: subrecord KSIZ of QUST has invalid value!\n";
                      return false;
                    }

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
                    if (subLength!=4*size_int)
                    {
                      std::cout <<"Error: sub record KWDA of QUST has invlaid length ("
                                <<subLength<<" bytes). Should be "<<4*size_int<<" bytes!\n";
                      return false;
                    }
                    //read KWDA's stuff
                    for (i=0; i<size_int; ++i)
                    {
                      in_File.read((char*) &tempUint32, 4);
                      bytesRead += 4;
                      if (!in_File.good())
                      {
                        std::cout << "Error while reading subrecord KWDA of QUST!\n";
                        return false;
                      }
                      al_entry.keywordArray.push_back(tempUint32);
                    }//for
                    break;
               case cALFE:
                    if (al_entry.hasALFE)
                    {
                      std::cout << "Error: QUST seems to have more than one ALFE subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALFE
                    if (!loadUint32SubRecordFromStream(in_File, cALFE, al_entry.unknownALFE)) return false;
                    bytesRead += 6;
                    al_entry.hasALFE = true;
                    break;
               case cALFD:
                    if (al_entry.hasALFD)
                    {
                      std::cout << "Error: QUST seems to have more than one ALFD subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALFD
                    if (!loadUint32SubRecordFromStream(in_File, cALFD, al_entry.unknownALFD)) return false;
                    bytesRead += 6;
                    al_entry.hasALFD = true;
                    break;
               case cALFI:
                    if (al_entry.hasALFI)
                    {
                      std::cout << "Error: QUST seems to have more than one ALFI subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALFI
                    if (!loadUint32SubRecordFromStream(in_File, cALFI, al_entry.unknownALFI)) return false;
                    bytesRead += 6;
                    al_entry.hasALFI = true;
                    break;
               case cALFL:
                    if (al_entry.hasALFL)
                    {
                      std::cout << "Error: QUST seems to have more than one ALFL subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALFL
                    if (!loadUint32SubRecordFromStream(in_File, cALFL, al_entry.unknownALFL)) return false;
                    bytesRead += 6;
                    al_entry.hasALFL = true;
                    break;
               case cALFR:
                    if (al_entry.hasALFR)
                    {
                      std::cout << "Error: QUST seems to have more than one ALFR subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALFR
                    if (!loadUint32SubRecordFromStream(in_File, cALFR, al_entry.unknownALFR)) return false;
                    bytesRead += 6;
                    al_entry.hasALFR = true;
                    break;
               case cALNA:
                    if (al_entry.hasALNA)
                    {
                      std::cout << "Error: QUST seems to have more than one ALNA subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALNA
                    if (!loadUint32SubRecordFromStream(in_File, cALNA, al_entry.unknownALNA)) return false;
                    bytesRead += 6;
                    al_entry.hasALNA = true;
                    break;
               case cALNT:
                    if (al_entry.hasALNT)
                    {
                      std::cout << "Error: QUST seems to have more than one ALNT subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALNT
                    if (!loadUint32SubRecordFromStream(in_File, cALNT, al_entry.unknownALNT)) return false;
                    bytesRead += 6;
                    al_entry.hasALNT = true;
                    break;
               case cALUA:
                    if (al_entry.hasALUA)
                    {
                      std::cout << "Error: QUST seems to have more than one ALUA subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALUA
                    if (!loadUint32SubRecordFromStream(in_File, cALUA, al_entry.unknownALUA)) return false;
                    bytesRead += 6;
                    al_entry.hasALUA = true;
                    break;
               case cALEQ:
                    if (al_entry.hasALEQ)
                    {
                      std::cout << "Error: QUST seems to have more than one ALEQ subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALEQ
                    if (!loadUint32SubRecordFromStream(in_File, cALEQ, al_entry.unknownALEQ)) return false;
                    bytesRead += 6;
                    al_entry.hasALEQ = true;
                    break;
               case cALEA:
                    if (al_entry.hasALEA)
                    {
                      std::cout << "Error: QUST seems to have more than one ALEA subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALEA
                    if (!loadUint32SubRecordFromStream(in_File, cALEA, al_entry.unknownALEA)) return false;
                    bytesRead += 6;
                    al_entry.hasALEA = true;
                    break;
               case cALSP:
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALSP
                    if (!loadUint32SubRecordFromStream(in_File, cALSP, tempUint32)) return false;
                    bytesRead += 6;
                    al_entry.unknownALSPs.push_back(tempUint32);
                    break;
               case cKNAM:
                    if (al_entry.hasKNAM)
                    {
                      std::cout << "Error: QUST seems to have more than one KNAM subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read KNAM
                    if (!loadUint32SubRecordFromStream(in_File, cKNAM, al_entry.unknownKNAM)) return false;
                    bytesRead += 6;
                    al_entry.hasKNAM = true;
                    break;
               case cCTDA:
                    if (!tempCTDA.loadFromStream(in_File, bytesRead))
                    {
                      std::cout << "Error while reading subrecord CTDA of QUST!\n";
                      return false;
                    }
                    al_entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound(tempCTDA, ""));
                    break;
               case cCIS2:
                    if (al_entry.unknownCTDA_CIS2s.empty())
                    {
                      std::cout << "Error in QUST: CIS2 subrecord without previous CTDA subrecord in alias structure.\n";
                      return false;
                    }
                    if (!al_entry.unknownCTDA_CIS2s.back().unknownCIS2.empty())
                    {
                      std::cout << "Error: QUST seems to have more than one CIS2 subrecord per CTDA in alias structure.\n";
                      return false;
                    }
                    //CIS2's length
                    in_File.read((char*) &subLength, 2);
                    bytesRead += 2;
                    if (subLength>511)
                    {
                      std::cout <<"Error: sub record CIS2 of QUST is longer than 511 characters!\n";
                      return false;
                    }
                    //read CIS2's stuff
                    memset(buffer, 0, 512);
                    in_File.read(buffer, subLength);
                    bytesRead += subLength;
                    if (!in_File.good())
                    {
                      std::cout << "Error while reading subrecord CIS2 of QUST!\n";
                      return false;
                    }
                    al_entry.unknownCTDA_CIS2s.back().unknownCIS2 = std::string(buffer);
                    break;
               case cSPOR:
                    if (al_entry.hasSPOR)
                    {
                      std::cout << "Error: QUST seems to have more than one SPOR subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read SPOR
                    if (!loadUint32SubRecordFromStream(in_File, cSPOR, al_entry.unknownSPOR)) return false;
                    bytesRead += 6;
                    al_entry.hasSPOR = true;
                    break;
               case cECOR:
                    if (al_entry.hasECOR)
                    {
                      std::cout << "Error: QUST seems to have more than one ECOR subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ECOR
                    if (!loadUint32SubRecordFromStream(in_File, cECOR, al_entry.unknownECOR)) return false;
                    bytesRead += 6;
                    al_entry.hasECOR = true;
                    break;
               case cALFC:
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALFC
                    if (!loadUint32SubRecordFromStream(in_File, cALFC, tempUint32)) return false;
                    bytesRead += 6;
                    al_entry.unknownALFCs.push_back(tempUint32);
                    break;
               case cALPC:
                    in_File.seekg(-4, std::ios_base::cur);
                    //read ALPC
                    if (!loadUint32SubRecordFromStream(in_File, cALPC, tempUint32)) return false;
                    bytesRead += 6;
                    al_entry.unknownALPCs.push_back(tempUint32);
                    break;
               case cVTCK:
                    if (al_entry.hasVTCK)
                    {
                      std::cout << "Error: QUST seems to have more than one VTCK subrecord per alias structure.\n";
                      return false;
                    }
                    in_File.seekg(-4, std::ios_base::cur);
                    //read VTCK
                    if (!loadUint32SubRecordFromStream(in_File, cVTCK, al_entry.unknownVTCK)) return false;
                    bytesRead += 6;
                    al_entry.hasVTCK = true;
                    break;
               case cALED:
                    //end marker
                    //ALED's length
                    in_File.read((char*) &subLength, 2);
                    bytesRead += 2;
                    if (subLength!=0)
                    {
                      std::cout <<"Error: sub record ALED of QUST has invalid length ("
                                <<subLength<<" bytes). Should be zero bytes!\n";
                      return false;
                    }
                    toDo = false;
                    break;
               default:
                    std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                              << "\", but only ALID, FNAM, ALFA, ALRT, ALCO, "
                              << "ALCA, ALCL, ALDN, ALFE, ALFD, ALFI, ALFL, "
                              << "ALFR, ALUA, ALEQ, ALEA, ALSP, KNAM, CTDA, CIS2, SPOR, ECOR, ALFC, ALPC, VTCK or ALED are allowed here!\n";
                    return false;
             }//swi
           }//while

           //presence checks
           if (!(hasReadFNAM))
           {
             std::cout << "Error: at least one required subrecord of alias structure in QUST is missing!\n";
             return false;
           }
           aliases.push_back(al_entry);
           lastReadRec = cALED;
           break;
      default:
          std::cout << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only VMAD, FULL, DNAM, FLTR, NEXT, INDX, QSDT, NAM0, CNAM or ANAM are allowed here!\n";
           return false;
    }//swi
  }//while

  if (hasUnpushedIndexEntry)
  {
    if (hasUnpushedQSDTRecord)
    {
      i_entry.theQSDTs.push_back(tempQSDT);
      hasUnpushedQSDTRecord = false;
    }
    indices.push_back(i_entry);
    hasUnpushedIndexEntry = false;
  }
  if (hasUnpushedQSTAEntry)
  {
    tempQOBJ.theQSTAs.push_back(tempQSTA);
    hasUnpushedQSTAEntry = false;
  }
  if (hasUnpushedQOBJEntry)
  {
    theQOBJs.push_back(tempQOBJ);
  }

  //presence checks
  if (!(hasReadANAM and hasReadDNAM))
  {
    std::cout << "Error: at least one required subrecord of QUST is missing!\n";
    std::cout << "ANAM: "<<hasReadANAM<<"    DNAM: "<<hasReadDNAM<<"\n";
    return false;
  }

  return in_File.good();
}

int32_t QuestRecord::getRecordType() const
{
  return cQUST;
}

} //namespace
