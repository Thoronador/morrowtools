/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#ifndef MW_HELPERIO_HPP
#define MW_HELPERIO_HPP

#include <cstdint>
#include <string>

/** \brief Converts four character "integer strings" to real C++ string.
 *
 * \remark This is not a "convert this integer to a string" function. Instead,
 *         it takes the four bytes from the integer an reinterprets each of them
 *         as a single character that - together - form a string of length 4.
 * \param value  the 32 bit integer value
 * \return Returns the string equivalent.
 */
std::string IntTo4Char(const uint32_t value);

/** \brief Utility function to write a message on wrong records to stderr.
 *
 * \param expected  the expected record header
 * \param unexpected  the record header that was actually found
 */
void UnexpectedRecord(const uint32_t expected, const uint32_t unexpected);

#endif // MW_HELPERIO_HPP
