/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2021  Dirk Stolle

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

#ifndef MW_SKILLS_HPP
#define MW_SKILLS_HPP

#include <map>
#include <string>
#include <fstream>
#include "records/SkillRecord.hpp"

namespace MWTP
{

class Skills
{
  public:
    /** \brief Provides access to the singleton instance.
     *
     * \return Returns a reference to the singleton instance.
     */
    static Skills& get();

    /** \brief Adds a new skill to the instance.
     *
     * \param SkillData   the skill to add
     */
    void addSkill(const SkillRecord& SkillData);

    /** \brief Checks whether a skill with the given index is present.
     *
     * \param Index  the index of the skill
     * \return Returns true, if a skill with the given index is present.
     *         Returns false otherwise.
     */
    bool hasSkill(const int32_t Index) const;

    /** Gets a reference to the skill with the given index.
     *
     * \param Index  the index of the skill
     * \return Returns a reference to the record with the given index, if such
     *         a record is present. Throws, if no such record exists.
     * \remarks If no record with the given index is present, the method will
     *          throw an exception. Use hasSkill() to determine, if a skill
     *          with the desired index is present.
     */
    const SkillRecord& getSkill(const int32_t Index) const;

    /** Gets the name of the game setting that contains the name of the skill.
     *
     * \param Index  the index of the skill
     * \return Returns the name of the game setting that contains the name of
     *         the skill specified by the given index.
     */
    static std::string getSettingNameForSkill(const int32_t Index);

    /** Gets the number of skills in the instance.
     *
     * \return Returns the number of skills in the instance.
     */
    unsigned int getNumberOfSkills() const;

    /** \brief Tries to read a skill record from the given input stream.
     *
     * \param input       the input stream that is used to read the record
     * \return If an error occurred, the function returns -1. Otherwise it
     *         returns the number of updated records. (Usually that is one.
     *         If, however, the record that was read is equal to the one already
     *         in the list, zero is returned and the existing record remains
     *         unchanged.)
     */
    int readNextRecord(std::istream& input);

    /// iterator type
    typedef std::map<int32_t, SkillRecord>::const_iterator Iterator;

    /** Returns constant iterator to the beginning of the internal structure. */
    Iterator begin() const;

    /** Returns constant iterator to the end of the internal structure. */
    Iterator end() const;

    #ifndef MW_UNSAVEABLE_RECORDS
    /** \brief Tries to save all available records to the given stream.
     *
     * \param output  the output stream that shall be used to save the records
     * \return Returns true on success, false on failure.
     */
    bool saveAllToStream(std::ostream& output) const;
    #endif // MW_UNSAVEABLE_RECORDS

    /** Removes all records from the instance. */
    void clear();
  private:
    /** Constructor - private due to singleton pattern. */
    Skills();

    /* no copy constructor due to singleton pattern */
    Skills(const Skills& op) = delete;
    Skills(Skills&& op) = delete;

    std::map<int32_t, SkillRecord> m_Skills; /**< internal data */
}; // class

} // namespace

#endif // MW_SKILLS_HPP
