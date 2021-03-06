/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2015  Thoronador

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

#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <string>
#include <stdint.h>

/* tries to convert a the string representation of a integer number into
   a 16-bit int and returns true on success, false on failure.

   parameters:
       str   - the string that contains the number
       value - the short int that will be used to store the result

   remarks:
       If false is returned, the value of parameter value is undefined.
*/
bool stringToShort(const std::string& str, int16_t& value);

/* tries to convert a the string representation of a integer number into
   a 32-bit int and returns true on success, false on failure.

   parameters:
       str   - the string that contains the number
       value - the int that will be used to store the result

   remarks:
       If false is returned, the value of parameter value is undefined.
*/
bool stringToLong(const std::string& str, int32_t& value);

/* converts an integer value into its string representation

   parameters:
       value - the integer value
*/
std::string intToString(const int value);

/* tries to convert a the string representation of a floating point value into
   a float and returns true on success, false on failure.

   parameters:
       str   - the string that contains the number
       value - the float that will be used to store the result

   remarks:
       If false is returned, the value of parameter value is undefined.
*/
bool stringToFloat(const std::string& str, float& value);

/* converts a floating point value into its string representation

   parameters:
       f - the floating point value
*/
std::string floatToString(const float f);

/* removes all leading and trailing spaces and (horizontal) tabulators from the
   given string

   parameters:
       str1 - the affected string
*/
void trim(std::string& str1);

/* removes all leading spaces and (horizontal) tabulators from the given string

   parameters:
       str1 - the affected string
*/
void trimLeft(std::string& str1);

/* returns the lower case version of the given string

   parameters:
       str1 - the string
*/
std::string lowerCase(const std::string& str1);

/*tries to remove the bracket pair surrounding the rest of the string, if they
  match. Returns true, if something was removed, false otherwise.

  parameters:
      expr - the string
*/
bool removeEnclosingBrackets(std::string& expr);

/*tries to remove the quote pair surrounding the rest of the string. Returns
  true, if something was removed, false otherwise.

  parameters:
      str1 - the string
*/
bool stripEnclosingQuotes(std::string& str1);

/* returns an integer less than zero, if the left string is ordered before the
   right string. Returns zero, if they are equivalent. Returns a value greater
   than zero if the left string is ordered after the right string. The
   comparison is case-insensitive (i.e. it treats the strings as if all
   characters were lower case).

   parameters:
       left  - the left string
       right - the other string
*/
int lowerCaseCompare(const std::string& left, const std::string& right);

#endif // UTILITYFUNCTIONS_H
