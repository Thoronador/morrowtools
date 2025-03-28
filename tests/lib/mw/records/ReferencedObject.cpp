/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2022, 2023, 2025  Dirk Stolle

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
#include "../../../../lib/mw/records/ReferencedObject.hpp"

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
    REQUIRE_FALSE( object.DoorData.has_value() );
    // DoorData is basically checked in test for RefDoorData.
    REQUIRE_FALSE( object.LockLevel.has_value() );
    REQUIRE( object.KeyID.empty() );
    REQUIRE( object.TrapID.empty() );
    REQUIRE( object.OwnerID.empty() );
    REQUIRE( object.OwnerFactionID.empty() );
    REQUIRE( object.FactionRank == 0 );
    REQUIRE( object.GlobalVarID.empty() );
    REQUIRE( object.SoulCreatureID.empty() );
    REQUIRE_FALSE( object.EnchantCharge.has_value() );
    REQUIRE_FALSE( object.NumberOfUses.has_value() );
    REQUIRE_FALSE( object.UnknownNAM9.has_value() );
    REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
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
        a.DoorData.reset();
        b.DoorData = RefDoorData();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.DoorData = RefDoorData();
        a.DoorData.value().PosX = 123.45f;
        b.DoorData = RefDoorData();
        b.DoorData.value().PosX = 678.901f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("LockLevel mismatch")
      {
        a.LockLevel.reset();
        b.LockLevel = 10;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.LockLevel = 20;
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
        a.EnchantCharge.reset();
        b.EnchantCharge = 1.25f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.EnchantCharge = 20.0f;
        b.EnchantCharge = 50.0f;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("NumberOfUses mismatch")
      {
        a.NumberOfUses.reset();
        a.NumberOfUses = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.NumberOfUses = 1;
        b.NumberOfUses = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("NAM9 mismatch")
      {
        a.UnknownNAM9.reset();
        b.UnknownNAM9 = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.UnknownNAM9 = 1;
        b.UnknownNAM9 = 234;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ReferenceBlockedByte mismatch")
      {
        a.ReferenceBlockedByte.reset();
        b.ReferenceBlockedByte = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.ReferenceBlockedByte = 0;
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

  SECTION("getWrittenSize")
  {
    ReferencedObject object;
    object.ObjectIndex = 0x01234567;
    object.ObjectID = "foo";
    object.Scale = 1.0f;
    object.PosX = 1.0f;
    object.PosY = 2.0f;
    object.PosZ = 3.0f;
    object.RotX = 0.5f;
    object.RotY = 2.5f;
    object.RotZ = 4.5f;

    SECTION("default size")
    {
      REQUIRE( object.getWrittenSize() == 56 );
    }

    SECTION("size adjusts when reference is deleted")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.isDeleted = true;
      object.DeletionLong = 0;
      REQUIRE( object.getWrittenSize() == 36 );
    }

    SECTION("size adjusts when scale is not one")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.Scale = 2.5f;
      REQUIRE( object.getWrittenSize() == 68 );
    }

    SECTION("size adjusts when door data is present")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.DoorData = RefDoorData();
      REQUIRE( object.getWrittenSize() == 88 );

      SECTION("size adjusts with length of exit name")
      {
        REQUIRE( object.getWrittenSize() == 88 );

        object.DoorData.value().ExitName = "bar";
        REQUIRE( object.getWrittenSize() == 100 );

        object.DoorData.value().ExitName = "bar12345";
        REQUIRE( object.getWrittenSize() == 105 );
      }
    }

    SECTION("size adjusts when LockLevel is set")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.LockLevel = 25;
      REQUIRE( object.getWrittenSize() == 68 );

      SECTION("size adjusts with length of key ID")
      {
        REQUIRE( object.getWrittenSize() == 68 );

        object.KeyID = "bar";
        REQUIRE( object.getWrittenSize() == 80 );

        object.KeyID = "bar12345";
        REQUIRE( object.getWrittenSize() == 85 );
      }
    }

    SECTION("size adjusts with length of trap ID")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.TrapID = "trap";
      REQUIRE( object.getWrittenSize() == 69 );

      object.TrapID = "Trap1234";
      REQUIRE( object.getWrittenSize() == 73 );
    }

    SECTION("size adjusts with length of owner ID")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.OwnerID = "foo";
      REQUIRE( object.getWrittenSize() == 68 );

      object.OwnerID = "foo1234";
      REQUIRE( object.getWrittenSize() == 72 );
    }

    SECTION("size adjusts with length of owner faction ID")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.OwnerFactionID = "foo";
      REQUIRE( object.getWrittenSize() == 80 );

      object.OwnerFactionID = "foo1234";
      REQUIRE( object.getWrittenSize() == 84 );
    }

    SECTION("size adjusts with length of global variable ID")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.GlobalVarID = "foo";
      REQUIRE( object.getWrittenSize() == 68 );

      object.GlobalVarID = "foo1234";
      REQUIRE( object.getWrittenSize() == 72 );
    }

    SECTION("size adjusts with length of soul creature ID")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.SoulCreatureID = "foo";
      REQUIRE( object.getWrittenSize() == 68 );

      object.SoulCreatureID = "foo_creature";
      REQUIRE( object.getWrittenSize() == 77 );
    }

    SECTION("size adjusts when enchant charge is present")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.EnchantCharge = 25.0f;
      REQUIRE( object.getWrittenSize() == 68 );
    }

    SECTION("size adjusts when number of uses is present")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.NumberOfUses = 1234;
      REQUIRE( object.getWrittenSize() == 68 );
    }

    SECTION("size adjusts when NAM9 is present")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.UnknownNAM9 = 0x12345678;
      REQUIRE( object.getWrittenSize() == 68 );
    }

    SECTION("size adjusts when ReferenceBlocked is present")
    {
      REQUIRE( object.getWrittenSize() == 56 );

      object.ReferenceBlockedByte = 0x01;
      REQUIRE( object.getWrittenSize() == 65 );
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t bytesRead = 0;
    char buffer[256];

    SECTION("default: load minimalistic reference data")
    {
      const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE_FALSE( object.NumberOfUses.has_value() );
      REQUIRE_FALSE( object.UnknownNAM9.has_value() );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference data with scale multiplier")
    {
      const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0XSCL\x04\0\0\0\xA4p=?DATA\x18\0\0\0\xF8\x80\xAC\x41\xF0w\xEF\x44pu&\xC4\0\0\0\0\0\0\0\0\xFC\x97Y@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE_FALSE( object.NumberOfUses.has_value() );
      REQUIRE_FALSE( object.UnknownNAM9.has_value() );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with door data")
    {
      const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0DODT\x18\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`E@DATA\x18\0\0\0\0\0\x80\x38\0\0\xB5\x45\0\0\x84\xC3\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE( object.DoorData.has_value() );
      REQUIRE( object.DoorData.value().PosX == 45036.90625f );
      REQUIRE( object.DoorData.value().PosY == 126711.8828125f );
      REQUIRE( object.DoorData.value().PosZ == 822.48498535156f );
      REQUIRE( object.DoorData.value().RotX == 0.0f );
      REQUIRE( object.DoorData.value().RotY == 0.0f );
      REQUIRE( object.DoorData.value().RotZ == 3.0839967727661f );
      REQUIRE( object.DoorData.value().ExitName.empty() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE_FALSE( object.NumberOfUses.has_value() );
      REQUIRE_FALSE( object.UnknownNAM9.has_value() );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with door data and exit name")
    {
      const auto data = "FRMR\x04\0\0\0\x20\x19\x17\0NAME\x18\0\0\0in_de_shipdoor_toplevel\0DODT\x18\0\0\0\x06\x8F|ET\xE7PE\xE1\xCA\xD3\xC3\0\0\0\0\xDB\x0F\xC9?\0\0\0\0DNAM#\0\0\0Zerfallenes Schiffswrack, Oberdeck\0DATA\x18\0\0\0\x8FR\xF7\x44\xE3\x15RE\xDB\xA1\x37\xC4\0\0\0\0\xFC\x97\xD9?4`E@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE( object.DoorData.has_value() );
      REQUIRE( object.DoorData.value().PosX == 4040.9389648438f );
      REQUIRE( object.DoorData.value().PosY == 3342.4580078125f );
      REQUIRE( object.DoorData.value().PosZ == -423.58499145508f );
      REQUIRE( object.DoorData.value().RotX == 0.0f );
      REQUIRE( object.DoorData.value().RotY == 1.5707963705063f );
      REQUIRE( object.DoorData.value().RotZ == 0.0f );
      REQUIRE( object.DoorData.value().ExitName == "Zerfallenes Schiffswrack, Oberdeck" );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE_FALSE( object.NumberOfUses.has_value() );
      REQUIRE_FALSE( object.UnknownNAM9.has_value() );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with door data and exit name and owner and NAM9")
    {
      const auto data = "FRMR\x04\0\0\0\x5A\xD7\x02\0\x4E\x41\x4D\x45\x17\0\0\0ex_velothi_loaddoor_01\0ANAM\x11\0\0\0a shady smuggler\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DODT\x18\0\0\0\xC5\xF7\x0E\xC5\x44\x30\x62\x44o\xB4X\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?DNAM\x18\0\0\0Molag Mar, Bodensektion\0DATA\x18\0\0\0$C\xD3G\x9C\x01o\xC7+\xBB\xEB\x44\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x0002d75a );
      REQUIRE( object.ObjectID == "ex_velothi_loaddoor_01" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 108166.28125f );
      REQUIRE( object.PosY == -61185.609375f );
      REQUIRE( object.PosZ == 1885.8489990234f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 0.0f );
      REQUIRE( object.DoorData.has_value() );
      REQUIRE( object.DoorData.value().PosX == -2287.4855957031f );
      REQUIRE( object.DoorData.value().PosY == 904.75415039062f );
      REQUIRE( object.DoorData.value().PosZ == -866.81927490234f );
      REQUIRE( object.DoorData.value().RotX == 0.0f );
      REQUIRE( object.DoorData.value().RotY == 0.0f );
      REQUIRE( object.DoorData.value().RotZ == 1.5707963705063f );
      REQUIRE( object.DoorData.value().ExitName == "Molag Mar, Bodensektion" );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID == "a shady smuggler" );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE( object.NumberOfUses.has_value() );
      REQUIRE( object.NumberOfUses.value() == 0 );
      REQUIRE( object.UnknownNAM9.has_value() );
      REQUIRE( object.UnknownNAM9.value() == 1 );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with lock")
    {
      const auto data = "FRMR\x04\0\0\0\x13\x19\x17\0NAME\x16\0\0\0de_p_chest_02_gold_25\0FLTV\x04\0\0\0\x0F\0\0\0DATA\x18\0\0\0\x1B\xDB|E\x0Bx{E\x80t4\xC4\xFD\xB5\xC5@\xAA\x61\x9C@~\xFFQ@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE( object.LockLevel.has_value() );
      REQUIRE( object.LockLevel.value() == 15 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE_FALSE( object.NumberOfUses.has_value() );
      REQUIRE_FALSE( object.UnknownNAM9.has_value() );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with ANAM, NAM9, INTV")
    {
      const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID == "kaushad" );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE( object.NumberOfUses.has_value() );
      REQUIRE( object.NumberOfUses.value() == 1 );
      REQUIRE( object.UnknownNAM9.has_value() );
      REQUIRE( object.UnknownNAM9.value() == 0x00000001 );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with ANAM, INTV, NAM9, and FLTV")
    {
      const auto data = "FRMR\x04\0\0\0\x99\xAC\x06\0NAME\x18\0\0\0chest_01_v_potion_al_02\0ANAM\x09\0\0\0arangaer\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0FLTV\x04\0\0\0K\0\0\0DATA\x18\0\0\0\xD6^\x0FHd\x17\x1AG\xB0K\x1F\x44\0\0\0\0\0\0\0\0O\xB9+@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x0006AC99 );
      REQUIRE( object.ObjectID == "chest_01_v_potion_al_02" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 146811.34375f );
      REQUIRE( object.PosY == 39447.390625f );
      REQUIRE( object.PosZ == 637.1826171875f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 2.683185338974f );
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE( object.LockLevel.has_value() );
      REQUIRE( object.LockLevel.value() == 75 );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID == "arangaer" );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE( object.NumberOfUses.has_value() );
      REQUIRE( object.NumberOfUses.value() == 0 );
      REQUIRE( object.UnknownNAM9.has_value() );
      REQUIRE( object.UnknownNAM9.value() == 0x00000001 );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with key and trap")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0FLTV\x04\0\0\0P\0\0\0KNAM\x15\0\0\0key_Forge of Rolamus\0TNAM\x10\0\0\0trap_paralyze00\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x0016AC3D );
      REQUIRE( object.ObjectID == "com_chest_Daed_crusher" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 3033.8781738281f );
      REQUIRE( object.PosY == 6119.068359375f );
      REQUIRE( object.PosZ == -862.12957763672f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 1.5707963705063f );
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE( object.LockLevel.has_value() );
      REQUIRE( object.LockLevel.value() == 80 );
      REQUIRE( object.KeyID == "key_Forge of Rolamus" );
      REQUIRE( object.TrapID == "trap_paralyze00" );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE_FALSE( object.NumberOfUses.has_value() );
      REQUIRE_FALSE( object.UnknownNAM9.has_value() );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with owner faction")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\0\0\0Ashlanders\0INDX\x04\0\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00171417 );
      REQUIRE( object.ObjectID == "active_de_bedroll" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == -96.384521484375f );
      REQUIRE( object.PosY == 672.50476074219f );
      REQUIRE( object.PosZ == -248.57118225098f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 2.8831877708435f );
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID == "Ashlanders" );
      REQUIRE( object.FactionRank == 0 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE( object.NumberOfUses.has_value() );
      REQUIRE( object.NumberOfUses.value() == 1 );
      REQUIRE( object.UnknownNAM9.has_value() );
      REQUIRE( object.UnknownNAM9.value() == 0x00000001 );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with global variable")
    {
      const auto data = "FRMR\x04\0\0\0\x86\xFE\x16\0NAME\x12\0\0\0active_com_bed_07\0ANAM\x0C\0\0\0raril giral\0BNAM\x11\0\0\0Rent_Vivec_Black\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0%\x14\xCF\x43\xFE\xA3\xBB\xC4\xAE\xD7\x8F\xC3\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x0016FE86 );
      REQUIRE( object.ObjectID == "active_com_bed_07" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 414.15737915039f );
      REQUIRE( object.PosY == -1501.1247558594f );
      REQUIRE( object.PosZ == -287.68499755859f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 1.5707963705063f );
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID == "raril giral" );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID == "Rent_Vivec_Black" );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE( object.NumberOfUses.has_value() );
      REQUIRE( object.NumberOfUses.value() == 0 );
      REQUIRE( object.UnknownNAM9.has_value() );
      REQUIRE( object.UnknownNAM9.value() == 0x00000001 );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with soul creature ID")
    {
      const auto data = "FRMR\x04\0\0\0\x4D\x97\x16\0NAME\x14\0\0\0Misc_SoulGem_Common\0XSOL\x06\0\0\0scamp\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xA7\xDC\x98\x45\x90\x1F\xE1\x45\xEA\xD6\x46\x46\x43v\xAF@\x0F\x43\xBC@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x0016974D );
      REQUIRE( object.ObjectID == "Misc_SoulGem_Common" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 4891.5815429688f );
      REQUIRE( object.PosY == 7203.9453125f );
      REQUIRE( object.PosZ == 12725.728515625f );
      REQUIRE( object.RotX == 5.4831862449646f );
      REQUIRE( object.RotY == 5.8831858634949f );
      REQUIRE( object.RotZ == 0.0f );
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID == "scamp" );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE( object.NumberOfUses.has_value() );
      REQUIRE( object.NumberOfUses.value() == 0 );
      REQUIRE( object.UnknownNAM9.has_value() );
      REQUIRE( object.UnknownNAM9.value() == 0x00000001 );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with enchantment charge")
    {
      const auto data = "FRMR\x04\0\0\0\xBC\x0E\x17\0NAME\x11\0\0\0iron shardmauler\0ANAM\x0F\0\0\0trilam drolnor\0XCHG\x04\0\0\0\0\0 AINTV\x04\0\0\0\xD0\x07\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0}J\xF9\x42\x9E\x9F\xA8\xC2\x8E\x1Dk\xC2\0\0\0\0\x99\xCC\xCC=t\xA9\xD2?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00170EBC );
      REQUIRE( object.ObjectID == "iron shardmauler" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == 124.64548492432f );
      REQUIRE( object.PosY == -84.311752319336f );
      REQUIRE( object.PosZ == -58.778861999512f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.099999614059925f );
      REQUIRE( object.RotZ == 1.6457962989807f );
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID == "trilam drolnor" );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE( object.EnchantCharge.has_value() );
      REQUIRE( object.EnchantCharge.value() == 10.0f );
      REQUIRE( object.NumberOfUses.has_value() );
      REQUIRE( object.NumberOfUses.value() == 2000 );
      REQUIRE( object.UnknownNAM9.has_value() );
      REQUIRE( object.UnknownNAM9.value() == 0x00000001 );
      REQUIRE_FALSE( object.ReferenceBlockedByte.has_value() );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("default: load reference with UNAM")
    {
      const auto data = "FRMR\x04\0\0\0\x26\x35\x15\0NAME\x13\0\0\0active_de_p_bed_04\0UNAM\x01\0\0\0\0DATA\x18\0\0\0\xAE\xF1\x19\xC3H\xA1\xEB\xC1`H\xC9\x42\0\0\0\0\0\0\0\0\xB1\x98\x93@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      ReferencedObject object;
      REQUIRE( object.loadFromStream(stream, bytesRead, buffer) );
      // Check data.
      REQUIRE( object.ObjectIndex == 0x00153526 );
      REQUIRE( object.ObjectID == "active_de_p_bed_04" );
      REQUIRE( object.Scale == 1.0f );
      REQUIRE( object.PosX == -153.9440612793f );
      REQUIRE( object.PosY == -29.453750610352f );
      REQUIRE( object.PosZ == 100.64135742188f );
      REQUIRE( object.RotX == 0.0f );
      REQUIRE( object.RotY == 0.0f );
      REQUIRE( object.RotZ == 4.612389087677f );
      REQUIRE_FALSE( object.DoorData.has_value() );
      REQUIRE_FALSE( object.LockLevel.has_value() );
      REQUIRE( object.KeyID.empty() );
      REQUIRE( object.TrapID.empty() );
      REQUIRE( object.OwnerID.empty() );
      REQUIRE( object.OwnerFactionID.empty() );
      REQUIRE( object.FactionRank == -1 );
      REQUIRE( object.GlobalVarID.empty() );
      REQUIRE( object.SoulCreatureID.empty() );
      REQUIRE_FALSE( object.EnchantCharge.has_value() );
      REQUIRE_FALSE( object.NumberOfUses.has_value() );
      REQUIRE_FALSE( object.UnknownNAM9.has_value() );
      REQUIRE( object.ReferenceBlockedByte.has_value() );
      REQUIRE( object.ReferenceBlockedByte.value() == 0 );
      REQUIRE_FALSE( object.isDeleted );
      REQUIRE( object.DeletionLong == 0 );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( object.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
      // Check size and predicted size.
      REQUIRE( data.size() == object.getWrittenSize() );
    }

    SECTION("corrupt data: length of FRMR is not four")
    {
      {
        const auto data = "FRMR\x03\0\0\0\x78\x19\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x05\0\0\0\x78\x19\x17\0\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x18\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x78\x19\x17\0NAME\x17\0\0\0bookskill_heavy armor5\0DATA\x19\0\0\0\x89\xC6\x14\x41\xA7\xA2\xBF\x44\xB6Pp\xC4\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x41\x19\x17\0NAME\x17\0\0\0ex_dae_malacath_attack\0XSCL\x05\0\0\0\xA4p=?\0DATA\x18\0\0\0\xF8\x80\xAC\x41\xF0w\xEF\x44pu&\xC4\0\0\0\0\0\0\0\0\xFC\x97Y@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x2C\x19\x17\0NAME\x16\0\0\0ex_dae_door_load_oval\0DODT\x19\0\0\0\xE8\xEC/G\xF1{\xF7G\x0A\x9FMD\0\0\0\0\0\0\0\0\x34`E@\0DATA\x18\0\0\0\0\0\x80\x38\0\0\xB5\x45\0\0\x84\xC3\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x13\x19\x17\0NAME\x16\0\0\0de_p_chest_02_gold_25\0FLTV\x05\0\0\0\x0F\0\0\0\0DATA\x18\0\0\0\x1B\xDB|E\x0Bx{E\x80t4\xC4\xFD\xB5\xC5@\xAA\x61\x9C@~\xFFQ@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: ANAM is present, but it already has CNAM")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\0\0\0Ashlanders\0INDX\x04\0\0\0\0\0\0\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x05\0\0\0\x01\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x94\x15\x17\0NAME\x12\0\0\0misc_de_goblet_07\0ANAM\x08\0\0\0kaushad\0INTV\x04\0\0\0\x01\0\0\0NAM9\x05\0\0\0\x01\0\0\0\0DATA\x18\0\0\0t\xED\xD4\xC0\x1E\xAF\x1C\x44\xEB\x10\x44\xC3\0\0\0\0\0\0\0\0\x12\x43\x8C@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
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

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: KNAM without previous FLTV")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0KNAM\x15\0\0\0key_Forge of Rolamus\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple KNAMs")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0FLTV\x04\0\0\0P\0\0\0KNAM\x15\0\0\0key_Forge of Rolamus\0KNAM\x15\0\0\0key_Forge of Rolamus\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of KNAM > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0FLTV\x04\0\0\0P\0\0\0KNAM\x15\x01\0\0key_Forge of Rolamus\0TNAM\x10\0\0\0trap_paralyze00\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of KNAM is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0FLTV\x04\0\0\0P\0\0\0KNAM\x95\0\0\0key_Forge of Rolamus\0TNAM\x10\0\0\0trap_paralyze00\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple TNAMs")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0FLTV\x04\0\0\0P\0\0\0KNAM\x15\0\0\0key_Forge of Rolamus\0TNAM\x10\0\0\0trap_paralyze00\0TNAM\x10\0\0\0trap_paralyze00\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of TNAM > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0FLTV\x04\0\0\0P\0\0\0KNAM\x15\0\0\0key_Forge of Rolamus\0TNAM\x10\x01\0\0trap_paralyze00\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of TNAM is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x3D\xAC\x16\0NAME\x17\0\0\0com_chest_Daed_crusher\0FLTV\x04\0\0\0P\0\0\0KNAM\x15\0\0\0key_Forge of Rolamus\0TNAM\x90\0\0\0trap_paralyze00\0DATA\x18\0\0\0\x0D\x9E=E\x8C\x38\xBF\x45K\x88W\xC4\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple CNAMs")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\0\0\0Ashlanders\0INDX\x04\0\0\0\0\0\0\0CNAM\x0B\0\0\0Ashlanders\0INDX\x04\0\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: CNAM is present, but it already has ANAM")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0ANAM\x08\0\0\0kaushad\0CNAM\x0B\0\0\0Ashlanders\0INDX\x04\0\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of CNAM > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\x01\0\0Ashlanders\0INDX\x04\0\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of CNAM is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x9B\0\0\0Ashlanders\0INDX\x04\0\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: CNAM is not followed by INDX")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\0\0\0Ashlanders\0FAIL\x04\0\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of INDX is not four")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\0\0\0Ashlanders\0INDX\x03\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\0\0\0Ashlanders\0INDX\x05\0\0\0\0\0\0\0\0INTV\x04\0\0\0\x01\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xE0\xC4\xC0\xC2N (D9\x92x\xC3\0\0\0\0\0\0\0\0&\x86\x38@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before INDX can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x17\x14\x17\0NAME\x12\0\0\0active_de_bedroll\0CNAM\x0B\0\0\0Ashlanders\0INDX\x04\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple BNAMs")
    {
      const auto data = "FRMR\x04\0\0\0\x86\xFE\x16\0NAME\x12\0\0\0active_com_bed_07\0ANAM\x0C\0\0\0raril giral\0BNAM\x11\0\0\0Rent_Vivec_Black\0BNAM\x11\0\0\0Rent_Vivec_Black\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0%\x14\xCF\x43\xFE\xA3\xBB\xC4\xAE\xD7\x8F\xC3\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of BNAM > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x86\xFE\x16\0NAME\x12\0\0\0active_com_bed_07\0ANAM\x0C\0\0\0raril giral\0BNAM\x11\x01\0\0Rent_Vivec_Black\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0%\x14\xCF\x43\xFE\xA3\xBB\xC4\xAE\xD7\x8F\xC3\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of BNAM is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x86\xFE\x16\0NAME\x12\0\0\0active_com_bed_07\0ANAM\x0C\0\0\0raril giral\0BNAM\x91\0\0\0Rent_Vivec_Black\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0%\x14\xCF\x43\xFE\xA3\xBB\xC4\xAE\xD7\x8F\xC3\0\0\0\0\0\0\0\0\xDB\x0F\xC9?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple XSOLs")
    {
      const auto data = "FRMR\x04\0\0\0\x4D\x97\x16\0\x4E\x41\x4D\x45\x14\0\0\0Misc_SoulGem_Common\0XSOL\x06\0\0\0scamp\0XSOL\x06\0\0\0scamp\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xA7\xDC\x98\x45\x90\x1F\xE1\x45\xEA\xD6\x46\x46\x43v\xAF@\x0F\x43\xBC@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of XSOL > 256")
    {
      const auto data = "FRMR\x04\0\0\0\x4D\x97\x16\0\x4E\x41\x4D\x45\x14\0\0\0Misc_SoulGem_Common\0XSOL\x06\x01\0\0scamp\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xA7\xDC\x98\x45\x90\x1F\xE1\x45\xEA\xD6\x46\x46\x43v\xAF@\x0F\x43\xBC@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of XSOL is beyond stream")
    {
      const auto data = "FRMR\x04\0\0\0\x4D\x97\x16\0\x4E\x41\x4D\x45\x14\0\0\0Misc_SoulGem_Common\0XSOL\x96\0\0\0scamp\0INTV\x04\0\0\0\0\0\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0\xA7\xDC\x98\x45\x90\x1F\xE1\x45\xEA\xD6\x46\x46\x43v\xAF@\x0F\x43\xBC@\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple XCHGs")
    {
      const auto data = "FRMR\x04\0\0\0\xBC\x0E\x17\0NAME\x11\0\0\0iron shardmauler\0ANAM\x0F\0\0\0trilam drolnor\0XCHG\x04\0\0\0\0\0 AXCHG\x04\0\0\0\0\0 AINTV\x04\0\0\0\xD0\x07\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0}J\xF9\x42\x9E\x9F\xA8\xC2\x8E\x1Dk\xC2\0\0\0\0\x99\xCC\xCC=t\xA9\xD2?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of XCHG is not four")
    {
      {
        const auto data = "FRMR\x04\0\0\0\xBC\x0E\x17\0NAME\x11\0\0\0iron shardmauler\0ANAM\x0F\0\0\0trilam drolnor\0XCHG\x03\0\0\0\0\0 INTV\x04\0\0\0\xD0\x07\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0}J\xF9\x42\x9E\x9F\xA8\xC2\x8E\x1Dk\xC2\0\0\0\0\x99\xCC\xCC=t\xA9\xD2?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\xBC\x0E\x17\0NAME\x11\0\0\0iron shardmauler\0ANAM\x0F\0\0\0trilam drolnor\0XCHG\x05\0\0\0\0\0 ABINTV\x04\0\0\0\xD0\x07\0\0NAM9\x04\0\0\0\x01\0\0\0DATA\x18\0\0\0}J\xF9\x42\x9E\x9F\xA8\xC2\x8E\x1Dk\xC2\0\0\0\0\x99\xCC\xCC=t\xA9\xD2?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before XCHG can be read")
    {
      const auto data = "FRMR\x04\0\0\0\xBC\x0E\x17\0NAME\x11\0\0\0iron shardmauler\0ANAM\x0F\0\0\0trilam drolnor\0XCHG\x04\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: multiple UNAMs")
    {
      const auto data = "FRMR\x04\0\0\0\x26\x35\x15\0NAME\x13\0\0\0active_de_p_bed_04\0UNAM\x01\0\0\0\0UNAM\x01\0\0\0\0DATA\x18\0\0\0\xAE\xF1\x19\xC3H\xA1\xEB\xC1`H\xC9\x42\0\0\0\0\0\0\0\0\xB1\x98\x93@"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }

    SECTION("corrupt data: length of UNAM is not one")
    {
      {
        const auto data = "FRMR\x04\0\0\0\x26\x35\x15\0NAME\x13\0\0\0active_de_p_bed_04\0UNAM\0\0\0\0DATA\x18\0\0\0\xAE\xF1\x19\xC3H\xA1\xEB\xC1`H\xC9\x42\0\0\0\0\0\0\0\0\xB1\x98\x93@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }

      {
        const auto data = "FRMR\x04\0\0\0\x26\x35\x15\0NAME\x13\0\0\0active_de_p_bed_04\0UNAM\x02\0\0\0\0\0DATA\x18\0\0\0\xAE\xF1\x19\xC3H\xA1\xEB\xC1`H\xC9\x42\0\0\0\0\0\0\0\0\xB1\x98\x93@"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip FRMR, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        ReferencedObject object;
        REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
      }
    }

    SECTION("corrupt data: stream ends before UNAM can be read")
    {
      const auto data = "FRMR\x04\0\0\0\x26\x35\x15\0NAME\x13\0\0\0active_de_p_bed_04\0UNAM\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip FRMR, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      ReferencedObject object;
      REQUIRE_FALSE( object.loadFromStream(stream, bytesRead, buffer) );
    }
  }
}
