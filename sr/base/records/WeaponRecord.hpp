/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2021  Thoronador

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

#ifndef SR_WEAPONRECORD_HPP
#define SR_WEAPONRECORD_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "BasicRecord.hpp"
#include "BinarySubRecord.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

struct WeaponRecord: public BasicRecord
{
  public:
    /* constructor */
    WeaponRecord();

    /* copy constructor */
    WeaponRecord(const WeaponRecord& other);

    /* assignment operator */
    WeaponRecord& operator=(const WeaponRecord& other);

    /* destructor */
    virtual ~WeaponRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const WeaponRecord& other) const;
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

    static const unsigned int cTypeSwordOneHand; //swords
    static const unsigned int cTypeDagger; //daggers
    static const unsigned int cTypeAxeOneHand; //axes
    static const unsigned int cTypeMaceOneHand; //maces
    static const unsigned int cTypeSwordTwoHand; //swords (and giant clubs)
    static const unsigned int cTypeBluntTwoHand; //axes, war hammers
    static const unsigned int cTypeBow; //bows and crossbows
    static const unsigned int cTypeStaves; //staves

    std::string editorID;
    BinarySubRecord unknownVMAD;
    uint8_t unknownOBND[12];
    LocalizedString name; //subrecord FULL
    std::string modelPath;
    BinarySubRecord unknownMODT;
    BinarySubRecord unknownMODS;
    uint32_t enchantingFormID; //subrecord EITM
    bool hasEAMT;
    uint16_t enchantmentAmount; //subrecord EAMT
    uint32_t equipTypeFormID; //subrecord ETYP
    uint32_t blockBashImpactDataSetFormID; //subrecord BIDS
    uint32_t alternateBlockMaterialFormID; //subrecord BAMT
    std::vector<uint32_t> keywordArray;
    LocalizedString description; //subrecord DESC
    std::string unknownNNAM;
    uint32_t impactDataSetFormID; //subrecord INAM
    uint32_t firstPersonModelObjectFormID; //subrecord WNAM
    uint32_t attackSoundFormID; // subrecord SNAM
    uint32_t attackSound2DFormID; //subrecord XNAM
    uint32_t attackLoopSoundFormID; //subrecord NAM7
    uint32_t attackFailSoundFormID; //subrecord TNAM
    uint32_t idleSoundFormID; //subrecord UNAM
    uint32_t equipSoundFormID; //subrecord NAM9
    uint32_t unequipSoundFormID; //subrecord NAM8
    //DATA
    uint32_t value;
    float weight;
    uint16_t baseDamage;
    //end of data
    uint8_t  unknownDNAM[100];
    uint8_t  unknownCRDT[16];
    uint32_t unknownVNAM;
    bool hasCNAM;
    uint32_t unknownCNAM;
}; //struct

} //namespace

#endif // SR_WEAPONRECORD_HPP
