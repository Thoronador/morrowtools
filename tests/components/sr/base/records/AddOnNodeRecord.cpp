/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/AddOnNodeRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("AddOnNodeRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    AddOnNodeRecord record;

    REQUIRE( record.editorID.empty() );
    for (const uint8_t byte: record.unknownOBND)
    {
      REQUIRE( byte == 0 );
    }
    REQUIRE( record.modelPath.empty() );
    REQUIRE_FALSE( record.unknownMODT.isPresent() );
    REQUIRE( record.unknownDATA == 0 );
    REQUIRE( record.soundDescriptorFormID == 0 );
    REQUIRE( record.MasterParticleSystemCap == 0 );
    REQUIRE( record.flags == 1 ); // yes, flags are one by default
  }

  SECTION("equals")
  {
    AddOnNodeRecord a;
    AddOnNodeRecord b;

    SECTION("equal")
    {
      a.editorID = "foo";
      a.modelPath = "foo\\bar.nif";
      a.unknownDATA = 0xDEADBEEF;
      a.unknownOBND = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B };
      a.soundDescriptorFormID = 0xAFFEAFFE;
      a.MasterParticleSystemCap = 0x1234;
      a.flags = 0x5678;

      b.editorID = "foo";
      b.modelPath = "foo\\bar.nif";
      b.unknownDATA = 0xDEADBEEF;
      b.unknownOBND = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B };
      b.soundDescriptorFormID = 0xAFFEAFFE;
      b.MasterParticleSystemCap = 0x1234;
      b.flags = 0x5678;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("editorID mismatch")
      {
        REQUIRE( a.equals(b) );
        a.editorID = "foo";
        b.editorID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("model path mismatch")
      {
        REQUIRE( a.equals(b) );
        a.modelPath = "foo\\bar.nif";
        b.modelPath = "foo\\quux.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("unknownDATA mismatch")
      {
        REQUIRE( a.equals(b) );
        a.unknownDATA = 0xDEADBEEF;
        b.unknownDATA = 0x0F00BA12;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("OBND mismatch")
      {
        REQUIRE( a.equals(b) );
        a.unknownOBND[11] = 0xFF;
        b.unknownOBND[11] = 0xBB;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("soundDescriptorFormID mismatch")
      {
        REQUIRE( a.equals(b) );
        a.soundDescriptorFormID = 123;
        b.soundDescriptorFormID = 456;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("MasterParticleSystemCap mismatch")
      {
        REQUIRE( a.equals(b) );
        a.MasterParticleSystemCap = 123;
        b.MasterParticleSystemCap = 456;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("flags mismatch")
      {
        REQUIRE( a.equals(b) );
        a.flags = 123;
        b.flags = 456;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    AddOnNodeRecord record;

    REQUIRE( record.getRecordType() == cADDN );
  }

  SECTION("getWriteSize")
  {
    AddOnNodeRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.modelPath = "foo\\bar.nif";

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 78 );

      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 66 );
    }

    SECTION("size adjusts with length of model path")
    {
      record.editorID = "foo";

      record.modelPath = "foo\\bar.nif"; // 11 characters
      REQUIRE( record.getWriteSize() == 66 );

      record.modelPath = "foo\\bar\\baz\\quux.nif"; // 20 characters
      REQUIRE( record.getWriteSize() == 75 );
    }

    SECTION("size increases when MODT is present")
    {
      record.editorID = "foo";
      record.modelPath = "foo\\bar.nif";
      record.unknownMODT.setPresence(false);
      REQUIRE( record.getWriteSize() == 66 );

      record.unknownMODT.setPresence(true);
      REQUIRE( record.getWriteSize() == 72 );

      // Reading MODT from stream to fill it with data.
      const std::string_view data = "\x13\0ActionShieldChange\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));
      REQUIRE( record.unknownMODT.loadFromStream(stream, cEDID, false) );
      // Check presence + length.
      REQUIRE( record.unknownMODT.isPresent() );
      REQUIRE( record.unknownMODT.size() == 19 );

      REQUIRE( record.getWriteSize() == 91 );
    }

    SECTION("size increases when sound descriptor ID is set")
    {
      record.editorID = "foo";
      record.modelPath = "foo\\bar.nif";

      record.soundDescriptorFormID = 0;
      REQUIRE( record.getWriteSize() == 66 );

      record.soundDescriptorFormID = 0xDEADBEEF;
      REQUIRE( record.getWriteSize() == 76 );
    }

    SECTION("deleted record has size zero")
    {
      record.headerFlags = BasicRecord::cDeletedFlag;
      REQUIRE( record.getWriteSize() == 0 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;

    SECTION("default: load record")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      AddOnNodeRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0001C2B0 );
      REQUIRE( record.headerRevision == 0x000E6713 );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "MPSFireWallBase" );
      const auto expectedOBND = std::array<uint8_t, 12>({ 0, 0x80, 0, 0x80, 0, 0x80, 0xFF, 0x7F, 0xFF, 0x7F, 0xFF, 0x7F });
      REQUIRE( record.unknownOBND == expectedOBND );
      REQUIRE( record.modelPath == "MPS\\MPSFireWallBase.nif" );
      REQUIRE( record.unknownMODT.isPresent() );
      REQUIRE( record.unknownMODT.size() == 24 );
      const auto MODT = std::string_view(reinterpret_cast<const char*>(record.unknownMODT.data()), record.unknownMODT.size());
      REQUIRE( MODT == "\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33"sv);
      REQUIRE( record.unknownDATA == 0x00000003 );
      REQUIRE( record.MasterParticleSystemCap == 0 );
      REQUIRE( record.flags == 1 );

      // Save it, because it's already filled with the data.
      std::ostringstream streamOut;
      // Writing should succeed.
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: cannot read header data")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0FAIL\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\x02MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\xF1\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0FAIL\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before OBND can be read completely")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not twelve")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0D\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no MODL")
    {
      const std::string_view data = "ADDN\x5A\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of MODL > 512")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\x02MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple MODTs")
    {
      const std::string_view data = "ADDN\x96\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before MODT can be read completely")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no DATA")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x46\x41IL\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DATA subrecord")
    {
      const std::string_view data = "ADDN\x6E\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44NAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const std::string_view data = "ADDN\x82\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DATA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not four")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x05\0\x03\0\0\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no DNAM")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0FAIL\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing DNAM subrecord")
    {
      const std::string_view data = "ADDN\x6E\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DNAMs")
    {
      const std::string_view data = "ADDN\x82\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x04\0\0\0\x01\0DNAM\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DNAM is not four")
    {
      const std::string_view data = "ADDN\x78\0\0\0\0\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0EDID\x10\0MPSFireWallBase\0OBND\x0C\0\0\x80\0\x80\0\x80\xFF\x7F\xFF\x7F\xFF\x7FMODL\x18\0MPS\\MPSFireWallBase.nif\0MODT\x18\0\x12\x77\x91\x7C\x64\x64\x73\0\x38\x97\x3C\xEA\xA8\x21\xF6\x78\x64\x64\x73\0\x58\x2C\x55\x33\x44\x41TA\x04\0\x03\0\0\0DNAM\x05\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read ADDN, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      AddOnNodeRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("save deleted record")
    {
      std::ostringstream stream;

      REQUIRE( stream.good() );

      AddOnNodeRecord record;
      // Fill data.
      // -- header
      record.headerFlags = BasicRecord::cDeletedFlag;
      record.headerFormID = 0x0001C2B0;
      record.headerRevision = 0x000E6713;
      record.headerVersion = 39;
      record.headerUnknown5 = 0x0002;
      // -- record data (does not really matter, will not be written)
      record.editorID = "MPSFireWallBase";

      // Record should be deleted.
      REQUIRE( record.isDeleted() );

      // Writing should succeed.
      REQUIRE( record.saveToStream(stream) );
      // Check written data.
      const std::string_view data = "ADDN\0\0\0\0\x20\0\0\0\xB0\xC2\x01\0\x13\x67\x0E\0\x27\0\x02\0"sv;
      REQUIRE( stream.str() == data );
    }
  }

  SECTION("alwaysLoaded")
  {
    AddOnNodeRecord record;
    REQUIRE_FALSE( record.alwaysLoaded() );
    record.flags |= AddOnNodeRecord::cFlagAlwaysLoaded;
    REQUIRE( record.alwaysLoaded() );
  }
}
