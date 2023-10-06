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

#ifndef MW_NAMEGEN_FACTORY_HPP
#define MW_NAMEGEN_FACTORY_HPP

#include <memory>
#include "Generator.hpp"
#include "../Gender.hpp"

namespace MWTP
{

/** \brief "Factory" for Generator instances.
 */
class Factory
{
  public:
    /// no constructor needed
    Factory() = delete;

    /** \brief Creates a Generate instance based on the given race and gender type.
     *
     * \param race    ID of the race
     * \param gender  gender of the generated names
     * \return Returns a unique_ptr to the created instance.
     *         Returns nullptr, if parameter combination is not supported.
     */
    static std::unique_ptr<Generator> create(const std::string& race, const Gender gender);
}; // class

} // namespace

#endif // MW_NAMEGEN_FACTORY_HPP
