/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013, 2021  Dirk Stolle

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

#include "NPCRecord.hpp"
#include <iostream>
#include "../SR_Constants.hpp"
#include "../../mw/HelperIO.hpp"
#include "../../base/CompressionFunctions.hpp"
#include "../../base/BufferStream.hpp"

namespace SRTP
{

/* FactionElem's functions */

NPCRecord::FactionElem::FactionElem()
: formID(0),
  rank(0)
{
}

bool NPCRecord::FactionElem::operator==(const FactionElem& other) const
{
  return (formID == other.formID) && (rank == other.rank);
}

/* PerkElem's functions */

NPCRecord::PerkElem::PerkElem()
: formID(0),
  valueTwo(0)
{
}

bool NPCRecord::PerkElem::operator==(const NPCRecord::PerkElem& other) const
{
  return (formID == other.formID) && (valueTwo == other.valueTwo);
}

/* TINXstructure's functions */

NPCRecord::TINXstructure::TINXstructure()
: unknownTINI(0),
  unknownTINC(0),
  unknownTINV(0),
  unknownTIAS(0)
{
}

bool NPCRecord::TINXstructure::operator==(const NPCRecord::TINXstructure& other) const
{
  return (unknownTINI == other.unknownTINI) && (unknownTINC == other.unknownTINC)
      && (unknownTINV == other.unknownTINV) && (unknownTIAS == other.unknownTIAS);
}

/* CSDXstruct's functions */

NPCRecord::CSDXstruct::CSDXstruct()
: unknownCSDT(0),
  unknownCSDI(0),
  unknownCSDC(0)
{
}

bool NPCRecord::CSDXstruct::operator==(const NPCRecord::CSDXstruct& other) const
{
  return (unknownCSDT == other.unknownCSDT) && (unknownCSDI == other.unknownCSDI)
      && (unknownCSDC == other.unknownCSDC);
}

/* NPCRecord's functions */

NPCRecord::NPCRecord()
: BasicRecord(),
  editorID(""),
  unknownVMAD(BinarySubRecord()),
  unknownOBND({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  unknownACBS({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  factions(std::vector<FactionElem>()),
  deathItemFormID(0),
  voiceTypeFormID(0),
  templateActorBaseFormID(0),
  raceFormID(0),
  unknownDEST(std::nullopt),
  spellFormIDs(std::vector<uint32_t>()),
  skinFormID(0),
  farAwayModelSkinFormID(0),
  unknownATKR(std::nullopt),
  unknownATKD(BinarySubRecord()),
  unknownATKE(""),
  perkList(std::vector<PerkElem>()),
  items(std::vector<ComponentData>()),
  spectatorOverridePackageListFormID(0),
  combatOverridePackageListFormID(0),
  unknownAIDT({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  unknownPKIDs(std::vector<uint32_t>()),
  keywords(std::vector<uint32_t>()),
  classFormID(0),
  name(LocalizedString()),
  unknownSHRT(std::nullopt),
  unknownDNAM({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  unknownPNAMs(std::vector<uint32_t>()),
  hairColorFormID(0),
  giftFilterFormID(0),
  combatStyleFormID(0),
  unknownNAM5(0),
  unknownNAM6(0),
  unknownNAM7(0),
  unknownNAM8(0),
  soundTemplateFormID(0),
  defaultOutfitFormID(0),
  sleepOutfitFormID(0),
  crimeFactionFormID(0),
  unknownCSDXs(std::vector<CSDXstruct>()),
  defaultPackageListFormID(0),
  faceComplexionFormID(0),
  unknownQNAM({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
  unknownNAM9(BinarySubRecord()),
  unknownNAMA(BinarySubRecord()),
  unknownTINXs(std::vector<TINXstructure>())
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool NPCRecord::equals(const NPCRecord& other) const
{
  return equalsBasic(other) && (editorID == other.editorID)
      && (unknownVMAD == other.unknownVMAD)
      && (unknownOBND == other.unknownOBND)
      && (unknownACBS == other.unknownACBS)
      && (factions == other.factions)
      && (deathItemFormID == other.deathItemFormID)
      && (voiceTypeFormID == other.voiceTypeFormID)
      && (templateActorBaseFormID == other.templateActorBaseFormID)
      && (raceFormID == other.raceFormID)
      && (unknownDEST == other.unknownDEST)
      && (spellFormIDs == other.spellFormIDs)
      && (skinFormID == other.skinFormID)
      && (farAwayModelSkinFormID == other.farAwayModelSkinFormID)
      && (unknownATKR == other.unknownATKR)
      && (unknownATKD == other.unknownATKD) && (unknownATKE == other.unknownATKE)
      && (perkList == other.perkList)
      && (items == other.items)
      && (spectatorOverridePackageListFormID == other.spectatorOverridePackageListFormID)
      && (combatOverridePackageListFormID == other.combatOverridePackageListFormID)
      && (unknownAIDT == other.unknownAIDT)
      && (unknownPKIDs == other.unknownPKIDs) && (keywords == other.keywords)
      && (classFormID == other.classFormID)
      && (name == other.name)
      && (unknownSHRT == other.unknownSHRT)
      && (unknownDNAM == other.unknownDNAM)
      && (unknownPNAMs == other.unknownPNAMs)
      && (hairColorFormID == other.hairColorFormID)
      && (giftFilterFormID == other.giftFilterFormID)
      && (combatStyleFormID == other.combatStyleFormID)
      && (unknownNAM5 == other.unknownNAM5) && (unknownNAM6 == other.unknownNAM6)
      && (unknownNAM7 == other.unknownNAM7) && (unknownNAM8 == other.unknownNAM8)
      && (soundTemplateFormID == other.soundTemplateFormID)
      && (defaultOutfitFormID == other.defaultOutfitFormID)
      && (sleepOutfitFormID == other.sleepOutfitFormID)
      && (crimeFactionFormID == other.crimeFactionFormID)
      && (unknownCSDXs == other.unknownCSDXs)
      && (defaultPackageListFormID==other.defaultPackageListFormID)
      && (faceComplexionFormID==other.faceComplexionFormID)
      && (unknownQNAM == other.unknownQNAM)
      && (unknownNAM9 == other.unknownNAM9) && (unknownNAMA == other.unknownNAMA)
      && (unknownTINXs == other.unknownTINXs);
}
#endif

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t NPCRecord::getWriteSize() const
{
  uint32_t writeSize = 4 /* EDID */ + 2 /* 2 bytes for length */
        + editorID.length() + 1 /* length of name +1 byte for NUL termination */
        + 4 /* OBND */ + 2 /* 2 bytes for length */ + 12 /* fixed size */
        + 4 /* ACBS */ + 2 /* 2 bytes for length */ + 24 /* fixed size */
        + factions.size() * (4 /* SNAM */ + 2 /* 2 bytes for length */ + 8 /* fixed size */)
        + 4 /* RNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* AIDT */ + 2 /* 2 bytes for length */ + 20 /* fixed size */
        + 4 /* CNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* DATA */ + 2 /* 2 bytes for length */ + 0 /* fixed size */
        + 4 /* DNAM */ + 2 /* 2 bytes for length */ + 52 /* fixed size */
        + unknownPNAMs.size() * (4 /* PNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */)
        + 4 /* NAM5 */ + 2 /* 2 bytes for length */ + 2 /* fixed size */
        + 4 /* NAM6 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* NAM7 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* NAM8 */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + 4 /* QNAM */ + 2 /* 2 bytes for length */ + 12 /* fixed size */;
  if (unknownVMAD.isPresent())
  {
    writeSize = writeSize + 4 /* VMAD */ + 2 /* 2 bytes for length */ + unknownVMAD.size() /* size of subrecord */;
  }
  if (deathItemFormID != 0)
  {
    writeSize = writeSize + 4 /* INAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (voiceTypeFormID != 0)
  {
    writeSize = writeSize + 4 /* VTCK */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (templateActorBaseFormID != 0)
  {
    writeSize = writeSize + 4 /* TPLT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownDEST.has_value())
  {
    writeSize = writeSize + 4 /* DEST */ + 2 /* 2 bytes for length */ + 8 /* fixed size */;
  }
  if (!spellFormIDs.empty())
  {
    writeSize = writeSize + 4 /* SPCT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
        + spellFormIDs.size() * (4 /* SPLO */ + 2 /* 2 bytes for length */ + 4 /* fixed size */);
  }
  if (skinFormID != 0)
  {
    writeSize = writeSize + 4 /* WNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (farAwayModelSkinFormID != 0)
  {
    writeSize = writeSize + 4 /* ANAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownATKR.has_value())
  {
    writeSize = writeSize + 4 /* ATKR */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownATKD.isPresent())
  {
    writeSize = writeSize + 4 /* ATKD */ + 2 /* 2 bytes for length */ + unknownATKD.size() /* size */;
  }
  if (!unknownATKE.empty())
  {
    writeSize = writeSize + 4 /* ATKE */ + 2 /* 2 bytes for length */
              + unknownATKE.length() + 1 /* length + NUL */;
  }
  if (!perkList.empty())
  {
    writeSize = writeSize + 4 /* PRKZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
              + perkList.size() * (4 /* PRKR */ + 2 /* 2 bytes for length */ + 8 /* fixed size */);
  }
  if (!items.empty())
  {
    writeSize = writeSize + 4 /* COCT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
              + items.size() * (4 /* CNTO */ + 2 /* 2 bytes for length */ + 8 /* fixed size */);
  }
  if (combatOverridePackageListFormID != 0)
  {
    writeSize = writeSize + 4 /* ECOR */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (spectatorOverridePackageListFormID != 0)
  {
    writeSize = writeSize + 4 /* SPOR */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (!unknownPKIDs.empty())
  {
    writeSize = writeSize + unknownPKIDs.size() * (4 /* PKID */ + 2 /* 2 bytes for length */ + 4 /* fixed size */);
  }
  if (!keywords.empty())
  {
    writeSize = writeSize + 4 /* KSIZ */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
              + 4 /* KWDA */ + 2 /* 2 bytes for length */ + 4 * keywords.size() /* fixed size */;
  }
  if (name.isPresent())
  {
    writeSize += name.getWriteSize();
  }
  if (unknownSHRT.has_value())
  {
    writeSize = writeSize + 4 /* SHRT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (hairColorFormID != 0)
  {
    writeSize = writeSize + 4 /* HCLF */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (giftFilterFormID != 0)
  {
    writeSize = writeSize + 4 /* GNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (combatStyleFormID != 0)
  {
    writeSize = writeSize + 4 /* ZNAM */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (soundTemplateFormID != 0)
  {
    writeSize = writeSize + 4 /* CSCR */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (defaultOutfitFormID != 0)
  {
    writeSize = writeSize + 4 /* DOFT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (sleepOutfitFormID != 0)
  {
    writeSize = writeSize + 4 /* SOFT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (crimeFactionFormID != 0)
  {
    writeSize = writeSize + 4 /* CRIF */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (!unknownCSDXs.empty())
  {
    writeSize = writeSize + unknownCSDXs.size() * (4 /* CSDT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
              + 4 /* CSDI */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
              + 4 /* CSDC */ + 2 /* 2 bytes for length */ + 1 /* fixed size */);
  }
  if (defaultPackageListFormID != 0)
  {
    writeSize = writeSize + 4 /* DPLT */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (faceComplexionFormID != 0)
  {
    writeSize = writeSize + 4 /* FTST */ + 2 /* 2 bytes for length */ + 4 /* fixed size */;
  }
  if (unknownNAM9.isPresent())
  {
    writeSize = writeSize + 4 /* NAM9 */ + 2 /* 2 bytes for length */ + unknownNAM9.size() /* size of subrecord */;
  }
  if (unknownNAMA.isPresent())
  {
    writeSize = writeSize + 4 /* NAMA */ + 2 /* 2 bytes for length */ + unknownNAMA.size() /* size of subrecord */;
  }
  if (!unknownTINXs.empty())
  {
    writeSize = writeSize + unknownTINXs.size() * (4 /* TINI */ + 2 /* 2 bytes for length */ + 2 /* fixed size */
               + 4 /* TINC */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
               + 4 /* TINV */ + 2 /* 2 bytes for length */ + 4 /* fixed size */
               + 4 /* TIAS */ + 2 /* 2 bytes for length */ + 2 /* fixed size */);
  }
  return writeSize;
}

bool NPCRecord::saveToStream(std::ostream& output) const
{
  output.write(reinterpret_cast<const char*>(&cNPC_), 4);
  if (!saveSizeAndUnknownValues(output, getWriteSize()))
    return false;

  // write editor ID (EDID)
  output.write(reinterpret_cast<const char*>(&cEDID), 4);
  uint16_t subLength = editorID.length() + 1;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(editorID.c_str(), subLength);

  if (unknownVMAD.isPresent())
  {
    if (!unknownVMAD.saveToStream(output, cVMAD))
    {
      std::cerr << "Error while writing subrecord VMAD of NPC_!\n!";
      return false;
    }
  }

  // write object bounds (OBND)
  output.write(reinterpret_cast<const char*>(&cOBND), 4);
  subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownOBND.data()), 12);

  // write ACBS
  output.write(reinterpret_cast<const char*>(&cACBS), 4);
  subLength = 24;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownACBS.data()), 24);

  for (const auto& faction: factions)
  {
    // write SNAM
    output.write(reinterpret_cast<const char*>(&cSNAM), 4);
    subLength = 8;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    // write faction data
    output.write(reinterpret_cast<const char*>(&faction.formID), 4);
    output.write(reinterpret_cast<const char*>(&faction.rank), 4);
  }

  if (deathItemFormID != 0)
  {
    // write death item form ID (INAM)
    output.write(reinterpret_cast<const char*>(&cINAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&deathItemFormID), 4);
  }

  if (voiceTypeFormID != 0)
  {
    // write voice type form ID (VTCK)
    output.write(reinterpret_cast<const char*>(&cVTCK), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&voiceTypeFormID), 4);
  }

  if (templateActorBaseFormID != 0)
  {
    // write template data's actor base form ID (TPLT)
    output.write(reinterpret_cast<const char*>(&cTPLT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&templateActorBaseFormID), 4);
  }

  // write race form ID (RNAM)
  output.write(reinterpret_cast<const char*>(&cRNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&raceFormID), 4);

  if (unknownDEST.has_value())
  {
    // write DEST
    output.write(reinterpret_cast<const char*>(&cDEST), 4);
    subLength = 8;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownDEST.value()), 8);
  }

  if (!spellFormIDs.empty())
  {
    const uint32_t count = spellFormIDs.size();
    // write spell count (SPCT)
    output.write(reinterpret_cast<const char*>(&cSPCT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&count), 4);

    for (const uint32_t spellFormID: spellFormIDs)
    {
      // write SPLO
      output.write(reinterpret_cast<const char*>(&cSPLO), 4);
      subLength = 4;
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      output.write(reinterpret_cast<const char*>(&spellFormID), 4);
    }
  }

  if (skinFormID != 0)
  {
    // write skin form ID (WNAM)
    output.write(reinterpret_cast<const char*>(&cWNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&skinFormID), 4);
  }

  if (farAwayModelSkinFormID != 0)
  {
    // write ANAM
    output.write(reinterpret_cast<const char*>(&cANAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&farAwayModelSkinFormID), 4);
  }

  if (unknownATKR.has_value())
  {
    // write ATKR
    output.write(reinterpret_cast<const char*>(&cATKR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownATKR.value()), 4);
  }

  if (unknownATKD.isPresent())
  {
    if (!unknownATKD.saveToStream(output, cATKD))
    {
      std::cerr << "Error while writing subrecord ATKD of NPC_!\n!";
      return false;
    }
  }

  if (!unknownATKE.empty())
  {
    // write ATKE
    output.write(reinterpret_cast<const char*>(&cATKE), 4);
    subLength = unknownATKE.length() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(unknownATKE.c_str(), subLength);
  }

  if (!perkList.empty())
  {
    const uint32_t count = perkList.size();
    // write perk count (PRKZ)
    output.write(reinterpret_cast<const char*>(&cPRKZ), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&count), 4);

    for (const auto& perk: perkList)
    {
      // write perk (PRKR)
      output.write(reinterpret_cast<const char*>(&cPRKR), 4);
      subLength = 8;
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      output.write(reinterpret_cast<const char*>(&perk.formID), 4);
      output.write(reinterpret_cast<const char*>(&perk.valueTwo), 4);
    }
  }

  if (!items.empty())
  {
    const uint32_t count = items.size();
    // write component count (COCT)
    output.write(reinterpret_cast<const char*>(&cCOCT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&count), 4);

    for (const auto& item: items)
    {
      // write component (CNTO)
      output.write(reinterpret_cast<const char*>(&cCNTO), 4);
      subLength = 8;
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      output.write(reinterpret_cast<const char*>(&item.formID), 4);
      output.write(reinterpret_cast<const char*>(&item.count), 4);
    }
  }

  if (spectatorOverridePackageListFormID != 0)
  {
    // write Spectator Override Package List Form ID (SPOR)
    output.write(reinterpret_cast<const char*>(&cSPOR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&spectatorOverridePackageListFormID), 4);
  }

  if (combatOverridePackageListFormID != 0)
  {
    // write Combat Override Package List Form ID (ECOR)
    output.write(reinterpret_cast<const char*>(&cECOR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&combatOverridePackageListFormID), 4);
  }

  // write AIDT
  output.write(reinterpret_cast<const char*>(&cAIDT), 4);
  subLength = 20;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownAIDT.data()), 20);

  for (const uint32_t pkid: unknownPKIDs)
  {
    // write PKID
    output.write(reinterpret_cast<const char*>(&cPKID), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&pkid), 4);
  }

  if (!keywords.empty())
  {
    const uint32_t k_Size = keywords.size();
    // write keyword size (KSIZ)
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&k_Size), 4);

    // write keyword array (KWDA)
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    for (const uint32_t keyword: keywords)
    {
      output.write(reinterpret_cast<const char*>(&keyword), 4);
    }
  }

  // write class form ID (CNAM)
  output.write(reinterpret_cast<const char*>(&cCNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&classFormID), 4);

  if (name.isPresent())
  {
    if (!name.saveToStream(output, cFULL))
      return false;
  }

  if (unknownSHRT.has_value())
  {
    // write SHRT
    output.write(reinterpret_cast<const char*>(&cSHRT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownSHRT.value()), 4);
  }

  // write DATA
  output.write(reinterpret_cast<const char*>(&cDATA), 4);
  subLength = 0; // Yes, size is really zero.
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  // zero size - nothing to write

  // write DNAM
  output.write(reinterpret_cast<const char*>(&cDNAM), 4);
  subLength = 52;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownDNAM.data()), 52);

  for (const uint32_t pnam: unknownPNAMs)
  {
    // write PNAM
    output.write(reinterpret_cast<const char*>(&cPNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&pnam), 4);
  }

  if (hairColorFormID != 0)
  {
    // write Hair Color Form ID (HCLF)
    output.write(reinterpret_cast<const char*>(&cHCLF), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&hairColorFormID), 4);
  }

  if (giftFilterFormID != 0)
  {
    // write gift filter form ID (GNAM)
    output.write(reinterpret_cast<const char*>(&cGNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&giftFilterFormID), 4);
  }

  if (combatStyleFormID != 0)
  {
    // write combat style form ID (ZNAM)
    output.write(reinterpret_cast<const char*>(&cZNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&combatStyleFormID), 4);
  }

  // write NAM5
  output.write(reinterpret_cast<const char*>(&cNAM5), 4);
  subLength = 2;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownNAM5), 2);

  // write NAM6
  output.write(reinterpret_cast<const char*>(&cNAM6), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownNAM6), 4);

  // write NAM7
  output.write(reinterpret_cast<const char*>(&cNAM7), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownNAM7), 4);

  // write NAM8
  output.write(reinterpret_cast<const char*>(&cNAM8), 4);
  subLength = 4; // fixed size
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownNAM8), 4);

  if (soundTemplateFormID != 0)
  {
    // write sound template form ID ("Inherits Sounds From..."), CSCR
    output.write(reinterpret_cast<const char*>(&cCSCR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&soundTemplateFormID), 4);
  }

  if (defaultOutfitFormID != 0)
  {
    // write default outfit form ID (DOFT)
    output.write(reinterpret_cast<const char*>(&cDOFT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&defaultOutfitFormID), 4);
  }

  if (sleepOutfitFormID != 0)
  {
    // write sleep outfit form ID (SOFT)
    output.write(reinterpret_cast<const char*>(&cSOFT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&sleepOutfitFormID), 4);
  }

  if (crimeFactionFormID != 0)
  {
    // write crime faction form ID (CRIF)
    output.write(reinterpret_cast<const char*>(&cCRIF), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&crimeFactionFormID), 4);
  }

  for (const auto& element: unknownCSDXs)
  {
    // write CSDT
    output.write(reinterpret_cast<const char*>(&cCSDT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&element.unknownCSDT), 4);

    // write CSDI
    output.write(reinterpret_cast<const char*>(&cCSDI), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&element.unknownCSDI), 4);

    // write CSDC
    output.write(reinterpret_cast<const char*>(&cCSDC), 4);
    subLength = 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&element.unknownCSDC), 1);
  }

  if (defaultPackageListFormID !=0)
  {
    // write default package list form ID (DPLT)
    output.write(reinterpret_cast<const char*>(&cDPLT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&defaultPackageListFormID), 4);
  }

  if (faceComplexionFormID != 0)
  {
    // write Face Complexion Form ID (FTST)
    output.write(reinterpret_cast<const char*>(&cFTST), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&faceComplexionFormID), 4);
  }

  // write QNAM
  output.write(reinterpret_cast<const char*>(&cQNAM), 4);
  subLength = 12;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(unknownQNAM.data()), 12);

  if (unknownNAM9.isPresent())
  {
    if (!unknownNAM9.saveToStream(output, cNAM9))
    {
      std::cerr << "Error while writing subrecord NAM9 of NPC_!\n!";
      return false;
    }
  }

  if (unknownNAMA.isPresent())
  {
    if (!unknownNAMA.saveToStream(output, cNAMA))
    {
      std::cerr << "Error while writing subrecord NAMA of NPC_!\n!";
      return false;
    }
  }

  for (const auto& element: unknownTINXs)
  {
    // write TINI
    output.write(reinterpret_cast<const char*>(&cTINI), 4);
    subLength = 2;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&element.unknownTINI), 2);

    // write TINC
    output.write(reinterpret_cast<const char*>(&cTINC), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&element.unknownTINC), 4);

    // write TINV
    output.write(reinterpret_cast<const char*>(&cTINV), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&element.unknownTINV), 4);

    // write TIAS
    output.write(reinterpret_cast<const char*>(&cTIAS), 4);
    subLength = 2;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&element.unknownTIAS), 2);
  }

  return output.good();
}
#endif

bool NPCRecord::loadFromStream(std::istream& in_File, const bool localized, const StringTable& table)
{
  uint32_t readSize = 0;
  if (!loadSizeAndUnknownValues(in_File, readSize))
    return false;
  uint32_t subRecName = 0;
  uint16_t subLength = 0;
  uint32_t bytesRead = 0;

  MWTP::BufferStream decompStream(nullptr, 0);
  std::basic_istream<char>* actual_in = &in_File;

  if (isCompressed())
  {
    uint32_t decompressedSize = 0;
    // read size of decompressed data
    in_File.read(reinterpret_cast<char*>(&decompressedSize), 4);
    if (!in_File.good())
    {
      std::cerr << "Error while reading decompression size of NPC_!\n";
      return false;
    }
    if (readSize <= 4)
    {
      std::cerr << "Error: Size of compressed NPC_ record is too small to contain any compressed data!\n";
      return false;
    }
    // buffer to read compressed data
    uint8_t * stream_buffer = new uint8_t[readSize - 4];
    in_File.read(reinterpret_cast<char*>(stream_buffer), readSize - 4);
    if (!in_File.good())
    {
      std::cerr << "Error while reading compressed data of NPC_!\n";
      delete[] stream_buffer;
      return false;
    }
    // now decompress here
    uint8_t * decompBuffer = new uint8_t [decompressedSize];
    if (!MWTP::zlibDecompress(stream_buffer, readSize - 4, decompBuffer, decompressedSize))
    {
      std::cerr << "Error while executing decompression function!\n";
      delete[] stream_buffer;
      stream_buffer = nullptr;
      delete[] decompBuffer;
      decompBuffer = nullptr;
      return false;
    }
    delete[] stream_buffer;
    stream_buffer = nullptr;
    // set underlying buffer for decompressed stream
    decompStream.buffer(reinterpret_cast<char*>(decompBuffer), decompressedSize);
    actual_in = &decompStream;
    // update read size
    readSize = decompressedSize;
  }

  // read EDID
  char buffer[512];
  if (!loadString512FromStream(*actual_in, editorID, buffer, cEDID, true, bytesRead))
    return false;

  unknownVMAD.setPresence(false);
  bool hasReadOBND = false;
  bool hasReadACBS = false;
  factions.clear();
  FactionElem tempFactionElem;
  deathItemFormID = 0;
  voiceTypeFormID = 0;
  templateActorBaseFormID = 0;
  raceFormID = 0;
  unknownDEST.reset();
  skinFormID = 0;
  farAwayModelSkinFormID = 0;
  unknownATKR.reset();
  unknownATKD.setPresence(false);
  unknownATKE.clear();
  spellFormIDs.clear();
  uint32_t tempUint32, size_int;
  perkList.clear();
  PerkElem tempPerk;
  items.clear();
  ComponentData tempComponent;
  spectatorOverridePackageListFormID = 0;
  combatOverridePackageListFormID = 0;
  bool hasReadAIDT = false;
  unknownPKIDs.clear();
  keywords.clear();
  classFormID = 0;
  name.reset();
  unknownSHRT.reset();
  bool hasReadDNAM = false;
  unknownPNAMs.clear();
  hairColorFormID = 0;
  giftFilterFormID = 0;
  combatStyleFormID = 0;
  bool hasReadNAM5 = false; unknownNAM5 = 0;
  bool hasReadNAM6 = false; unknownNAM6 = 0;
  bool hasReadNAM7 = false; unknownNAM7 = 0;
  bool hasReadNAM8 = false; unknownNAM8 = 0;
  defaultOutfitFormID = 0;
  sleepOutfitFormID = 0;
  soundTemplateFormID = 0;
  crimeFactionFormID = 0;
  unknownCSDXs.clear();
  CSDXstruct tempCSDX;
  defaultPackageListFormID = 0;
  faceComplexionFormID = 0;
  bool hasReadQNAM = false;
  unknownNAM9.setPresence(false);
  unknownNAMA.setPresence(false);
  unknownTINXs.clear();
  TINXstructure tempTINX;
  while (bytesRead < readSize)
  {
    // read next header
    actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
    bytesRead += 4;
    switch (subRecName)
    {
      case cVMAD:
           if (unknownVMAD.isPresent())
           {
             std::cerr << "Error: NPC_ seems to have more than one VMAD subrecord.\n";
             return false;
           }
           if (!unknownVMAD.loadFromStream(*actual_in, cVMAD, false))
           {
             std::cerr << "Error while reading subrecord VMAD of NPC_!\n";
             return false;
           }
           bytesRead += (2 + unknownVMAD.size());
           break;
      case cOBND:
           if (hasReadOBND)
           {
             std::cerr << "Error: NPC_ seems to have more than one OBND subrecord.\n";
             return false;
           }
           // OBND's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record OBND of NPC_ has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read OBND
           actual_in->read(reinterpret_cast<char*>(unknownOBND.data()), 12);
           bytesRead += 12;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord OBND of NPC_!\n";
             return false;
           }
           hasReadOBND = true;
           break;
      case cACBS:
           if (hasReadACBS)
           {
             std::cerr << "Error: NPC_ seems to have more than one ACBS subrecord.\n";
             return false;
           }
           // read ACBS's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 24)
           {
             std::cerr << "Error: Sub record ACBS of NPC_ has invalid length ("
                       << subLength << " bytes). Should be 24 bytes.\n";
             return false;
           }
           // read ACBS
           actual_in->read(reinterpret_cast<char*>(unknownACBS.data()), 24);
           bytesRead += 24;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord ACBS of NPC_!\n";
             return false;
           }
           hasReadACBS = true;
           break;
      case cSNAM:
           // read SNAM's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record SNAM of NPC_ has invalid length ("
                       << subLength << " bytes). Should be 8 bytes.\n";
             return false;
           }
           // read SNAM
           tempFactionElem.formID = 0;
           tempFactionElem.rank = 0;
           actual_in->read(reinterpret_cast<char*>(&tempFactionElem.formID), 4);
           actual_in->read(reinterpret_cast<char*>(&tempFactionElem.rank), 4);
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord SNAM of NPC_!\n";
             return false;
           }
           bytesRead += 8;
           factions.push_back(tempFactionElem);
           break;
      case cINAM:
           if (deathItemFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one INAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cINAM, deathItemFormID, false))
             return false;
           bytesRead += 6;
           if (deathItemFormID == 0)
           {
             std::cerr << "Error: Subrecord INAM of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cVTCK:
           if (voiceTypeFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one VTCK subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cVTCK, voiceTypeFormID, false))
             return false;
           bytesRead += 6;
           if (voiceTypeFormID == 0)
           {
             std::cerr << "Error: Subrecord VTCK of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cTPLT:
           if (templateActorBaseFormID)
           {
             std::cerr << "Error: NPC_ seems to have more than one TPLT subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cTPLT, templateActorBaseFormID, false))
             return false;
           bytesRead += 6;
           if (templateActorBaseFormID == 0)
           {
             std::cerr << "Error: Subrecord TPLT of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cRNAM:
           if (raceFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one RNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cRNAM, raceFormID, false))
             return false;
           bytesRead += 6;
           if (raceFormID == 0)
           {
             std::cerr << "Error: Subrecord RNAM of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cDEST:
           if (unknownDEST.has_value())
           {
             std::cerr << "Error: NPC_ seems to have more than one DEST subrecord.\n";
             return false;
           }
           // DEST's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 8)
           {
             std::cerr << "Error: Sub record DEST of NPC_ has invalid length ("
                       << subLength << " bytes). Should be eight bytes.\n";
             return false;
           }
           // read DEST
           unknownDEST = 0;
           actual_in->read(reinterpret_cast<char*>(&unknownDEST.value()), 8);
           bytesRead += 8;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord DEST of NPC_!\n";
             return false;
           }
           break;
      case cWNAM:
           if (skinFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one WNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cWNAM, skinFormID, false))
             return false;
           bytesRead += 6;
           if (skinFormID == 0)
           {
             std::cerr << "Error: Subrecord WNAM of NPC_ is zero!\n";
             return false;
           }
           break;
      case cANAM:
           if (farAwayModelSkinFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one ANAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cANAM, farAwayModelSkinFormID, false))
             return false;
           bytesRead += 6;
           if (farAwayModelSkinFormID == 0)
           {
             std::cerr << "Error: Subrecord ANAM of NPC_ is zero!\n";
             return false;
           }
           break;
      case cATKR:
           if (unknownATKR.has_value())
           {
             std::cerr << "Error: NPC_ seems to have more than one ATKR subrecord.\n";
             return false;
           }
           unknownATKR = 0;
           if (!loadUint32SubRecordFromStream(*actual_in, cATKR, unknownATKR.value(), false))
             return false;
           bytesRead += 6;
           break;
      case cATKD:
           if (unknownATKD.isPresent())
           {
             std::cerr << "Error: NPC_ seems to have more than one ATKD subrecord.\n";
             return false;
           }
           // read ATKD
           if (!unknownATKD.loadFromStream(*actual_in, cATKD, false))
           {
             std::cerr << "Error while reading subrecord ATKD of NPC_!\n";
             return false;
           }
           bytesRead += (2 + unknownATKD.size());
           if (unknownATKD.size() != 44)
           {
             std::cerr << "Error: Sub record ATKD of NPC_ has invalid length ("
                       << unknownATKD.size() << " bytes). Should be 44 bytes.\n";
             return false;
           }
           break;
      case cATKE:
           if (!unknownATKE.empty())
           {
             std::cerr << "Error: NPC_ seems to have more than one ATKE subrecord.\n";
             return false;
           }
           if (!loadString512FromStream(*actual_in, unknownATKE, buffer, cATKE, false, bytesRead))
             return false;
           if (unknownATKE.empty())
           {
             std::cerr << "Error: Subrecord ATKE of NPC_ is empty.\n";
             return false;
           }
           break;
      case cSPCT:
           if (!spellFormIDs.empty())
           {
             std::cerr << "Error: NPC_ seems to have more than one SPCT subrecord.\n";
             return false;
           }
           // read SPCT
           if (!loadUint32SubRecordFromStream(*actual_in, cSPCT, size_int, false))
             return false;
           bytesRead += 6;
           if (size_int == 0)
           {
             std::cerr << "Error: Value of subrecord SPCT of NPC_ is zero!\n";
             return false;
           }

           for (uint32_t i = 0; i < size_int; ++i)
           {
             // read SPLO
             actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
             bytesRead += 4;
             if (subRecName != cSPLO)
             {
               UnexpectedRecord(cSPLO, subRecName);
               return false;
             }
             // SPLO's length
             actual_in->read(reinterpret_cast<char*>(&subLength), 2);
             bytesRead += 2;
             if (subLength != 4)
             {
               std::cerr << "Error: Sub record SPLO of NPC_ has invalid length ("
                         << subLength << " bytes). Should be four bytes.\n";
               return false;
             }
             // read SPLO
             actual_in->read(reinterpret_cast<char*>(&tempUint32), 4);
             bytesRead += 4;
             if (!actual_in->good())
             {
               std::cerr << "Error while reading subrecord SPLO of NPC_!\n";
               return false;
             }
             spellFormIDs.push_back(tempUint32);
           }
           break;
      case cPRKZ:
           if (!perkList.empty())
           {
             std::cerr << "Error: NPC_ seems to have more than one PRKZ subrecord.\n";
             return false;
           }
           // read PRKZ
           if (!loadUint32SubRecordFromStream(*actual_in, cPRKZ, size_int, false))
             return false;
           bytesRead += 6;
           if (size_int == 0)
           {
             std::cerr << "Error: Value of subrecord PRKZ of NPC_ is zero!\n";
             return false;
           }

           for (uint32_t i = 0; i < size_int; ++i)
           {
             // read PRKR
             actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
             bytesRead += 4;
             if (subRecName != cPRKR)
             {
               UnexpectedRecord(cPRKR, subRecName);
               return false;
             }
             // PRKR's length
             actual_in->read(reinterpret_cast<char*>(&subLength), 2);
             bytesRead += 2;
             if (subLength != 8)
             {
               std::cerr << "Error: Sub record PRKR of NPC_ has invalid length ("
                         << subLength << " bytes). Should be 8 bytes.\n";
               return false;
             }
             // read PRKR
             actual_in->read(reinterpret_cast<char*>(&tempPerk.formID), 4);
             actual_in->read(reinterpret_cast<char*>(&tempPerk.valueTwo), 4);
             bytesRead += 8;
             if (!actual_in->good())
             {
               std::cerr << "Error while reading subrecord PRKR of NPC_!\n";
               return false;
             }
             perkList.push_back(tempPerk);
           }
           break;
      case cCOCT:
           if (!items.empty())
           {
             std::cerr << "Error: NPC_ seems to have more than one COCT subrecord.\n";
             return false;
           }
           // read COCT
           if (!loadUint32SubRecordFromStream(*actual_in, cCOCT, size_int, false))
             return false;
           bytesRead += 6;
           if (size_int == 0)
           {
             std::cerr << "Error: Value of subrecord COCT of NPC_ is zero!\n";
             return false;
           }

           for (uint32_t i = 0; i < size_int; ++i)
           {
             // read CNTO
             actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
             bytesRead += 4;
             if (subRecName != cCNTO)
             {
               UnexpectedRecord(cCNTO, subRecName);
               return false;
             }
             // CNTO's length
             actual_in->read(reinterpret_cast<char*>(&subLength), 2);
             bytesRead += 2;
             if (subLength != 8)
             {
               std::cerr << "Error: Sub record CNTO of NPC_ has invalid length ("
                         << subLength << " bytes). Should be 8 bytes.\n";
               return false;
             }
             // read CNTO
             actual_in->read(reinterpret_cast<char*>(&tempComponent.formID), 4);
             actual_in->read(reinterpret_cast<char*>(&tempComponent.count), 4);
             bytesRead += 8;
             if (!actual_in->good())
             {
               std::cerr << "Error while reading subrecord CNTO of NPC_!\n";
               return false;
             }
             items.push_back(tempComponent);
           }
           break;
      case cSPOR:
           if (spectatorOverridePackageListFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one SPOR subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cSPOR, spectatorOverridePackageListFormID, false))
             return false;
           bytesRead += 6;
           if (spectatorOverridePackageListFormID == 0)
           {
             std::cerr << "Error: Subrecord SPOR of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cECOR:
           if (combatOverridePackageListFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one ECOR subrecord.\n";
             return false;
           }
           // read ECOR
           if (!loadUint32SubRecordFromStream(*actual_in, cECOR, combatOverridePackageListFormID, false))
             return false;
           bytesRead += 6;
           if (combatOverridePackageListFormID == 0)
           {
             std::cerr << "Error: Subrecord ECOR of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cAIDT:
           if (hasReadAIDT)
           {
             std::cerr << "Error: NPC_ seems to have more than one AIDT subrecord.\n";
             return false;
           }
           // AIDT's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 20)
           {
             std::cerr << "Error: Sub record AIDT of NPC_ has invalid length ("
                       << subLength << " bytes). Should be 20 bytes.\n";
             return false;
           }
           //read AIDT
           actual_in->read(reinterpret_cast<char*>(unknownAIDT.data()), 20);
           bytesRead += 20;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord AIDT of NPC_!\n";
             return false;
           }
           hasReadAIDT = true;
           break;
      case cPKID:
           // PKID's length
           if (!loadUint32SubRecordFromStream(*actual_in, cPKID, tempUint32, false))
             return false;
           bytesRead += 6;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord PKID of NPC_!\n";
             return false;
           }
           unknownPKIDs.push_back(tempUint32);
           break;
      case cKSIZ:
           if (!loadKeywords(*actual_in, keywords, bytesRead))
             return false;
           break;
      case cCNAM:
           if (classFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one CNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cCNAM, classFormID, false))
             return false;
           bytesRead += 6;
           if (classFormID == 0)
           {
             std::cerr << "Error: Subrecord CNAM of NPC_ is zero!\n";
             return false;
           }
           break;
      case cFULL:
           if (name.isPresent())
           {
             std::cerr << "Error: NPC_ seems to have more than one FULL subrecord.\n";
             return false;
           }
           if (!name.loadFromStream(*actual_in, cFULL, false, bytesRead, localized, table, buffer))
             return false;
           break;
      case cSHRT:
           if (unknownSHRT.has_value())
           {
             std::cerr << "Error: NPC_ seems to have more than one SHRT subrecord.\n";
             return false;
           }
           // read SHRT
           unknownSHRT = 0;
           if (!loadUint32SubRecordFromStream(*actual_in, cSHRT, unknownSHRT.value(), false))
             return false;
           bytesRead += 6;
           break;
      case cDATA:
           if (hasReadDNAM)
           {
             std::cerr << "Error: NPC_ seems to have more than one DNAM subrecord.\n";
             return false;
           }
           // DATA's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 0)
           {
             std::cerr << "Error: Sub record DATA of NPC_ has invalid length ("
                       << subLength << " bytes). Should be zero bytes.\n";
             return false;
           }
           // DATA is zero bytes - no need to read anything for it.

           // read DNAM
           actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cDNAM)
           {
             UnexpectedRecord(cDNAM, subRecName);
             return false;
           }
           // DNAM's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 52)
           {
             std::cerr << "Error: Sub record DNAM of NPC_ has invalid length ("
                       << subLength << " bytes). Should be 52 bytes.\n";
             return false;
           }
           // read DNAM
           actual_in->read(reinterpret_cast<char*>(unknownDNAM.data()), 52);
           bytesRead += 52;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord DNAM of NPC_!\n";
             return false;
           }
           hasReadDNAM = true;
           break;
      case cPNAM:
           if (!loadUint32SubRecordFromStream(*actual_in, cPNAM, tempUint32, false))
             return false;
           bytesRead += 6;
           unknownPNAMs.push_back(tempUint32);
           break;
      case cHCLF:
           if (hairColorFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one HCLF subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cHCLF, hairColorFormID, false))
             return false;
           bytesRead += 6;
           if (hairColorFormID == 0)
           {
             std::cerr << "Error: Subrecord HCLF of NPC_ is zero!\n";
             return false;
           }
           break;
      case cGNAM:
           if (giftFilterFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one GNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cGNAM, giftFilterFormID, false))
             return false;
           bytesRead += 6;
           if (giftFilterFormID == 0)
           {
             std::cerr << "Error: Subrecord GNAM of NPC_ is zero!\n";
             return false;
           }
           break;
      case cZNAM:
           if (combatStyleFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one ZNAM subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cZNAM, combatStyleFormID, false))
             return false;
           bytesRead += 6;
           if (combatStyleFormID == 0)
           {
             std::cerr << "Error: Subrecord ZNAM of NPC_ is zero!\n";
             return false;
           }
           break;
      case cNAM5:
           if (hasReadNAM5)
           {
             std::cerr << "Error: NPC_ seems to have more than one NAM5 subrecord.\n";
             return false;
           }
           // NAM5's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record NAM5 of NPC_ has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read NAM5
           actual_in->read(reinterpret_cast<char*>(&unknownNAM5), 2);
           bytesRead += 2;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord NAM5 of NPC_!\n";
             return false;
           }
           hasReadNAM5 = true;
           break;
      case cNAM6:
           if (hasReadNAM6)
           {
             std::cerr << "Error: NPC_ seems to have more than one NAM6 subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM6, unknownNAM6, false))
             return false;
           bytesRead += 6;
           hasReadNAM6 = true;
           break;
      case cNAM7:
           if (hasReadNAM7)
           {
             std::cerr << "Error: NPC_ seems to have more than one NAM7 subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM7, unknownNAM7, false))
             return false;
           bytesRead += 6;
           hasReadNAM7 = true;
           break;
      case cNAM8:
           if (hasReadNAM8)
           {
             std::cerr << "Error: NPC_ seems to have more than one NAM8 subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cNAM8, unknownNAM8, false))
             return false;
           bytesRead += 6;
           hasReadNAM8 = true;
           break;
      case cDOFT:
           if (defaultOutfitFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one DOFT subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cDOFT, defaultOutfitFormID, false))
             return false;
           bytesRead += 6;
           if (defaultOutfitFormID == 0)
           {
             std::cerr << "Error: Subrecord DOFT of NPC_ is zero!\n";
             return false;
           }
           break;
      case cSOFT:
           if (sleepOutfitFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one SOFT subrecord.\n";
             return false;
           }
           // read SOFT
           if (!loadUint32SubRecordFromStream(*actual_in, cSOFT, sleepOutfitFormID, false))
             return false;
           bytesRead += 6;
           if (sleepOutfitFormID == 0)
           {
             std::cerr << "Error: Subrecord SOFT of NPC_ is zero!\n";
             return false;
           }
           break;
      case cCRIF:
           if (crimeFactionFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one CRIF subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cCRIF, crimeFactionFormID, false))
             return false;
           bytesRead += 6;
           if (crimeFactionFormID == 0)
           {
             std::cerr << "Error: Subrecord CRIF of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cCSCR:
           if (soundTemplateFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one CSCR subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cCSCR, soundTemplateFormID, false))
             return false;
           bytesRead += 6;
           if (soundTemplateFormID == 0)
           {
             std::cerr << "Error: Subrecord CSCR of NPC_ has value zero!\n";
             return false;
           }
           break;
      case cCSDT:
           // read CSDT
           if (!loadUint32SubRecordFromStream(*actual_in, cCSDT, tempCSDX.unknownCSDT, false))
             return false;
           bytesRead += 6;

           // read CSDI
           if (!loadUint32SubRecordFromStream(*actual_in, cCSDI, tempCSDX.unknownCSDI, true))
             return false;
           bytesRead += 10;

           // read CSDC
           actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cCSDC)
           {
             UnexpectedRecord(cCSDC, subRecName);
             return false;
           }
           // CSDC's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 1)
           {
             std::cerr << "Error: Sub record CSDC of NPC_ has invalid length ("
                       << subLength << " bytes). Should be one byte.\n";
             return false;
           }
           // read CSDC
           actual_in->read(reinterpret_cast<char*>(&tempCSDX.unknownCSDC), 1);
           bytesRead += 1;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord CSDC of NPC_!\n";
             return false;
           }
           unknownCSDXs.push_back(tempCSDX);
           break;
      case cCSDI:
           // set CSDT to zero
           tempCSDX.unknownCSDT = 0;

           // read CSDI
           if (!loadUint32SubRecordFromStream(*actual_in, cCSDI, tempCSDX.unknownCSDI, false))
             return false;
           bytesRead += 6;

           // read CSDC
           actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cCSDC)
           {
             UnexpectedRecord(cCSDC, subRecName);
             return false;
           }
           // CSDC's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 1)
           {
             std::cerr << "Error: Sub record CSDC of NPC_ has invalid length ("
                       << subLength << " bytes). Should be one byte.\n";
             return false;
           }
           // read CSDC
           actual_in->read(reinterpret_cast<char*>(&tempCSDX.unknownCSDC), 1);
           bytesRead += 1;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord CSDC of NPC_!\n";
             return false;
           }
           unknownCSDXs.push_back(tempCSDX);
           break;
      case cDPLT:
           if (defaultPackageListFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one DPLT subrecord.\n";
             return false;
           }
           // read DPLT
           if (!loadUint32SubRecordFromStream(*actual_in, cDPLT, defaultPackageListFormID, false))
             return false;
           bytesRead += 6;
           // check value
           if (defaultPackageListFormID == 0)
           {
             std::cerr << "Error: Subrecord DPLT of NPC_ is zero!\n";
             return false;
           }
           break;
      case cFTST:
           if (faceComplexionFormID != 0)
           {
             std::cerr << "Error: NPC_ seems to have more than one FTST subrecord.\n";
             return false;
           }
           if (!loadUint32SubRecordFromStream(*actual_in, cFTST, faceComplexionFormID, false))
             return false;
           bytesRead += 6;
           if (faceComplexionFormID == 0)
           {
             std::cerr << "Error: Subrecord FTST of NPC_ is zero!\n";
             return false;
           }
           break;
      case cQNAM:
           if (hasReadQNAM)
           {
             std::cerr << "Error: NPC_ seems to have more than one QNAM subrecord.\n";
             return false;
           }
           // QNAM's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 12)
           {
             std::cerr << "Error: Sub record QNAM of NPC_ has invalid length ("
                       << subLength << " bytes). Should be 12 bytes.\n";
             return false;
           }
           // read QNAM
           actual_in->read(reinterpret_cast<char*>(unknownQNAM.data()), 12);
           bytesRead += 12;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord QNAM of NPC_!\n";
             return false;
           }
           hasReadQNAM = true;
           break;
      case cNAM9:
           if (unknownNAM9.isPresent())
           {
             std::cerr << "Error: NPC_ seems to have more than one NAM9 subrecord.\n";
             return false;
           }
           if (!unknownNAM9.loadFromStream(*actual_in, cNAM9, false))
           {
             std::cerr << "Error while reading subrecord NAM9 of NPC_!\n";
             return false;
           }
           bytesRead += (2 + unknownNAM9.size());
           if (unknownNAM9.size() != 76)
           {
             std::cerr << "Error: Sub record NAM9 of NPC_ has invalid length ("
                       << unknownNAM9.size()<< " bytes). Should be 76 bytes.\n";
             return false;
           }
           break;
      case cNAMA:
           if (unknownNAMA.isPresent())
           {
             std::cerr << "Error: NPC_ seems to have more than one NAMA subrecord.\n";
             return false;
           }
           if (!unknownNAMA.loadFromStream(*actual_in, cNAMA, false))
           {
             std::cerr << "Error while reading subrecord NAMA of NPC_!\n";
             return false;
           }
           bytesRead += (2 + unknownNAMA.size());
           if (unknownNAMA.size() != 16)
           {
             std::cerr << "Error: Sub record NAMA of NPC_ has invalid length ("
                       << unknownNAMA.size() << " bytes). Should be 16 bytes.\n";
             return false;
           }
           break;
      case cTINI:
           // TINI's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record TINI of NPC_ has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read TINI
           tempTINX.unknownTINI = 0;
           actual_in->read(reinterpret_cast<char*>(&tempTINX.unknownTINI), 2);
           bytesRead += 2;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord TINI of NPC_!\n";
             return false;
           }

           // read TINC
           if (!loadUint32SubRecordFromStream(*actual_in, cTINC, tempTINX.unknownTINC, true))
             return false;
           bytesRead += 10;

           // read TINV
           if (!loadUint32SubRecordFromStream(*actual_in, cTINV, tempTINX.unknownTINV, true))
             return false;
           bytesRead += 10;

           // read TIAS header
           actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cTIAS)
           {
             UnexpectedRecord(cTIAS, subRecName);
             return false;
           }
           // read TIAS's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 2)
           {
             std::cerr << "Error: Sub record TIAS of NPC_ has invalid length ("
                       << subLength << " bytes). Should be two bytes.\n";
             return false;
           }
           // read TIAS
           actual_in->read(reinterpret_cast<char*>(&tempTINX.unknownTIAS), 2);
           bytesRead += 2;
           if (!actual_in->good())
           {
             std::cerr << "Error while reading subrecord TIAS of NPC_!\n";
             return false;
           }
           unknownTINXs.push_back(tempTINX);
           break;
      case cDSTD:
           /* We just skip all the DSTD/DSTF stuff here. */
           //DSTD's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 20)
           {
             std::cerr << "Error: Sub record DSTD of NPC_ has invalid length ("
                       << subLength << " bytes). Should be 20 bytes.\n";
             return false;
           }
           // skip DSTD
           actual_in->seekg(20, std::ios_base::cur);
           bytesRead += 20;

           // read DSTF
           actual_in->read(reinterpret_cast<char*>(&subRecName), 4);
           bytesRead += 4;
           if (subRecName != cDSTF)
           {
             UnexpectedRecord(cDSTF, subRecName);
             return false;
           }
           // DSTF's length
           actual_in->read(reinterpret_cast<char*>(&subLength), 2);
           bytesRead += 2;
           if (subLength != 0)
           {
             std::cerr << "Error: Sub record DSTF of NPC_ has invalid length ("
                       << subLength << " bytes). Should be zero bytes.\n";
             return false;
           }
           // nothing to skip, DSTF has length zero
           break;
      default:
           std::cerr << "Error: Found unexpected subrecord \"" << IntTo4Char(subRecName)
                     << "\", but only VMAD, OBND, ACBS, SNAM, INAM, VTCK, TPLT,"
                     << " RNAM, DEST, WNAM, ANAM, ATKR, ATKD, ATKE, SPCT, PRKZ,"
                     << " CNTO, ECOR, AIDT, KSIZ, CNAM, FULL, SHRT, DATA, PNAM,"
                     << " HCLF, GNAM, ZNAM, NAM5, NAM6, NAM7, NAM8, DPLT, DOFT,"
                     << " SOFT, CRIF, CSCR, CSDT, CSDI, FTST, QNAM, NAM9, NAMA or TINI are allowed here!\n";
           std::clog << "DEBUG: Stream position: " << actual_in->tellg() << "\n";
           return false;
    }
  }

  // check presence of all required subrecords
  if (!hasReadOBND || !hasReadACBS || (raceFormID == 0) || !hasReadAIDT
        || (classFormID == 0) || !hasReadDNAM || !hasReadNAM5 || !hasReadNAM6
        || !hasReadNAM7 || !hasReadNAM8 || !hasReadQNAM)
  {
    std::cerr << "Error: At least one required subrecord of NPC_ was not found!\n";
    return false;
  }

  return actual_in->good();
}

uint32_t NPCRecord::getRecordType() const
{
  return cNPC_;
}

} // namespace
