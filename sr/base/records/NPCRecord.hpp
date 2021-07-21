/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2013  Thoronador

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

#ifndef SR_NPCRECORD_HPP
#define SR_NPCRECORD_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "ComponentData.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct NPCRecord: public BasicRecord
{
  public:
    /* constructor */
    NPCRecord();

    /* destructor */
    virtual ~NPCRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const NPCRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File   - the input file stream
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::ifstream& in_File, const bool localized, const StringTable& table);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    //struct for factions
    struct FactionElem
    {
      uint32_t formID;
      uint32_t rank;

      /* equality operator */
      bool operator==(const FactionElem& other) const;
    }; //struct

    //struct for perks
    struct PerkElem
    {
      uint32_t formID;
      uint32_t valueTwo; //unknown

      /* equality operator */
      bool operator==(const PerkElem& other) const;
    };//struct

    //struct for TINI-TINC-TINV-TIAS sequences
    struct TINXstructure
    {
      uint16_t unknownTINI;
      uint32_t unknownTINC;
      uint32_t unknownTINV;
      uint16_t unknownTIAS;

      /* equality operator */
      bool operator==(const TINXstructure& other) const;
    }; //struct

    //struct for CSDT and following two subrecords
    struct CSDXstruct
    {
      uint32_t unknownCSDT;
      uint32_t unknownCSDI;
      uint8_t unknownCSDC;

      /* equality operator */
      bool operator==(const CSDXstruct& other) const;
    }; //struct

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    uint8_t unknownACBS[24];
    std::vector<FactionElem> factions; //subrecords SNAM
    uint32_t deathItemFormID; //subrecord INAM
    uint32_t voiceTypeFormID; //subrecord VTCK
    uint32_t templateActorBaseFormID; //subrecord TPLT
    uint32_t raceFormID; //subrecord RNAM
    bool hasDEST;
    uint64_t unknownDEST;
    uint32_t skinFormID; //subrecord WNAM
    uint32_t farAwayModelSkinFormID; //subrecord ANAM
    bool hasATKR;
    uint32_t unknownATKR;
    BinarySubRecord unknownATKD;
    std::string unknownATKE;
    std::vector<uint32_t> spellFormIDs;
    std::vector<PerkElem> perkList;
    std::vector<ComponentData> items;
    uint32_t spectatorOverridePackageListFormID; //subrecord SPOR
    uint32_t combatOverridePackageListFormID; //subrecord ECOR
    uint8_t unknownAIDT[20];
    std::vector<uint32_t> unknownPKIDs;
    std::vector<uint32_t> keywordArray;
    uint32_t classFormID; //subrecord CNAM
    LocalizedString name; //subrecord FULL
    bool hasSHRT;
    uint32_t unknownSHRT;
    uint8_t unknownDNAM[52];
    std::vector<uint32_t> unknownPNAMs;
    uint32_t hairColorFormID; //subrecord HCLF
    uint32_t giftFilterFormID; //subrecord GNAM
    uint32_t combatStyleFormID; //subrecord ZNAM
    uint16_t unknownNAM5;
    uint32_t unknownNAM6;
    uint32_t unknownNAM7;
    uint32_t unknownNAM8;
    uint32_t defaultOutfitFormID; //subrecord DOFT
    uint32_t sleepOutfitFormID; //subrecord SOFT
    uint32_t crimeFactionFormID; //subrecord CRIF
    uint32_t soundTemplateFormID; //subrecord CSCR, ID of the NPC_ from which the sounds are inherited
    std::vector<CSDXstruct> unknownCSDXs;
    uint32_t defaultPackageListFormID; //subrecord DPLT
    uint32_t faceComplexionFormID; //subrecord FST
    uint8_t unknownQNAM[12];
    BinarySubRecord unknownNAM9;
    BinarySubRecord unknownNAMA;
    std::vector<TINXstructure> unknownTINXs;
}; //struct

} //namespace

#endif // SR_NPCRECORD_HPP
