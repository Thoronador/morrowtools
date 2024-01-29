/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

#include "../../../lib/locate_catch.hpp"
#include "../../../../apps/mw/name_generator/NameGenFunctions.hpp"
#include "../../../../lib/mw/Races.hpp"

TEST_CASE("MW name generator-related functions")
{
  using namespace MWTP;

  SECTION("removeNonPlayableRaces")
  {
    RaceRecord playable;
    playable.recordID = "PlayableRace";
    playable.Name = "foo";
    playable.Flags |= 0x00000001;
    REQUIRE( playable.isPlayable() );

    RaceRecord non_playable;
    non_playable.recordID = "NpcRace";
    non_playable.Name = "bar";
    REQUIRE_FALSE( non_playable.isPlayable() );

    Races::get().addRecord(playable);
    Races::get().addRecord(non_playable);

    REQUIRE( Races::get().hasRecord("PlayableRace") );
    REQUIRE( Races::get().hasRecord("NpcRace") );

    removeNonPlayableRaces();

    REQUIRE( Races::get().hasRecord("PlayableRace") );
    REQUIRE_FALSE( Races::get().hasRecord("NpcRace") );
  }
}
