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

#include "OrcNamesPurge.hpp"

namespace MWTP
{

std::vector<std::string> OrcNamesPurge::purge()
{
  return {
    "aundae vampire 2",
    "bm_werewolf_wild02",
    "cattle_orc_f01",
    "cattle_orc_m01",
    "chaplain ogrul",
    "dead orc male_02",
    "dreggs",
    "fore paw",
    "gakkenfeld",
    "gnaw tooth",
    "Koffutto Gilgar",
    "sloomalah",
    "umbra"
  };
}

} // namespace
