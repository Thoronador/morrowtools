/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013, 2021  Dirk Stolle

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

#ifndef MW_MAGICEFFECTS_HPP
#define MW_MAGICEFFECTS_HPP

#include <string>
#include <map>
#include <fstream>
#include "records/MagicEffectRecord.hpp"

namespace MWTP
{

class MagicEffects
{
  public:
    /** \brief Provides access to the singleton instance.
     *
     * \return Returns a reference to the singleton instance.
     */
    static MagicEffects& get();

    /** \brief Adds an effect to the instance.
     *
     * \param effect   the effect to add
     * \remarks An existing record with the same index will be replaced.
     */
    void addEffect(const MagicEffectRecord& effect);

    /** \brief Checks whether an effect with the given index is present.
     *
     * \param index  the index of the magic effect
     * \return Returns true, if an effect with the given index is present.
     *         Returns false otherwise.
     */
    bool hasEffect(const EffectIndex index) const;

    /** Gets the number of effects in the instance.
     *
     * \return Returns the number of effects in the instance.
     */
    unsigned int getNumberOfEffects() const;

    /** Gets a reference to the effect with the given index.
     *
     * \param index  the index of the effect
     * \return Returns a reference to the effect with the given index, if such
     *         an effect is present. Throws, if no such effect exists.
     * \remarks If no effect with the given index is present, the method will
     *          throw an exception. Use hasEffect() to determine, if an effect
     *          with the desired index is present.
     */
    const MagicEffectRecord& getEffect(const EffectIndex index) const;

    /** \brief Gets the name of the game setting that contains the name of the
     * effect specified by the given index.
     *
     * \param index  the index of the effect
     * \return Returns the name of the game setting that contains the name of
     *         the effect.
     */
    static std::string getSettingNameForEffect(const EffectIndex index);

    /** \brief Checks whether the given index identifies a skill-related effect.
     *
     * \param index  the index of the effect
     * \return Returns true, if the index identifies a skill-related effect.
     *         Returns false otherwise.
     */
    static bool isSkillRelatedEffect(const EffectIndex index);

    /** \brief Checks whether the given index identifies an attribute-related
     * effect.
     *
     * \param index  the index of the effect
     * \return Returns true, if the index identifies an attribute-related
     *         effect. Returns false otherwise.
     */
    static bool isAttributeRelatedEffect(const EffectIndex index);

    /** \brief Reads a magic effect record from the given input stream.
     *
     * \param input  the input stream that is used to read the record
     * \return  If an error occurred, the function returns -1. Otherwise it
     *          returns the number of updated records. (Usually that is one.
     *          If, however, the record that was read is equal to the one
     *          already in the list, zero is returned.)
     */
    int readNextRecord(std::istream& input);

    /// iterator type for list
    typedef std::map<EffectIndex, MagicEffectRecord>::const_iterator Iterator;

    /** Returns a constant iterator to the beginning of the internal list. */
    Iterator begin() const;

    /** Returns a constant iterator to the end of the internal list. */
    Iterator end() const;

    #ifndef MW_UNSAVEABLE_RECORDS
    /** \brief Tries to save all available magic effects to the given stream.
     *
     * \param output  the output stream that shall be used to save the data
     * \return Returns true on success, false on failure.
     */
    bool saveAllToStream(std::ostream& output) const;
    #endif // MW_UNSAVEABLE_RECORDS

    /** Removes all effects from the instance. */
    void clear();
  private:
    MagicEffects();

    /* copy + move constructor: deleted, because this is a singleton */
    MagicEffects(const MagicEffects& op) = delete;
    MagicEffects(MagicEffects&& op) = delete;

    std::map<EffectIndex, MagicEffectRecord> m_Effects; /**< internal list */
}; // class

} // namespace

#endif // MW_MAGICEFFECTS_HPP
