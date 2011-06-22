/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef TRANSLATORXML_H
#define TRANSLATORXML_H

/* This header has all the XML-related functions of the cell translator.
   At the moment, this is only one, but it might become more.
*/

#include <string>
#include <map>
#include "../base/ComparisonFunctor.h"

namespace MWTP
{

typedef std::map<std::string, std::string, ci_less> CellListType;

/* Enumeration type for the direction of translation. */
enum TransDir {td_en_de, td_de_en};

/* tries to read the list of cells from the XML file located at fileName and
   stores them in the list pointed to by cells. Returns true on success.

   parameters:
       fileName  - the file name of the XML file that shall be read
       cells     - the structure that will hold the cell names and their
                   translation
       direction - the direction of translation. This parameter decides, in
                   which order the cell names are stored in the list 'cells'.
*/
bool readCellListFromXML(const std::string& fileName, CellListType& cells, const TransDir direction = td_en_de);

} //namespace

#endif // TRANSLATORXML_H
