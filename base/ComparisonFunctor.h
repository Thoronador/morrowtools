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

#ifndef COMPARISONFUNCTOR_H
#define COMPARISONFUNCTOR_H

#include "UtilityFunctions.h"

namespace MWTP
{

struct ci_less : public std::binary_function<std::string, std::string, bool>
{
  bool
  operator()(const std::string& __x, const std::string& __y) const
  { return (lowerCaseCompare(__x, __y)<0); }
};

} //namespace

#endif // COMPARISONFUNCTOR_H
