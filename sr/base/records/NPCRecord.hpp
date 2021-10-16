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

#ifndef SR_NPCRECORD_HPP
#define SR_NPCRECORD_HPP

#include <array>
#include <optional>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "ComponentData.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about an NPC or a creature. */
struct NPCRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    NPCRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const NPCRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the record's data would occupy in a file
     *         stream, NOT including the header data.
     *
     * \return Returns the size in bytes that the record would need. Size of the
     *         header is not included.
     */
    virtual uint32_t getWriteSize() const;

    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param in_File    the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
    virtual uint32_t getRecordType() const;

    /// Type for information about faction membership.
    struct FactionElem
    {
      uint32_t formID; /**< the form ID of the faction */
      uint32_t rank;   /**< rank within the faction */

      FactionElem();

      bool operator==(const FactionElem& other) const;
    };

    /// Type for known perks.
    struct PerkElem
    {
      uint32_t formID;   /**< the form ID of the perk */
      uint32_t valueTwo; //unknown

      PerkElem();

      bool operator==(const PerkElem& other) const;
    };

    /// Holds data of TINI-TINC-TINV-TIAS subrecord sequences.
    struct TINXstructure
    {
      uint16_t unknownTINI;
      uint32_t unknownTINC;
      uint32_t unknownTINV;
      uint16_t unknownTIAS;

      TINXstructure();

      bool operator==(const TINXstructure& other) const;
    };

    /// Holds data of CSDT and the following two subrecords.
    struct CSDXstruct
    {
      uint32_t unknownCSDT;
      uint32_t unknownCSDI;
      uint8_t unknownCSDC;

      CSDXstruct();

      bool operator==(const CSDXstruct& other) const;
    };

    std::string editorID; /**< ID of the record in the editor */
    BinarySubRecord unknownVMAD;
    std::array<uint8_t, 12> unknownOBND;
    std::array<uint8_t, 24> unknownACBS;
    std::vector<FactionElem> factions; // subrecords SNAM
    uint32_t deathItemFormID; // subrecord INAM
    uint32_t voiceTypeFormID; // subrecord VTCK
    uint32_t templateActorBaseFormID; // subrecord TPLT
    uint32_t raceFormID; // subrecord RNAM
    std::optional<uint64_t> unknownDEST;
    std::vector<uint32_t> spellFormIDs;
    uint32_t skinFormID; // subrecord WNAM
    uint32_t farAwayModelSkinFormID; // subrecord ANAM
    std::optional<uint32_t> unknownATKR;
    BinarySubRecord unknownATKD;
    std::string unknownATKE;
    std::vector<PerkElem> perkList;
    std::vector<ComponentData> items;
    uint32_t spectatorOverridePackageListFormID; // subrecord SPOR
    uint32_t combatOverridePackageListFormID; // subrecord ECOR
    std::array<uint8_t, 20> unknownAIDT;
    std::vector<uint32_t> unknownPKIDs;
    std::vector<uint32_t> keywords;
    uint32_t classFormID; // subrecord CNAM
    LocalizedString name; // subrecord FULL
    std::optional<uint32_t> unknownSHRT;
    std::array<uint8_t, 52> unknownDNAM;
    std::vector<uint32_t> unknownPNAMs;
    uint32_t hairColorFormID; // subrecord HCLF
    uint32_t giftFilterFormID; // subrecord GNAM
    uint32_t combatStyleFormID; // subrecord ZNAM
    uint16_t unknownNAM5;
    uint32_t unknownNAM6;
    uint32_t unknownNAM7;
    uint32_t unknownNAM8;
    uint32_t soundTemplateFormID; // subrecord CSCR, ID of the NPC_ from which the sounds are inherited
    uint32_t defaultOutfitFormID; // subrecord DOFT
    uint32_t sleepOutfitFormID; // subrecord SOFT
    uint32_t crimeFactionFormID; // subrecord CRIF
    std::vector<CSDXstruct> unknownCSDXs;
    uint32_t defaultPackageListFormID; // subrecord DPLT
    uint32_t faceComplexionFormID; // subrecord FTST
    std::array<uint8_t, 12> unknownQNAM;
    BinarySubRecord unknownNAM9;
    BinarySubRecord unknownNAMA;
    std::vector<TINXstructure> unknownTINXs;
}; // struct

} // namespace

#endif // SR_NPCRECORD_HPP
