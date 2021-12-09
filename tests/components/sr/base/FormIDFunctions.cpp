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
#include "../../../../sr/base/FormIDFunctions.hpp"

TEST_CASE("FormIDFunctions")
{
  using namespace SRTP;

  SECTION("changeModIndexInSitu")
  {
    uint32_t formId = 0x01ABCDEF;

    changeModIndexInSitu(formId, 1);
    REQUIRE( formId == 0x01ABCDEF );

    changeModIndexInSitu(formId, 2);
    REQUIRE( formId == 0x02ABCDEF );

    changeModIndexInSitu(formId, 128);
    REQUIRE( formId == 0x80ABCDEF );

    changeModIndexInSitu(formId, 255);
    REQUIRE( formId == 0xFFABCDEF );

    changeModIndexInSitu(formId, 160);
    REQUIRE( formId == 0xA0ABCDEF );
  }

  SECTION("extractModIndex")
  {
    REQUIRE( extractModIndex(0) == 0 );
    REQUIRE( extractModIndex(0x01ABCDEF) == 1 );
    REQUIRE( extractModIndex(0x0ABBBBBB) == 10 );
    REQUIRE( extractModIndex(0x0FABCDEF) == 15 );
    REQUIRE( extractModIndex(0x34ABCDEF) == 52 );
    REQUIRE( extractModIndex(0x80FFEEDD) == 128 );
    REQUIRE( extractModIndex(0xFFABCDEF) == 255 );
  }

  SECTION("extractObjectIndex")
  {
    REQUIRE( extractObjectIndex(0) == 0 );
    REQUIRE( extractObjectIndex(0x01ABCDEF) == 0xABCDEF );
    REQUIRE( extractObjectIndex(0x0ABBBBBB) == 0xBBBBBB );
    REQUIRE( extractObjectIndex(0xCCFFFFFF) == 0xFFFFFF );
    REQUIRE( extractObjectIndex(0x34FEDCBA) == 0xFEDCBA );
    REQUIRE( extractObjectIndex(0x80FFEEDD) == 0xFFEEDD );
    REQUIRE( extractObjectIndex(0xFF654321) == 0x654321 );
  }

  SECTION("getFormIDAsString")
  {
    REQUIRE( getFormIDAsString(0) == "00000000" );
    REQUIRE( getFormIDAsString(0x12ABCDEF) == "12ABCDEF" );

    REQUIRE( getFormIDAsString(0x00000012) == "00000012" );
    REQUIRE( getFormIDAsString(0x00001234) == "00001234" );
    REQUIRE( getFormIDAsString(0x00123456) == "00123456" );
    REQUIRE( getFormIDAsString(0x12345678) == "12345678" );
    REQUIRE( getFormIDAsString(0x34567890) == "34567890" );
    REQUIRE( getFormIDAsString(0x567890AB) == "567890AB" );
    REQUIRE( getFormIDAsString(0x7890ABCD) == "7890ABCD" );
    REQUIRE( getFormIDAsString(0x90ABCDEF) == "90ABCDEF" );
    REQUIRE( getFormIDAsString(0xABCDEF01) == "ABCDEF01" );
    REQUIRE( getFormIDAsString(0xCDEF0123) == "CDEF0123" );
    REQUIRE( getFormIDAsString(0xEF012345) == "EF012345" );

    REQUIRE( getFormIDAsString(0xFFFFFFFF) == "FFFFFFFF" );
  }

  SECTION("getFormIDAsStringWithFile")
  {
    SECTION("load order including addons")
    {
      const std::vector<std::string> loadOrder = { "Skyrim.esm", "Update.esm", "Dawnguard.esm", "HearthFires.esm", "Dragonborn.esm" };

      REQUIRE( getFormIDAsStringWithFile(0x00ABCDEF, loadOrder, true) == "00ABCDEF (file: Skyrim.esm)" );
      REQUIRE( getFormIDAsStringWithFile(0x00ABCDEF, loadOrder, false) == "00ABCDEF" );
      REQUIRE( getFormIDAsStringWithFile(0x01654321, loadOrder, true) == "xx654321 (file: Update.esm)" );
      REQUIRE( getFormIDAsStringWithFile(0x01654321, loadOrder, false) == "xx654321" );
      REQUIRE( getFormIDAsStringWithFile(0x02BADC0D, loadOrder, true) == "xxBADC0D (file: Dawnguard.esm)" );
      REQUIRE( getFormIDAsStringWithFile(0x02BADC0D, loadOrder, false) == "xxBADC0D" );
      REQUIRE( getFormIDAsStringWithFile(0x03123456, loadOrder, true) == "xx123456 (file: HearthFires.esm)" );
      REQUIRE( getFormIDAsStringWithFile(0x03123456, loadOrder, false) == "xx123456" );
      REQUIRE( getFormIDAsStringWithFile(0x04FEDCBA, loadOrder, true) == "xxFEDCBA (file: Dragonborn.esm)" );
      REQUIRE( getFormIDAsStringWithFile(0x04FEDCBA, loadOrder, false) == "xxFEDCBA" );
      REQUIRE( getFormIDAsStringWithFile(0x05FEDCBA, loadOrder, true) == "xxFEDCBA (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0x05FEDCBA, loadOrder, false) == "xxFEDCBA" );
    }

    SECTION("load order without addons")
    {
      const std::vector<std::string> loadOrder = { "Skyrim.esm", "Update.esm" };

      REQUIRE( getFormIDAsStringWithFile(0x00ABCDEF, loadOrder, true) == "00ABCDEF (file: Skyrim.esm)" );
      REQUIRE( getFormIDAsStringWithFile(0x00ABCDEF, loadOrder, false) == "00ABCDEF" );
      REQUIRE( getFormIDAsStringWithFile(0x01654321, loadOrder, true) == "xx654321 (file: Update.esm)" );
      REQUIRE( getFormIDAsStringWithFile(0x01654321, loadOrder, false) == "xx654321" );
      REQUIRE( getFormIDAsStringWithFile(0x02BADC0D, loadOrder, true) == "xxBADC0D (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0x02BADC0D, loadOrder, false) == "xxBADC0D" );
      REQUIRE( getFormIDAsStringWithFile(0x03123456, loadOrder, true) == "xx123456 (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0x03123456, loadOrder, false) == "xx123456" );
      REQUIRE( getFormIDAsStringWithFile(0x04FEDCBA, loadOrder, true) == "xxFEDCBA (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0x04FEDCBA, loadOrder, false) == "xxFEDCBA" );
      REQUIRE( getFormIDAsStringWithFile(0x05FEDCBA, loadOrder, true) == "xxFEDCBA (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0x05FEDCBA, loadOrder, false) == "xxFEDCBA" );
    }

    SECTION("empty load order")
    {
      const std::vector<std::string> loadOrder = { };

      REQUIRE( getFormIDAsStringWithFile(0x00ABCDEF, loadOrder, true) == "00ABCDEF (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0x00ABCDEF, loadOrder, false) == "00ABCDEF" );
      REQUIRE( getFormIDAsStringWithFile(0x01654321, loadOrder, true) == "xx654321 (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0x01654321, loadOrder, false) == "xx654321" );
      REQUIRE( getFormIDAsStringWithFile(0xFFFEDCBA, loadOrder, true) == "xxFEDCBA (file: unknown)" );
      REQUIRE( getFormIDAsStringWithFile(0xFFFEDCBA, loadOrder, false) == "xxFEDCBA" );
    }
  }

  SECTION("getFormIDAsStringXX")
  {
    REQUIRE( getFormIDAsStringXX(0) == "00000000" );
    REQUIRE( getFormIDAsStringXX(0x12ABCDEF) == "xxABCDEF" );

    REQUIRE( getFormIDAsStringXX(0x00000012) == "00000012" );
    REQUIRE( getFormIDAsStringXX(0x00001234) == "00001234" );
    REQUIRE( getFormIDAsStringXX(0x00123456) == "00123456" );
    REQUIRE( getFormIDAsStringXX(0x12345678) == "xx345678" );
    REQUIRE( getFormIDAsStringXX(0x34567890) == "xx567890" );
    REQUIRE( getFormIDAsStringXX(0x567890AB) == "xx7890AB" );
    REQUIRE( getFormIDAsStringXX(0x7890ABCD) == "xx90ABCD" );
    REQUIRE( getFormIDAsStringXX(0x90ABCDEF) == "xxABCDEF" );
    REQUIRE( getFormIDAsStringXX(0xABCDEF01) == "xxCDEF01" );
    REQUIRE( getFormIDAsStringXX(0xCDEF0123) == "xxEF0123" );
    REQUIRE( getFormIDAsStringXX(0xEF012345) == "xx012345" );

    REQUIRE( getFormIDAsStringXX(0xFFFFFFFF) == "xxFFFFFF" );
  }
}
