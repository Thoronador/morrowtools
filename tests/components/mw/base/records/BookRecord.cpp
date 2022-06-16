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

#include "../../../locate_catch.hpp"
#include <sstream>
#include <string_view>
#include "../../../../../mw/base/records/BookRecord.hpp"

TEST_CASE("MWTP::BookRecord")
{
  using namespace MWTP;
  using namespace std::string_view_literals;

  SECTION("constructor")
  {
    BookRecord record;

    REQUIRE( record.recordID.empty() );
    REQUIRE( record.ModelPath.empty() );
    REQUIRE( record.Name.empty() );
    REQUIRE( record.Weight == 0.0f );
    REQUIRE( record.Value == 0 );
    REQUIRE( record.Scroll == 0 );
    REQUIRE( record.SkillID == -1 );
    REQUIRE( record.EnchantPoints == 0 );
    REQUIRE( record.InventoryIcon.empty() );
    REQUIRE( record.ScriptID.empty() );
    REQUIRE( record.Text.empty() );
    REQUIRE( record.EnchantmentID.empty() );
  }

  SECTION("equals")
  {
    BookRecord a;
    BookRecord b;

    SECTION("equal")
    {
      a.recordID = "BookOne";
      a.ModelPath = "item01.nif";

      b.recordID = "BookOne";
      b.ModelPath = "item01.nif";

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

      SECTION("ModelPath mismatch")
      {
        a.ModelPath = "mesh01.nif";
        b.ModelPath = "mesh02.nif";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Name mismatch")
      {
        a.Name = "Alice's book";
        b.Name = "Bob's book";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Weight mismatch")
      {
        a.Weight = 1.5f;
        b.Weight = 2.5f;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Value mismatch")
      {
        a.Value = 15;
        b.Value = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Scroll mismatch")
      {
        a.Scroll = 0;
        b.Scroll = 1;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("SkillID mismatch")
      {
        a.SkillID = 5;
        b.SkillID = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EnchantPoints mismatch")
      {
        a.EnchantPoints = 5;
        b.EnchantPoints = 25;

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("InventoryIcon mismatch")
      {
        a.InventoryIcon = "icon01.dds";
        b.InventoryIcon = "other_icon.dds";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("ScriptID mismatch")
      {
        a.ScriptID = "ThisScript";
        b.ScriptID = "ThatScript";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("Text mismatch")
      {
        a.Text = "One upon a time in Vvardenfell ...";
        b.Text = "Ash storms are nasty.";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }

      SECTION("EnchantmentID mismatch")
      {
        a.EnchantmentID = "ThisID";
        b.EnchantmentID = "ThatID";

        REQUIRE_FALSE( a.equals(b) );
        REQUIRE_FALSE( b.equals(a) );
      }
    }
  }

  SECTION("loadFromStream")
  {
    uint32_t dummy = 0;

    SECTION("default: load record without script, text and enchantment ID")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BookRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "sc_paper plain" );
      REQUIRE( record.ModelPath == "m\\Misc_paper_plain_01.nif" );
      REQUIRE( record.Name == "Papier" );
      REQUIRE( record.Weight == 1.0f );
      REQUIRE( record.Value == 3 );
      REQUIRE( record.Scroll == 1 );
      REQUIRE( record.SkillID == 0xFFFFFFFF );
      REQUIRE( record.EnchantPoints == 50 );
      REQUIRE( record.InventoryIcon == "m\\Tx_paper_plain_01.tga" );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Text.empty() );
      REQUIRE( record.EnchantmentID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with text and enchantment")
    {
      const auto data = "BOOK\x1E\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x19\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BookRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "sc_ekashslocksplitter" );
      REQUIRE( record.ModelPath == "m\\text_scroll_01.nif" );
      REQUIRE( record.Name == "Rolle 'Ekashs Schlossteiler'" );
      REQUIRE( record.Weight == 0.2f );
      REQUIRE( record.Value == 343 );
      REQUIRE( record.Scroll == 1 );
      REQUIRE( record.SkillID == 0xFFFFFFFF );
      REQUIRE( record.EnchantPoints == 0 );
      REQUIRE( record.InventoryIcon == "m\\tx_scroll_open_01.tga" );
      REQUIRE( record.ScriptID.empty() );
      REQUIRE( record.Text == "<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"" );
      REQUIRE( record.EnchantmentID == "sc_ekashslocksplitter_en" );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("default: load record with text and script")
    {
      const auto data = "BOOK\xEB\x01\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0bk_EggOfTime\0MODL\x14\0\0\0m\\Text_Folio_03.NIF\0FNAM\x10\0\0\0Das Ei der Zeit\0BKDT\x14\0\0\0\0\0\x80@\xE8\x03\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0SCRI\x0A\0\0\0eotScript\0ITEX\x12\0\0\0m\\Tx_folio_03.tga\0TEXTR\x01\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"3\" FACE=\"Magic Cards\"><BR><IMG SRC=\"EggOfTime_Illust1.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust2.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust3.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust4.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR>\""sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should succeed.
      BookRecord record;
      REQUIRE( record.loadFromStream(stream) );
      // Check data.
      // -- header
      REQUIRE( record.getHeaderOne() == 0 );
      REQUIRE( record.getHeaderFlags() == 0 );
      // -- record data
      REQUIRE( record.recordID == "bk_EggOfTime" );
      REQUIRE( record.ModelPath == "m\\Text_Folio_03.NIF" );
      REQUIRE( record.Name == "Das Ei der Zeit" );
      REQUIRE( record.Weight == 4.0f );
      REQUIRE( record.Value == 1000 );
      REQUIRE( record.Scroll == 0 );
      REQUIRE( record.SkillID == 0xFFFFFFFF );
      REQUIRE( record.EnchantPoints == 0 );
      REQUIRE( record.InventoryIcon == "m\\Tx_folio_03.tga" );
      REQUIRE( record.ScriptID == "eotScript" );
      REQUIRE( record.Text == "<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"3\" FACE=\"Magic Cards\"><BR><IMG SRC=\"EggOfTime_Illust1.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust2.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust3.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust4.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR>\"" );
      REQUIRE( record.EnchantmentID.empty() );

      // Writing should succeed.
      std::ostringstream streamOut;
      REQUIRE( record.saveToStream(streamOut) );
      // Check written data.
      REQUIRE( streamOut.str() == data );
    }

    SECTION("corrupt data: stream ends before header can be read")
    {
      const auto data = "BOOK\x91\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no NAME")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0FAIL\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME > 256")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\x01\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of NAME is beyond stream")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x8F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no MODL")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0FAIL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL > 256")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\x01\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of MODL is beyond stream")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x8A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no FNAM")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FAIL\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM > 256")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\x01\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of FNAM is beyond stream")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x87\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no BKDT")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0FAIL\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of BKDT is not 20")
    {
      {
        const auto data = "BOOK\x83\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x13\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }

      {
        const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x15\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
        std::istringstream stream;
        stream.str(std::string(data));

        // read BOOK, because header is handled before loadFromStream.
        stream.read(reinterpret_cast<char*>(&dummy), 4);
        REQUIRE( stream.good() );

        // Reading should fail.
        BookRecord record;
        REQUIRE_FALSE( record.loadFromStream(stream) );
      }
    }

    SECTION("corrupt data: stream ends before BKDT can be read")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no SCRI")
    {
      const auto data = "BOOK\xEB\x01\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0bk_EggOfTime\0MODL\x14\0\0\0m\\Text_Folio_03.NIF\0FNAM\x10\0\0\0Das Ei der Zeit\0BKDT\x14\0\0\0\0\0\x80@\xE8\x03\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0FAIL\x0A\0\0\0eotScript\0ITEX\x12\0\0\0m\\Tx_folio_03.tga\0TEXTR\x01\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"3\" FACE=\"Magic Cards\"><BR><IMG SRC=\"EggOfTime_Illust1.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust2.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust3.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust4.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR>\""sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple SCRIs")
    {
      const auto data = "BOOK\xFD\x01\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0bk_EggOfTime\0MODL\x14\0\0\0m\\Text_Folio_03.NIF\0FNAM\x10\0\0\0Das Ei der Zeit\0BKDT\x14\0\0\0\0\0\x80@\xE8\x03\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0SCRI\x0A\0\0\0eotScript\0SCRI\x0A\0\0\0eotScript\0ITEX\x12\0\0\0m\\Tx_folio_03.tga\0TEXTR\x01\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"3\" FACE=\"Magic Cards\"><BR><IMG SRC=\"EggOfTime_Illust1.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust2.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust3.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust4.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR>\""sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI > 256")
    {
      const auto data = "BOOK\xEB\x01\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0bk_EggOfTime\0MODL\x14\0\0\0m\\Text_Folio_03.NIF\0FNAM\x10\0\0\0Das Ei der Zeit\0BKDT\x14\0\0\0\0\0\x80@\xE8\x03\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0SCRI\x0A\x01\0\0eotScript\0ITEX\x12\0\0\0m\\Tx_folio_03.tga\0TEXTR\x01\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"3\" FACE=\"Magic Cards\"><BR><IMG SRC=\"EggOfTime_Illust1.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust2.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust3.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust4.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR>\""sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of SCRI is beyond stream")
    {
      const auto data = "BOOK\xEB\x01\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0bk_EggOfTime\0MODL\x14\0\0\0m\\Text_Folio_03.NIF\0FNAM\x10\0\0\0Das Ei der Zeit\0BKDT\x14\0\0\0\0\0\x80@\xE8\x03\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0SCRI\xFA\0\0\0eotSc"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty SCRI")
    {
      const auto data = "BOOK\xE2\x01\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0bk_EggOfTime\0MODL\x14\0\0\0m\\Text_Folio_03.NIF\0FNAM\x10\0\0\0Das Ei der Zeit\0BKDT\x14\0\0\0\0\0\x80@\xE8\x03\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0SCRI\x01\0\0\0\0ITEX\x12\0\0\0m\\Tx_folio_03.tga\0TEXTR\x01\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"3\" FACE=\"Magic Cards\"><BR><IMG SRC=\"EggOfTime_Illust1.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust2.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust3.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR><IMG SRC=\"EggOfTime_Illust4.TGA\" WIDTH=\"240\" HEIGHT=\"240\"><BR><BR>\""sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ITEX")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0FAIL\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ITEXs")
    {
      const auto data = "BOOK\xA4\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0ITEX\x18\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX > 256")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x18\x01\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ITEX is beyond stream")
    {
      const auto data = "BOOK\x84\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x28\0\0\0m\\Tx_paper_plain_01.tga\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ITEX")
    {
      const auto data = "BOOK\x6D\0\0\0\0\0\0\0\0\0\0\0NAME\x0F\0\0\0sc_paper plain\0MODL\x1A\0\0\0m\\Misc_paper_plain_01.nif\0FNAM\x07\0\0\0Papier\0BKDT\x14\0\0\0\0\0\x80?\x03\0\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\x32\0\0\0ITEX\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no TEXT")
    {
      const auto data = "BOOK\x1E\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0FAILY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x19\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple TEXTs")
    {
      const auto data = "BOOK\x29\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"TEXT\x03\0\0\0fooENAM\x19\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of TEXT > 65535")
    {
      const auto data = "BOOK\x1E\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\x01\x01\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x19\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of TEXT is beyond stream")
    {
      const auto data = "BOOK\x1E\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\x01\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x19\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty TEXT")
    {
      const auto data = "BOOK\x9A\0\0\0\0\0\0\0\0\0\0\0NAME\x0D\0\0\0bk_EggOfTime\0MODL\x14\0\0\0m\\Text_Folio_03.NIF\0FNAM\x10\0\0\0Das Ei der Zeit\0BKDT\x14\0\0\0\0\0\x80@\xE8\x03\0\0\0\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0SCRI\x0A\0\0\0eotScript\0ITEX\x12\0\0\0m\\Tx_folio_03.tga\0TEXT\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: no ENAM")
    {
      const auto data = "BOOK\x1E\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"FAIL\x19\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: multiple ENAMs")
    {
      const auto data = "BOOK\x3F\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x19\0\0\0sc_ekashslocksplitter_en\0ENAM\x19\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM > 256")
    {
      const auto data = "BOOK\x1E\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x19\x01\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: length of ENAM is beyond stream")
    {
      const auto data = "BOOK\x1E\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x29\0\0\0sc_ekashslocksplitter_en\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }

    SECTION("corrupt data: empty ENAM")
    {
      const auto data = "BOOK\x06\x01\0\0\0\0\0\0\0\0\0\0NAME\x16\0\0\0sc_ekashslocksplitter\0MODL\x15\0\0\0m\\text_scroll_01.nif\0FNAM\x1D\0\0\0Rolle 'Ekashs Schlossteiler'\0BKDT\x14\0\0\0\xCD\xCCL>W\x01\0\0\x01\0\0\0\xFF\xFF\xFF\xFF\0\0\0\0ITEX\x18\0\0\0m\\tx_scroll_open_01.tga\0TEXTY\0\0\0<DIV ALIGN=\"CENTER\"><FONT COLOR=\"000000\" SIZE=\"2\" FACE=\"Daedric\">WOE UPON YOU</FONT><BR>\"ENAM\x01\0\0\0\0"sv;
      std::istringstream stream;
      stream.str(std::string(data));

      // read BOOK, because header is handled before loadFromStream.
      stream.read(reinterpret_cast<char*>(&dummy), 4);
      REQUIRE( stream.good() );

      // Reading should fail.
      BookRecord record;
      REQUIRE_FALSE( record.loadFromStream(stream) );
    }
  }
}
