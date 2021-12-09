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
#include "../../../base/UtilityFunctions.hpp"

TEST_CASE("UtilityFunctions")
{
  SECTION("floatToString")
  {
    REQUIRE( floatToString(0) == "0" );
    REQUIRE( floatToString(1) == "1" );
    REQUIRE( floatToString(1.2f) == "1.2" );
    REQUIRE( floatToString(1.25f) == "1.25" );
    REQUIRE( floatToString(12.34) == "12.34" );
    REQUIRE( floatToString(1.2345) == "1.2345" );
    REQUIRE( floatToString(1000.75) == "1000.75" );

    REQUIRE( floatToString(-1) == "-1" );
    REQUIRE( floatToString(-1.2f) == "-1.2" );
    REQUIRE( floatToString(-1.25f) == "-1.25" );
    REQUIRE( floatToString(-12.34) == "-12.34" );
    REQUIRE( floatToString(-1.2345) == "-1.2345" );
    REQUIRE( floatToString(-1000.75) == "-1000.75" );
  }

  SECTION("intToString")
  {
    REQUIRE( intToString(0) == "0" );
    REQUIRE( intToString(1) == "1" );
    REQUIRE( intToString(12) == "12" );
    REQUIRE( intToString(123) == "123" );
    REQUIRE( intToString(1234) == "1234" );
    REQUIRE( intToString(12345) == "12345" );
    REQUIRE( intToString(32767) == "32767" );

    REQUIRE( intToString(-1) == "-1" );
    REQUIRE( intToString(-12) == "-12" );
    REQUIRE( intToString(-123) == "-123" );
    REQUIRE( intToString(-1234) == "-1234" );
    REQUIRE( intToString(-12345) == "-12345" );
    REQUIRE( intToString(-32768) == "-32768" );
  }

  SECTION("lowerCase")
  {
    REQUIRE( lowerCase("") == "" );
    REQUIRE( lowerCase(" ") == " " );
    REQUIRE( lowerCase("abcdefghijklmnopqrstuvwxyz") == "abcdefghijklmnopqrstuvwxyz" );
    REQUIRE( lowerCase("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == "abcdefghijklmnopqrstuvwxyz" );
    REQUIRE( lowerCase("AB1234CD") == "ab1234cd" );
    REQUIRE( lowerCase("AbCdEfGhIjKlMnOpQrStUvWxYz") == "abcdefghijklmnopqrstuvwxyz" );
    REQUIRE( lowerCase("0123456789") == "0123456789" );
  }

  SECTION("lowerCaseCompare")
  {
    REQUIRE( lowerCaseCompare("", "") == 0 );

    REQUIRE( lowerCaseCompare("abc", "bcde") < 0 );
    REQUIRE( lowerCaseCompare("ABC", "bcdef") < 0 );
    REQUIRE( lowerCaseCompare("abc", "BCDf") < 0 );
    REQUIRE( lowerCaseCompare("aBcD", "bCdE") < 0 );
    REQUIRE( lowerCaseCompare("AbCd", "bCdE") < 0 );
    REQUIRE( lowerCaseCompare("AbCd", "BcDe") < 0 );
    REQUIRE( lowerCaseCompare("aBcD", "BcDe") < 0 );

    REQUIRE( lowerCaseCompare("abc", "abc") == 0 );
    REQUIRE( lowerCaseCompare("ABC", "abc") == 0 );
    REQUIRE( lowerCaseCompare("abc", "ABC") == 0 );
    REQUIRE( lowerCaseCompare("abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz") == 0 );
    REQUIRE( lowerCaseCompare("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz") == 0 );
    REQUIRE( lowerCaseCompare("AB1234CD", "ab1234cd") == 0 );
    REQUIRE( lowerCaseCompare("AbCdEfGhIjKlMnOpQrStUvWxYz", "abcdefghijklmnopqrstuvwxyz") == 0 );
    REQUIRE( lowerCaseCompare("0123456789", "0123456789") == 0 );

    REQUIRE( lowerCaseCompare("def", "abc") > 0 );
    REQUIRE( lowerCaseCompare("DEF", "abc") > 0 );
    REQUIRE( lowerCaseCompare("def", "ABC") > 0 );
  }

  SECTION("removeEnclosingBrackets")
  {
    std::string s;

    SECTION("empty string")
    {
      s = "";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == "" );
    }

    SECTION("string too short")
    {
      s = "(";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == "(" );

      s = ")";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == ")" );

      s = "a";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == "a" );
    }

    SECTION("simple brackets")
    {
      s = "( Abc )";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == " Abc " );

      s = "(Abc1)";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == "Abc1" );

      s = "(f(x))";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == "f(x)" );

      s = "( CellChanged == 0 )";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == " CellChanged == 0 " );
    }

    SECTION("multiple brackets")
    {
      // Only the outermost pair gets removed.
      s = "(( Abc ))";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == "( Abc )" );

      s = "((Abc1))";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == "(Abc1)" );

      s = "((((((((((((too much?))))))))))))";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == "(((((((((((too much?)))))))))))" );
    }

    SECTION("nested brackets")
    {
      // Only the outermost pair gets removed.
      s = "( Abc * ( item->foo + 1 ) )";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == " Abc * ( item->foo + 1 ) " );

      s = "( timer < (swingTime * 4) )";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == " timer < (swingTime * 4) " );

      s = "((Abc1))";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == "(Abc1)" );

      s = "((((((((((((too much?))))))))))))";
      REQUIRE( removeEnclosingBrackets(s) );
      REQUIRE( s == "(((((((((((too much?)))))))))))" );
    }

    SECTION("wrong nesting")
    {
      // String should keep its original value, if the nesting of the brackets
      // is incorrect.
      s = "( timer < ()swingTime * 4) )";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == "( timer < ()swingTime * 4) )" );

      s = "( timer < (swingTime * 4)";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == "( timer < (swingTime * 4)" );

      s = "( timer < (swingTime * 4) ))";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == "( timer < (swingTime * 4) ))" );

      s = "timer < (swingTime * 4) )";
      REQUIRE_FALSE( removeEnclosingBrackets(s) );
      REQUIRE( s == "timer < (swingTime * 4) )" );
    }
  }

  SECTION("stringToFloat")
  {
    float value;

    SECTION("empty string")
    {
      REQUIRE_FALSE( stringToFloat("", value) );
    }

    SECTION("normal input data")
    {
      REQUIRE( stringToFloat("0", value));
      REQUIRE( value == 0.0 );

      REQUIRE( stringToFloat("0.0", value));
      REQUIRE( value == 0.0 );

      REQUIRE( stringToFloat("1", value));
      REQUIRE( value == 1 );

      REQUIRE( stringToFloat("1.25", value));
      REQUIRE( value == 1.25 );

      REQUIRE( stringToFloat("123.45", value));
      REQUIRE( value == 123.45f );

      REQUIRE( stringToFloat("12345.75", value));
      REQUIRE( value == 12345.75f );

      REQUIRE( stringToFloat("-1", value));
      REQUIRE( value == -1 );

      REQUIRE( stringToFloat("-1.25", value));
      REQUIRE( value == -1.25 );

      REQUIRE( stringToFloat("-1.234", value));
      REQUIRE( value == -1.234f );

      REQUIRE( stringToFloat("-123.45", value));
      REQUIRE( value == -123.45f );
    }

    SECTION("invalid input data")
    {
      REQUIRE_FALSE( stringToFloat("-", value));
      REQUIRE_FALSE( stringToFloat("1.2a", value));
      REQUIRE_FALSE( stringToFloat("  1.2", value));
      REQUIRE_FALSE( stringToFloat("1abcdef", value));
      REQUIRE_FALSE( stringToFloat("Hello", value));
      REQUIRE_FALSE( stringToFloat("Hello12.3", value));
    }

    SECTION("overflow")
    {
      REQUIRE_FALSE( stringToFloat("350000000000000000000000000000000000000000000000", value));
      REQUIRE_FALSE( stringToFloat("-350000000000000000000000000000000000000000000000", value));
    }
  }

  SECTION("stringToLong")
  {
    int32_t value;

    SECTION("empty string")
    {
      REQUIRE_FALSE( stringToLong("", value) );
    }

    SECTION("normal input data")
    {
      REQUIRE( stringToLong("0", value));
      REQUIRE( value == 0 );

      REQUIRE( stringToLong("1", value));
      REQUIRE( value == 1 );

      REQUIRE( stringToLong("1234", value));
      REQUIRE( value == 1234 );

      REQUIRE( stringToLong("12345", value));
      REQUIRE( value == 12345 );

      REQUIRE( stringToLong("2147483647", value));
      REQUIRE( value == 2147483647 );

      REQUIRE( stringToLong("-1", value));
      REQUIRE( value == -1 );

      REQUIRE( stringToLong("-12", value));
      REQUIRE( value == -12 );

      REQUIRE( stringToLong("-1234", value));
      REQUIRE( value == -1234 );

      REQUIRE( stringToLong("-12345", value));
      REQUIRE( value == -12345 );

      REQUIRE( stringToLong("-2147483647", value));
      REQUIRE( value == -2147483647 );
    }

    SECTION("invalid input data")
    {
      REQUIRE_FALSE( stringToLong("-", value));
      REQUIRE_FALSE( stringToLong("12a", value));
      REQUIRE_FALSE( stringToLong("  12", value));
      REQUIRE_FALSE( stringToLong("1abcdef", value));
      REQUIRE_FALSE( stringToLong("Hello", value));
      REQUIRE_FALSE( stringToLong("Hello123", value));
    }

    SECTION("overflow")
    {
      REQUIRE_FALSE( stringToLong("2147483648", value));
      REQUIRE_FALSE( stringToLong("-2147483649", value));
      // overflow more
      REQUIRE_FALSE( stringToLong("2147489999", value));
    }
  }

  SECTION("stringToShort")
  {
    int16_t value;

    SECTION("empty string")
    {
      REQUIRE_FALSE( stringToShort("", value) );
    }

    SECTION("normal input data")
    {
      REQUIRE( stringToShort("0", value));
      REQUIRE( value == 0 );

      REQUIRE( stringToShort("1", value));
      REQUIRE( value == 1 );

      REQUIRE( stringToShort("1234", value));
      REQUIRE( value == 1234 );

      REQUIRE( stringToShort("12345", value));
      REQUIRE( value == 12345 );

      REQUIRE( stringToShort("32767", value));
      REQUIRE( value == 32767 );

      REQUIRE( stringToShort("-1", value));
      REQUIRE( value == -1 );

      REQUIRE( stringToShort("-12", value));
      REQUIRE( value == -12 );

      REQUIRE( stringToShort("-1234", value));
      REQUIRE( value == -1234 );

      REQUIRE( stringToShort("-12345", value));
      REQUIRE( value == -12345 );

      REQUIRE( stringToShort("-32767", value));
      REQUIRE( value == -32767 );
    }

    SECTION("invalid input data")
    {
      REQUIRE_FALSE( stringToShort("-", value));
      REQUIRE_FALSE( stringToShort("12a", value));
      REQUIRE_FALSE( stringToShort("  12", value));
      REQUIRE_FALSE( stringToShort("1abcdef", value));
      REQUIRE_FALSE( stringToShort("Hello", value));
      REQUIRE_FALSE( stringToShort("Hello123", value));
    }

    SECTION("overflow")
    {
      REQUIRE_FALSE( stringToShort("32768", value));
      REQUIRE_FALSE( stringToShort("-32769", value));
      // overflow more
      REQUIRE_FALSE( stringToShort("45678", value));
    }
  }

  SECTION("stripEnclosingQuotes")
  {
    std::string s;

    SECTION("empty string")
    {
      s = "";
      REQUIRE_FALSE( stripEnclosingQuotes(s) );
      REQUIRE( s == "" );
    }

    SECTION("quotes")
    {
      s = "\"\"";
      REQUIRE( stripEnclosingQuotes(s) );
      REQUIRE( s == "" );

      s = "\"To remove or not to remove, that is the question!\"";
      REQUIRE( stripEnclosingQuotes(s) );
      REQUIRE( s == "To remove or not to remove, that is the question!" );

      // Single quotes should not get removed.
      s = "'To remove or not to remove, that is the question!'";
      REQUIRE_FALSE( stripEnclosingQuotes(s) );
      REQUIRE( s == "'To remove or not to remove, that is the question!'" );
    }

    SECTION("mismatched quotes")
    {
      s = "\"foo'";
      REQUIRE_FALSE( stripEnclosingQuotes(s) );
      REQUIRE( s == "\"foo'" );

      s = "'foo\"";
      REQUIRE_FALSE( stripEnclosingQuotes(s) );
      REQUIRE( s == "'foo\"" );

      s = "\"foo";
      REQUIRE_FALSE( stripEnclosingQuotes(s) );
      REQUIRE( s == "\"foo" );

      s = "foo\"";
      REQUIRE_FALSE( stripEnclosingQuotes(s) );
      REQUIRE( s == "foo\"" );
    }

    SECTION("nested quotes")
    {
      s = "\"foo \"bar\"\"";
      REQUIRE( stripEnclosingQuotes(s) );
      REQUIRE( s == "foo \"bar\"" );
    }
  }

  SECTION("trim")
  {
    std::string s;

    SECTION("empty string")
    {
      s = "";
      trim(s);
      REQUIRE( s == "" );
    }

    SECTION("whitespace string")
    {
      s = "        ";
      trim(s);
      REQUIRE( s == "" );

      s = "\t   \t\t\t\t   ";
      trim(s);
      REQUIRE( s == "" );
    }

    SECTION("nothing to trim")
    {
      s = "abcdefghijk";
      trim(s);
      REQUIRE( s == "abcdefghijk" );
    }

    SECTION("left side trimming")
    {
      s = " a";
      trim(s);
      REQUIRE( s == "a" );

      s = " abc";
      trim(s);
      REQUIRE( s == "abc" );

      s = "            abcd";
      trim(s);
      REQUIRE( s == "abcd" );

      s = "  \t  \t    abcdef";
      trim(s);
      REQUIRE( s == "abcdef" );
    }

    SECTION("right side trimming")
    {
      s = "a ";
      trim(s);
      REQUIRE( s == "a" );

      s = "abc ";
      trim(s);
      REQUIRE( s == "abc" );

      s = "abcd            ";
      trim(s);
      REQUIRE( s == "abcd" );

      s = "abcdef  \t  \t    ";
      trim(s);
      REQUIRE( s == "abcdef" );
    }

    SECTION("trimming both sides")
    {
      s = " a ";
      trim(s);
      REQUIRE( s == "a" );

      s = " abc ";
      trim(s);
      REQUIRE( s == "abc" );

      s = "         abcd            ";
      trim(s);
      REQUIRE( s == "abcd" );

      s = "  \t  \t    abcdef  \t  \t    ";
      trim(s);
      REQUIRE( s == "abcdef" );
    }
  }

  SECTION("trimLeft")
  {
    std::string s;

    SECTION("empty string")
    {
      s = "";
      trimLeft(s);
      REQUIRE( s == "" );
    }

    SECTION("whitespace string")
    {
      s = "        ";
      trimLeft(s);
      REQUIRE( s == "" );

      s = "\t   \t\t\t\t   ";
      trimLeft(s);
      REQUIRE( s == "" );
    }

    SECTION("nothing to trim")
    {
      s = "abcdefghijk";
      trimLeft(s);
      REQUIRE( s == "abcdefghijk" );
    }

    SECTION("left side trimming")
    {
      s = " a";
      trimLeft(s);
      REQUIRE( s == "a" );

      s = " abc";
      trimLeft(s);
      REQUIRE( s == "abc" );

      s = "            abcd";
      trimLeft(s);
      REQUIRE( s == "abcd" );

      s = "  \t  \t    abcdef";
      trimLeft(s);
      REQUIRE( s == "abcdef" );
    }

    SECTION("right side trimming does not happen")
    {
      s = "abcd            ";
      trimLeft(s);
      REQUIRE( s == "abcd            " );

      s = "abcdef  \t  \t    ";
      trimLeft(s);
      REQUIRE( s == "abcdef  \t  \t    " );
    }

    SECTION("whitespace on both sides")
    {
      s = " a ";
      trimLeft(s);
      REQUIRE( s == "a " );

      s = " abc ";
      trimLeft(s);
      REQUIRE( s == "abc " );

      s = "         abcd            ";
      trimLeft(s);
      REQUIRE( s == "abcd            " );

      s = "  \t  \t    abcdef  \t  \t    ";
      trimLeft(s);
      REQUIRE( s == "abcdef  \t  \t    " );
    }
  }
}
