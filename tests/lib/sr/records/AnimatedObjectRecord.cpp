/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2021, 2024, 2025  Dirk Stolle

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
#include "../../../../lib/sr/records/AnimatedObjectRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"
#include "../../limited_streambuf.hpp"

TEST_CASE("AnimatedObjectRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    AnimatedObjectRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.unknownBNAM.empty() );
  }

  SECTION("equals")
  {
    AnimatedObjectRecord a;
    AnimatedObjectRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      b.editorID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("editorID mismatch")
      {
        a.editorID = "foo";
        b.editorID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("model path mismatch")
      {
        a.modelPath = "foo\\bar.nif";
        b.modelPath = "foo2\\bar3.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MODT mismatch")
      {
        a.unknownMODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownMODT.setPresence(false);
        b.unknownMODT.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("BNAM mismatch")
      {
        a.unknownBNAM = "foo";
        b.unknownBNAM = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    AnimatedObjectRecord record;

    REQUIRE( record.getRecordType() == cANIO );
  }

  SECTION("getWriteSize")
  {
    AnimatedObjectRecord record;

    record.editorID = "foo";
    record.modelPath = "foo\\bar.nif";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 28 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 40 );
    }

    SECTION("size adjusts with length of model path")
    {
      REQUIRE( record.getWriteSize() == 28 );

      record.modelPath = "foo\\bar12.nif";
      REQUIRE( record.getWriteSize() == 30 );

      record.modelPath = "foo\\barbarbarbar.nif";
      REQUIRE( record.getWriteSize() == 37 );
    }

    SECTION("size adjusts with length of MODT")
    {
      REQUIRE( record.getWriteSize() == 28 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "MODT\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 52 );

      {
        const std::string_view data = "MODT\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownMODT.loadFromStream(stream, cMODT, true) );
      }
      REQUIRE( record.getWriteSize() == 35 );
    }

    SECTION("size adjusts with length of BNAM")
    {
      REQUIRE( record.getWriteSize() == 28 );

      record.unknownBNAM = "foo";
      REQUIRE( record.getWriteSize() == 38 );

      record.unknownBNAM = "foobarbaz";
      REQUIRE( record.getWriteSize() == 44 );
    }
  }

  SECTION("loadFromStream")
  {
    StringTable dummy_table;

    SECTION("default: load record")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AnimatedObjectRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000200 );
      REQUIRE( record.headerFormID == 0x0010FD00 );
      REQUIRE( record.headerRevision == 0x0061680D );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0000 );
      // -- record data
      REQUIRE( record.editorID == "AnimObjectIronSword" );
      REQUIRE( record.modelPath == "AnimObjects\\AnimObjectIronSword.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE( record.unknownBNAM == "AnimObjectUnequip" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0FAIL\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\x02GnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x6E\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0FAIL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\x02GnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple model paths")
    {
      const auto data = "ANIO\x98\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: model path is empty string")
    {
      const auto data = "ANIO\x4B\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL\x01\0\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing model path")
    {
      const auto data = "ANIO\x32\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const auto data = "ANIO\x80\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of MODT can be read")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of BNAM > 512")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\x02GnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple BNAMs")
    {
      const auto data = "ANIO\x86\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: BNAM is empty string")
    {
      const auto data = "ANIO\x5D\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x01\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AnimatedObjectRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("failure: cannot write header data")
    {
      AnimatedObjectRecord record;
      // Set some header data.
      record.headerFlags = 0;
      record.headerFormID = 0x0010FAF8;
      record.headerRevision = 0x0078680C;
      record.headerVersion = 39;
      record.headerUnknown5 = 0x0001;

      // Writing should fail due to limited stream storage.
      MWTP::limited_streambuf<15> buffer;
      std::ostream stream(&buffer);
      REQUIRE( stream.good() );

      REQUIRE_FALSE( record.saveToStream(stream) );
    }

    SECTION("failure: cannot write MODT to stream")
    {
      const auto data = "ANIO\x6E\0\0\0\0\x02\0\0\0\xFD\x10\0\x0D\x68\x61\0\x28\0\0\0EDID\x14\0AnimObjectIronSword\0MODL$\0AnimObjects\\AnimObjectIronSword.nif\0MODT\x0C\0\x02\0\0\0\0\0\0\0\0\0\0\0BNAM\x12\0AnimObjectUnequip\0"sv;
      std::istringstream stream_in;
      stream_in.str(std::string(data));

      // Skip ANIO, because header is handled before loadFromStream.
      stream_in.seekg(4);
      REQUIRE( stream_in.good() );

      // Reading should succeed.
      AnimatedObjectRecord record;
      StringTable dummy_table;
      REQUIRE( record.loadFromStream(stream_in, true, dummy_table) );
      // Check data of MODT.
      REQUIRE( record.unknownMODT.isPresent() );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x02\0\0\0\0\0\0\0\0\0\0\0"sv );

      // Writing should fail due to limited stream storage.
      MWTP::limited_streambuf<102> buffer;
      std::ostream stream_out(&buffer);
      REQUIRE( stream_out.good() );

      REQUIRE_FALSE( record.saveToStream(stream_out) );
    }
  }
}
