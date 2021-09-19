/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Thoronador

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

#ifndef SR_SPELLITEM_HPP
#define SR_SPELLITEM_HPP

#include <cstdint>
#include <fstream>

namespace SRTP
{

/** Holds data about a spell or spell-like effect. */
struct SpellItem
{
  public:
    /** Constructor, creates an empty record. */
    SpellItem();

    #ifndef SR_NO_RECORD_EQUALITY
    /** \brief Checks whether another instance contains the same data.
     *
     * \param other   the other record to compare with
     * \return Returns true, if @other contains the same data as instance.
     *         Returns false otherwise.
     */
    bool operator==(const SpellItem& other) const;
    #endif

    #ifndef SR_UNSAVEABLE_RECORDS
    /** \brief Writes the record to the given output stream.
     *
     * \param output  the output stream
     * \return Returns true on success (record was written to stream).
     *         Returns false, if an error occurred.
     */
    bool saveToStream(std::ostream& output) const;
    #endif

    /** \brief Loads the record from the given input stream.
     *
     * \param inout       the input stream to read from
     * \param recordName  name of the parent record
     * \param bytesRead   variable that holds the number of bytes read so far
     * \return Returns true on success (record was loaded from stream).
     *         Returns false, if an error occurred.
     * \remarks The header (SPIT) is not included, reading starts with the
     *          length value.
     */
    bool loadFromStream(std::istream& input, const uint32_t recordName, uint32_t& bytesRead);

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

    uint32_t castingCost;
    uint32_t flags;
    uint32_t type;
    float chargeTime; /**< charging time in seconds */
    uint32_t castingType;
    uint32_t delivery;
    float castDuration;
    float range; /**< spell range in feet */
    uint32_t castingPerkFormID; /**< form id of perk for halving costs */

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

#endif // SR_SPELLITEM_HPP
