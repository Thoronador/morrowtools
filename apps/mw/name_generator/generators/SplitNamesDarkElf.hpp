/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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

#ifndef MW_NAMEGEN_GENERATOR_SPLIT_DARKELF_HPP
#define MW_NAMEGEN_GENERATOR_SPLIT_DARKELF_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "../../../../lib/mw/records/NPCRecord.hpp"

namespace MWTP
{

class SplitNamesDarkElf
{
  public:
    SplitNamesDarkElf();

    virtual ~SplitNamesDarkElf() = default;

    /** \brief Generates n Ashlander names.
     *
     * \param n   the amount of names to generate
     * \param female   whether to generate female names
     * \return Returns a vector containing the generated names.
     */
    std::vector<std::string> generate_ashlanders(const uint_least16_t n, const bool female);


    /** \brief Generates n non-Ashlander names.
     *
     * \param n   the amount of names to generate
     * \param female   whether to generate female names
     * \return Returns a vector containing the generated names.
     */
    std::vector<std::string> generate_non_ashlanders(const uint_least16_t n, const bool female);


    /** \brief Determines whether an NPC is an Ashlander.
     *
     * \param npc   the NPC to check
     * \return Returns true, if the NPC is an Ashlander.
     */
    static bool is_ashlander(const NPCRecord& npc);
  private:
    void prepare(const bool female);

    static std::vector<std::string> generate(const uint_least16_t n,
                                             const std::vector<std::string>& first_names,
                                             const std::vector<std::string>& last_names);

    std::vector<std::string> ashlander_first_names;
    std::vector<std::string> ashlander_last_names;

    std::vector<std::string> house_first_names;
    std::vector<std::string> house_last_names;
}; // class

} // namespace

#endif // MW_NAMEGEN_GENERATOR_SPLIT_DARKELF_HPP
