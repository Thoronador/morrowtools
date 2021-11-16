/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014, 2021  Thoronador

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

#include "AliasEntry.hpp"
#ifndef SR_NO_RECORD_EQUALITY
#include <iostream>
#include "../../../../sr/base/SR_Constants.hpp"
#endif // SR_NO_RECORD_EQUALITY

namespace SRTP
{

AliasEntry::AliasEntry()
: unknownALST(std::nullopt),
  unknownALLS(std::nullopt),
  aliasID(""), //subrecord ALID
  unknownFNAM(0),
  unknownALFA(std::nullopt),
  locationRefTypeFormID(0), //subrecord ALRT
  createReferenceToObjectFormID(0), //subrecord ALCO
  unknownALCA(std::nullopt),
  unknownALCL(std::nullopt),
  displayNameFormID(0), // subrecord ALDN
  components(std::vector<ComponentData>()),
  keywords(std::vector<uint32_t>()),
  unknownALFE(std::nullopt),
  unknownALFD(std::nullopt),
  forcedIntoAliasID(std::nullopt), // subrecord ALFI
  specificLocationFormID(0),
  specificReferenceID(0),
  unknownALNA(std::nullopt),
  unknownALNT(std::nullopt),
  uniqueActorFormID(0), // subrecord ALUA
  externalAliasReferenceFormID(0), // subrecord ALEQ
  unknownALEA(std::nullopt),
  keywordFormID(0), // subrecord KNAM
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>()),
  spectatorOverridePackageListFormID(0), // subrecord SPOR
  observeDeadBodyOverridePackageListFormID(0), // subrecord OCOR
  guardWarnOverridePackageListFormID(0), // subrecord GWOR
  combatOverridePackageListFormID(0), // subrecord ECOR
  spellFormIDs(std::vector<uint32_t>()), // subrecords ALSP
  factionFormIDs(std::vector<uint32_t>()), // subrecords ALFC
  packageDataFormIDs(std::vector<uint32_t>()), // subrecords ALPC
  unknownVTCK(std::nullopt)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool AliasEntry::operator==(const AliasEntry& other) const
{
  return (unknownALLS == other.unknownALLS)
      && (unknownALST == other.unknownALST)
      && (aliasID == other.aliasID) && (unknownFNAM == other.unknownFNAM)
      && (unknownALFA == other.unknownALFA)
      && (locationRefTypeFormID == other.locationRefTypeFormID)
      && (createReferenceToObjectFormID == other.createReferenceToObjectFormID)
      && (unknownALCA == other.unknownALCA)
      && (unknownALCL == other.unknownALCL)
      && (displayNameFormID == other.displayNameFormID)
      && (components == other.components) && (keywords == other.keywords)
      && (unknownALFE == other.unknownALFE)
      && (unknownALFD == other.unknownALFD)
      && (forcedIntoAliasID == other.forcedIntoAliasID)
      && (specificLocationFormID == other.specificLocationFormID)
      && (specificReferenceID == other.specificReferenceID)
      && (unknownALNA == other.unknownALNA)
      && (unknownALNT == other.unknownALNT)
      && (uniqueActorFormID == other.uniqueActorFormID)
      && (externalAliasReferenceFormID == other.externalAliasReferenceFormID)
      && (unknownALEA == other.unknownALEA)
      && (spellFormIDs == other.spellFormIDs)
      && (keywordFormID == other.keywordFormID)
      && (unknownCTDA_CIS2s == other.unknownCTDA_CIS2s)
      && (spectatorOverridePackageListFormID == other.spectatorOverridePackageListFormID)
      && (observeDeadBodyOverridePackageListFormID == other.observeDeadBodyOverridePackageListFormID)
      && (guardWarnOverridePackageListFormID == other.guardWarnOverridePackageListFormID)
      && (combatOverridePackageListFormID == other.combatOverridePackageListFormID)
      && (factionFormIDs == other.factionFormIDs)
      && (packageDataFormIDs == other.packageDataFormIDs)
      && (unknownVTCK == other.unknownVTCK);
}
#endif // SR_NO_RECORD_EQUALITY

void AliasEntry::clear()
{
  unknownALST.reset();
  unknownALLS.reset();
  aliasID.clear(); // subrecord ALID
  unknownFNAM = 0;
  unknownALFA.reset();
  locationRefTypeFormID = 0;
  createReferenceToObjectFormID = 0;
  unknownALCA.reset();
  unknownALCL.reset();
  displayNameFormID = 0;
  components.clear();
  keywords.clear();
  unknownALFE.reset();
  unknownALFD.reset();
  forcedIntoAliasID.reset();
  specificLocationFormID = 0;
  specificReferenceID = 0;
  unknownALNA.reset();
  unknownALNT.reset();
  uniqueActorFormID = 0;
  externalAliasReferenceFormID = 0;
  unknownALEA.reset();
  keywordFormID = 0;
  unknownCTDA_CIS2s.clear();
  spectatorOverridePackageListFormID = 0;
  observeDeadBodyOverridePackageListFormID = 0;
  guardWarnOverridePackageListFormID = 0;
  combatOverridePackageListFormID = 0;
  spellFormIDs.clear();
  factionFormIDs.clear();
  packageDataFormIDs.clear();
  unknownVTCK.reset();
}

#ifndef SR_UNSAVEABLE_RECORDS
uint32_t AliasEntry::getWriteSize() const
{
  uint32_t writeSize = 4 /* FNAM */ + 2 /* length bytes */ + 4 /* data size */
      + spellFormIDs.size() * (4 /* ALSP */ + 2 /* length bytes */ + 4)
      + factionFormIDs.size() * (4 /* ALFC */ + 2 /* length bytes */ + 4)
      + packageDataFormIDs.size() * (4 /* ALPC */ + 2 /* length bytes */ + 4)
      + 4 /* ALED */ + 2 /* length bytes */ + 0 /* no data */;
  if (unknownALST.has_value())
  {
    writeSize += 4 /* ALST */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownALLS.has_value())
  {
    writeSize += 4 /* ALLS */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (!aliasID.empty())
  {
    writeSize += 4 /* ALID */ + 2 /* length bytes */ + aliasID.size() + 1;
  }
  if (unknownALFA.has_value())
  {
    writeSize += 4 /* ALFA */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (locationRefTypeFormID != 0)
  {
    writeSize += 4 /* ALRT */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (createReferenceToObjectFormID != 0)
  {
    writeSize += 4 /* ALCO */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownALCA.has_value())
  {
    writeSize += 4 /* ALCA */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownALCL.has_value())
  {
    writeSize += 4 /* ALCL */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (displayNameFormID != 0)
  {
    writeSize += 4 /* ALCO */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (!components.empty())
  {
    writeSize += 4 /* COCT */ + 2 /* length bytes */ + 4 /* data size */
        + components.size() * (4 /* CNTO */ + 2 /* length bytes */ + 8 /* data size */);
  }
  if (!keywords.empty())
  {
    writeSize += 4 /* KSIZ */ + 2 /* length bytes */ + 4 /* data size */
        + 4 /* KWDA */ + 2 /* length bytes */ + 4 * keywords.size();
  }
  if (unknownALFE.has_value())
  {
    writeSize += 4 /* ALFE */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownALFD.has_value())
  {
    writeSize += 4 /* ALFD */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (forcedIntoAliasID.has_value())
  {
    writeSize += 4 /* ALFI */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (specificLocationFormID != 0)
  {
    writeSize += 4 /* ALFL */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (specificReferenceID != 0)
  {
    writeSize += 4 /* ALFR */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownALNA.has_value())
  {
    writeSize += 4 /* ALNA */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownALNT.has_value())
  {
    writeSize += 4 /* ALNT */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (uniqueActorFormID != 0)
  {
    writeSize += 4 /* ALUA */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (externalAliasReferenceFormID != 0)
  {
    writeSize += 4 /* ALEQ */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownALEA.has_value())
  {
    writeSize += 4 /* ALEA */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (keywordFormID != 0)
  {
    writeSize += 4 /* KNAM */ + 2 /* length bytes */ + 4 /* data size */;
  }
  for (const auto& cc: unknownCTDA_CIS2s)
  {
    writeSize += cc.getWriteSize();
  }
  if (spectatorOverridePackageListFormID != 0)
  {
    writeSize += 4 /* SPOR */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (observeDeadBodyOverridePackageListFormID != 0)
  {
    writeSize += 4 /* OCOR */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (guardWarnOverridePackageListFormID != 0)
  {
    writeSize += 4 /* GWOR */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (combatOverridePackageListFormID != 0)
  {
    writeSize += 4 /* ECOR */ + 2 /* length bytes */ + 4 /* data size */;
  }
  if (unknownVTCK.has_value())
  {
    writeSize += 4 /* VTCK */ + 2 /* length bytes */ + 4 /* data size */;
  }
  return writeSize;
}

bool AliasEntry::saveToStream(std::ostream& output) const
{
  // Exactly one of ALST or ALLS must be set.
  if (unknownALST.has_value() == unknownALLS.has_value())
  {
    std::cerr << "Error: Exactly one of either ALST or ALLS must be set in AliasEntry!\n";
    return false;
  }

  uint16_t subLength = 0;
  if (unknownALST.has_value())
  {
    // write ALST
    output.write(reinterpret_cast<const char*>(&cALST), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALST.value()), 4);
  }
  if (unknownALLS.has_value())
  {
    // write ALLS
    output.write(reinterpret_cast<const char*>(&cALLS), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALLS.value()), 4);
  }
  if (!aliasID.empty())
  {
    // write ALID
    output.write(reinterpret_cast<const char*>(&cALID), 4);
    subLength = aliasID.size() + 1;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(aliasID.c_str(), subLength);
  }

  // write FNAM
  output.write(reinterpret_cast<const char*>(&cFNAM), 4);
  subLength = 4;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  output.write(reinterpret_cast<const char*>(&unknownFNAM), 4);

  if (unknownALFA.has_value())
  {
    // write ALFA
    output.write(reinterpret_cast<const char*>(&cALFA), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALFA.value()), 4);
  }
  if (locationRefTypeFormID != 0)
  {
    // write ALRT
    output.write(reinterpret_cast<const char*>(&cALRT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&locationRefTypeFormID), 4);
  }
  if (createReferenceToObjectFormID != 0)
  {
    // write ALCO
    output.write(reinterpret_cast<const char*>(&cALCO), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&createReferenceToObjectFormID), 4);
  }
  if (unknownALCA.has_value())
  {
    // write ALCA
    output.write(reinterpret_cast<const char*>(&cALCA), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALCA.value()), 4);
  }
  if (unknownALCL.has_value())
  {
    // write ALCL
    output.write(reinterpret_cast<const char*>(&cALCL), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALCL.value()), 4);
  }
  if (displayNameFormID != 0)
  {
    // write ALDN
    output.write(reinterpret_cast<const char*>(&cALDN), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&displayNameFormID), 4);
  }
  if (!components.empty())
  {
    // write COCT
    output.write(reinterpret_cast<const char*>(&cCOCT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t componentCount = components.size();
    output.write(reinterpret_cast<const char*>(&componentCount), 4);
    for (const auto& data: components)
    {
      // write CNTO
      output.write(reinterpret_cast<const char*>(&cCNTO), 4);
      subLength = 8;
      output.write(reinterpret_cast<const char*>(&subLength), 2);
      output.write(reinterpret_cast<const char*>(&data.formID), 4);
      output.write(reinterpret_cast<const char*>(&data.count), 4);
    }
  }
  if (!keywords.empty())
  {
    // write KSIZ
    output.write(reinterpret_cast<const char*>(&cKSIZ), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    const uint32_t keywordCount = keywords.size();
    output.write(reinterpret_cast<const char*>(&keywordCount), 4);
    // write KWDA
    output.write(reinterpret_cast<const char*>(&cKWDA), 4);
    subLength = keywordCount * 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    for (const uint32_t kw: keywords)
    {
      output.write(reinterpret_cast<const char*>(&kw), 4);
    }
  }
  if (unknownALFE.has_value())
  {
    // write ALFE
    output.write(reinterpret_cast<const char*>(&cALFE), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALFE.value()), 4);
  }
  if (unknownALFD.has_value())
  {
    // write ALFD
    output.write(reinterpret_cast<const char*>(&cALFD), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALFD.value()), 4);
  }
  if (forcedIntoAliasID.has_value())
  {
    // write ALFI
    output.write(reinterpret_cast<const char*>(&cALFI), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&forcedIntoAliasID.value()), 4);
  }
  if (specificLocationFormID != 0)
  {
    // write ALFL
    output.write(reinterpret_cast<const char*>(&cALFL), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&specificLocationFormID), 4);
  }
  if (specificReferenceID != 0)
  {
    // write ALFR
    output.write(reinterpret_cast<const char*>(&cALFR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&specificReferenceID), 4);
  }
  if (unknownALNA.has_value())
  {
    // write ALNA
    output.write(reinterpret_cast<const char*>(&cALNA), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALNA.value()), 4);
  }
  if (unknownALNT.has_value())
  {
    // write ALNT
    output.write(reinterpret_cast<const char*>(&cALNT), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALNT.value()), 4);
  }
  if (uniqueActorFormID != 0)
  {
    // write ALUA
    output.write(reinterpret_cast<const char*>(&cALUA), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&uniqueActorFormID), 4);
  }
  if (externalAliasReferenceFormID != 0)
  {
    // write ALEQ
    output.write(reinterpret_cast<const char*>(&cALEQ), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&externalAliasReferenceFormID), 4);
  }
  if (unknownALEA.has_value())
  {
    // write ALEA
    output.write(reinterpret_cast<const char*>(&cALEA), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownALEA.value()), 4);
  }
  if (keywordFormID != 0)
  {
    // write KNAM
    output.write(reinterpret_cast<const char*>(&cKNAM), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&keywordFormID), 4);
  }
  for (const auto& cc: unknownCTDA_CIS2s)
  {
    if (!cc.saveToStream(output))
      return false;
  }
  if (spectatorOverridePackageListFormID != 0)
  {
    // write SPOR
    output.write(reinterpret_cast<const char*>(&cSPOR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&spectatorOverridePackageListFormID), 4);
  }
  if (observeDeadBodyOverridePackageListFormID != 0)
  {
    // write OCOR
    output.write(reinterpret_cast<const char*>(&cOCOR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&observeDeadBodyOverridePackageListFormID), 4);
  }
  if (guardWarnOverridePackageListFormID != 0)
  {
    // write GWOR
    output.write(reinterpret_cast<const char*>(&cGWOR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&guardWarnOverridePackageListFormID), 4);
  }
  if (combatOverridePackageListFormID != 0)
  {
    // write ECOR
    output.write(reinterpret_cast<const char*>(&cECOR), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&combatOverridePackageListFormID), 4);
  }
  for (const uint32_t spellID: spellFormIDs)
  {
    // write ALSP
    output.write(reinterpret_cast<const char*>(&cALSP), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&spellID), 4);
  }
  for (const uint32_t factionID: factionFormIDs)
  {
    // write ALFC
    output.write(reinterpret_cast<const char*>(&cALFC), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&factionID), 4);
  }
  for (const uint32_t packageID: packageDataFormIDs)
  {
    // write ALPC
    output.write(reinterpret_cast<const char*>(&cALPC), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&packageID), 4);
  }
  if (unknownVTCK.has_value())
  {
    // write VTCK
    output.write(reinterpret_cast<const char*>(&cVTCK), 4);
    subLength = 4;
    output.write(reinterpret_cast<const char*>(&subLength), 2);
    output.write(reinterpret_cast<const char*>(&unknownVTCK.value()), 4);
  }

  // write ALED
  output.write(reinterpret_cast<const char*>(&cALED), 4);
  subLength = 0;
  output.write(reinterpret_cast<const char*>(&subLength), 2);
  return output.good();
}
#endif

/*bool QuestRecord::AliasEntry::loadFromStream(std::istream& in_File, uint32_t& bytesRead, char * buffer)
{
  ...

  return in_File.good();
} */

} // namespace
