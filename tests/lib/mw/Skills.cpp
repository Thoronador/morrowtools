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

#include "../locate_catch.hpp"
#include <filesystem>
#include <string_view>
#include "../../../lib/mw/Skills.hpp"

TEST_CASE("MWTP::Skills")
{
  using namespace MWTP;

  SECTION("get (Singleton)")
  {
    auto& mgr = Skills::get();
    auto& mgr2 = Skills::get();

    REQUIRE( &mgr == &mgr2 );
  }

  SECTION("addSkill / hasSkill / getSkill")
  {
    auto& mgr = Skills::get();
    mgr.clear();

    SkillRecord recordOne;
    recordOne.SkillIndex = 1;
    recordOne.Specialization = SkillSpecialization::Magic;

    SkillRecord recordTwo;
    recordTwo.SkillIndex = 2;
    recordTwo.Specialization = SkillSpecialization::Stealth;

    REQUIRE_FALSE( mgr.hasSkill(1) );
    REQUIRE_FALSE( mgr.hasSkill(2) );
    mgr.addSkill(recordOne);
    REQUIRE( mgr.getNumberOfSkills() == 1 );
    REQUIRE( mgr.hasSkill(1) );
    REQUIRE( mgr.getSkill(1).equals(recordOne) );
    REQUIRE_FALSE( mgr.hasSkill(2) );
    mgr.addSkill(recordTwo);
    REQUIRE( mgr.getNumberOfSkills() == 2 );
    REQUIRE( mgr.hasSkill(1) );
    REQUIRE( mgr.hasSkill(2) );
    REQUIRE_FALSE( mgr.hasSkill(3) );
    REQUIRE( mgr.getSkill(2).equals(recordTwo) );
  }

  SECTION("getSkill throws when ID is not present")
  {
    auto& mgr = Skills::get();
    mgr.clear();

    REQUIRE_FALSE( mgr.hasSkill(5) );
    REQUIRE_THROWS( mgr.getSkill(5) );
  }

  SECTION("clear")
  {
    auto& mgr = Skills::get();
    mgr.clear();

    SkillRecord recordOne;
    recordOne.SkillIndex = 1;
    recordOne.Specialization = SkillSpecialization::Magic;

    SkillRecord recordTwo;
    recordTwo.SkillIndex = 2;
    recordTwo.Specialization = SkillSpecialization::Stealth;

    SkillRecord recordThree;
    recordThree.SkillIndex = 3;
    recordThree.Specialization = SkillSpecialization::Combat;

    mgr.addSkill(recordOne);
    mgr.addSkill(recordTwo);
    mgr.addSkill(recordThree);
    REQUIRE( mgr.hasSkill(1) );
    REQUIRE( mgr.hasSkill(2) );
    REQUIRE( mgr.hasSkill(3) );

    mgr.clear();
    REQUIRE_FALSE( mgr.hasSkill(1) );
    REQUIRE_FALSE( mgr.hasSkill(2) );
    REQUIRE_FALSE( mgr.hasSkill(3) );
  }

  SECTION("getNumberOfSkills")
  {
    auto& mgr = Skills::get();
    mgr.clear();

    SkillRecord recordOne;
    recordOne.SkillIndex = 1;
    recordOne.Specialization = SkillSpecialization::Magic;

    SkillRecord recordTwo;
    recordTwo.SkillIndex = 2;
    recordTwo.Specialization = SkillSpecialization::Stealth;

    SkillRecord recordThree;
    recordThree.SkillIndex = 3;
    recordThree.Specialization = SkillSpecialization::Combat;

    REQUIRE( mgr.getNumberOfSkills() == 0 );
    mgr.addSkill(recordOne);
    REQUIRE( mgr.getNumberOfSkills() == 1 );
    mgr.addSkill(recordTwo);
    REQUIRE( mgr.getNumberOfSkills() == 2 );
    mgr.addSkill(recordThree);
    REQUIRE( mgr.getNumberOfSkills() == 3 );


    SkillRecord recordFour;
    recordFour.SkillIndex = 4;
    recordFour.Specialization = SkillSpecialization::Magic;

    SkillRecord recordFive;
    recordFive.SkillIndex = 5;
    recordFive.Specialization = SkillSpecialization::Stealth;

    mgr.addSkill(recordFour);
    mgr.addSkill(recordFive);
    REQUIRE( mgr.getNumberOfSkills() == 5 );
  }

  SECTION("begin / end")
  {
    auto& mgr = Skills::get();
    mgr.clear();

    SkillRecord recordOne;
    recordOne.SkillIndex = 1;
    recordOne.Specialization = SkillSpecialization::Magic;

    SkillRecord recordTwo;
    recordTwo.SkillIndex = 2;
    recordTwo.Specialization = SkillSpecialization::Stealth;

    SkillRecord recordThree;
    recordThree.SkillIndex = 3;
    recordThree.Specialization = SkillSpecialization::Combat;

    REQUIRE( mgr.begin() == mgr.end() );

    mgr.addSkill(recordOne);
    mgr.addSkill(recordTwo);
    mgr.addSkill(recordThree);

    REQUIRE_FALSE( mgr.begin() == mgr.end() );

    auto iter = mgr.begin();
    REQUIRE( iter->first == 1 );
    ++iter;
    REQUIRE( iter->first == 2 );
    ++iter;
    REQUIRE( iter->first == 3 );
    ++iter;
    REQUIRE( iter == mgr.end() );

    mgr.clear();
    REQUIRE( mgr.begin() == mgr.end() );
  }

  SECTION("readNextRecord + saveToStream: basic stuff")
  {
    using namespace std::string_view_literals;
    const auto data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = Skills::get();
    mgr.clear();

    // read SKIL, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasSkill(19) );
    REQUIRE( mgr.getSkill(19).Attribute == 3 );
    REQUIRE( mgr.getNumberOfSkills() == 1 );

    // save it
    std::ostringstream streamOut;
    REQUIRE( mgr.saveAllToStream(streamOut) );
    // check written data
    REQUIRE( streamOut.str() == data );
  }

  SECTION("readNextRecord: reading the same record twice still yields only one record")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe.SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = Skills::get();
    mgr.clear();

    // read SKIL, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    // read SKIL, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  0 );

    REQUIRE( mgr.hasSkill(19) );
    REQUIRE( mgr.getSkill(19).Attribute == 3 );
    REQUIRE( mgr.getNumberOfSkills() == 1 );
  }

  SECTION("readNextRecord + saveToStream: multiple records")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x13\0\0\0SKDT\x18\0\0\0\x03\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe.SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0INDX\x04\0\0\0\x14\0\0\0SKDT\x18\0\0\0\x02\0\0\0\x02\0\0\0\0\0\x80>\0\0\0@\0\0\x80?\0\0\x80?DESC\x87\0\0\0Diese Fertigkeit besteht darin, sich ungesehen und ungeh\xF6rt zu bewegen. Versierte Schleicher sind gleichzeitig meist gute Taschendiebe."sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = Skills::get();
    mgr.clear();

    // read SKIL, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasSkill(19) );
    REQUIRE( mgr.getSkill(19).Attribute == 3 );
    REQUIRE( mgr.getNumberOfSkills() == 1 );

    // read SKIL, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasSkill(20) );
    REQUIRE( mgr.getSkill(20).Attribute == 2 );
    REQUIRE( mgr.getNumberOfSkills() == 2 );

    // save it
    std::ostringstream streamOut;
    REQUIRE( mgr.saveAllToStream(streamOut) );
    // check written data
    REQUIRE( streamOut.str() == data );
  }

  SECTION("readNextRecord: failure")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "SKIL\xBB\0\0\0\0\0\0\0\0\0\0\0FAIL"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = Skills::get();
    mgr.clear();

    // read SKIL, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) == -1 );

    REQUIRE_FALSE( mgr.hasSkill(19) );
    REQUIRE( mgr.getNumberOfSkills() == 0 );
  }

  SECTION("saveAllToStream: failure with bad stream")
  {
    std::stringstream stream;
    stream.str(std::string("bla"));

    uint32_t dummy = 0;
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE_FALSE( stream.good() );
    // Writing stuff should fail.
    auto& mgr = Skills::get();
    REQUIRE_FALSE( mgr.saveAllToStream(stream) );
  }

  SECTION("getSettingNameForSkill: known effects")
  {
    REQUIRE( Skills::getSettingNameForSkill(SkillBlock) == "sSkillBlock");
    REQUIRE( Skills::getSettingNameForSkill(SkillArmorer) == "sSkillArmorer" );
    REQUIRE( Skills::getSettingNameForSkill(SkillMediumArmor) == "sSkillMediumarmor" );
    REQUIRE( Skills::getSettingNameForSkill(SkillHeavyArmor) == "sSkillHeavyarmor" );
    REQUIRE( Skills::getSettingNameForSkill(SkillBluntWeapon) == "sSkillBluntweapon" );
    REQUIRE( Skills::getSettingNameForSkill(SkillLongBlade) == "sSkillLongblade" );
    REQUIRE( Skills::getSettingNameForSkill(SkillAxe) == "sSkillAxe" );
    REQUIRE( Skills::getSettingNameForSkill(SkillSpear) == "sSkillSpear" );
    REQUIRE( Skills::getSettingNameForSkill(SkillAthletics) == "sSkillAthletics" );
    REQUIRE( Skills::getSettingNameForSkill(SkillEnchant) == "sSkillEnchant" );
    REQUIRE( Skills::getSettingNameForSkill(SkillDestruction) == "sSkillDestruction" );
    REQUIRE( Skills::getSettingNameForSkill(SkillAlteration) == "sSkillAlteration" );
    REQUIRE( Skills::getSettingNameForSkill(SkillIllusion) == "sSkillIllusion" );
    REQUIRE( Skills::getSettingNameForSkill(SkillConjuration) == "sSkillConjuration" );
    REQUIRE( Skills::getSettingNameForSkill(SkillMysticism) == "sSkillMysticism" );
    REQUIRE( Skills::getSettingNameForSkill(SkillRestoration) == "sSkillRestoration" );
    REQUIRE( Skills::getSettingNameForSkill(SkillAlchemy) == "sSkillAlchemy" );
    REQUIRE( Skills::getSettingNameForSkill(SkillUnarmored) == "sSkillUnarmored" );
    REQUIRE( Skills::getSettingNameForSkill(SkillSecurity) == "sSkillSecurity" );
    REQUIRE( Skills::getSettingNameForSkill(SkillSneak) == "sSkillSneak" );
    REQUIRE( Skills::getSettingNameForSkill(SkillAcrobatics) == "sSkillAcrobatics" );
    REQUIRE( Skills::getSettingNameForSkill(SkillLightArmor) == "sSkillLightarmor" );
    REQUIRE( Skills::getSettingNameForSkill(SkillShortBlade) == "sSkillShortblade" );
    REQUIRE( Skills::getSettingNameForSkill(SkillMarksman) == "sSkillMarksman" );
    REQUIRE( Skills::getSettingNameForSkill(SkillMercantile) == "sSkillMercantile" );
    REQUIRE( Skills::getSettingNameForSkill(SkillSpeechcraft) == "sSkillSpeechcraft" );
    REQUIRE( Skills::getSettingNameForSkill(SkillHandToHand) == "sSkillHandtohand" );
  }

  SECTION("getSettingNameForSkill: throw on unknown index")
  {
    REQUIRE_THROWS( Skills::getSettingNameForSkill(150) );
  }
}
