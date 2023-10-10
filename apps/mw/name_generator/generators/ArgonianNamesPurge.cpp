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

#include "ArgonianNamesPurge.hpp"

namespace MWTP
{

std::vector<std::string> ArgonianNamesPurge::purge()
{
  return {
    "BM_werewolf_maze1",
    "BM_werewolf_skaal1A3",
    "BM_werewolf_skaal1B3",
    "BM_werewolf_skaal1C3",
    "BM_werewolf_skaal1D3",
    "BM_werewolf_skaal1E3",
    "BM_werewolf_skaal1F3",
    "BM_werewolf_skaal1G3",
    "BM_werewolf_skaal2a",
    "BM_werewolf_wildhunt",
    "BM_werewolf_wildhunt2",
    "BM_werewolf_wildhunt3",
    "BM_werewolf_wildhunt4",
    "cattle_arg_f01",
    "cattle_arg_f02",
    "cattle_arg_m01",
    "dialog placeholder",
    "lord cluttermonkey",
    "quarra vampire 5",
    "smokeskin_killer",
    "werewolf_bearhunt",
    "werewolf_ceremony"
  };
}

} // namespace
