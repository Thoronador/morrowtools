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

#ifndef MW_NAMEGEN_GENERATOR_HPP
#define MW_NAMEGEN_GENERATOR_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace MWTP
{

/// Basic interface for name generators.
class Generator
{
  public:
    virtual ~Generator() = default;


    /** \brief Generates n names.
     *
     * \param n   the amount of names to generate
     * \return Returns a vector containing the generated names.
     */
    virtual std::vector<std::string> generate(const uint_least16_t n) = 0;


    /** \brief Gets the NPC IDs that need to be removed before generation starts.
     *
     * \return Returns a vector of the IDs that shall be removed.
     */
    virtual std::vector<std::string> purge() = 0;
}; // class

} // namespace

#endif // MW_NAMEGEN_GENERATOR_HPP
