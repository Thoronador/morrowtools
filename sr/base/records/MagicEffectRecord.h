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

#ifndef SR_MAGICEFFECTRECORD_H
#define SR_MAGICEFFECTRECORD_H

#include "BasicRecord.h"
#include <map>
#include <string>
#include <vector>
#include "BinarySubRecord.h"
#include "CTDAData.h"

namespace SRTP
{

struct MagicEffectRecord: public BasicRecord
{
  public:
    /* constructor */
    MagicEffectRecord();

    /* destructor */
    virtual ~MagicEffectRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /* returns true, if the other record contains the same data */
    bool equals(const MagicEffectRecord& other) const;
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
    static const uint32_t cFlagHostile;
    static const uint32_t cFlagRecover;
    static const uint32_t cFlagDetrimetral;
    static const uint32_t cFlagNoHitEvent;
    static const uint32_t cFlagDispelEffects;
    static const uint32_t cFlagNoDuration;
    static const uint32_t cFlagNoMagnitude;
    static const uint32_t cFlagNoArea;
    static const uint32_t cFlagFXPersist;
    static const uint32_t cFlagGoryVisuals;
    static const uint32_t cFlagHideInUI;
    static const uint32_t cFlagNoRecast;
    static const uint32_t cFlagPowerAffectsMagnitude;
    static const uint32_t cFlagPowerAffectsDuration;
    static const uint32_t cFlagPainless;
    static const uint32_t cFlagNoHitEffect;
    static const uint32_t cFlagNoDeathDispel;

    /* magic skill constants */
    static const uint32_t cSkillAlteration;
    static const uint32_t cSkillConjuration;
    static const uint32_t cSkillDestruction;
    static const uint32_t cSkillIllusion;
    static const uint32_t cSkillRestoration;
    static const uint32_t cSkillNone;

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

    /* sound level constants */
    static const uint32_t cSoundLevelLoud;
    static const uint32_t cSoundLevelNormal;
    static const uint32_t cSoundLevelSilent;
    static const uint32_t cSoundLevelVeryLoud;

    /* sound type constants */
    static const uint32_t cSoundTypeDrawSheathe;
    static const uint32_t cSoundTypeCharge;
    static const uint32_t cSoundTypeReady;
    static const uint32_t cSoundTypeRelease;
    static const uint32_t cSoundTypeCastLoop;
    static const uint32_t cSoundTypeOnHit;

    std::string editorID;
    BinarySubRecord unknownVMAD;
    bool hasFULL;
    uint32_t fullNameStringID;
    uint32_t menuDisplayObjectFormID; //subrecord MDOB
    std::vector<uint32_t> keywordArray;
    //subrecord DATA
    uint32_t flags;
    float baseCost;
    uint32_t unknownDATA03;
    uint32_t magicSkill;
    uint32_t unknownDATA05;
    uint32_t unknownDATA06;
    uint32_t unknownDATA07;
    float    taperWeight;
    uint32_t hitShaderFormID;
    uint32_t enchantShaderFormID;
    uint32_t skillLevel;
    uint32_t area;
    float    castingTime;
    float    taperCurve;
    float    taperDuration;
    uint32_t unknownDATA16;
    uint32_t unknownDATA17;
    uint32_t assocItem1;
    uint32_t projectileFormID;
    uint32_t explosionFormID;
    uint32_t castingType;
    uint32_t delivery;
    uint32_t unknownDATA23;
    uint32_t castingArtFormID;
    uint32_t hitEffectArtFormID;
    uint32_t impactDataSetFormID;
    float    skillUsageMult;
    uint32_t dualCastingArt;
    float    dualCastingScale;
    uint32_t enchantArtFormID;
    uint32_t unknownDATA31;
    uint32_t unknownDATA32;
    uint32_t equipAbilityFormID;
    uint32_t imageSpaceModFormID;
    uint32_t perkFormID;
    uint32_t castingSoundLevel;
    float scriptEffectAIDataScore;
    float scriptEffectAIDataDelayTime;
    //end of DATA
    std::map<uint32_t, uint32_t> sounds; //subrecord SNDD
    uint32_t descriptionStringID; //DNAM
    std::vector<CTDAData> unknownCTDAs;
}; //struct

} //namespace

#endif // SR_MAGICEFFECTRECORD_H
