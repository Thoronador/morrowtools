/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2024  Dirk Stolle

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

#include "../locate_catch.hpp"
#include "../../../lib/base/SlashFunctions.hpp"

TEST_CASE("SlashFunctions")
{
  using namespace MWTP;

  SECTION("flipBackslashes")
  {
    SECTION("empty string")
    {
      REQUIRE( flipBackslashes("") == "" );
    }

    SECTION("no backslashes - no changes")
    {
      REQUIRE( flipBackslashes("/") == "/" );
      REQUIRE( flipBackslashes("foo") == "foo" );
      REQUIRE( flipBackslashes("/foo") == "/foo" );
      REQUIRE( flipBackslashes("foo/") == "foo/" );
      REQUIRE( flipBackslashes("foo/bar") == "foo/bar" );
      REQUIRE( flipBackslashes("foo/bar/baz") == "foo/bar/baz" );
    }

    SECTION("all backslashes get flipped")
    {
      REQUIRE( flipBackslashes("\\") == "/" );
      REQUIRE( flipBackslashes("foo") == "foo" );
      REQUIRE( flipBackslashes("\\foo") == "/foo" );
      REQUIRE( flipBackslashes("foo\\") == "foo/" );
      REQUIRE( flipBackslashes("foo\\bar") == "foo/bar" );
      REQUIRE( flipBackslashes("foo\\\\bar") == "foo//bar" );
      REQUIRE( flipBackslashes("foo\\bar\\") == "foo/bar/" );
      REQUIRE( flipBackslashes("foo\\bar\\baz") == "foo/bar/baz" );
      REQUIRE( flipBackslashes("foo\\bar\\baz\\") == "foo/bar/baz/" );
      REQUIRE( flipBackslashes("\\foo\\bar\\baz\\") == "/foo/bar/baz/" );
    }
  }

  SECTION("flipForwardSlashes")
  {
    SECTION("empty string")
    {
      REQUIRE( flipForwardSlashes("") == "" );
    }

    SECTION("no forward slashes - no changes")
    {
      REQUIRE( flipForwardSlashes("\\") == "\\" );
      REQUIRE( flipForwardSlashes("foo") == "foo" );
      REQUIRE( flipForwardSlashes("\\foo") == "\\foo" );
      REQUIRE( flipForwardSlashes("foo\\") == "foo\\" );
      REQUIRE( flipForwardSlashes("foo\\bar") == "foo\\bar" );
      REQUIRE( flipForwardSlashes("foo\\bar\\baz") == "foo\\bar\\baz" );
    }

    SECTION("all forward slashes get flipped")
    {
      REQUIRE( flipForwardSlashes("/") == "\\" );
      REQUIRE( flipForwardSlashes("foo") == "foo" );
      REQUIRE( flipForwardSlashes("/foo") == "\\foo" );
      REQUIRE( flipForwardSlashes("foo/") == "foo\\" );
      REQUIRE( flipForwardSlashes("foo/bar") == "foo\\bar" );
      REQUIRE( flipForwardSlashes("foo//bar") == "foo\\\\bar" );
      REQUIRE( flipForwardSlashes("foo/bar/") == "foo\\bar\\" );
      REQUIRE( flipForwardSlashes("foo/bar/baz") == "foo\\bar\\baz" );
      REQUIRE( flipForwardSlashes("foo/bar/baz/") == "foo\\bar\\baz\\" );
      REQUIRE( flipForwardSlashes("/foo/bar/baz/") == "\\foo\\bar\\baz\\" );
    }
  }
}
