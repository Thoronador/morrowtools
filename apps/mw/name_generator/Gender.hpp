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

#ifndef MW_GENDER_HPP
#define MW_GENDER_HPP

#include <string>

namespace MWTP
{

/// enumeration determining for which gender the names are generated
enum class Gender { Male, Female, Both };

/** \brief Translates enumeration value in a human-readable string.
 *
 * \param g   the enumeration value to translate
 * \return Returns a human-readable string desribing the enumeration value.
 */
std::string to_string(const Gender g);

} // namespace

#endif // MW_GENDER_HPP
