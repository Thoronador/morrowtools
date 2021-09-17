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

#ifndef SR_SPELLRECORD_HPP
#define SR_SPELLRECORD_HPP

#include <array>
#include <cstdint>
#include <string>
#include "BasicRecord.hpp"
#include "EffectBlock.hpp"
#include "LocalizedString.hpp"

namespace SRTP
{

/** Holds information about a spell. */
struct SpellRecord: public BasicRecord
{
  public:
    /** Constructor, creates an empty record. */
    SpellRecord();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool equals(const SpellRecord& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Gets the size in bytes that the record's data would occupy in a file
     *         stream, NOT including the header data.
     *
     * \return Returns the size in bytes that the record would need. Size of the
     *         header is not included.
     */
    virtual uint32_t getWriteSize() const;

    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    virtual bool saveToStream(std::ostream& output) const;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param in_File    the input stream
     * \param localized  whether the file to read from is localized or not
     * \param table      the associated string table for localized files
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     */
    virtual bool loadFromStream(std::istream& in_File, const bool localized, const StringTable& table);

    /** \brief Gets the record's type, usually its header.
     *
     * \return Returns the record's type.
     */
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
    std::array<uint8_t, 12> unknownOBND;
    LocalizedString name; // subrecord FULL
    uint32_t menuDisplayObjectFormID; // subrecord MDOB
    uint32_t equipTypeFormID; // subrecord ETYP
    LocalizedString description; // subrecord DESC
    // subrecord SPIT
    uint32_t castingCost;
    uint32_t flags;
    uint32_t type;
    float chargeTime;
    uint32_t castingType;
    uint32_t delivery;
    float castDuration;
    float range; /**< spell range in feet */
    uint32_t castingPerkFormID;
    // end of subrecord SPIT
    std::vector<EffectBlock> effects;

    /** \brief Checks whether the cost etc. are calculated automatically.
     *
     * \return Returns true, if the costs etc. are calculated automatically.
     *         Returns false otherwise.
     */
    bool doesAutoCalc() const;

    /** \brief Checks whether the spell is a PC start spell.
     *
     * \return Returns true, if the spell is a PC start spell.
     *         Returns false otherwise.
     */
    bool isPCStartSpell() const;

    /** \brief Checks whether the ignores LOS for area effects.
     *
     * \return Returns true, if the spell ignores LOS for area effects.
     *         Returns false otherwise.
     */
    bool areaEffectIgnoresLOS() const;

    /** \brief Checks whether the spell ignores resistance.
     *
     * \return Returns true, if the spell ignores resistance.
     *         Returns false otherwise.
     */
    bool ignoresResistance() const;

    /** \brief Checks whether the spell cannot be absorbed or reflected.
     *
     * \return Returns true, if the spell cannot be absorbed or reflected.
     *         Returns false otherwise.
     */
    bool disallowsAbsorbAndReflect() const;

    /** \brief Checks whether the spell has no dual cast modifications.
     *
     * \return Returns true, if the spell has no dual cast modifications.
     *         Returns false otherwise.
     */
    bool noDualCastModifications() const;
}; // struct

} // namespace

#endif // SR_SPELLRECORD_HPP
