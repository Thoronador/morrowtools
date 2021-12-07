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
#include <filesystem>
#include <fstream>
#include <string_view>
#include "../../../../mw/base/MapBasedRecordManager.hpp"
#include "../../../../mw/base/records/StaticRecord.hpp"

TEST_CASE("MWTP::MapBasedRecordManager")
{
  using namespace MWTP;

  SECTION("get (Singleton)")
  {
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    auto& mgr2 = MapBasedRecordManager<StaticRecord>::getSingleton();

    REQUIRE( &mgr == &mgr2 );
  }

  SECTION("addRecord / hasRecord / getRecord")
  {
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    StaticRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.ModelPath = "foo.nif";

    StaticRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordOne.ModelPath = "bar.nif";

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

  SECTION("addRecord with empty ID does not add anything")
  {
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    StaticRecord recordEmptyID;
    recordEmptyID.recordID = "";
    recordEmptyID.ModelPath = "zero.nif";

    REQUIRE( mgr.getNumberOfRecords() == 0 );
    mgr.addRecord(recordEmptyID);
    REQUIRE_FALSE( mgr.hasRecord("") );
    REQUIRE( mgr.getNumberOfRecords() == 0 );
  }

  SECTION("getRecord throws when ID is not present")
  {
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    REQUIRE_FALSE( mgr.hasRecord("NIL_NotInList") );
    REQUIRE_THROWS( mgr.getRecord("NIL_NotInList") );
  }

  SECTION("removeRecord")
  {
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    StaticRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.ModelPath = "some.nif";

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
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    StaticRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.ModelPath = "foo.nif";

    StaticRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordTwo.ModelPath = "bar.nif";

    StaticRecord recordThree;
    recordThree.recordID = "TestThree";
    recordThree.ModelPath = "another.nif";

    mgr.addRecord(recordOne);
    mgr.addRecord(recordTwo);
    mgr.addRecord(recordThree);
    REQUIRE( mgr.hasRecord("TestOne") );
    REQUIRE( mgr.hasRecord("TestTwo") );
    REQUIRE( mgr.hasRecord("TestThree") );

    mgr.clearAll();
    REQUIRE_FALSE( mgr.hasRecord("TestOne") );
    REQUIRE_FALSE( mgr.hasRecord("TestTwo") );
    REQUIRE_FALSE( mgr.hasRecord("TestThree") );
  }

  SECTION("getNumberOfRecords")
  {
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    StaticRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.ModelPath = "foo.nif";

    StaticRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordTwo.ModelPath = "bar.nif";

    StaticRecord recordThree;
    recordThree.recordID = "TestThree";
    recordThree.ModelPath = "another.nif";

    REQUIRE( mgr.getNumberOfRecords() == 0 );
    mgr.addRecord(recordOne);
    REQUIRE( mgr.getNumberOfRecords() == 1 );
    mgr.addRecord(recordTwo);
    REQUIRE( mgr.getNumberOfRecords() == 2 );
    mgr.addRecord(recordThree);
    REQUIRE( mgr.getNumberOfRecords() == 3 );


    StaticRecord recordFour;
    recordFour.recordID = "Test4";
    recordFour.ModelPath = "quattro.nif";

    StaticRecord recordFive;
    recordFive.recordID = "TestFive";
    recordFour.ModelPath = "high_5.nif";

    mgr.addRecord(recordFour);
    mgr.addRecord(recordFive);
    REQUIRE( mgr.getNumberOfRecords() == 5 );
  }

  SECTION("begin / end")
  {
    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    StaticRecord recordOne;
    recordOne.recordID = "TestOne";
    recordOne.ModelPath = "foo.nif";

    StaticRecord recordTwo;
    recordTwo.recordID = "TestTwo";
    recordTwo.ModelPath = "bar.nif";

    StaticRecord recordThree;
    recordThree.recordID = "TestThree";
    recordThree.ModelPath = "another.nif";

    REQUIRE( mgr.getBegin() == mgr.getEnd() );

    mgr.addRecord(recordOne);
    mgr.addRecord(recordTwo);
    mgr.addRecord(recordThree);

    REQUIRE_FALSE( mgr.getBegin() == mgr.getEnd() );

    auto iter = mgr.getBegin();
    REQUIRE( iter->first == "TestOne" );
    ++iter;
    REQUIRE( iter->first == "TestThree" );
    ++iter;
    REQUIRE( iter->first == "TestTwo" );
    ++iter;
    REQUIRE( iter == mgr.getEnd() );

    mgr.clearAll();
    REQUIRE( mgr.getBegin() == mgr.getEnd() );
  }

  SECTION("readNextRecord + saveToStream: basic stuff")
  {
    using namespace std::string_view_literals;
    const auto data = "STAT\x43\0\0\0\0\0\0\0\0\x04\0\0NAME\x18\0\0\0Active_MH_Forcefield_05\0MODL\x1B\0\0\0f\\Active_MH_Forcefield.NIF\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    // read STAT, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasRecord("Active_MH_Forcefield_05") );
    REQUIRE( mgr.getRecord("Active_MH_Forcefield_05").ModelPath == "f\\Active_MH_Forcefield.NIF" );
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
    const std::string_view data = "STAT\x43\0\0\0\0\0\0\0\0\x04\0\0NAME\x18\0\0\0Active_MH_Forcefield_05\0MODL\x1B\0\0\0f\\Active_MH_Forcefield.NIF\0STAT\x43\0\0\0\0\0\0\0\0\x04\0\0NAME\x18\0\0\0Active_MH_Forcefield_05\0MODL\x1B\0\0\0f\\Active_MH_Forcefield.NIF\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    // read STAT, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    // read STAT, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  0 );

    REQUIRE( mgr.hasRecord("Active_MH_Forcefield_05") );
    REQUIRE( mgr.getRecord("Active_MH_Forcefield_05").ModelPath == "f\\Active_MH_Forcefield.NIF" );
    REQUIRE( mgr.getNumberOfRecords() == 1 );
  }

  SECTION("readNextRecord + saveToStream: multiple records")
  {
    using namespace std::string_view_literals;
    const auto data = "STAT\x43\0\0\0\0\0\0\0\0\x04\0\0NAME\x18\0\0\0Active_MH_Forcefield_05\0MODL\x1B\0\0\0f\\Active_MH_Forcefield.NIF\0STAT\x3C\0\0\0\0\0\0\0\0\0\0\0NAME\x13\0\0\0Ex_MH_Lantern_Post\0MODL\x19\0\0\0x\\Ex_MH_Lantern_Post.NIF\0"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    // read STAT, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasRecord("Active_MH_Forcefield_05") );
    REQUIRE( mgr.getRecord("Active_MH_Forcefield_05").ModelPath == "f\\Active_MH_Forcefield.NIF" );
    REQUIRE( mgr.getNumberOfRecords() == 1 );

    // read STAT, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) ==  1 );

    REQUIRE( mgr.hasRecord("Ex_MH_Lantern_Post") );
    REQUIRE( mgr.getRecord("Ex_MH_Lantern_Post").ModelPath == "x\\Ex_MH_Lantern_Post.NIF" );
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
    const std::string_view data = "STAT\x43\0\0\0\0\0\0\0\0\x04\0\0FAIL"sv;
    std::istringstream stream;
    stream.str(std::string(data));

    uint32_t dummy = 0;

    auto& mgr = MapBasedRecordManager<StaticRecord>::getSingleton();
    mgr.clearAll();

    // read STAT, because header is handled before loadFromStream.
    stream.read(reinterpret_cast<char*>(&dummy), 4);
    REQUIRE( stream.good() );
    // read record
    REQUIRE( mgr.readNextRecord(stream) == -1 );

    REQUIRE_FALSE( mgr.hasRecord("Active_MH_Forcefield_05") );
    REQUIRE( mgr.getNumberOfRecords() == 0 );
  }
}
