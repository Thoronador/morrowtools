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

#include "KhajiitPurge.hpp"

namespace MWTP
{

std::vector<std::string> KhajiitPurge::purge()
{
  return {
    "cattle_kha_f01",
    "cattle_kha_f02",
    "cattle_kha_m01",
    "m'aiq",
    "quarra vampire 3",
    "thief dead 00"
  };
}

} // namespace
