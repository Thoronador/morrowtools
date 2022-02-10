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
#include "../../../../../mw/base/records/DialogueInfoRecord.hpp"

TEST_CASE("MWTP::DialogueInfoRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    DialogueInfoRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.PreviousInfoID.empty() );
    REQUIRE( record.NextInfoID.empty() );
    REQUIRE( record.UnknownLong == 0 );
    REQUIRE( record.Disposition == 0 );
    REQUIRE( record.Rank == 0 );
    REQUIRE( record.Gender == 0 );
    REQUIRE( record.PCRank == 0 );
    REQUIRE( record.UnknownByte == 0 );
    REQUIRE( record.ActorID.empty() );
    REQUIRE( record.RaceID.empty() );
    REQUIRE( record.ClassID.empty() );
    REQUIRE( record.FactionID.empty() );
    REQUIRE( record.CellID.empty() );
    REQUIRE( record.PCFactionID.empty() );
    REQUIRE( record.SoundFile.empty() );
    REQUIRE_FALSE( record.isQuestName );
    REQUIRE_FALSE( record.isQuestFinished );
    REQUIRE_FALSE( record.isQuestRestart );
    REQUIRE( record.Response.empty() );
    REQUIRE( record.Functions.empty() );
    REQUIRE( record.ResultString.empty() );
  }

  SECTION("equals")
  {
    DialogueInfoRecord a;
    DialogueInfoRecord b;

    SECTION("equal")
    {
      a.recordID = "foo";
      a.NextInfoID = "bar";

      b.recordID = "foo";
      b.NextInfoID = "bar";

      REQUIRE( a.equals(b) );
      REQUIRE( b.equals(a) );
    }

    SECTION("unequal")
    {
      SECTION("recordID mismatch")
      {
        a.recordID = "foo";
        b.recordID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PreviousInfoID mismatch")
      {
        a.PreviousInfoID = "foo";
        b.PreviousInfoID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("NextInfoID mismatch")
      {
        a.NextInfoID = "foo";
        b.NextInfoID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("UnknownLong mismatch")
      {
        a.UnknownLong = 1;
        b.UnknownLong = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Disposition mismatch")
      {
        a.Disposition = 1;
        b.Disposition = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Rank mismatch")
      {
        a.Rank = 1;
        b.Rank = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Gender mismatch")
      {
        a.Gender = 1;
        b.Gender = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PCRank mismatch")
      {
        a.PCRank = 1;
        b.PCRank = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("UnknownByte mismatch")
      {
        a.UnknownByte = 1;
        b.UnknownByte = 2;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ActorID mismatch")
      {
        a.ActorID = "foo";
        b.ActorID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("RaceID mismatch")
      {
        a.RaceID = "foo";
        b.RaceID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ClassID mismatch")
      {
        a.ClassID = "foo";
        b.ClassID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("FactionID mismatch")
      {
        a.FactionID = "foo";
        b.FactionID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("CellID mismatch")
      {
        a.CellID = "foo";
        b.CellID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("PCFactionID mismatch")
      {
        a.PCFactionID = "foo";
        b.PCFactionID = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SoundFile mismatch")
      {
        a.SoundFile = "foo";
        b.SoundFile = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("isQuestName mismatch")
      {
        a.isQuestName = true;
        b.isQuestName = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("isQuestFinished mismatch")
      {
        a.isQuestFinished = true;
        b.isQuestFinished = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("isQuestRestart mismatch")
      {
        a.isQuestRestart = true;
        b.isQuestRestart = false;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Response mismatch")
      {
        a.Response = "foo";
        b.Response = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Functions mismatch")
      {
        a.Functions.clear();
        b.Functions.push_back(FuncVarRecord());

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ResultString mismatch")
      {
        a.ResultString = "foo";
        b.ResultString = "bar";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    SECTION("default: load info")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      DialogueInfoRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "226471411756873695" );
      REQUIRE( record.PreviousInfoID.empty() );
      REQUIRE( record.NextInfoID.empty() );
      REQUIRE( record.UnknownLong == 0 );
      REQUIRE( record.Disposition == 0 );
      REQUIRE( record.Rank == 7 );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0 );
      REQUIRE( record.ActorID.empty() );
      REQUIRE( record.RaceID.empty() );
      REQUIRE( record.ClassID.empty() );
      REQUIRE( record.FactionID == "Mages Guild" );
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.PCFactionID.empty() );
      REQUIRE( record.SoundFile.empty() );
      REQUIRE_FALSE( record.isQuestName );
      REQUIRE_FALSE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren." );
      REQUIRE( record.Functions.size() == 1 );
      REQUIRE( record.Functions[0].Index == 51 );
      REQUIRE( record.Functions[0].Type == 52 );
      REQUIRE( record.Functions[0].Function == 0x584A );
      REQUIRE( record.Functions[0].CompareOp == 52 );
      REQUIRE( record.Functions[0].Name == "MG_Advancement" );
      REQUIRE_FALSE( record.Functions[0].isFloat );
      REQUIRE( record.Functions[0].iVal == 60 );
      REQUIRE( record.ResultString.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no INAM")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0FAIL\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INAM > 256")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\x01\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INAM is beyond stream")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no PNAM")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0FAIL\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PNAM > 256")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\x01\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of PNAM is beyond stream")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x91\0\0\0N"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NNAM")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NONE\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NNAM > 256")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\x01\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NNAM is beyond stream")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\xA1\0\0\0foo\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no DATA")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0FAIL\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DATA is not twelve")
    {
      {
        const auto data = "INFO\x44\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0B\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip INFO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DialogueInfoRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "INFO\x46\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0D\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip INFO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DialogueInfoRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before DATA can be read")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FAIL\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple FNAMs")
    {
      const auto data = "INFO\x59\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\x01\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\xFE\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple NAMEs")
    {
      const auto data = "INFO\x50\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.NAME\x03\0\0\0fooSCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 512")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\x02\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xF1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCVR below five")
    {
      const auto data = "INFO\x35\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x03\0\0\0\x33\x34JINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCVR above 260")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\x01\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCVR is beyond stream")
    {;
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x23\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no INTV/FLTV after SCVR")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementFAIL\x04\0\0\0<\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of INTV is not four")
    {
      {
        const auto data = "INFO\x44\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x03\0\0\0<\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip INFO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DialogueInfoRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "INFO\x46\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x05\0\0\0<\0\0\0\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // Skip INFO, because header is handled before loadFromStream.
        stream.seekg(4);
        REQUIRE( stream.good() );

        // Reading should fail.
        DialogueInfoRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before INTV can be read")
    {
      const auto data = "INFO\x45\x01\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x32\x32\x36\x34\x37\x31\x34\x31\x31\x37\x35\x36\x38\x37\x33\x36\x39\x35\0PNAM\x01\0\0\0\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\x07\xFF\xFF\0FNAM\x0C\0\0\0Mages Guild\0NAME\xC1\0\0\0Es ist eine H\xF6hle an der Westk\xFCste von Sheogorad, westlich von Dagon Fel. Bevor man die K\xFCste erreicht, passiert man eine kleine Bucht westlich von Dagon Fel. Lasst Euch davon nicht irritieren.SCVR\x13\0\0\0\x33\x34JX4MG_AdvancementINTV\x04\0\0\0<\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
