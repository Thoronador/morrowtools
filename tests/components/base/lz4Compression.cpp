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
#include "../../../base/lz4Compression.hpp"
#include "../../../base/DirectoryFunctions.hpp"

TEST_CASE("lz4Compression")
{
  const std::string delim = std::string(1, MWTP::pathDelimiter);
  const std::string currentFile = std::string(__FILE__);
  const std::string test_directory = currentFile.substr(0, currentFile.size() - std::string("lz4Compression.cpp").size())
          .append("test_files").append(delim);

  SECTION("lz4Decompress")
  {
    SECTION("normal data")
    {
      char compressed[475];

      {
        std::ifstream stream(test_directory + "lorem_ipsum.lz4");
        stream.read(compressed, 475);
        REQUIRE( stream.good() );
        stream.close();
      }

      char uncompressed[535];

      {
        std::ifstream stream(test_directory + "lorem_ipsum.txt");
        stream.read(uncompressed, 535);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[535];
      std::memset(decompression, 0, 535);
      // Decompression must succeed.
      REQUIRE( MWTP::lz4Decompress(reinterpret_cast<uint8_t*>(compressed), 475,
                                   reinterpret_cast<uint8_t*>(decompression), 535) );
      // Decompressed data shall match content of lorem_ipsum.txt.
      REQUIRE( std::memcmp(uncompressed, decompression, 535) == 0 );
    }

    SECTION("trivially invalid input data")
    {
      uint8_t a[10];
      uint8_t b[10];
      // Giving nullptr or size zero should never succeed.
      REQUIRE_FALSE( MWTP::lz4Decompress(nullptr, 10, b, 10) );
      REQUIRE_FALSE( MWTP::lz4Decompress(a, 0, b, 10) );
      REQUIRE_FALSE( MWTP::lz4Decompress(a, 10, nullptr, 10) );
      REQUIRE_FALSE( MWTP::lz4Decompress(a, 10, b, 0) );
    }

    SECTION("input data too short")
    {
      char compressed[466];
      compressed[465] = '\xFF';
      {
        std::ifstream stream(test_directory + "lorem_ipsum.lz4");
        stream.read(compressed, 465);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[535];
      std::memset(decompression, 0, 535);
      // Decompression should fail, because it is 465 bytes instead of the real 475.
      REQUIRE_FALSE( MWTP::lz4Decompress(reinterpret_cast<uint8_t*>(compressed), 465,
                                         reinterpret_cast<uint8_t*>(decompression), 535) );
    }

    SECTION("input data too long")
    {
      char compressed[480];
      std::memset(compressed, 0, 480);
      {
        std::ifstream stream(test_directory + "lorem_ipsum.lz4");
        stream.read(compressed, 475);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[535];
      std::memset(decompression, 0, 535);
      // Decompression should fail, because it's 480 bytes instead of the real 475.
      REQUIRE_FALSE( MWTP::lz4Decompress(reinterpret_cast<uint8_t*>(compressed), 480,
                                         reinterpret_cast<uint8_t*>(decompression), 535) );
    }

    SECTION("output buffer too short")
    {
      char compressed[475];

      {
        std::ifstream stream(test_directory + "lorem_ipsum.lz4");
        stream.read(compressed, 475);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[530];

      std::memset(decompression, 0, 530);
      // Decompression should fail, because output is 530 bytes but requires 535 bytes.
      REQUIRE_FALSE( MWTP::lz4Decompress(reinterpret_cast<uint8_t*>(compressed), 475,
                                         reinterpret_cast<uint8_t*>(decompression), 530) );
    }

    SECTION("output buffer too long")
    {
      char compressed[475];

      {
        std::ifstream stream(test_directory + "lorem_ipsum.lz4");
        stream.read(compressed, 475);
        REQUIRE( stream.good() );
        stream.close();
      }

      char decompression[555];

      std::memset(decompression, 0, 555);
      // Decompression should fail, because output is 555 bytes but requires only 535 bytes.
      REQUIRE_FALSE( MWTP::lz4Decompress(reinterpret_cast<uint8_t*>(compressed), 475,
                                         reinterpret_cast<uint8_t*>(decompression), 555) );
    }
  }

  SECTION("lz4Version")
  {
    REQUIRE_FALSE( MWTP::lz4Version().empty() );
  }
}
