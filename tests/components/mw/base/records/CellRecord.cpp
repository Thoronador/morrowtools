/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

#include <catch.hpp>
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/CellRecord.hpp"

TEST_CASE("MWTP::CellRecord")
{
  using namespace MWTP;

  SECTION("constructor")
  {
    CellRecord record;

    REQUIRE( record.CellID.empty() );
    REQUIRE( record.CellFlags == 0 );
    REQUIRE( record.GridX == 0 );
    REQUIRE( record.GridY == 0 );
    REQUIRE( record.RegionID.empty() );
    REQUIRE( record.NumReferences == 0 );
    REQUIRE( record.ColourRef == 0 );
    REQUIRE_FALSE( record.WaterHeight.has_value() );
    REQUIRE_FALSE( record.Ambience.isPresent );
    REQUIRE( record.ReferencesPersistent.empty() );
    REQUIRE( record.ReferencesOther.empty() );
  }

  SECTION("equals()")
  {
    CellRecord a;
    CellRecord b;

    SECTION("equal")
    {
      a.CellFlags = 1;
      a.CellID = "foo";

      b.CellFlags = 1;
      b.CellID = "foo";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("CellID mismatch")
      {
        a.CellID = "foo";
        b.CellID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CellFlags mismatch")
      {
        a.CellFlags = 1;
        b.CellFlags = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

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

      SECTION("RegionID mismatch")
      {
        a.RegionID = "foo";
        b.RegionID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NumReferences mismatch")
      {
        a.NumReferences = 1;
        b.NumReferences = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ColourRef mismatch")
      {
        a.ColourRef = 0x00123456;
        b.ColourRef = 0x00789ABC;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("WaterHeight mismatch")
      {
        a.WaterHeight.reset();
        b.WaterHeight = 0.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.WaterHeight = 1.0f;
        a.WaterHeight = 125.0f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Ambience mismatch")
      {
        a.Ambience.isPresent = false;
        b.Ambience.isPresent = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.Ambience.isPresent = true;
        a.Ambience.FogColour = 0x00123456;
        a.Ambience.isPresent = true;
        a.Ambience.FogColour = 0x00789ABC;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("persistent references mismatch")
      {
        a.ReferencesPersistent.clear();
        b.ReferencesPersistent.push_back(ReferencedObject());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("other references mismatch")
      {
        a.ReferencesOther.clear();
        b.ReferencesOther.push_back(ReferencedObject());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    using namespace std::string_view_literals;

    SECTION("default: load interior cell with ref number")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      CellRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.CellID == "Zerfallenes Schiffswrack, Unterdeck" );
      REQUIRE( record.CellFlags == 0x00000007 );
      // Grid coordinates are useless for interior cells.
      REQUIRE( record.GridX == 2631703 );
      REQUIRE( record.GridY == 1065353216 );
      REQUIRE( record.RegionID.empty() );
      REQUIRE( record.NumReferences == 10 );
      REQUIRE( record.ColourRef == 0 );
      REQUIRE( record.WaterHeight.has_value() );
      REQUIRE( record.WaterHeight.value() == 0.0f );
      REQUIRE( record.Ambience.isPresent );
      REQUIRE( record.Ambience.AmbientColour == 0x00363623 );
      REQUIRE( record.Ambience.SunlightColour == 0x00282817 );
      REQUIRE( record.Ambience.FogColour == 0x00282817 );
      REQUIRE( record.Ambience.FogDensity == 1.0f );
      REQUIRE( record.ReferencesPersistent.size() == 2 );
      REQUIRE( record.ReferencesPersistent[0].ObjectID == "Slaughterfish_Small" );
      REQUIRE( record.ReferencesPersistent[1].ObjectID == "in_de_shipdoor_toplevel" );
      REQUIRE( record.ReferencesOther.size() == 9 );
      REQUIRE( record.ReferencesOther[0].ObjectID == "barrel_01_float_food20" );
      REQUIRE( record.ReferencesOther[1].ObjectID == "barrel_01_float_food20" );
      REQUIRE( record.ReferencesOther[2].ObjectID == "in_de_ship_lowerlevel" );
      REQUIRE( record.ReferencesOther[8].ObjectID == "contain_corpse_water" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "CELL\x12\x03\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0FAIL$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0NAME$\x01\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "CELL\xAC\0\0\0\0\0\0\0\0\0\0\0NAME\xAC\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0FAIL\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA is not twelve")
    {
      {
        const auto data = "CELL\xE2\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0B\0\0\0\x07\0\0\0\x17((\0\0\0?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CELL\xE4\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0D\0\0\0\x07\0\0\0\x17((\0\0\0\x80?\0WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before DATA can be read")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: reference (FRMR) fails to load")
    {
      const auto data = "CELL\xE1\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x16\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple WHGTs")
    {
      const auto data = "CELL\xEF\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of WHGT is not four")
    {
      {
        const auto data = "CELL\xE2\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x03\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CELL\xE4\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x05\0\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before WHGT can be read")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple AMBIs")
    {
      const auto data = "CELL\xFB\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of AMBI is not 16")
    {
      {
        const auto data = "CELL\xE2\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x0F\0\0\0#66\0\x17((\0\x17((\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CELL\xE4\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x11\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?\0FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before AMBI can be read")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple NAM0s")
    {
      const auto data = "CELL\xEF\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0\0\0NAM0\x04\0\0\0\x0A\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAM0 is not four")
    {
      {
        const auto data = "CELL\xE2\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x03\0\0\0\x0A\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "CELL\xE4\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x05\0\0\0\x0A\0\0\0\0FRMR\x04\0\0\0!\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\xCE\x46\xEE\x44y\xFD\x8E\x45\x0Dz5\xC4\x90\x61\x46@\x0B:S@5\xFA\x8E<FRMR\x04\0\0\0\"\x19\x17\0NAME\x17\0\0\0barrel_01_float_food20\0DATA\x18\0\0\0\x87\xBB\xED\x44~\xD0\x81\x45 a\"\xC4$+\xB3?\xAE\xA5\x39@\xBA\x1AQ?FRMR\x04\0\0\0#\x19\x17\0NAME\x16\0\0\0in_de_ship_lowerlevel\0DATA\x18\0\0\0N\x9A\xFD\x44'\xF7|E\xC9uB\xC4\x96\xD2\xC5@\x12\x45\x9C@\x16\x1CR@FRMR\x04\0\0\0$\x19\x17\0NAME\x0C\0\0\0NorthMarker\0DATA\x18\0\0\0\xE2X\x07\x45\x81P^E\x12\x93^\xC4\0\0\0\0\0\0\0\0\xFF\xFF?@FRMR\x04\0\0\0%\x19\x17\0NAME\x14\0\0\0misc_de_pot_blue_01\0DATA\x18\0\0\0\xFB\xA2\xEA\x44N\x8A\x86\x45\xDF\xCDj\xC4\xA9\xDC\xB5@\xDC\x0F\xA9@\0\0\0\0FRMR\x04\0\0\0&\x19\x17\0NAME\x14\0\0\0furn_de_ex_bench_01\0DATA\x18\0\0\0\x9AT\xEA\x44\x94\x12\x8E\x45^\x1Cp\xC4\xCD\xCC\xCC=x\xA9\xC2@\xA9\xDC\xA5@FRMR\x04\0\0\0'\x19\x17\0NAME\x13\0\0\0Sound_Boat_Creak00\0DATA\x18\0\0\0\x8D\x95\xEA\x44\x45\x64tE\xABXC\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0(\x19\x17\0NAME\x14\0\0\0p_water_breathing_s\0XSCL\x04\0\0\0\x15\xAEG?DATA\x18\0\0\0dA\xE4\x44\xE5T\x84\x45\xA2\xCD\x64\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0)\x19\x17\0NAME\x15\0\0\0contain_corpse_water\0DATA\x18\0\0\0\xC9\xB6\xE6\x44\xB4(\x87\x45\xBBnj\xC4\0\0\0\0\xA9\xDC\xB5@\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // skip CELL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        CellRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before NAM0 can be read")
    {
      const auto data = "CELL\xE3\x03\0\0\0\0\0\0\0\0\0\0NAME$\0\0\0Zerfallenes Schiffswrack, Unterdeck\0DATA\x0C\0\0\0\x07\0\0\0\x17((\0\0\0\x80?WHGT\x04\0\0\0\0\0\0\0AMBI\x10\0\0\0#66\0\x17((\0\x17((\0\0\0\x80?FRMR\x04\0\0\0\x1F\x19\x17\0NAME\x14\0\0\0Slaughterfish_Small\0DATA\x18\0\0\0\x92\xDC\xF1\x44\x39\xA0\x80\x45\xB5\xE3\x66\xC4\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0 \x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@NAM0\x04\0\0\0\x0A\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }

  SECTION("fun with flags")
  {
    CellRecord record;

    SECTION("isInterior")
    {
      REQUIRE_FALSE( record.isInterior() );

      record.CellFlags = 0x00000001;
      REQUIRE( record.isInterior() );

      record.CellFlags = 0x00000002;
      REQUIRE_FALSE( record.isInterior() );

      record.CellFlags = 0x0000FFFF;
      REQUIRE( record.isInterior() );
    }

    SECTION("hasWater")
    {
      REQUIRE_FALSE( record.hasWater() );

      record.CellFlags = 0x00000001;
      REQUIRE_FALSE( record.hasWater() );

      record.CellFlags = 0x00000002;
      REQUIRE( record.hasWater() );

      record.CellFlags = 0x0000FFFF;
      REQUIRE( record.hasWater() );
    }
  }
}
