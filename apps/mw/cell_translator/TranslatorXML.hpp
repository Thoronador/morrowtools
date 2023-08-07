/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Dirk Stolle

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

#ifndef MW_TRANSLATORXML_HPP
#define MW_TRANSLATORXML_HPP

/* This header has all the XML-related functions of the cell translator.
   At the moment, this is only one, but it might become more.
*/

#include <string>
#include <map>
#include "../../../lib/base/ComparisonFunctor.hpp"

namespace MWTP
{

typedef std::map<std::string, std::string, ci_less> CellListType;

/* Enumeration type for the direction of translation. */
enum TransDir {td_en_de, td_de_en};

/* tries to read the list of cells from the XML file located at fileName and
   stores them in the list pointed to by cells. Any previously stored content
   in the cells list will be erased during the function.
   Returns true on success.

   parameters:
       fileName   - the file name of the XML file that shall be read
       cells      - the structure that will hold the cell names and their
                    translation.
       direction  - the direction of translation. This parameter decides, in
                    which order the cell names are stored in the list 'cells'.
       noIdentity - if set to true, the list won't contain cells whose names
                    are identical in both languages.
*/
bool readCellListFromXML(const std::string& fileName, CellListType& cells, const TransDir direction = td_en_de, const bool noIdentity=true);

} //namespace

#endif // MW_TRANSLATORXML_HPP
