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

#include "../../../locate_catch.hpp"
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
    REQUIRE_FALSE( record.ColourRef.has_value() );
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
        a.ColourRef.reset();
        a.ColourRef = 0x00123456;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

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
      REQUIRE( record.isInterior() );
      REQUIRE( record.RegionID.empty() );
      REQUIRE( record.NumReferences == 10 );
      REQUIRE_FALSE( record.ColourRef.has_value() );
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

    SECTION("default: load exterior cell with region but with empty name")
    {
      const auto data = "CELL\x3A\0\0\0\0\0\0\0\0\0\0\0NAME\x01\0\0\0\0DATA\x0C\0\0\0\x02\0\0\0\x17\0\0\0\x07\0\0\0RGNN\x15\0\0\0Azura's Coast Region\0"sv;
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
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.CellFlags == 0x00000002 );
      REQUIRE( record.GridX == 23 );
      REQUIRE( record.GridY == 7 );
      REQUIRE_FALSE( record.isInterior() );
      REQUIRE( record.RegionID == "Azura's Coast Region" );
      REQUIRE( record.NumReferences == 0 );
      REQUIRE_FALSE( record.ColourRef.has_value() );
      REQUIRE_FALSE( record.WaterHeight.has_value() );
      REQUIRE_FALSE( record.Ambience.isPresent );
      REQUIRE( record.Ambience.AmbientColour == 0 );
      REQUIRE( record.Ambience.SunlightColour == 0 );
      REQUIRE( record.Ambience.FogColour == 0 );
      REQUIRE( record.Ambience.FogDensity == 0.0f );
      REQUIRE( record.ReferencesPersistent.empty() );
      REQUIRE( record.ReferencesOther.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load exterior cell with colour reference")
    {
      const auto data = "CELL\xB1\x0A\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0Rotheran\0DATA\x0C\0\0\0B\0\0\0\x06\0\0\0\x12\0\0\0RGNN\x0A\0\0\0Sheogorad\0NAM5\x04\0\0\0\xCA\xA5`\0FRMR\x04\0\0\0\xFF\xF1\x01\0NAME\x11\0\0\0dolvasie veloren\0DATA\x18\0\0\0\0\x80SG\0\x80\x16H\0\0\xE0\x44\0\0\0\0\0\0\0\0Av\x8F@FRMR\x04\0\0\0\xFE\xF1\x01\0NAME\x10\0\0\0tirasie andalen\0DATA\x18\0\0\0\x16\x0AQG\xA1y\x15HM}\xDE\x44\0\0\0\0\0\0\0\0\xA8\xDC\xC5@FRMR\x04\0\0\0\xF3\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0s\x85\xD5\x43OR\x1F\x44\0\0\xB8\xC3\0\0\0\0\0\0\0\0\xE4\xCB\x96@DNAM\x1A\0\0\0Rotheran, Propylon-Kammer\0DATA\x18\0\0\0V]UG\x0Ci\x15H\xEF\xCC\xE6\x44\0\0\0\0\0\0\0\0\xDB\x0F\xC9?FRMR\x04\0\0\0\xFA\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0\x37\x89I@\xC9\x16\x65\x44%f\xD4\xC3\0\0\0\0\0\0\0\0\0\0\0\0DNAM\x18\0\0\0Rotheran, Gemeindeh\xFCtte\0DATA\x18\0\0\0\x1F@OG\x8C\xD0\x15H\x06n\xE6\x44\0\0\0\0\0\0\0\0\xA5\xFF\x12@FRMR\x04\0\0\0\xFC\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0)\x1A\x11\x45s\xA8>Dh\x11'\xC2\0\0\0\0\0\0\0\0\xD4\xD8\x95@DNAM\x10\0\0\0Rotheran, Arena\0DATA\x18\0\0\0X]TG\x8B\x8D\x16H\xEF\x8C\xE6\x44\0\0\0\0\0\0\0\0A`\xB0@NAM0\x04\0\0\0\x1E\0\0\0FRMR\x04\0\0\0\xF1\xF1\x01\0NAME\x15\0\0\0ex_stronghold_fort03\0DATA\x18\0\0\0X\x1DUG\x8B\x95\x14HxF\x0F\x45\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF2\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0X\x1DUG\x8AK\x15H\xED\x8C\xEE\x44\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF4\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon02\0DATA\x18\0\0\0X\x9DQG\x8B\x8D\x15H\xEF\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF5\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0T\x1DKG\x8A\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF6\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0R\x1DWG\x8B\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDA\x0F\xC9@FRMR\x04\0\0\0\xF7\xF1\x01\0NAME\x18\0\0\0ex_stronghold_sandpit00\0DATA\x18\0\0\0\x9F\x9CMG\x14\xC1\x16H\xEF\x8C\xDE\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF8\xF1\x01\0NAME\x15\0\0\0ex_stronghold_dome00\0DATA\x18\0\0\0\xC3\x41MG*Y\x15H\xEF\x8C\xFE\x44\0\0\0\0\0\0\0\0\xEBRE@FRMR\x04\0\0\0\xF9\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\x86\xC0NG\xC4\xC4\x15Ha\x95\xEE\x44\0\0\0\0\0\0\0\0\xF2\x0E\x13@FRMR\x04\0\0\0\xFB\xF1\x01\0NAME\x18\0\0\0ex_stronghold_smdwell00\0DATA\x18\0\0\0j\xC3UG.\xCE\x16H\xC3\xAB\xEE\x44\0\0\0\0\0\0\0\0\xCD\xCCL?FRMR\x04\0\0\0\xFD\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\xDB\xDDTGq\x96\x16HI\xB4\xEE\x44\0\0\0\0\0\0\0\0|e\xB0@FRMR\x04\0\0\0\xB5\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xDD\x83RG\x9E-\x10HM1\x18\x44\0\0\0\0\0\0\0\0\x91/\xBB@FRMR\x04\0\0\0\xB6\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0$\x1CNG\xEDJ\x12H\xC3\xDA,D\0\0\0\0\0\0\0\0\xE8~\x88?FRMR\x04\0\0\0\xB7\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xB2ZPGO\"\x12H\xEB\xBC\x8C\x44\0\0\0\0\0\0\0\0\xEDR%@FRMR\x04\0\0\0\xB8\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0'\xC4JGB\x90\x11Hx\xD9\x98\x44\0\0\0\0\0\0\0\0^?\xE4?FRMR\x04\0\0\0\xB9\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0*\x94NG7|\x10H\xDA\xB4\x80\x44\0\0\0\0\0\0\0\0\x1C\x86\x98@FRMR\x04\0\0\0\xBA\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0^:GGo\x13\x12H\xEF\xD7\x42\x44\0\0\0\0\0\0\0\0e?\x04@FRMR\x04\0\0\0\xBB\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\xECQ\xB8?DATA\x18\0\0\0\xE9\x81GG\x9Bz\x11Hq\x03\xD0\x44\0\0\0\0\0\0\0\0\xDC\x30\xC4>FRMR\x04\0\0\0\xBC\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0l\x13\x44G\xBCy\x15H\xA3\x8FTD\0\0\0\0\0\0\0\0\xC5\x62\xAE@FRMR\x04\0\0\0\xBD\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xC2\xE7\x43G\xAD\xCB\x13H\x01\xC5\x81\x44\0\0\0\0\0\0\0\08q\xC7@FRMR\x04\0\0\0\xBE\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x0B\xD0^G\x11\xBA\x11Hq\xB0\xF9\x42\0\0\0\0\0\0\0\0\xA3\xDC\x95@FRMR\x04\0\0\0\xBF\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0w\xF6ZG\x89\xD9\x12H\x19QjC\0\0\0\0\0\0\0\09\x0C\xB1?FRMR\x04\0\0\0\xC0\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x93\x43\\G\x9E\xD1\x10H\x9C+\xAD\x42\0\0\0\0\0\0\0\0\x0E\xD9}@FRMR\x04\0\0\0\xC1\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x35\x81WG)4\x12H\x8C\x15@D\0\0\0\0\0\0\0\0\xC4\x62\xAE@FRMR\x04\0\0\0\xC2\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xA2\x9FYGi\xF4\x10H\x9A\x0E+D\0\0\0\0\0\0\0\0Cv\xAF@FRMR\x04\0\0\0\xC3\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xFB\x63]G-\xE7\x16H\xC2\xF7\xF8\x43\0\0\0\0\0\0\0\0\xAF\xD7??FRMR\x04\0\0\0\x17,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\xCF\x01KG\xD9\xF3\x10HR\xF5\x30\x45\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0\x18,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\x88\x1D[G\xCD\x1E\x12HM\x80\x07\x45\0\0\0\0\0\0\0\0\0\0\0\0"sv;
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
      REQUIRE( record.CellID == "Rotheran" );
      REQUIRE( record.CellFlags == 0x00000042 );
      REQUIRE( record.GridX == 6 );
      REQUIRE( record.GridY == 18 );
      REQUIRE_FALSE( record.isInterior() );
      REQUIRE( record.RegionID == "Sheogorad" );
      REQUIRE( record.NumReferences == 30 );
      REQUIRE( record.ColourRef.has_value() );
      REQUIRE( record.ColourRef.value() == 0x0060A5CA );
      REQUIRE_FALSE( record.WaterHeight.has_value() );
      REQUIRE_FALSE( record.Ambience.isPresent );
      REQUIRE( record.ReferencesPersistent.size() == 5 );
      REQUIRE( record.ReferencesPersistent[0].ObjectID == "dolvasie veloren" );
      REQUIRE( record.ReferencesPersistent[1].ObjectID == "tirasie andalen" );
      REQUIRE( record.ReferencesPersistent[2].ObjectID == "in_strong_vaultdoor00" );
      REQUIRE( record.ReferencesPersistent[3].ObjectID == "in_strong_vaultdoor00" );
      REQUIRE( record.ReferencesPersistent[4].ObjectID == "in_strong_vaultdoor00" );
      REQUIRE( record.ReferencesOther.size() == 27 );
      REQUIRE( record.ReferencesOther[0].ObjectID == "ex_stronghold_fort03" );
      REQUIRE( record.ReferencesOther[1].ObjectID == "ex_stronghold_enter00" );
      REQUIRE( record.ReferencesOther[2].ObjectID == "ex_stronghold_pylon02" );
      REQUIRE( record.ReferencesOther[26].ObjectID == "ex_shore_cliffracer_lev+0" );

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

    SECTION("corrupt data: no RGNN")
    {
      const auto data = "CELL\x3A\0\0\0\0\0\0\0\0\0\0\0NAME\x01\0\0\0\0DATA\x0C\0\0\0\x02\0\0\0\x17\0\0\0\x07\0\0\0FAIL\x15\0\0\0Azura's Coast Region\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple RGNNs")
    {
      const auto data = "CELL\x57\0\0\0\0\0\0\0\0\0\0\0NAME\x01\0\0\0\0DATA\x0C\0\0\0\x02\0\0\0\x17\0\0\0\x07\0\0\0RGNN\x15\0\0\0Azura's Coast Region\0RGNN\x15\0\0\0Azura's Coast Region\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RGNN > 256")
    {
      const auto data = "CELL\x3A\0\0\0\0\0\0\0\0\0\0\0NAME\x01\0\0\0\0DATA\x0C\0\0\0\x02\0\0\0\x17\0\0\0\x07\0\0\0RGNN\x15\x01\0\0Azura's Coast Region\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RGNN is beyond stream")
    {
      const auto data = "CELL\x3A\0\0\0\0\0\0\0\0\0\0\0NAME\x01\0\0\0\0DATA\x0C\0\0\0\x02\0\0\0\x17\0\0\0\x07\0\0\0RGNN\x25\0\0\0Azura's Coast Region\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple NAM5s")
    {
      const auto data = "CELL\xBD\x0A\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0Rotheran\0DATA\x0C\0\0\0B\0\0\0\x06\0\0\0\x12\0\0\0RGNN\x0A\0\0\0Sheogorad\0NAM5\x04\0\0\0\xCA\xA5`\0NAM5\x04\0\0\0\xCA\xA5`\0FRMR\x04\0\0\0\xFF\xF1\x01\0NAME\x11\0\0\0dolvasie veloren\0DATA\x18\0\0\0\0\x80SG\0\x80\x16H\0\0\xE0\x44\0\0\0\0\0\0\0\0Av\x8F@FRMR\x04\0\0\0\xFE\xF1\x01\0NAME\x10\0\0\0tirasie andalen\0DATA\x18\0\0\0\x16\x0AQG\xA1y\x15HM}\xDE\x44\0\0\0\0\0\0\0\0\xA8\xDC\xC5@FRMR\x04\0\0\0\xF3\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0s\x85\xD5\x43OR\x1F\x44\0\0\xB8\xC3\0\0\0\0\0\0\0\0\xE4\xCB\x96@DNAM\x1A\0\0\0Rotheran, Propylon-Kammer\0DATA\x18\0\0\0V]UG\x0Ci\x15H\xEF\xCC\xE6\x44\0\0\0\0\0\0\0\0\xDB\x0F\xC9?FRMR\x04\0\0\0\xFA\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0\x37\x89I@\xC9\x16\x65\x44%f\xD4\xC3\0\0\0\0\0\0\0\0\0\0\0\0DNAM\x18\0\0\0Rotheran, Gemeindeh\xFCtte\0DATA\x18\0\0\0\x1F@OG\x8C\xD0\x15H\x06n\xE6\x44\0\0\0\0\0\0\0\0\xA5\xFF\x12@FRMR\x04\0\0\0\xFC\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0)\x1A\x11\x45s\xA8>Dh\x11'\xC2\0\0\0\0\0\0\0\0\xD4\xD8\x95@DNAM\x10\0\0\0Rotheran, Arena\0DATA\x18\0\0\0X]TG\x8B\x8D\x16H\xEF\x8C\xE6\x44\0\0\0\0\0\0\0\0A`\xB0@NAM0\x04\0\0\0\x1E\0\0\0FRMR\x04\0\0\0\xF1\xF1\x01\0NAME\x15\0\0\0ex_stronghold_fort03\0DATA\x18\0\0\0X\x1DUG\x8B\x95\x14HxF\x0F\x45\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF2\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0X\x1DUG\x8AK\x15H\xED\x8C\xEE\x44\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF4\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon02\0DATA\x18\0\0\0X\x9DQG\x8B\x8D\x15H\xEF\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF5\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0T\x1DKG\x8A\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF6\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0R\x1DWG\x8B\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDA\x0F\xC9@FRMR\x04\0\0\0\xF7\xF1\x01\0NAME\x18\0\0\0ex_stronghold_sandpit00\0DATA\x18\0\0\0\x9F\x9CMG\x14\xC1\x16H\xEF\x8C\xDE\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF8\xF1\x01\0NAME\x15\0\0\0ex_stronghold_dome00\0DATA\x18\0\0\0\xC3\x41MG*Y\x15H\xEF\x8C\xFE\x44\0\0\0\0\0\0\0\0\xEBRE@FRMR\x04\0\0\0\xF9\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\x86\xC0NG\xC4\xC4\x15Ha\x95\xEE\x44\0\0\0\0\0\0\0\0\xF2\x0E\x13@FRMR\x04\0\0\0\xFB\xF1\x01\0NAME\x18\0\0\0ex_stronghold_smdwell00\0DATA\x18\0\0\0j\xC3UG.\xCE\x16H\xC3\xAB\xEE\x44\0\0\0\0\0\0\0\0\xCD\xCCL?FRMR\x04\0\0\0\xFD\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\xDB\xDDTGq\x96\x16HI\xB4\xEE\x44\0\0\0\0\0\0\0\0|e\xB0@FRMR\x04\0\0\0\xB5\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xDD\x83RG\x9E-\x10HM1\x18\x44\0\0\0\0\0\0\0\0\x91/\xBB@FRMR\x04\0\0\0\xB6\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0$\x1CNG\xEDJ\x12H\xC3\xDA,D\0\0\0\0\0\0\0\0\xE8~\x88?FRMR\x04\0\0\0\xB7\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xB2ZPGO\"\x12H\xEB\xBC\x8C\x44\0\0\0\0\0\0\0\0\xEDR%@FRMR\x04\0\0\0\xB8\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0'\xC4JGB\x90\x11Hx\xD9\x98\x44\0\0\0\0\0\0\0\0^?\xE4?FRMR\x04\0\0\0\xB9\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0*\x94NG7|\x10H\xDA\xB4\x80\x44\0\0\0\0\0\0\0\0\x1C\x86\x98@FRMR\x04\0\0\0\xBA\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0^:GGo\x13\x12H\xEF\xD7\x42\x44\0\0\0\0\0\0\0\0e?\x04@FRMR\x04\0\0\0\xBB\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\xECQ\xB8?DATA\x18\0\0\0\xE9\x81GG\x9Bz\x11Hq\x03\xD0\x44\0\0\0\0\0\0\0\0\xDC\x30\xC4>FRMR\x04\0\0\0\xBC\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0l\x13\x44G\xBCy\x15H\xA3\x8FTD\0\0\0\0\0\0\0\0\xC5\x62\xAE@FRMR\x04\0\0\0\xBD\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xC2\xE7\x43G\xAD\xCB\x13H\x01\xC5\x81\x44\0\0\0\0\0\0\0\08q\xC7@FRMR\x04\0\0\0\xBE\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x0B\xD0^G\x11\xBA\x11Hq\xB0\xF9\x42\0\0\0\0\0\0\0\0\xA3\xDC\x95@FRMR\x04\0\0\0\xBF\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0w\xF6ZG\x89\xD9\x12H\x19QjC\0\0\0\0\0\0\0\09\x0C\xB1?FRMR\x04\0\0\0\xC0\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x93\x43\\G\x9E\xD1\x10H\x9C+\xAD\x42\0\0\0\0\0\0\0\0\x0E\xD9}@FRMR\x04\0\0\0\xC1\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x35\x81WG)4\x12H\x8C\x15@D\0\0\0\0\0\0\0\0\xC4\x62\xAE@FRMR\x04\0\0\0\xC2\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xA2\x9FYGi\xF4\x10H\x9A\x0E+D\0\0\0\0\0\0\0\0Cv\xAF@FRMR\x04\0\0\0\xC3\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xFB\x63]G-\xE7\x16H\xC2\xF7\xF8\x43\0\0\0\0\0\0\0\0\xAF\xD7??FRMR\x04\0\0\0\x17,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\xCF\x01KG\xD9\xF3\x10HR\xF5\x30\x45\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0\x18,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\x88\x1D[G\xCD\x1E\x12HM\x80\x07\x45\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // skip CELL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      CellRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAM5 is not four")
    {
      {
        const auto data = "CELL\xB0\x0A\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0Rotheran\0DATA\x0C\0\0\0B\0\0\0\x06\0\0\0\x12\0\0\0RGNN\x0A\0\0\0Sheogorad\0NAM5\x03\0\0\0\xCA\xA5`FRMR\x04\0\0\0\xFF\xF1\x01\0NAME\x11\0\0\0dolvasie veloren\0DATA\x18\0\0\0\0\x80SG\0\x80\x16H\0\0\xE0\x44\0\0\0\0\0\0\0\0Av\x8F@FRMR\x04\0\0\0\xFE\xF1\x01\0NAME\x10\0\0\0tirasie andalen\0DATA\x18\0\0\0\x16\x0AQG\xA1y\x15HM}\xDE\x44\0\0\0\0\0\0\0\0\xA8\xDC\xC5@FRMR\x04\0\0\0\xF3\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0s\x85\xD5\x43OR\x1F\x44\0\0\xB8\xC3\0\0\0\0\0\0\0\0\xE4\xCB\x96@DNAM\x1A\0\0\0Rotheran, Propylon-Kammer\0DATA\x18\0\0\0V]UG\x0Ci\x15H\xEF\xCC\xE6\x44\0\0\0\0\0\0\0\0\xDB\x0F\xC9?FRMR\x04\0\0\0\xFA\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0\x37\x89I@\xC9\x16\x65\x44%f\xD4\xC3\0\0\0\0\0\0\0\0\0\0\0\0DNAM\x18\0\0\0Rotheran, Gemeindeh\xFCtte\0DATA\x18\0\0\0\x1F@OG\x8C\xD0\x15H\x06n\xE6\x44\0\0\0\0\0\0\0\0\xA5\xFF\x12@FRMR\x04\0\0\0\xFC\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0)\x1A\x11\x45s\xA8>Dh\x11'\xC2\0\0\0\0\0\0\0\0\xD4\xD8\x95@DNAM\x10\0\0\0Rotheran, Arena\0DATA\x18\0\0\0X]TG\x8B\x8D\x16H\xEF\x8C\xE6\x44\0\0\0\0\0\0\0\0A`\xB0@NAM0\x04\0\0\0\x1E\0\0\0FRMR\x04\0\0\0\xF1\xF1\x01\0NAME\x15\0\0\0ex_stronghold_fort03\0DATA\x18\0\0\0X\x1DUG\x8B\x95\x14HxF\x0F\x45\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF2\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0X\x1DUG\x8AK\x15H\xED\x8C\xEE\x44\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF4\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon02\0DATA\x18\0\0\0X\x9DQG\x8B\x8D\x15H\xEF\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF5\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0T\x1DKG\x8A\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF6\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0R\x1DWG\x8B\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDA\x0F\xC9@FRMR\x04\0\0\0\xF7\xF1\x01\0NAME\x18\0\0\0ex_stronghold_sandpit00\0DATA\x18\0\0\0\x9F\x9CMG\x14\xC1\x16H\xEF\x8C\xDE\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF8\xF1\x01\0NAME\x15\0\0\0ex_stronghold_dome00\0DATA\x18\0\0\0\xC3\x41MG*Y\x15H\xEF\x8C\xFE\x44\0\0\0\0\0\0\0\0\xEBRE@FRMR\x04\0\0\0\xF9\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\x86\xC0NG\xC4\xC4\x15Ha\x95\xEE\x44\0\0\0\0\0\0\0\0\xF2\x0E\x13@FRMR\x04\0\0\0\xFB\xF1\x01\0NAME\x18\0\0\0ex_stronghold_smdwell00\0DATA\x18\0\0\0j\xC3UG.\xCE\x16H\xC3\xAB\xEE\x44\0\0\0\0\0\0\0\0\xCD\xCCL?FRMR\x04\0\0\0\xFD\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\xDB\xDDTGq\x96\x16HI\xB4\xEE\x44\0\0\0\0\0\0\0\0|e\xB0@FRMR\x04\0\0\0\xB5\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xDD\x83RG\x9E-\x10HM1\x18\x44\0\0\0\0\0\0\0\0\x91/\xBB@FRMR\x04\0\0\0\xB6\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0$\x1CNG\xEDJ\x12H\xC3\xDA,D\0\0\0\0\0\0\0\0\xE8~\x88?FRMR\x04\0\0\0\xB7\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xB2ZPGO\"\x12H\xEB\xBC\x8C\x44\0\0\0\0\0\0\0\0\xEDR%@FRMR\x04\0\0\0\xB8\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0'\xC4JGB\x90\x11Hx\xD9\x98\x44\0\0\0\0\0\0\0\0^?\xE4?FRMR\x04\0\0\0\xB9\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0*\x94NG7|\x10H\xDA\xB4\x80\x44\0\0\0\0\0\0\0\0\x1C\x86\x98@FRMR\x04\0\0\0\xBA\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0^:GGo\x13\x12H\xEF\xD7\x42\x44\0\0\0\0\0\0\0\0e?\x04@FRMR\x04\0\0\0\xBB\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\xECQ\xB8?DATA\x18\0\0\0\xE9\x81GG\x9Bz\x11Hq\x03\xD0\x44\0\0\0\0\0\0\0\0\xDC\x30\xC4>FRMR\x04\0\0\0\xBC\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0l\x13\x44G\xBCy\x15H\xA3\x8FTD\0\0\0\0\0\0\0\0\xC5\x62\xAE@FRMR\x04\0\0\0\xBD\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xC2\xE7\x43G\xAD\xCB\x13H\x01\xC5\x81\x44\0\0\0\0\0\0\0\08q\xC7@FRMR\x04\0\0\0\xBE\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x0B\xD0^G\x11\xBA\x11Hq\xB0\xF9\x42\0\0\0\0\0\0\0\0\xA3\xDC\x95@FRMR\x04\0\0\0\xBF\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0w\xF6ZG\x89\xD9\x12H\x19QjC\0\0\0\0\0\0\0\09\x0C\xB1?FRMR\x04\0\0\0\xC0\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x93\x43\\G\x9E\xD1\x10H\x9C+\xAD\x42\0\0\0\0\0\0\0\0\x0E\xD9}@FRMR\x04\0\0\0\xC1\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x35\x81WG)4\x12H\x8C\x15@D\0\0\0\0\0\0\0\0\xC4\x62\xAE@FRMR\x04\0\0\0\xC2\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xA2\x9FYGi\xF4\x10H\x9A\x0E+D\0\0\0\0\0\0\0\0Cv\xAF@FRMR\x04\0\0\0\xC3\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xFB\x63]G-\xE7\x16H\xC2\xF7\xF8\x43\0\0\0\0\0\0\0\0\xAF\xD7??FRMR\x04\0\0\0\x17,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\xCF\x01KG\xD9\xF3\x10HR\xF5\x30\x45\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0\x18,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\x88\x1D[G\xCD\x1E\x12HM\x80\x07\x45\0\0\0\0\0\0\0\0\0\0\0\0"sv;
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
        const auto data = "CELL\xB2\x0A\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0Rotheran\0DATA\x0C\0\0\0B\0\0\0\x06\0\0\0\x12\0\0\0RGNN\x0A\0\0\0Sheogorad\0NAM5\x05\0\0\0\xCA\xA5`\0\0FRMR\x04\0\0\0\xFF\xF1\x01\0NAME\x11\0\0\0dolvasie veloren\0DATA\x18\0\0\0\0\x80SG\0\x80\x16H\0\0\xE0\x44\0\0\0\0\0\0\0\0Av\x8F@FRMR\x04\0\0\0\xFE\xF1\x01\0NAME\x10\0\0\0tirasie andalen\0DATA\x18\0\0\0\x16\x0AQG\xA1y\x15HM}\xDE\x44\0\0\0\0\0\0\0\0\xA8\xDC\xC5@FRMR\x04\0\0\0\xF3\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0s\x85\xD5\x43OR\x1F\x44\0\0\xB8\xC3\0\0\0\0\0\0\0\0\xE4\xCB\x96@DNAM\x1A\0\0\0Rotheran, Propylon-Kammer\0DATA\x18\0\0\0V]UG\x0Ci\x15H\xEF\xCC\xE6\x44\0\0\0\0\0\0\0\0\xDB\x0F\xC9?FRMR\x04\0\0\0\xFA\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0\x37\x89I@\xC9\x16\x65\x44%f\xD4\xC3\0\0\0\0\0\0\0\0\0\0\0\0DNAM\x18\0\0\0Rotheran, Gemeindeh\xFCtte\0DATA\x18\0\0\0\x1F@OG\x8C\xD0\x15H\x06n\xE6\x44\0\0\0\0\0\0\0\0\xA5\xFF\x12@FRMR\x04\0\0\0\xFC\xF1\x01\0NAME\x16\0\0\0in_strong_vaultdoor00\0DODT\x18\0\0\0)\x1A\x11\x45s\xA8>Dh\x11'\xC2\0\0\0\0\0\0\0\0\xD4\xD8\x95@DNAM\x10\0\0\0Rotheran, Arena\0DATA\x18\0\0\0X]TG\x8B\x8D\x16H\xEF\x8C\xE6\x44\0\0\0\0\0\0\0\0A`\xB0@NAM0\x04\0\0\0\x1E\0\0\0FRMR\x04\0\0\0\xF1\xF1\x01\0NAME\x15\0\0\0ex_stronghold_fort03\0DATA\x18\0\0\0X\x1DUG\x8B\x95\x14HxF\x0F\x45\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF2\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0X\x1DUG\x8AK\x15H\xED\x8C\xEE\x44\0\0\0\0\0\0\0\0\xD6\x0F\xC9?FRMR\x04\0\0\0\xF4\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon02\0DATA\x18\0\0\0X\x9DQG\x8B\x8D\x15H\xEF\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF5\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0T\x1DKG\x8A\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF6\xF1\x01\0NAME\x16\0\0\0ex_stronghold_pylon00\0DATA\x18\0\0\0R\x1DWG\x8B\x8D\x15H\xEB\x8C\xB6\x44\0\0\0\0\0\0\0\0\xDA\x0F\xC9@FRMR\x04\0\0\0\xF7\xF1\x01\0NAME\x18\0\0\0ex_stronghold_sandpit00\0DATA\x18\0\0\0\x9F\x9CMG\x14\xC1\x16H\xEF\x8C\xDE\x44\0\0\0\0\0\0\0\0\xDB\x0FI@FRMR\x04\0\0\0\xF8\xF1\x01\0NAME\x15\0\0\0ex_stronghold_dome00\0DATA\x18\0\0\0\xC3\x41MG*Y\x15H\xEF\x8C\xFE\x44\0\0\0\0\0\0\0\0\xEBRE@FRMR\x04\0\0\0\xF9\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\x86\xC0NG\xC4\xC4\x15Ha\x95\xEE\x44\0\0\0\0\0\0\0\0\xF2\x0E\x13@FRMR\x04\0\0\0\xFB\xF1\x01\0NAME\x18\0\0\0ex_stronghold_smdwell00\0DATA\x18\0\0\0j\xC3UG.\xCE\x16H\xC3\xAB\xEE\x44\0\0\0\0\0\0\0\0\xCD\xCCL?FRMR\x04\0\0\0\xFD\xF1\x01\0NAME\x16\0\0\0ex_stronghold_enter00\0DATA\x18\0\0\0\xDB\xDDTGq\x96\x16HI\xB4\xEE\x44\0\0\0\0\0\0\0\0|e\xB0@FRMR\x04\0\0\0\xB5\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xDD\x83RG\x9E-\x10HM1\x18\x44\0\0\0\0\0\0\0\0\x91/\xBB@FRMR\x04\0\0\0\xB6\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0$\x1CNG\xEDJ\x12H\xC3\xDA,D\0\0\0\0\0\0\0\0\xE8~\x88?FRMR\x04\0\0\0\xB7\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xB2ZPGO\"\x12H\xEB\xBC\x8C\x44\0\0\0\0\0\0\0\0\xEDR%@FRMR\x04\0\0\0\xB8\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0'\xC4JGB\x90\x11Hx\xD9\x98\x44\0\0\0\0\0\0\0\0^?\xE4?FRMR\x04\0\0\0\xB9\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0*\x94NG7|\x10H\xDA\xB4\x80\x44\0\0\0\0\0\0\0\0\x1C\x86\x98@FRMR\x04\0\0\0\xBA\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0^:GGo\x13\x12H\xEF\xD7\x42\x44\0\0\0\0\0\0\0\0e?\x04@FRMR\x04\0\0\0\xBB\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\xECQ\xB8?DATA\x18\0\0\0\xE9\x81GG\x9Bz\x11Hq\x03\xD0\x44\0\0\0\0\0\0\0\0\xDC\x30\xC4>FRMR\x04\0\0\0\xBC\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0l\x13\x44G\xBCy\x15H\xA3\x8FTD\0\0\0\0\0\0\0\0\xC5\x62\xAE@FRMR\x04\0\0\0\xBD\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xC2\xE7\x43G\xAD\xCB\x13H\x01\xC5\x81\x44\0\0\0\0\0\0\0\08q\xC7@FRMR\x04\0\0\0\xBE\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x0B\xD0^G\x11\xBA\x11Hq\xB0\xF9\x42\0\0\0\0\0\0\0\0\xA3\xDC\x95@FRMR\x04\0\0\0\xBF\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0w\xF6ZG\x89\xD9\x12H\x19QjC\0\0\0\0\0\0\0\09\x0C\xB1?FRMR\x04\0\0\0\xC0\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x93\x43\\G\x9E\xD1\x10H\x9C+\xAD\x42\0\0\0\0\0\0\0\0\x0E\xD9}@FRMR\x04\0\0\0\xC1\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\x35\x81WG)4\x12H\x8C\x15@D\0\0\0\0\0\0\0\0\xC4\x62\xAE@FRMR\x04\0\0\0\xC2\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xA2\x9FYGi\xF4\x10H\x9A\x0E+D\0\0\0\0\0\0\0\0Cv\xAF@FRMR\x04\0\0\0\xC3\xB6\x04\0NAME\x11\0\0\0Ex_T_menhir_L_01\0XSCL\x04\0\0\0\0\0\0@DATA\x18\0\0\0\xFB\x63]G-\xE7\x16H\xC2\xF7\xF8\x43\0\0\0\0\0\0\0\0\xAF\xD7??FRMR\x04\0\0\0\x17,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\xCF\x01KG\xD9\xF3\x10HR\xF5\x30\x45\0\0\0\0\0\0\0\0\0\0\0\0FRMR\x04\0\0\0\x18,\x06\0NAME\x1A\0\0\0ex_shore_cliffracer_lev+0\0DATA\x18\0\0\0\x88\x1D[G\xCD\x1E\x12HM\x80\x07\x45\0\0\0\0\0\0\0\0\0\0\0\0"sv;
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

    SECTION("corrupt data: stream ends before NAM5 can be read")
    {
      const auto data = "CELL\xB1\x0A\0\0\0\0\0\0\0\0\0\0NAME\x09\0\0\0Rotheran\0DATA\x0C\0\0\0B\0\0\0\x06\0\0\0\x12\0\0\0RGNN\x0A\0\0\0Sheogorad\0NAM5\x04\0\0\0\xCA"sv;
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
