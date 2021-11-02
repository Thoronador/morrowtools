/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2015, 2021  Thoronador

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

#include <iostream>
#include <string>
#include "ESMReaderFinderReferences.hpp"

/* auxiliary function #1:
   Returns true, if the keyword is found in haystack. Character case is taken
   into account, if caseMatters is true.

   parameters:
       haystack    - the string to search in
       keyword     - the string to search for
       caseMatters - If true, case-sensitive search is performed. Otherwise the
                     search is case-insensitive.
*/
bool matchesKeyword(const std::string& haystack, const std::string& keyword, const bool caseMatters);

/*...and another auxiliary function (#2):
  Writes all references of a base ID to the given ostream.

  parameters:
      baseID    - baseID of the object
      refMap    - reference map as produced by ESMReaderFinderReferences
      basic_out - the output stream to which the references are written
*/
void showRefIDs(const uint32_t baseID, const std::map<uint32_t, std::vector<SRTP::ESMReaderFinderReferences::CellRefIDPair> >& refMap, std::basic_ostream<char>& basic_out);
