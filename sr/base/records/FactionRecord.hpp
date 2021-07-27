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

struct FactionRecord: public BasicRecord
{
  public:
    /* constructor */
    FactionRecord();

    /* destructor */
    virtual ~FactionRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const FactionRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output stream
    */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File   - the input stream
          localized - whether the file to read from is localized or not
          table     - the associated string table for localized files
    */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /* returns the record's type, usually its header */
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

    //structure for interfaction relations
    struct InterfactionRelation
    {
      uint32_t factionFormID;
      uint32_t mod;
      uint32_t groupCombatReaction;

      /* equality operator */
      bool operator==(const InterfactionRelation& other) const;
    };//struct

    //structure for ranks
    struct RankData
    {
      uint32_t index;              //RNAM
      LocalizedString maleName;   //MNAM
      LocalizedString femaleName; //FNAM

      /* constructor */
      RankData();

      /* equality operator */
      bool operator==(const RankData& other) const;
    };//struct

    //structure for vendor stuff
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

      /* constructor */
      VendorData();

      /* equality operator */
      bool operator==(const VendorData& other) const;
    };//struct

    std::string editorID;
    std::vector<InterfactionRelation> relations; //subrecords XNAM
    LocalizedString name; //subrecord FULL
    uint32_t flags; //subrecord DATA
    uint32_t exteriorJailMarkerRefID; //subrecord JAIL
    uint32_t followerWaitMarkerRefID; //subrecord WAIT
    uint32_t stolenGoodsContainerRefID; //subrecord STOL
    uint32_t playerInventoryContainerRefID; //subrecord PLCN
    uint32_t crimeFactionListFormID; //subrecord CRGR, opt.
    uint32_t jailOutfitFormID; //subrecord JOUT, opt.
    BinarySubRecord unknownCRVA; //subrecord CRVA
    std::vector<RankData> ranks; //subrecord RNAM/MNAM/FNAM
    uint32_t vendorListFormID; //subrecord VEND
    uint32_t vendorContainterFormID; //subrecord VENC
    VendorData vendorStuff; //subrecord VENV
    BinarySubRecord unknownPLVD;
    std::vector<CTDA_CIS2_compound> conditions; //subrecords CITC and CTDA(s)+CIS2(s)

    /* returns true, if the NPC is a vendor according to the set flags */
    inline bool isVendor() const
    {
      return ((cFlagVendor&flags)!=0);
    }
}; //struct

} //namespace

#endif // SR_FACTIONRECORD_HPP
