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
: hasALST(false),
  unknownALST(0),
  hasALLS(false),
  unknownALLS(0),
  aliasID(""), //subrecord ALID
  unknownFNAM(0),
  hasALFA(false),
  unknownALFA(0),
  locationRefTypeFormID(0), //subrecord ALRT
  createReferenceToObjectFormID(0), //subrecord ALCO
  hasALCA(false),
  unknownALCA(0),
  hasALCL(false),
  unknownALCL(0),
  displayNameFormID(0), //subrecord ALDN
  components(std::vector<ComponentData>()),
  keywordArray(std::vector<uint32_t>()),
  hasALFE(false),
  unknownALFE(0),
  hasALFD(false),
  unknownALFD(0),
  hasALFI(false),
  forcedIntoAliasID(0), //subrecord ALFI
  specificLocationFormID(0), //subrecord ALFL
  specificReferenceID(0), //subrecord ALFR
  hasALNA(false),
  unknownALNA(0),
  hasALNT(false),
  unknownALNT(0),
  uniqueActorFormID(0), //subrecord ALUA
  externalAliasReferenceFormID(0), //subrecord ALEQ
  hasALEA(false),
  unknownALEA(0),
  keywordFormID(0), //subrecord KNAM
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>()),
  spectatorOverridePackageListFormID(0), //subrecord SPOR
  combatOverridePackageListFormID(0), //subrecord ECOR
  spellFormIDs(std::vector<uint32_t>()), //subrecords ALSP
  factionFormIDs(std::vector<uint32_t>()), //subrecords ALFC
  packageDataFormIDs(std::vector<uint32_t>()), //subrecords ALPC
  hasVTCK(false),
  unknownVTCK(0)
{
}

#ifndef SR_NO_RECORD_EQUALITY
bool AliasEntry::operator==(const AliasEntry& other) const
{
  return ((hasALLS==other.hasALLS) and ((unknownALLS==other.unknownALLS) or (!hasALLS))
      and (hasALST==other.hasALST) and ((unknownALST==other.unknownALST) or (!hasALST))
      and (aliasID==other.aliasID) and (unknownFNAM==other.unknownFNAM)
      and (hasALFA==other.hasALFA) and ((unknownALFA==other.unknownALFA) or (!hasALFA))
      and (locationRefTypeFormID==other.locationRefTypeFormID)
      and (createReferenceToObjectFormID==other.createReferenceToObjectFormID)
      and (hasALCA==other.hasALCA) and ((unknownALCA==other.unknownALCA) or (!hasALCA))
      and (hasALCL==other.hasALCL) and ((unknownALCL==other.unknownALCL) or (!hasALCL))
      and (displayNameFormID==other.displayNameFormID)
      and (components==other.components) and (keywordArray==other.keywordArray)
      and (hasALFE==other.hasALFE) and ((unknownALFE==other.unknownALFE) or (!hasALFE))
      and (hasALFD==other.hasALFD) and ((unknownALFD==other.unknownALFD) or (!hasALFD))
      and (hasALFI==other.hasALFI) and ((forcedIntoAliasID==other.forcedIntoAliasID) or (!hasALFI))
      and (specificLocationFormID==other.specificLocationFormID)
      and (specificReferenceID==other.specificReferenceID)
      and (hasALNA==other.hasALNA) and ((unknownALNA==other.unknownALNA) or (!hasALNA))
      and (hasALNT==other.hasALNT) and ((unknownALNT==other.unknownALNT) or (!hasALNT))
      and (uniqueActorFormID==other.uniqueActorFormID)
      and (externalAliasReferenceFormID==other.externalAliasReferenceFormID)
      and (hasALEA==other.hasALEA) and ((unknownALEA==other.unknownALEA) or (!hasALEA))
      and (spellFormIDs==other.spellFormIDs)
      and (keywordFormID==other.keywordFormID)
      and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s)
      and (spectatorOverridePackageListFormID==other.spectatorOverridePackageListFormID)
      and (combatOverridePackageListFormID==other.combatOverridePackageListFormID)
      and (factionFormIDs==other.factionFormIDs)
      and (packageDataFormIDs==other.packageDataFormIDs)
      and (hasVTCK==other.hasVTCK) and ((unknownVTCK==other.unknownVTCK) or (!hasVTCK)));
}
#endif // SR_NO_RECORD_EQUALITY

void AliasEntry::clear()
{
  hasALST = false;
  unknownALST = 0;
  hasALLS = false;
  unknownALLS = 0;
  aliasID.clear(); //subrecord ALID
  unknownFNAM = 0;
  hasALFA = false;
  unknownALFA = 0;
  locationRefTypeFormID = 0;
  createReferenceToObjectFormID = 0;
  hasALCA = false;
  unknownALCA = 0;
  hasALCL = false;
  unknownALCL = 0;
  displayNameFormID = 0;
  components.clear();
  keywordArray.clear();
  hasALFE = false;
  unknownALFE = 0;
  hasALFD = false;
  unknownALFD = 0;
  hasALFI = false;
  forcedIntoAliasID = 0;
  specificLocationFormID = 0;
  specificReferenceID = 0;
  hasALNA = false;
  unknownALNA = 0;
  hasALNT = false;
  unknownALNT = 0;
  uniqueActorFormID = 0;
  externalAliasReferenceFormID = 0;
  hasALEA = false;
  unknownALEA = 0;
  keywordFormID = 0;
  unknownCTDA_CIS2s.clear();
  spectatorOverridePackageListFormID = 0;
  combatOverridePackageListFormID = 0;
  spellFormIDs.clear();
  factionFormIDs.clear();
  packageDataFormIDs.clear();
  hasVTCK = false;
  unknownVTCK = 0;
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
             std::cerr <<"Error: sub record ALID of QUST is longer than 511 characters!\n";
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
             std::cerr <<"Error: sub record ALED of QUST has invalid length ("
                       <<subLength<<" bytes). Should be zero bytes!\n";
             return false;
           }
           toDo = false;
           break;
      default:
           std::cerr << "Error: found unexpected subrecord \""<<IntTo4Char(subRecName)
                     << "\", but only ALID, FNAM, ALFE, ALFD, ALFR, CTDA, VTCK or ALED are allowed here!\n";
           return false;
    }//swi
  }//while

  //presence checks
  if (!(hasReadFNAM and hasReadVTCK))
  {
    std::cerr << "Error: at least one required subrecord of alias structure in QUST is missing!\n";
    return false;
  }

  return in_File.good();
} */

} // namespace
