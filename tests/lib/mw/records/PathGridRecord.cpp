/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2022, 2023  Dirk Stolle

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
#include <sstream>
#include <string_view>
#include "../../../../lib/mw/records/PathGridRecord.hpp"

TEST_CASE("MWTP::PathGridRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    PathGridRecord record;

    REQUIRE( record.GridX == 0 );
    REQUIRE( record.GridY == 0 );
    REQUIRE( record.Granularity == 0 );
    REQUIRE( record.CellName.empty() );
    REQUIRE( record.Points.empty() );
    REQUIRE( record.Connections.empty() );
  }

  SECTION("equals")
  {
    PathGridRecord a;
    PathGridRecord b;

    SECTION("equal")
    {
      a.GridX = 1;
      a.GridY = 2;
      a.Granularity = 512;

      b.GridX = 1;
      b.GridY = 2;
      b.Granularity = 512;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("GridX mismatch")
      {
        a.GridX = 1;
        b.GridX = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("GridY mismatch")
      {
        a.GridY = 1;
        b.GridY = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Granularity mismatch")
      {
        a.Granularity = 128;
        b.Granularity = 256;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CellName mismatch")
      {
        a.CellName = "Alice's place";
        b.CellName = "Bob's place";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Points mismatch")
      {
        a.Points.clear();
        b.Points.push_back(GridPointData());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Connections mismatch")
      {
        a.Connections.clear();
        b.Connections.push_back(GridConnection());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load record of interior cell")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      PathGridRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.GridX == 0 );
      REQUIRE( record.GridY == 0 );
      REQUIRE( record.Granularity == 1024 );
      REQUIRE( record.CellName == "Zainab Lager, Zelt des Ashkhan" );
      REQUIRE( record.Points.size() == 8 );

      REQUIRE( record.Points[0].X == -7 );
      REQUIRE( record.Points[0].Y == 297 );
      REQUIRE( record.Points[0].Z == -265 );
      REQUIRE( record.Points[0].Flags == 0x01 );
      REQUIRE( record.Points[0].ConnectionCount == 2 );
      REQUIRE( record.Points[0].Unknown == 0 );

      REQUIRE( record.Points[1].X == 125 );
      REQUIRE( record.Points[1].Y == 440 );
      REQUIRE( record.Points[1].Z == -267 );
      REQUIRE( record.Points[1].Flags == 0x01 );
      REQUIRE( record.Points[1].ConnectionCount == 2 );
      REQUIRE( record.Points[1].Unknown == 0 );

      REQUIRE( record.Points[2].X == -144 );
      REQUIRE( record.Points[2].Y == 443 );
      REQUIRE( record.Points[2].Z == -270 );
      REQUIRE( record.Points[2].Flags == 0x01 );
      REQUIRE( record.Points[2].ConnectionCount == 2 );
      REQUIRE( record.Points[2].Unknown == 0 );

      REQUIRE( record.Points[3].X == -9 );
      REQUIRE( record.Points[3].Y == 547 );
      REQUIRE( record.Points[3].Z == -270 );
      REQUIRE( record.Points[3].Flags == 0x01 );
      REQUIRE( record.Points[3].ConnectionCount == 2 );
      REQUIRE( record.Points[3].Unknown == 0 );

      REQUIRE( record.Points[4].X == -94 );
      REQUIRE( record.Points[4].Y == 372 );
      REQUIRE( record.Points[4].Z == -267 );
      REQUIRE( record.Points[4].Flags == 0x01 );
      REQUIRE( record.Points[4].ConnectionCount == 2 );
      REQUIRE( record.Points[4].Unknown == 0 );

      REQUIRE( record.Points[5].X == -90 );
      REQUIRE( record.Points[5].Y == 533 );
      REQUIRE( record.Points[5].Z == -264 );
      REQUIRE( record.Points[5].Flags == 0x01 );
      REQUIRE( record.Points[5].ConnectionCount == 2 );
      REQUIRE( record.Points[5].Unknown == 0 );

      REQUIRE( record.Points[6].X == 80 );
      REQUIRE( record.Points[6].Y == 521 );
      REQUIRE( record.Points[6].Z == -267 );
      REQUIRE( record.Points[6].Flags == 0x01 );
      REQUIRE( record.Points[6].ConnectionCount == 2 );
      REQUIRE( record.Points[6].Unknown == 0 );

      REQUIRE( record.Points[7].X == 74 );
      REQUIRE( record.Points[7].Y == 365 );
      REQUIRE( record.Points[7].Z == -267 );
      REQUIRE( record.Points[7].Flags == 0x01 );
      REQUIRE( record.Points[7].ConnectionCount == 2 );
      REQUIRE( record.Points[7].Unknown == 0 );

      REQUIRE( record.Connections.size() == 16 );

      REQUIRE( record.Connections[0].Start == 0 );
      REQUIRE( record.Connections[0].End == 4 );

      REQUIRE( record.Connections[1].Start == 0 );
      REQUIRE( record.Connections[1].End == 7 );

      REQUIRE( record.Connections[2].Start == 1 );
      REQUIRE( record.Connections[2].End == 6 );

      REQUIRE( record.Connections[3].Start == 1 );
      REQUIRE( record.Connections[3].End == 7 );

      REQUIRE( record.Connections[4].Start == 2 );
      REQUIRE( record.Connections[4].End == 4 );

      REQUIRE( record.Connections[5].Start == 2 );
      REQUIRE( record.Connections[5].End == 5 );

      REQUIRE( record.Connections[6].Start == 3 );
      REQUIRE( record.Connections[6].End == 5 );

      REQUIRE( record.Connections[7].Start == 3 );
      REQUIRE( record.Connections[7].End == 6 );

      REQUIRE( record.Connections[8].Start == 4 );
      REQUIRE( record.Connections[8].End == 0 );

      REQUIRE( record.Connections[9].Start == 4 );
      REQUIRE( record.Connections[9].End == 2 );

      REQUIRE( record.Connections[10].Start == 5 );
      REQUIRE( record.Connections[10].End == 2 );

      REQUIRE( record.Connections[11].Start == 5 );
      REQUIRE( record.Connections[11].End == 3 );

      REQUIRE( record.Connections[12].Start == 6 );
      REQUIRE( record.Connections[12].End == 1 );

      REQUIRE( record.Connections[13].Start == 6 );
      REQUIRE( record.Connections[13].End == 3 );

      REQUIRE( record.Connections[14].Start == 7 );
      REQUIRE( record.Connections[14].End == 0 );

      REQUIRE( record.Connections[15].Start == 7 );
      REQUIRE( record.Connections[15].End == 1 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "PGRD\x81\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0FAIL\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA is not 12")
    {
      {
        const auto data = "PGRD\x0A\x01\0\0\0\0\0\0\0\0\0\0DATA\x0B\0\0\0\0\0\0\0\0\0\0\0\0\x04\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip PGRD, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        PathGridRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "PGRD\x0C\x01\0\0\0\0\0\0\0\0\0\0DATA\x0D\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip PGRD, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        PathGridRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before DATA can be read")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0FAIL\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\x01\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\xFE\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no PGRP")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0FAIL\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PGRP is not a multiple of eight")
    {
      const auto data = "PGRD\x0C\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x81\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PGRP is beyond stream")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: number of points in DATA does not match the number of points in PGRP")
    {
      SECTION("DATA has more")
      {
        const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x09\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip PGRD, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        PathGridRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      SECTION("DATA has less")
      {
        const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x07\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip PGRD, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        PathGridRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: no PGRC")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0FAIL@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PGRC is not a multiple of four")
    {
      const auto data = "PGRD\x0C\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC\x41\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0\0\0\x05\0\0\0\x05\0\0\0\x06\0\0\0\0\0\0\0\x02\0\0\0\x02\0\0\0\x03\0\0\0\x01\0\0\0\x03\0\0\0\0\0\0\0\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PGRC is beyond stream")
    {
      const auto data = "PGRD\x0B\x01\0\0\0\0\0\0\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\0\x04\x08\0NAME\x1F\0\0\0Zainab Lager, Zelt des Ashkhan\0PGRP\x80\0\0\0\xF9\xFF\xFF\xFF)\x01\0\0\xF7\xFE\xFF\xFF\x01\x02\0\0}\0\0\0\xB8\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0p\xFF\xFF\xFF\xBB\x01\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xF7\xFF\xFF\xFF#\x02\0\0\xF2\xFE\xFF\xFF\x01\x02\0\0\xA2\xFF\xFF\xFFt\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0\xA6\xFF\xFF\xFF\x15\x02\0\0\xF8\xFE\xFF\xFF\x01\x02\0\0P\0\0\0\x09\x02\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0J\0\0\0m\x01\0\0\xF5\xFE\xFF\xFF\x01\x02\0\0PGRC@\0\0\0\x04\0\0\0\x07\0\0\0\x06\0\0\0\x07\0\0\0\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip PGRD, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      PathGridRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
