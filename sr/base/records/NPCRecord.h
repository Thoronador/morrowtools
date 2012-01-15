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

#ifndef SR_NPCRECORD_H
#define SR_NPCRECORD_H

#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include "ComponentData.h"
#include <string>
#include <vector>
#include <stdint.h>

namespace SRTP
{

struct NPCRecord: public BasicRecord
{
  public:
    /* constructor */
    NPCRecord();

    /* destructor */
    virtual ~NPCRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const NPCRecord& other) const;

    /* writes the record to the given output stream and returns true on success

      parameters:
          output   - the output file stream
    */
    virtual bool saveToStream(std::ofstream& output) const;

    /* loads the record from the given input stream and returns true on success

      parameters:
          in_File - the input file stream
    */
    virtual bool loadFromStream(std::ifstream& in_File);

    /* returns the record's type, usually its header */
    virtual int32_t getRecordType() const;

    /* returns the size in bytes that the record's data would occupy in a file
       stream, NOT including the header data
    */
    virtual uint32_t getWriteSize() const;

    //struct for perks
    struct PerkElem
    {
      uint32_t formID;
      uint32_t valueTwo; //unknown

      /* equality operator */
      bool operator==(const PerkElem& other) const;
    };//struct

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    uint8_t unknownACBS[24];
    std::vector<uint64_t> unknownSNAMs;
    bool hasINAM;
    uint32_t unknownINAM;
    bool hasVTCK;
    uint32_t unknownVTCK;
    uint32_t unknownTPLT;
    uint32_t unknownRNAM;
    bool hasWNAM;
    uint32_t unknownWNAM;
    bool hasATKR;
    uint32_t unknownATKR;
    std::vector<uint32_t> spellFormIDs;
    std::vector<PerkElem> perkList;
    std::vector<ComponentData> items;
    uint8_t unknownAIDT[20];
    std::vector<uint32_t> unknownPKIDs;
    std::vector<uint32_t> keywordArray;
    uint32_t unknownCNAM;
    uint8_t unknownDNAM[52];
    bool hasZNAM;
    uint32_t unknownZNAM;
    uint16_t unknownNAM5;
    uint32_t unknownNAM6;
    uint32_t unknownNAM7;
    uint32_t unknownNAM8;
    bool hasCSCR;
    uint32_t unknownCSCR;
    bool hasDPLT;
    uint32_t unknownDPLT;
    uint8_t unknownQNAM[12];
}; //struct

} //namespace

#endif // SR_NPCRECORD_H
