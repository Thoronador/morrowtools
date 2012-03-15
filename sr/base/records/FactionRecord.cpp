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

#include "FactionRecord.h"
#include <iostream>
#include "../SR_Constants.h"
#include "../../../mw/base/HelperIO.h"

namespace SRTP
{

/* group combat reaction constants */
const uint32_t FactionRecord::ReactionNeutral = 0x00000000;
const uint32_t FactionRecord::ReactionEnemy   = 0x00000001;
const uint32_t FactionRecord::ReactionAlly    = 0x00000002;
const uint32_t FactionRecord::ReactionFriend  = 0x00000003;

/* InterfactionRelation's functions */
bool FactionRecord::InterfactionRelation::operator==(const FactionRecord::InterfactionRelation& other) const
{
  return ((factionFormID==other.factionFormID) and (mod==other.mod)
      and (groupCombatReaction==other.groupCombatReaction));
}

/* RankData's functions */
bool FactionRecord::RankData::operator==(const FactionRecord::RankData& other) const
{
  return ((index==other.index) and(maleNameStringID==other.maleNameStringID)
      and (femaleNameStringID==other.femaleNameStringID));
}

/* FactionRecord's functions */

FactionRecord::FactionRecord()
: BasicRecord()
{
  editorID = "";
  relations.clear(); //subrecords XNAM
  hasFULL = false;
  nameStringID = 0; //subrecord FULL
  unknownDATA = 0;
  exteriorJailMarkerRefID = 0; //subrecord JAIL opt.
  followerWaitMarkerRefID = 0; //subrecord WAIT, opt.
  stolenGoodsContainerRefID = 0; //subrecord STOL, opt.
  playerInventoryContainerRefID = 0; //subrecord PLCN, opt.
  crimeFactionListFormID = 0; //subrecord CRGR, opt.
  jailOutfitFormID = 0; //subrecord JOUT, opt.
  unknownCRVA.setPresence(false); //subrecord CRVA, opt.
  ranks.clear();
  hasVEND = false;
  unknownVEND = 0;
  hasVENC = false;
  unknownVENC = 0;
  unknownVENV.setPresence(false);
  unknownPLVD.setPresence(false);
  conditions.clear();
}

FactionRecord::~FactionRecord()
{
  //empty
}

#ifndef SR_NO_RECORD_EQUALITY
bool FactionRecord::equals(const FactionRecord& other) const
{
  return ((equalsBasic(other)) and (editorID==other.editorID)
      and (relations==other.relations)
      and (hasFULL==other.hasFULL) and ((nameStringID==other.nameStringID) or (!hasFULL))
      and (unknownDATA==other.unknownDATA)
      and (exteriorJailMarkerRefID==other.exteriorJailMarkerRefID)
      and (followerWaitMarkerRefID==other.followerWaitMarkerRefID)
      and (stolenGoodsContainerRefID==other.stolenGoodsContainerRefID)
      and (playerInventoryContainerRefID==other.playerInventoryContainerRefID)
      and (crimeFactionListFormID==other.crimeFactionListFormID)
      and (jailOutfitFormID==other.jailOutfitFormID)
      and (unknownCRVA==other.unknownCRVA) and (ranks==other.ranks)
      and (hasVEND==other.hasVEND) and ((unknownVEND==other.unknownVEND) or (!hasVEND))
      and (hasVENC==other.hasVENC) and ((unknownVENC==other.unknownVENC) or (!hasVENC))
      and (unknownVENV==other.unknownVENV) and (unknownPLVD==other.unknownPLVD)
      and (conditions==other.conditions));
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FactionRecord::getWriteSize() const
{
  uint32_t writeSize;
  writeSize = 4 /* EDID */ +2 /* 2 bytes for length */
        +editorID.length()+1 /* length of name +1 byte for NUL termination */
        +relations.size()*(4 /* XNAM */ +2 /* 2 bytes for length */ +12 /* fixed length of 12 bytes */)
        +4 /* DATA */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  if (hasFULL)
  {
    writeSize = writeSize +4 /* FULL */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if FULL
  if (exteriorJailMarkerRefID!=0)
  {
    writeSize = writeSize +4 /* JAIL */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if JAIL
  if (followerWaitMarkerRefID!=0)
  {
    writeSize = writeSize +4 /* WAIT */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if WAIT
  if (stolenGoodsContainerRefID!=0)
  {
    writeSize = writeSize +4 /* STOL */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if STOL
  if (playerInventoryContainerRefID!=0)
  {
    writeSize = writeSize +4 /* PLCN */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if PLCN
  if (crimeFactionListFormID!=0)
  {
    writeSize = writeSize +4 /* CRGR */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if CRGR
  if (jailOutfitFormID!=0)
  {
    writeSize = writeSize +4 /* JOUT */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if JOUT
  if (unknownCRVA.isPresent())
  {
    writeSize = writeSize +4 /* CRVA */ +2 /* 2 bytes for length */
               +unknownCRVA.getSize() /* fixed length of 20 or 16 or 12 bytes */;
  }//if CRVA
  if (!ranks.empty())
  {
    unsigned int i;
    for (i=0; i<ranks.size(); ++i)
    {
      writeSize = writeSize +4 /* RNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
      if (ranks[i].maleNameStringID!=0)
      {
        writeSize = writeSize +4 /* MNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
      }
      if (ranks[i].femaleNameStringID!=0)
      {
        writeSize = writeSize +4 /* FNAM */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
      }
    }//for
  }//if ranks
  if (hasVEND)
  {
    writeSize = writeSize +4 /* VEND */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if VEND
  if (hasVENC)
  {
    writeSize = writeSize +4 /* VENC */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
  }//if VENC
  if (unknownVENV.isPresent())
  {
    writeSize = writeSize +4 /* VENV */ +2 /* 2 bytes for length */ +unknownVENV.getSize() /* length */;
  }//if VENV
  if (unknownPLVD.isPresent())
  {
    writeSize = writeSize +4 /* PLVD */ +2 /* 2 bytes for length */ +unknownPLVD.getSize() /* length */;
  }
  if (!conditions.empty())
  {
    writeSize = writeSize +4 /* CITC */ +2 /* 2 bytes for length */ +4 /* fixed length of four bytes */;
    unsigned int i;
    for (i=0; i<conditions.size(); ++i)
    {
      //calculate size of CTDA/CIS2
      writeSize += conditions[i].getWriteSize();
    }//for
  }//if conditions
  return writeSize;
}

bool FactionRecord::saveToStream(std::ofstream& output) const
{
  output.write((const char*) &cFACT, 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize())) return false;

  //write EDID
  output.write((const char*) &cEDID, 4);
  //EDID's length
  uint16_t subLength = editorID.length()+1;
  output.write((const char*) &subLength, 2);
  //write editor ID
  output.write(editorID.c_str(), subLength);

  if (!relations.empty())
  {
    unsigned int i;
    for (i=0; i<relations.size(); ++i)
    {
      //write XNAM
      output.write((const char*) &cXNAM, 4);
      //XNAM's length
      subLength = 12; //fixed
      output.write((const char*) &subLength, 2);
      //write XNAM's data
      output.write((const char*) &relations[i].factionFormID, 4);
      output.write((const char*) &relations[i].mod, 4);
      output.write((const char*) &relations[i].groupCombatReaction, 4);
    }//for
  }//if relations

  if (hasFULL)
  {
    //write FULL
    output.write((const char*) &cFULL, 4);
    //FULL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write FULL's data
    output.write((const char*) &nameStringID, 4);
  }//if has FULL

  //write DATA
  output.write((const char*) &cDATA, 4);
  //DATA's length
  subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write DATA's data :P
  output.write((const char*) &unknownDATA, 4);

  if (exteriorJailMarkerRefID!=0)
  {
    //write JAIL
    output.write((const char*) &cJAIL, 4);
    //JAIL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write JAIL's data
    output.write((const char*) &exteriorJailMarkerRefID, 4);
  }//if has JAIL

  if (followerWaitMarkerRefID!=0)
  {
    //write WAIT
    output.write((const char*) &cWAIT, 4);
    //WAIT's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write WAIT's data
    output.write((const char*) &followerWaitMarkerRefID, 4);
  }//if has WAIT

  if (stolenGoodsContainerRefID!=0)
  {
    //write STOL
    output.write((const char*) &cSTOL, 4);
    //STOL's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write STOL's data
    output.write((const char*) &stolenGoodsContainerRefID, 4);
  }//if has STOL

  if (playerInventoryContainerRefID!=0)
  {
    //write PLCN
    output.write((const char*) &cPLCN, 4);
    //PLCN's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write PLCN's data
    output.write((const char*) &playerInventoryContainerRefID, 4);
  }//if has PLCN

  if (crimeFactionListFormID!=0)
  {
    //write CRGR
    output.write((const char*) &cCRGR, 4);
    //CRGR's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write CRGR's data
    output.write((const char*) &crimeFactionListFormID, 4);
  }//if has CRGR

  if (jailOutfitFormID!=0)
  {
    //write JOUT
    output.write((const char*) &cJOUT, 4);
    //JOUT's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write JOUT's data
    output.write((const char*) &jailOutfitFormID, 4);
  }//if has JOUT

  //write CRVA
  if (unknownCRVA.isPresent())
  {
    if (!unknownCRVA.saveToStream(output, cCRVA))
    {
      std::cout << "Error while writing subrecord CRVA of FACT!\n";
      return false;
    }
  }

  if (!ranks.empty())
  {
    unsigned int i;
    for (i=0; i<ranks.size(); ++i)
    {
      //write RNAM
      output.write((const char*) &cRNAM, 4);
      //RNAM's length
      subLength = 4; //fixed
      output.write((const char*) &subLength, 2);
      //write rank index
      output.write((const char*) &(ranks[i].index), 4);

      if (ranks[i].maleNameStringID!=0)
      {
        //write MNAM
        output.write((const char*) &cMNAM, 4);
        //MNAM's length
        subLength = 4; //fixed
        output.write((const char*) &subLength, 2);
        //write male rank name's string index
        output.write((const char*) &(ranks[i].maleNameStringID), 4);
      }
      if (ranks[i].femaleNameStringID!=0)
      {
        //write FNAM
        output.write((const char*) &cFNAM, 4);
        //FNAM's length
        subLength = 4; //fixed
        output.write((const char*) &subLength, 2);
        //write female rank name's string index
        output.write((const char*) &(ranks[i].femaleNameStringID), 4);
      }
    }//for
  }//if ranks

  if (hasVEND)
  {
    //write VEND
    output.write((const char*) &cVEND, 4);
    //VEND's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write VEND's data
    output.write((const char*) &unknownVEND, 4);
  }//if has VEND

  if (hasVENC)
  {
    //write VENC
    output.write((const char*) &cVENC, 4);
    //VENC's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write VENC's data
    output.write((const char*) &unknownVENC, 4);
  }//if has VENC


  if (unknownVENV.isPresent())
  {
    //write VENV
    if (!unknownVENV.saveToStream(output, cVENV))
    {
      std::cout << "Error while writing subrecord VENV of FACT!\n";
      return false;
    }//if
  }//if VENV

  if (unknownPLVD.isPresent())
  {
    //write PLVD
    if (!unknownPLVD.saveToStream(output, cPLVD))
    {
      std::cout << "Error while writing subrecord PLVD of FACT!\n";
      return false;
    }//if
  }//if PLVD

  if (!conditions.empty())
  {
    const uint32_t cond_count = conditions.size();
    //write CITC
    output.write((const char*) &cCITC, 4);
    //CITC's length
    subLength = 4; //fixed
    output.write((const char*) &subLength, 2);
    //write CITC's data
    output.write((const char*) &cond_count, 4);

    unsigned int i;
    for (i=0; i<cond_count; ++i)
    {
      //write CTDA/CIS2
      if (!conditions[i].saveToStream(output))
      {
        std::cout << "Error while writing subrecord CTDA or CIS2 of FACT!\n";
        return false;
      }
    }//for
  }//if conditions

  return output.good();
}
#endif

bool FactionRecord::loadFromStream(std::ifstream& in_File)
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
  bytesRead += 4;
  if (subLength>511)
  {
    std::cout <<"Error: sub record EDID of FACT is longer than 511 characters!\n";
    return false;
  }
  //read EDID's stuff
  char buffer[512];
  memset(buffer, 0, 512);
  in_File.read(buffer, subLength);
  bytesRead += subLength;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord EDID of FACT!\n";
    return false;
  }
  editorID = std::string(buffer);

  relations.clear();
  InterfactionRelation tempInterfacRel;
  hasFULL = false; nameStringID = 0;
  bool hasReadDATA = false;
  unknownDATA = 0;
  exteriorJailMarkerRefID = 0; //subrecord JAIL
  followerWaitMarkerRefID = 0; //subrecord WAIT
  stolenGoodsContainerRefID = 0; //subrecord STOL
  playerInventoryContainerRefID = 0; //subrecord PLCN
  crimeFactionListFormID = 0;
  jailOutfitFormID = 0;
  unknownCRVA.setPresence(false);
  ranks.clear();
  RankData tempRank;
  bool hasUnpushedRank = false;
  hasVEND = false; unknownVEND = 0;
  hasVENC = false; unknownVENC = 0;
  unknownVENV.setPresence(false);
  unknownPLVD.setPresence(false);
  conditions.clear();
  CTDA_CIS2_compound tempCTDA_CIS2;
  uint32_t cond_count = 0;
  bool hasUnpushedCTDA_CIS2 = false;
  while (bytesRead<readSize)
  {
    //read next subrecord's name
    in_File.read((char*) &subRecName, 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cXNAM:
           //XNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength!=12)
           {
             std::cout <<"Error: subrecord XNAM of FACT has invalid length ("
                       <<subLength <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           //read XNAM's stuff
           in_File.read((char*) &(tempInterfacRel.factionFormID), 4);
           in_File.read((char*) &(tempInterfacRel.mod), 4);
           in_File.read((char*) &(tempInterfacRel.groupCombatReaction), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord XNAM of FACT!\n";
             return false;
           }
           relations.push_back(tempInterfacRel);
           break;
      case cFULL:
           if (hasFULL)
           {
             std::cout << "Error: FACT seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cFULL, nameStringID, false))
           {
             return false;
           }
           bytesRead += 6;
           hasFULL = true;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cout << "Error: FACT seems to have more than one DATA subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cDATA, unknownDATA, false))
           {
             return false;
           }
           bytesRead += 6;
           hasReadDATA = true;
           break;
      case cJAIL:
           if (exteriorJailMarkerRefID!=0)
           {
             std::cout << "Error: FACT seems to have more than one JAIL subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cJAIL, exteriorJailMarkerRefID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (exteriorJailMarkerRefID==0)
           {
             std::cout << "Error: JAIL subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cWAIT:
           if (followerWaitMarkerRefID!=0)
           {
             std::cout << "Error: FACT seems to have more than one WAIT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cWAIT, followerWaitMarkerRefID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (followerWaitMarkerRefID==0)
           {
             std::cout << "Error: WAIT subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cSTOL:
           if (stolenGoodsContainerRefID!=0)
           {
             std::cout << "Error: FACT seems to have more than one STOL subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cSTOL, stolenGoodsContainerRefID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (stolenGoodsContainerRefID==0)
           {
             std::cout << "Error: STOL subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cPLCN:
           if (playerInventoryContainerRefID!=0)
           {
             std::cout << "Error: FACT seems to have more than one PLCN subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cPLCN, playerInventoryContainerRefID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (playerInventoryContainerRefID==0)
           {
             std::cout << "Error: PLCN subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cCRGR:
           if (crimeFactionListFormID!=0)
           {
             std::cout << "Error: FACT seems to have more than one CRGR subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cCRGR, crimeFactionListFormID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (crimeFactionListFormID==0)
           {
             std::cout << "Error: CRGR subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cJOUT:
           if (jailOutfitFormID!=0)
           {
             std::cout << "Error: FACT seems to have more than one JOUT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cJOUT, jailOutfitFormID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (jailOutfitFormID==0)
           {
             std::cout << "Error: JOUT subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cCRVA:
           if (unknownCRVA.isPresent())
           {
             std::cout << "Error: FACT seems to have more than one CRVA subrecord!\n";
             return false;
           }
           //read CRVA
           if (!unknownCRVA.loadFromStream(in_File, cCRVA, false))
           {
             std::cout << "Error while reading subrecord CRVA of FACT!\n";
             return false;
           }
           subLength = unknownCRVA.getSize();
           bytesRead += (2+subLength);
           //length check
           if ((subLength!=20) and (subLength!=16)  and (subLength!=12))
           {
             std::cout <<"Error: subrecord CRVA of FACT has invalid length ("
                       <<subLength <<" bytes). Should be 12 or 16 or 20 bytes!\n";
             return false;
           }
           break;
      case cRNAM:
           if (hasUnpushedRank)
           {
             /* //zero index for both male and female is allowed (but kind of
             // useless, at least for player-accessable factions)
             if ((tempRank.maleNameStringID==0) and (tempRank.femaleNameStringID==0))
             {
               std::cout << "Error while reading record of type FACT: string "
                        << "indices for both male and female name are zero!\n";
               return false;
             } */
             ranks.push_back(tempRank);
           }
           tempRank.index = 0;
           tempRank.maleNameStringID = 0;
           tempRank.femaleNameStringID = 0;
           if (!loadUint32SubRecordFromStream(in_File, cRNAM, tempRank.index, false))
           {
             return false;
           }
           bytesRead += 6;
           hasUnpushedRank = true;
           break;
      case cMNAM:
           if (!hasUnpushedRank)
           {
             std::cout << "Error while reading record of type FACT: MNAM "
                       << "without previous RNAM subrecord encountered!\n";
             return false;
           }
           if (tempRank.maleNameStringID!=0)
           {
             std::cout << "Error: FACT seems to have more than one MNAM "
                       << "subrecord per RNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cMNAM, tempRank.maleNameStringID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (tempRank.maleNameStringID==0)
           {
             std::cout << "Error: subrecord MNAM of FACT has zero value!\n";
             return false;
           }
           break;
      case cFNAM:
           if (!hasUnpushedRank)
           {
             std::cout << "Error while reading record of type FACT: FNAM "
                       << "without previous RNAM subrecord encountered!\n";
             return false;
           }
           if (tempRank.femaleNameStringID!=0)
           {
             std::cout << "Error: FACT seems to have more than one FNAM "
                       << "subrecord per RNAM subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cFNAM, tempRank.femaleNameStringID, false))
           {
             return false;
           }
           bytesRead += 6;
           if (tempRank.femaleNameStringID==0)
           {
             std::cout << "Error: subrecord FNAM of FACT has zero value!\n";
             return false;
           }
           break;
      case cVEND:
           if (hasVEND)
           {
             std::cout << "Error: FACT seems to have more than one VEND subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cVEND, unknownVEND, false))
           {
             return false;
           }
           bytesRead += 6;
           hasVEND = true;
           break;
      case cVENC:
           if (hasVENC)
           {
             std::cout << "Error: FACT seems to have more than one VENC subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cVENC, unknownVENC, false))
           {
             return false;
           }
           bytesRead += 6;
           hasVENC = true;
           break;
      case cVENV:
           if (unknownVENV.isPresent())
           {
             std::cout << "Error: FACT seems to have more than one VENV subrecord!\n";
             return false;
           }
           //read VENV
           if (!unknownVENV.loadFromStream(in_File, cVENV, false))
           {
             std::cout << "Error while reading subrecord VENV of FACT!\n";
             return false;
           }
           //check VENV's length
           if (unknownVENV.getSize()!=12)
           {
             std::cout <<"Error: subrecord VENV of FACT has invalid length ("
                       <<unknownVENV.getSize() <<" bytes). Should be 12 bytes!\n";
             return false;
           }
           bytesRead += (2+12);
           break;
      case cPLVD:
           if (unknownPLVD.isPresent())
           {
             std::cout << "Error: FACT seems to have more than one PLVD subrecord!\n";
             return false;
           }
           //read PLVD
           if (!unknownPLVD.loadFromStream(in_File, cPLVD, false))
           {
             std::cout << "Error while reading subrecord PLVD of FACT!\n";
             return false;
           }
           //check length of PLVD
           if (unknownPLVD.getSize()!=12)
           {
             std::cout <<"Error: subrecord PLVD of FACT has invalid length ("
                       <<unknownPLVD.getSize()<<" bytes). Should be 12 bytes!\n";
             return false;
           }
           bytesRead += (2+12);
           break;
      case cCITC:
           if ((!conditions.empty()) or (cond_count!=0))
           {
             std::cout << "Error: FACT seems to have more than one CITC subrecord!\n";
             return false;
           }
           //read CITC
           if (!loadUint32SubRecordFromStream(in_File, cCITC, cond_count, false))
           {
             return false;
           }
           bytesRead += 6;

           if (cond_count==0)
           {
             std::cout << "Error: subrecord CITC of FACT has value zero!\n";
             return false;
           }
           break;
      case cCTDA:
           if (cond_count==0)
           {
             std::cout << "Error while reading record of type FACT: CTDA "
                       << "without previous CITC subrecord encountered!\n";
             return false;
           }
           //push what's still left
           if (hasUnpushedCTDA_CIS2)
           {
             conditions.push_back(tempCTDA_CIS2);
           }
           if (conditions.size()>=cond_count)
           {
             std::cout << "Error while reading record of type FACT: found more"
                       << " CTDA records than CITC's value suggested!\n";
             return false;
           }
           //clear stuff
           tempCTDA_CIS2.unknownCTDA.clear();
           tempCTDA_CIS2.unknownCIS2.clear();
           //read CTDA's data
           if (!tempCTDA_CIS2.unknownCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cout << "Error while reading subrecord CTDA of FACT!\n";
             return false;
           }
           hasUnpushedCTDA_CIS2 = true;
           break;
      case cCIS2:
           if (!hasUnpushedCTDA_CIS2)
           {
             std::cout << "Error while reading record of type FACT: CIS2 "
                       << "without previous CTDA subrecord encountered!\n";
             return false;
           }
           if (!tempCTDA_CIS2.unknownCIS2.empty())
           {
             std::cout << "Error: FACT seems to have more than one CIS2 "
                       << "subrecord per CTDA subrecord!\n";
             return false;
           }
           //CIS2's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 4;
           if (subLength>511)
           {
             std::cout <<"Error: sub record CIS2 of FACT is longer than 511 characters!\n";
             return false;
           }
           //read CIS2's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord CIS2 of FACT!\n";
             return false;
           }
           tempCTDA_CIS2.unknownCIS2 = std::string(buffer);
           if (tempCTDA_CIS2.unknownCIS2.empty())
           {
             std::cout << "Error: subrecord CIS2 of FACT is empty!\n";
             return false;
           }
           conditions.push_back(tempCTDA_CIS2);
           hasUnpushedCTDA_CIS2 = false;
           break;
      default:
           std::cout << "Error: unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only XNAM, FULL, DATA, JAIL, WAIT, STOL,"
                     << " PLCN, CRGR, JOUT, CRVA, RNAM, MNAM, FNAM, VEND, VENC,"
                     << " VENV, PLVD, CITC, CTDA or CIS2 are allowed here!\n";
           return false;
           break;
    }//swi
  }//while

  //handle remaining rank data chunk
  if (hasUnpushedRank)
  {
    /* //zero index for both male and female is allowed (but kind of useless, at least for player-accessable factions)
    if ((tempRank.maleNameStringID==0) and (tempRank.femaleNameStringID==0))
    {
      std::cout << "Error while reading record of type FACT: string indices "
                << "for both male and female name are zero!\n";
      return false;
    }*/
    ranks.push_back(tempRank);
  }//if unpushed stuff

  //push what's still left for conditions
  if (hasUnpushedCTDA_CIS2)
  {
    conditions.push_back(tempCTDA_CIS2);
  }
  //size check for conditions
  if (conditions.size()!=cond_count)
  {
    std::cout << "Error while reading FACT: expected "<<cond_count
              << " condition record(s), but only "<<conditions.size()
              << " record(s) was/were found!\n";
    return false;
  }

  //presence checks
  if (!hasReadDATA)
  {
    std::cout << "Error: at least one required subrecord of FACT is missing!\n";
    return false;
  }

  return in_File.good();
}

int32_t FactionRecord::getRecordType() const
{
  return cFACT;
}

    std::string editorID;
    std::vector<FactionRecord::InterfactionRelation> relations; //subrecords XNAM
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    uint32_t unknownDATA;
    bool hasWAIT;
    uint32_t unknownWAIT;
    bool hasSTOL;
    uint32_t unknownSTOL;
    bool hasPLCN;
    uint32_t unknownPLCN;
    uint32_t crimeFactionListFormID; //subrecord CRGR, opt.
    uint32_t jailOutfitFormID; //subrecord JOUT, opt.
    uint8_t unknownCRVA[20]; //subrecord CRVA
    bool hasVEND;
    uint32_t unknownVEND;
    bool hasVENC;
    uint32_t unknownVENC;
    uint8_t unknownVENV[12];
    BinarySubRecord unknownPLVD;

} //namespace
