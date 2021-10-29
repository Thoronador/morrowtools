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

    REQUIRE_FALSE( entry.unknownALST.has_value() );
    REQUIRE_FALSE( entry.unknownALLS.has_value() );
    REQUIRE( entry.aliasID.empty() );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.unknownALFA.has_value() );
    REQUIRE( entry.locationRefTypeFormID == 0 );
    REQUIRE( entry.createReferenceToObjectFormID == 0 );
    REQUIRE_FALSE( entry.unknownALCA.has_value() );
    REQUIRE_FALSE( entry.unknownALCL.has_value() );
    REQUIRE( entry.displayNameFormID == 0 );
    REQUIRE( entry.components.empty() );
    REQUIRE( entry.keywordArray.empty() );
    REQUIRE_FALSE( entry.unknownALFE.has_value() );
    REQUIRE_FALSE( entry.unknownALFD.has_value() );
    REQUIRE_FALSE( entry.forcedIntoAliasID.has_value() );
    REQUIRE( entry.specificLocationFormID == 0 );
    REQUIRE( entry.specificReferenceID == 0 );
    REQUIRE_FALSE( entry.unknownALNA.has_value() );
    REQUIRE_FALSE( entry.unknownALNT.has_value() );
    REQUIRE( entry.uniqueActorFormID == 0 );
    REQUIRE( entry.externalAliasReferenceFormID == 0 );
    REQUIRE_FALSE( entry.unknownALEA.has_value() );
    REQUIRE( entry.keywordFormID == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
    REQUIRE( entry.spectatorOverridePackageListFormID == 0 );
    REQUIRE( entry.combatOverridePackageListFormID == 0 );
    REQUIRE( entry.spellFormIDs.empty() );
    REQUIRE( entry.factionFormIDs.empty() );
    REQUIRE( entry.packageDataFormIDs.empty() );
    REQUIRE_FALSE( entry.unknownVTCK.has_value() );
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
        a.unknownALST = 0;
        b.unknownALST.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALST.reset();
        b.unknownALST = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALST = 1;
        b.unknownALST = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALLS mismatch")
      {
        a.unknownALLS = 0;
        b.unknownALLS.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALLS.reset();
        b.unknownALLS = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALLS = 1;
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
        a.unknownALFA = 0;
        b.unknownALFA.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALFA.reset();
        b.unknownALFA = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALFA = 1;
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
        a.unknownALCA = 0;
        b.unknownALCA.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALCA.reset();
        b.unknownALCA = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALCA = 1;
        b.unknownALCA = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALCL mismatch")
      {
        a.unknownALCL = 0;
        b.unknownALCL.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALCL.reset();
        b.unknownALCL = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALCL = 1;
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
        a.unknownALFE = 0;
        b.unknownALFE.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALFE.reset();
        b.unknownALFE = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALFE = 1;
        b.unknownALFE = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALFD mismatch")
      {
        a.unknownALFD = 0;
        b.unknownALFD.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALFD.reset();
        b.unknownALFD = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALFD = 1;
        b.unknownALFD = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("forcedIntoAliasID mismatch")
      {
        a.forcedIntoAliasID = 1;
        b.forcedIntoAliasID.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.forcedIntoAliasID.reset();
        b.forcedIntoAliasID = 1;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.forcedIntoAliasID = 1;
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
        a.unknownALNA = 0;
        b.unknownALNA.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALNA.reset();
        b.unknownALNA = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALNA = 1;
        b.unknownALNA = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("ALNT mismatch")
      {
        a.unknownALNT = 0;
        b.unknownALNT.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALNT.reset();
        b.unknownALNT = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALNT = 1;
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
        a.unknownALEA = 1;
        b.unknownALEA.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALEA.reset();
        b.unknownALEA = 1;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownALEA = 1;
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
        a.unknownVTCK = 0;
        b.unknownVTCK.reset();

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownVTCK.reset();
        b.unknownVTCK = 0;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );

        a.unknownVTCK = 1;
        b.unknownVTCK = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }
    }
  }

  SECTION("clear")
  {
    AliasEntry entry;

    entry.unknownALST = 1;
    entry.unknownALLS = 1;
    entry.aliasID = "foo";
    entry.unknownFNAM = 1;
    entry.unknownALFA = 1;
    entry.locationRefTypeFormID = 1;
    entry.createReferenceToObjectFormID = 1;
    entry.unknownALCA = 1;
    entry.unknownALCL = 1;
    entry.displayNameFormID = 1;
    entry.components = { ComponentData(), ComponentData() };
    entry.keywordArray = { 0x01234567 };
    entry.unknownALFE = 1;
    entry.unknownALFD = 1;
    entry.forcedIntoAliasID = 1;
    entry.specificLocationFormID = 1;
    entry.specificReferenceID = 1;
    entry.unknownALNA = 1;
    entry.unknownALNT = 1;
    entry.uniqueActorFormID = 1;
    entry.externalAliasReferenceFormID = 1;
    entry.unknownALEA = 1;
    entry.keywordFormID = 1;
    entry.unknownCTDA_CIS2s = { CTDA_CIS2_compound() };
    entry.spectatorOverridePackageListFormID = 1;
    entry.combatOverridePackageListFormID = 1;
    entry.spellFormIDs = { 0x01234567 };
    entry.factionFormIDs = { 0x01234567 };
    entry.packageDataFormIDs = { 0x01234567 };
    entry.unknownVTCK = 1;

    entry.clear();

    REQUIRE_FALSE( entry.unknownALST.has_value() );
    REQUIRE_FALSE( entry.unknownALLS.has_value() );
    REQUIRE( entry.aliasID.empty() );
    REQUIRE( entry.unknownFNAM == 0 );
    REQUIRE_FALSE( entry.unknownALFA.has_value() );
    REQUIRE( entry.locationRefTypeFormID == 0 );
    REQUIRE( entry.createReferenceToObjectFormID == 0 );
    REQUIRE_FALSE( entry.unknownALCA.has_value() );
    REQUIRE_FALSE( entry.unknownALCL.has_value() );
    REQUIRE( entry.displayNameFormID == 0 );
    REQUIRE( entry.components.empty() );
    REQUIRE( entry.keywordArray.empty() );
    REQUIRE_FALSE( entry.unknownALFE.has_value() );
    REQUIRE_FALSE( entry.unknownALFD.has_value() );
    REQUIRE_FALSE( entry.forcedIntoAliasID.has_value() );
    REQUIRE( entry.specificLocationFormID == 0 );
    REQUIRE( entry.specificReferenceID == 0 );
    REQUIRE_FALSE( entry.unknownALNA.has_value() );
    REQUIRE_FALSE( entry.unknownALNT.has_value() );
    REQUIRE( entry.uniqueActorFormID == 0 );
    REQUIRE( entry.externalAliasReferenceFormID == 0 );
    REQUIRE_FALSE( entry.unknownALEA.has_value() );
    REQUIRE( entry.keywordFormID == 0 );
    REQUIRE( entry.unknownCTDA_CIS2s.empty() );
    REQUIRE( entry.spectatorOverridePackageListFormID == 0 );
    REQUIRE( entry.combatOverridePackageListFormID == 0 );
    REQUIRE( entry.spellFormIDs.empty() );
    REQUIRE( entry.factionFormIDs.empty() );
    REQUIRE( entry.packageDataFormIDs.empty() );
    REQUIRE_FALSE( entry.unknownVTCK.has_value() );
  }
}
