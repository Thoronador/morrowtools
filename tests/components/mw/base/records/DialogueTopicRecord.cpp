/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2022  Dirk Stolle

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
#include "../../../../../mw/base/records/DialogueTopicRecord.hpp"

TEST_CASE("MWTP::DialogueTopicRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    DialogueTopicRecord record;

    REQUIRE( record.DialogueID.empty() );
    REQUIRE( record.Type == DialogueTopicType::Regular );
  }

  SECTION("equals")
  {
    DialogueTopicRecord a;
    DialogueTopicRecord b;

    SECTION("equal")
    {
      a.DialogueID = "Tree";
      a.Type = DialogueTopicType::Voice;

      b.DialogueID = "Tree";
      b.Type = DialogueTopicType::Voice;

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("recordID mismatch")
      {
        a.DialogueID = "foo";
        b.DialogueID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Type mismatch")
      {
        a.Type = DialogueTopicType::Regular;
        b.Type = DialogueTopicType::Greeting;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load regular topic")
    {
      const auto data = "DIAL\x16\0\0\0\0\0\0\0\0\0\0\0NAME\x05\0\0\0Alit\0DATA\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DialogueTopicRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.DialogueID == "Alit" );
      REQUIRE( record.Type == DialogueTopicType::Regular );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load voice")
    {
      const auto data = "DIAL\x17\0\0\0\0\0\0\0\0\0\0\0NAME\x06\0\0\0Hello\0DATA\x01\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DialogueTopicRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.DialogueID == "Hello" );
      REQUIRE( record.Type == DialogueTopicType::Voice );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load greeting")
    {
      const auto data = "DIAL\x1C\0\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0Greeting 7\0DATA\x01\0\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DialogueTopicRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.DialogueID == "Greeting 7" );
      REQUIRE( record.Type == DialogueTopicType::Greeting );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load persuasion")
    {
      const auto data = "DIAL\x1C\0\0\0\0\0\0\0\0\0\0\0NAME\x0B\0\0\0Taunt Fail\0DATA\x01\0\0\0\x03"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DialogueTopicRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.DialogueID == "Taunt Fail" );
      REQUIRE( record.Type == DialogueTopicType::Persuasion );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load journal")
    {
      const auto data = "DIAL\x18\0\0\0\0\0\0\0\0\0\0\0NAME\x07\0\0\0FG_Vas\0DATA\x01\0\0\0\x04"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DialogueTopicRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.DialogueID == "FG_Vas" );
      REQUIRE( record.Type == DialogueTopicType::Journal );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }


    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "DIAL\x16\0\0\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueTopicRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "DIAL\x16\0\0\0\0\0\0\0\0\0\0\0FAIL\x05\0\0\0Alit\0DATA\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueTopicRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 512")
    {
      const auto data = "DIAL\x16\0\0\0\0\0\0\0\0\0\0\0NAME\x05\x01\0\0Alit\0DATA\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueTopicRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "DIAL\x16\0\0\0\0\0\0\0\0\0\0\0NAME\x25\0\0\0Alit\0DATA\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueTopicRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "DIAL\x16\0\0\0\0\0\0\0\0\0\0\0NAME\x05\0\0\0Alit\0FAIL\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueTopicRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA is not one")
    {
      {
        const auto data = "DIAL\x15\0\0\0\0\0\0\0\0\0\0\0NAME\x05\x01\0\0Alit\0DATA\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip DIAL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DialogueTopicRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "DIAL\x17\0\0\0\0\0\0\0\0\0\0\0NAME\x05\x01\0\0Alit\0DATA\x02\0\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip DIAL, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DialogueTopicRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before DATA can be read")
    {
      const auto data = "DIAL\x16\0\0\0\0\0\0\0\0\0\0\0NAME\x05\0\0\0Alit\0DATA\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip DIAL, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueTopicRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
