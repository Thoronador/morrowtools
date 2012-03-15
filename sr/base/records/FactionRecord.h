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

#ifndef SR_FACTIONRECORD_H
#define SR_FACTIONRECORD_H

#include "BasicRecord.h"
#include <string>
#include <vector>
#include "BinarySubRecord.h"
#include "CTDAData.h"

namespace SRTP
{

struct FactionRecord: public BasicRecord
{
  public:
    /* constructor */
    FactionRecord();

    /* destructor */
    virtual ~FactionRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const FactionRecord& other) const;

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
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const;

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
      uint32_t maleNameStringID;   //MNAM
      uint32_t femaleNameStringID; //FNAM

      /* equality operator */
      bool operator==(const RankData& other) const;
    };//struct

    std::string editorID;
    std::vector<InterfactionRelation> relations; //subrecords XNAM
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    uint32_t unknownDATA;
    uint32_t exteriorJailMarkerRefID; //subrecord JAIL
    uint32_t followerWaitMarkerRefID; //subrecord WAIT
    uint32_t stolenGoodsContainerRefID; //subrecord STOL
    uint32_t playerInventoryContainerRefID; //subrecord PLCN
    uint32_t crimeFactionListFormID; //subrecord CRGR, opt.
    uint32_t jailOutfitFormID; //subrecord JOUT, opt.
    BinarySubRecord unknownCRVA; //subrecord CRVA
    std::vector<RankData> ranks; //subrecord RNAM/MNAM/FNAM
    bool hasVEND;
    uint32_t unknownVEND;
    bool hasVENC;
    uint32_t unknownVENC;
    BinarySubRecord unknownVENV;
    BinarySubRecord unknownPLVD;
    std::vector<CTDA_CIS2_compound> conditions; //subrecords CITC and CTDA(s)+CIS2(s)
}; //struct

} //namespace

#endif // SR_FACTIONRECORD_H
