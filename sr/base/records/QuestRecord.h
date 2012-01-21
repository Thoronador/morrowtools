/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#ifndef SR_QUESTRECORD_H
#define SR_QUESTRECORD_H

#include <string>
#include <vector>
#include "BasicRecord.h"
#include "BinarySubRecord.h"
#include "ComponentData.h"
#include "CTDAData.h"

namespace SRTP
{

struct QuestRecord: public BasicRecord
{
  public:
    /* constructor */
    QuestRecord();

    /* destructor */
    virtual ~QuestRecord();

    /* returns true, if the other record contains the same data */
    bool equals(const QuestRecord& other) const;

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

    /* type for holding quest stages / indices*/
    struct IndexEntry
    {

      struct QSDTRecord
      {
        bool isFinisher; //true, if entry finishes the quest
        bool hasNAM0;
        uint32_t unknownNAM0;
        BinarySubRecord unknownSCHR;
        std::string unknownSCTX;
        bool hasQNAM;
        uint32_t unknownQNAM;
        std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
        bool hasCNAM;
        uint32_t unknownCNAM; //CNAM - string ID of journal text?

        /* constructor */
        QSDTRecord();

        /* equality operator */
        bool operator==(const QSDTRecord& other) const;
      }; //struct
      uint16_t index; //stage index
      uint16_t indexUnknownPart; //flags or something?
      std::vector<QSDTRecord> theQSDTs;

      /* constructor */
      IndexEntry();

      /* equality operator */
      bool operator==(const IndexEntry& other) const;
    };//struct

    //struct for alias-related data
    struct AliasEntry
    {
      bool hasALST;
      uint32_t unknownALST;
      bool hasALLS;
      uint32_t unknownALLS;
      std::string aliasID; //subrecord ALID
      uint32_t unknownFNAM;
      bool hasALFA;
      uint32_t unknownALFA;
      bool hasALRT;
      uint32_t unknownALRT;
      bool hasALCO;
      uint32_t unknownALCO;
      bool hasALCA;
      uint32_t unknownALCA;
      bool hasALCL;
      uint32_t unknownALCL;
      bool hasALDN;
      uint32_t unknownALDN;
      std::vector<ComponentData> components;
      std::vector<uint32_t> keywordArray;
      bool hasALFE;
      uint32_t unknownALFE;
      bool hasALFD;
      uint32_t unknownALFD;
      bool hasALFI;
      uint32_t unknownALFI;
      bool hasALFL;
      uint32_t unknownALFL;
      bool hasALFR;
      uint32_t unknownALFR;
      bool hasALNA;
      uint32_t unknownALNA;
      bool hasALNT;
      uint32_t unknownALNT;
      bool hasALUA;
      uint32_t unknownALUA;
      bool hasALEQ;
      uint32_t unknownALEQ;
      bool hasALEA;
      uint32_t unknownALEA;
      bool hasKNAM;
      uint32_t unknownKNAM;
      std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
      bool hasSPOR;
      uint32_t unknownSPOR;
      bool hasECOR;
      uint32_t unknownECOR;
      std::vector<uint32_t> unknownALSPs;
      std::vector<uint32_t> unknownALFCs;
      std::vector<uint32_t> unknownALPCs;
      bool hasVTCK;
      uint32_t unknownVTCK;

      /* constructor */
      AliasEntry();

      /* clears all data members */
      void clear();

      /* equality operator */
      bool operator==(const AliasEntry& other) const;
    };//struct

    //struct for QOBJ
    struct QOBJEntry
    {
      //struct for QSTA stuff
      struct QSTAEntry
      {
        uint64_t unknownQSTA;
        std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;

        /* constructor */
        QSTAEntry();

        /* clears all data members */
        void clear();

        /* equality operator */
        bool operator==(const QSTAEntry& other) const;
      };//struct

      uint16_t unknownQOBJ;
      uint32_t unknownFNAM;
      uint32_t unknownNNAM;
      std::vector<QSTAEntry> theQSTAs;

      /* constructor */
      QOBJEntry();

      /* clears all data members */
      void clear();

      /* equality operator */
      bool operator==(const QOBJEntry& other) const;
    };//struct

    std::string editorID;
    BinarySubRecord unknownVMAD;
    bool hasFULL;
    uint32_t unknownFULL;
    uint8_t unknownDNAM[12];
    bool hasENAM;
    uint32_t unknownENAM;
    std::vector<uint32_t> unknownQTGLs;
    std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
    std::string filter;
    std::vector<IndexEntry> indices;
    std::vector<QOBJEntry> theQOBJs;
    uint32_t unknownANAM;
    std::vector<AliasEntry> aliases;
};//struct

} //namespace

#endif // SR_QUESTRECORD_H
