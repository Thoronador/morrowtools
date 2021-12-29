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
#include <string_view>
#include "../../../base/ByteBuffer.hpp"

TEST_CASE("ByteBuffer")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ByteBuffer bb;

    REQUIRE( bb.data() == nullptr );
    REQUIRE( bb.size() == 0 );
  }

  SECTION("move constructor")
  {
    SECTION("empty buffer")
    {
      ByteBuffer a;
      ByteBuffer b(std::move(a));

      REQUIRE( b.data() == nullptr );
      REQUIRE( b.size() == 0 );
    }

    SECTION("filled buffer")
    {
      ByteBuffer a;
      a.copy_from(reinterpret_cast<const uint8_t*>("foo"), 4);

      ByteBuffer b(std::move(a));

      REQUIRE( a.data() == nullptr );
      REQUIRE( a.size() == 0 );

      REQUIRE( b.data() != nullptr );
      REQUIRE( b.size() == 4 );

      const auto view = std::string_view(reinterpret_cast<const char*>(b.data()), b.size());
      REQUIRE( view == "foo\0"sv );
    }
  }

  SECTION("copy constructor")
  {
    SECTION("empty buffer")
    {
      const ByteBuffer a;
      REQUIRE( a.data() == nullptr );
      REQUIRE( a.size() == 0 );
      ByteBuffer b(a);
      REQUIRE( b.data() == nullptr );
      REQUIRE( b.size() == 0 );
    }

    SECTION("filled buffer")
    {
      ByteBuffer a;
      a.copy_from(reinterpret_cast<const uint8_t*>("foo"), 4);

      REQUIRE( a.data() != nullptr );
      REQUIRE( a.size() == 4 );

      ByteBuffer b(a);

      REQUIRE( a.data() != nullptr );
      REQUIRE( a.size() == 4 );

      const auto a_view = std::string_view(reinterpret_cast<const char*>(a.data()), a.size());
      REQUIRE( a_view == "foo\0"sv );

      REQUIRE( b.data() != nullptr );
      REQUIRE( b.size() == 4 );

      const auto b_view = std::string_view(reinterpret_cast<const char*>(b.data()), b.size());
      REQUIRE( b_view == "foo\0"sv );
    }
  }

  SECTION("assignment operator")
  {
    SECTION("empty buffer")
    {
      const ByteBuffer a;
      REQUIRE( a.data() == nullptr );
      REQUIRE( a.size() == 0 );
      ByteBuffer b = a;
      REQUIRE( b.data() == nullptr );
      REQUIRE( b.size() == 0 );
    }

    SECTION("filled buffer")
    {
      ByteBuffer a;
      a.copy_from(reinterpret_cast<const uint8_t*>("foo"), 4);

      REQUIRE( a.data() != nullptr );
      REQUIRE( a.size() == 4 );

      ByteBuffer b = a;

      REQUIRE( a.data() != nullptr );
      REQUIRE( a.size() == 4 );

      const auto a_view = std::string_view(reinterpret_cast<const char*>(a.data()), a.size());
      REQUIRE( a_view == "foo\0"sv );

      REQUIRE( b.data() != nullptr );
      REQUIRE( b.size() == 4 );

      const auto b_view = std::string_view(reinterpret_cast<const char*>(b.data()), b.size());
      REQUIRE( b_view == "foo\0"sv );
    }
  }

  SECTION("copy_from")
  {
    SECTION("buffer pointer is null and size is zero")
    {
      ByteBuffer bb;
      bb.copy_from(nullptr, 0);
      REQUIRE( bb.data() == nullptr );
      REQUIRE( bb.size() == 0 );
    }

    SECTION("buffer pointer is null, but size is non-zero")
    {
      ByteBuffer bb;
      bb.copy_from(nullptr, 3);
      REQUIRE( bb.data() == nullptr );
      REQUIRE( bb.size() == 0 );
    }

    SECTION("buffer pointer is non-null, but size is zero")
    {
      ByteBuffer bb;
      bb.copy_from(reinterpret_cast<const uint8_t*>("foo"), 0);
      REQUIRE( bb.data() == nullptr );
      REQUIRE( bb.size() == 0 );
    }

    SECTION("non-null buffer and non-zero size")
    {
      const auto data = "\0foo\0bar\x01quux\0baz"sv;
      REQUIRE( data.data() != nullptr );
      REQUIRE( data.size() == 17 );
      ByteBuffer bb;
      bb.copy_from(reinterpret_cast<const uint8_t*>(data.data()), data.size());

      REQUIRE( bb.data() != nullptr );
      REQUIRE( bb.size() == 17 );

      const auto view = std::string_view(reinterpret_cast<const char*>(bb.data()), bb.size());
      REQUIRE( view == data );
    }
  }

  SECTION("reset")
  {
    SECTION("empty buffer")
    {
      ByteBuffer bb;
      bb.reset();
      REQUIRE( bb.data() == nullptr );
      REQUIRE( bb.size() == 0 );
    }

    SECTION("filled buffer")
    {
      ByteBuffer bb;
      bb.copy_from(reinterpret_cast<const uint8_t*>("foo"), 4);

      REQUIRE( bb.data() != nullptr );
      REQUIRE( bb.size() == 4 );

      bb.reset();

      REQUIRE( bb.data() == nullptr );
      REQUIRE( bb.size() == 0 );
    }
  }
}
