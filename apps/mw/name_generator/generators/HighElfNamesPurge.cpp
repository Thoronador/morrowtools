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

#include "HighElfNamesPurge.hpp"

namespace MWTP
{

std::vector<std::string> HighElfNamesPurge::purge()
{
  return {
    "aundae vampire 3",
    "dead de ship capt11-12",
    "dead he male_01",
    "mehrunesworshipper_f01",
    "mehrunesworshipper_m01",
    "mehrunesworshipper_m02",
    "wizard dead 00"
  };
}

} // namespace
