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

#ifndef MW_NAMEGENFUNCTIONS_HPP
#define MW_NAMEGENFUNCTIONS_HPP

#include <optional>
#include <string>
#include "Gender.hpp"

namespace MWTP
{

/// Removes possibly existing non-playable races from the data pool.
void removeNonPlayableRaces();


/** \brief Lets the user select a playable race via standard input / output.
 *
 * \return Returns the ID of the selected race, if a valid selection was made.
 *         Returns an empty string, if the selection failed.
 */
std::string selectRace();


/** \brief Lets the user select a playable gender via standard input / output.
 *
 * \return Returns the selected gender, if a valid selection was made.
 *         Returns an empty optional, if the selection failed.
 */
std::optional<Gender> selectGender();


/** \brief Lets the user select the number of names to generate.
 *
 * \return Returns the count, if a valid input was made.
 *         Returns an empty optional, if the input was invalid.
 */
std::optional<uint_least16_t> selectCount();

} // namespace

#endif // MW_NAMEGENFUNCTIONS_HPP
