/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_QUESTRECORD_HPP
#define SR_QUESTRECORD_HPP

#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "ComponentData.hpp"
#include "CTDAData.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct QuestRecord: public BasicRecord
{
  public:
    /* constructor */
    QuestRecord();

    /* destructor */
    virtual ~QuestRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const QuestRecord& other) const;
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

    /* type for holding quest stages / indices*/
    struct IndexEntry
    {

      struct QSDTRecord
      {
        bool isFinisher; //true, if entry finishes the quest
        uint32_t nextQuestFormID; //subrecord NAM0
        BinarySubRecord unknownSCHR;
        std::string unknownSCTX;
        bool hasQNAM;
        uint32_t unknownQNAM;
        std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
        LocalizedString logEntry; //CNAM - string ID of log entry

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

      /* returns true, if the finishing flag is set for one of the index' QSDTs */
      bool hasFinishingQSDT() const;
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
      uint32_t locationRefTypeFormID; //subrecord ALRT
      uint32_t createReferenceToObjectFormID; //subrecord ALCO
      bool hasALCA;
      uint32_t unknownALCA;
      bool hasALCL;
      uint32_t unknownALCL;
      uint32_t displayNameFormID; //subrecord ALDN
      std::vector<ComponentData> components;
      std::vector<uint32_t> keywordArray;
      bool hasALFE;
      uint32_t unknownALFE;
      bool hasALFD;
      uint32_t unknownALFD;
      bool hasALFI;
      uint32_t forcedIntoAliasID; //subrecord ALFI
      uint32_t specificLocationFormID; //subrecord ALFL
      uint32_t specificReferenceID; //subrecord ALFR
      bool hasALNA;
      uint32_t unknownALNA;
      bool hasALNT;
      uint32_t unknownALNT;
      uint32_t uniqueActorFormID; //subrecord ALUA
      uint32_t externalAliasReferenceFormID; //subrecord ALEQ
      bool hasALEA;
      uint32_t unknownALEA;
      uint32_t keywordFormID; //subrecord KNAM
      std::vector<CTDA_CIS2_compound> unknownCTDA_CIS2s;
      uint32_t spectatorOverridePackageListFormID; //subrecord SPOR
      uint32_t combatOverridePackageListFormID; //subrecord ECOR
      std::vector<uint32_t> spellFormIDs; //subrecords ALSP
      std::vector<uint32_t> factionFormIDs; //subrecords ALFC
      std::vector<uint32_t> packageDataFormIDs; //subrecords ALPC
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
      LocalizedString displayText; //subrecord NNAM
      std::vector<QSTAEntry> theQSTAs;

      /* constructor */
      QOBJEntry();

      /* clears all data members */
      void clear();

      /* equality operator */
      bool operator==(const QOBJEntry& other) const;
    };//struct


    /* returns true, if an QOBJ record with the given index is present

       parameters:
           idx - the requested index
    */
    bool hasQOBJForIndex(const uint16_t idx) const;

    /* returns the QOBJ record with the given index, if it is present.
       If no such QOBJ is present, the function will throw an exception. Use
       hasQOBJForIndex() to check for presence first.

       parameters:
           idx - the requested index
    */
    const QOBJEntry& getQOBJForIndex(const uint16_t idx) const;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    LocalizedString name; //subrecord FULL
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

#endif // SR_QUESTRECORD_HPP
