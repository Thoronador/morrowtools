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

#ifndef SR_SPELLRECORD_H
#define SR_SPELLRECORD_H

#include "BasicRecord.h"
#include "EffectBlock.h"
#include <string>
#include <stdint.h>

namespace SRTP
{

struct SpellRecord: public BasicRecord
{
  public:
    /* constructor */
    SpellRecord();

    /* destructor */
    virtual ~SpellRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const SpellRecord& other) const;
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

    /* flag constants */
    static const uint32_t cFlagNoAutoCalc;
    static const uint32_t cFlagPCStartSpell;
    static const uint32_t cFlagAreaEffectIgnoresLOS;
    static const uint32_t cFlagIgnoreResistance;
    static const uint32_t cFlagDisallowAbsorbReflect;
    static const uint32_t cFlagNoDualCastModifications;

    /* type constants */
    static const uint32_t cSpell;
    static const uint32_t cDisease;
    static const uint32_t cPower;
    static const uint32_t cLesserPower;
    static const uint32_t cAbility;
    static const uint32_t cPoison;
    static const uint32_t cAddiction;
    static const uint32_t cVoicePower;

    /* casting type constants */
    static const uint32_t cConstantEffect;
    static const uint32_t cFireAndForget;
    static const uint32_t cConcentration;

    /* delivery constants */
    static const uint32_t cSelf;
    static const uint32_t cContact;
    static const uint32_t cAimed;
    static const uint32_t cTargetActor;
    static const uint32_t cTargetLocation;

    std::string editorID;
    uint8_t unknownOBND[12];
    bool hasFULL;
    uint32_t nameStringID; //subrecord FULL
    uint32_t menuDisplayObjectFormID; //subrecord MDOB
    uint32_t equipTypeFormID; //subrecord ETYP
    uint32_t descriptionStringID; //subrecord DESC
    //subrecord SPIT
    uint32_t castingCost;
    uint32_t flags;
    uint32_t type;
    float chargeTime;
    uint32_t castingType;
    uint32_t delivery;
    float castDuration;
    float range; // (in ft)
    uint32_t castingPerkFormID;
    //end of subrecord SPIT
    std::vector<EffectBlock> effects;

    /* returns true, if the cost etc. are calculated automatically */
    bool doesAutoCalc() const;

    /* returns true, if the spell is a PC start spell, according to flags */
    bool isPCStartSpell() const;

    /* returns true, if the spell ignores LOS for area effects, according to flags */
    bool areaEffectIgnoresLOS() const;

    /* returns true, if the spell ignores resistance, according to flags */
    bool ignoresResistance() const;

    /* returns true, if the spell cannot be absorbed or reflected, according to flags */
    bool disallowsAbsorbAndReflect() const;

    /* returns true, if the spell has no dual cast modifications, according to flags */
    bool noDualCastModifications() const;
}; //struct

} //namespace

#endif // SR_SPELLRECORD_H
