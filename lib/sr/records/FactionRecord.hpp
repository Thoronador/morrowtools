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

#ifndef SR_FACTIONRECORD_HPP
#define SR_FACTIONRECORD_HPP

#include "BasicRecord.hpp"
#include <string>
#include <vector>
#include "BinarySubRecord.hpp"
#include "CTDAData.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about a faction. */
struct FactionRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    FactionRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const FactionRecord& other) const;
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

    /* flag constants */
    static const uint32_t cFlagHiddenFromPC;
    static const uint32_t cFlagSpecialCombat;
    static const uint32_t cFlagTrackCrime;
    static const uint32_t cFlagIgnoresMurder;
    static const uint32_t cFlagIgnoresAssault;
    static const uint32_t cFlagIgnoresStealing;
    static const uint32_t cFlagIgnoresTrespass;
    static const uint32_t cFlagDoNotReportCrimesAgainstMembers;
    static const uint32_t cFlagIgnoresPickpocket;
    static const uint32_t cFlagVendor;
    static const uint32_t cFlagCanBeOwner;
    static const uint32_t cFlagIgnoresWerewolf;

    /* group combat reaction constants */
    static const uint32_t ReactionNeutral;
    static const uint32_t ReactionEnemy;
    static const uint32_t ReactionAlly;
    static const uint32_t ReactionFriend;

    struct InterfactionRelation
    {
      uint32_t factionFormID;
      uint32_t mod;
      uint32_t groupCombatReaction;

      bool operator==(const InterfactionRelation& other) const;
    };

    /// Holds information about a rank in a faction.
    struct RankData
    {
      uint32_t index;             //RNAM
      LocalizedString maleName;   //MNAM
      LocalizedString femaleName; //FNAM

      /** Constructor, creates an empty struct. */
      RankData();

      /* equality operator */
      bool operator==(const RankData& other) const;
    };

    struct VendorData
    {
      /* flag constants */
      static const uint32_t cFlagOnlyBuysStolenItems;
      static const uint32_t cFlagNotBuySell;

      uint16_t startHour;
      uint16_t endHour;
      uint32_t radius;
      uint32_t flagsVendor;

      bool isPresent;

      /** Constructor, creates an empty struct. */
      VendorData();

      /* equality operator */
      bool operator==(const VendorData& other) const;
    };

    std::string editorID;
    std::vector<InterfactionRelation> relations; // subrecords XNAM
    LocalizedString name; // subrecord FULL
    uint32_t flags; // subrecord DATA
    uint32_t exteriorJailMarkerRefID; // subrecord JAIL
    uint32_t followerWaitMarkerRefID; // subrecord WAIT
    uint32_t stolenGoodsContainerRefID; // subrecord STOL
    uint32_t playerInventoryContainerRefID; // subrecord PLCN
    uint32_t crimeFactionListFormID; // subrecord CRGR, opt.
    uint32_t jailOutfitFormID; // subrecord JOUT, opt.
    BinarySubRecord unknownCRVA; // subrecord CRVA
    std::vector<RankData> ranks; // subrecord RNAM/MNAM/FNAM
    uint32_t vendorListFormID; // subrecord VEND
    uint32_t vendorContainterFormID; // subrecord VENC
    VendorData vendorStuff; // subrecord VENV
    BinarySubRecord unknownPLVD;
    std::vector<CTDA_CIS2_compound> conditions; // subrecords CITC and CTDA(s)+CIS2(s)

    /* returns true, if the NPC is a vendor according to the set flags */
    inline bool isVendor() const
    {
      return (cFlagVendor & flags) != 0;
    }
}; // struct

} // namespace

#endif // SR_FACTIONRECORD_HPP
