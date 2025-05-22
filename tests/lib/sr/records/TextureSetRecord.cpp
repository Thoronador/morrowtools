/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2025  Dirk Stolle

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
#include "../../limited_streambuf.hpp"
#include "../../../../lib/sr/records/TextureSetRecord.hpp"
#include "../../../../lib/sr/SR_Constants.hpp"
#include "../../../../lib/sr/StringTable.hpp"

TEST_CASE("TextureSetRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    TextureSetRecord record;

    REQUIRE( record.editorID.empty() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    REQUIRE( record.texture00.empty() );
    REQUIRE( record.texture01.empty() );
    REQUIRE( record.texture02.empty() );
    REQUIRE( record.texture03.empty() );
    REQUIRE( record.texture04.empty() );
    REQUIRE( record.texture05.empty() );
    REQUIRE( record.texture07.empty() );
    REQUIRE_FALSE( record.unknownDODT.has_value() );
    REQUIRE( record.unknownDNAM == 0 );
  }

  SECTION("equals")
  {
    TextureSetRecord a;
    TextureSetRecord b;

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

      SECTION("OBND mismatch")
      {
        a.unknownOBND[11] = 12;
        b.unknownOBND[11] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("texture00 mismatch")
      {
        a.texture00 = "foo\\bar.dds";
        b.texture00 = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("texture01 mismatch")
      {
        a.texture01 = "foo\\bar.dds";
        b.texture01 = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("texture02 mismatch")
      {
        a.texture02 = "foo\\bar.dds";
        b.texture02 = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("texture03 mismatch")
      {
        a.texture03 = "foo\\bar.dds";
        b.texture03 = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("texture04 mismatch")
      {
        a.texture04 = "foo\\bar.dds";
        b.texture04 = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("texture05 mismatch")
      {
        a.texture05 = "foo\\bar.dds";
        b.texture05 = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("texture07 mismatch")
      {
        a.texture07 = "foo\\bar.dds";
        b.texture07 = "foo2\\bar3.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DODT mismatch")
      {
        constexpr auto data = std::array<uint8_t, 36>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
        a.unknownDODT = data;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownDODT.reset();
        b.unknownDODT = data;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownDODT = data;
        b.unknownDODT = data;

        a.unknownDODT.value()[35] = 0x12;
        b.unknownDODT.value()[35] = 0x34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DNAM mismatch")
      {
        a.unknownDNAM = 12;
        b.unknownDNAM = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    TextureSetRecord record;

    REQUIRE( record.getRecordType() == cTXST );
  }

  SECTION("getWriteSize")
  {
    TextureSetRecord record;

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 62 );
    }

    SECTION("size adjusts with length of texture00 path")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds"; // seven characters
      REQUIRE( record.getWriteSize() == 50 );

      record.texture00 = "foobar.dds"; // ten characters
      REQUIRE( record.getWriteSize() == 53 );

      record.texture00 = "foobarbarbarbar.dds";
      REQUIRE( record.getWriteSize() == 62 );
    }

    SECTION("size adjusts with length of texture01 path")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.texture01 = "foo1.dds"; // eight characters
      REQUIRE( record.getWriteSize() == 65 );

      record.texture01 = "foobar01.dds"; // twelve characters
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of texture02 path")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.texture02 = "foo1.dds"; // eight characters
      REQUIRE( record.getWriteSize() == 65 );

      record.texture02 = "foobar01.dds"; // twelve characters
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of texture03 path")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.texture03 = "foo1.dds"; // eight characters
      REQUIRE( record.getWriteSize() == 65 );

      record.texture03 = "foobar01.dds"; // twelve characters
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of texture04 path")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.texture04 = "foo1.dds"; // eight characters
      REQUIRE( record.getWriteSize() == 65 );

      record.texture04 = "foobar01.dds"; // twelve characters
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of texture05 path")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.texture05 = "foo1.dds"; // eight characters
      REQUIRE( record.getWriteSize() == 65 );

      record.texture05 = "foobar01.dds"; // twelve characters
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts with length of texture07 path")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.texture07 = "foo1.dds"; // eight characters
      REQUIRE( record.getWriteSize() == 65 );

      record.texture07 = "foobar01.dds"; // twelve characters
      REQUIRE( record.getWriteSize() == 69 );
    }

    SECTION("size adjusts when DODT is present")
    {
      record.editorID = "foo";
      record.texture00 = "foo.dds";
      REQUIRE( record.getWriteSize() == 50 );

      record.unknownDODT = std::array<uint8_t, 36>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
      REQUIRE( record.getWriteSize() == 92 );
    }
  }

  SECTION("loadFromStream")
  {
    StringTable dummy_table;

    SECTION("default: load record with TX01")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TextureSetRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0010FAF8 );
      REQUIRE( record.headerRevision == 0x0078680C );
      REQUIRE( record.headerVersion == 39 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "VaerminaRobes" );
      REQUIRE( record.unknownOBND[0] == 0x00 );
      REQUIRE( record.unknownOBND[1] == 0x00 );
      REQUIRE( record.unknownOBND[2] == 0x00 );
      REQUIRE( record.unknownOBND[3] == 0x00 );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x00 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x00 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x00 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.texture00 == "Clothes\\Monk\\VaerminaVariantRobes.dds" );
      REQUIRE( record.texture01 == "Clothes\\Monk\\VaerminaVariantRobes_n.dds" );
      REQUIRE( record.texture02.empty() );
      REQUIRE( record.texture03.empty() );
      REQUIRE( record.texture04.empty() );
      REQUIRE( record.texture05.empty() );
      REQUIRE( record.texture07.empty() );
      REQUIRE_FALSE( record.unknownDODT.has_value() );
      REQUIRE( record.unknownDNAM == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with TX01, TX02, TX05 and DODT")
    {
      const auto data = "TXST\xE6\0\0\0\0\0\0\0\xDE\xD0\x0F\0\x0A\x66K\0%\0\x01\0EDID\x1C\0DecalPuddlesLargeSmallGroup\0OBND\x0C\0\xF8\xFF\xE2\xFF\xEC\xFF\x07\0\x1D\0\x13\0TX00\x18\0Puddle\\DecalPuddles.dds\0TX01\x1A\0Puddle\\DecalPuddles_N.dds\0TX02\x1A\0Puddle\\DecalPuddles_m.dds\0TX05\x1C\0Cubemaps\\QuickSkyDark_e.dds\0DODT$\0\0\0\x80\x43\0\0\0D\0\0\x80\x43\0\0\0D\0\0\0C\0\0\xC8\x43\0\0\x80?\x04z\x7F\0\xFF\xFF\xFF\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TextureSetRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x000FD0DE );
      REQUIRE( record.headerRevision == 0x004B660A );
      REQUIRE( record.headerVersion == 37 );
      REQUIRE( record.headerUnknown5 == 0x0001 );
      // -- record data
      REQUIRE( record.editorID == "DecalPuddlesLargeSmallGroup" );
      REQUIRE( record.unknownOBND[0] == 0xF8 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xE2 );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0xEC );
      REQUIRE( record.unknownOBND[5] == 0xFF );
      REQUIRE( record.unknownOBND[6] == 0x07 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x1D );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x13 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.texture00 == "Puddle\\DecalPuddles.dds" );
      REQUIRE( record.texture01 == "Puddle\\DecalPuddles_N.dds" );
      REQUIRE( record.texture02 == "Puddle\\DecalPuddles_m.dds" );
      REQUIRE( record.texture03.empty() );
      REQUIRE( record.texture04.empty() );
      REQUIRE( record.texture05 == "Cubemaps\\QuickSkyDark_e.dds" );
      REQUIRE( record.texture07.empty() );
      REQUIRE( record.unknownDODT.has_value() );
      // DODT$\0 \0\0\x80\x43\0\0\0D\0\0\x80 \x43\0\0\0D\0\0\0C\0 \0\xC8\x43\0\0\x80?\x04z\x7F \0\xFF\xFF\xFF\0
      const auto& DODT = record.unknownDODT.value();
      REQUIRE( DODT[0] == 0 );      // }
      REQUIRE( DODT[1] == 0 );      // } float, 256.0f
      REQUIRE( DODT[2] == 0x80 );   // }
      REQUIRE( DODT[3] == 0x43 );   // }
      REQUIRE( DODT[4] == 0x00 );   // )
      REQUIRE( DODT[5] == 0x00 );   // ) float, 512.0f
      REQUIRE( DODT[6] == 0x00 );   // )
      REQUIRE( DODT[7] == 0x44 );   // )
      REQUIRE( DODT[8] == 0x00 );   // ]
      REQUIRE( DODT[9] == 0x00 );   // ] float, 256.0f
      REQUIRE( DODT[10] == 0x80 );  // ]
      REQUIRE( DODT[11] == 0x43 );  // ]
      REQUIRE( DODT[12] == 0x00 );  // }
      REQUIRE( DODT[13] == 0x00 );  // } float, 512.0f
      REQUIRE( DODT[14] == 0x00 );  // }
      REQUIRE( DODT[15] == 0x44 );  // }
      REQUIRE( DODT[16] == 0x00 );  // )
      REQUIRE( DODT[17] == 0x00 );  // ) float, 128.0f
      REQUIRE( DODT[18] == 0x00 );  // )
      REQUIRE( DODT[19] == 0x43 );  // )
      REQUIRE( DODT[20] == 0x00 );  // ]
      REQUIRE( DODT[21] == 0x00 );  // ] float, 400.0f
      REQUIRE( DODT[22] == 0xC8 );  // ]
      REQUIRE( DODT[23] == 0x43 );  // ]
      REQUIRE( DODT[24] == 0x00 );  // }
      REQUIRE( DODT[25] == 0x00 );  // } float, 1.0f
      REQUIRE( DODT[26] == 0x80 );  // }
      REQUIRE( DODT[27] == 0x3F );  // }
      REQUIRE( DODT[28] == 0x04 );
      REQUIRE( DODT[29] == 0x7A );
      REQUIRE( DODT[30] == 0x7F );
      REQUIRE( DODT[31] == 0x00 );
      REQUIRE( DODT[32] == 0xFF );
      REQUIRE( DODT[33] == 0xFF );
      REQUIRE( DODT[34] == 0xFF );
      REQUIRE( DODT[35] == 0x00 );

      REQUIRE( record.unknownDNAM == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with TX01, TX03, and TX05")
    {
      const auto data = "TXST\xAD\0\0\0\0\0\0\0\x84\xF6\x10\0\x10h\x03\0(\0\x02\0EDID\x0D\0SkinHorseEye\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00\x19\0Actors\\Horse\\Horse04.dds\0TX01\x1B\0Actors\\Horse\\Horse01_n.dds\0TX03\x1C\0Actors\\Horse\\Horse01_sk.dds\0TX05\x18\0Cubemaps\\\x45yeCubeMap.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TextureSetRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x0010F684 );
      REQUIRE( record.headerRevision == 0x00036810 );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0002 );
      // -- record data
      REQUIRE( record.editorID == "SkinHorseEye" );
      REQUIRE( record.unknownOBND[0] == 0x00 );
      REQUIRE( record.unknownOBND[1] == 0x00 );
      REQUIRE( record.unknownOBND[2] == 0x00 );
      REQUIRE( record.unknownOBND[3] == 0x00 );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x00 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x00 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x00 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.texture00 == "Actors\\Horse\\Horse04.dds" );
      REQUIRE( record.texture01 == "Actors\\Horse\\Horse01_n.dds" );
      REQUIRE( record.texture02.empty() );
      REQUIRE( record.texture03 == "Actors\\Horse\\Horse01_sk.dds" );
      REQUIRE( record.texture04.empty() );
      REQUIRE( record.texture05 == "Cubemaps\\EyeCubeMap.dds" );
      REQUIRE( record.texture07.empty() );
      REQUIRE_FALSE( record.unknownDODT.has_value() );
      REQUIRE( record.unknownDNAM == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with TX01, TX03, TX04 and TX07")
    {
      const auto data = "TXST\x1B\x01\0\0\0\0\0\0\x46\x16\x05\0\x1C\x63\\\0\"\0\x05\0EDID\x17\0SkinHeadMaleImperial20\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00#\0Actors\\Character\\Male\\MaleHead.dds\0TX01/\0Actors\\Character\\ImperialMale\\MaleHead_msn.dds\0TX03&\0Actors\\Character\\Male\\MaleHead_sk.dds\0TX04)\0Actors\\Character\\Male\\BlankDetailmap.dds\0TX07%\0Actors\\Character\\Male\\MaleHead_S.dds\0DNAM\x02\0\x06\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      TextureSetRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0 );
      REQUIRE( record.headerFormID == 0x00051646 );
      REQUIRE( record.headerRevision == 0x005C631C );
      REQUIRE( record.headerVersion == 34 );
      REQUIRE( record.headerUnknown5 == 0x0005 );
      // -- record data
      REQUIRE( record.editorID == "SkinHeadMaleImperial20" );
      REQUIRE( record.unknownOBND[0] == 0x00 );
      REQUIRE( record.unknownOBND[1] == 0x00 );
      REQUIRE( record.unknownOBND[2] == 0x00 );
      REQUIRE( record.unknownOBND[3] == 0x00 );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x00 );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x00 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x00 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.texture00 == "Actors\\Character\\Male\\MaleHead.dds" );
      REQUIRE( record.texture01 == "Actors\\Character\\ImperialMale\\MaleHead_msn.dds" );
      REQUIRE( record.texture02.empty() );
      REQUIRE( record.texture03 == "Actors\\Character\\Male\\MaleHead_sk.dds" );
      REQUIRE( record.texture04 == "Actors\\Character\\Male\\BlankDetailmap.dds" );
      REQUIRE( record.texture05.empty() );
      REQUIRE( record.texture07 == "Actors\\Character\\Male\\MaleHead_S.dds" );
      REQUIRE_FALSE( record.unknownDODT.has_value() );
      REQUIRE( record.unknownDNAM == 0x0006 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const std::string_view data = "TXST\x1B\x01\0\0\0\0\0\0\x46\x16"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0FAIL\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\x02VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x88\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of EDID can be read")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0Vaermina"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0FAIL\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const auto data = "TXST\x87\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0B\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TXST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TextureSetRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "TXST\x89\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TXST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TextureSetRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of OBND can be read")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of TX00 > 512")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\x02\x43lothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DNAM is not two")
    {
      {
        const auto data = "TXST\x87\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x01\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TXST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TextureSetRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "TXST\x89\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x03\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TXST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TextureSetRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: multiple DNAMs")
    {
      const auto data = "TXST\x90\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DNAM is missing")
    {
      const auto data = "TXST\x80\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before DNAM can be read")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DODT is not 36")
    {
      {
        const auto data = "TXST\xE5\0\0\0\0\0\0\0\xDE\xD0\x0F\0\x0A\x66K\0%\0\x01\0EDID\x1C\0DecalPuddlesLargeSmallGroup\0OBND\x0C\0\xF8\xFF\xE2\xFF\xEC\xFF\x07\0\x1D\0\x13\0TX00\x18\0Puddle\\DecalPuddles.dds\0TX01\x1A\0Puddle\\DecalPuddles_N.dds\0TX02\x1A\0Puddle\\DecalPuddles_m.dds\0TX05\x1C\0Cubemaps\\QuickSkyDark_e.dds\0DODT\x23\0\0\0\x80\x43\0\0D\0\0\x80\x43\0\0\0D\0\0\0C\0\0\xC8\x43\0\0\x80?\x04z\x7F\0\xFF\xFF\xFF\0DNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TXST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TextureSetRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "TXST\xE7\0\0\0\0\0\0\0\xDE\xD0\x0F\0\x0A\x66K\0%\0\x01\0EDID\x1C\0DecalPuddlesLargeSmallGroup\0OBND\x0C\0\xF8\xFF\xE2\xFF\xEC\xFF\x07\0\x1D\0\x13\0TX00\x18\0Puddle\\DecalPuddles.dds\0TX01\x1A\0Puddle\\DecalPuddles_N.dds\0TX02\x1A\0Puddle\\DecalPuddles_m.dds\0TX05\x1C\0Cubemaps\\QuickSkyDark_e.dds\0DODT\x25\0\0\0\x80\x43\0\0\0D\0\0\x80\x43\0\0\0D\0\0\0C\0\0\xC8\x43\0\0\x80?\x04z\x7F\0\xFF\xFF\xFF\0\0DNAM\x02\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip TXST, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        TextureSetRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: multiple DODTs")
    {
      const auto data = "TXST\x10\x01\0\0\0\0\0\0\xDE\xD0\x0F\0\x0A\x66K\0%\0\x01\0EDID\x1C\0DecalPuddlesLargeSmallGroup\0OBND\x0C\0\xF8\xFF\xE2\xFF\xEC\xFF\x07\0\x1D\0\x13\0TX00\x18\0Puddle\\DecalPuddles.dds\0TX01\x1A\0Puddle\\DecalPuddles_N.dds\0TX02\x1A\0Puddle\\DecalPuddles_m.dds\0TX05\x1C\0Cubemaps\\QuickSkyDark_e.dds\0DODT$\0\0\0\x80\x43\0\0\0D\0\0\x80\x43\0\0\0D\0\0\0C\0\0\xC8\x43\0\0\x80?\x04z\x7F\0\xFF\xFF\xFF\0DODT$\0\0\0\x80\x43\0\0\0D\0\0\x80\x43\0\0\0D\0\0\0C\0\0\xC8\x43\0\0\x80?\x04z\x7F\0\xFF\xFF\xFF\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before DODT can be read completely")
    {
      const auto data = "TXST\xE6\0\0\0\0\0\0\0\xDE\xD0\x0F\0\x0A\x66K\0%\0\x01\0EDID\x1C\0DecalPuddlesLargeSmallGroup\0OBND\x0C\0\xF8\xFF\xE2\xFF\xEC\xFF\x07\0\x1D\0\x13\0TX00\x18\0Puddle\\DecalPuddles.dds\0TX01\x1A\0Puddle\\DecalPuddles_N.dds\0TX02\x1A\0Puddle\\DecalPuddles_m.dds\0TX05\x1C\0Cubemaps\\QuickSkyDark_e.dds\0DODT$\0\0\0\x80\x43\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TX01 paths")
    {
      const auto data = "TXST\xB6\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0TX01(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before TX01 can be read completely")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX01(\0Clot"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TX02 paths")
    {
      const auto data = "TXST\xB6\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX02(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0TX02(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before TX02 can be read completely")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX02(\0Clot"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TX03 paths")
    {
      const auto data = "TXST\xB6\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX03(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0TX03(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before TX03 can be read completely")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX03(\0Clot"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TX04 paths")
    {
      const auto data = "TXST\xB6\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX04(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0TX04(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before TX04 can be read completely")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX04(\0Clot"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TX05 paths")
    {
      const auto data = "TXST\xB6\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX05(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0TX05(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before TX05 can be read completely")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX05(\0Clot"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TX07 paths")
    {
      const auto data = "TXST\xB6\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX07(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0TX07(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before TX07 can be read completely")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX07(\0Clot"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: unsupported record type")
    {
      const auto data = "TXST\x88\0\0\0\0\0\0\0\xF8\xFA\x10\0\x0Chx\0'\0\x01\0EDID\x0E\0VaerminaRobes\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0TX00&\0Clothes\\Monk\\VaerminaVariantRobes.dds\0TX99(\0Clothes\\Monk\\VaerminaVariantRobes_n.dds\0DNAM\x02\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip TXST, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      TextureSetRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }

  SECTION("saveToStream")
  {
    SECTION("failure: cannot write header data")
    {
      TextureSetRecord record;
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
  }
}
