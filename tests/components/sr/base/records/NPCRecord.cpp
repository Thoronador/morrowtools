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
#include <sstream>
#include <string_view>
#include "../../../../../sr/base/records/NPCRecord.hpp"
#include "../../../../../sr/base/SR_Constants.hpp"
#include "../../../../../sr/base/StringTable.hpp"

TEST_CASE("NPCRecord")
{
  using namespace SRTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    NPCRecord record;

    REQUIRE( record.editorID.empty() );
    REQUIRE_FALSE( record.unknownVMAD.isPresent() );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownOBND[i] == 0 );
    }
    for (unsigned int i = 0; i < 24; ++i)
    {
      REQUIRE( record.unknownACBS[i] == 0 );
    }
    REQUIRE( record.factions.empty() );
    REQUIRE( record.deathItemFormID == 0 );
    REQUIRE( record.voiceTypeFormID == 0 );
    REQUIRE( record.templateActorBaseFormID == 0 );
    REQUIRE( record.raceFormID == 0 );
    REQUIRE_FALSE( record.hasDEST );
    REQUIRE( record.unknownDEST == 0 );
    REQUIRE( record.skinFormID == 0 );
    REQUIRE( record.farAwayModelSkinFormID == 0 );
    REQUIRE_FALSE( record.hasATKR );
    REQUIRE( record.unknownATKR == 0 );
    REQUIRE_FALSE( record.unknownATKD.isPresent() );
    REQUIRE( record.unknownATKE.empty() );
    REQUIRE( record.spellFormIDs.empty() );
    REQUIRE( record.perkList.empty() );
    REQUIRE( record.items.empty() );
    REQUIRE( record.spectatorOverridePackageListFormID == 0 );
    REQUIRE( record.combatOverridePackageListFormID == 0 );
    for (unsigned int i = 0; i < 20; ++i)
    {
      REQUIRE( record.unknownAIDT[i] == 0 );
    }
    REQUIRE( record.unknownPKIDs.empty() );
    REQUIRE( record.keywordArray.empty() );
    REQUIRE( record.classFormID == 0 );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.hasSHRT );
    REQUIRE( record.unknownSHRT == 0 );
    for (unsigned int i = 0; i < 52; ++i)
    {
      REQUIRE( record.unknownDNAM[i] == 0 );
    }
    REQUIRE( record.unknownPNAMs.empty() );
    REQUIRE( record.hairColorFormID == 0 );
    REQUIRE( record.giftFilterFormID == 0 );
    REQUIRE( record.combatStyleFormID == 0 );
    REQUIRE( record.unknownNAM5 == 0 );
    REQUIRE( record.unknownNAM6 == 0 );
    REQUIRE( record.unknownNAM7 == 0 );
    REQUIRE( record.unknownNAM8 == 0 );
    REQUIRE( record.defaultOutfitFormID == 0 );
    REQUIRE( record.sleepOutfitFormID == 0 );
    REQUIRE( record.crimeFactionFormID == 0 );
    REQUIRE( record.soundTemplateFormID == 0 );
    REQUIRE( record.unknownCSDXs.empty() );
    REQUIRE( record.defaultPackageListFormID == 0 );
    REQUIRE( record.faceComplexionFormID == 0 );
    for (unsigned int i = 0; i < 12; ++i)
    {
      REQUIRE( record.unknownQNAM[i] == 0 );
    }
    REQUIRE_FALSE( record.unknownNAM9.isPresent() );
    REQUIRE_FALSE( record.unknownNAMA.isPresent() );
    REQUIRE( record.unknownTINXs.empty() );
  }

  SECTION("equals")
  {
    NPCRecord a;
    NPCRecord b;

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

      SECTION("VMAD mismatch")
      {
        a.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownVMAD.setPresence(false);
        b.unknownVMAD.setPresence(true);

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

      SECTION("ACBS mismatch")
      {
        a.unknownACBS[23] = 12;
        b.unknownACBS[23] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("factions mismatch")
      {
        a.factions.push_back(NPCRecord::FactionElem());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("deathItemFormID mismatch")
      {
        a.deathItemFormID = 0x01234567;
        b.deathItemFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("voiceTypeFormID mismatch")
      {
        a.voiceTypeFormID = 0x01234567;
        b.voiceTypeFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("templateActorBaseFormID mismatch")
      {
        a.templateActorBaseFormID = 0x01234567;
        b.templateActorBaseFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("raceFormID mismatch")
      {
        a.raceFormID = 0x01234567;
        b.raceFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DEST mismatch")
      {
        a.hasDEST = true;
        b.hasDEST = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasDEST = false;
        b.hasDEST = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasDEST = true;
        a.unknownDEST = 1;
        b.hasDEST = true;
        b.unknownDEST = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("skinFormID mismatch")
      {
        a.skinFormID = 0x01234567;
        b.skinFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("farAwayModelSkinFormID mismatch")
      {
        a.farAwayModelSkinFormID = 0x01234567;
        b.farAwayModelSkinFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ATKR mismatch")
      {
        a.hasATKR = true;
        b.hasATKR = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasATKR = false;
        b.hasATKR = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasATKR = true;
        a.unknownATKR = 1;
        b.hasATKR = true;
        b.unknownATKR = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ATKD mismatch")
      {
        a.unknownATKD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownATKD.setPresence(false);
        b.unknownATKD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ATKE mismatch")
      {
        a.unknownATKE = "foo";
        b.unknownATKE = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("spell form IDs mismatch")
      {
        a.spellFormIDs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.spellFormIDs.push_back(0x01234567);
        b.spellFormIDs.push_back(0x89ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("perkList mismatch")
      {
        a.perkList.push_back(NPCRecord::PerkElem());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("items mismatch")
      {
        a.items.push_back(ComponentData());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("spectatorOverridePackageListFormID mismatch")
      {
        a.spectatorOverridePackageListFormID = 0x01234567;
        b.spectatorOverridePackageListFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("combatOverridePackageListFormID mismatch")
      {
        a.combatOverridePackageListFormID = 0x01234567;
        b.combatOverridePackageListFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("AIDT mismatch")
      {
        a.unknownAIDT[19] = 12;
        b.unknownAIDT[19] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PKIDs mismatch")
      {
        a.unknownPKIDs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.unknownPKIDs.push_back(0x01234567);
        b.unknownPKIDs.push_back(0x89ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("keywords mismatch")
      {
        a.keywordArray.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.keywordArray.push_back(0x01234567);
        b.keywordArray.push_back(0x89ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("classFormID mismatch")
      {
        a.classFormID = 0x01234567;
        b.classFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("name mismatch")
      {
        a.name = LocalizedString(LocalizedString::Type::Index, 2, "");

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SHRT mismatch")
      {
        a.hasSHRT = true;
        b.hasSHRT = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasSHRT = false;
        b.hasSHRT = true;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.hasSHRT = true;
        a.unknownSHRT = 1;
        b.hasSHRT = true;
        b.unknownSHRT = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("DNAM mismatch")
      {
        a.unknownDNAM[51] = 12;
        b.unknownDNAM[51] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PNAMs mismatch")
      {
        a.unknownPNAMs.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.unknownPNAMs.push_back(0x01234567);
        b.unknownPNAMs.push_back(0x89ABCDEF);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("hairColorFormID mismatch")
      {
        a.hairColorFormID = 0x01234567;
        b.hairColorFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("giftFilterFormID mismatch")
      {
        a.giftFilterFormID = 0x01234567;
        b.giftFilterFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("combatStyleFormID mismatch")
      {
        a.combatStyleFormID = 0x01234567;
        b.combatStyleFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM5 mismatch")
      {
        a.unknownNAM5 = 1;
        b.unknownNAM5 = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM6 mismatch")
      {
        a.unknownNAM6 = 0x01234567;
        b.unknownNAM6 = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM7 mismatch")
      {
        a.unknownNAM7 = 0x01234567;
        b.unknownNAM7 = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM8 mismatch")
      {
        a.unknownNAM8 = 0x01234567;
        b.unknownNAM8 = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("defaultOutfitFormID mismatch")
      {
        a.defaultOutfitFormID = 0x01234567;
        b.defaultOutfitFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("sleepOutfitFormID mismatch")
      {
        a.sleepOutfitFormID = 0x01234567;
        b.sleepOutfitFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("crimeFactionFormID mismatch")
      {
        a.crimeFactionFormID = 0x01234567;
        b.crimeFactionFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("soundTemplateFormID mismatch")
      {
        a.soundTemplateFormID = 0x01234567;
        b.soundTemplateFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CSDX mismatch")
      {
        a.unknownCSDXs.push_back(NPCRecord::CSDXstruct());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("defaultPackageListFormID mismatch")
      {
        a.defaultPackageListFormID = 0x01234567;
        b.defaultPackageListFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("faceComplexionFormID mismatch")
      {
        a.faceComplexionFormID = 0x01234567;
        b.faceComplexionFormID = 0x89ABCDEF;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("QNAM mismatch")
      {
        a.unknownQNAM[11] = 12;
        b.unknownQNAM[11] = 34;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAM9 mismatch")
      {
        a.unknownNAM9.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownNAM9.setPresence(false);
        b.unknownNAM9.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NAMA mismatch")
      {
        a.unknownVMAD.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownNAMA.setPresence(false);
        b.unknownNAMA.setPresence(true);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("TINX mismatch")
      {
        a.unknownTINXs.push_back(NPCRecord::TINXstructure());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("getRecordType")
  {
    NPCRecord record;

    REQUIRE( record.getRecordType() == cNPC_ );
  }

  SECTION("getWriteSize")
  {
    NPCRecord record;

    record.editorID = "foo";

    SECTION("size adjusts with length of editor ID")
    {
      record.editorID = "foo"; // three characters
      REQUIRE( record.getWriteSize() == 224 );

      record.editorID = "foobarfoobarbaz"; // 15 characters
      REQUIRE( record.getWriteSize() == 236 );
    }

    SECTION("size adjusts with length of VMAD")
    {
      REQUIRE( record.getWriteSize() == 224 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "VMAD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 248 );

      {
        const std::string_view data = "VMAD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownVMAD.loadFromStream(stream, cVMAD, true) );
      }
      REQUIRE( record.getWriteSize() == 231 );
    }

    SECTION("size adjusts with length of factions")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.factions.push_back(NPCRecord::FactionElem());
      REQUIRE( record.getWriteSize() == 238 );

      record.factions.push_back(NPCRecord::FactionElem());
      REQUIRE( record.getWriteSize() == 252 );
    }

    SECTION("size adjusts when deathItemFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.deathItemFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when voiceTypeFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.voiceTypeFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when templateActorBaseFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.templateActorBaseFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when DEST is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.hasDEST = true;
      REQUIRE( record.getWriteSize() == 238 );
    }

    SECTION("size adjusts when skinFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.skinFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when farAwayModelSkinFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.farAwayModelSkinFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when ATKR is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.hasATKR = true;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts with length of ATKD")
    {
      REQUIRE( record.getWriteSize() == 224 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "ATKD\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownATKD.loadFromStream(stream, cATKD, true) );
      }
      REQUIRE( record.getWriteSize() == 248 );

      {
        const std::string_view data = "ATKD\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownATKD.loadFromStream(stream, cATKD, true) );
      }
      REQUIRE( record.getWriteSize() == 231 );
    }

    SECTION("size adjusts with length of ATKE")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.unknownATKE = "foo";
      REQUIRE( record.getWriteSize() == 234 );

      record.unknownATKE = "foobar";
      REQUIRE( record.getWriteSize() == 237 );
    }

    SECTION("size adjusts with length of spell form IDs")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.spellFormIDs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 244 );

      record.spellFormIDs.push_back(0x01234568);
      REQUIRE( record.getWriteSize() == 254 );
    }

    SECTION("size adjusts with length of perk list")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.perkList.push_back(NPCRecord::PerkElem());
      REQUIRE( record.getWriteSize() == 248 );

      record.perkList.push_back(NPCRecord::PerkElem());
      REQUIRE( record.getWriteSize() == 262 );
    }

    SECTION("size adjusts with length of item list")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.items.push_back(ComponentData());
      REQUIRE( record.getWriteSize() == 248 );

      record.items.push_back(ComponentData());
      REQUIRE( record.getWriteSize() == 262 );
    }

    SECTION("size adjusts when spectatorOverridePackageListFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.spectatorOverridePackageListFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when combatOverridePackageListFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.combatOverridePackageListFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts with length of PKIDs")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.unknownPKIDs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 234 );

      record.unknownPKIDs.push_back(0x01234568);
      REQUIRE( record.getWriteSize() == 244 );
    }

    SECTION("size adjusts with length of keywords")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.keywordArray.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 244 );

      record.keywordArray.push_back(0x01234568);
      REQUIRE( record.getWriteSize() == 248 );
    }

    SECTION("size adjusts when name is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.name = LocalizedString(LocalizedString::Type::Index, 1, "");
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when SHRT is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.hasSHRT = true;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts with length of PNAMs")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.unknownPNAMs.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 234 );

      record.unknownPNAMs.push_back(0x01234568);
      REQUIRE( record.getWriteSize() == 244 );
    }

    SECTION("size adjusts when hairColorFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.hairColorFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when giftFilterFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.giftFilterFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when combatStyleFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.combatStyleFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when defaultOutfitFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.defaultOutfitFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when sleepOutfitFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.sleepOutfitFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when crimeFactionFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.crimeFactionFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when soundTemplateFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.soundTemplateFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts with length of CSDXs")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.unknownCSDXs.push_back(NPCRecord::CSDXstruct());
      REQUIRE( record.getWriteSize() == 251 );

      record.unknownCSDXs.push_back(NPCRecord::CSDXstruct());
      REQUIRE( record.getWriteSize() == 278 );
    }

    SECTION("size adjusts when defaultPackageListFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.defaultPackageListFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts when faceComplexionFormID is present")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.faceComplexionFormID = 0x01234567;
      REQUIRE( record.getWriteSize() == 234 );
    }

    SECTION("size adjusts with length of NAM9")
    {
      REQUIRE( record.getWriteSize() == 224 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "NAM9\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownNAM9.loadFromStream(stream, cNAM9, true) );
      }
      REQUIRE( record.getWriteSize() == 248 );

      {
        const std::string_view data = "NAM9\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownNAM9.loadFromStream(stream, cNAM9, true) );
      }
      REQUIRE( record.getWriteSize() == 231 );
    }

    SECTION("size adjusts with length of NAMA")
    {
      REQUIRE( record.getWriteSize() == 224 );

      // Reading data from a stream is currently the only way to get a record
      // with data, so we do that here.
      {
        const std::string_view data = "NAMA\x12\0_23456789012345678"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownNAMA.loadFromStream(stream, cNAMA, true) );
      }
      REQUIRE( record.getWriteSize() == 248 );

      {
        const std::string_view data = "NAMA\x01\0_"sv;
        std::istringstream stream;
        stream.str(std::string(data));
        REQUIRE( record.unknownNAMA.loadFromStream(stream, cNAMA, true) );
      }
      REQUIRE( record.getWriteSize() == 231 );
    }

    SECTION("size adjusts with length of TINXs")
    {
      REQUIRE( record.getWriteSize() == 224 );

      record.unknownTINXs.push_back(NPCRecord::TINXstructure());
      REQUIRE( record.getWriteSize() == 260 );

      record.unknownTINXs.push_back(NPCRecord::TINXstructure());
      REQUIRE( record.getWriteSize() == 296 );
    }
  }
}
