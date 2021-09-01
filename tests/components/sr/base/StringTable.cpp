/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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
#include <filesystem>
#include <fstream>
#include <string_view>
#include "../../../../sr/base/StringTable.hpp"

TEST_CASE("StringTable")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    StringTable table;

    REQUIRE( table.getNumberOfTableEntries() == 0 );
  }

  SECTION("addString / hasString / getString")
  {
    StringTable table;

    REQUIRE_FALSE( table.hasString(1) );
    REQUIRE_FALSE( table.hasString(42) );
    table.addString(1, "foo");
    REQUIRE( table.hasString(1) );
    REQUIRE( table.getString(1) == "foo" );
    REQUIRE_FALSE( table.hasString(42) );
    table.addString(42, "foobar");
    REQUIRE( table.hasString(1) );
    REQUIRE( table.hasString(42) );
    REQUIRE_FALSE( table.hasString(43) );
    REQUIRE( table.getString(42) == "foobar" );
  }

  SECTION("addString with ID zero does not add anything")
  {
    StringTable table;
    REQUIRE( table.getNumberOfTableEntries() == 0 );
    table.addString(0, "foobar");
    REQUIRE_FALSE( table.hasString(0) );
    REQUIRE( table.getNumberOfTableEntries() == 0 );
  }

  SECTION("getString throws when index is not present")
  {
    StringTable table;

    REQUIRE_FALSE( table.hasString(3) );
    REQUIRE_THROWS( table.getString(3) );
  }

  SECTION("deleteString")
  {
    StringTable table;

    REQUIRE_FALSE( table.hasString(1) );
    table.addString(1, "foo");
    REQUIRE( table.hasString(1) );
    REQUIRE( table.deleteString(1) );
    REQUIRE_FALSE( table.hasString(1) );
    REQUIRE_FALSE( table.deleteString(1) );

    REQUIRE_FALSE( table.deleteString(5) );
  }

  SECTION("tabulaRasa")
  {
    StringTable table;

    table.addString(1, "foo");
    table.addString(2, "bar");
    table.addString(3, "foobar");
    REQUIRE( table.hasString(1) );
    REQUIRE( table.hasString(2) );
    REQUIRE( table.hasString(3) );

    table.tabulaRasa();
    REQUIRE_FALSE( table.hasString(1) );
    REQUIRE_FALSE( table.hasString(2) );
    REQUIRE_FALSE( table.hasString(3) );
  }

  SECTION("getNumberOfTableEntries")
  {
    StringTable table;

    REQUIRE( table.getNumberOfTableEntries() == 0 );
    table.addString(11, "foo");
    REQUIRE( table.getNumberOfTableEntries() == 1 );
    table.addString(22, "bar");
    REQUIRE( table.getNumberOfTableEntries() == 2 );
    table.addString(33, "foobar");
    REQUIRE( table.getNumberOfTableEntries() == 3 );
    table.addString(44, "foobar");
    table.addString(45, "foobar");
    REQUIRE( table.getNumberOfTableEntries() == 5 );
  }

  SECTION("readTable: .strings (NUL-terminated)")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "\x03\0\0\0\x0F\0\0\0\x78\x56\x34\x12\0\0\0\0\xFE\xAF\x00\x00\x04\0\0\0\x2A\x00\x00\x00\x08\0\0\0foo\0bar\0foobar\0"sv;

    // write strings file
    {
      std::ofstream file("foobar.strings");
      file.write(data.data(), data.size());
      file.close();
    }

    StringTable table;
    REQUIRE( table.readTable("foobar.strings", StringTable::DataType::sdNULterminated) );

    REQUIRE( table.hasString(0x12345678) );
    REQUIRE( table.getString(0x12345678) == "foo" );
    REQUIRE( table.hasString(0x0000AFFE) );
    REQUIRE( table.getString(0x0000AFFE) == "bar" );
    REQUIRE( table.hasString(42) );
    REQUIRE( table.getString(42) == "foobar" );

    REQUIRE( std::filesystem::remove("foobar.strings") );
  }

  SECTION("readTable: .strings (NUL-terminated), guessing type")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "\x03\0\0\0\x0F\0\0\0\x55\x44\x33\x22\0\0\0\0\xFE\xAF\xAD\xDE\x04\0\0\0\x2A\x00\x00\x00\x08\0\0\0foo\0bar\0foobar\0"sv;

    // write strings file
    {
      std::ofstream file("foobar.guess.strings");
      file.write(data.data(), data.size());
      file.close();
    }

    StringTable table;
    REQUIRE( table.readTable("foobar.guess.strings", StringTable::DataType::sdUnknown) );

    REQUIRE( table.hasString(0x22334455) );
    REQUIRE( table.getString(0x22334455) == "foo" );
    REQUIRE( table.hasString(0xDEADAFFE) );
    REQUIRE( table.getString(0xDEADAFFE) == "bar" );
    REQUIRE( table.hasString(42) );
    REQUIRE( table.getString(42) == "foobar" );

    REQUIRE( std::filesystem::remove("foobar.guess.strings") );
  }

  SECTION("writeTable: .strings (NUL-terminated)")
  {
    // fill table and write it to file
    {
      StringTable table;
      table.addString(0x12345678, "foo");
      table.addString(0x0000AFFE, "bar");
      table.addString(42, "foobar");
      REQUIRE( table.writeTable("foobar.write.strings", StringTable::DataType::sdNULterminated) );
    }

    // read file back into a table
    {
      StringTable table;
      REQUIRE( table.readTable("foobar.write.strings", StringTable::DataType::sdNULterminated) );

      REQUIRE( table.hasString(0x12345678) );
      REQUIRE( table.getString(0x12345678) == "foo" );
      REQUIRE( table.hasString(0x0000AFFE) );
      REQUIRE( table.getString(0x0000AFFE) == "bar" );
      REQUIRE( table.hasString(42) );
      REQUIRE( table.getString(42) == "foobar" );
    }

    REQUIRE( std::filesystem::remove("foobar.write.strings") );
  }

  SECTION("readTable: .dlstrings (Pascal style + NUL), guessing type")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "\x03\0\0\0\x0F\0\0\0\xAA\x56\x34\x12\0\0\0\0\xFE\xAF\x01\xC0\x08\0\0\0\x2A\x00\x00\x00\x10\0\0\0\x04\0\0\0foo\0\x04\0\0\0bar\0\x07\0\0\0foobar\0"sv;

    // write strings file
    {
      std::ofstream file("foobar.guess.dlstrings");
      file.write(data.data(), data.size());
      file.close();
    }

    StringTable table;
    REQUIRE( table.readTable("foobar.guess.dlstrings", StringTable::DataType::sdUnknown) );

    REQUIRE( table.hasString(0x123456AA) );
    REQUIRE( table.getString(0x123456AA) == "foo" );
    REQUIRE( table.hasString(0xC001AFFE) );
    REQUIRE( table.getString(0xC001AFFE) == "bar" );
    REQUIRE( table.hasString(42) );
    REQUIRE( table.getString(42) == "foobar" );

    REQUIRE( std::filesystem::remove("foobar.guess.dlstrings") );
  }

  SECTION("readTable: .ilstrings (Pascal style + NUL)")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "\x03\0\0\0\x0F\0\0\0\x78\x56\x34\x12\0\0\0\0\xFE\xAF\x00\x00\x08\0\0\0\x2A\x00\x00\x00\x10\0\0\0\x04\0\0\0foo\0\x04\0\0\0bar\0\x07\0\0\0foobar\0"sv;

    // write strings file
    {
      std::ofstream file("foobar.ilstrings");
      file.write(data.data(), data.size());
      file.close();
    }

    StringTable table;
    REQUIRE( table.readTable("foobar.ilstrings", StringTable::DataType::sdPascalStyle) );

    REQUIRE( table.hasString(0x12345678) );
    REQUIRE( table.getString(0x12345678) == "foo" );
    REQUIRE( table.hasString(0x0000AFFE) );
    REQUIRE( table.getString(0x0000AFFE) == "bar" );
    REQUIRE( table.hasString(42) );
    REQUIRE( table.getString(42) == "foobar" );

    REQUIRE( std::filesystem::remove("foobar.ilstrings") );
  }

  SECTION("readTable: .ilstrings (Pascal style + NUL), guessing type")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "\x03\0\0\0\x0F\0\0\0\xFF\x56\x34\x12\0\0\0\0\xFE\xAF\x01\xC0\x08\0\0\0\x2A\x00\x00\x00\x10\0\0\0\x04\0\0\0foo\0\x04\0\0\0bar\0\x07\0\0\0foobar\0"sv;

    // write strings file
    {
      std::ofstream file("foobar.guess.ilstrings");
      file.write(data.data(), data.size());
      file.close();
    }

    StringTable table;
    REQUIRE( table.readTable("foobar.guess.ilstrings", StringTable::DataType::sdUnknown) );

    REQUIRE( table.hasString(0x123456FF) );
    REQUIRE( table.getString(0x123456FF) == "foo" );
    REQUIRE( table.hasString(0xC001AFFE) );
    REQUIRE( table.getString(0xC001AFFE) == "bar" );
    REQUIRE( table.hasString(42) );
    REQUIRE( table.getString(42) == "foobar" );

    REQUIRE( std::filesystem::remove("foobar.guess.ilstrings") );
  }

  SECTION("readTable: non-conforming file extension, guessing type")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "\x03\0\0\0\x0F\0\0\0\xFF\x56\x34\x12\0\0\0\0\xFE\xAF\x01\xC0\x08\0\0\0\x2A\x00\x00\x00\x10\0\0\0\x04\0\0\0foo\0\x04\0\0\0bar\0\x07\0\0\0foobar\0"sv;

    // write strings file
    {
      std::ofstream file("foobar.guess.nonconform");
      file.write(data.data(), data.size());
      file.close();
    }

    StringTable table;
    REQUIRE_FALSE( table.readTable("foobar.guess.nonconform", StringTable::DataType::sdUnknown) );

    REQUIRE( std::filesystem::remove("foobar.guess.nonconform") );
  }

  SECTION("readTable: missing file extension, guessing type")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "\x03\0\0\0\x0F\0\0\0\xFF\x56\x34\x12\0\0\0\0\xFE\xAF\x01\xC0\x08\0\0\0\x2A\x00\x00\x00\x10\0\0\0\x04\0\0\0foo\0\x04\0\0\0bar\0\x07\0\0\0foobar\0"sv;

    // write strings file
    {
      std::ofstream file("foobar_guess_no_extension");
      file.write(data.data(), data.size());
      file.close();
    }

    StringTable table;
    REQUIRE_FALSE( table.readTable("foobar_guess_no_extension", StringTable::DataType::sdUnknown) );

    REQUIRE( std::filesystem::remove("foobar_guess_no_extension") );
  }

  SECTION("writeTable: .ilstrings (Pascal style + NUL)")
  {
    // fill table and write it to file
    {
      StringTable table;
      table.addString(0x12345678, "foo");
      table.addString(0x0000AFFE, "bar");
      table.addString(42, "foobar");
      REQUIRE( table.writeTable("foobar.write.ilstrings", StringTable::DataType::sdPascalStyle) );
    }

    // read file back into a table
    {
      StringTable table;
      REQUIRE( table.readTable("foobar.write.ilstrings", StringTable::DataType::sdPascalStyle) );

      REQUIRE( table.hasString(0x12345678) );
      REQUIRE( table.getString(0x12345678) == "foo" );
      REQUIRE( table.hasString(0x0000AFFE) );
      REQUIRE( table.getString(0x0000AFFE) == "bar" );
      REQUIRE( table.hasString(42) );
      REQUIRE( table.getString(42) == "foobar" );
    }

    REQUIRE( std::filesystem::remove("foobar.write.ilstrings") );
  }
}
