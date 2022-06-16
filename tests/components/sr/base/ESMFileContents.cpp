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

#include "../../locate_catch.hpp"
#include <iostream>
#include "../../../../sr/base/ESMFileContents.hpp"

TEST_CASE("ESMFileContents")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    ESMFileContents contents;

    REQUIRE( contents.getNumberOfGroups() == 0 );
    REQUIRE( contents.m_Groups.empty() );
  }

  SECTION("addNewGroup / getNumberOfGroups")
  {
    ESMFileContents contents;

    REQUIRE( contents.getNumberOfGroups() == 0 );

    contents.addNewGroup();
    REQUIRE( contents.getNumberOfGroups() == 1 );

    contents.addNewGroup();
    auto& g = contents.addNewGroup();
    REQUIRE( contents.getNumberOfGroups() == 3 );

    // Subgroups do not count.
    g.addSubGroup(GroupData());
    REQUIRE( contents.getNumberOfGroups() == 3 );
  }

  SECTION("removeContents")
  {
    ESMFileContents contents;

    contents.addNewGroup();
    contents.addNewGroup();
    contents.addNewGroup();
    contents.addNewGroup();

    REQUIRE_FALSE( contents.getNumberOfGroups() == 0 );

    contents.removeContents();
    REQUIRE( contents.getNumberOfGroups() == 0 );
  }

  SECTION("purgeEmptyGroups")
  {
    ESMFileContents contents;

    SECTION("no content")
    {
      REQUIRE( contents.purgeEmptyGroups() == 0 );
    }

    SECTION("two empty groups")
    {
      contents.addNewGroup();
      contents.addNewGroup();

      REQUIRE( contents.purgeEmptyGroups() == 2 );
    }

    SECTION("two empty groups with empty sub group")
    {
      contents.addNewGroup().addSubGroup(GroupData());
      contents.addNewGroup();

      REQUIRE( contents.purgeEmptyGroups() == 3 );
    }
  }

  SECTION("traverseGroups")
  {
    ESMFileContents contents;

    SECTION("function is nullptr")
    {
      contents.traverseGroups(nullptr);
    }

    SECTION("simple traversal")
    {
      contents.addNewGroup();
      contents.addNewGroup().addSubGroup(GroupData());

      auto fun = [](const Group& current, const Group* parent) {
          std::cout << "This is fun.\n";
          return true;
      };

      contents.traverseGroups(fun);
    }

    SECTION("function failure on sub group")
    {
      contents.addNewGroup().addSubGroup(GroupData());
      contents.addNewGroup();

      auto fun = [](const Group& current, const Group* parent) {
          return parent == nullptr;
      };

      contents.traverseGroups(fun);
    }

    SECTION("function failure on top-level group")
    {
      contents.addNewGroup().addSubGroup(GroupData());
      contents.addNewGroup();

      auto fun = [](const Group& current, const Group* parent) {
          return parent != nullptr;
      };

      contents.traverseGroups(fun);
    }
  }

  SECTION("determineLatestGroup")
  {
    ESMFileContents contents;

    SECTION("empty")
    {
      REQUIRE( contents.determineLatestGroup(1) == nullptr );
      REQUIRE( contents.determineLatestGroup(2) == nullptr );
      REQUIRE( contents.determineLatestGroup(3) == nullptr );
    }

    SECTION("level zero")
    {
      contents.addNewGroup();
      REQUIRE( contents.determineLatestGroup(0) == nullptr );
    }

    SECTION("level above zero")
    {
      auto& group = contents.addNewGroup();
      auto& sub = group.addSubGroup(GroupData());
      // top-level group
      REQUIRE_FALSE( contents.determineLatestGroup(1) == nullptr );
      REQUIRE( contents.determineLatestGroup(1) == &group );
      // sub group
      REQUIRE_FALSE( contents.determineLatestGroup(2) == nullptr );
      REQUIRE( contents.determineLatestGroup(2) == &group );
    }
  }
}
