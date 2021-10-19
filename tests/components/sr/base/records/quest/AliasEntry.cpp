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
#include "../../../../../../sr/base/records/quest/AliasEntry.hpp"

TEST_CASE("AliasEntry")
{
  using namespace SRTP;

  SECTION("constructor")
  {
    AliasEntry entry;

    REQUIRE_FALSE( entry.hasALST );
    REQUIRE( entry.unknownALST == 0 );
    REQUIRE_FALSE( entry.hasALLS );
    REQUIRE( entry.unknownALLS == 0 );
    REQUIRE( entry.aliasID.empty() );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.hasALFA );
    REQUIRE( entry.unknownALFA == 0 );
    REQUIRE( entry.locationRefTypeFormID == 0 );
    REQUIRE( entry.createReferenceToObjectFormID == 0 );
    REQUIRE_FALSE( entry.hasALCA );
    REQUIRE( entry.unknownALCA == 0 );
    REQUIRE_FALSE( entry.hasALCL );
    REQUIRE( entry.unknownALCL == 0 );
    REQUIRE( entry.displayNameFormID == 0 );
    REQUIRE( entry.components.empty() );
    REQUIRE( entry.keywordArray.empty() );
    REQUIRE_FALSE( entry.hasALFE );
    REQUIRE( entry.unknownALFE == 0 );
    REQUIRE_FALSE( entry.hasALFD );
    REQUIRE( entry.unknownALFD == 0 );
    REQUIRE_FALSE( entry.hasALFI );
    REQUIRE( entry.forcedIntoAliasID == 0 );
    REQUIRE( entry.specificLocationFormID == 0 );
    REQUIRE( entry.specificReferenceID == 0 );
    REQUIRE_FALSE( entry.hasALNA );
    REQUIRE( entry.unknownALNA == 0 );
    REQUIRE_FALSE( entry.hasALNT );
    REQUIRE( entry.unknownALNT == 0 );
    REQUIRE( entry.uniqueActorFormID == 0 );
    REQUIRE( entry.externalAliasReferenceFormID == 0 );
    REQUIRE_FALSE( entry.hasALEA );
    REQUIRE( entry.unknownALEA == 0 );
    REQUIRE( entry.keywordFormID == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
    REQUIRE( entry.spectatorOverridePackageListFormID == 0 );
    REQUIRE( entry.combatOverridePackageListFormID == 0 );
    REQUIRE( entry.spellFormIDs.empty() );
    REQUIRE( entry.factionFormIDs.empty() );
    REQUIRE( entry.packageDataFormIDs.empty() );
    REQUIRE_FALSE( entry.hasVTCK );
    REQUIRE( entry.unknownVTCK == 0 );
  }

  SECTION("equality")
  {
    AliasEntry a;
    AliasEntry b;

    SECTION("equal")
    {
      a.aliasID = "foo";
      b.aliasID = "foo";

      REQUIRE( a == b );
      REQUIRE( b == a );
    }

    SECTION("unequal")
    {
      SECTION("ALST mismatch")
      {
        a.hasALST = true;
        b.hasALST = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALST = false;
        b.hasALST = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALST = true;
        a.unknownALST = 1;
        b.hasALST = true;
        b.unknownALST = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALLS mismatch")
      {
        a.hasALLS = true;
        b.hasALLS = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALLS = false;
        b.hasALLS = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALLS = true;
        a.unknownALLS = 1;
        b.hasALLS = true;
        b.unknownALLS = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("aliasID mismatch")
      {
        a.aliasID = "foo";
        b.aliasID = "bar";

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("FNAM mismatch")
      {
        a.unknownFNAM = 1;
        a.unknownFNAM = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALFA mismatch")
      {
        a.hasALFA = true;
        b.hasALFA = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFA = false;
        b.hasALFA = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFA = true;
        a.unknownALFA = 1;
        b.hasALFA = true;
        b.unknownALFA = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("locationRefTypeFormID mismatch")
      {
        a.locationRefTypeFormID = 1;
        b.locationRefTypeFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("createReferenceToObjectFormID mismatch")
      {
        a.createReferenceToObjectFormID = 1;
        b.createReferenceToObjectFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALCA mismatch")
      {
        a.hasALCA = true;
        b.hasALCA = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALCA = false;
        b.hasALCA = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALCA = true;
        a.unknownALCA = 1;
        b.hasALCA = true;
        b.unknownALCA = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALCL mismatch")
      {
        a.hasALCL = true;
        b.hasALCL = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALCL = false;
        b.hasALCL = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALCL = true;
        a.unknownALCL = 1;
        b.hasALCL = true;
        b.unknownALCL = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("displayNameFormID mismatch")
      {
        a.displayNameFormID = 1;
        b.displayNameFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("components mismatch")
      {
        a.components.push_back(ComponentData());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("keywords mismatch")
      {
        a.keywordArray.push_back(0x01234567);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALFE mismatch")
      {
        a.hasALFE = true;
        b.hasALFE = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFE = false;
        b.hasALFE = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFE = true;
        a.unknownALFE = 1;
        b.hasALFE = true;
        b.unknownALFE = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALFD mismatch")
      {
        a.hasALFD = true;
        b.hasALFD = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFD = false;
        b.hasALFD = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFD = true;
        a.unknownALFD = 1;
        b.hasALFD = true;
        b.unknownALFD = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("forcedIntoAliasID mismatch")
      {
        a.hasALFI = true;
        b.hasALFI = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFI = false;
        b.hasALFI = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALFI = true;
        a.forcedIntoAliasID = 1;
        b.hasALFI = true;
        b.forcedIntoAliasID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("specificLocationFormID mismatch")
      {
        a.specificLocationFormID = 1;
        b.specificLocationFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("specificReferenceID mismatch")
      {
        a.specificReferenceID = 1;
        b.specificReferenceID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALNA mismatch")
      {
        a.hasALNA = true;
        b.hasALNA = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALNA = false;
        b.hasALNA = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALNA = true;
        a.unknownALNA = 1;
        b.hasALNA = true;
        b.unknownALNA = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALNT mismatch")
      {
        a.hasALNT = true;
        b.hasALNT = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALNT = false;
        b.hasALNT = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALNT = true;
        a.unknownALNT = 1;
        b.hasALNT = true;
        b.unknownALNT = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("uniqueActorFormID mismatch")
      {
        a.uniqueActorFormID = 1;
        b.uniqueActorFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("externalAliasReferenceFormID mismatch")
      {
        a.externalAliasReferenceFormID = 1;
        b.externalAliasReferenceFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALEA mismatch")
      {
        a.hasALEA = true;
        b.hasALEA = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALEA = false;
        b.hasALEA = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasALEA = true;
        a.unknownALEA = 1;
        b.hasALEA = true;
        b.unknownALEA = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("keywordFormID mismatch")
      {
        a.keywordFormID = 1;
        b.keywordFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("CTDA/CIS2 mismatch")
      {
        a.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("spectatorOverridePackageListFormID mismatch")
      {
        a.spectatorOverridePackageListFormID = 1;
        b.spectatorOverridePackageListFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("combatOverridePackageListFormID mismatch")
      {
        a.combatOverridePackageListFormID = 1;
        b.combatOverridePackageListFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("spellFormIDs mismatch")
      {
        a.spellFormIDs.push_back(0x01234567);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("factionFormIDs mismatch")
      {
        a.factionFormIDs.push_back(0x01234567);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("packageDataFormIDs mismatch")
      {
        a.packageDataFormIDs.push_back(0x01234567);

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("VTCK mismatch")
      {
        a.hasVTCK = true;
        b.hasVTCK = false;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasVTCK = false;
        b.hasVTCK = true;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.hasVTCK = true;
        a.unknownVTCK = 1;
        b.hasVTCK = true;
        b.unknownVTCK = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("clear")
  {
    AliasEntry entry;

    entry.hasALST = true;
    entry.unknownALST = 1;
    entry.hasALLS = true;
    entry.unknownALLS = 1;
    entry.aliasID = "foo";
    entry.unknownFNAM = 1;
    entry.hasALFA = true;
    entry.unknownALFA = 1;
    entry.locationRefTypeFormID = 1;
    entry.createReferenceToObjectFormID = 1;
    entry.hasALCA = true;
    entry.unknownALCA = 1;
    entry.hasALCL = true;
    entry.unknownALCL = 1;
    entry.displayNameFormID = 1;
    entry.components = { ComponentData(), ComponentData() };
    entry.keywordArray = { 0x01234567 };
    entry.hasALFE = true;
    entry.unknownALFE = 1;
    entry.hasALFD = true;
    entry.unknownALFD = 1;
    entry.hasALFI = true;
    entry.forcedIntoAliasID = 1;
    entry.specificLocationFormID = 1;
    entry.specificReferenceID = 1;
    entry.hasALNA = true;
    entry.unknownALNA = 1;
    entry.hasALNT = true;
    entry.unknownALNT = 1;
    entry.uniqueActorFormID = 1;
    entry.externalAliasReferenceFormID = 1;
    entry.hasALEA = true;
    entry.unknownALEA = 1;
    entry.keywordFormID = 1;
    entry.unknownCTDA_CIS2s = { CTDA_CIS2_compound() };
    entry.spectatorOverridePackageListFormID = 1;
    entry.combatOverridePackageListFormID = 1;
    entry.spellFormIDs = { 0x01234567 };
    entry.factionFormIDs = { 0x01234567 };
    entry.packageDataFormIDs = { 0x01234567 };
    entry.hasVTCK = true;
    entry.unknownVTCK = 1;

    entry.clear();

    REQUIRE_FALSE( entry.hasALST );
    REQUIRE( entry.unknownALST == 0 );
    REQUIRE_FALSE( entry.hasALLS );
    REQUIRE( entry.unknownALLS == 0 );
    REQUIRE( entry.aliasID.empty() );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.hasALFA );
    REQUIRE( entry.unknownALFA == 0 );
    REQUIRE( entry.locationRefTypeFormID == 0 );
    REQUIRE( entry.createReferenceToObjectFormID == 0 );
    REQUIRE_FALSE( entry.hasALCA );
    REQUIRE( entry.unknownALCA == 0 );
    REQUIRE_FALSE( entry.hasALCL );
    REQUIRE( entry.unknownALCL == 0 );
    REQUIRE( entry.displayNameFormID == 0 );
    REQUIRE( entry.components.empty() );
    REQUIRE( entry.keywordArray.empty() );
    REQUIRE_FALSE( entry.hasALFE );
    REQUIRE( entry.unknownALFE == 0 );
    REQUIRE_FALSE( entry.hasALFD );
    REQUIRE( entry.unknownALFD == 0 );
    REQUIRE_FALSE( entry.hasALFI );
    REQUIRE( entry.forcedIntoAliasID == 0 );
    REQUIRE( entry.specificLocationFormID == 0 );
    REQUIRE( entry.specificReferenceID == 0 );
    REQUIRE_FALSE( entry.hasALNA );
    REQUIRE( entry.unknownALNA == 0 );
    REQUIRE_FALSE( entry.hasALNT );
    REQUIRE( entry.unknownALNT == 0 );
    REQUIRE( entry.uniqueActorFormID == 0 );
    REQUIRE( entry.externalAliasReferenceFormID == 0 );
    REQUIRE_FALSE( entry.hasALEA );
    REQUIRE( entry.unknownALEA == 0 );
    REQUIRE( entry.keywordFormID == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
    REQUIRE( entry.spectatorOverridePackageListFormID == 0 );
    REQUIRE( entry.combatOverridePackageListFormID == 0 );
    REQUIRE( entry.spellFormIDs.empty() );
    REQUIRE( entry.factionFormIDs.empty() );
    REQUIRE( entry.packageDataFormIDs.empty() );
    REQUIRE_FALSE( entry.hasVTCK );
    REQUIRE( entry.unknownVTCK == 0 );
  }
}
