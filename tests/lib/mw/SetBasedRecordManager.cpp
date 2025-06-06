/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021, 2023  Dirk Stolle

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
#include "../../../lib/mw/SetBasedRecordManager.hpp"
#include "../../../lib/mw/records/GlobalRecord.hpp"

TEST_CASE("MWTP::SetBasedRecordManager")
{
  using namespace MWTP;

  SECTION("get (Singleton)")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    auto& mgr2 = SetBasedRecordManager<GlobalRecord>::get();

    REQUIRE( &mgr == &mgr2 );
  }

  SECTION("addRecord / hasRecord / getRecord")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    GlobalRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.Type = GlobalType::Short;
    recordOne.floatValue = 5;

    GlobalRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordOne.Type = GlobalType::Long;
    recordOne.floatValue = 9001;

    REQUIRE_FALSE( mgr.hasRecord("TestOne") );
    REQUIRE_FALSE( mgr.hasRecord("TestTwo") );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.getNumberOfRecords() == 1 );
    REQUIRE( mgr.hasRecord("TestOne") );
    REQUIRE( mgr.getRecord("TestOne").equals(recordOne) );
    REQUIRE_FALSE( mgr.hasRecord("TestTwo") );
    mgr.addRecord(recordTwo);
    REQUIRE( mgr.getNumberOfRecords() == 2 );
    REQUIRE( mgr.hasRecord("TestOne") );
    REQUIRE( mgr.hasRecord("TestTwo") );
    REQUIRE_FALSE( mgr.hasRecord("TestThree") );
    REQUIRE( mgr.getRecord("TestTwo").equals(recordTwo) );
  }

  SECTION("id is not case-sensitive")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    GlobalRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.Type = GlobalType::Short;
    recordOne.floatValue = 5;

    REQUIRE_FALSE( mgr.hasRecord("TestOne") );
    REQUIRE_FALSE( mgr.hasRecord("TESTONE") );
    REQUIRE_FALSE( mgr.hasRecord("testone") );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.getNumberOfRecords() == 1 );
    REQUIRE( mgr.hasRecord("TestOne") );
    REQUIRE( mgr.getRecord("TestOne").equals(recordOne) );
    REQUIRE( mgr.hasRecord("TESTONE") );
    REQUIRE( mgr.getRecord("TESTONE").equals(recordOne) );
    REQUIRE( mgr.hasRecord("testone") );
    REQUIRE( mgr.getRecord("testone").equals(recordOne) );
    REQUIRE_FALSE( mgr.hasRecord("TestTwo") );
  }

  SECTION("addRecord with empty ID does not add anything")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    GlobalRecord recordEmptyID;
    recordEmptyID.recordID = "";
    recordEmptyID.Type = GlobalType::Short;
    recordEmptyID.floatValue = 5;

    REQUIRE( mgr.getNumberOfRecords() == 0 );
    mgr.addRecord(recordEmptyID);
    REQUIRE_FALSE( mgr.hasRecord("") );
    REQUIRE( mgr.getNumberOfRecords() == 0 );
  }

  SECTION("getRecord throws when ID is not present")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    REQUIRE_FALSE( mgr.hasRecord("NIL_NotInList") );
    REQUIRE_THROWS( mgr.getRecord("NIL_NotInList") );
  }

  SECTION("removeRecord")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    GlobalRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.Type = GlobalType::Short;
    recordOne.floatValue = 5;

    REQUIRE_FALSE( mgr.hasRecord("TestOne") );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.hasRecord("TestOne") );
    REQUIRE( mgr.removeRecord("TestOne") );
    REQUIRE_FALSE( mgr.hasRecord("TestOne") );
    REQUIRE_FALSE( mgr.removeRecord("TestOne") );

    REQUIRE_FALSE( mgr.removeRecord("TestTwo") );
    REQUIRE_FALSE( mgr.removeRecord("TestMore") );
  }

  SECTION("clear")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    GlobalRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.Type = GlobalType::Short;
    recordOne.floatValue = 5;

    GlobalRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordOne.Type = GlobalType::Long;
    recordOne.floatValue = 9001;

    GlobalRecord recordThree;
    recordThree.recordID = "TestThree";
    recordThree.Type = GlobalType::Float;
    recordThree.floatValue = 1.0f;

    mgr.addRecord(recordOne);
    mgr.addRecord(recordTwo);
    mgr.addRecord(recordThree);
    REQUIRE( mgr.hasRecord("TestOne") );
    REQUIRE( mgr.hasRecord("TestTwo") );
    REQUIRE( mgr.hasRecord("TestThree") );

    mgr.clear();
    REQUIRE_FALSE( mgr.hasRecord("TestOne") );
    REQUIRE_FALSE( mgr.hasRecord("TestTwo") );
    REQUIRE_FALSE( mgr.hasRecord("TestThree") );
  }

  SECTION("getNumberOfRecords")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    GlobalRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.Type = GlobalType::Short;
    recordOne.floatValue = 5;

    GlobalRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordOne.Type = GlobalType::Long;
    recordOne.floatValue = 9001;

    GlobalRecord recordThree;
    recordThree.recordID = "TestThree";
    recordThree.Type = GlobalType::Float;
    recordThree.floatValue = 1.0f;

    REQUIRE( mgr.getNumberOfRecords() == 0 );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.getNumberOfRecords() == 1 );
    mgr.addRecord(recordTwo);
    REQUIRE( mgr.getNumberOfRecords() == 2 );
    mgr.addRecord(recordThree);
    REQUIRE( mgr.getNumberOfRecords() == 3 );


    GlobalRecord recordFour;
    recordFour.recordID = "Test4";
    recordFour.Type = GlobalType::Short;
    recordFour.floatValue = 4;

    GlobalRecord recordFive;
    recordFive.recordID = "TestFive";
    recordFive.Type = GlobalType::Short;
    recordFive.floatValue = 4;

    mgr.addRecord(recordFour);
    mgr.addRecord(recordFive);
    REQUIRE( mgr.getNumberOfRecords() == 5 );
  }

  SECTION("begin / end")
  {
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    GlobalRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.Type = GlobalType::Short;
    recordOne.floatValue = 5;

    GlobalRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordOne.Type = GlobalType::Long;
    recordOne.floatValue = 9001;

    GlobalRecord recordThree;
    recordThree.recordID = "TestThree";
    recordThree.Type = GlobalType::Float;
    recordThree.floatValue = 1.0f;

    REQUIRE( mgr.begin() == mgr.end() );

    mgr.addRecord(recordOne);
    mgr.addRecord(recordTwo);
    mgr.addRecord(recordThree);

    REQUIRE_FALSE( mgr.begin() == mgr.end() );

    auto iter = mgr.begin();
    REQUIRE( iter->recordID == "TestOne" );
    ++iter;
    REQUIRE( iter->recordID == "TestThree" );
    ++iter;
    REQUIRE( iter->recordID == "TestTwo" );
    ++iter;
    REQUIRE( iter == mgr.end() );

    mgr.clear();
    REQUIRE( mgr.begin() == mgr.end() );
  }

  SECTION("readNextRecord + saveToStream: basic stuff")
  {
    using namespace std::string_view_literals;
    const auto data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80;D"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    // Skip GLOB, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasRecord("NPCVoiceDistance") );
    REQUIRE( mgr.getRecord("NPCVoiceDistance").Type == GlobalType::Short );
    REQUIRE( mgr.getRecord("NPCVoiceDistance").asShort() == 750 );
    REQUIRE( mgr.getNumberOfRecords() == 1 );

    // save it
    std::ostringstream streamOut;
    REQUIRE( mgr.saveAllToStream(streamOut) );
    // check written data
    REQUIRE( streamOut.str() == data );
  }

  SECTION("readNextRecord: reading the same record twice still yields only one record")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80;DGLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0NPCVoiceDistance\0FNAM\x01\0\0\0sFLTV\x04\0\0\0\0\x80;D"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    // Skip GLOB, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    // Skip GLOB, because header is handled before loadFromStream.
    stream.seekg(4, std::ios_base::cur);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  0 );

    REQUIRE( mgr.hasRecord("NPCVoiceDistance") );
    REQUIRE( mgr.getRecord("NPCVoiceDistance").Type == GlobalType::Short );
    REQUIRE( mgr.getRecord("NPCVoiceDistance").asShort() == 750 );
    REQUIRE( mgr.getNumberOfRecords() == 1 );
  }

  SECTION("readNextRecord + saveToStream: multiple records")
  {
    using namespace std::string_view_literals;
    const auto data = "GLOB\x24\0\0\0\0\0\0\0\0\0\0\0NAME\x07\0\0\0PCGold\0FNAM\x01\0\0\0lFLTV\x04\0\0\0\0\0\0\0GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0NAME\x11\0\0\0WerewolfClawMult\0FNAM\x01\0\0\0fFLTV\x04\0\0\0\0\0\xC8\x41"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    // Skip GLOB, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasRecord("PCGold") );
    REQUIRE( mgr.getRecord("PCGold").Type == GlobalType::Long );
    REQUIRE( mgr.getRecord("PCGold").asLong() == 0 );
    REQUIRE( mgr.getNumberOfRecords() == 1 );

    // Skip GLOB, because header is handled before loadFromStream.
    stream.seekg(4, std::ios_base::cur);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasRecord("WerewolfClawMult") );
    REQUIRE( mgr.getRecord("WerewolfClawMult").Type == GlobalType::Float );
    REQUIRE( mgr.getRecord("WerewolfClawMult").floatValue == 25.0f );
    REQUIRE( mgr.getNumberOfRecords() == 2 );

    // save it
    std::ostringstream streamOut;
    REQUIRE( mgr.saveAllToStream(streamOut) );
    // check written data
    REQUIRE( streamOut.str() == data );
  }

  SECTION("readNextRecord: failure")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "GLOB\x2E\0\0\0\0\0\0\0\0\0\0\0FAIL"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    mgr.clear();

    // Skip GLOB, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) == -1 );

    REQUIRE_FALSE( mgr.hasRecord("NPCVoiceDistance") );
    REQUIRE( mgr.getNumberOfRecords() == 0 );
  }

  SECTION("saveAllToStream: failure with bad stream")
  {
    std::stringstream stream;
    stream.str(std::string("bla"));

    uint32_t dummy = 0;
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE_FALSE( stream.good() );
    // Writing stuff should fail.
    auto& mgr = SetBasedRecordManager<GlobalRecord>::get();
    REQUIRE_FALSE( mgr.saveAllToStream(stream) );
  }
}
