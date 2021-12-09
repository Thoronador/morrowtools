/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#include <catch.hpp>
#include "../../../../sr/base/DependencySolver.hpp"

TEST_CASE("DependencyElement")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    SECTION("default")
    {
      DependencyElement element;

      REQUIRE( element.name.empty() );
      const auto& tes4 = element.header;
      REQUIRE( tes4.version == 0.0f );
      REQUIRE( tes4.numRecordsAndGroups == 0 );
      REQUIRE( tes4.nextObjectID == 0 );
      REQUIRE( tes4.authorName.empty() );
      REQUIRE( tes4.summary.empty() );
      REQUIRE( tes4.dependencies.empty() );
      REQUIRE( tes4.unknownONAM.empty() );
      REQUIRE( tes4.unknownIntValue == 0 );
      REQUIRE_FALSE( tes4.unknownINCC.has_value() );
    }

    SECTION("with file name")
    {
      DependencyElement element("foo.esm");

      REQUIRE( element.name == "foo.esm" );
      const auto& tes4 = element.header;
      REQUIRE( tes4.version == 0.0f );
      REQUIRE( tes4.numRecordsAndGroups == 0 );
      REQUIRE( tes4.nextObjectID == 0 );
      REQUIRE( tes4.authorName.empty() );
      REQUIRE( tes4.summary.empty() );
      REQUIRE( tes4.dependencies.empty() );
      REQUIRE( tes4.unknownONAM.empty() );
      REQUIRE( tes4.unknownIntValue == 0 );
      REQUIRE_FALSE( tes4.unknownINCC.has_value() );
    }

    SECTION("with file name and header")
    {
      Tes4HeaderRecord header;
      header.version = 0.94f;
      header.numRecordsAndGroups = 42;
      header.nextObjectID = 1;
      header.authorName = "Foo and the bar";
      header.summary = "A foo walks into a bar ...";

      DependencyElement element("foo.esm", header);

      REQUIRE( element.name == "foo.esm" );
      REQUIRE( element.header.equals(header) );
    }
  }

  SECTION("operator less")
  {
    SECTION("less")
    {
      REQUIRE( DependencyElement("bar.esm") < DependencyElement("foo.esm") );
      REQUIRE( DependencyElement("bar.esm") < DependencyElement("FOO.ESM") );
      REQUIRE( DependencyElement("BAR.ESM") < DependencyElement("foo.esm") );
      REQUIRE( DependencyElement("BAR.ESM") < DependencyElement("FOO.ESM") );
    }

    SECTION("equal")
    {
      REQUIRE_FALSE( DependencyElement("bar.esm") < DependencyElement("bar.esm") );
      REQUIRE_FALSE( DependencyElement("bar.esm") < DependencyElement("BAR.ESM") );
      REQUIRE_FALSE( DependencyElement("BAR.ESM") < DependencyElement("bar.esm") );
      REQUIRE_FALSE( DependencyElement("BAR.ESM") < DependencyElement("BAR.ESM") );
    }

    SECTION("greater")
    {
      REQUIRE_FALSE( DependencyElement("foo.esm") < DependencyElement("bar.esm") );
      REQUIRE_FALSE( DependencyElement("FOO.ESM") < DependencyElement("bar.esm") );
      REQUIRE_FALSE( DependencyElement("foo.esm") < DependencyElement("BAR.ESM") );
      REQUIRE_FALSE( DependencyElement("FOO.ESM") < DependencyElement("BAR.ESM") );
    }
  }
}
