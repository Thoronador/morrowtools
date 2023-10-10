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

#ifndef MW_NAMEGEN_GENERATOR_IMPERIAL_PURGE_HPP
#define MW_NAMEGEN_GENERATOR_IMPERIAL_PURGE_HPP

#include "Generator.hpp"

namespace MWTP
{

/// Implements the purge() method for all generators of Imperial names.
class ImperialNamesPurge: public Generator
{
  public:
    /** \brief Gets the NPC IDs that need to be removed before generation starts.
     *
     * \return Returns a vector of the IDs that shall be removed.
     */
    std::vector<std::string> purge() override;
}; // class

} // namespace

#endif // MW_NAMEGEN_GENERATOR_IMPERIAL_PURGE_HPP
