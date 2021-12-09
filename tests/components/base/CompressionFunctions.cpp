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
#include <cstring>
#include <fstream>
#include "../../../base/CompressionFunctions.hpp"
#include "../../../base/DirectoryFunctions.hpp"

TEST_CASE("CompressionFunctions")
{
  const std::string delim = std::string(1, MWTP::pathDelimiter);
  const std::string currentFile = std::string(__FILE__);
  const std::string test_directory = currentFile.substr(0, currentFile.size() - std::string("CompressionFunctions.cpp").size())
          .append("test_files").append(delim);

  SECTION("zlibDecompress")
  {
    SECTION("normal data")
    {
      char compressed[98];

      {
        std::ifstream stream(test_directory + "compressed.z");
        stream.read(compressed, 98);
        REQUIRE( stream.good() );
        stream.close();
      }

      char uncompressed[136];

      {
        std::ifstream stream(test_directory + "uncompressed.z");
        stream.read(uncompressed, 136);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[136];
      std::memset(decompression, 0, 136);
      // Decompression must succeed.
      REQUIRE( MWTP::zlibDecompress((uint8_t*) compressed, 98, (uint8_t*) decompression, 136) );
      // Decompressed data shall match content of uncompressed.z.
      REQUIRE( std::memcmp(uncompressed, decompression, 136) == 0 );
    }

    SECTION("trivially invalid input data")
    {
      uint8_t a[10];
      uint8_t b[10];
      // Giving nullptr or size zero should never succeed.
      REQUIRE_FALSE( MWTP::zlibDecompress(nullptr, 10, b, 10) );
      REQUIRE_FALSE( MWTP::zlibDecompress(a, 0, b, 10) );
      REQUIRE_FALSE( MWTP::zlibDecompress(a, 10, nullptr, 10) );
      REQUIRE_FALSE( MWTP::zlibDecompress(a, 10, b, 0) );
    }

    SECTION("input data too short")
    {
      char compressed[98];

      {
        std::ifstream stream(test_directory + "compressed.z");
        stream.read(compressed, 98);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[136];
      std::memset(decompression, 0, 136);
      // Decompression should fail, because its 97 bytes instead of the real 98.
      REQUIRE_FALSE( MWTP::zlibDecompress((uint8_t*) compressed, 97, (uint8_t*) decompression, 136) );
    }

    SECTION("output buffer too short")
    {
      char compressed[98];

      {
        std::ifstream stream(test_directory + "compressed.z");
        stream.read(compressed, 98);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[120];
      std::memset(decompression, 0, 120);
      // Decompression should fail, because output is 120 bytes but requires 136 bytes.
      REQUIRE_FALSE( MWTP::zlibDecompress((uint8_t*) compressed, 98, (uint8_t*) decompression, 120) );
    }
  }

  SECTION("zlibVersion")
  {
    REQUIRE_FALSE( MWTP::zlibVersion().empty() );
  }
}
