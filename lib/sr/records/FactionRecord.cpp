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

#include "FactionRecord.hpp"
#include <cstring>
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"

namespace SRTP
{

/* flag constants */
const uint32_t FactionRecord::cFlagHiddenFromPC                    = 0x00000001;
const uint32_t FactionRecord::cFlagSpecialCombat                   = 0x00000002;
const uint32_t FactionRecord::cFlagTrackCrime                      = 0x00000040;
const uint32_t FactionRecord::cFlagIgnoresMurder                   = 0x00000080;
const uint32_t FactionRecord::cFlagIgnoresAssault                  = 0x00000100;
const uint32_t FactionRecord::cFlagIgnoresStealing                 = 0x00000200;
const uint32_t FactionRecord::cFlagIgnoresTrespass                 = 0x00000400;
const uint32_t FactionRecord::cFlagDoNotReportCrimesAgainstMembers = 0x00000800;
const uint32_t FactionRecord::cFlagIgnoresPickpocket               = 0x00002000;
const uint32_t FactionRecord::cFlagVendor                          = 0x00004000;
const uint32_t FactionRecord::cFlagCanBeOwner                      = 0x00008000;
const uint32_t FactionRecord::cFlagIgnoresWerewolf                 = 0x00010000;

/* group combat reaction constants */
const uint32_t FactionRecord::ReactionNeutral = 0x00000000;
const uint32_t FactionRecord::ReactionEnemy   = 0x00000001;
const uint32_t FactionRecord::ReactionAlly    = 0x00000002;
const uint32_t FactionRecord::ReactionFriend  = 0x00000003;

/* vendor flag constants */
const uint32_t FactionRecord::VendorData::cFlagOnlyBuysStolenItems = 0x00000001;
const uint32_t FactionRecord::VendorData::cFlagNotBuySell          = 0x00000100;

/* InterfactionRelation's functions */
bool FactionRecord::InterfactionRelation::operator==(const FactionRecord::InterfactionRelation& other) const
{
  return (factionFormID == other.factionFormID) && (mod == other.mod)
      && (groupCombatReaction == other.groupCombatReaction);
}

/* VendorData's functions */
FactionRecord::VendorData::VendorData()
: startHour(0),
  endHour(0),
  radius(0),
  flagsVendor(0),
  isPresent(false)
{
}

bool FactionRecord::VendorData::operator==(const VendorData& other) const
{
  if (isPresent!=other.isPresent)
    return false;
  if (!isPresent)
    return true;
  return (startHour == other.startHour) && (endHour == other.endHour)
      && (radius == other.radius) && (flagsVendor == other.flagsVendor);
}

/* RankData's functions */
FactionRecord::RankData::RankData()
: index(0),
  maleName(LocalizedString()),
  femaleName(LocalizedString())
{
}

bool FactionRecord::RankData::operator==(const FactionRecord::RankData& other) const
{
  return (index == other.index) && (maleName == other.maleName)
      && (femaleName == other.femaleName);
}

/* FactionRecord's functions */

FactionRecord::FactionRecord()
: BasicRecord(), editorID(""),
  relations(std::vector<InterfactionRelation>()), // subrecords XNAM
  name(LocalizedString()),
  flags(0),
  exteriorJailMarkerRefID(0), // subrecord JAIL opt.
  followerWaitMarkerRefID(0), // subrecord WAIT, opt.
  stolenGoodsContainerRefID(0), // subrecord STOL, opt.
  playerInventoryContainerRefID(0), // subrecord PLCN, opt.
  crimeFactionListFormID(0), // subrecord CRGR, opt.
  jailOutfitFormID(0), // subrecord JOUT, opt.
  unknownCRVA(BinarySubRecord()), // subrecord CRVA, opt.
  ranks(std::vector<RankData>()),
  vendorListFormID(0),
  vendorContainterFormID(0),
  vendorStuff(VendorData()),
  unknownPLVD(BinarySubRecord()),
  conditions(std::vector<CTDA_CIS2_compound>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool FactionRecord::equals(const FactionRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (relations == other.relations)
      && (name == other.name)
      && (flags == other.flags)
      && (exteriorJailMarkerRefID == other.exteriorJailMarkerRefID)
      && (followerWaitMarkerRefID == other.followerWaitMarkerRefID)
      && (stolenGoodsContainerRefID == other.stolenGoodsContainerRefID)
      && (playerInventoryContainerRefID == other.playerInventoryContainerRefID)
      && (crimeFactionListFormID == other.crimeFactionListFormID)
      && (jailOutfitFormID == other.jailOutfitFormID)
      && (unknownCRVA == other.unknownCRVA) && (ranks == other.ranks)
      && (vendorListFormID == other.vendorListFormID)
      && (vendorContainterFormID == other.vendorContainterFormID)
      && (vendorStuff == other.vendorStuff) && (unknownPLVD == other.unknownPLVD)
      && (conditions == other.conditions);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t FactionRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + relations.size() * (4 /* XNAM */ + 2 /* 2 bytes for length */ + 12 /* fixed length of 12 bytes */)
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (exteriorJailMarkerRefID != 0)
  {
    writeSize = writeSize + 4 /* JAIL */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (followerWaitMarkerRefID != 0)
  {
    writeSize = writeSize + 4 /* WAIT */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (stolenGoodsContainerRefID != 0)
  {
    writeSize = writeSize + 4 /* STOL */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (playerInventoryContainerRefID != 0)
  {
    writeSize = writeSize + 4 /* PLCN */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (crimeFactionListFormID != 0)
  {
    writeSize = writeSize + 4 /* CRGR */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (jailOutfitFormID != 0)
  {
    writeSize = writeSize + 4 /* JOUT */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (unknownCRVA.isPresent())
  {
    writeSize = writeSize + 4 /* CRVA */ + 2 /* 2 bytes for length */
               + unknownCRVA.size() /* fixed length of 20 or 16 or 12 bytes */;
  }
  for (const auto& rank: ranks)
  {
    writeSize = writeSize + 4 /* RNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */
               + rank.maleName.getWriteSize() /* MNAM */
               + rank.femaleName.getWriteSize() /* FNAM */;
  }
  if (vendorListFormID != 0)
  {
    writeSize = writeSize + 4 /* VEND */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (vendorContainterFormID != 0)
  {
    writeSize = writeSize + 4 /* VENC */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
  }
  if (vendorStuff.isPresent)
  {
    writeSize = writeSize + 4 /* VENV */ + 2 /* 2 bytes for length */ + 12 /* fixed length of 12 bytes */;
  }
  if (unknownPLVD.isPresent())
  {
    writeSize = writeSize + 4 /* PLVD */ + 2 /* 2 bytes for length */ + unknownPLVD.size() /* length */;
  }
  if (!conditions.empty())
  {
    writeSize = writeSize + 4 /* CITC */ + 2 /* 2 bytes for length */ + 4 /* fixed length of four bytes */;
    for (const auto& condition: conditions)
    {
      // calculate size of CTDA/CIS2
      writeSize += condition.getWriteSize();
    }
  }
  return writeSize;
}

bool FactionRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cFACT), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write((const char*) &subLength, 2);
  output.write(editorID.c_str(), subLength);

  if (name.isPresent())
  {
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  for (const auto& relation: relations)
  {
    // write inter-faction relation (XNAM)
    output.write(reinterpret_cast<const char*>(&cXNAM), 4);
    subLength = 12; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&relation.factionFormID), 4);
    output.write(reinterpret_cast<const char*>(&relation.mod), 4);
    output.write(reinterpret_cast<const char*>(&relation.groupCombatReaction), 4);
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 4; // fixed length
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // write DATA's data :P (or flags, for short)
  output.write(reinterpret_cast<const char*>(&flags), 4);

  if (exteriorJailMarkerRefID != 0)
  {
    // write jail marker ID (JAIL)
    output.write(reinterpret_cast<const char*>(&cJAIL), 4);
    subLength = 4; // fixed length
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&exteriorJailMarkerRefID), 4);
  }

  if (followerWaitMarkerRefID != 0)
  {
    // write wait marker id (WAIT)
    output.write(reinterpret_cast<const char*>(&cWAIT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&followerWaitMarkerRefID), 4);
  }

  if (stolenGoodsContainerRefID != 0)
  {
    // write stolen goods container reference id (STOL)
    output.write(reinterpret_cast<const char*>(&cSTOL), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&stolenGoodsContainerRefID), 4);
  }

  if (playerInventoryContainerRefID != 0)
  {
    // write PLCN
    output.write(reinterpret_cast<const char*>(&cPLCN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&playerInventoryContainerRefID), 4);
  }

  if (crimeFactionListFormID != 0)
  {
    // write CRGR
    output.write(reinterpret_cast<const char*>(&cCRGR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&crimeFactionListFormID), 4);
  }

  if (jailOutfitFormID != 0)
  {
    // write jail outfit form ID (JOUT)
    output.write(reinterpret_cast<const char*>(&cJOUT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&jailOutfitFormID), 4);
  }

  // write CRVA
  if (unknownCRVA.isPresent())
  {
    if (!unknownCRVA.saveToStream(output, cCRVA))
    {
      std::cerr << "Error while writing subrecord CRVA of FACT!\n";
      return false;
    }
  }

  for (const auto& rank: ranks)
  {
    // write rank index (RNAM)
    output.write(reinterpret_cast<const char*>(&cRNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&rank.index), 4);

    if (rank.maleName.isPresent())
    {
      if (!rank.maleName.saveToStream(output, cMNAM))
        return false;
    }
    if (rank.femaleName.isPresent())
    {
      if (!rank.femaleName.saveToStream(output, cFNAM))
        return false;
    }
  }

  if (vendorListFormID != 0)
  {
    // write vendor list's form ID (VEND)
    output.write(reinterpret_cast<const char*>(&cVEND), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&vendorListFormID), 4);
  }

  if (vendorContainterFormID != 0)
  {
    // write Vendor Containter's form ID (VENC)
    output.write(reinterpret_cast<const char*>(&cVENC), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&vendorContainterFormID), 4);
  }


  if (vendorStuff.isPresent)
  {
    // write vendor data (VENV)
    output.write(reinterpret_cast<const char*>(&cVENV), 4);
    subLength = 12;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&vendorStuff.startHour), 2);
    output.write(reinterpret_cast<const char*>(&vendorStuff.endHour), 2);
    output.write(reinterpret_cast<const char*>(&vendorStuff.radius), 4);
    output.write(reinterpret_cast<const char*>(&vendorStuff.flagsVendor), 4);
  }

  if (unknownPLVD.isPresent())
  {
    if (!unknownPLVD.saveToStream(output, cPLVD))
    {
      std::cerr << "Error while writing subrecord PLVD of FACT!\n";
      return false;
    }
  }

  if (!conditions.empty())
  {
    const uint32_t cond_count = conditions.size();
    // write condition item counter (CITC)
    output.write(reinterpret_cast<const char*>(&cCITC), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&cond_count), 4);

    for (const auto& condition: conditions)
    {
      // write CTDA/CIS2
      if (!condition.saveToStream(output))
      {
        std::cerr << "Error while writing subrecord CTDA or CIS2 of FACT!\n";
        return false;
      }
    }
  }

  return output.good();
}
#endif

bool FactionRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  //read EDID
  char buffer[512];
  if (!loadString512FromStream(in_File, editorID, buffer, cEDID, true, bytesRead))
    return false;

  relations.clear();
  InterfactionRelation tempInterfacRel;
  name.reset();
  bool hasReadDATA = false;
  flags = 0;
  exteriorJailMarkerRefID = 0; // subrecord JAIL
  followerWaitMarkerRefID = 0; // subrecord WAIT
  stolenGoodsContainerRefID = 0; // subrecord STOL
  playerInventoryContainerRefID = 0; // subrecord PLCN
  crimeFactionListFormID = 0;
  jailOutfitFormID = 0;
  unknownCRVA.setPresence(false);
  ranks.clear();
  RankData tempRank;
  bool hasUnpushedRank = false;
  vendorListFormID = 0;
  vendorContainterFormID = 0;
  // VENV
  vendorStuff.startHour = 0;
  vendorStuff.endHour = 0;
  vendorStuff.radius = 0;
  vendorStuff.flagsVendor = 0;
  vendorStuff.isPresent = false;
  // end of VENV
  unknownPLVD.setPresence(false);
  conditions.clear();
  CTDA_CIS2_compound tempCTDA_CIS2;
  uint32_t cond_count = 0;
  bool hasUnpushedCTDA_CIS2 = false;
  while (bytesRead < readSize)
  {
    // read next subrecord's name
    in_File.read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch(subRecName)
    {
      case cXNAM:
           // XNAM's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Subrecord XNAM of FACT has invalid length ("
                       << subLength << " bytes). Should be 12 bytes!\n";
             return false;
           }
           // read XNAM's stuff
           in_File.read(reinterpret_cast<char*>(&tempInterfacRel.factionFormID), 4);
           in_File.read(reinterpret_cast<char*>(&tempInterfacRel.mod), 4);
           in_File.read(reinterpret_cast<char*>(&tempInterfacRel.groupCombatReaction), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord XNAM of FACT!\n";
             return false;
           }
           if (tempInterfacRel.factionFormID == 0)
           {
             std::cerr << "Error: XNAM subrecord of FACT has form ID of value zero!\n";
             return false;
           }
           relations.push_back(tempInterfacRel);
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: FACT seems to have more than one FULL subrecord!\n";
             return false;
           }
           if (!name.loadFromStream(in_File, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cDATA:
           if (hasReadDATA)
           {
             std::cerr << "Error: FACT seems to have more than one DATA subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cDATA, flags, false))
             return false;
           bytesRead += 6;
           hasReadDATA = true;
           break;
      case cJAIL:
           if (exteriorJailMarkerRefID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one JAIL subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cJAIL, exteriorJailMarkerRefID, false))
             return false;
           bytesRead += 6;
           if (exteriorJailMarkerRefID == 0)
           {
             std::cerr << "Error: JAIL subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cWAIT:
           if (followerWaitMarkerRefID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one WAIT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cWAIT, followerWaitMarkerRefID, false))
             return false;
           bytesRead += 6;
           if (followerWaitMarkerRefID == 0)
           {
             std::cerr << "Error: WAIT subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cSTOL:
           if (stolenGoodsContainerRefID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one STOL subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cSTOL, stolenGoodsContainerRefID, false))
             return false;
           bytesRead += 6;
           if (stolenGoodsContainerRefID == 0)
           {
             std::cerr << "Error: STOL subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cPLCN:
           if (playerInventoryContainerRefID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one PLCN subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cPLCN, playerInventoryContainerRefID, false))
             return false;
           bytesRead += 6;
           if (playerInventoryContainerRefID == 0)
           {
             std::cerr << "Error: PLCN subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cCRGR:
           if (crimeFactionListFormID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one CRGR subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cCRGR, crimeFactionListFormID, false))
             return false;
           bytesRead += 6;
           if (crimeFactionListFormID == 0)
           {
             std::cerr << "Error: CRGR subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cJOUT:
           if (jailOutfitFormID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one JOUT subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cJOUT, jailOutfitFormID, false))
             return false;
           bytesRead += 6;
           if (jailOutfitFormID == 0)
           {
             std::cerr << "Error: JOUT subrecord of FACT has value zero!\n";
             return false;
           }
           break;
      case cCRVA:
           if (unknownCRVA.isPresent())
           {
             std::cerr << "Error: FACT seems to have more than one CRVA subrecord!\n";
             return false;
           }
           if (!unknownCRVA.loadFromStream(in_File, cCRVA, false))
           {
             std::cerr << "Error while reading subrecord CRVA of FACT!\n";
             return false;
           }
           subLength = unknownCRVA.size();
           bytesRead += (2 + subLength);
           // length check
           if ((subLength != 20) && (subLength != 16)  && (subLength != 12))
           {
             std::cerr << "Error: Subrecord CRVA of FACT has invalid length ("
                       << subLength << " bytes). Should be 12 or 16 or 20 bytes!\n";
             return false;
           }
           break;
      case cRNAM:
           if (hasUnpushedRank)
           {
             ranks.push_back(tempRank);
           }
           tempRank.index = 0;
           tempRank.maleName.reset();
           tempRank.femaleName.reset();
           if (!loadUint32SubRecordFromStream(in_File, cRNAM, tempRank.index, false))
             return false;
           bytesRead += 6;
           hasUnpushedRank = true;
           break;
      case cMNAM:
           if (!hasUnpushedRank)
           {
             std::cerr << "Error while reading record of type FACT: MNAM "
                       << "without previous RNAM subrecord encountered!\n";
             return false;
           }
           if (tempRank.maleName.isPresent())
           {
             std::cerr << "Error: FACT seems to have more than one MNAM "
                       << "subrecord per RNAM subrecord!\n";
             return false;
           }
           if (!tempRank.maleName.loadFromStream(in_File, cMNAM, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cFNAM:
           if (!hasUnpushedRank)
           {
             std::cerr << "Error while reading record of type FACT: FNAM "
                       << "without previous RNAM subrecord encountered!\n";
             return false;
           }
           if (tempRank.femaleName.isPresent())
           {
             std::cerr << "Error: FACT seems to have more than one FNAM "
                       << "subrecord per RNAM subrecord!\n";
             return false;
           }
           if (!tempRank.femaleName.loadFromStream(in_File, cFNAM, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cVEND:
           if (vendorListFormID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one VEND subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cVEND, vendorListFormID, false))
             return false;
           bytesRead += 6;
           if (vendorListFormID == 0)
           {
             std::cerr << "Error: Subrecord VEND of FACT has value zero!\n";
             return false;
           }
           break;
      case cVENC:
           if (vendorContainterFormID != 0)
           {
             std::cerr << "Error: FACT seems to have more than one VENC subrecord!\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(in_File, cVENC, vendorContainterFormID, false))
             return false;
           bytesRead += 6;
           if (vendorContainterFormID == 0)
           {
             std::cerr << "Error: Subrecord VENC of FACT has value zero!\n";
             return false;
           }
           break;
      case cVENV:
           if (vendorStuff.isPresent)
           {
             std::cerr << "Error: FACT seems to have more than one VENV subrecord!\n";
             return false;
           }
           // VENV's length
           in_File.read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Subrecord VENV of FACT has invalid length ("
                       << subLength << " bytes). Should be 12 bytes!\n";
             return false;
           }
           // read VENV's stuff
           in_File.read(reinterpret_cast<char*>(&vendorStuff.startHour), 2);
           in_File.read(reinterpret_cast<char*>(&vendorStuff.endHour), 2);
           in_File.read(reinterpret_cast<char*>(&vendorStuff.radius), 4);
           in_File.read(reinterpret_cast<char*>(&vendorStuff.flagsVendor), 4);
           bytesRead += 12;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord VENV of FACT!\n";
             return false;
           }
           vendorStuff.isPresent = true;
           break;
      case cPLVD:
           if (unknownPLVD.isPresent())
           {
             std::cerr << "Error: FACT seems to have more than one PLVD subrecord!\n";
             return false;
           }
           if (!unknownPLVD.loadFromStream(in_File, cPLVD, false))
           {
             std::cerr << "Error while reading subrecord PLVD of FACT!\n";
             return false;
           }
           // check length of PLVD
           if (unknownPLVD.size() != 12)
           {
             std::cerr << "Error: Subrecord PLVD of FACT has invalid length ("
                       << unknownPLVD.size() << " bytes). Should be 12 bytes!\n";
             return false;
           }
           bytesRead += (2 + 12);
           break;
      case cCITC:
           if (!conditions.empty() || (cond_count != 0))
           {
             std::cerr << "Error: FACT seems to have more than one CITC subrecord!\n";
             return false;
           }
           // read CITC
           if (!loadUint32SubRecordFromStream(in_File, cCITC, cond_count, false))
             return false;
           bytesRead += 6;

           if (cond_count == 0)
           {
             std::cerr << "Error: Subrecord CITC of FACT has value zero!\n";
             return false;
           }
           break;
      case cCTDA:
           if (cond_count == 0)
           {
             std::cerr << "Error while reading record of type FACT: CTDA "
                       << "without previous CITC subrecord encountered!\n";
             return false;
           }
           // push what's still left
           if (hasUnpushedCTDA_CIS2)
           {
             conditions.push_back(tempCTDA_CIS2);
           }
           if (conditions.size() >= cond_count)
           {
             std::cerr << "Error while reading record of type FACT: found more"
                       << " CTDA records than CITC's value suggested!\n";
             return false;
           }
           // clear stuff
           tempCTDA_CIS2.unknownCTDA.clear();
           tempCTDA_CIS2.unknownCISx.clear();
           // read CTDA's data
           if (!tempCTDA_CIS2.unknownCTDA.loadFromStream(in_File, bytesRead))
           {
             std::cerr << "Error while reading subrecord CTDA of FACT!\n";
             return false;
           }
           hasUnpushedCTDA_CIS2 = true;
           break;
      case cCIS2:
           if (!hasUnpushedCTDA_CIS2)
           {
             std::cerr << "Error while reading record of type FACT: CIS2 "
                       << "without previous CTDA subrecord encountered!\n";
             return false;
           }
           if (!tempCTDA_CIS2.unknownCISx.empty())
           {
             std::cerr << "Error: FACT seems to have more than one CIS2 "
                       << "subrecord per CTDA subrecord!\n";
             return false;
           }
           if (!loadString512FromStream(in_File, tempCTDA_CIS2.unknownCISx, buffer, cCIS2, false, bytesRead))
             return false;
           if (tempCTDA_CIS2.unknownCISx.empty())
           {
             std::cerr << "Error: Subrecord CIS2 of FACT is empty!\n";
             return false;
           }
           conditions.push_back(tempCTDA_CIS2);
           hasUnpushedCTDA_CIS2 = false;
           break;
      default:
           std::cerr << "Error: Unexpected record type \""<<IntTo4Char(subRecName)
                     << "\" found, but only XNAM, FULL, DATA, JAIL, WAIT, STOL,"
                     << " PLCN, CRGR, JOUT, CRVA, RNAM, MNAM, FNAM, VEND, VENC,"
                     << " VENV, PLVD, CITC, CTDA or CIS2 are allowed here!\n";
           return false;
           break;
    }
  }

  // handle remaining rank data chunk
  if (hasUnpushedRank)
  {
    ranks.push_back(tempRank);
  }

  // push what's still left for conditions
  if (hasUnpushedCTDA_CIS2)
  {
    conditions.push_back(tempCTDA_CIS2);
  }
  // size check for conditions
  if (conditions.size() != cond_count)
  {
    std::cerr << "Error while reading FACT: Expected " << cond_count
              << " condition record(s), but only " << conditions.size()
              << " record(s) was/were found!\n";
    return false;
  }

  // presence checks
  if (!hasReadDATA)
  {
    std::cerr << "Error: At least one required subrecord of FACT is missing!\n";
    return false;
  }

  return in_File.good();
}

uint32_t FactionRecord::getRecordType() const
{
  return cFACT;
}

} // namespace
