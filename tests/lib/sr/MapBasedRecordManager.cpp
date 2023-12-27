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
#include <fstream>
#include <string_view>
#include "../../../lib/sr/MapBasedRecordManager.hpp"
#include "../../../lib/sr/records/ActionRecord.hpp"

TEST_CASE("MapBasedRecordManager")
{
  using namespace SRTP;

  SECTION("get (Singleton)")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    auto& mgr2 = MapBasedRecordManager<ActionRecord>::get();

    REQUIRE( &mgr == &mgr2 );
  }

  SECTION("addRecord / hasRecord / getRecord")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    ActionRecord recordOne;
    recordOne.headerFormID = 0xDEADBEEF;
    recordOne.editorID = "TestOne";

    ActionRecord recordTwo;
    recordTwo.headerFormID = 0xF00BAA12;
    recordTwo.editorID = "TestTwo";

    REQUIRE_FALSE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE_FALSE( mgr.hasRecord(0xF00BAA12) );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.getNumberOfRecords() == 1 );
    REQUIRE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE( mgr.getRecord(0xDEADBEEF).equals(recordOne) );
    REQUIRE_FALSE( mgr.hasRecord(0xF00BAA12) );
    mgr.addRecord(recordTwo);
    REQUIRE( mgr.getNumberOfRecords() == 2 );
    REQUIRE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE( mgr.hasRecord(0xF00BAA12) );
    REQUIRE_FALSE( mgr.hasRecord(0xF00F0000) );
    REQUIRE( mgr.getRecord(0xF00BAA12).equals(recordTwo) );
  }

  SECTION("addRecord with ID zero does not add anything")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    ActionRecord recordZero;
    recordZero.headerFormID = 0x00000000;
    recordZero.editorID = "TestZero";

    REQUIRE( mgr.getNumberOfRecords() == 0 );
    mgr.addRecord(recordZero);
    REQUIRE_FALSE( mgr.hasRecord(0x00000000) );
    REQUIRE( mgr.getNumberOfRecords() == 0 );
  }

  SECTION("getRecord throws when ID is not present")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    REQUIRE_FALSE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE_THROWS( mgr.getRecord(0xDEADBEEF) );
  }

  SECTION("removeRecord")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    ActionRecord recordOne;
    recordOne.headerFormID = 0xDEADBEEF;
    recordOne.editorID = "TestOne";

    REQUIRE_FALSE( mgr.hasRecord(0xDEADBEEF) );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE( mgr.removeRecord(0xDEADBEEF) == 1 );
    REQUIRE_FALSE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE( mgr.removeRecord(0xDEADBEEF) == 0 );

    REQUIRE( mgr.removeRecord(0xF00BAA12) == 0 );
    REQUIRE( mgr.removeRecord(0x000F0000) == 0 );
  }

  SECTION("clear")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    ActionRecord recordOne;
    recordOne.headerFormID = 0xDEADBEEF;
    recordOne.editorID = "TestOne";

    ActionRecord recordTwo;
    recordTwo.headerFormID = 0xF00BAA12;
    recordTwo.editorID = "TestTwo";

    ActionRecord recordThree;
    recordThree.headerFormID = 0x00000333;
    recordThree.editorID = "TestThree";

    mgr.addRecord(recordOne);
    mgr.addRecord(recordTwo);
    mgr.addRecord(recordThree);
    REQUIRE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE( mgr.hasRecord(0xF00BAA12) );
    REQUIRE( mgr.hasRecord(0x00000333) );

    mgr.clear();
    REQUIRE_FALSE( mgr.hasRecord(0xDEADBEEF) );
    REQUIRE_FALSE( mgr.hasRecord(0xF00BAA12) );
    REQUIRE_FALSE( mgr.hasRecord(0x00000333) );
  }

  SECTION("getNumberOfRecords")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    ActionRecord recordOne;
    recordOne.headerFormID = 0xDEADBEEF;
    recordOne.editorID = "TestOne";

    ActionRecord recordTwo;
    recordTwo.headerFormID = 0xF00BAA12;
    recordTwo.editorID = "TestTwo";

    ActionRecord recordThree;
    recordThree.headerFormID = 0x00000333;
    recordThree.editorID = "TestThree";

    REQUIRE( mgr.getNumberOfRecords() == 0 );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.getNumberOfRecords() == 1 );
    mgr.addRecord(recordTwo);
    REQUIRE( mgr.getNumberOfRecords() == 2 );
    mgr.addRecord(recordThree);
    REQUIRE( mgr.getNumberOfRecords() == 3 );

    ActionRecord recordFour;
    recordFour.headerFormID = 0x00004444;
    recordFour.editorID = "Test4";

    ActionRecord recordFive;
    recordFive.headerFormID = 0x00055555;
    recordFive.editorID = "TestFive";

    mgr.addRecord(recordFour);
    mgr.addRecord(recordFive);
    REQUIRE( mgr.getNumberOfRecords() == 5 );
  }

  SECTION("begin / end")
  {
    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    ActionRecord recordOne;
    recordOne.headerFormID = 0xDEADBEEF;
    recordOne.editorID = "TestOne";

    ActionRecord recordTwo;
    recordTwo.headerFormID = 0xF00BAA12;
    recordTwo.editorID = "TestTwo";

    ActionRecord recordThree;
    recordThree.headerFormID = 0x00000333;
    recordThree.editorID = "TestThree";

    REQUIRE( mgr.begin() == mgr.end() );

    mgr.addRecord(recordOne);
    mgr.addRecord(recordTwo);
    mgr.addRecord(recordThree);

    REQUIRE_FALSE( mgr.begin() == mgr.end() );

    auto iter = mgr.begin();
    REQUIRE( iter->first == 0x00000333 );
    ++iter;
    REQUIRE( iter->first == 0xDEADBEEF );
    ++iter;
    REQUIRE( iter->first == 0xF00BAA12 );
    ++iter;
    REQUIRE( iter == mgr.end() );

    mgr.clear();
    REQUIRE( mgr.begin() == mgr.end() );
  }

  SECTION("readNextRecord + saveToStream: basic stuff")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    StringTable dummyTable;

    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    // Skip AACT, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream, true, dummyTable) ==  1 );

    REQUIRE( mgr.hasRecord(0x00094065) );
    REQUIRE( mgr.getRecord(0x00094065).editorID == "ActionShieldChange" );
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
    const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    StringTable dummyTable;

    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    // Skip AACT, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream, true, dummyTable) ==  1 );

    // Skip AACT, because header is handled before loadFromStream.
    stream.seekg(4, std::ios_base::cur);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream, true, dummyTable) ==  0 );

    REQUIRE( mgr.hasRecord(0x00094065) );
    REQUIRE( mgr.getRecord(0x00094065).editorID == "ActionShieldChange" );
    REQUIRE( mgr.getNumberOfRecords() == 1 );
  }

  SECTION("readNextRecord + saveToStream: multiple records")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x13\0ActionShieldChange\0AACT\x16\0\0\0\0\0\0\0\x66\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x10\0ActionShieldTwo\0AACT\x12\0\0\0\0\0\0\0\x67\x40\x09\0\x11\x60\x0C\0\x1F\0\x01\0EDID\x0C\0ActionThree\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    StringTable dummyTable;

    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    // Skip AACT, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream, true, dummyTable) ==  1 );

    REQUIRE( mgr.hasRecord(0x00094065) );
    REQUIRE( mgr.getRecord(0x00094065).editorID == "ActionShieldChange" );
    REQUIRE( mgr.getNumberOfRecords() == 1 );

    // Skip AACT, because header is handled before loadFromStream.
    stream.seekg(4, std::ios_base::cur);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream, true, dummyTable) ==  1 );

    REQUIRE( mgr.hasRecord(0x00094066) );
    REQUIRE( mgr.getRecord(0x00094066).editorID == "ActionShieldTwo" );
    REQUIRE( mgr.getNumberOfRecords() == 2 );

    // Skip AACT, because header is handled before loadFromStream.
    stream.seekg(4, std::ios_base::cur);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream, true, dummyTable) ==  1 );

    REQUIRE( mgr.hasRecord(0x00094067) );
    REQUIRE( mgr.getRecord(0x00094067).editorID == "ActionThree" );
    REQUIRE( mgr.getNumberOfRecords() == 3 );

    // save it
    std::ostringstream streamOut;
    REQUIRE( mgr.saveAllToStream(streamOut) );
    // check written data
    REQUIRE( streamOut.str() == data );
  }

  SECTION("readNextRecord: failure")
  {
    using namespace std::string_view_literals;
    const std::string_view data = "AACT\x19\0\0\0\0\0\0\0\x65\x40\x09\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    StringTable dummyTable;

    auto& mgr = MapBasedRecordManager<ActionRecord>::get();
    mgr.clear();

    // Skip AACT, because header is handled before loadFromStream.
    stream.seekg(4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream, true, dummyTable) == -1 );

    REQUIRE_FALSE( mgr.hasRecord(0x00094065) );
    REQUIRE( mgr.getNumberOfRecords() == 0 );
  }
}
