/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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
    REQUIRE_FALSE( record.unknownDEST.has_value() );
    REQUIRE( record.skinFormID == 0 );
    REQUIRE( record.farAwayModelSkinFormID == 0 );
    REQUIRE_FALSE( record.unknownATKR.has_value() );
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
    REQUIRE( record.keywords.empty() );
    REQUIRE( record.classFormID == 0 );
    REQUIRE_FALSE( record.name.isPresent() );
    REQUIRE_FALSE( record.unknownSHRT.has_value() );
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
        a.unknownDEST = 0;
        b.unknownDEST.reset();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownDEST.reset();
        b.unknownDEST = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownDEST = 1;
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
        a.unknownATKR = 0;
        b.unknownATKR.reset();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownATKR.reset();
        b.unknownATKR = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownATKR = 1;
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
        a.keywords.push_back(0x01234567);

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        b.keywords.push_back(0x01234567);
        b.keywords.push_back(0x89ABCDEF);

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
        a.unknownSHRT = 0;
        b.unknownSHRT.reset();

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownSHRT.reset();
        b.unknownSHRT = 0;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );

        a.unknownSHRT = 1;
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

      record.unknownDEST = 1;
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

      record.unknownATKR = 1;
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

      record.keywords.push_back(0x01234567);
      REQUIRE( record.getWriteSize() == 244 );

      record.keywords.push_back(0x01234568);
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

      record.unknownSHRT = 1;
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

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;
    StringTable dummy_table;
    dummy_table.addString(0x00008845, "foo");
    dummy_table.addString(0x0000C59F, "foo");

    SECTION("default: load compressed record")
    {
      const auto data = "NPC_\xD1\0\0\0\0\0\x04\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0R\x01\0\0x\xDAsu\xF1t\xE1\x61\xF0\x0D\x34\x34\x30q)JL\xCF\xCF\x63\xF0w\xF2\x03\x8A\x20\x03Gg\xA7`\x09\x38\x8F\x11L\xA6\x30(3d\xF3\x83X\xC1~\x8E\xBE\x1C\x0Cst\x99\x19\x10<a\xA8Z\x08\x8F\xC1\x97\x15\x89\x37\xD9\x99\x13\xCC\x0B\x09\xF0\x09\x61\x61h[\xC7\xCF\x10\x04\x14ga\xA8\x99#\xC0\xE0\xE8\xE9\x12\"\xC2\xC0\xC8\x62\x04\xB3\x06\x0E\xBC\x83=\xA3X\xC0\x82\xDE\xE1.\x8E,\x0C\x91\xB1\xCC\x0C\xCE`}\x1C\x05\x8C\x0Cn\xA1>>,\x0C\xAE\x1D\x0C\x0C.\x8E!\x8E@\x12(c\xC2\xC0\x8A\x01\x18\x30\x80)\xD3\x34\x86&\xA8\x65\x8C=!\x0CQ`3\x9B{X\x18\x80\x0CS&\x86\xFF\x20\xDA\x8C\x05(\xDB`\x0F\x64\x99\x83X\x1EN@\x96\x05\xC4\x35\x81@&jh\x01\0i\xE8+<"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NPCRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x20040000 );
      REQUIRE( record.headerFormID == 0x0001CA05 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0007 );
      // -- record data
      REQUIRE( record.editorID == "MQ104Dragon" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
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
      REQUIRE( record.unknownACBS[0] == 0x00 );
      REQUIRE( record.unknownACBS[1] == 0x00 );
      REQUIRE( record.unknownACBS[2] == 0x00 );
      REQUIRE( record.unknownACBS[3] == 0x00 );
      REQUIRE( record.unknownACBS[4] == 0x00 );
      REQUIRE( record.unknownACBS[5] == 0x00 );
      REQUIRE( record.unknownACBS[6] == 0x00 );
      REQUIRE( record.unknownACBS[7] == 0x00 );
      REQUIRE( record.unknownACBS[8] == 0x01 );
      REQUIRE( record.unknownACBS[9] == 0x00 );
      REQUIRE( record.unknownACBS[10] == 0x00 );
      REQUIRE( record.unknownACBS[11] == 0x00 );
      REQUIRE( record.unknownACBS[12] == 0x00 );
      REQUIRE( record.unknownACBS[13] == 0x00 );
      REQUIRE( record.unknownACBS[14] == 0x64 );
      REQUIRE( record.unknownACBS[15] == 0x00 );
      REQUIRE( record.unknownACBS[16] == 0x23 );
      REQUIRE( record.unknownACBS[17] == 0x00 );
      REQUIRE( record.unknownACBS[18] == 0x6B );
      REQUIRE( record.unknownACBS[19] == 0x0F );
      REQUIRE( record.unknownACBS[20] == 0x00 );
      REQUIRE( record.unknownACBS[21] == 0x00 );
      REQUIRE( record.unknownACBS[22] == 0x00 );
      REQUIRE( record.unknownACBS[23] == 0x00 );
      REQUIRE( record.factions.size() == 4 );
      REQUIRE( record.factions[0].formID == 0x00032D9C );
      REQUIRE( record.factions[0].rank == 0 );
      REQUIRE( record.factions[1].formID == 0x00000013 );
      REQUIRE( record.factions[1].rank == 0 );
      REQUIRE( record.factions[2].formID == 0x00054D00 );
      REQUIRE( record.factions[2].rank == 0 );
      REQUIRE( record.factions[3].formID == 0x00094393 );
      REQUIRE( record.factions[3].rank == 0 );
      REQUIRE( record.deathItemFormID == 0 );
      REQUIRE( record.voiceTypeFormID == 0 );
      REQUIRE( record.templateActorBaseFormID == 0x000FAE86 );
      REQUIRE( record.raceFormID == 0x00109C7C );
      REQUIRE_FALSE( record.unknownDEST.has_value() );
      REQUIRE( record.skinFormID == 0 );
      REQUIRE( record.farAwayModelSkinFormID == 0 );
      REQUIRE_FALSE( record.unknownATKR.has_value() );
      REQUIRE_FALSE( record.unknownATKD.isPresent() );
      REQUIRE( record.unknownATKE.empty() );
      REQUIRE( record.spellFormIDs.empty() );
      REQUIRE( record.perkList.empty() );
      REQUIRE( record.items.empty() );
      REQUIRE( record.spectatorOverridePackageListFormID == 0 );
      REQUIRE( record.combatOverridePackageListFormID == 0 );
      REQUIRE( record.unknownAIDT[0] == 0x01 );
      REQUIRE( record.unknownAIDT[1] == 0x04 );
      REQUIRE( record.unknownAIDT[2] == 0x32 );
      REQUIRE( record.unknownAIDT[3] == 0x00 );
      REQUIRE( record.unknownAIDT[4] == 0x00 );
      REQUIRE( record.unknownAIDT[5] == 0x01 );
      REQUIRE( record.unknownAIDT[6] == 0x00 );
      REQUIRE( record.unknownAIDT[7] == 0x00 );
      REQUIRE( record.unknownAIDT[8] == 0x00 );
      REQUIRE( record.unknownAIDT[9] == 0x00 );
      REQUIRE( record.unknownAIDT[10] == 0x00 );
      REQUIRE( record.unknownAIDT[11] == 0x00 );
      REQUIRE( record.unknownAIDT[12] == 0x00 );
      REQUIRE( record.unknownAIDT[13] == 0x00 );
      REQUIRE( record.unknownAIDT[14] == 0x00 );
      REQUIRE( record.unknownAIDT[15] == 0x00 );
      REQUIRE( record.unknownAIDT[16] == 0x00 );
      REQUIRE( record.unknownAIDT[17] == 0x00 );
      REQUIRE( record.unknownAIDT[18] == 0x00 );
      REQUIRE( record.unknownAIDT[19] == 0x00 );
      REQUIRE( record.unknownPKIDs.empty() );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x00035D59 );
      REQUIRE( record.classFormID == 0x00017008 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00008845 );
      REQUIRE_FALSE( record.unknownSHRT.has_value() );
      const auto DNAM = std::string_view(reinterpret_cast<const char*>(record.unknownDNAM.data()), 52);
      REQUIRE( DNAM == "\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0"sv );
      REQUIRE( record.unknownPNAMs.empty() );
      REQUIRE( record.hairColorFormID == 0 );
      REQUIRE( record.giftFilterFormID == 0 );
      REQUIRE( record.combatStyleFormID == 0x00048C83 );
      REQUIRE( record.unknownNAM5 == 0x00FF );
      REQUIRE( record.unknownNAM6 == 0x3F800000 );
      REQUIRE( record.unknownNAM7 == 0x42480000 );
      REQUIRE( record.unknownNAM8 == 1 );
      REQUIRE( record.defaultOutfitFormID == 0 );
      REQUIRE( record.sleepOutfitFormID == 0 );
      REQUIRE( record.crimeFactionFormID == 0 );
      REQUIRE( record.soundTemplateFormID == 0 );
      REQUIRE( record.unknownCSDXs.empty() );
      REQUIRE( record.defaultPackageListFormID == 0 );
      REQUIRE( record.faceComplexionFormID == 0 );
      const auto QNAM = std::string_view(reinterpret_cast<const char*>(record.unknownQNAM.data()), 12);
      REQUIRE( QNAM == "\0\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE_FALSE( record.unknownNAM9.isPresent() );
      REQUIRE_FALSE( record.unknownNAMA.isPresent() );
      REQUIRE( record.unknownTINXs.empty() );
    }

    SECTION("default: load uncompressed record")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NPCRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x20000000 );
      REQUIRE( record.headerFormID == 0x0001CA05 );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x0007 );
      // -- record data
      REQUIRE( record.editorID == "MQ104Dragon" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
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
      REQUIRE( record.unknownACBS[0] == 0x00 );
      REQUIRE( record.unknownACBS[1] == 0x00 );
      REQUIRE( record.unknownACBS[2] == 0x00 );
      REQUIRE( record.unknownACBS[3] == 0x00 );
      REQUIRE( record.unknownACBS[4] == 0x00 );
      REQUIRE( record.unknownACBS[5] == 0x00 );
      REQUIRE( record.unknownACBS[6] == 0x00 );
      REQUIRE( record.unknownACBS[7] == 0x00 );
      REQUIRE( record.unknownACBS[8] == 0x01 );
      REQUIRE( record.unknownACBS[9] == 0x00 );
      REQUIRE( record.unknownACBS[10] == 0x00 );
      REQUIRE( record.unknownACBS[11] == 0x00 );
      REQUIRE( record.unknownACBS[12] == 0x00 );
      REQUIRE( record.unknownACBS[13] == 0x00 );
      REQUIRE( record.unknownACBS[14] == 0x64 );
      REQUIRE( record.unknownACBS[15] == 0x00 );
      REQUIRE( record.unknownACBS[16] == 0x23 );
      REQUIRE( record.unknownACBS[17] == 0x00 );
      REQUIRE( record.unknownACBS[18] == 0x6B );
      REQUIRE( record.unknownACBS[19] == 0x0F );
      REQUIRE( record.unknownACBS[20] == 0x00 );
      REQUIRE( record.unknownACBS[21] == 0x00 );
      REQUIRE( record.unknownACBS[22] == 0x00 );
      REQUIRE( record.unknownACBS[23] == 0x00 );
      REQUIRE( record.factions.size() == 4 );
      REQUIRE( record.factions[0].formID == 0x00032D9C );
      REQUIRE( record.factions[0].rank == 0 );
      REQUIRE( record.factions[1].formID == 0x00000013 );
      REQUIRE( record.factions[1].rank == 0 );
      REQUIRE( record.factions[2].formID == 0x00054D00 );
      REQUIRE( record.factions[2].rank == 0 );
      REQUIRE( record.factions[3].formID == 0x00094393 );
      REQUIRE( record.factions[3].rank == 0 );
      REQUIRE( record.deathItemFormID == 0 );
      REQUIRE( record.voiceTypeFormID == 0 );
      REQUIRE( record.templateActorBaseFormID == 0x000FAE86 );
      REQUIRE( record.raceFormID == 0x00109C7C );
      REQUIRE_FALSE( record.unknownDEST.has_value() );
      REQUIRE( record.skinFormID == 0 );
      REQUIRE( record.farAwayModelSkinFormID == 0 );
      REQUIRE_FALSE( record.unknownATKR.has_value() );
      REQUIRE_FALSE( record.unknownATKD.isPresent() );
      REQUIRE( record.unknownATKE.empty() );
      REQUIRE( record.spellFormIDs.empty() );
      REQUIRE( record.perkList.empty() );
      REQUIRE( record.items.empty() );
      REQUIRE( record.spectatorOverridePackageListFormID == 0 );
      REQUIRE( record.combatOverridePackageListFormID == 0 );
      REQUIRE( record.unknownAIDT[0] == 0x01 );
      REQUIRE( record.unknownAIDT[1] == 0x04 );
      REQUIRE( record.unknownAIDT[2] == 0x32 );
      REQUIRE( record.unknownAIDT[3] == 0x00 );
      REQUIRE( record.unknownAIDT[4] == 0x00 );
      REQUIRE( record.unknownAIDT[5] == 0x01 );
      REQUIRE( record.unknownAIDT[6] == 0x00 );
      REQUIRE( record.unknownAIDT[7] == 0x00 );
      REQUIRE( record.unknownAIDT[8] == 0x00 );
      REQUIRE( record.unknownAIDT[9] == 0x00 );
      REQUIRE( record.unknownAIDT[10] == 0x00 );
      REQUIRE( record.unknownAIDT[11] == 0x00 );
      REQUIRE( record.unknownAIDT[12] == 0x00 );
      REQUIRE( record.unknownAIDT[13] == 0x00 );
      REQUIRE( record.unknownAIDT[14] == 0x00 );
      REQUIRE( record.unknownAIDT[15] == 0x00 );
      REQUIRE( record.unknownAIDT[16] == 0x00 );
      REQUIRE( record.unknownAIDT[17] == 0x00 );
      REQUIRE( record.unknownAIDT[18] == 0x00 );
      REQUIRE( record.unknownAIDT[19] == 0x00 );
      REQUIRE( record.unknownPKIDs.empty() );
      REQUIRE( record.keywords.size() == 1 );
      REQUIRE( record.keywords[0] == 0x00035D59 );
      REQUIRE( record.classFormID == 0x00017008 );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x00008845 );
      REQUIRE_FALSE( record.unknownSHRT.has_value() );
      const auto DNAM = std::string_view(reinterpret_cast<const char*>(record.unknownDNAM.data()), 52);
      REQUIRE( DNAM == "\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0"sv );
      REQUIRE( record.unknownPNAMs.empty() );
      REQUIRE( record.hairColorFormID == 0 );
      REQUIRE( record.giftFilterFormID == 0 );
      REQUIRE( record.combatStyleFormID == 0x00048C83 );
      REQUIRE( record.unknownNAM5 == 0x00FF );
      REQUIRE( record.unknownNAM6 == 0x3F800000 );
      REQUIRE( record.unknownNAM7 == 0x42480000 );
      REQUIRE( record.unknownNAM8 == 1 );
      REQUIRE( record.defaultOutfitFormID == 0 );
      REQUIRE( record.sleepOutfitFormID == 0 );
      REQUIRE( record.crimeFactionFormID == 0 );
      REQUIRE( record.soundTemplateFormID == 0 );
      REQUIRE( record.unknownCSDXs.empty() );
      REQUIRE( record.defaultPackageListFormID == 0 );
      REQUIRE( record.faceComplexionFormID == 0 );
      const auto QNAM = std::string_view(reinterpret_cast<const char*>(record.unknownQNAM.data()), 12);
      REQUIRE( QNAM == "\0\0\0\0\0\0\0\0\0\0\0\0"sv );
      REQUIRE_FALSE( record.unknownNAM9.isPresent() );
      REQUIRE_FALSE( record.unknownNAMA.isPresent() );
      REQUIRE( record.unknownTINXs.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with INAM, VTCK, spells, WNAM, perks, items, SHRT, CSCR, DOFT and ATKR")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      NPCRecord record;
      REQUIRE( record.loadFromStream(stream, true, dummy_table) );
      // Check data.
      // -- header
      REQUIRE( record.headerFlags == 0x00000000 );
      REQUIRE( record.headerFormID == 0x0003763A );
      REQUIRE( record.headerRevision == 0x0055691B );
      REQUIRE( record.headerVersion == 40 );
      REQUIRE( record.headerUnknown5 == 0x000C );
      // -- record data
      REQUIRE( record.editorID == "dunRagnOtar" );
      REQUIRE_FALSE( record.unknownVMAD.isPresent() );
      REQUIRE( record.unknownOBND[0] == 0xE6 );
      REQUIRE( record.unknownOBND[1] == 0xFF );
      REQUIRE( record.unknownOBND[2] == 0xEE );
      REQUIRE( record.unknownOBND[3] == 0xFF );
      REQUIRE( record.unknownOBND[4] == 0x00 );
      REQUIRE( record.unknownOBND[5] == 0x00 );
      REQUIRE( record.unknownOBND[6] == 0x1A );
      REQUIRE( record.unknownOBND[7] == 0x00 );
      REQUIRE( record.unknownOBND[8] == 0x12 );
      REQUIRE( record.unknownOBND[9] == 0x00 );
      REQUIRE( record.unknownOBND[10] == 0x95 );
      REQUIRE( record.unknownOBND[11] == 0x00 );
      REQUIRE( record.unknownACBS[0] == 0x10 );
      REQUIRE( record.unknownACBS[1] == 0x00 );
      REQUIRE( record.unknownACBS[2] == 0x04 );
      REQUIRE( record.unknownACBS[3] == 0x00 );
      REQUIRE( record.unknownACBS[4] == 0x64 );
      REQUIRE( record.unknownACBS[5] == 0x00 );
      REQUIRE( record.unknownACBS[6] == 0x00 );
      REQUIRE( record.unknownACBS[7] == 0x00 );
      REQUIRE( record.unknownACBS[8] == 0x32 );
      REQUIRE( record.unknownACBS[9] == 0x00 );
      REQUIRE( record.unknownACBS[10] == 0x00 );
      REQUIRE( record.unknownACBS[11] == 0x00 );
      REQUIRE( record.unknownACBS[12] == 0x64 );
      REQUIRE( record.unknownACBS[13] == 0x00 );
      REQUIRE( record.unknownACBS[14] == 0x64 );
      REQUIRE( record.unknownACBS[15] == 0x00 );
      REQUIRE( record.unknownACBS[16] == 0x23 );
      REQUIRE( record.unknownACBS[17] == 0x00 );
      REQUIRE( record.unknownACBS[18] == 0x7F );
      REQUIRE( record.unknownACBS[19] == 0x0E );
      REQUIRE( record.unknownACBS[20] == 0x00 );
      REQUIRE( record.unknownACBS[21] == 0x00 );
      REQUIRE( record.unknownACBS[22] == 0x00 );
      REQUIRE( record.unknownACBS[23] == 0x00 );
      REQUIRE( record.factions.size() == 2 );
      REQUIRE( record.factions[0].formID == 0x00000013 );
      REQUIRE( record.factions[0].rank == 0 );
      REQUIRE( record.factions[1].formID == 0x00106643 );
      REQUIRE( record.factions[1].rank == 0 );
      REQUIRE( record.deathItemFormID == 0x0003AD7E );
      REQUIRE( record.voiceTypeFormID == 0x00029986 );
      REQUIRE( record.templateActorBaseFormID == 0x0008A1C9 );
      REQUIRE( record.raceFormID == 0x000131EF );
      REQUIRE_FALSE( record.unknownDEST.has_value() );
      REQUIRE( record.spellFormIDs.size() == 1 );
      REQUIRE( record.spellFormIDs[0] == 0x00020258 );
      REQUIRE( record.skinFormID == 0x0003B5AB );
      REQUIRE( record.farAwayModelSkinFormID == 0 );
      REQUIRE( record.unknownATKR.has_value() );
      REQUIRE( record.unknownATKR.value() == 0x000131EF );
      REQUIRE_FALSE( record.unknownATKD.isPresent() );
      REQUIRE( record.unknownATKE.empty() );
      REQUIRE( record.perkList.size() == 9 );
      REQUIRE( record.perkList[0].formID == 0x00053128 );
      REQUIRE( record.perkList[0].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[1].formID == 0x000581FC );
      REQUIRE( record.perkList[1].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[2].formID == 0x000D799A );
      REQUIRE( record.perkList[2].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[3].formID == 0x0010FCFA );
      REQUIRE( record.perkList[3].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[4].formID == 0x000CB419 );
      REQUIRE( record.perkList[4].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[5].formID == 0x000581F8 );
      REQUIRE( record.perkList[5].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[6].formID == 0x000581F9 );
      REQUIRE( record.perkList[6].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[7].formID == 0x00068BCC );
      REQUIRE( record.perkList[7].valueTwo == 0x13C3D801 );
      REQUIRE( record.perkList[8].formID == 0x000581F5 );
      REQUIRE( record.perkList[8].valueTwo == 0x13C3D801 );
      REQUIRE( record.items.size() == 1 );
      REQUIRE( record.items[0].formID == 0x00029B87 );
      REQUIRE( record.items[0].count == 1 );
      REQUIRE( record.spectatorOverridePackageListFormID == 0 );
      REQUIRE( record.combatOverridePackageListFormID == 0 );
      REQUIRE( record.unknownAIDT[0] == 0x00 );
      REQUIRE( record.unknownAIDT[1] == 0x04 );
      REQUIRE( record.unknownAIDT[2] == 0x32 );
      REQUIRE( record.unknownAIDT[3] == 0x00 );
      REQUIRE( record.unknownAIDT[4] == 0x00 );
      REQUIRE( record.unknownAIDT[5] == 0x00 );
      REQUIRE( record.unknownAIDT[6] == 0x00 );
      REQUIRE( record.unknownAIDT[7] == 0x00 );
      REQUIRE( record.unknownAIDT[8] == 0x00 );
      REQUIRE( record.unknownAIDT[9] == 0x00 );
      REQUIRE( record.unknownAIDT[10] == 0x00 );
      REQUIRE( record.unknownAIDT[11] == 0x00 );
      REQUIRE( record.unknownAIDT[12] == 0x00 );
      REQUIRE( record.unknownAIDT[13] == 0x00 );
      REQUIRE( record.unknownAIDT[14] == 0x00 );
      REQUIRE( record.unknownAIDT[15] == 0x00 );
      REQUIRE( record.unknownAIDT[16] == 0x00 );
      REQUIRE( record.unknownAIDT[17] == 0x00 );
      REQUIRE( record.unknownAIDT[18] == 0x00 );
      REQUIRE( record.unknownAIDT[19] == 0x00 );
      REQUIRE( record.unknownPKIDs.size() == 3 );
      REQUIRE( record.unknownPKIDs[0] == 0x0009448B );
      REQUIRE( record.unknownPKIDs[1] == 0x00088978 );
      REQUIRE( record.unknownPKIDs[2] == 0x0009BCBD );
      REQUIRE( record.keywords.empty() );
      REQUIRE( record.classFormID == 0x0001CE1C );
      REQUIRE( record.name.isPresent() );
      REQUIRE( record.name.getType() == LocalizedString::Type::Index );
      REQUIRE( record.name.getIndex() == 0x0000C59F );
      REQUIRE( record.unknownSHRT.has_value() );
      REQUIRE( record.unknownSHRT.value() == 0x00010148 );
      const auto DNAM = std::string_view(reinterpret_cast<const char*>(record.unknownDNAM.data()), 52);
      REQUIRE( DNAM == "\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0"sv );
      REQUIRE( record.unknownPNAMs.empty() );
      REQUIRE( record.hairColorFormID == 0 );
      REQUIRE( record.giftFilterFormID == 0 );
      REQUIRE( record.combatStyleFormID == 0x000BB365 );
      REQUIRE( record.unknownNAM5 == 0x00FF );
      REQUIRE( record.unknownNAM6 == 0x3F800000 ); // 1.0f, a single-precision float
      REQUIRE( record.unknownNAM7 == 0x3F800000 ); // 1.0f
      REQUIRE( record.unknownNAM8 == 1 );
      REQUIRE( record.defaultOutfitFormID == 0x000F849A );
      REQUIRE( record.sleepOutfitFormID == 0 );
      REQUIRE( record.crimeFactionFormID == 0 );
      REQUIRE( record.soundTemplateFormID == 0x00046B83 );
      REQUIRE( record.unknownCSDXs.empty() );
      REQUIRE( record.defaultPackageListFormID == 0x00032AE2 );
      REQUIRE( record.faceComplexionFormID == 0 );
      const auto QNAM = std::string_view(reinterpret_cast<const char*>(record.unknownQNAM.data()), 12);
      REQUIRE( QNAM == "\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv );
      REQUIRE_FALSE( record.unknownNAM9.isPresent() );
      REQUIRE_FALSE( record.unknownNAMA.isPresent() );
      REQUIRE( record.unknownTINXs.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before decompressed size of a compressed record can be read")
    {
      const auto data = "NPC_\xD1\0\0\0\0\0\x04\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0R\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: record size is too small to contain compressed data")
    {
      const auto data = "NPC_\x04\0\0\0\0\0\x04\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0R\x01\0\0x\xDAsu\xF1t\xE1\x61\xF0\x0D\x34\x34\x30q)JL\xCF\xCF\x63\xF0w\xF2\x03\x8A\x20\x03Gg\xA7`\x09\x38\x8F\x11L\xA6\x30(3d\xF3\x83X\xC1~\x8E\xBE\x1C\x0Cst\x99\x19\x10<a\xA8Z\x08\x8F\xC1\x97\x15\x89\x37\xD9\x99\x13\xCC\x0B\x09\xF0\x09\x61\x61h[\xC7\xCF\x10\x04\x14ga\xA8\x99#\xC0\xE0\xE8\xE9\x12\"\xC2\xC0\xC8\x62\x04\xB3\x06\x0E\xBC\x83=\xA3X\xC0\x82\xDE\xE1.\x8E,\x0C\x91\xB1\xCC\x0C\xCE`}\x1C\x05\x8C\x0Cn\xA1>>,\x0C\xAE\x1D\x0C\x0C.\x8E!\x8E@\x12(c\xC2\xC0\x8A\x01\x18\x30\x80)\xD3\x34\x86&\xA8\x65\x8C=!\x0CQ`3\x9B{X\x18\x80\x0CS&\x86\xFF\x20\xDA\x8C\x05(\xDB`\x0F\x64\x99\x83X\x1EN@\x96\x05\xC4\x35\x81@&jh\x01\0i\xE8+<"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all compressed data of a record can be read")
    {
      const auto data = "NPC_\xD1\0\0\0\0\0\x04\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0R\x01\0\0x\xDAsu\xF1t\xE1"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: invalid compressed data in a compressed record")
    {
      const auto data = "NPC_\xD1\0\0\0\0\0\x04\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0R\x01\0\0x\xFFzz\xF1t\xE1\x61\xF0\x0D\x34\x34\x30q)JL\xCF\xCF\x63\xF0w\xF2\x03\x8A\x20\x03Gg\xA7`\x09\x38\x8F\x11L\xA6\x30(3d\xF3\x83X\xC1~\x8E\xBE\x1C\x0Cst\x99\x19\x10<a\xA8Z\x08\x8F\xC1\x97\x15\x89\x37\xD9\x99\x13\xCC\x0B\x09\xF0\x09\x61\x61h[\xC7\xCF\x10\x04\x14ga\xA8\x99#\xC0\xE0\xE8\xE9\x12\"\xC2\xC0\xC8\x62\x04\xB3\x06\x0E\xBC\x83=\xA3X\xC0\x82\xDE\xE1.\x8E,\x0C\x91\xB1\xCC\x0C\xCE`}\x1C\x05\x8C\x0Cn\xA1>>,\x0C\xAE\x1D\x0C\x0C.\x8E!\x8E@\x12(c\xC2\xC0\x8A\x01\x18\x30\x80)\xD3\x34\x86&\xA8\x65\x8C=!\x0CQ`3\x9B{X\x18\x80\x0CS&\x86\xFF\x20\xDA\x8C\x05(\xDB`\x0F\x64\x99\x83X\x1EN@\x96\x05\xC4\x35\x81@&jh\x01\0i\xE8+<"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no EDID")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0FAIL\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID > 512")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\x02MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of EDID is beyond stream")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x52\x01MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of VMAD can be read")
    {
      const std::string_view data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x52\x01MQ104Dragon\0VMAD\x64\0\xFE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no OBND")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0FAIL\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing OBND")
    {
      const auto data = "NPC_\x40\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple OBND")
    {
      const auto data = "NPC_\x64\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of OBND is not 12")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0B\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of OBND can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing ACBS")
    {
      const auto data = "NPC_\x34\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ACBS")
    {
      const auto data = "NPC_\x70\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ACBS is not 24")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x17\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x19\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of ACBS can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SNAM is not 8")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x07\0\x9C-\x03\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x09\0\x9C-\x03\0\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of SNAM can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple INAMs")
    {
      const auto data = "NPC_\x4E\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of INAM is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x03\0~\xAD\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x05\0~\xAD\x03\0\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of INAM can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: INAM is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0\0\0\0\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple VTCKs")
    {
      const auto data = "NPC_\x4E\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of VTCK is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x03\0\x86\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x05\0\x86\x99\x02\0\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of VTCK can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: VTCK is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\0\0\0\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple TPLTs")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of TPLT is not four")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x03\0\x86\xAE\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x05\0\x86\xAE\x0F\0\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of TPLT can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: TPLT is zero")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\0\0\0\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple RNAMs")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of RNAM is not four")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x03\0|\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x05\0|\x9C\x10\0\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of RNAM can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: RNAM is zero")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0\0\0\0\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SPCTs")
    {
      const auto data = "NPC_\x58\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SPCT is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x03\0\x01\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x05\0\x01\0\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of SPCT can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: SPCT is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\0\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no SPLO after SPCT")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0FAIL\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SPLO is not four")
    {
      {
        const auto data = "NPC_\x42\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x02\0\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x05\0X\x02\x02\0\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of SPLO can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple WNAMs")
    {
      const auto data = "NPC_\x4E\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of WNAM is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x03\0\xAB\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x05\0\xAB\xB5\x03\0\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of WNAM can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: WNAM is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\0\0\0\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ATKRs")
    {
      const auto data = "NPC_\x4E\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ATKR is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x03\0\xEF\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x05\0\xEF\x31\x01\0\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of ATKR can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple PRKZs")
    {
      const auto data = "NPC_\x5C\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x01\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of PRKZ is not four")
    {
      {
        const auto data = "NPC_\x41\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x01\0\x09PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x05\0\x09\0\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of PRKZ can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: PRKZ is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\0\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no PRKR after PRKZ")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0FAIL\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of PRKR is not eight")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x07\0\xFC\x81\x05\0\x01\xD8\xC3PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x09\0\xFC\x81\x05\0\x01\xD8\xC3\x13\0PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of PRKR can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple COCTs")
    {
      const auto data = "NPC_\x5C\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0COCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of COCT is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x03\0\x01\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x05\0\x01\0\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of COCT can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: COCT is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\0\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no CNTO after COCT")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0FAIL\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CNTO is not eight")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x07\0\x87\x9B\x02\0\x01\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x09\0\x87\x9B\x02\0\x01\0\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of CNTO can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple AIDTs")
    {
      const auto data = "NPC_\x6C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of AIDT is not 20")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x13\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x15\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of AIDT can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: AIDT is missing")
    {
      const auto data = "NPC_\x38\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple KSIZs")
    {
      const auto data = "NPC_\x66\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KSIZ is not four")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x03\0\x01\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x05\0\x01\0\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before KSIZ can be read completely")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\0\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: KSIZ is zero and KWDA is missing")
    {
      const auto data = "NPC_\x48\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\0\0\0\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no KWDA")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0FAIL\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing KWDA")
    {
      const auto data = "NPC_\x48\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of KWDA is not a multiple of four or it's zero")
    {
      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x05\0Y]\x03\0\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x4E\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\0\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of KWDA can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple CNAMs")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CNAM is not four")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x03\0\x08p\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x05\0\x08p\x01\0\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of CNAM can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: CNAM is zero")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\0\0\0\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple FULL entries")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of FULL can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0\xFA"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple SHRTs")
    {
      const auto data = "NPC_\x4E\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of SHRT is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x03\0H\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x05\0H\x01\x01\0\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of SHRT can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DATAs")
    {
      const auto data = "NPC_\x92\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DATA is not zero")
    {
      const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\x01\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: no DNAM after DATA")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0FAIL4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DNAM is not 52")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM3\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM5\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of DNAM can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple ZNAMs")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of ZNAM is not four")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x03\0\x83\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x05\0\x83\x8C\x04\0\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of ZNAM can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: ZNAM is zero")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\0\0\0\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM5s")
    {
      const auto data = "NPC_\x5A\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of NAM5 is not two")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x01\0\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x03\0\xFF\0\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream ends before all of NAM5 can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM6s")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of NAM6 can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM7s")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of NAM7 can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple NAM8s")
    {
      const auto data = "NPC_\x5C\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0NAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: stream ends before all of NAM8 can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple CSCRs")
    {
      const auto data = "NPC_\x4E\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of CSCR is not four")
    {
      {
        const auto data = "NPC_\x43\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x03\0k\x04\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x05\0\x83k\x04\0\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of CSCR can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: CSCR is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\0\0\0\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple DOFTs")
    {
      const auto data = "NPC_\x4E\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A\x84\x0F\0DOFT\x04\0\x9A\x84\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of DOFT is not four")
    {
      {
        const auto data = "NPC_\x42\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x02\0\x0F\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x45\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x05\0\x9A\x84\x0F\0\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of DOFT can be read")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\x9A"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: DOFT is zero")
    {
      const auto data = "NPC_\x44\x02\0\0\0\0\0\0\x3A\x76\x03\0\x1B\x69\x55\0\x28\0\x0C\0EDID\x0C\0dunRagnOtar\0OBND\x0C\0\xE6\xFF\xEE\xFF\0\0\x1A\0\x12\0\x95\0ACBS\x18\0\x10\0\x04\0d\0\0\0\x32\0\0\0\x64\0d\0#\0\x7F\x0E\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0Cf\x10\0\0\0\0\0INAM\x04\0~\xAD\x03\0VTCK\x04\0\x86\x99\x02\0TPLT\x04\0\xC9\xA1\x08\0RNAM\x04\0\xEF\x31\x01\0SPCT\x04\0\x01\0\0\0SPLO\x04\0X\x02\x02\0WNAM\x04\0\xAB\xB5\x03\0ATKR\x04\0\xEF\x31\x01\0PRKZ\x04\0\x09\0\0\0PRKR\x08\0(1\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xFC\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\x9Ay\x0D\0\x01\xD8\xC3\x13PRKR\x08\0\xFA\xFC\x10\0\x01\xD8\xC3\x13PRKR\x08\0\x19\xB4\x0C\0\x01\xD8\xC3\x13PRKR\x08\0\xF8\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xF9\x81\x05\0\x01\xD8\xC3\x13PRKR\x08\0\xCC\x8B\x06\0\x01\xD8\xC3\x13PRKR\x08\0\xF5\x81\x05\0\x01\xD8\xC3\x13\x43OCT\x04\0\x01\0\0\0CNTO\x08\0\x87\x9B\x02\0\x01\0\0\0AIDT\x14\0\0\x04\x32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0PKID\x04\0\x8B\x44\x09\0PKID\x04\0x\x89\x08\0PKID\x04\0\xBD\xBC\x09\0CNAM\x04\0\x1C\xCE\x01\0FULL\x04\0\x9F\xC5\0\0SHRT\x04\0H\x01\x01\0DATA\0\0DNAM4\0\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x37\x0F\x0F\x64\x64\x64\x0F\x64\x0F\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xD2\x05!\x02\0\0\x03\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0e\xB3\x0B\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0\x80?NAM8\x04\0\x01\0\0\0CSCR\x04\0\x83k\x04\0DOFT\x04\0\0\0\0\0DPLT\x04\0\xE2*\x03\0QNAM\x0C\0\x81\x80\0?\x81\x80\0?\x81\x80\0?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: missing QNAM")
    {
      const auto data = "NPC_\x40\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: multiple QNAMs")
    {
      const auto data = "NPC_\x64\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0QNAM\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }

    SECTION("corrupt data: length of QNAM is not 12")
    {
      {
        const auto data = "NPC_\x51\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0B\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }

      {
        const auto data = "NPC_\x53\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0D\0\0\0\0\0\0\0\0\0\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read NPC_, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        NPCRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
      }
    }

    SECTION("corrupt data: stream end before all of QNAM can be read")
    {
      const auto data = "NPC_\x52\x01\0\0\0\0\0\x20\x05\xCA\x01\0\x1B\x69\x55\0\x28\0\x07\0EDID\x0C\0MQ104Dragon\0OBND\x0C\0\0\0\0\0\0\0\0\0\0\0\0\0ACBS\x18\0\0\0\0\0\0\0\0\0\x01\0\0\0\0\0\x64\0#\0k\x0F\0\0\0\0SNAM\x08\0\x9C-\x03\0\0\0\0\0SNAM\x08\0\x13\0\0\0\0\0\0\0SNAM\x08\0\0M\x05\0\0\0\0\0SNAM\x08\0\x93\x43\x09\0\0\0\0\0TPLT\x04\0\x86\xAE\x0F\0RNAM\x04\0|\x9C\x10\0AIDT\x14\0\x01\x04\x32\0\0\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0KSIZ\x04\0\x01\0\0\0KWDA\x04\0Y]\x03\0CNAM\x04\0\x08p\x01\0FULL\x04\0E\x88\0\0DATA\0\0DNAM4\0\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\x05\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\x35\x02\x96\0\x82\0\x01\0\0\0\0\0\x01\x8CT\0ZNAM\x04\0\x83\x8C\x04\0NAM5\x02\0\xFF\0NAM6\x04\0\0\0\x80?NAM7\x04\0\0\0HBNAM8\x04\0\x01\0\0\0QNAM\x0C\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read NPC_, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      NPCRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream, true, dummy_table) );
    }
  }
}
