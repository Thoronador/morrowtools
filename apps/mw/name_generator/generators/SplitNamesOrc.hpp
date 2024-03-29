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

#ifndef MW_NAMEGEN_GENERATOR_SPLIT_ORC_HPP
#define MW_NAMEGEN_GENERATOR_SPLIT_ORC_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace MWTP
{

class SplitNamesOrc
{
  public:
    SplitNamesOrc();

    virtual ~SplitNamesOrc() = default;

    /** \brief Generates n names.
     *
     * \param n   the amount of names to generate
     * \param female   whether to generate female names
     * \return Returns a vector containing the generated names.
     */
    std::vector<std::string> generate(const uint_least16_t n, const bool female);
  private:
    void prepare(const bool female);

    std::vector<std::string> first_names;
    std::vector<std::string> clan_names;
}; // class

} // namespace

#endif // MW_NAMEGEN_GENERATOR_SPLIT_ORC_HPP
