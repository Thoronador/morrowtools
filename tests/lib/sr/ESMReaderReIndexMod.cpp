/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2022  Dirk Stolle

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
#include <filesystem>
#include <string_view>
#include "../../../lib/sr/ESMReaderReIndex.hpp"
#include "../../../lib/sr/FormIDFunctions.hpp"
#include "TestReaderReIndexMod.hpp"

std::vector<std::string> getTestLoadOrder()
{
  return std::vector<std::string>{
    "foo.esm",
    "Bar.esm",
    "baz.esm",
    "quux.esp"
  };
}

TEST_CASE("SRTP::ESMReaderReIndexMod")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    // Tests use derived class TestReaderReIndexMod, because ESMReaderReIndexMod
    // is an abstract class and cannot be instantiated.
    TestReaderReIndexMod reader(getTestLoadOrder());
    REQUIRE( reader.indexMapsNeedsUpdate() );
  }

  SECTION("indexMapsNeedsUpdate")
  {
    const auto lo = getTestLoadOrder();
    TestReaderReIndexMod reader(lo);
    // Initially, an update is needed.
    REQUIRE( reader.indexMapsNeedsUpdate() );
    // But after a successful update ...
    REQUIRE( reader.updateIndexMap_public(lo[0]) );
    // ... no update is needed.
    REQUIRE_FALSE( reader.indexMapsNeedsUpdate() );
  }

  SECTION("requestIndexMapUpdate")
  {
    const auto lo = getTestLoadOrder();
    TestReaderReIndexMod reader(lo);
    REQUIRE( reader.updateIndexMap_public(lo[0]) );
    // No update is needed at the moment.
    REQUIRE_FALSE( reader.indexMapsNeedsUpdate() );
    // And requestIndexMapUpdate() changes that.
    reader.requestIndexMapUpdate(lo[1]);
    REQUIRE( reader.indexMapsNeedsUpdate() );
  }

  SECTION("updateIndexMap")
  {
    SECTION("empty load order")
    {
      TestReaderReIndexMod reader({});
      REQUIRE_FALSE( reader.updateIndexMap_public("foo.esm") );
    }

    SECTION("load order with more than 255 entries")
    {
      std::vector<std::string> loadOrder { "foo.esm" };
      for (unsigned int i = 1; i < 260; ++i)
      {
        loadOrder.push_back("file_" + std::to_string(i) + ".esm");
      }

      TestReaderReIndexMod reader(loadOrder);
      REQUIRE_FALSE( reader.updateIndexMap_public("foo.esm") );
    }

    SECTION("ESM with more than 255 dependencies (should not be possible)")
    {
      TestReaderReIndexMod reader(getTestLoadOrder());
      for (unsigned int i = 1; i < 260; ++i)
      {
        Tes4HeaderRecord::MasterFile file;
        file.fileName = "file_" + std::to_string(i) + ".esm";
        file.data = 0x0123456789ABCDEF;
        reader.addDependencyToHeaderData(file);
      }

      REQUIRE_FALSE( reader.updateIndexMap_public("foo.esm") );
    }

    SECTION("load order does not contain a dependency file")
    {
      TestReaderReIndexMod reader(getTestLoadOrder());

      Tes4HeaderRecord::MasterFile file;
      file.fileName = "file_not_in_load_order.esm";
      file.data = 0x0123456789ABCDEF;
      reader.addDependencyToHeaderData(file);

      REQUIRE_FALSE( reader.updateIndexMap_public("foo.esm") );
    }

    SECTION("load order does not contain a current ESM file")
    {
      TestReaderReIndexMod reader(getTestLoadOrder());
      REQUIRE_FALSE( reader.updateIndexMap_public("current_mod_not_in_load_order.esm") );
    }

    SECTION("update with no dependencies in current ESM file")
    {
      TestReaderReIndexMod reader(getTestLoadOrder());
      REQUIRE( reader.updateIndexMap_public("quux.esp") );
    }

    SECTION("update with some dependencies in current ESM file")
    {
      TestReaderReIndexMod reader(getTestLoadOrder());

      // Add dependency (usually gets loaded during call to readESM(), but we
      // do not want to create a whole ESM file here on the fly).
      {
        Tes4HeaderRecord::MasterFile file;
        file.fileName = "foo.esm";
        file.data = 0x0123456789ABCDEF;
        reader.addDependencyToHeaderData(file);
        file.fileName = "baz.esm";
        reader.addDependencyToHeaderData(file);
      }
      REQUIRE( reader.updateIndexMap_public("quux.esp") );
    }
  }

  SECTION("reIndex")
  {
    SECTION("mod index not found in map")
    {
      TestReaderReIndexMod reader({});
      uint32_t id = 0x42ABCDEF;
      REQUIRE_FALSE( reader.reIndex_public(id) );
    }

    SECTION("reIndex with no dependencies in current ESM file")
    {
      TestReaderReIndexMod reader(getTestLoadOrder());
      REQUIRE( reader.updateIndexMap_public("quux.esp") );

      uint32_t id = 0x00ABCDEF;
      REQUIRE( reader.reIndex_public(id) );
      // "quux.esp" is fourth (i. e. index 3) in global load order, so the new
      // mod index should be three.
      REQUIRE( id == 0x03ABCDEF );
    }

    SECTION("reIndex with one dependency in current ESM file")
    {
      TestReaderReIndexMod reader(getTestLoadOrder());

      // Add dependency (usually gets loaded during call to readESM(), but we
      // do not want to create a whole ESM file here on the fly).
      {
        Tes4HeaderRecord::MasterFile file;
        file.fileName = "baz.esm";
        file.data = 0x0123456789ABCDEF;
        reader.addDependencyToHeaderData(file);
      }
      // Update index map for current file.
      REQUIRE( reader.updateIndexMap_public("quux.esp") );

      // Mod index zero means first dependency, i. e. baz.esm.
      uint32_t id = 0x00ABCDEF;
      REQUIRE( reader.reIndex_public(id) );
      // "baz.esm" is third (i. e. index 2) in global load order, so the new
      // mod index should be two.
      REQUIRE( id == 0x02ABCDEF );

      // Mod index one means quux.esp.
      id = 0x01ABCDEF;
      REQUIRE( reader.reIndex_public(id) );
      // "quux.esp" is fourth (i. e. index 3) in global load order, so the new
      // mod index should be three.
      REQUIRE( id == 0x03ABCDEF );

      // Mod index two is out of range, because that would required three files,
      // but there are only two in the current mod: its dependency baz.esm and
      // the mod itself, quux.esp.
      id = 0x02ABCDEF;
      REQUIRE_FALSE( reader.reIndex_public(id) );
      // Mod index will not be changed.
      REQUIRE( id == 0x02ABCDEF );
    }

    SECTION("reIndex with several dependencies in current ESM file")
    {
      const std::vector<std::string> loadOrder = {
        "null.esm",
        "uno.esm",
        "two.esm",
        "drei.esm",
        "quattro.esm",
        "cinq.esm",
        "seis.esm",
      };
      TestReaderReIndexMod reader(loadOrder);

      // Add dependencies (usually gets loaded during call to readESM(), but we
      // do not want to create a whole ESM file here on the fly).
      {
        Tes4HeaderRecord::MasterFile file;
        file.fileName = "null.esm";
        file.data = 0x0123456789ABCDEF;
        reader.addDependencyToHeaderData(file);
        file.fileName = "two.esm";
        reader.addDependencyToHeaderData(file);
        file.fileName = "quattro.esm";
        reader.addDependencyToHeaderData(file);
      }
      // Update index map for current file.
      REQUIRE( reader.updateIndexMap_public("cinq.esm") );

      // Mod index zero means first dependency, i. e. null.esm.
      uint32_t id = 0x00ABCDEF;
      REQUIRE( reader.reIndex_public(id) );
      // "null.esm" is first (i. e. index 0) in global load order, so the new
      // mod index should be zero.
      REQUIRE( id == 0x00ABCDEF );

      // Mod index one means two.esm in local dependency list, but it's third
      // (index two) in global load order, so the new mod index should be two.
      id = 0x01ABCDEF;
      REQUIRE( reader.reIndex_public(id) );
      REQUIRE( id == 0x02ABCDEF );

      // Mod index two means quattro.esm in local dependency list, but it's
      // fifth (index 4) in global load order, so the new mod index should be
      // four.
      id = 0x02ABCDEF;
      REQUIRE( reader.reIndex_public(id) );
      REQUIRE( id == 0x04ABCDEF );

      // Mod index three means cinq.esm in local dependency list, but it's
      // sixth (index 5) in global load order, so the new mod index should be
      // five.
      id = 0x03ABCDEF;
      REQUIRE( reader.reIndex_public(id) );
      REQUIRE( id == 0x05ABCDEF );

      // Mod index four is out of range, because that would required five files
      // in the mod-local dependency list, but there are only four in the
      // current mod: its three dependencies null.esm, two.esm, quattro.esm,
      // and the mod itself, cinq.esm.
      id = 0x04ABCDEF;
      REQUIRE_FALSE( reader.reIndex_public(id) );
      // Mod index will not be changed.
      REQUIRE( id == 0x04ABCDEF );
    }

    SECTION("reIndex file where global load order equals local dependency order")
    {
      const auto loadOrder = getTestLoadOrder();
      TestReaderReIndexMod reader(loadOrder);

      // Add dependencies (usually gets loaded during call to readESM(), but we
      // do not want to create a whole ESM file here on the fly).
      {
        Tes4HeaderRecord::MasterFile file;
        file.data = 0x0123456789ABCDEF;
        // Add all files except the last as dependency.
        // Last file will be the "current mod".
        for (unsigned int i = 0; i < loadOrder.size() - 1; ++i)
        {
          file.fileName = loadOrder[i];
          reader.addDependencyToHeaderData(file);
        }
      }

      // Update index map for current file.
      REQUIRE( reader.updateIndexMap_public(loadOrder.back()) );

      for (unsigned int idx = 0; idx < loadOrder.size(); ++ idx)
      {
        // Set mod index of id to current idx value.
        uint32_t id = ((0x000000FF & idx) << 24) | 0x00ABCDEF;
        // Double check, just to be safe.
        REQUIRE( extractModIndex(id) == idx );
        // "Re"-index form id.
        REQUIRE( reader.reIndex_public(id) );
        // Mod index is still the same, it did not change, because the global
        // and local load order are the same.
        REQUIRE( extractModIndex(id) == idx );
      }

      // Test with mod index out of range.
      uint32_t id = ((0x000000FF & loadOrder.size()) << 24) | 0x00ABCDEF;
      // Double check, just to be safe.
      REQUIRE( extractModIndex(id) == loadOrder.size() );
      // Re-indexing fails.
      REQUIRE_FALSE( reader.reIndex_public(id) );
    }
  }
}
