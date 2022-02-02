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
#include "../../../../../mw/base/records/ReferencedObject.hpp"

TEST_CASE("MWTP::ReferencedObject")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    ReferencedObject object;

    REQUIRE( object.ObjectIndex == 0 );
    REQUIRE( object.ObjectID.empty() );
    REQUIRE( object.Scale == 1.0f );
    REQUIRE( object.PosX == 0.0f );
    REQUIRE( object.PosY == 0.0f );
    REQUIRE( object.PosZ == 0.0f );
    REQUIRE( object.RotX == 0.0f );
    REQUIRE( object.RotY == 0.0f );
    REQUIRE( object.RotZ == 0.0f );
    REQUIRE_FALSE( object.hasDoorData );
    // DoorData is basically checked in test for RefDoorData.
    REQUIRE_FALSE( object.hasFLTV );
    REQUIRE( object.LockLevel == 0 );
    REQUIRE( object.KeyID.empty() );
    REQUIRE( object.TrapID.empty() );
    REQUIRE( object.OwnerID.empty() );
    REQUIRE( object.OwnerFactionID.empty() );
    REQUIRE( object.FactionRank == 0 );
    REQUIRE( object.GlobalVarID.empty() );
    REQUIRE( object.SoulCreatureID.empty() );
    REQUIRE_FALSE( object.hasXCHG );
    REQUIRE( object.EnchantCharge == 0.0f );
    REQUIRE( object.NumberOfUses == 0 );
    REQUIRE_FALSE( object.hasNAM9 );
    REQUIRE( object.UnknownNAM9 == 0 );
    REQUIRE_FALSE( object.hasUNAM );
    REQUIRE( object.ReferenceBlockedByte == 0 );
    REQUIRE_FALSE( object.isDeleted );
    REQUIRE( object.DeletionLong == 0 );
  }

  SECTION("equality operator")
  {
    ReferencedObject a;
    ReferencedObject b;

    SECTION("equal")
    {
      a.ObjectIndex = 1;
      a.ObjectID = "foo";

      b.ObjectIndex = 1;
      b.ObjectID = "foo";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("ObjectIndex mismatch")
      {
        a.ObjectIndex = 1;
        b.ObjectIndex = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ObjectID mismatch")
      {
        a.ObjectID = "foo";
        b.ObjectID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Scale mismatch")
      {
        a.Scale = 1.0f;
        b.Scale = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("PosX mismatch")
      {
        a.PosX = 1.0f;
        b.PosX = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("PosY mismatch")
      {
        a.PosY = 1.0f;
        b.PosY = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("PosZ mismatch")
      {
        a.PosZ = 1.0f;
        b.PosZ = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("RotX mismatch")
      {
        a.RotX = 1.0f;
        b.RotX = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("RotY mismatch")
      {
        a.RotY = 1.0f;
        b.RotY = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("RotZ mismatch")
      {
        a.RotZ = 1.0f;
        b.RotZ = 2.5f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("DoorData mismatch")
      {
        a.hasDoorData = false;
        b.hasDoorData = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasDoorData = true;
        a.DoorData.PosX = 123.45f;
        b.hasDoorData = true;
        b.DoorData.PosX = 678.901f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("LockLevel mismatch")
      {
        a.hasFLTV = false;
        b.hasFLTV = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasFLTV = true;
        a.LockLevel = 20;
        b.hasFLTV = true;
        b.LockLevel = 30;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("KeyID mismatch")
      {
        a.KeyID = "foo";
        b.KeyID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("TrapID mismatch")
      {
        a.TrapID = "foo";
        b.TrapID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("OwnerID mismatch")
      {
        a.OwnerID = "foo";
        b.OwnerID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("OwnerFactionID mismatch")
      {
        a.OwnerFactionID = "foo";
        b.OwnerFactionID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("FactionRank mismatch")
      {
        a.FactionRank = 1;
        b.FactionRank = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("GlobalVarID mismatch")
      {
        a.GlobalVarID = "foo";
        b.GlobalVarID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("SoulCreatureID mismatch")
      {
        a.SoulCreatureID = "foo";
        b.SoulCreatureID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("EnchantCharge mismatch")
      {
        a.hasXCHG = false;
        b.hasXCHG = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasXCHG = true;
        a.EnchantCharge = 20;
        b.hasXCHG = true;
        b.EnchantCharge = 50;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("NumberOfUses mismatch")
      {
        a.NumberOfUses = 1;
        b.NumberOfUses = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("NAM9 mismatch")
      {
        a.hasNAM9 = false;
        b.hasNAM9 = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasNAM9 = true;
        a.UnknownNAM9 = 1;
        b.hasNAM9 = true;
        b.UnknownNAM9 = 234;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ReferenceBlockedByte mismatch")
      {
        a.hasUNAM = false;
        b.hasUNAM = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasUNAM = true;
        a.ReferenceBlockedByte = 0;
        b.hasUNAM = true;
        b.ReferenceBlockedByte = 1;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("Deletion mismatch")
      {
        a.isDeleted = false;
        b.isDeleted = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.isDeleted = true;
        a.DeletionLong = 123;
        b.isDeleted = true;
        b.DeletionLong = 456;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    uint32_t bytesRead = 0;
    char buffer[256];

    SECTION("default: load minimalistic reference data")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00171978 );
      REQUIRE( object.ObjectID == "bookskill_heavy armor5" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 9.2984704971313f );
      REQUIRE( object.PosY == 1533.0828857422f );
      REQUIRE( object.PosZ == -961.26110839844f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 0.0f );
      REQUIRE_FALSE( object.hasDoorData );
      REQUIRE_FALSE( object.hasFLTV );
      REQUIRE( object.LockLevel == 0 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.hasXCHG );
      REQUIRE( object.EnchantCharge == 0.0f );
      REQUIRE( object.NumberOfUses == 0 );
      REQUIRE_FALSE( object.hasNAM9 );
      REQUIRE( object.UnknownNAM9 == 0 );
      REQUIRE_FALSE( object.hasUNAM );
      REQUIRE( object.ReferenceBlockedByte == 0 );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load reference data with scale multiplier")
    {
      const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0XSCL\x04\0\0\0\xA4p=?DATA\x18\0\0\0\xF8\x80\xAC\x41\xF0w\xEF\x44pu&\xC4\0\0\0\0\0\0\0\0\xFC\x97Y@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00171941 );
      REQUIRE( object.ObjectID == "ex_dae_malacath_attack" );
      REQUIRE( object.Scale == 0.74f );
      REQUIRE( object.PosX == 21.562973022461f );
      REQUIRE( object.PosY == 1915.748046875f );
      REQUIRE( object.PosZ == -665.8349609375f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 3.3999013900757f );
      REQUIRE_FALSE( object.hasDoorData );
      REQUIRE_FALSE( object.hasFLTV );
      REQUIRE( object.LockLevel == 0 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.hasXCHG );
      REQUIRE( object.EnchantCharge == 0.0f );
      REQUIRE( object.NumberOfUses == 0 );
      REQUIRE_FALSE( object.hasNAM9 );
      REQUIRE( object.UnknownNAM9 == 0 );
      REQUIRE_FALSE( object.hasUNAM );
      REQUIRE( object.ReferenceBlockedByte == 0 );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load reference with door data")
    {
      const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0DODT\x18\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`E@DATA\x18\0\0\0\0\0\x80\x38\0\0\xB5\x45\0\0\x84\xC3\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x0017192C );
      REQUIRE( object.ObjectID == "ex_dae_door_load_oval" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 0.00006103515625f );
      REQUIRE( object.PosY == 5792.0f );
      REQUIRE( object.PosZ == -264.0f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 0.0f );
      REQUIRE( object.hasDoorData );
      REQUIRE( object.DoorData.PosX == 45036.90625f );
      REQUIRE( object.DoorData.PosY == 126711.8828125f );
      REQUIRE( object.DoorData.PosZ == 822.48498535156f );
      REQUIRE( object.DoorData.RotX == 0.0f );
      REQUIRE( object.DoorData.RotY == 0.0f );
      REQUIRE( object.DoorData.RotZ == 3.0839967727661f );
      REQUIRE( object.DoorData.ExitName.empty() );
      REQUIRE_FALSE( object.hasFLTV );
      REQUIRE( object.LockLevel == 0 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.hasXCHG );
      REQUIRE( object.EnchantCharge == 0.0f );
      REQUIRE( object.NumberOfUses == 0 );
      REQUIRE_FALSE( object.hasNAM9 );
      REQUIRE( object.UnknownNAM9 == 0 );
      REQUIRE_FALSE( object.hasUNAM );
      REQUIRE( object.ReferenceBlockedByte == 0 );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load reference with door data and exit name")
    {
      const auto data = "FRMR\x04\0\0\0\x20\x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00171920 );
      REQUIRE( object.ObjectID == "in_de_shipdoor_toplevel" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 1978.5799560547f );
      REQUIRE( object.PosY == 3361.3679199219f );
      REQUIRE( object.PosZ == -734.52899169922f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 1.6999506950378f );
      REQUIRE( object.RotZ == 3.0839967727661f );
      REQUIRE( object.hasDoorData );
      REQUIRE( object.DoorData.PosX == 4040.9389648438f );
      REQUIRE( object.DoorData.PosY == 3342.4580078125f );
      REQUIRE( object.DoorData.PosZ == -423.58499145508f );
      REQUIRE( object.DoorData.RotX == 0.0f );
      REQUIRE( object.DoorData.RotY == 1.5707963705063f );
      REQUIRE( object.DoorData.RotZ == 0.0f );
      REQUIRE( object.DoorData.ExitName == "Zerfallenes Schiffswrack, Oberdeck" );
      REQUIRE_FALSE( object.hasFLTV );
      REQUIRE( object.LockLevel == 0 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.hasXCHG );
      REQUIRE( object.EnchantCharge == 0.0f );
      REQUIRE( object.NumberOfUses == 0 );
      REQUIRE_FALSE( object.hasNAM9 );
      REQUIRE( object.UnknownNAM9 == 0 );
      REQUIRE_FALSE( object.hasUNAM );
      REQUIRE( object.ReferenceBlockedByte == 0 );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load reference with lock")
    {
      const auto data = "FRMR\x04\0\0\0\x13\x19\x17\0NAME\x16\0\0\0de_p_chest_02_gold_25\0FLTV\x04\0\0\0\x0F\0\0\0DATA\x18\0\0\0\x1B\xDB|E\x0Bx{E\x80t4\xC4\xFD\xB5\xC5@\xAA\x61\x9C@~\xFFQ@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00171913 );
      REQUIRE( object.ObjectID == "de_p_chest_02_gold_25" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 4045.6940917969f );
      REQUIRE( object.PosY == 4023.5026855469f );
      REQUIRE( object.PosZ == -721.8203125f );
      REQUIRE( object.RotX == 6.1784653663635f );
      REQUIRE( object.RotY == 4.8869218826294f );
      REQUIRE( object.RotZ == 3.2812190055847f );
      REQUIRE_FALSE( object.hasDoorData );
      REQUIRE( object.hasFLTV );
      REQUIRE( object.LockLevel == 15 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.hasXCHG );
      REQUIRE( object.EnchantCharge == 0.0f );
      REQUIRE( object.NumberOfUses == 0 );
      REQUIRE_FALSE( object.hasNAM9 );
      REQUIRE( object.UnknownNAM9 == 0 );
      REQUIRE_FALSE( object.hasUNAM );
      REQUIRE( object.ReferenceBlockedByte == 0 );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load reference with ANAM, NAM9, INTV")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00171594 );
      REQUIRE( object.ObjectID == "misc_de_goblet_07" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == -6.6539859771729f );
      REQUIRE( object.PosY == 626.73620605469f );
      REQUIRE( object.PosZ == -196.06608581543f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 4.3831872940063f );
      REQUIRE_FALSE( object.hasDoorData );
      REQUIRE_FALSE( object.hasFLTV );
      REQUIRE( object.LockLevel == 0 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID == "kaushad" );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.hasXCHG );
      REQUIRE( object.EnchantCharge == 0.0f );
      REQUIRE( object.NumberOfUses == 1 );
      REQUIRE( object.hasNAM9 );
      REQUIRE( object.UnknownNAM9 == 0x00000001 );
      REQUIRE_FALSE( object.hasUNAM );
      REQUIRE( object.ReferenceBlockedByte == 0 );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: length of FRMR is not four")
    {
      {
        const auto data = "FRMR\x03\0\0\0\x78\x19\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x05\0\0\0\x78\x19\x17\0\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before FRMR can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0FAIL\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\x01\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x57\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\0\0\0bookskill_heavy armor5\0FAIL\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of DATA is not 24")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x17\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x19\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before DATA can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: no XSCL")
    {
      const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0FAIL\x04\0\0\0\xA4p=?DATA\x18\0\0\0\xF8\x80\xAC\x41\xF0w\xEF\x44pu&\xC4\0\0\0\0\0\0\0\0\xFC\x97Y@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple XSCL")
    {
      const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0XSCL\x04\0\0\0\xA4p=?XSCL\x04\0\0\0\xA4p=?DATA\x18\0\0\0\xF8\x80\xAC\x41\xF0w\xEF\x44pu&\xC4\0\0\0\0\0\0\0\0\xFC\x97Y@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of XSCL is not four")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0XSCL\x03\0\0\0\xA4p=DATA\x18\0\0\0\xF8\x80\xAC\x41\xF0w\xEF\x44pu&\xC4\0\0\0\0\0\0\0\0\xFC\x97Y@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0XSCL\x05\0\0\0\xA4p=?\0DATA\x18\0\0\0\xF8\x80\xAC\x41\xF0w\xEF\x44pu&\xC4\0\0\0\0\0\0\0\0\xFC\x97Y@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before XSCL can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0XSCL\x04\0\0\0\xA4p"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: no DODT")
    {
      const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0FAIL\x18\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`E@DATA\x18\0\0\0\0\0\x80\x38\0\0\xB5\x45\0\0\x84\xC3\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple DODT")
    {
      const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0DODT\x18\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`E@DODT\x18\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`E@DATA\x18\0\0\0\0\0\x80\x38\0\0\xB5\x45\0\0\x84\xC3\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of DODT is not 24")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0DODT\x17\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`EDATA\x18\0\0\0\0\0\x80\x38\0\0\xB5\x45\0\0\x84\xC3\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0DODT\x19\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`E@\0DATA\x18\0\0\0\0\0\x80\x38\0\0\xB5\x45\0\0\x84\xC3\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before DODT can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0DODT\x18\0\0\0\xE8\xEC"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: no DNAM")
    {
      const auto data = "FRMR\x04\0\0\0\x20\x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0FAIL#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: DNAM without previous DODT")
    {
      const auto data = "FRMR\x04\0\0\0\x20\x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple DNAMs")
    {
      const auto data = "FRMR\x04\0\0\0\x20\x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of DNAM > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x20\x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\x01\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of DNAM is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x20\x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM\x83\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of FLTV is not four")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x13\x19\x17\0NAME\x16\0\0\0de_p_chest_02_gold_25\0FLTV\x03\0\0\0\x0F\0\0DATA\x18\0\0\0\x1B\xDB|E\x0Bx{E\x80t4\xC4\xFD\xB5\xC5@\xAA\x61\x9C@~\xFFQ@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x13\x19\x17\0NAME\x16\0\0\0de_p_chest_02_gold_25\0FLTV\x05\0\0\0\x0F\0\0\0\0DATA\x18\0\0\0\x1B\xDB|E\x0Bx{E\x80t4\xC4\xFD\xB5\xC5@\xAA\x61\x9C@~\xFFQ@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before FLTV can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x13\x19\x17\0NAME\x16\0\0\0de_p_chest_02_gold_25\0FLTV\x04\0\0\0\x0F\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple FLTVs")
    {
      const auto data = "FRMR\x04\0\0\0\x13\x19\x17\0NAME\x16\0\0\0de_p_chest_02_gold_25\0FLTV\x04\0\0\0\x0F\0\0\0FLTV\x04\0\0\0\x0F\0\0\0DATA\x18\0\0\0\x1B\xDB|E\x0Bx{E\x80t4\xC4\xFD\xB5\xC5@\xAA\x61\x9C@~\xFFQ@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple ANAMs")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of ANAM > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\x01\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of ANAM is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\xB8\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of INTV is not four")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x03\0\0\0\x01\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x05\0\0\0\x01\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before INTV can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple INTVs")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of NAM9 is not four")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x03\0\0\0\x01\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x05\0\0\0\x01\0\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read FRMR, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before NAM9 can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple NAM9s")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read FRMR, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }
  }
}
