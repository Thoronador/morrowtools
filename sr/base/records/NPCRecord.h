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

    //struct fot TINI-TINC-TINV-TIAS sequences
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
    std::vector<uint64_t> unknownSNAMs;
    bool hasINAM;
    uint32_t unknownINAM;
    bool hasVTCK;
    uint32_t unknownVTCK;
    bool hasTPLT;
    uint32_t unknownTPLT;
    uint32_t unknownRNAM;
    bool hasDEST;
    uint64_t unknownDEST;
    bool hasWNAM;
    uint32_t unknownWNAM;
    bool hasANAM;
    uint32_t unknownANAM;
    bool hasATKR;
    uint32_t unknownATKR;
    BinarySubRecord unknownATKD;
    std::string unknownATKE;
    std::vector<uint32_t> spellFormIDs;
    std::vector<PerkElem> perkList;
    std::vector<ComponentData> items;
    bool hasSPOR;
    uint32_t unknownSPOR;
    bool hasECOR;
    uint32_t unknownECOR;
    uint8_t unknownAIDT[20];
    std::vector<uint32_t> unknownPKIDs;
    std::vector<uint32_t> keywordArray;
    uint32_t unknownCNAM;
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    bool hasSHRT;
    uint32_t unknownSHRT;
    uint8_t unknownDNAM[52];
    std::vector<uint32_t> unknownPNAMs;
    bool hasHCLF;
    uint32_t unknownHCLF;
    bool hasGNAM;
    uint32_t unknownGNAM;
    bool hasZNAM;
    uint32_t unknownZNAM;
    uint16_t unknownNAM5;
    uint32_t unknownNAM6;
    uint32_t unknownNAM7;
    uint32_t unknownNAM8;
    bool hasDOFT;
    uint32_t unknownDOFT;
    bool hasSOFT;
    uint32_t unknownSOFT;
    bool hasCRIF;
    uint32_t unknownCRIF;
    bool hasCSCR;
    uint32_t unknownCSCR;
    std::vector<CSDXstruct> unknownCSDXs;
    bool hasDPLT;
    uint32_t unknownDPLT;
    bool hasFTST;
    uint32_t unknownFTST;
    uint8_t unknownQNAM[12];
    BinarySubRecord unknownNAM9;
    BinarySubRecord unknownNAMA;
    std::vector<TINXstructure> unknownTINXs;
}; //struct

} //namespace

#endif // SR_NPCRECORD_H
