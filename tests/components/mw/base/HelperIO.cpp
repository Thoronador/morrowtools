/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2021  Thoronador

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
#include "../../../../mw/base/HelperIO.hpp"
#include "../../../../mw/base/MW_Constants.hpp"

TEST_CASE("HelperIO")
{
  SECTION("IntTo4Char")
  {
    REQUIRE( IntTo4Char(MWTP::cALCH) == "ALCH" );
    REQUIRE( IntTo4Char(MWTP::cBOOK) == "BOOK" );
    REQUIRE( IntTo4Char(MWTP::cDIAL) == "DIAL" );
    REQUIRE( IntTo4Char(MWTP::cFLAG) == "FLAG" );
    REQUIRE( IntTo4Char(MWTP::cINGR) == "INGR" );
    REQUIRE( IntTo4Char(MWTP::cMISC) == "MISC" );
    REQUIRE( IntTo4Char(MWTP::cNPC_) == "NPC_" );
    REQUIRE( IntTo4Char(MWTP::cONAM) == "ONAM" );
    REQUIRE( IntTo4Char(MWTP::cPGRD) == "PGRD" );
    REQUIRE( IntTo4Char(MWTP::cQSTR) == "QSTR" );
    REQUIRE( IntTo4Char(MWTP::cSCTX) == "SCTX" );
    REQUIRE( IntTo4Char(MWTP::cSCVR) == "SCVR" );
    REQUIRE( IntTo4Char(MWTP::cTES3) == "TES3" );
    REQUIRE( IntTo4Char(MWTP::cWEAP) == "WEAP" );
    REQUIRE( IntTo4Char(MWTP::cXCHG) == "XCHG" );
  }

  SECTION("UnexpectedRecord")
  {
    // Cannot be tested, since it prints to standard error output only.
  }
}
