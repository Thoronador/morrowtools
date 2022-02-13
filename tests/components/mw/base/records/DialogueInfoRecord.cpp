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

    SECTION("default: load info with float value in function data")
    {
      const auto data = "INFO\x02\x01\0\0\0\0\0\0\0\0\0\0\x49\x4E\x41\x4D\x14\0\0\0\x37\x33\x37\x32\x31\x31\x38\x34\x38\x32\x39\x31\x37\x33\x31\x30\x30\x35\x32\0PNAM\x14\0\0\0\x32\x33\x35\x37\x35\x32\x30\x39\x32\x33\x32\x32\x30\x39\x33\x30\x33\x35\x36\0NNAM\x15\0\0\0\x31\x31\x39\x35\x35\x32\x39\x31\x35\x32\x32\x34\x39\x34\x33\x32\x35\x33\x33\x35\0DATA\x0C\0\0\0\x02\xF8\x12\x01\0\0\0\0\xFF\xFF\xFF\0ONAM\x11\0\0\0lugrub gro-ogdum\0NAME)\0\0\0Ich dachte, ich h\xE4tte Euch fortgeschickt.SCVR\x0A\0\0\0\x30\x33\x66X2timerFLTV\x04\0\0\0\0\0 ABNAM)\0\0\0SetFight 100\r\nStartCombat Player\r\nGoodbye"sv;
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
      REQUIRE( record.recordID == "7372118482917310052" );
      REQUIRE( record.PreviousInfoID == "2357520923220930356" );
      REQUIRE( record.NextInfoID == "11955291522494325335" );
      REQUIRE( record.UnknownLong == 0x0112F802 );
      REQUIRE( record.Disposition == 0 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0 );
      REQUIRE( record.ActorID == "lugrub gro-ogdum" );
      REQUIRE( record.RaceID.empty() );
      REQUIRE( record.ClassID.empty() );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.PCFactionID.empty() );
      REQUIRE( record.SoundFile.empty() );
      REQUIRE_FALSE( record.isQuestName );
      REQUIRE_FALSE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "Ich dachte, ich h\xE4tte Euch fortgeschickt." );
      REQUIRE( record.Functions.size() == 1 );
      REQUIRE( record.Functions[0].Index == 48 );
      REQUIRE( record.Functions[0].Type == 51 );
      REQUIRE( record.Functions[0].Function == 0x5866 );
      REQUIRE( record.Functions[0].CompareOp == 50 );
      REQUIRE( record.Functions[0].Name == "timer" );
      REQUIRE( record.Functions[0].isFloat );
      REQUIRE( record.Functions[0].fVal == 10.0f );
      REQUIRE( record.ResultString == "SetFight 100\r\nStartCombat Player\r\nGoodbye" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load info with actor id")
    {
      const auto data = "INFO\x6F\0\0\0\0\0\0\0\0\0\0\0INAM\x14\0\0\0\x37\x35\x32\x39\x31\x39\x39\x31\x35\x31\x30\x31\x39\x39\x32\x32\x36\x39\x30\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0984727539316418937\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ONAM\x08\0\0\0Rabinna\0NAME\x03\0\0\0Ja?"sv;
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
      REQUIRE( record.recordID == "7529199151019922690" );
      REQUIRE( record.PreviousInfoID.empty() );
      REQUIRE( record.NextInfoID == "984727539316418937" );
      REQUIRE( record.UnknownLong == 0 );
      REQUIRE( record.Disposition == 0 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0 );
      REQUIRE( record.ActorID == "Rabinna" );
      REQUIRE( record.RaceID.empty() );
      REQUIRE( record.ClassID.empty() );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.PCFactionID.empty() );
      REQUIRE( record.SoundFile.empty() );
      REQUIRE_FALSE( record.isQuestName );
      REQUIRE_FALSE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "Ja?" );
      REQUIRE( record.Functions.empty() );
      REQUIRE( record.ResultString.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load info with race id and sound file")
    {
      const auto data = "INFO\x93\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x32\x31\x35\x32\x36\x39\x31\x36\x39\x38\x37\x34\x31\x33\x32\x33\x33\0PNAM\x12\0\0\0\x33\x32\x33\x38\x32\x38\x33\x36\x31\x36\x34\x36\x36\x33\x36\x35\x38\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x01\0\0\0\0\0\0\0\xFF\0\xFF\0RNAM\x09\0\0\0Imperial\0SNAM\x15\0\0\0vo\\i\\m\\Idl_IM001.mp3\0NAME\x0C\0\0\0[Schn\xFC\x66\x66\x65ln]"sv;
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
      REQUIRE( record.recordID == "21526916987413233" );
      REQUIRE( record.PreviousInfoID == "32382836164663658" );
      REQUIRE( record.NextInfoID.empty() );
      REQUIRE( record.UnknownLong == 0x00000001 );
      REQUIRE( record.Disposition == 0 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0x00 );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0 );
      REQUIRE( record.ActorID.empty() );
      REQUIRE( record.RaceID == "Imperial" );
      REQUIRE( record.ClassID.empty() );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.PCFactionID.empty() );
      REQUIRE( record.SoundFile == "vo\\i\\m\\Idl_IM001.mp3" );
      REQUIRE_FALSE( record.isQuestName );
      REQUIRE_FALSE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "[Schn\xFC\x66\x66\x65ln]" );
      REQUIRE( record.Functions.empty() );
      REQUIRE( record.ResultString.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load info with class id and result string")
    {
      const auto data = "INFO\xA7\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x31\x34\x36\x38\x32\x34\x38\x39\x34\x34\x30\x39\x32\x34\x39\x37\x36\0PNAM\x14\0\0\0\x32\x31\x38\x34\x31\x32\x39\x32\x38\x30\x32\x31\x36\x35\x34\x31\x35\x31\x35\0NNAM\x15\0\0\0\x31\x34\x33\x37\x38\x33\x31\x36\x35\x31\x32\x30\x30\x33\x35\x31\x35\x39\x37\x30\0DATA\x0C\0\0\0\x02\0\0\0\0\0\0\0\xFF\xFF\xFF\0CNAM\x09\0\0\0Publican\0NAME\x0E\0\0\0Was wollt Ihr?BNAM\x11\0\0\0;publican default"sv;
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
      REQUIRE( record.recordID == "14682489440924976" );
      REQUIRE( record.PreviousInfoID == "2184129280216541515" );
      REQUIRE( record.NextInfoID == "14378316512003515970" );
      REQUIRE( record.UnknownLong == 0x00000002 );
      REQUIRE( record.Disposition == 0 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0 );
      REQUIRE( record.ActorID.empty() );
      REQUIRE( record.RaceID.empty() );
      REQUIRE( record.ClassID == "Publican" );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.PCFactionID.empty() );
      REQUIRE( record.SoundFile.empty() );
      REQUIRE_FALSE( record.isQuestName );
      REQUIRE_FALSE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "Was wollt Ihr?" );
      REQUIRE( record.Functions.empty() );
      REQUIRE( record.ResultString == ";publican default" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load info with cell id and PC faction id")
    {
      const auto data = "INFO\xBF\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x34\x39\x32\x34\x31\x32\x39\x34\x31\x33\x35\x34\x31\x35\x36\x37\x36\0PNAM\x12\0\0\099367325152192966\0NNAM\x13\0\0\0971627586828521359\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ANAM#\0\0\0Sadrith Mora, Zur Dreckigen Muriel\0DNAM\x0E\0\0\0Thieves Guild\0NAME\x13\0\0\0Sie ist hier. Oben."sv;
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
      REQUIRE( record.recordID == "49241294135415676" );
      REQUIRE( record.PreviousInfoID == "99367325152192966" );
      REQUIRE( record.NextInfoID == "971627586828521359" );
      REQUIRE( record.UnknownLong == 0x00000000 );
      REQUIRE( record.Disposition == 0 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0 );
      REQUIRE( record.ActorID.empty() );
      REQUIRE( record.RaceID.empty() );
      REQUIRE( record.ClassID.empty() );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.CellID == "Sadrith Mora, Zur Dreckigen Muriel" );
      REQUIRE( record.PCFactionID == "Thieves Guild" );
      REQUIRE( record.SoundFile.empty() );
      REQUIRE_FALSE( record.isQuestName );
      REQUIRE_FALSE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "Sie ist hier. Oben." );
      REQUIRE( record.Functions.empty() );
      REQUIRE( record.ResultString.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load info with quest name")
    {
      const auto data = "INFO\x78\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x30\x33\x30\x38\x31\x35\x33\x31\x37\x32\x32\x38\x32\x35\x31\x37\x39\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0\x33\x32\x31\x39\x33\x36\x30\x32\x32\x32\x36\x38\x35\x35\x33\x30\x37\x36\0DATA\x0C\0\0\0\x04\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\x06NAME\x14\0\0\0Eine Insel im NordenQSTN\x01\0\0\0\x01"sv;
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
      REQUIRE( record.recordID == "103081531722825179" );
      REQUIRE( record.PreviousInfoID.empty() );
      REQUIRE( record.NextInfoID == "321936022268553076" );
      REQUIRE( record.UnknownLong == 0xFFFFFF04 );
      REQUIRE( record.Disposition == 0 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0x06 );
      REQUIRE( record.ActorID.empty() );
      REQUIRE( record.RaceID.empty() );
      REQUIRE( record.ClassID.empty() );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.PCFactionID.empty() );
      REQUIRE( record.SoundFile.empty() );
      REQUIRE( record.isQuestName );
      REQUIRE_FALSE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "Eine Insel im Norden" );
      REQUIRE( record.Functions.empty() );
      REQUIRE( record.ResultString.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load info with quest finisher")
    {
      const auto data = "INFO\x7E\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x36\x34\x32\x31\x34\x31\x38\x38\x33\x30\x35\x39\x35\x36\x36\x30\x30\0PNAM\x14\0\0\09503227231245829738\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x04\xF8\x12\x01,\x01\0\0\xFF\xFF\xFF\0NAME\x19\0\0\0Meine Festung ist fertig.QSTF\x01\0\0\0\x01"sv;
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
      REQUIRE( record.recordID == "164214188305956600" );
      REQUIRE( record.PreviousInfoID == "9503227231245829738" );
      REQUIRE( record.NextInfoID.empty() );
      REQUIRE( record.UnknownLong == 0x0112F804 );
      REQUIRE( record.Disposition == 300 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
      REQUIRE( record.UnknownByte == 0 );
      REQUIRE( record.ActorID.empty() );
      REQUIRE( record.RaceID.empty() );
      REQUIRE( record.ClassID.empty() );
      REQUIRE( record.FactionID.empty() );
      REQUIRE( record.CellID.empty() );
      REQUIRE( record.PCFactionID.empty() );
      REQUIRE( record.SoundFile.empty() );
      REQUIRE_FALSE( record.isQuestName );
      REQUIRE( record.isQuestFinished );
      REQUIRE_FALSE( record.isQuestRestart );
      REQUIRE( record.Response == "Meine Festung ist fertig." );
      REQUIRE( record.Functions.empty() );
      REQUIRE( record.ResultString.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load info with quest restart")
    {
      const auto data = "INFO\xB5\0\0\0\0\0\0\0\0\0\0\0INAM\x15\0\0\0\x32\x39\x33\x33\x34\x32\x39\x32\x35\x33\x31\x33\x34\x34\x34\x32\x36\x34\x34\x37\0PNAM\x12\0\0\0\x32\x36\x37\x33\x32\x37\x35\x37\x31\x35\x34\x37\x35\x34\x31\x31\x38\0NNAM\x13\0\0\0\x34\x37\x36\x30\x32\x32\x30\x39\x32\x32\x31\x38\x32\x33\x36\x38\x30\x32\0DATA\x0C\0\0\0\x04\xF8\x12\x01p\0\0\0\xFF\xFF\xFF\0NAME>\0\0\0Ich habe mich bereit erkl\xE4rt, Rabinna zu Im-Kilaya zu bringen.QSTR\x01\0\0\0\x01"sv;
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
      REQUIRE( record.recordID == "29334292531344426447" );
      REQUIRE( record.PreviousInfoID == "26732757154754118" );
      REQUIRE( record.NextInfoID == "476022092218236802" );
      REQUIRE( record.UnknownLong == 0x0112F804 );
      REQUIRE( record.Disposition == 112 );
      REQUIRE( record.Rank == 0xFF );
      REQUIRE( record.Gender == 0xFF );
      REQUIRE( record.PCRank == 0xFF );
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
      REQUIRE( record.isQuestRestart );
      REQUIRE( record.Response == "Ich habe mich bereit erkl\xE4rt, Rabinna zu Im-Kilaya zu bringen." );
      REQUIRE( record.Functions.empty() );
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

    SECTION("corrupt data: multiple ONAMs")
    {
      const auto data = "INFO\x7F\0\0\0\0\0\0\0\0\0\0\0INAM\x14\0\0\0\x37\x35\x32\x39\x31\x39\x39\x31\x35\x31\x30\x31\x39\x39\x32\x32\x36\x39\x30\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0984727539316418937\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ONAM\x08\0\0\0Rabinna\0ONAM\x08\0\0\0Rabinna\0NAME\x03\0\0\0Ja?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ONAM > 256")
    {
      const auto data = "INFO\x6F\0\0\0\0\0\0\0\0\0\0\0INAM\x14\0\0\0\x37\x35\x32\x39\x31\x39\x39\x31\x35\x31\x30\x31\x39\x39\x32\x32\x36\x39\x30\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0984727539316418937\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ONAM\x08\x01\0\0Rabinna\0NAME\x03\0\0\0Ja?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ONAM is beyond stream")
    {
      const auto data = "INFO\x6F\0\0\0\0\0\0\0\0\0\0\0INAM\x14\0\0\0\x37\x35\x32\x39\x31\x39\x39\x31\x35\x31\x30\x31\x39\x39\x32\x32\x36\x39\x30\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0984727539316418937\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ONAM\x58\0\0\0Rabinna\0NAME\x03\0\0\0Ja?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple RNAMs")
    {
      const auto data = "INFO\xA4\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x32\x31\x35\x32\x36\x39\x31\x36\x39\x38\x37\x34\x31\x33\x32\x33\x33\0PNAM\x12\0\0\0\x33\x32\x33\x38\x32\x38\x33\x36\x31\x36\x34\x36\x36\x33\x36\x35\x38\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x01\0\0\0\0\0\0\0\xFF\0\xFF\0RNAM\x09\0\0\0Imperial\0RNAM\x09\0\0\0Imperial\0SNAM\x15\0\0\0vo\\i\\m\\Idl_IM001.mp3\0NAME\x0C\0\0\0[Schn\xFC\x66\x66\x65ln]"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RNAM > 256")
    {
      const auto data = "INFO\x93\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x32\x31\x35\x32\x36\x39\x31\x36\x39\x38\x37\x34\x31\x33\x32\x33\x33\0PNAM\x12\0\0\0\x33\x32\x33\x38\x32\x38\x33\x36\x31\x36\x34\x36\x36\x33\x36\x35\x38\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x01\0\0\0\0\0\0\0\xFF\0\xFF\0RNAM\x09\x01\0\0Imperial\0SNAM\x15\0\0\0vo\\i\\m\\Idl_IM001.mp3\0NAME\x0C\0\0\0[Schn\xFC\x66\x66\x65ln]"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of RNAM is beyond stream")
    {
      const auto data = "INFO\x93\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x32\x31\x35\x32\x36\x39\x31\x36\x39\x38\x37\x34\x31\x33\x32\x33\x33\0PNAM\x12\0\0\0\x33\x32\x33\x38\x32\x38\x33\x36\x31\x36\x34\x36\x36\x33\x36\x35\x38\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x01\0\0\0\0\0\0\0\xFF\0\xFF\0RNAM\x99\0\0\0Imperial\0SNAM\x15\0\0\0vo\\i\\m\\Idl_IM001.mp3\0NAME\x0C\0\0\0[Schn\xFC\x66\x66\x65ln]"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple CNAMs")
    {
      const auto data = "INFO\xB8\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x31\x34\x36\x38\x32\x34\x38\x39\x34\x34\x30\x39\x32\x34\x39\x37\x36\0PNAM\x14\0\0\0\x32\x31\x38\x34\x31\x32\x39\x32\x38\x30\x32\x31\x36\x35\x34\x31\x35\x31\x35\0NNAM\x15\0\0\0\x31\x34\x33\x37\x38\x33\x31\x36\x35\x31\x32\x30\x30\x33\x35\x31\x35\x39\x37\x30\0DATA\x0C\0\0\0\x02\0\0\0\0\0\0\0\xFF\xFF\xFF\0CNAM\x09\0\0\0Publican\0CNAM\x09\0\0\0Publican\0NAME\x0E\0\0\0Was wollt Ihr?BNAM\x11\0\0\0;publican default"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNAM > 256")
    {
      const auto data = "INFO\xA7\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x31\x34\x36\x38\x32\x34\x38\x39\x34\x34\x30\x39\x32\x34\x39\x37\x36\0PNAM\x14\0\0\0\x32\x31\x38\x34\x31\x32\x39\x32\x38\x30\x32\x31\x36\x35\x34\x31\x35\x31\x35\0NNAM\x15\0\0\0\x31\x34\x33\x37\x38\x33\x31\x36\x35\x31\x32\x30\x30\x33\x35\x31\x35\x39\x37\x30\0DATA\x0C\0\0\0\x02\0\0\0\0\0\0\0\xFF\xFF\xFF\0CNAM\x09\x01\0\0Publican\0NAME\x0E\0\0\0Was wollt Ihr?BNAM\x11\0\0\0;publican default"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of CNAM is beyond stream")
    {
      const auto data = "INFO\xA7\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x31\x34\x36\x38\x32\x34\x38\x39\x34\x34\x30\x39\x32\x34\x39\x37\x36\0PNAM\x14\0\0\0\x32\x31\x38\x34\x31\x32\x39\x32\x38\x30\x32\x31\x36\x35\x34\x31\x35\x31\x35\0NNAM\x15\0\0\0\x31\x34\x33\x37\x38\x33\x31\x36\x35\x31\x32\x30\x30\x33\x35\x31\x35\x39\x37\x30\0DATA\x0C\0\0\0\x02\0\0\0\0\0\0\0\xFF\xFF\xFF\0CNAM\x59\0\0\0Publican\0NAME\x0E\0\0\0Was wollt Ihr?BNAM\x11\0\0\0;publican default"sv;
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

    SECTION("corrupt data: multiple ANAMs")
    {
      const auto data = "INFO\xCB\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x34\x39\x32\x34\x31\x32\x39\x34\x31\x33\x35\x34\x31\x35\x36\x37\x36\0PNAM\x12\0\0\099367325152192966\0NNAM\x13\0\0\0971627586828521359\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ANAM#\0\0\0Sadrith Mora, Zur Dreckigen Muriel\0ANAM\x04\0\0\0foo\0DNAM\x0E\0\0\0Thieves Guild\0NAME\x13\0\0\0Sie ist hier. Oben."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ANAM > 256")
    {
      const auto data = "INFO\xBF\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x34\x39\x32\x34\x31\x32\x39\x34\x31\x33\x35\x34\x31\x35\x36\x37\x36\0PNAM\x12\0\0\099367325152192966\0NNAM\x13\0\0\0971627586828521359\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ANAM#\x01\0\0Sadrith Mora, Zur Dreckigen Muriel\0DNAM\x0E\0\0\0Thieves Guild\0NAME\x13\0\0\0Sie ist hier. Oben."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ANAM is beyond stream")
    {
      const auto data = "INFO\xBF\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x34\x39\x32\x34\x31\x32\x39\x34\x31\x33\x35\x34\x31\x35\x36\x37\x36\0PNAM\x12\0\0\099367325152192966\0NNAM\x13\0\0\0971627586828521359\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ANAM\x63\0\0\0Sadrith Mora, Zur Dreckigen Muriel\0DNAM\x0E\0\0\0Thieves Guild\0NAME\x13\0\0\0Sie ist hier. Oben."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple DNAMs")
    {
      const auto data = "INFO\xD5\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x34\x39\x32\x34\x31\x32\x39\x34\x31\x33\x35\x34\x31\x35\x36\x37\x36\0PNAM\x12\0\0\099367325152192966\0NNAM\x13\0\0\0971627586828521359\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ANAM#\0\0\0Sadrith Mora, Zur Dreckigen Muriel\0DNAM\x0E\0\0\0Thieves Guild\0DNAM\x0E\0\0\0Thieves Guild\0NAME\x13\0\0\0Sie ist hier. Oben."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DNAM > 256")
    {
      const auto data = "INFO\xBF\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x34\x39\x32\x34\x31\x32\x39\x34\x31\x33\x35\x34\x31\x35\x36\x37\x36\0PNAM\x12\0\0\099367325152192966\0NNAM\x13\0\0\0971627586828521359\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ANAM#\0\0\0Sadrith Mora, Zur Dreckigen Muriel\0DNAM\x0E\x01\0\0Thieves Guild\0NAME\x13\0\0\0Sie ist hier. Oben."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of DNAM is beyond stream")
    {
      const auto data = "INFO\xBF\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x34\x39\x32\x34\x31\x32\x39\x34\x31\x33\x35\x34\x31\x35\x36\x37\x36\0PNAM\x12\0\0\099367325152192966\0NNAM\x13\0\0\0971627586828521359\0DATA\x0C\0\0\0\0\0\0\0\0\0\0\0\xFF\xFF\xFF\0ANAM#\0\0\0Sadrith Mora, Zur Dreckigen Muriel\0DNAM\x3E\0\0\0Thieves Guild\0NAME\x13\0\0\0Sie ist hier. Oben."sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SNAMs")
    {
      const auto data = "INFO\xB0\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x32\x31\x35\x32\x36\x39\x31\x36\x39\x38\x37\x34\x31\x33\x32\x33\x33\0PNAM\x12\0\0\0\x33\x32\x33\x38\x32\x38\x33\x36\x31\x36\x34\x36\x36\x33\x36\x35\x38\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x01\0\0\0\0\0\0\0\xFF\0\xFF\0RNAM\x09\0\0\0Imperial\0SNAM\x15\0\0\0vo\\i\\m\\Idl_IM001.mp3\0SNAM\x15\0\0\0vo\\i\\m\\Idl_IM001.mp3\0NAME\x0C\0\0\0[Schn\xFC\x66\x66\x65ln]"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SNAM > 256")
    {
      const auto data = "INFO\x93\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x32\x31\x35\x32\x36\x39\x31\x36\x39\x38\x37\x34\x31\x33\x32\x33\x33\0PNAM\x12\0\0\0\x33\x32\x33\x38\x32\x38\x33\x36\x31\x36\x34\x36\x36\x33\x36\x35\x38\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x01\0\0\0\0\0\0\0\xFF\0\xFF\0RNAM\x09\0\0\0Imperial\0SNAM\x15\x01\0\0vo\\i\\m\\Idl_IM001.mp3\0NAME\x0C\0\0\0[Schn\xFC\x66\x66\x65ln]"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SNAM is beyond stream")
    {
      const auto data = "INFO\x93\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x32\x31\x35\x32\x36\x39\x31\x36\x39\x38\x37\x34\x31\x33\x32\x33\x33\0PNAM\x12\0\0\0\x33\x32\x33\x38\x32\x38\x33\x36\x31\x36\x34\x36\x36\x33\x36\x35\x38\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x01\0\0\0\0\0\0\0\xFF\0\xFF\0RNAM\x09\0\0\0Imperial\0SNAM\x35\0\0\0vo\\i\\m\\Idl_IM001.mp3\0NAME\x0C\0\0\0[Schn\xFC\x66\x66\x65ln]"sv;
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

    SECTION("corrupt data: multiple BNAMs")
    {
      const auto data = "INFO\xC0\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x31\x34\x36\x38\x32\x34\x38\x39\x34\x34\x30\x39\x32\x34\x39\x37\x36\0PNAM\x14\0\0\0\x32\x31\x38\x34\x31\x32\x39\x32\x38\x30\x32\x31\x36\x35\x34\x31\x35\x31\x35\0NNAM\x15\0\0\0\x31\x34\x33\x37\x38\x33\x31\x36\x35\x31\x32\x30\x30\x33\x35\x31\x35\x39\x37\x30\0DATA\x0C\0\0\0\x02\0\0\0\0\0\0\0\xFF\xFF\xFF\0CNAM\x09\0\0\0Publican\0NAME\x0E\0\0\0Was wollt Ihr?BNAM\x11\0\0\0;publican defaultBNAM\x11\0\0\0;publican default"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM > 768")
    {
      const auto data = "INFO\xA7\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x31\x34\x36\x38\x32\x34\x38\x39\x34\x34\x30\x39\x32\x34\x39\x37\x36\0PNAM\x14\0\0\0\x32\x31\x38\x34\x31\x32\x39\x32\x38\x30\x32\x31\x36\x35\x34\x31\x35\x31\x35\0NNAM\x15\0\0\0\x31\x34\x33\x37\x38\x33\x31\x36\x35\x31\x32\x30\x30\x33\x35\x31\x35\x39\x37\x30\0DATA\x0C\0\0\0\x02\0\0\0\0\0\0\0\xFF\xFF\xFF\0CNAM\x09\0\0\0Publican\0NAME\x0E\0\0\0Was wollt Ihr?BNAM\x11\x03\0\0;publican default"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BNAM is beyond stream")
    {
      const auto data = "INFO\xA7\0\0\0\0\0\0\0\0\0\0\0INAM\x12\0\0\0\x31\x34\x36\x38\x32\x34\x38\x39\x34\x34\x30\x39\x32\x34\x39\x37\x36\0PNAM\x14\0\0\0\x32\x31\x38\x34\x31\x32\x39\x32\x38\x30\x32\x31\x36\x35\x34\x31\x35\x31\x35\0NNAM\x15\0\0\0\x31\x34\x33\x37\x38\x33\x31\x36\x35\x31\x32\x30\x30\x33\x35\x31\x35\x39\x37\x30\0DATA\x0C\0\0\0\x02\0\0\0\0\0\0\0\xFF\xFF\xFF\0CNAM\x09\0\0\0Publican\0NAME\x0E\0\0\0Was wollt Ihr?BNAM\x15\0\0\0;publican default"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple QSTNs")
    {
      const auto data = "INFO\x81\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x30\x33\x30\x38\x31\x35\x33\x31\x37\x32\x32\x38\x32\x35\x31\x37\x39\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0\x33\x32\x31\x39\x33\x36\x30\x32\x32\x32\x36\x38\x35\x35\x33\x30\x37\x36\0DATA\x0C\0\0\0\x04\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\x06NAME\x14\0\0\0Eine Insel im NordenQSTN\x01\0\0\0\x01QSTN\x01\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of QSTN is not one")
    {
      {
        const auto data = "INFO\x77\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x30\x33\x30\x38\x31\x35\x33\x31\x37\x32\x32\x38\x32\x35\x31\x37\x39\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0\x33\x32\x31\x39\x33\x36\x30\x32\x32\x32\x36\x38\x35\x35\x33\x30\x37\x36\0DATA\x0C\0\0\0\x04\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\x06NAME\x14\0\0\0Eine Insel im NordenQSTN\0\0\0\0"sv;
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
        const auto data = "INFO\x79\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x30\x33\x30\x38\x31\x35\x33\x31\x37\x32\x32\x38\x32\x35\x31\x37\x39\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0\x33\x32\x31\x39\x33\x36\x30\x32\x32\x32\x36\x38\x35\x35\x33\x30\x37\x36\0DATA\x0C\0\0\0\x04\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\x06NAME\x14\0\0\0Eine Insel im NordenQSTN\x02\0\0\0\x01\0"sv;
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

    SECTION("corrupt data: stream ends before QSTN can be read")
    {
      const auto data = "INFO\x78\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x30\x33\x30\x38\x31\x35\x33\x31\x37\x32\x32\x38\x32\x35\x31\x37\x39\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0\x33\x32\x31\x39\x33\x36\x30\x32\x32\x32\x36\x38\x35\x35\x33\x30\x37\x36\0DATA\x0C\0\0\0\x04\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\x06NAME\x14\0\0\0Eine Insel im NordenQSTN\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: value of QSTN is not one")
    {
      const auto data = "INFO\x78\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x30\x33\x30\x38\x31\x35\x33\x31\x37\x32\x32\x38\x32\x35\x31\x37\x39\0PNAM\x01\0\0\0\0NNAM\x13\0\0\0\x33\x32\x31\x39\x33\x36\x30\x32\x32\x32\x36\x38\x35\x35\x33\x30\x37\x36\0DATA\x0C\0\0\0\x04\xFF\xFF\xFF\0\0\0\0\xFF\xFF\xFF\x06NAME\x14\0\0\0Eine Insel im NordenQSTN\x01\0\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple QSTFs")
    {
      const auto data = "INFO\x86\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x36\x34\x32\x31\x34\x31\x38\x38\x33\x30\x35\x39\x35\x36\x36\x30\x30\0PNAM\x14\0\0\09503227231245829738\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x04\xF8\x12\x01,\x01\0\0\xFF\xFF\xFF\0NAME\x19\0\0\0Meine Festung ist fertig.QSTF\x01\0\0\0\x01QSTF\x01\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of QSTF is not one")
    {
      {
        const auto data = "INFO\x7D\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x36\x34\x32\x31\x34\x31\x38\x38\x33\x30\x35\x39\x35\x36\x36\x30\x30\0PNAM\x14\0\0\09503227231245829738\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x04\xF8\x12\x01,\x01\0\0\xFF\xFF\xFF\0NAME\x19\0\0\0Meine Festung ist fertig.QSTF\0\0\0\0"sv;
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
        const auto data = "INFO\x7F\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x36\x34\x32\x31\x34\x31\x38\x38\x33\x30\x35\x39\x35\x36\x36\x30\x30\0PNAM\x14\0\0\09503227231245829738\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x04\xF8\x12\x01,\x01\0\0\xFF\xFF\xFF\0NAME\x19\0\0\0Meine Festung ist fertig.QSTF\x02\0\0\0\x01\0"sv;
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

    SECTION("corrupt data: stream ends before QSTF can be read")
    {
      const auto data = "INFO\x7E\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x36\x34\x32\x31\x34\x31\x38\x38\x33\x30\x35\x39\x35\x36\x36\x30\x30\0PNAM\x14\0\0\09503227231245829738\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x04\xF8\x12\x01,\x01\0\0\xFF\xFF\xFF\0NAME\x19\0\0\0Meine Festung ist fertig.QSTF\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: value of QSTF is not one")
    {
      const auto data = "INFO\x7E\0\0\0\0\0\0\0\0\0\0\0INAM\x13\0\0\0\x31\x36\x34\x32\x31\x34\x31\x38\x38\x33\x30\x35\x39\x35\x36\x36\x30\x30\0PNAM\x14\0\0\09503227231245829738\0NNAM\x01\0\0\0\0DATA\x0C\0\0\0\x04\xF8\x12\x01,\x01\0\0\xFF\xFF\xFF\0NAME\x19\0\0\0Meine Festung ist fertig.QSTF\x01\0\0\0\x02"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple QSTRs")
    {
      const auto data = "INFO\xBE\0\0\0\0\0\0\0\0\0\0\0INAM\x15\0\0\0\x32\x39\x33\x33\x34\x32\x39\x32\x35\x33\x31\x33\x34\x34\x34\x32\x36\x34\x34\x37\0PNAM\x12\0\0\0\x32\x36\x37\x33\x32\x37\x35\x37\x31\x35\x34\x37\x35\x34\x31\x31\x38\0NNAM\x13\0\0\0\x34\x37\x36\x30\x32\x32\x30\x39\x32\x32\x31\x38\x32\x33\x36\x38\x30\x32\0DATA\x0C\0\0\0\x04\xF8\x12\x01p\0\0\0\xFF\xFF\xFF\0NAME>\0\0\0Ich habe mich bereit erkl\xE4rt, Rabinna zu Im-Kilaya zu bringen.QSTR\x01\0\0\0\x01QSTR\x01\0\0\0\x01"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of QSTR is not one")
    {
      {
        const auto data = "INFO\xB4\0\0\0\0\0\0\0\0\0\0\0INAM\x15\0\0\0\x32\x39\x33\x33\x34\x32\x39\x32\x35\x33\x31\x33\x34\x34\x34\x32\x36\x34\x34\x37\0PNAM\x12\0\0\0\x32\x36\x37\x33\x32\x37\x35\x37\x31\x35\x34\x37\x35\x34\x31\x31\x38\0NNAM\x13\0\0\0\x34\x37\x36\x30\x32\x32\x30\x39\x32\x32\x31\x38\x32\x33\x36\x38\x30\x32\0DATA\x0C\0\0\0\x04\xF8\x12\x01p\0\0\0\xFF\xFF\xFF\0NAME>\0\0\0Ich habe mich bereit erkl\xE4rt, Rabinna zu Im-Kilaya zu bringen.QSTR\0\0\0\0"sv;
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
        const auto data = "INFO\xB6\0\0\0\0\0\0\0\0\0\0\0INAM\x15\0\0\0\x32\x39\x33\x33\x34\x32\x39\x32\x35\x33\x31\x33\x34\x34\x34\x32\x36\x34\x34\x37\0PNAM\x12\0\0\0\x32\x36\x37\x33\x32\x37\x35\x37\x31\x35\x34\x37\x35\x34\x31\x31\x38\0NNAM\x13\0\0\0\x34\x37\x36\x30\x32\x32\x30\x39\x32\x32\x31\x38\x32\x33\x36\x38\x30\x32\0DATA\x0C\0\0\0\x04\xF8\x12\x01p\0\0\0\xFF\xFF\xFF\0NAME>\0\0\0Ich habe mich bereit erkl\xE4rt, Rabinna zu Im-Kilaya zu bringen.QSTR\x02\0\0\0\x01\0"sv;
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

    SECTION("corrupt data: stream ends before QSTR can be read")
    {
      const auto data = "INFO\xB5\0\0\0\0\0\0\0\0\0\0\0INAM\x15\0\0\0\x32\x39\x33\x33\x34\x32\x39\x32\x35\x33\x31\x33\x34\x34\x34\x32\x36\x34\x34\x37\0PNAM\x12\0\0\0\x32\x36\x37\x33\x32\x37\x35\x37\x31\x35\x34\x37\x35\x34\x31\x31\x38\0NNAM\x13\0\0\0\x34\x37\x36\x30\x32\x32\x30\x39\x32\x32\x31\x38\x32\x33\x36\x38\x30\x32\0DATA\x0C\0\0\0\x04\xF8\x12\x01p\0\0\0\xFF\xFF\xFF\0NAME>\0\0\0Ich habe mich bereit erkl\xE4rt, Rabinna zu Im-Kilaya zu bringen.QSTR\x01\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // Skip INFO, because header is handled before loadFromStream.
      stream.seekg(4);
      REQUIRE( stream.good() );

      // Reading should fail.
      DialogueInfoRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: value of QSTR is not one")
    {
      const auto data = "INFO\xB5\0\0\0\0\0\0\0\0\0\0\0INAM\x15\0\0\0\x32\x39\x33\x33\x34\x32\x39\x32\x35\x33\x31\x33\x34\x34\x34\x32\x36\x34\x34\x37\0PNAM\x12\0\0\0\x32\x36\x37\x33\x32\x37\x35\x37\x31\x35\x34\x37\x35\x34\x31\x31\x38\0NNAM\x13\0\0\0\x34\x37\x36\x30\x32\x32\x30\x39\x32\x32\x31\x38\x32\x33\x36\x38\x30\x32\0DATA\x0C\0\0\0\x04\xF8\x12\x01p\0\0\0\xFF\xFF\xFF\0NAME>\0\0\0Ich habe mich bereit erkl\xE4rt, Rabinna zu Im-Kilaya zu bringen.QSTR\x01\0\0\0\x02"sv;
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
