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
#include <sstream>
#include <string_view>

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
    REQUIRE( entry.keywords.empty() );
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
    REQUIRE( entry.observeDeadBodyOverridePackageListFormID == 0 );
    REQUIRE( entry.guardWarnOverridePackageListFormID == 0 );
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
        a.keywords.push_back(0x01234567);

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

      SECTION("observeDeadBodyOverridePackageListFormID mismatch")
      {
        a.observeDeadBodyOverridePackageListFormID = 1;
        b.observeDeadBodyOverridePackageListFormID = 2;

        REQUIRE_FALSE( a == b );
        REQUIRE_FALSE( b == a );
      }

      SECTION("guardWarnOverridePackageListFormID mismatch")
      {
        a.guardWarnOverridePackageListFormID = 1;
        b.guardWarnOverridePackageListFormID = 2;

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
    entry.keywords = { 0x01234567 };
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
    entry.observeDeadBodyOverridePackageListFormID = 1;
    entry.guardWarnOverridePackageListFormID = 1;
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
    REQUIRE( entry.keywords.empty() );
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
    REQUIRE( entry.observeDeadBodyOverridePackageListFormID == 0 );
    REQUIRE( entry.guardWarnOverridePackageListFormID == 0 );
    REQUIRE( entry.combatOverridePackageListFormID == 0 );
    REQUIRE( entry.spellFormIDs.empty() );
    REQUIRE( entry.factionFormIDs.empty() );
    REQUIRE( entry.packageDataFormIDs.empty() );
    REQUIRE_FALSE( entry.unknownVTCK.has_value() );
  }

  SECTION("getWriteSize")
  {
    AliasEntry entry;
    entry.unknownALST = 1;
    entry.unknownFNAM = 2;

    SECTION("size with ALST")
    {
      REQUIRE( entry.getWriteSize() == 26 );
    }

    SECTION("size with ALLS")
    {
      entry.unknownALST.reset();
      entry.unknownALLS = 1;
      REQUIRE( entry.getWriteSize() == 26 );
    }

    SECTION("size adjusts with length of ALID")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.aliasID = "foo"; // three characters
      REQUIRE( entry.getWriteSize() == 36 );

      entry.aliasID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( entry.getWriteSize() == 48 );
    }

    SECTION("size adjusts when ALFA is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALFA = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when locationRefTypeFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.locationRefTypeFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when createReferenceToObjectFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.createReferenceToObjectFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when ALCA is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALCA = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when ALCL is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALCL = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when displayNameFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.displayNameFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts with length of components")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.components.push_back(ComponentData());
      REQUIRE( entry.getWriteSize() == 50 );

      entry.components.push_back(ComponentData());
      REQUIRE( entry.getWriteSize() == 64 );
    }

    SECTION("size adjusts with length of keywords")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.keywords.push_back(0x01234567);
      REQUIRE( entry.getWriteSize() == 46 );

      entry.keywords.push_back(0x01234568);
      REQUIRE( entry.getWriteSize() == 50 );

      entry.keywords.push_back(0x01234569);
      REQUIRE( entry.getWriteSize() == 54 );
    }

    SECTION("size adjusts when ALFE is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALFE = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when ALFD is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALFD = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when ALFI is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.forcedIntoAliasID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when specificLocationFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.specificLocationFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when specificReferenceID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.specificReferenceID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when ALNA is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALNA = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when ALNT is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALNT = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when uniqueActorFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.uniqueActorFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when externalAliasReferenceFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.externalAliasReferenceFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when ALEA is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownALEA = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when keywordFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.keywordFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts with length of CTDA/CIS2")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( entry.getWriteSize() == 64 );

      entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      REQUIRE( entry.getWriteSize() == 102 );

      entry.unknownCTDA_CIS2s.back().unknownCISx = "foo";
      REQUIRE( entry.getWriteSize() == 112 );
    }

    SECTION("size adjusts when spectatorOverridePackageListFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.spectatorOverridePackageListFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when observeDeadBodyOverridePackageListFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.observeDeadBodyOverridePackageListFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when guardWarnOverridePackageListFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.guardWarnOverridePackageListFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts when combatOverridePackageListFormID is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.combatOverridePackageListFormID = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }

    SECTION("size adjusts with length of spellFormIDs")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.spellFormIDs.push_back(0x01234567);
      REQUIRE( entry.getWriteSize() == 36 );

      entry.spellFormIDs.push_back(0x01234568);
      REQUIRE( entry.getWriteSize() == 46 );

      entry.spellFormIDs.push_back(0x01234569);
      REQUIRE( entry.getWriteSize() == 56 );
    }

    SECTION("size adjusts with length of factionFormIDs")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.factionFormIDs.push_back(0x01234567);
      REQUIRE( entry.getWriteSize() == 36 );

      entry.factionFormIDs.push_back(0x01234568);
      REQUIRE( entry.getWriteSize() == 46 );

      entry.factionFormIDs.push_back(0x01234569);
      REQUIRE( entry.getWriteSize() == 56 );
    }

    SECTION("size adjusts with length of packageDataFormIDs")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.packageDataFormIDs.push_back(0x01234567);
      REQUIRE( entry.getWriteSize() == 36 );

      entry.packageDataFormIDs.push_back(0x01234568);
      REQUIRE( entry.getWriteSize() == 46 );

      entry.packageDataFormIDs.push_back(0x01234569);
      REQUIRE( entry.getWriteSize() == 56 );
    }

    SECTION("size adjusts when VTCK is present")
    {
      REQUIRE( entry.getWriteSize() == 26 );

      entry.unknownVTCK = 0x01234567;
      REQUIRE( entry.getWriteSize() == 36 );
    }
  }

  SECTION("saveToStream")
  {
    using namespace std::string_view_literals;

    AliasEntry entry;
    entry.unknownALST = 0x00112233;
    entry.unknownFNAM = 2;

    SECTION("entry with ALLS")
    {
      entry.unknownALST.reset();
      entry.unknownALLS = 0x00112233;

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "ALLS\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0ALED\0\0"sv );
    }

    SECTION("entry with ALFA, ALRT, ALDN")
    {
      entry.unknownALFA = 0x00223344;
      entry.locationRefTypeFormID = 0x00333333; // ALRT
      entry.displayNameFormID = 0x00444444; // ALDN

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "ALST\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0ALFA\x04\0\x44\x33\x22\0ALRT\x04\0\x33\x33\x33\0ALDN\x04\0\x44\x44\x44\0ALED\0\0"sv );
    }

    SECTION("entry with components")
    {
      entry.components.push_back(ComponentData());
      entry.components.back().formID = 0x00123456;
      entry.components.back().count = 66;

      entry.components.push_back(ComponentData());
      entry.components.back().formID = 0x00222222;
      entry.components.back().count = 2;

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      const auto data = "ALST\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0COCT\x04\0\x02\0\0\0CNTO\x08\0\x56\x34\x12\0\x42\0\0\0CNTO\x08\0\x22\x22\x22\0\x02\0\0\0ALED\0\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("entry with keywords")
    {
      entry.keywords.push_back(0x00123456);
      entry.keywords.push_back(0x00222222);

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      const auto data = "ALST\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0KSIZ\x04\0\x02\0\0\0KWDA\x08\0\x56\x34\x12\0\x22\x22\x22\0ALED\0\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("entry with ALFE, ALFD, ALFI, ALFL")
    {
      entry.unknownALFE = 0x00223344;
      entry.unknownALFD = 0x00333333;
      entry.forcedIntoAliasID = 0x00444444; // ALFI
      entry.specificLocationFormID = 0x00555555; // ALFL

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "ALST\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0ALFE\x04\0\x44\x33\x22\0ALFD\x04\0\x33\x33\x33\0ALFI\x04\0\x44\x44\x44\0ALFL\x04\0\x55\x55\x55\0ALED\0\0"sv );
    }

    SECTION("entry with ALNA, ALNT, ALUA, ALEQ, ALEA, KNAM")
    {
      entry.unknownALNA = 0x00223344;
      entry.unknownALNT = 0x00333333;
      entry.uniqueActorFormID = 0x00444444; // ALUA
      entry.externalAliasReferenceFormID = 0x00555555; // ALEQ
      entry.unknownALEA = 0x00111111;
      entry.keywordFormID = 0x00777777; // KNAM

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      const auto data = "ALST\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0ALNA\x04\0\x44\x33\x22\0ALNT\x04\0\x33\x33\x33\0ALUA\x04\0\x44\x44\x44\0ALEQ\x04\0\x55\x55\x55\0ALEA\x04\0\x11\x11\x11\0KNAM\x04\0\x77\x77\x77\0ALED\0\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("entry with CTDA/CIS2")
    {
      entry.unknownCTDA_CIS2s.push_back(CTDA_CIS2_compound());
      entry.unknownCTDA_CIS2s.back().unknownCISx = "bla";

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      const auto data = "ALST\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0CTDA\x20\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0CIS2\x04\0bla\0ALED\0\0"sv;
      REQUIRE( stream.str() == data );
    }

    SECTION("entry with ALSP")
    {
      entry.spellFormIDs.push_back(0x00223344);
      entry.spellFormIDs.push_back(0x00333333);

      // Writing should succeed.
      std::ostringstream stream;
      REQUIRE( entry.saveToStream(stream) );
      // Check written data.
      REQUIRE( stream.str() == "ALST\x04\0\x33\x22\x11\0FNAM\x04\0\x02\0\0\0ALSP\x04\0\x44\x33\x22\0ALSP\x04\0\x33\x33\x33\0ALED\0\0"sv );
    }

    SECTION("fail: ALST and ALLS")
    {
      entry.unknownALST = 1;
      entry.unknownALLS = 1;

      // Writing should fail.
      std::ostringstream stream;
      REQUIRE_FALSE( entry.saveToStream(stream) );
    }

    SECTION("fail: no ALST and no ALLS")
    {
      entry.unknownALST.reset();
      entry.unknownALLS.reset();

      // Writing should fail.
      std::ostringstream stream;
      REQUIRE_FALSE( entry.saveToStream(stream) );
    }
  }
}
