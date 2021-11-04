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
  keywordArray(std::vector<uint32_t>()),
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
      && (components == other.components) && (keywordArray == other.keywordArray)
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
  keywordArray.clear();
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
  combatOverridePackageListFormID = 0;
  spellFormIDs.clear();
  factionFormIDs.clear();
  packageDataFormIDs.clear();
  unknownVTCK.reset();
}

/*bool QuestRecord::AliasEntry::loadFromStream(std::istream& in_File, uint32_t& bytesRead, char * buffer)
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
             std::cerr << "Error: QUST seems to have more than one ALID subrecord per alias structure.\n";
             return false;
           }
           //ALID's length
           in_File.read((char*) &subLength, 2);
           bytesRead += 2;
           if (subLength>511)
           {
             std::cerr <<"Error: Sub record ALID of QUST is longer than 511 characters!\n";
             return false;
           }
           //read ALID's stuff
           memset(buffer, 0, 512);
           in_File.read(buffer, subLength);
           bytesRead += subLength;
           if (!in_File.good())
           {
             std::cerr << "Error while reading subrecord ALID of QUST!\n";
             return false;
           }
           aliasID = std::string(buffer);
           break;
      case cFNAM:
           if (hasReadFNAM)
           {
             std::cerr << "Error: QUST seems to have more than one FNAM subrecord per alias structure.\n";
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
             std::cerr << "Error: QUST seems to have more than one ALFE subrecord per alias structure.\n";
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
             std::cerr << "Error: QUST seems to have more than one ALFD subrecord per alias structure.\n";
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
             std::cerr << "Error: QUST seems to have more than one ALFR subrecord per alias structure.\n";
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
             std::cerr << "Error while reading subrecord CTDA of QUST!\n";
             return false;
           }
           unknownCTDAs.push_back(tempCTDA);
           break;
      case cVTCK:
           if (hasReadVTCK)
           {
             std::cerr << "Error: QUST seems to have more than one VTCK subrecord per alias structure.\n";
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
             std::cerr <<"Error: Sub record ALED of QUST has invalid length ("
                       <<subLength<<" bytes). Should be zero bytes!\n";
             return false;
           }
           toDo = false;
           break;
      default:
           std::cerr << "Error: Found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only ALID, FNAM, ALFE, ALFD, ALFR, CTDA, VTCK or ALED are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadFNAM and hasReadVTCK))
  {
    std::cerr << "Error: At least one required subrecord of alias structure in QUST is missing!\n";
    return false;
  }

  return in_File.good();
} */

} // namespace
