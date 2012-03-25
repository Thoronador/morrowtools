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

#ifndef SR_CHARACTERREFERENCERECORD_H
#define SR_CHARACTERREFERENCERECORD_H

#include "BasicRecord.h"
#include <string>
#include <vector>
#include "BinarySubRecord.h"
#include "SubBlock.h"

namespace SRTP
{

struct CharacterReferenceRecord: public BasicRecord
{
  public:
    /* constructor */
    CharacterReferenceRecord();

    /* destructor */
    virtual ~CharacterReferenceRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const CharacterReferenceRecord& other) const;
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
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual uint32_t getRecordType() const;

    /* structure for linked references */
    struct LinkedRef
    {
      uint32_t keywordFormID;
      uint32_t referenceFormID;

      /* equality operator */
      inline bool operator==(const LinkedRef& other) const
      {
        return ((keywordFormID==other.keywordFormID) and (referenceFormID==other.referenceFormID));
      }
    };//struct

    /* structure for enable parent */
    struct EnableParent
    {
      uint32_t refID;
      uint32_t flags;

      static const uint32_t FlagSetEnableStateToOppositeOfParent;
      static const uint32_t FlagPopIn;

      /* returns true, if the opposite of parent's enable state is set */
      bool oppositeOfParent() const;

      /* returns true, if the pop in flag is set */
      bool popsIn() const;

      /* returns true, of the XESP subrecord is present */
      bool isPresent() const;

      /* equality operator */
      bool operator==(const EnableParent& other) const;

      /* clears the data */
      void clear();
    };//struct

    /* structure for XAPD / XAPR pairs */
    /*struct XAPD_XAPR_pair
    {
      uint8_t unknownXAPD;
      uint64_t unknownXAPR;

      // equality operator
      bool operator==(const XAPD_XAPR_pair& other) const;
    }; //struct
    */

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint32_t baseObjectFormID; //subrecord NAME
    BinarySubRecord unknownXRGD;
    BinarySubRecord unknownXRGB;
    bool hasXLCM;
    uint32_t unknownXLCM;
    bool hasINAM;
    uint32_t unknownINAM;
    bool hasPDTO;
    uint64_t unknownPDTO;
    std::vector<LinkedRef> linkedReferences; //subrecords XLKR
    std::vector<uint8_t> XAPDs; //subrecords XAPD
    std::vector<uint64_t> XAPRs; //subrecords XAPR
    uint32_t persistenceLocationFormID; //subrecord XLCN
    uint32_t locationRefTypeFormID; //subrecord XLRT
    uint32_t encounterZoneFormID; //subrecord XEZN
    EnableParent subXESP; //subrecord XESP
    uint32_t ownerFormID; //subrecord XOWN
    float scale;
    std::vector<SubBlock> subBlocks;
    uint8_t unknownDATA[24];
}; //struct

} //namespace

#endif // SR_CHARACTERREFERENCERECORD_H
