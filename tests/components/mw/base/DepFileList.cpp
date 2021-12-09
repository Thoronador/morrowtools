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
#include "../../../../mw/base/DepFiles.hpp"

TEST_CASE("MWTP::DepFileList")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    DepFileList list;

    REQUIRE( list.getSize() == 0 );
    REQUIRE( list.isEmpty() );
  }

  SECTION("equality operator")
  {
    DepFileList a;
    DepFileList b;

    SECTION("equal")
    {
      SECTION("empty lists")
      {
        REQUIRE( a == b );
        REQUIRE( b == a );
      }

      SECTION("non-empty lists")
      {
        a.push_back(DepFile("abc.esm"));
        a.push_back(DepFile("blah.esp"));

        b.push_back(DepFile("abc.esm"));
        b.push_back(DepFile("blah.esp"));

        REQUIRE( a == b );
        REQUIRE( b == a );
      }
    }

    SECTION("unequal")
    {
      SECTION("different number of files")
      {
        a.push_back(DepFile("abc.esm"));
        a.push_back(DepFile("blah.esp"));

        b.push_back(DepFile("abc.esm"));

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("same number of files with different context")
      {
        a.push_back(DepFile("abc.esm"));
        a.push_back(DepFile("blah.esp"));

        b.push_back(DepFile("abc.esm"));
        b.push_back(DepFile("blub.esm"));

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("isEmpty")
  {
    DepFileList dfl;

    REQUIRE( dfl.isEmpty() );
    dfl.push_back(DepFile());
    REQUIRE_FALSE( dfl.isEmpty() );
  }

  SECTION("getSize")
  {
    DepFileList dfl;

    REQUIRE( dfl.getSize() == 0 );
    dfl.push_back(DepFile());
    REQUIRE( dfl.getSize() == 1 );
    dfl.push_back(DepFile());
    REQUIRE( dfl.getSize() == 2 );
    dfl.push_back(DepFile());
    REQUIRE( dfl.getSize() == 3 );
  }

  SECTION("at")
  {
    DepFileList dfl;

    SECTION("empty")
    {
      REQUIRE_THROWS( dfl.at(0) );
    }

    SECTION("non-empty")
    {
      DepFile one("one.esm");
      DepFile two("two.esm");

      dfl.push_back(one);
      dfl.push_back(two);

      REQUIRE( dfl.at(0).name == "one.esm" );
      REQUIRE( dfl.at(1).name == "two.esm" );

      REQUIRE_THROWS( dfl.at(2) );
    }
  }

  SECTION("push_front")
  {
    DepFileList dfl;


    DepFile one("one.esm");
    DepFile two("two.esm");

    dfl.push_front(one);
    dfl.push_front(two);

    REQUIRE( dfl.at(0).name == two.name );
    REQUIRE( dfl.at(1).name == one.name );

    DepFile three("t33.esm");
    dfl.push_front(three);

    REQUIRE( dfl.at(0).name == three.name );
    REQUIRE( dfl.at(1).name == two.name );
    REQUIRE( dfl.at(2).name == one.name );
  }

  SECTION("hasDepFile")
  {
    DepFileList dfl;

    SECTION("empty")
    {
      REQUIRE_FALSE( dfl.hasDepFile("foo.esm") );
    }

    SECTION("non-empty")
    {
      dfl.push_back(DepFile("foo.esm"));

      REQUIRE( dfl.hasDepFile("foo.esm") );
      REQUIRE_FALSE( dfl.hasDepFile("bar.esm") );

      dfl.push_back(DepFile("bar.esm"));
      REQUIRE( dfl.hasDepFile("bar.esm") );
    }
  }

  SECTION("clear")
  {
    DepFileList dfl;

    dfl.push_back(DepFile("foo.esm"));
    dfl.push_back(DepFile("bar.esm"));
    REQUIRE( dfl.getSize() == 2 );
    dfl.clear();
    REQUIRE( dfl.getSize() == 0 );
  }

  SECTION("removeEntry")
  {
    DepFileList dfl;

    dfl.push_back(DepFile("foo.esm"));
    dfl.push_back(DepFile("bar.esm"));

    REQUIRE_FALSE( dfl.removeEntry(3) );
    REQUIRE_FALSE( dfl.removeEntry(2) );
    REQUIRE( dfl.removeEntry(1) );
    REQUIRE( dfl.removeEntry(0) );
    REQUIRE_FALSE( dfl.removeEntry(0) );
  }

  SECTION("removeDuplicates")
  {
    DepFileList dfl;

    SECTION("empty list")
    {
      REQUIRE( dfl.removeDuplicates() == 0 );
    }

    SECTION("list without duplicates")
    {
      dfl.push_back(DepFile("foo.esm"));
      dfl.push_back(DepFile("bar.esm"));
      dfl.push_back(DepFile("baz.esp"));

      REQUIRE( dfl.removeDuplicates() == 0 );
    }

    SECTION("list with one duplicate")
    {
      dfl.push_back(DepFile("foo.esm"));
      dfl.push_back(DepFile("foo.esm"));
      dfl.push_back(DepFile("bar.esm"));
      dfl.push_back(DepFile("baz.esp"));

      REQUIRE( dfl.removeDuplicates() == 1 );
      REQUIRE( dfl.at(0).name == "foo.esm" );
      REQUIRE( dfl.at(1).name == "bar.esm" );
      REQUIRE( dfl.at(2).name == "baz.esp" );
    }

    SECTION("list with one duplicate, but it is not sorted")
    {
      dfl.push_back(DepFile("foo.esm"));
      dfl.push_back(DepFile("bar.esm"));
      dfl.push_back(DepFile("foo.esm"));
      dfl.push_back(DepFile("baz.esp"));
      // No duplicates are removed, if list is not sorted.
      REQUIRE( dfl.removeDuplicates() == 0 );
    }

    SECTION("list with multiple duplicates")
    {
      dfl.push_back(DepFile("foo.esm"));
      dfl.push_back(DepFile("foo.esm"));
      dfl.push_back(DepFile("bar.esm"));
      dfl.push_back(DepFile("baz.esp"));
      dfl.push_back(DepFile("baz.esp"));
      dfl.push_back(DepFile("baz.esp"));
      dfl.push_back(DepFile("baz.esp"));

      REQUIRE( dfl.removeDuplicates() == 4 );
      REQUIRE( dfl.at(0).name == "foo.esm" );
      REQUIRE( dfl.at(1).name == "bar.esm" );
      REQUIRE( dfl.at(2).name == "baz.esp" );
    }
  }

  SECTION("writeDeps")
  {
    DepFileList dfl;

    dfl.push_back(DepFile("foo.esm"));
    dfl.push_back(DepFile("bar.esm"));
    // Cannot really test this, because it only writes to standard output.
    dfl.writeDeps();
  }

  SECTION("sort")
  {
    DepFileList dfl;

    SECTION("empty list")
    {
      dfl.sort();

      REQUIRE( dfl.isEmpty() );
    }

    SECTION("already sorted")
    {
      DepFile one("foo.esm");
      one.modified = static_cast<std::time_t>(123);
      DepFile two("bar.esm");
      two.modified = static_cast<std::time_t>(12345);
      DepFile three("baz.esp");

      dfl.push_back(one);
      dfl.push_back(two);
      dfl.push_back(three);

      dfl.sort();

      REQUIRE( dfl.at(0) == one );
      REQUIRE( dfl.at(1) == two );
      REQUIRE( dfl.at(2) == three );
    }

    SECTION("ESM file before plugin file")
    {
      DepFile plugin("bar.esp");
      DepFile esm("foo.esm");

      dfl.push_back(plugin);
      dfl.push_back(esm);

      dfl.sort();

      REQUIRE( dfl.at(0) == esm );
      REQUIRE( dfl.at(1) == plugin );
    }

    SECTION("modification time is tie breaker for same types")
    {
      DepFile pluginEarly("baz.esp");
      pluginEarly.modified = static_cast<std::time_t>(123);

      DepFile pluginLate("bar.esp");
      pluginLate.modified = static_cast<std::time_t>(1234);

      DepFile esmEarly("foo.esm");
      esmEarly.modified = static_cast<std::time_t>(12);

      DepFile esmLate("abc.esm");
      esmLate.modified = static_cast<std::time_t>(12345);

      dfl.push_back(pluginLate);
      dfl.push_back(pluginEarly);
      dfl.push_back(esmEarly);
      dfl.push_back(esmLate);

      dfl.sort();

      REQUIRE( dfl.at(0) == esmEarly );
      REQUIRE( dfl.at(1) == esmLate );
      REQUIRE( dfl.at(2) == pluginEarly );
      REQUIRE( dfl.at(3) == pluginLate );
    }
  }
}
