/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2021  Thoronador

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

#ifndef MWTP_COMPARISONFUNCTOR_HPP
#define MWTP_COMPARISONFUNCTOR_HPP

#include "UtilityFunctions.hpp"
#include <functional>

namespace MWTP
{

struct ci_less : public std::function<bool(const std::string&, const std::string&)>
{
  bool
  operator()(const std::string& __x, const std::string& __y) const
  { return (lowerCaseCompare(__x, __y)<0); }
};

} // namespace

#endif // COMPARISONFUNCTOR_HPP
