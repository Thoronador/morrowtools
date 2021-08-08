/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2015, 2021  Thoronador

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

#ifndef UTILITYFUNCTIONS_HPP
#define UTILITYFUNCTIONS_HPP

#include <cstdint>
#include <string>

/** \brief Tries to convert the string representation of an integer number into
 *         a 16-bit int and returns true on success, false on failure.
 *
 * \param str    the string that contains the number
 * \param value  the short int that will be used to store the result
 * \return Returns true on success, false on failure.
 * \remark If false is returned, the value of parameter @value is undefined.
 */
bool stringToShort(const std::string& str, int16_t& value);

/** \brief Tries to convert a the string representation of a integer number into
 * a 32-bit int and returns true on success, false on failure.
 *
 *
 * \param str    the string that contains the number
 * \param value  the int that will be used to store the result
 * \return Returns true on success, false on failure.
 * \remarks If false is returned, the value of parameter @value is undefined.
 */
bool stringToLong(const std::string& str, int32_t& value);

/** \brief Converts an integer value into its string representation.
 *
 * \param value  the integer value
 * \return Returns the string representation of @value.
 */
std::string intToString(const int value);

/** \brief Tries to convert a the string representation of a floating point
 *         value into a float and returns true on success, false on failure.
 *
 * \param str    the string that contains the number
 * \param value  the float that will be used to store the result
 * \return Returns true on success, or false on failure.
 * \remarks If false is returned, the value of parameter value is undefined.
 */
bool stringToFloat(const std::string& str, float& value);

/** \brief Converts a floating point value into its string representation.
 *
 * \param f  the floating point value
 * \return Returns the string prepresentation of @f.
 */
std::string floatToString(const float f);

/** \brief Removes all leading and trailing spaces and (horizontal) tabulators
 *         from the given string.
 * \param str1  the affected string, will be trimmed in place
 */
void trim(std::string& str1);

/** \brief Removes all leading spaces and (horizontal) tabulators from the
 *         given string.
 * \param str1  the affected string
 */
void trimLeft(std::string& str1);

/** \brief Returns the lower case version of the given string.
 *
 * \param str1  the string
 * \return Returns the lower case version of the given string.
 */
std::string lowerCase(const std::string& str1);

/** \brief Tries to remove the bracket pair surrounding the rest of the string,
 *  if they match.
 *
 * \return Returns true, if something was removed, false otherwise.
 * \param expr  the string
 */
bool removeEnclosingBrackets(std::string& expr);

/** \brief Tries to remove the quote pair surrounding the rest of the string.
 *
 * \return Returns true, if something was removed, false otherwise.
 * \param str1  the string
 */
bool stripEnclosingQuotes(std::string& str1);

/** Three-way compares two strings, ignoring the case.
 *
 * \param left   the left string
 * \param right  the other string
 * \return Returns an integer less than zero, if the left string is ordered
 *         before the right string. Returns zero, if they are equivalent.
 *         Returns a value greater than zero if the left string is ordered after
 *         the right string. The comparison is case-insensitive (i.e. it treats
 *         the strings as if all characters were lower case).
 */
int lowerCaseCompare(const std::string& left, const std::string& right);

#endif // UTILITYFUNCTIONS_HPP
