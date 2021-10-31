/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#ifndef MWTP_SLASHFUNCTIONS_HPP
#define MWTP_SLASHFUNCTIONS_HPP

#include <string>

namespace MWTP
{

/** \brief Replaces all backslashes in a string by forward slashes.
 *
 * \param s  the string where backslashes shall be flipped.
 * \return Returns s with all backslashes replaced by forward slashes.
 */
std::string flipBackslashes(std::string s);

} // namespace

#endif // MWTP_SLASHFUNCTIONS_HPP
