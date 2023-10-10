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

#include "../../../../lib/locate_catch.hpp"
#include "../../../../../apps/mw/name_generator/generators/Generator.hpp"
#include "../../../../../apps/mw/name_generator/generators/ArgonianNamesPurge.hpp"
#include "../../../../../apps/mw/name_generator/generators/BretonNamesPurge.hpp"
#include "../../../../../apps/mw/name_generator/generators/ImperialNamesPurge.hpp"
#include "../../../../../apps/mw/name_generator/generators/KhajiitNamesPurge.hpp"

template<typename T>
class PurgeGenerator: public T
{
  public:
    std::vector<std::string> generate([[maybe_unused]] const uint_least16_t n) override
    {
      // Return some bogus data. It's not relevant in the test. It is just here
      // to implement the missing method required by the Generator interface.
      return { "foo", "bar" };
    }
};

TEST_CASE("MWTP name_generator: NamesPurge")
{
  using namespace MWTP;

  SECTION("ArgonianNamesPurge")
  {
    PurgeGenerator<ArgonianNamesPurge> instance;
    REQUIRE_FALSE( instance.purge().empty() );
  }

  SECTION("BretonNamesPurge")
  {
    PurgeGenerator<BretonNamesPurge> instance;
    REQUIRE_FALSE( instance.purge().empty() );
  }

  SECTION("ImperialNamesPurge")
  {
    PurgeGenerator<ImperialNamesPurge> instance;
    REQUIRE_FALSE( instance.purge().empty() );
  }

  SECTION("KhajiitNamesPurge")
  {
    PurgeGenerator<KhajiitNamesPurge> instance;
    REQUIRE_FALSE( instance.purge().empty() );
  }
}
