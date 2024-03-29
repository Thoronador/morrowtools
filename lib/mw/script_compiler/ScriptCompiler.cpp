/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014, 2022  Dirk Stolle

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

#include "ScriptCompiler.hpp"
#include <iostream>
#include <stdexcept>
#include "CompilerCodes.hpp"
#include "CompiledChunk.hpp"
#include "../../base/UtilityFunctions.hpp"
#include "ParserNode.hpp"
#include "../Statics.hpp"
#include "../NPCs.hpp"
#include "../MagicEffects.hpp"
#include "../Globals.hpp"
#include "../Creatures.hpp"
#include "../Containers.hpp"
#include "../Activators.hpp"
#include "../Scripts.hpp"

namespace MWTP
{

namespace ScriptCompiler
{

enum SC_CompareType{compNone, compLess, compLessEqual, compEqual,
                    compGreaterEqual, compGreater, compNotEqual};

void StripEnclosingQuotes(std::string& str1)
{
  if (str1.empty() || (str1.length() < 2))
    return;
  if ((str1.at(0) == '"') && (str1.at(str1.length() - 1) == '"'))
  {
    str1 = str1.substr(1, str1.length() - 2);
  }
  return;
}

std::string::size_type getCommentStart(const std::string& line)
{
  const std::string::size_type len = line.length();
  std::string::size_type look = 0;
  bool outsideQuote = true;
  while (look < len)
  {
    if (line.at(look) == '"')
    {
      outsideQuote = !outsideQuote;
    }
    else if ((outsideQuote) && (line.at(look) == ';'))
    {
      // found a place where comment starts
      return look;
    }
    ++look;
  }
  return std::string::npos;
}

std::string::size_type getPosOf_To_(const std::string& line)
{
  const std::string::size_type len = line.length();
  std::string::size_type look = 0;
  bool outsideQuote = true;
  while (look < len)
  {
    if (line.at(look) == '"')
    {
      outsideQuote = !outsideQuote;
    }
    else if (outsideQuote && ((line.at(look) == ' ') || (line.at(look) == '\t')))
    {
      // found a place where " to " could start
      if (look + 3 < len)
      {
        if ((line.at(look + 1) == 't') || (line.at(look + 1) == 'T'))
        {
          if ((line.at(look + 2) == 'o') || (line.at(look + 2) == 'O'))
          {
            if ((line.at(look + 3) == ' ') || (line.at(look + 3) == '\t'))
              return look;
          } // 'o' found
        } // 't' found
      }
    }
    ++look;
  }
  return std::string::npos;
}

std::string::size_type getQualifierStart(const std::string& line)
{
  const std::string::size_type len = line.length();
  std::string::size_type look = 0;
  bool outsideQuote = true;
  while (look < len)
  {
    if (line.at(look) == '"')
    {
      outsideQuote = !outsideQuote;
    }
    else if (outsideQuote && (line.at(look) == '-'))
    {
      // found a place where arrow could start
      if (look + 1 < len)
      {
        // check if next character is ">"
        if (line.at(look + 1) == '>')
          return look;
      }
    }
    ++look;
  }
  return std::string::npos;
}

std::string::size_type getComparePos(const std::string& line, SC_CompareType& comp)
{
  const std::string::size_type len = line.length();
  std::string::size_type look = 0;
  bool outsideQuote = true;
  while (look < len)
  {
    if (line.at(look) == '"')
    {
      outsideQuote = !outsideQuote;
    }
    else if (outsideQuote)
    {
      switch (line.at(look))
      {
        case '<':
             if (look < len - 1)
             {
               if (line.at(look + 1) == '=')
               {
                 comp = compLessEqual;
                 return look;
               }
             }
             comp = compLess;
             return look;
             break;
        case '=':
             if (look < len - 1)
             {
               if (line.at(look + 1) == '=')
               {
                 comp = compEqual;
                 return look;
               }
             }
             break;
        case '>':
             if (look < len - 1)
             {
               if (line.at(look + 1) == '=')
               {
                 comp = compGreaterEqual;
                 return look;
               }
             }
             // make sure we don't split a qualifier ("->") here
             if (look > 0)
             {
               if (line.at(look - 1) != '-')
               {
                 comp = compGreater;
                 return look;
               }
             }
             else
             {
               comp = compGreater;
               return look;
             }
             break;
        case '!':
             if (look < len - 1)
             {
               if (line.at(look + 1) == '=')
               {
                 comp = compNotEqual;
                 return look;
               }
             }
             break;
      }
    }
    ++look;
  }
  comp = compNone;
  return std::string::npos;
}

std::string::size_type getDotPosition(const std::string& line)
{
  const std::string::size_type len = line.length();
  std::string::size_type look = 0;
  bool outsideQuote = true;
  while (look < len)
  {
    if (line.at(look) == '"')
    {
      outsideQuote = !outsideQuote;
    }
    else if (outsideQuote && (line.at(look) == '.'))
    {
      // found a dot
      return look;
    }
    ++look;
  }
  return std::string::npos;
}

std::vector<std::string> explodeParams(const std::string& source)
{
  std::vector<std::string> result;
  if (source.empty())
    return result;

  const unsigned int len = source.length();
  unsigned int look = 0;
  unsigned int offset = 0;
  bool insideQuote = false;
  while (look < len)
  {
    if (source.at(look) == '"')
    {
      insideQuote = !insideQuote;
    }
    else if ((!insideQuote) && ((source[look] == ' ')
              || (source[look] == ',') || (source[look] == '\t')))
    {
      // found a place where to split
      const unsigned int length = look - offset;
      if (length > 0) // skip empty params
      {
        result.push_back(source.substr(offset, length));
        StripEnclosingQuotes(result.back());
      }
      offset = look + 1;
    }
    ++look;
  }
  // add the rest, if not finished yet
  if (offset < len)
  {
    result.push_back(source.substr(offset));
    StripEnclosingQuotes(result.back());
  }
  if (insideQuote)
  {
    std::cout << "Warning: explodeParams: Quotes did not match!\n";
  }
  return result;
}

// returns true, if the given string is one piece that will not be divided
bool isSingleToken(const std::string& line)
{
  if ((getDotPosition(line) == std::string::npos)
      && (getNextOperatorPos(line, 0) == std::string::npos))
  {
    // all things checked? I hope so.
    return explodeParams(line).size() < 2;
  }
  return false;
}

unsigned int getEndifForIf(const std::vector<std::string>& lines, const unsigned int start)
{
  const std::vector<std::string>::size_type len = lines.size();
  if (start >= len)
    return start;
  unsigned int look = start + 1;
  while (look < len)
  {
    const std::string lowerLine = lowerCase(lines.at(look));
    if (lowerLine == "endif")
    {
      return look;
    }
    else if (lowerLine.substr(0,3) == "if ")
    {
      const unsigned int res = getEndifForIf(lines, look);
      if (res == look)
      {
        // no match found
        return start;
      }
      // set look to the position of endif of the inner if block
      look = res;
    }
    ++look;
  }
  // nothing found, return start (i.e. failure)
  return start;
}

unsigned int getEndOfIf(const std::vector<std::string>& lines, const unsigned int start)
{
  const std::vector<std::string>::size_type len = lines.size();
  if (start >= len)
    return start;
  unsigned int look = start + 1;
  while (look < len)
  {
    const std::string lowerLine = lowerCase(lines.at(look));
    if ((lowerLine == "endif") || (lowerLine == "else")
        || (lowerLine.substr(0,7) == "elseif "))
    {
      return look;
    }
    else if (lowerLine.substr(0,3) == "if ")
    {
      const unsigned int res = getEndifForIf(lines, look);
      if (res == look)
      {
        // no match found
        return start;
      }
      // set look to the position of endif of the inner if block
      look = res;
    }
    ++look;
  }
  // nothing found, return start (i.e. failure)
  return start;
}

unsigned int getEndOfElse(const std::vector<std::string>& lines, const unsigned int start)
{
  const std::vector<std::string>::size_type len = lines.size();
  if (start >= len)
    return start;
  unsigned int look = start + 1;
  while (look < len)
  {
    const std::string lowerLine = lowerCase(lines.at(look));
    if (lowerLine == "endif")
    {
      return look;
    }
    else if (lowerLine.substr(0,3) == "if ")
    {
      const unsigned int res = getEndifForIf(lines, look);
      if (res == look)
      {
        // no match found
        return start;
      }
      // set look to the position of endif of the inner if block
      look = res;
    }
    ++look;
  }
  // nothing found, return start (i.e. failure)
  return start;
}

unsigned int getEndOfElseIf(const std::vector<std::string>& lines, const unsigned int start)
{
  const std::vector<std::string>::size_type len = lines.size();
  if (start >= len)
    return start;
  unsigned int look = start + 1;
  while (look < len)
  {
    const std::string lowerLine = lowerCase(lines.at(look));
    if ((lowerLine == "endif") || (lowerLine == "else")
      || (lowerLine.substr(0,7) == "elseif "))
    {
      return look;
    }
    else if (lowerLine.substr(0,3) == "if ")
    {
      const unsigned int res = getEndifForIf(lines, look);
      if (res == look)
      {
        // no match found
        return start;
      }
      // set look to the position of endif of the inner if block
      look = res;
    }
    ++look;
  }
  // nothing found, return start (i.e. failure)
  return start;
}

unsigned int getEndOfWhile(const std::vector<std::string>& lines, const unsigned int start)
{
  const std::vector<std::string>::size_type len = lines.size();
  if (start >= len)
    return start;
  unsigned int look = start + 1;
  while (look < len)
  {
    const std::string lowerLine = lowerCase(lines.at(look));
    // Is this the end of the loop?
    if (lowerLine == "endwhile")
    {
      return look;
    }
    else if (lowerLine.substr(0,6) == "while ")
    {
      const unsigned int res = getEndOfWhile(lines, look);
      if (res == look)
      {
        // no match found
        return start;
      }
      // set look to the position of endwhile of the inner while loop
      look = res;
    }
    ++look;
  }
  // nothing found, return start (i.e. failure)
  return start;
}

bool getAnimationGroupIndex(const std::string& groupName, int16_t& result)
{
  /***** TODO: add more animation groups *****/
  const std::string lcGroup = lowerCase(groupName);
  if (lcGroup == "attack1")
  {
    result = 0x83;
    return true;
  }
  if (lcGroup == "attack2")
  {
    result = 0x84;
    return true;
  }
  if (lcGroup == "attack3")
  {
    result = 0x85;
    return true;
  }
  if (lcGroup == "bowandarrow")
  {
    result = 0x8B;
    return true;
  }
  if (lcGroup == "crossbow")
  {
    result = 0x8A;
    return true;
  }
  if (lcGroup == "death1")
  {
    result = 27;
    return true;
  }
  if (lcGroup == "death2")
  {
    result = 28;
    return true;
  }
  if (lcGroup == "death3")
  {
    result = 29;
    return true;
  }
  if (lcGroup == "death4")
  {
    result = 30;
    return true;
  }
  if (lcGroup == "death5")
  {
    result = 31;
    return true;
  }
  if (lcGroup == "deathknockdown")
  {
    result = 0x20;
    return true;
  }
  if (lcGroup == "deathknockout")
  {
    result = 0x21;
    return true;
  }
  if (lcGroup == "handtohand")
  {
    result = 0x89;
    return true;
  }
  if (lcGroup == "hit1")
  {
    result = 0x13;
    return true;
  }
  if (lcGroup == "hit2")
  {
    result = 0x14;
    return true;
  }
  if (lcGroup == "hit3")
  {
    result = 0x15;
    return true;
  }
  if (lcGroup == "hit4")
  {
    result = 0x16;
    return true;
  }
  if (lcGroup == "hit5")
  {
    result = 0x17;
    return true;
  }
  if (lcGroup == "idle")
  {
    result = 0;
    return true;
  }
  if (lcGroup == "idle1h")
  {
    result = 0x0A;
    return true;
  }
  if (lcGroup == "idle2")
  {
    result = 1;
    return true;
  }
  if (lcGroup == "idle2c")
  {
    result = 0x0B;
    return true;
  }
  if (lcGroup == "idle2w")
  {
    result = 0x0C;
    return true;
  }
  if (lcGroup == "idle3")
  {
    result = 2;
    return true;
  }
  if (lcGroup == "idle4")
  {
    result = 3;
    return true;
  }
  if (lcGroup == "idle5")
  {
    result = 4;
    return true;
  }
  if (lcGroup == "idle6")
  {
    result = 5;
    return true;
  }
  if (lcGroup == "idle7")
  {
    result = 6;
    return true;
  }
  if (lcGroup == "idle8")
  {
    result = 7;
    return true;
  }
  if (lcGroup == "idle9")
  {
    result = 8;
    return true;
  }
  if (lcGroup == "idlecrossbow")
  {
    result = 0x0F;
    return true;
  }
  if (lcGroup == "idlehh")
  {
    result = 0x09;
    return true;
  }
  if (lcGroup == "idlesneak")
  {
    result = 0x10;
    return true;
  }
  if (lcGroup == "idlespell")
  {
    result = 0x0E;
    return true;
  }
  if (lcGroup == "idlestorm")
  {
    result = 0x11;
    return true;
  }
  if (lcGroup == "idleswim")
  {
    result = 0x0D;
    return true;
  }
  if (lcGroup == "inventoryhandtohand")
  {
    result = 0x92;
    return true;
  }
  if (lcGroup == "inventoryweapononehand")
  {
    result = 0x93;
    return true;
  }
  if (lcGroup == "inventoryweapontwohand")
  {
    result = 0x94;
    return true;
  }
  if (lcGroup == "inventoryweapontwowide")
  {
    result = 0x95;
    return true;
  }
  if (lcGroup == "jump")
  {
    result = 0x43;
    return true;
  }
  if (lcGroup == "jump1h")
  {
    result = 0x61;
    return true;
  }
  if (lcGroup == "jump2c")
  {
    result = 0x70;
    return true;
  }
  if (lcGroup == "jump2w")
  {
    result = 0x7F;
    return true;
  }
  if (lcGroup == "jumphh")
  {
    result = 0x52;
    return true;
  }
  if (lcGroup == "knockdown")
  {
    result = 0x22;
    return true;
  }
  if (lcGroup == "knockout")
  {
    result = 0x23;
    return true;
  }
  if (lcGroup == "pickprobe")
  {
    result = 0x91;
    return true;
  }
  if (lcGroup == "runback")
  {
    result = 0x3C;
    return true;
  }
  if (lcGroup == "runback1h")
  {
    result = 0x5A;
    return true;
  }
  if (lcGroup == "runback2c")
  {
    result = 0x69;
    return true;
  }
  if (lcGroup == "runback2w")
  {
    result = 0x78;
    return true;
  }
  if (lcGroup == "runbackhh")
  {
    result = 0x4B;
    return true;
  }
  if (lcGroup == "runforward")
  {
    result = 0x3B;
    return true;
  }
  if (lcGroup == "runforward1h")
  {
    result = 0x59;
    return true;
  }
  if (lcGroup == "runforward2c")
  {
    result = 0x68;
    return true;
  }
  if (lcGroup == "runforward2w")
  {
    result = 0x77;
    return true;
  }
  if (lcGroup == "runforwardhh")
  {
    result = 0x4A;
    return true;
  }
  if (lcGroup == "runleft")
  {
    result = 0x3D;
    return true;
  }
  if (lcGroup == "runleft1h")
  {
    result = 0x5B;
    return true;
  }
  if (lcGroup == "runleft2c")
  {
    result = 0x6A;
    return true;
  }
  if (lcGroup == "runleft2w")
  {
    result = 0x79;
    return true;
  }
  if (lcGroup == "runlefthh")
  {
    result = 0x4C;
    return true;
  }
  if (lcGroup == "runright")
  {
    result = 0x3E;
    return true;
  }
  if (lcGroup == "runright1h")
  {
    result = 0x5C;
    return true;
  }
  if (lcGroup == "runright2c")
  {
    result = 0x6B;
    return true;
  }
  if (lcGroup == "runright2w")
  {
    result = 0x7A;
    return true;
  }
  if (lcGroup == "runrighthh")
  {
    result = 0x4D;
    return true;
  }
  if (lcGroup == "shield")
  {
    result = 0x90;
    return true;
  }
  if (lcGroup == "sneakback")
  {
    result = 0x40;
    return true;
  }
  if (lcGroup == "sneakback1h")
  {
    result = 0x5E;
    return true;
  }
  if (lcGroup == "sneakback2c")
  {
    result = 0x6D;
    return true;
  }
  if (lcGroup == "sneakback2w")
  {
    result = 0x7C;
    return true;
  }
  if (lcGroup == "sneakbackhh")
  {
    result = 0x4F;
    return true;
  }
  if (lcGroup == "sneakforward")
  {
    result = 0x3F;
    return true;
  }
  if (lcGroup == "sneakforward1h")
  {
    result = 0x5D;
    return true;
  }
  if (lcGroup == "sneakforward2c")
  {
    result = 0x6C;
    return true;
  }
  if (lcGroup == "sneakforward2w")
  {
    result = 0x7B;
    return true;
  }
  if (lcGroup == "sneakforwardhh")
  {
    result = 0x4E;
    return true;
  }
  if (lcGroup == "sneakleft")
  {
    result = 0x41;
    return true;
  }
  if (lcGroup == "sneakleft1h")
  {
    result = 0x5F;
    return true;
  }
  if (lcGroup == "sneakleft2c")
  {
    result = 0x6E;
    return true;
  }
  if (lcGroup == "sneakleft2w")
  {
    result = 0x7D;
    return true;
  }
  if (lcGroup == "sneaklefthh")
  {
    result = 0x50;
    return true;
  }
  if (lcGroup == "sneakright")
  {
    result = 0x42;
    return true;
  }
  if (lcGroup == "sneakright1h")
  {
    result = 0x60;
    return true;
  }
  if (lcGroup == "sneakright2c")
  {
    result = 0x6F;
    return true;
  }
  if (lcGroup == "sneakright2w")
  {
    result = 0x7E;
    return true;
  }
  if (lcGroup == "sneakright2w")
  {
    result = 0x51;
    return true;
  }
  if (lcGroup == "spellcast")
  {
    result = 0x80;
    return true;
  }
  if (lcGroup == "spellturnleft")
  {
    result = 0x81;
    return true;
  }
  if (lcGroup == "spellturnright")
  {
    result = 0x82;
    return true;
  }
  if (lcGroup == "swimattack1")
  {
    result = 0x86;
    return true;
  }
  if (lcGroup == "swimattack2")
  {
    result = 0x87;
    return true;
  }
  if (lcGroup == "swimattack3")
  {
    result = 0x88;
    return true;
  }
  if (lcGroup == "swimdeath")
  {
    result = 0x24;
    return true;
  }
  if (lcGroup == "swimdeath2")
  {
    result = 0x25;
    return true;
  }
  if (lcGroup == "swimdeath3")
  {
    result = 0x26;
    return true;
  }
  if (lcGroup == "swimdeathknockdown")
  {
    result = 0x27;
    return true;
  }
  if (lcGroup == "swimdeathknockout")
  {
    result = 0x28;
    return true;
  }
  if (lcGroup == "swimhit1")
  {
    result = 0x18;
    return true;
  }
  if (lcGroup == "swimhit2")
  {
    result = 0x19;
    return true;
  }
  if (lcGroup == "swimhit3")
  {
    result = 0x1A;
    return true;
  }
  if (lcGroup == "swimknockdown")
  {
    result = 0x2A;
    return true;
  }
  if (lcGroup == "swimknockout")
  {
    result = 0x29;
    return true;
  }
  if (lcGroup == "swimrunback")
  {
    result = 0x30;
    return true;
  }
  if (lcGroup == "swimrunforward")
  {
    result = 0x2F;
    return true;
  }
  if (lcGroup == "swimrunleft")
  {
    result = 0x31;
    return true;
  }
  if (lcGroup == "swimrunright")
  {
    result = 0x32;
    return true;
  }
  if (lcGroup == "swimturnleft")
  {
    result = 0x33;
    return true;
  }
  if (lcGroup == "swimturnright")
  {
    result = 0x34;
    return true;
  }
  if (lcGroup == "swimwalkback")
  {
    result = 0x2C;
    return true;
  }
  if (lcGroup == "swimwalkforward")
  {
    result = 0x2B;
    return true;
  }
  if (lcGroup == "swimwalkleft")
  {
    result = 0x2D;
    return true;
  }
  if (lcGroup == "swimwalkright")
  {
    result = 0x2E;
    return true;
  }
  if (lcGroup == "throwweapon")
  {
    result = 0x8C;
    return true;
  }
  if (lcGroup == "torch")
  {
    result = 0x12;
    return true;
  }
  if (lcGroup == "turnleft")
  {
    result = 0x39;
    return true;
  }
  if (lcGroup == "turnleft1h")
  {
    result = 0x57;
    return true;
  }
  if (lcGroup == "turnleft2c")
  {
    result = 0x66;
    return true;
  }
  if (lcGroup == "turnleft2w")
  {
    result = 0x75;
    return true;
  }
  if (lcGroup == "turnlefthh")
  {
    result = 0x48;
    return true;
  }
  if (lcGroup == "turnright")
  {
    result = 0x3A;
    return true;
  }
  if (lcGroup == "turnright1h")
  {
    result = 0x58;
    return true;
  }
  if (lcGroup == "turnright2c")
  {
    result = 0x67;
    return true;
  }
  if (lcGroup == "turnright2w")
  {
    result = 0x76;
    return true;
  }
  if (lcGroup == "turnrighthh")
  {
    result = 0x49;
    return true;
  }
  if (lcGroup == "walkback")
  {
    result = 0x36;
    return true;
  }
  if (lcGroup == "walkback1h")
  {
    result = 0x54;
    return true;
  }
  if (lcGroup == "walkback2c")
  {
    result = 0x63;
    return true;
  }
  if (lcGroup == "walkback2w")
  {
    result = 0x72;
    return true;
  }
  if (lcGroup == "walkbackhh")
  {
    result = 0x45;
    return true;
  }
  if (lcGroup == "walkforward")
  {
    result = 0x35;
    return true;
  }
  if (lcGroup == "walkforward1h")
  {
    result = 0x53;
    return true;
  }
  if (lcGroup == "walkforward2c")
  {
    result = 0x62;
    return true;
  }
  if (lcGroup == "walkforward2w")
  {
    result = 0x71;
    return true;
  }
  if (lcGroup == "walkforwardhh")
  {
    result = 0x44;
    return true;
  }
  if (lcGroup == "walkleft")
  {
    result = 0x37;
    return true;
  }
  if (lcGroup == "walkleft1h")
  {
    result = 0x55;
    return true;
  }
  if (lcGroup == "walkleft2c")
  {
    result = 0x64;
    return true;
  }
  if (lcGroup == "walkleft2w")
  {
    result = 0x73;
    return true;
  }
  if (lcGroup == "walklefthh")
  {
    result = 0x46;
    return true;
  }
  if (lcGroup == "walkright")
  {
    result = 0x38;
    return true;
  }
  if (lcGroup == "walkright1h")
  {
    result = 0x56;
    return true;
  }
  if (lcGroup == "walkright2c")
  {
    result = 0x65;
    return true;
  }
  if (lcGroup == "walkright2w")
  {
    result = 0x74;
    return true;
  }
  if (lcGroup == "walkrighthh")
  {
    result = 0x47;
    return true;
  }
  if (lcGroup == "weapononehand")
  {
    result = 0x8D;
    return true;
  }
  if (lcGroup == "weapontwohand")
  {
    result = 0x8E;
    return true;
  }
  if (lcGroup == "weapontwowide")
  {
    result = 0x8F;
    return true;
  }
  return false;
}

SC_VarRef getScriptsVariableTypeWithIndex(const ScriptRecord& theScript, const std::string& varName)
{
  if (theScript.LocalVars.size() != (theScript.NumShorts + theScript.NumLongs
                                     + theScript.NumFloats))
  {
    std::cerr << "ScriptCompiler: Error: Number of local vars of script \""
              << theScript.recordID << "\" does not match the variables inside.\n";
    return SC_VarRef(vtGlobal, 0);
  }
  const std::string lcName = lowerCase(varName);
  for (unsigned int i = 0; i < theScript.LocalVars.size(); ++i)
  {
    if (lcName == lowerCase(theScript.LocalVars.at(i)))
    {
      // match found
      // Is it a float var?
      if (i >= theScript.NumShorts+theScript.NumLongs)
      {
        return SC_VarRef(vtFloat, i - (theScript.NumShorts + theScript.NumLongs) + 1);
      }
      // Is it a long var?
      if (i >= theScript.NumShorts)
      {
        return SC_VarRef(vtLong, i - theScript.NumShorts + 1);
      }
      // it's a short
      else return SC_VarRef(vtShort, i + 1);
    }
  }
  // no match found
  return SC_VarRef(vtGlobal, 0);
}

SC_VarRef getForeignVariableTypeWithIndex(const std::string& objectID, const std::string& varName)
{
  std::string ScriptID = "";
  if (Activators::get().hasRecord(objectID))
  {
    ScriptID = Activators::get().getRecord(objectID).ScriptName;
  }
  else if (NPCs::get().hasRecord(objectID))
  {
    ScriptID = NPCs::get().getRecord(objectID).ScriptID;
  }
  else if (Creatures::get().hasRecord(objectID))
  {
    ScriptID = Creatures::get().getRecord(objectID).ScriptID;
  }
  else if (Containers::get().hasRecord(objectID))
  {
    ScriptID = Containers::get().getRecord(objectID).ScriptID;
  }
  ///TODO: add more stuff (e.g. weapons) later
  if (!ScriptID.empty())
  {
    if (Scripts::get().hasRecord(ScriptID))
    {
      return getScriptsVariableTypeWithIndex(Scripts::get().getRecord(ScriptID), varName);
    }
  }
  return SC_VarRef(vtGlobal, 0);
}

/*returns the proper ID (with exact upper/lower case spelling) of an object, if
  present. If no object can be found, objectID is returned.
*/
const std::string& getObjectsProperID(const std::string& objectID)
{
  if (Activators::get().hasRecord(objectID))
  {
    return Activators::get().getRecord(objectID).recordID;
  }
  if (NPCs::get().hasRecord(objectID))
  {
    return NPCs::get().getRecord(objectID).recordID;
  }
  if (Creatures::get().hasRecord(objectID))
  {
    return Creatures::get().getRecord(objectID).recordID;
  }
  if (Statics::get().hasRecord(objectID))
  {
    return Statics::get().getRecord(objectID).recordID;
  }
  if (Containers::get().hasRecord(objectID))
  {
    return Containers::get().getRecord(objectID).recordID;
  }
  return objectID;
}

bool ScriptFunctions_ZeroParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  // entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "activate")
  {
    chunk.pushCode(CodeActivate);
    return true;
  }
  if (lowerFunction == "becomewerewolf")
  {
    chunk.pushCode(CodeBecomeWerewolf);
    return true;
  }
  if (lowerFunction == "cellchanged")
  {
    chunk.pushCode(CodeCellChanged);
    return true;
  }
  if (lowerFunction == "cellupdate")
  {
    chunk.pushCode(CodeCellUpdate);
    return true;
  }
  if (lowerFunction == "clearforcejump")
  {
    chunk.pushCode(CodeClearForceJump);
    return true;
  }
  if (lowerFunction == "clearforcemovejump")
  {
    chunk.pushCode(CodeClearForceMoveJump);
    return true;
  }
  if (lowerFunction == "clearforcerun")
  {
    chunk.pushCode(CodeClearForceRun);
    return true;
  }
  if (lowerFunction == "clearforcesneak")
  {
    chunk.pushCode(CodeClearForceSneak);
    return true;
  }
  if (lowerFunction == "clearinfoactor")
  {
    chunk.pushCode(CodeClearInfoActor);
    return true;
  }
  if (lowerFunction == "disable")
  {
    chunk.pushCode(CodeDisable);
    return true;
  }
  if (lowerFunction == "disablelevitation")
  {
    chunk.pushCode(CodeDisableLevitation);
    return true;
  }
  if (lowerFunction == "disableplayercontrols")
  {
    chunk.pushCode(CodeDisablePlayerControls);
    return true;
  }
  if (lowerFunction == "disableplayerfighting")
  {
    chunk.pushCode(CodeDisablePlayerFighting);
    return true;
  }
  if (lowerFunction == "disableplayerjumping")
  {
    chunk.pushCode(CodeDisablePlayerJumping);
    return true;
  }
  if (lowerFunction == "disableplayerlooking")
  {
    chunk.pushCode(CodeDisablePlayerLooking);
    return true;
  }
  if (lowerFunction == "disableplayermagic")
  {
    chunk.pushCode(CodeDisablePlayerMagic);
    return true;
  }
  if (lowerFunction == "disableplayerviewswitch")
  {
    chunk.pushCode(CodeDisablePlayerViewSwitch);
    return true;
  }
  if (lowerFunction == "disableteleporting")
  {
    chunk.pushCode(CodeDisableTeleporting);
    return true;
  }
  if (lowerFunction == "disablevanitymode")
  {
    chunk.pushCode(CodeDisableVanityMode);
    return true;
  }
  if (lowerFunction == "dontsaveobject")
  {
    chunk.pushCode(CodeDontSaveObject);
    return true;
  }
  if (lowerFunction == "enable")
  {
    chunk.pushCode(CodeEnable);
    return true;
  }
  if (lowerFunction == "enablebirthmenu")
  {
    chunk.pushCode(CodeEnableBirthMenu);
    return true;
  }
  if (lowerFunction == "enableclassmenu")
  {
    chunk.pushCode(CodeEnableClassMenu);
    return true;
  }
  if (lowerFunction == "enableinventorymenu")
  {
    chunk.pushCode(CodeEnableInventoryMenu);
    return true;
  }
  if (lowerFunction == "enablelevelupmenu")
  {
    chunk.pushCode(CodeEnableLevelUpMenu);
    return true;
  }
  if (lowerFunction == "enablelevitation")
  {
    chunk.pushCode(CodeEnableLevitation);
    return true;
  }
  if (lowerFunction == "enablemagicmenu")
  {
    chunk.pushCode(CodeEnableMagicMenu);
    return true;
  }
  if (lowerFunction == "enablemapmenu")
  {
    chunk.pushCode(CodeEnableMapMenu);
    return true;
  }
  if (lowerFunction == "enablenamemenu")
  {
    chunk.pushCode(CodeEnableNameMenu);
    return true;
  }
  if (lowerFunction == "enableplayercontrols")
  {
    chunk.pushCode(CodeEnablePlayerControls);
    return true;
  }
  if (lowerFunction == "enableplayerfighting")
  {
    chunk.pushCode(CodeEnablePlayerFighting);
    return true;
  }
  if (lowerFunction == "enableplayerjumping")
  {
    chunk.pushCode(CodeEnablePlayerJumping);
    return true;
  }
  if (lowerFunction == "enableplayerlooking")
  {
    chunk.pushCode(CodeEnablePlayerLooking);
    return true;
  }
  if (lowerFunction == "enableplayermagic")
  {
    chunk.pushCode(CodeEnablePlayerMagic);
    return true;
  }
  if (lowerFunction == "enableplayerviewswitch")
  {
    chunk.pushCode(CodeEnablePlayerViewSwitch);
    return true;
  }
  if (lowerFunction == "enableracemenu")
  {
    chunk.pushCode(CodeEnableRaceMenu);
    return true;
  }
  if (lowerFunction == "enablerest")
  {
    chunk.pushCode(CodeEnableRest);
    return true;
  }
  if (lowerFunction == "enablestatreviewmenu")
  {
    chunk.pushCode(CodeEnableStatReviewMenu);
    return true;
  }
  if (lowerFunction == "enablestatsmenu")
  {
    chunk.pushCode(CodeEnableStatsMenu);
    return true;
  }
  if (lowerFunction == "enableteleporting")
  {
    chunk.pushCode(CodeEnableTeleporting);
    return true;
  }
  if (lowerFunction == "enablevanitymode")
  {
    chunk.pushCode(CodeEnableVanityMode);
    return true;
  }
  if (lowerFunction == "fall")
  {
    chunk.pushCode(CodeFall);
    return true;
  }
  if (lowerFunction == "fillmap")
  {
    chunk.pushCode(CodeFillMap);
    return true;
  }
  if (lowerFunction == "fixme")
  {
    chunk.pushCode(CodeFixMe);
    return true;
  }
  if (lowerFunction == "forcegreeting")
  {
    chunk.pushCode(CodeForceGreeting);
    return true;
  }
  if (lowerFunction == "forcejump")
  {
    chunk.pushCode(CodeForceJump);
    return true;
  }
  if (lowerFunction == "forcemovejump")
  {
    chunk.pushCode(CodeForceMoveJump);
    return true;
  }
  if (lowerFunction == "forcerun")
  {
    chunk.pushCode(CodeForceRun);
    return true;
  }
  if (lowerFunction == "forcesneak")
  {
    chunk.pushCode(CodeForceSneak);
    return true;
  }
  if ((lowerFunction.substr(0,3) == "get") && (lowerFunction.length() > 3))
  {
    if (lowerFunction.at(3) == 'a')
    {
      if (lowerFunction == "getacrobatics")
      {
        chunk.pushCode(CodeGetAcrobatics);
        return true;
      }
      if (lowerFunction == "getagility")
      {
        chunk.pushCode(CodeGetAgility);
        return true;
      }
      if (lowerFunction == "getaipackagedone")
      {
        chunk.pushCode(CodeGetAIPackageDone);
        return true;
      }
      if (lowerFunction == "getalarm")
      {
        chunk.pushCode(CodeGetAlarm);
        return true;
      }
      if (lowerFunction == "getalchemy")
      {
        chunk.pushCode(CodeGetAlchemy);
        return true;
      }
      if (lowerFunction == "getalteration")
      {
        chunk.pushCode(CodeGetAlteration);
        return true;
      }
      if (lowerFunction == "getarmorbonus")
      {
        chunk.pushCode(CodeGetArmorBonus);
        return true;
      }
      if (lowerFunction == "getarmorer")
      {
        chunk.pushCode(CodeGetArmorer);
        return true;
      }
      if (lowerFunction == "getathletics")
      {
        chunk.pushCode(CodeGetAthletics);
        return true;
      }
      if (lowerFunction == "getattackbonus")
      {
        chunk.pushCode(CodeGetAttackBonus);
        return true;
      }
      if (lowerFunction == "getattacked")
      {
        chunk.pushCode(CodeGetAttacked);
        return true;
      }
      if (lowerFunction == "getaxe")
      {
        chunk.pushCode(CodeGetAxe);
        return true;
      }
      return false; // all zero argument functions with "GetA..." done
    } // if fourth character is 'a'
    if (lowerFunction == "getblightdisease")
    {
      chunk.pushCode(CodeGetBlightDisease);
      return true;
    }
    if (lowerFunction == "getblindness")
    {
      chunk.pushCode(CodeGetBlindness);
      return true;
    }
    if (lowerFunction == "getblock")
    {
      chunk.pushCode(CodeGetBlock);
      return true;
    }
    if (lowerFunction == "getbluntweapon")
    {
      chunk.pushCode(CodeGetBluntWeapon);
      return true;
    }
    if (lowerFunction == "getbuttonpressed")
    {
      chunk.pushCode(CodeGetButtonPressed);
      return true;
    }
    if (lowerFunction == "getcastpenalty")
    {
      chunk.pushCode(CodeGetCastPenalty);
      return true;
    }
    if (lowerFunction == "getchameleon")
    {
      chunk.pushCode(CodeGetChameleon);
      return true;
    }
    if (lowerFunction == "getcollidingactor")
    {
      chunk.pushCode(CodeGetCollidingActor);
      return true;
    }
    if (lowerFunction == "getcollidingpc")
    {
      chunk.pushCode(CodeGetCollidingPC);
      return true;
    }
    if (lowerFunction == "getcommondisease")
    {
      chunk.pushCode(CodeGetCommonDisease);
      return true;
    }
    if (lowerFunction == "getconjuration")
    {
      chunk.pushCode(CodeGetConjuration);
      return true;
    }
    if (lowerFunction == "getcurrentaipackage")
    {
      chunk.pushCode(CodeGetCurrentAIPackage);
      return true;
    }
    if (lowerFunction == "getcurrenttime")
    {
      chunk.pushCode(CodeGetCurrentTime);
      return true;
    }
    if (lowerFunction == "getcurrentweather")
    {
      chunk.pushCode(CodeGetCurrentWeather);
      return true;
    }
    if (lowerFunction == "getdefendbonus")
    {
      chunk.pushCode(CodeGetDefendBonus);
      return true;
    }
    if (lowerFunction == "getdestruction")
    {
      chunk.pushCode(CodeGetDestruction);
      return true;
    }
    if (lowerFunction == "getdisabled")
    {
      chunk.pushCode(CodeGetDisabled);
      return true;
    }
    if (lowerFunction == "getdisposition")
    {
      chunk.pushCode(CodeGetDisposition);
      return true;
    }
    if (lowerFunction == "getenchant")
    {
      chunk.pushCode(CodeGetEnchant);
      return true;
    }
    if (lowerFunction == "getendurance")
    {
      chunk.pushCode(CodeGetEndurance);
      return true;
    }
    if (lowerFunction == "getfatigue")
    {
      chunk.pushCode(CodeGetFatigue);
      return true;
    }
    if (lowerFunction == "getfight")
    {
      chunk.pushCode(CodeGetFight);
      return true;
    }
    if (lowerFunction == "getflee")
    {
      chunk.pushCode(CodeGetFlee);
      return true;
    }
    if (lowerFunction == "getflying")
    {
      chunk.pushCode(CodeGetFlying);
      return true;
    }
    if (lowerFunction == "getforcejump")
    {
      chunk.pushCode(CodeGetForceJump);
      return true;
    }
    if (lowerFunction == "getforcemovejump")
    {
      chunk.pushCode(CodeGetForceMoveJump);
      return true;
    }
    if (lowerFunction == "getforcerun")
    {
      chunk.pushCode(CodeGetForceRun);
      return true;
    }
    if (lowerFunction == "getforcesneak")
    {
      chunk.pushCode(CodeGetForceSneak);
      return true;
    }
    if (lowerFunction == "gethandtohand")
    {
      chunk.pushCode(CodeGetHandToHand);
      return true;
    }
    if (lowerFunction == "gethealth")
    {
      chunk.pushCode(CodeGetHealth);
      return true;
    }
    if (lowerFunction == "gethealthgetratio")
    {
      chunk.pushCode(CodeGetHealthGetRatio);
      return true;
    }
    if (lowerFunction == "getheavyarmor")
    {
      chunk.pushCode(CodeGetHeavyArmor);
      return true;
    }
    if (lowerFunction == "gethello")
    {
      chunk.pushCode(CodeGetHello);
      return true;
    }
    if (lowerFunction == "getillusion")
    {
      chunk.pushCode(CodeGetIllusion);
      return true;
    }
    if (lowerFunction == "getintelligence")
    {
      chunk.pushCode(CodeGetIntelligence);
      return true;
    }
    if (lowerFunction == "getinterior")
    {
      chunk.pushCode(CodeGetInterior);
      return true;
    }
    // check for both correct and earlier misspelled version of function name
    if ((lowerFunction == "getinvisible") || (lowerFunction == "getinvisibile"))
    {
      chunk.pushCode(CodeGetInvisible);
      return true;
    }
    if (lowerFunction == "getlevel")
    {
      chunk.pushCode(CodeGetLevel);
      return true;
    }
    if (lowerFunction == "getlightarmor")
    {
      chunk.pushCode(CodeGetLightArmor);
      return true;
    }
    if (lowerFunction == "getlocked")
    {
      chunk.pushCode(CodeGetLocked);
      return true;
    }
    if (lowerFunction == "getlongblade")
    {
      chunk.pushCode(CodeGetLongBlade);
      return true;
    }
    if (lowerFunction == "getluck")
    {
      chunk.pushCode(CodeGetLuck);
      return true;
    }
    if (lowerFunction == "getmagicka")
    {
      chunk.pushCode(CodeGetMagicka);
      return true;
    }
    if (lowerFunction == "getmarksman")
    {
      chunk.pushCode(CodeGetMarksman);
      return true;
    }
    if (lowerFunction == "getmasserphase")
    {
      chunk.pushCode(CodeGetMasserPhase);
      return true;
    }
    if (lowerFunction == "getmediumarmor")
    {
      chunk.pushCode(CodeGetMediumArmor);
      return true;
    }
    if (lowerFunction == "getmercantile")
    {
      chunk.pushCode(CodeGetMercantile);
      return true;
    }
    if (lowerFunction == "getmysticism")
    {
      chunk.pushCode(CodeGetMysticism);
      return true;
    }
    if (lowerFunction == "getparalysis")
    {
      chunk.pushCode(CodeGetParalysis);
      return true;
    }
    if (lowerFunction == "getpccrimelevel")
    {
      chunk.pushCode(CodeGetPCCrimeLevel);
      return true;
    }
    if (lowerFunction == "getpcfacrep")
    {
      // push function code
      chunk.pushCode(CodeGetPCFacRep);
      // push NUL byte to indicate absence of optional parameter
      chunk.data.push_back(0);
      return true;
    }
    if (lowerFunction == "getpcinjail")
    {
      chunk.pushCode(CodeGetPCInJail);
      return true;
    }
    if (lowerFunction == "getpcjumping")
    {
      chunk.pushCode(CodeGetPCJumping);
      return true;
    }
    if (lowerFunction == "getpcrank")
    {
      // push function code
      chunk.pushCode(CodeGetPCRank);
      // push NUL byte to indicate absence of optional parameter
      chunk.data.push_back(0);
      return true;
    }
    if (lowerFunction == "getpcrunning")
    {
      chunk.pushCode(CodeGetPCRunning);
      return true;
    }
    if (lowerFunction == "getpcsleep")
    {
      chunk.pushCode(CodeGetPCSleep);
      return true;
    }
    if (lowerFunction == "getpcsneaking")
    {
      chunk.pushCode(CodeGetPCSneaking);
      return true;
    }
    if (lowerFunction == "getpctraveling")
    {
      chunk.pushCode(CodeGetPCTraveling);
      return true;
    }
    if (lowerFunction == "getpcvisionbonus")
    {
      chunk.pushCode(CodeGetPCVisionBonus);
      return true;
    }
    if (lowerFunction == "getpersonality")
    {
      chunk.pushCode(CodeGetPersonality);
      return true;
    }
    if (lowerFunction == "getplayercontrolsdisabled")
    {
      chunk.pushCode(CodeGetPlayerControlsDisabled);
      return true;
    }
    if (lowerFunction == "getplayerfightingdisabled")
    {
      chunk.pushCode(CodeGetPlayerFightingDisabled);
      return true;
    }
    if (lowerFunction == "getplayerjumpingdisabled")
    {
      chunk.pushCode(CodeGetPlayerJumpingDisabled);
      return true;
    }
    if (lowerFunction == "getplayerlookingdisabled")
    {
      chunk.pushCode(CodeGetPlayerLookingDisabled);
      return true;
    }
    if (lowerFunction == "getplayermagicdisabled")
    {
      chunk.pushCode(CodeGetPlayerMagicDisabled);
      return true;
    }
    if (lowerFunction == "getreputation")
    {
      chunk.pushCode(CodeGetReputation);
      return true;
    }
    if (lowerFunction == "getresistblight")
    {
      chunk.pushCode(CodeGetResistBlight);
      return true;
    }
    if (lowerFunction == "getresistcorprus")
    {
      chunk.pushCode(CodeGetResistCorprus);
      return true;
    }
    if (lowerFunction == "getresistdisease")
    {
      chunk.pushCode(CodeGetResistDisease);
      return true;
    }
    if (lowerFunction == "getresistfire")
    {
      chunk.pushCode(CodeGetResistFire);
      return true;
    }
    if (lowerFunction == "getresistfrost")
    {
      chunk.pushCode(CodeGetResistFrost);
      return true;
    }
    if (lowerFunction == "getresistmagicka")
    {
      chunk.pushCode(CodeGetResistMagicka);
      return true;
    }
    if (lowerFunction == "getresistnormalweapons")
    {
      chunk.pushCode(CodeGetResistNormalWeapons);
      return true;
    }
    if (lowerFunction == "getresistparalysis")
    {
      chunk.pushCode(CodeGetResistParalysis);
      return true;
    }
    if (lowerFunction == "getresistpoison")
    {
      chunk.pushCode(CodeGetResistPoison);
      return true;
    }
    if (lowerFunction == "getresistshock")
    {
      chunk.pushCode(CodeGetResistShock);
      return true;
    }
    if (lowerFunction == "getrestoration")
    {
      chunk.pushCode(CodeGetRestoration);
      return true;
    }
    if (lowerFunction == "getscale")
    {
      chunk.pushCode(CodeGetScale);
      return true;
    }
    if (lowerFunction == "getsecondspassed")
    {
      chunk.pushCode(CodeGetSecondsPassed);
      return true;
    }
    if (lowerFunction == "getsecundaphase")
    {
      chunk.pushCode(CodeGetSecundaPhase);
      return true;
    }
    if (lowerFunction == "getsecurity")
    {
      chunk.pushCode(CodeGetSecurity);
      return true;
    }
    if (lowerFunction == "getshortblade")
    {
      chunk.pushCode(CodeGetShortBlade);
      return true;
    }
    if (lowerFunction == "getsilence")
    {
      chunk.pushCode(CodeGetSilence);
      return true;
    }
    if (lowerFunction == "getsneak")
    {
      chunk.pushCode(CodeGetSneak);
      return true;
    }
    if (lowerFunction == "getspear")
    {
      chunk.pushCode(CodeGetSpear);
      return true;
    }
    if (lowerFunction == "getspeechcraft")
    {
      chunk.pushCode(CodeGetSpeechcraft);
      return true;
    }
    if (lowerFunction == "getspeed")
    {
      chunk.pushCode(CodeGetSpeed);
      return true;
    }
    if (lowerFunction == "getspellreadied")
    {
      chunk.pushCode(CodeGetSpellReadied);
      return true;
    }
    if (lowerFunction == "getstandingactor")
    {
      chunk.pushCode(CodeGetStandingActor);
      return true;
    }
    if (lowerFunction == "getstandingpc")
    {
      chunk.pushCode(CodeGetStandingPC);
      return true;
    }
    if (lowerFunction == "getstrength")
    {
      chunk.pushCode(CodeGetStrength);
      return true;
    }
    if (lowerFunction == "getsuperjump")
    {
      chunk.pushCode(CodeGetSuperJump);
      return true;
    }
    if (lowerFunction == "getswimspeed")
    {
      chunk.pushCode(CodeGetSwimSpeed);
      return true;
    }
    if (lowerFunction == "getunarmored")
    {
      chunk.pushCode(CodeGetUnarmored);
      return true;
    }
    if (lowerFunction == "getvanitymodedisabled")
    {
      chunk.pushCode(CodeGetVanityModeDisabled);
      return true;
    }
    if (lowerFunction == "getwaterbreathing")
    {
      chunk.pushCode(CodeGetWaterBreathing);
      return true;
    }
    if (lowerFunction == "getwaterlevel")
    {
      chunk.pushCode(CodeGetWaterLevel);
      return true;
    }
    if (lowerFunction == "getwaterwalking")
    {
      chunk.pushCode(CodeGetWaterWalking);
      return true;
    }
    if (lowerFunction == "getweapondrawn")
    {
      chunk.pushCode(CodeGetWeaponDrawn);
      return true;
    }
    if (lowerFunction == "getweapontype")
    {
      chunk.pushCode(CodeGetWeaponType);
      return true;
    }
    if (lowerFunction == "getwerewolfkills")
    {
      chunk.pushCode(CodeGetWerewolfKills);
      return true;
    }
    if (lowerFunction == "getwillpower")
    {
      chunk.pushCode(CodeGetWillpower);
      return true;
    }
    if (lowerFunction == "getwindspeed")
    {
      chunk.pushCode(CodeGetWindSpeed);
      return true;
    }
    // no appropriate get function found, return here
    return false;
  } // get functions
  if (lowerFunction == "goodbye")
  {
    chunk.pushCode(CodeGoodbye);
    return true;
  }
  if (lowerFunction == "gotojail")
  {
    chunk.pushCode(CodeGotoJail);
    return true;
  }
  if (lowerFunction == "iswerewolf")
  {
    chunk.pushCode(CodeIsWerewolf);
    return true;
  }
  if (lowerFunction == "lowerrank")
  {
    chunk.pushCode(CodeLowerRank);
    return true;
  }
  if (lowerFunction == "menumode")
  {
    chunk.pushCode(CodeMenuMode);
    return true;
  }
  if (lowerFunction == "menutest")
  {
    chunk.pushCode(CodeMenuTest);
    // parameter is usually a short value, but it's omitted in this variant
    // push string's length as short (not byte)
    chunk.pushShort(0);
    return true;
  }
  if (lowerFunction == "onactivate")
  {
    chunk.pushCode(CodeOnActivate);
    return true;
  }
  if (lowerFunction == "ondeath")
  {
    chunk.pushCode(CodeOnDeath);
    return true;
  }
  if (lowerFunction == "onknockout")
  {
    chunk.pushCode(CodeOnKnockout);
    return true;
  }
  if (lowerFunction == "onmurder")
  {
    chunk.pushCode(CodeOnMurder);
    return true;
  }
  if (lowerFunction == "payfine")
  {
    chunk.pushCode(CodePayFine);
    return true;
  }
  if (lowerFunction == "payfinethief")
  {
    chunk.pushCode(CodePayFineThief);
    return true;
  }
  if (lowerFunction == "pcforce1stperson")
  {
    chunk.pushCode(CodePCForce1stPerson);
    return true;
  }
  if (lowerFunction == "pcforce3rdperson")
  {
    chunk.pushCode(CodePCForce3rdPerson);
    return true;
  }
  if (lowerFunction == "pcget3rdperson")
  {
    chunk.pushCode(CodePCGet3rdPerson);
    return true;
  }
  if (lowerFunction == "pcraiserank")
  {
    // push function
    chunk.pushCode(CodePCRaiseRank);
    // parameter is faction ID, but it's omitted here
    // push IDs length
    chunk.data.push_back(0);
    return true;
  } // if PCRaiseRank
  if (lowerFunction == "raiserank")
  {
    chunk.pushCode(CodeRaiseRank);
    return true;
  }
  if (lowerFunction == "resurrect")
  {
    chunk.pushCode(CodeResurrect);
    return true;
  }
  if (lowerFunction == "samefaction")
  {
    chunk.pushCode(CodeSameFaction);
    return true;
  }
  if (lowerFunction == "saydone")
  {
    chunk.pushCode(CodeSayDone);
    return true;
  }
  if (lowerFunction == "setatstart")
  {
    chunk.pushCode(CodeSetAtStart);
    return true;
  }
  if (lowerFunction == "setwerewolfacrobatics")
  {
    chunk.pushCode(CodeSetWerewolfAcrobatics);
    return true;
  }
  if (lowerFunction == "showrestmenu")
  {
    chunk.pushCode(CodeShowRestMenu);
    return true;
  }
  if (lowerFunction == "skipanim")
  {
    chunk.pushCode(CodeSkipAnim);
    return true;
  }
  if (lowerFunction == "stopcombat")
  {
    chunk.pushCode(CodeStopCombat);
    return true;
  }
  if (lowerFunction == "turnmoonred")
  {
    chunk.pushCode(CodeTurnMoonRed);
    return true;
  }
  if (lowerFunction == "turnmoonwhite")
  {
    chunk.pushCode(CodeTurnMoonWhite);
    return true;
  }
  if (lowerFunction == "undowerewolf")
  {
    chunk.pushCode(CodeUndoWerewolf);
    return true;
  }
  if (lowerFunction == "unlock")
  {
    chunk.pushCode(CodeUnlock);
    return true;
  }
  if (lowerFunction == "wakeuppc")
  {
    chunk.pushCode(CodeWakeUpPC);
    return true;
  }
  if (lowerFunction == "xbox")
  {
    chunk.pushCode(CodeXBox);
    return true;
  }
  return false;
}

bool ScriptFunctions_ModStatFunctions(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  uint16_t functionCode = 0;
  if (lowerFunction == "modacrobatics")
  {
    functionCode = CodeModAcrobatics;
  }
  else if (lowerFunction == "modagility")
  {
    functionCode = CodeModAgility;
  }
  else if (lowerFunction == "modalarm")
  {
    functionCode = CodeModAlarm;
  }
  else if (lowerFunction == "modalchemy")
  {
    functionCode = CodeModAlchemy;
  }
  else if (lowerFunction == "modalteration")
  {
    functionCode = CodeModAlteration;
  }
  else if (lowerFunction == "modarmorbonus")
  {
    functionCode = CodeModArmorBonus;
  }
  else if (lowerFunction == "modarmorer")
  {
    functionCode = CodeModArmorer;
  }
  else if (lowerFunction == "modathletics")
  {
    functionCode = CodeModAthletics;
  }
  else if (lowerFunction == "modttackbonus")
  {
    functionCode = CodeModAttackBonus;
  }
  else if (lowerFunction == "modaxe")
  {
    functionCode = CodeModAxe;
  }
  else if (lowerFunction == "modblindness")
  {
    functionCode = CodeModBlindness;
  }
  else if (lowerFunction == "modblock")
  {
    functionCode = CodeModBlock;
  }
  else if (lowerFunction == "modbluntweapon")
  {
    functionCode = CodeModBluntWeapon;
  }
  else if (lowerFunction == "modcastpenalty")
  {
    functionCode = CodeModCastPenalty;
  }
  else if (lowerFunction == "modchameleon")
  {
    functionCode = CodeModChameleon;
  }
  else if (lowerFunction == "modconjuration")
  {
    functionCode = CodeModConjuration;
  }
  else if (lowerFunction == "modcurrentfatigue")
  {
    functionCode = CodeModCurrentFatigue;
  }
  else if (lowerFunction == "modcurrenthealth")
  {
    functionCode = CodeModCurrentHealth;
  }
  else if (lowerFunction == "modcurrentmagicka")
  {
    functionCode = CodeModCurrentMagicka;
  }
  else if (lowerFunction == "moddefendbonus")
  {
    functionCode = CodeModDefendBonus;
  }
  else if (lowerFunction == "moddestruction")
  {
    functionCode = CodeModDestruction;
  }
  else if (lowerFunction == "moddisposition")
  {
    functionCode = CodeModDisposition;
  }
  else if (lowerFunction == "modenchant")
  {
    functionCode = CodeModEnchant;
  }
  else if (lowerFunction == "modendurance")
  {
    functionCode = CodeModEndurance;
  }
  else if (lowerFunction == "modfatigue")
  {
    functionCode = CodeModFatigue;
  }
  else if (lowerFunction == "modfight")
  {
    functionCode = CodeModFight;
  }
  else if (lowerFunction == "modflee")
  {
    functionCode = CodeModFlee;
  }
  else if (lowerFunction == "modflying")
  {
    functionCode = CodeModFlying;
  }
  else if (lowerFunction == "modhandtohand")
  {
    functionCode = CodeModHandToHand;
  }
  else if (lowerFunction == "modhealth")
  {
    functionCode = CodeModHealth;
  }
  else if (lowerFunction == "modheavyarmor")
  {
    functionCode = CodeModHeavyArmor;
  }
  else if (lowerFunction == "modhello")
  {
    functionCode = CodeModHello;
  }
  else if (lowerFunction == "modillusion")
  {
    functionCode = CodeModIllusion;
  }
  else if (lowerFunction == "modintelligence")
  {
    functionCode = CodeModIntelligence;
  }
  else if (lowerFunction == "modinvisible")
  {
    functionCode = CodeModInvisible;
  }
  else if (lowerFunction == "modlightarmor")
  {
    functionCode = CodeModLightArmor;
  }
  else if (lowerFunction == "modlongblade")
  {
    functionCode = CodeModLongBlade;
  }
  else if (lowerFunction == "modluck")
  {
    functionCode = CodeModLuck;
  }
  else if (lowerFunction == "modmagicka")
  {
    functionCode = CodeModMagicka;
  }
  else if (lowerFunction == "modmarksman")
  {
    functionCode = CodeModMarksman;
  }
  else if (lowerFunction == "modmediumarmor")
  {
    functionCode = CodeModMediumArmor;
  }
  else if (lowerFunction == "modmercantile")
  {
    functionCode = CodeModMercantile;
  }
  else if (lowerFunction == "modmysticism")
  {
    functionCode = CodeModMysticism;
  }
  else if (lowerFunction == "modparalysis")
  {
    functionCode = CodeModParalysis;
  }
  else if (lowerFunction == "modpccrimelevel")
  {
    functionCode = CodeModPCCrimeLevel;
  }
  else if (lowerFunction == "modpcvisionbonus")
  {
    functionCode = CodeModPCVisionBonus;
  }
  else if (lowerFunction == "modpersonality")
  {
    functionCode = CodeModPersonality;
  }
  else if (lowerFunction == "modreputation")
  {
    functionCode = CodeModReputation;
  }
  else if (lowerFunction == "modresistblight")
  {
    functionCode = CodeModResistBlight;
  }
  else if (lowerFunction == "modresistcorprus")
  {
    functionCode = CodeModResistCorprus;
  }
  else if (lowerFunction == "modresistdisease")
  {
    functionCode = CodeModResistDisease;
  }
  else if (lowerFunction == "modresistfire")
  {
    functionCode = CodeModResistFire;
  }
  else if (lowerFunction == "modresistfrost")
  {
    functionCode = CodeModResistFrost;
  }
  else if (lowerFunction == "modresistmagicka")
  {
    functionCode = CodeModResistMagicka;
  }
  else if (lowerFunction == "modresistnormalweapons")
  {
    functionCode = CodeModResistNormalWeapons;
  }
  else if (lowerFunction == "modresistparalysis")
  {
    functionCode = CodeModResistParalysis;
  }
  else if (lowerFunction == "modresistpoison")
  {
    functionCode = CodeModResistPoison;
  }
  else if (lowerFunction == "modresistshock")
  {
    functionCode = CodeModResistShock;
  }
  else if (lowerFunction == "modrestoration")
  {
    functionCode = CodeModRestoration;
  }
  else if (lowerFunction == "modscale")
  {
    functionCode = CodeModScale;
  }
  else if (lowerFunction == "modsecurity")
  {
    functionCode = CodeModSecurity;
  }
  else if (lowerFunction == "modshortblade")
  {
    functionCode = CodeModShortBlade;
  }
  else if (lowerFunction == "modsilence")
  {
    functionCode = CodeModSilence;
  }
  else if (lowerFunction == "modsneak")
  {
    functionCode = CodeModSneak;
  }
  else if (lowerFunction == "modspear")
  {
    functionCode = CodeModSpear;
  }
  else if (lowerFunction == "modspeechcraft")
  {
    functionCode = CodeModSpeechcraft;
  }
  else if (lowerFunction == "modspeed")
  {
    functionCode = CodeModSpeed;
  }
  else if (lowerFunction == "modstrength")
  {
    functionCode = CodeModStrength;
  }
  else if (lowerFunction == "modsuperjump")
  {
    functionCode = CodeModSuperJump;
  }
  else if (lowerFunction == "modswimspeed")
  {
    functionCode = CodeModSwimSpeed;
  }
  else if (lowerFunction == "modunarmored")
  {
    functionCode = CodeModUnarmored;
  }
  else if (lowerFunction == "modwaterbreathing")
  {
    functionCode = CodeModWaterBreathing;
  }
  else if (lowerFunction == "modwaterlevel")
  {
    functionCode = CodeModWaterLevel;
  }
  else if (lowerFunction == "modwaterwalking")
  {
    functionCode = CodeModWaterWalking;
  }
  else if (lowerFunction == "modwillpower")
  {
    functionCode = CodeModWillpower;
  }
  //Found something? If not, return false.
  if (functionCode==0) return false;

  if (params.size()<2)
  {
    std::cout << "ScriptCompiler: Error: Function \""<<params[0]<<"\" needs one parameter!\n";
    return false;
  }
  chunk.pushCode(functionCode);
  //parameter is float value or float var
  float mod_value;
  if (stringToFloat(params[1], mod_value))
  {
    chunk.pushFloat(mod_value);
  }
  else
  {
    //could still be a local var here
    SC_VarRef localRef = chunk.getVariableTypeWithIndex(params[1]);
    //if type is float, push
    switch(localRef.Type)
    {
      case vtFloat:
      case vtLong:
      case vtShort:
           //push reference and add two NUL bytes to fill up (needs four bytes)
           chunk.pushNonGlobalRefWithTwoZeroFillers(localRef);
           break;
      default:
           //encountered unknown variable name - error!
           std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a proper "
                     << "float value and does not name a local var either.\n";
           return false;
           break;//pro forma
    }//swi
  }//else
  return true;
}//function ScriptFunctions_ModStatFunctions


bool ScriptFunctions_SetStatFunctions(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  uint16_t functionCode = 0;
  if (lowerFunction == "setacrobatics")
  {
    functionCode = CodeSetAcrobatics;
  }
  else if (lowerFunction == "setagility")
  {
    functionCode = CodeSetAgility;
  }
  else if (lowerFunction == "setalarm")
  {
    functionCode = CodeSetAlarm;
  }
  else if (lowerFunction == "setalchemy")
  {
    functionCode = CodeSetAlchemy;
  }
  else if (lowerFunction == "setalteration")
  {
    functionCode = CodeSetAlteration;
  }
  else if (lowerFunction == "setarmorbonus")
  {
    functionCode = CodeSetArmorBonus;
  }
  else if (lowerFunction == "setarmorer")
  {
    functionCode = CodeSetArmorer;
  }
  else if (lowerFunction == "setathletics")
  {
    functionCode = CodeSetAthletics;
  }
  else if (lowerFunction == "setattackbonus")
  {
    functionCode = CodeSetAttackBonus;
  }
  else if (lowerFunction == "setaxe")
  {
    functionCode = CodeSetAxe;
  }
  else if (lowerFunction == "setblindness")
  {
    functionCode = CodeSetBlindness;
  }
  else if (lowerFunction == "setblock")
  {
    functionCode = CodeSetBlock;
  }
  else if (lowerFunction == "setbluntweapon")
  {
    functionCode = CodeSetBluntWeapon;
  }
  else if (lowerFunction == "setcastpenalty")
  {
    functionCode = CodeSetCastPenalty;
  }
  else if (lowerFunction == "setchameleon")
  {
    functionCode = CodeSetChameleon;
  }
  else if (lowerFunction == "setconjuration")
  {
    functionCode = CodeSetConjuration;
  }
  else if (lowerFunction == "setdefendbonus")
  {
    functionCode = CodeSetDefendBonus;
  }
  else if (lowerFunction == "setdestruction")
  {
    functionCode = CodeSetDestruction;
  }
  else if (lowerFunction == "setdisposition")
  {
    functionCode = CodeSetDisposition;
  }
  else if (lowerFunction == "setenchant")
  {
    functionCode = CodeSetEnchant;
  }
  else if (lowerFunction == "setendurance")
  {
    functionCode = CodeSetEndurance;
  }
  else if (lowerFunction == "setfatigue")
  {
    functionCode = CodeSetFatigue;
  }
  else if (lowerFunction == "setfight")
  {
    functionCode = CodeSetFight;
  }
  else if (lowerFunction == "setflee")
  {
    functionCode = CodeSetFlee;
  }
  else if (lowerFunction == "setflying")
  {
    functionCode = CodeSetFlying;
  }
  else if (lowerFunction == "sethandtohand")
  {
    functionCode = CodeSetHandToHand;
  }
  else if (lowerFunction == "sethealth")
  {
    functionCode = CodeSetHealth;
  }
  else if (lowerFunction == "setheavyarmor")
  {
    functionCode = CodeSetHeavyArmor;
  }
  else if (lowerFunction == "sethello")
  {
    functionCode = CodeSetHello;
  }
  else if (lowerFunction == "setillusion")
  {
    functionCode = CodeSetIllusion;
  }
  else if (lowerFunction == "setintelligence")
  {
    functionCode = CodeSetIntelligence;
  }
  else if (lowerFunction == "setinvisible")
  {
    functionCode = CodeSetInvisible;
  }
  else if (lowerFunction == "setlightarmor")
  {
    functionCode = CodeSetLightArmor;
  }
  else if (lowerFunction == "setlongblade")
  {
    functionCode = CodeSetLongBlade;
  }
  else if (lowerFunction == "setluck")
  {
    functionCode = CodeSetLuck;
  }
  else if (lowerFunction == "setmagicka")
  {
    functionCode = CodeSetMagicka;
  }
  else if (lowerFunction == "setmarksman")
  {
    functionCode = CodeSetMarksman;
  }
  else if (lowerFunction == "setmediumarmor")
  {
    functionCode = CodeSetMediumArmor;
  }
  else if (lowerFunction == "setmercantile")
  {
    functionCode = CodeSetMercantile;
  }
  else if (lowerFunction == "setmysticism")
  {
    functionCode = CodeSetMysticism;
  }
  else if (lowerFunction == "setparalysis")
  {
    functionCode = CodeSetParalysis;
  }
  else if (lowerFunction == "setpccrimelevel")
  {
    functionCode = CodeSetPCCrimeLevel;
  }
  else if (lowerFunction == "setpcvisionbonus")
  {
    functionCode = CodeSetPCVisionBonus;
  }
  else if (lowerFunction == "setpersonality")
  {
    functionCode = CodeSetPersonality;
  }
  else if (lowerFunction == "setreputation")
  {
    functionCode = CodeSetReputation;
  }
  else if (lowerFunction == "setresistblight")
  {
    functionCode = CodeSetResistBlight;
  }
  else if (lowerFunction == "setresistcorpurs")
  {
    functionCode = CodeSetResistCorprus;
  }
  else if (lowerFunction == "setresistdisease")
  {
    functionCode = CodeSetResistDisease;
  }
  else if (lowerFunction == "setresistfire")
  {
    functionCode = CodeSetResistFire;
  }
  else if (lowerFunction == "setresistfrost")
  {
    functionCode = CodeSetResistFrost;
  }
  else if (lowerFunction == "setresistmagicka")
  {
    functionCode = CodeSetResistMagicka;
  }
  else if (lowerFunction == "setresistnormalweapons")
  {
    functionCode = CodeSetResistNormalWeapons;
  }
  else if (lowerFunction == "setresistparalysis")
  {
    functionCode = CodeSetResistParalysis;
  }
  else if (lowerFunction == "setresistpoison")
  {
    functionCode = CodeSetResistPoison;
  }
  else if (lowerFunction == "setresistshock")
  {
    functionCode = CodeSetResistShock;
  }
  else if (lowerFunction == "setrestoration")
  {
    functionCode = CodeSetRestoration;
  }
  else if (lowerFunction == "setscale")
  {
    functionCode = CodeSetScale;
  }
  else if (lowerFunction == "setsecurity")
  {
    functionCode = CodeSetSecurity;
  }
  else if (lowerFunction == "setshortblade")
  {
    functionCode = CodeSetShortBlade;
  }
  else if (lowerFunction == "setsilence")
  {
    functionCode = CodeSetSilence;
  }
  else if (lowerFunction == "setneak")
  {
    functionCode = CodeSetSneak;
  }
  else if (lowerFunction == "setspear")
  {
    functionCode = CodeSetSpear;
  }
  else if (lowerFunction == "setspeechcraft")
  {
    functionCode = CodeSetSpeechcraft;
  }
  else if (lowerFunction == "setspeed")
  {
    functionCode = CodeSetSpeed;
  }
  else if (lowerFunction == "setstrength")
  {
    functionCode = CodeSetStrength;
  }
  else if (lowerFunction == "setsuperjump")
  {
    functionCode = CodeSetSuperJump;
  }
  else if (lowerFunction == "setswimspeed")
  {
    functionCode = CodeSetSwimSpeed;
  }
  else if (lowerFunction == "setumarmored")
  {
    functionCode = CodeSetUnarmored;
  }
  else if (lowerFunction == "setwaterbreathing")
  {
    functionCode = CodeSetWaterBreathing;
  }
  else if (lowerFunction == "setwaterlevel")
  {
    functionCode = CodeSetWaterLevel;
  }
  else if (lowerFunction == "setwaterwalking")
  {
    functionCode = CodeSetWaterWalking;
  }
  else if (lowerFunction == "setwillpower")
  {
    functionCode = CodeSetWillpower;
  }
  //Found something? If not, return false.
  if (functionCode==0) return false;

  if (params.size()<2)
  {
    std::cout << "ScriptCompiler: Error: Function \""<<params[0]<<"\" needs one parameter!\n";
    return false;
  }
  //push function
  chunk.pushCode(functionCode);
  //parameter is float value or float var
  float set_value;
  if (stringToFloat(params[1], set_value))
  {
    chunk.pushFloat(set_value);
  }
  else
  {
    //could still be a local var here
    SC_VarRef localRef = chunk.getVariableTypeWithIndex(params[1]);
    //if type is float, push
    switch(localRef.Type)
    {
      case vtFloat:
      case vtLong:
      case vtShort:
           //push reference and add two NUL bytes to fill up (needs four bytes)
           chunk.pushNonGlobalRefWithTwoZeroFillers(localRef);
           break;
      default:
           //encountered unknown variable name - error!
           std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a proper "
                     << "float value and does not name a local var either.\n";
           return false;
           break;//pro forma
    }//swi
  }//else
  return true;
}//ScriptFunctions_SetStatFunctions

bool ScriptFunctions_MessageBox(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "messagebox")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: MessageBox needs at least one parameter!\n";
      return false;
    }
    if (params.size()>257)
    {
      std::cout << "ScriptCompiler: Error: MessageBox shouldn't have more than 256 parameters!\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeMessageBox);
    //first parameter is a string
    //second to n-th parameters are either a local var or another string
    //push string's length as short (not byte)
    chunk.pushShort(params[1].length());
    //push parameter string
    chunk.pushString(params[1]);
    //now check if params are local vars
    uint8_t arg_count = 0;
    uint8_t button_count = 0;
    std::vector<SC_VarRef> refArray(params.size()-2, SC_VarRef());

    for (uint16_t i=2; i<params.size(); ++i)
    {
      //check for vars
      refArray[i-2] = chunk.getVariableTypeWithIndex(params[i]);
      if (refArray[i-2].Type!=vtGlobal)
      {
        //it's a local var
        ++arg_count;
      }
      else if (Globals::get().hasRecord(params[i]))
      {
        //It's a global var, that counts as argument.
        ++arg_count;
      }
      else
      {
        //It's a string.
        ++button_count;
        //Since all of the other params are now strings, too, we can exit the
        // loop here.
        i = params.size();//just to be sure
        break;
      }
    }//for
    //adjust button count
    button_count = params.size()-2-arg_count;
    //push number of arguments
    chunk.data.push_back(arg_count);
    //push arguments, if present
    if (arg_count > 0)
    {
      for (unsigned int i = 0; i < arg_count; ++i)
      {
        if (refArray[i].Type != vtGlobal)
        {
          // push local ref
          chunk.pushNonGlobalRef(refArray[i]);
        }
        else
        {
          //push global
          const std::string& globName = Globals::get().getRecord(params[i+2]).recordID;
          //push G for global
          chunk.data.push_back('G');
          //push length (including null-terminating byte)
          chunk.data.push_back(globName.length()+1);
          //push global name itself
          chunk.pushString(globName);
          //push terminating NUL character
          chunk.data.push_back(0);
        }//else
      }//for
    }//arguments
    //push number of buttons
    chunk.data.push_back(button_count);
    //push buttons, if present
    if (button_count > 0)
    {
      for (unsigned int i = 2 + arg_count; i < params.size(); ++i)
      {
        //push length of string + one byte for NUL
        chunk.data.push_back(params[i].length() + 1);
        //push the string
        chunk.pushString(params[i]);
        //push NUL byte
        chunk.data.push_back(0);
      }//for
    }//buttons
    return true;
  }//if MessageBox
  //no match here
  return false;
}//function for MessageBox

bool ScriptFunctions_OneParameter(const std::vector<std::string>& params, CompiledChunk& chunk, const bool isCompare)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "addspell")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: AddSpell needs one parameter!\n";
      return false;
    }
    //parameter is ID of spell
    chunk.pushCode(CodeAddSpell);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }
  if (lowerFunction == "addtopic")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: AddTopic needs one parameter!\n";
      return false;
    }
    //parameter is ID of topic
    chunk.pushCode(CodeAddTopic);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//AddTopic
  if (lowerFunction == "aiactivate")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: AIActivate needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeAIActivate);
    //parameter is ID of target
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push target ID
    chunk.pushString(params[1]);
    //after ID, a NUL byte is pushed in that one-param version
    chunk.data.push_back(0);
    return true;
  }
  if (lowerFunction == "dropsoulgem")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: DropSoulGem needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeDropSoulGem);
    //parameter is ID of creature
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push creature ID
    chunk.pushString(params[1]);
    return true;
  }
  if (lowerFunction == "equip")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: Equip needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeEquip);
    //parameter is ID of item
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push item ID
    chunk.pushString(params[1]);
    return true;
  }
  if (lowerFunction == "explodespell")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: ExplodeSpell needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeExplodeSpell);
    //parameter is ID of spell
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push spell ID
    chunk.pushString(params[1]);
    return true;
  }
  if (lowerFunction == "fadein")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: FadeIn needs one parameter!\n";
      return false;
    }
    //parameter is time for fading (float)
    //check float
    float fade_time;
    if (!stringToFloat(params[1], fade_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeFadeIn);
    //push float
    chunk.pushFloat(fade_time);
    return true;
  }
  if (lowerFunction == "fadeout")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: FadeOut needs one parameter!\n";
      return false;
    }
    //parameter is time for fading (float)
    //check float
    float fade_time;
    if (!stringToFloat(params[1], fade_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }
    //push function name
    chunk.pushCode(CodeFadeOut);
    //push float
    chunk.pushFloat(fade_time);
    return true;
  }
  if (lowerFunction == "getangle")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetAngle needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetAngle);
    //parameter is axis, given as upper case character
    //push axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to GetAngle.\n";
    }
    chunk.data.push_back(Axis);
    return true;
  }//if
  if (lowerFunction == "getarmortype")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetArmorType needs one parameter!\n";
      return false;
    }
    //parameter is slot index
    int16_t slot;
    if (!stringToShort(params[1], slot))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "short value.\n";
      return false;
    }
    //function code
    chunk.pushCode(CodeGetArmorType);
    //push slot index
    chunk.pushShort(slot);
    return true;
  }//if
  if (lowerFunction == "getdeadcount")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetDeathCount needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetDeathCount);
    //parameter is creature/NPC ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID itself
    chunk.pushString(params[1]);
    return true;
  }//if GetDeadCount
  if (lowerFunction == "getdetected")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetDetected needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetDetected);
    //parameter is NPC ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID itself
    chunk.pushString(params[1]);
    return true;
  }//if GetDetected
  if (lowerFunction == "getdistance")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetDistance needs one parameter!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeGetDistance);
    //push extra data for compare
    if (isCompare)
    {
      /*I don't know why this is there in the compiled data, but it has to be
        there to produce identical compiled data.*/
      chunk.data.push_back(0x20);
      chunk.data.push_back(0x72);
    }
    //parameter is object ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if GetDistance
  if (lowerFunction == "geteffect")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetEffect needs one parameter!\n";
      return false;
    }
    //parameter is mgef ID as string, but the chunk needs it as short
    //That's why we do some ugly brute force here, but an inverse function would
    // be the weapon of choice.
    int16_t effectID = -1;
    const std::string lowerEffect = lowerCase(params[1]);
    for (int i = 0; i <= 136; ++i)
    {
      const auto idx = static_cast<EffectIndex>(i);
      if (lowerEffect == lowerCase(MagicEffects::getSettingNameForEffect(idx)))
      {
        //found it!
        effectID = i;
        break;
      }
    }//for
    if (effectID==-1)
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" does not name a "
                << "magic effect for GetEffect.\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeGetEffect);
    //push effect ID (as byte, not short)
    chunk.data.push_back(effectID);
    return true;
  }//if GetEffect
  if (lowerFunction == "getitemcount")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetItemCount needs one parameter!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeGetItemCount);
    //push extra data for compare
    if (isCompare)
    {
      /*I don't know why this is there in the compiled data, but it has to be
        there to produce identical compiled data.*/
      chunk.data.push_back(0x20);
      chunk.data.push_back(0x6F);
    }
    //parameter is item ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "getjournalindex")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetJounalIndex needs one parameter!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeGetJournalIndex);
    //push extra data for compare
    if (isCompare)
    {
      /*I don't know why this is there in the compiled data, but it has to be
        there to produce identical compiled data.*/
      chunk.data.push_back(0x20);
      chunk.data.push_back(0x64);
    }
    //parameter is quest ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push quest ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if ((lowerFunction == "getlineofsight") || (lowerFunction == "getlos"))
  {
    if (params.size() < 2)
    {
      std::cout << "ScriptCompiler: Error: GetLineOfSight needs one parameter!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeGetLineOfSight);
    //push extra compare data
    if (isCompare)
    {
      /*I don't know why this is there in the compiled data, but it has to be
        there to produce identical compiled data.*/
      chunk.data.push_back(0x20);
      chunk.data.push_back(0x72);
    }//if
    //parameter is object ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "getpccell")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetPCCell needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetPCCell);
    //parameter is cell name or a part thereof
    //push name's length
    chunk.data.push_back(params[1].length());
    //push name
    chunk.pushString(params[1]);
    return true;
  }//if GetPCCell
  if (lowerFunction == "getpcfacrep")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetPCFacRep needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetPCFacRep);
    //parameter is faction ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push faction ID
    chunk.pushString(params[1]);
    return true;
  }//if GetPCFacRep
  if (lowerFunction == "getpcrank")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetPCRank needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetPCRank);
    //parameter is faction ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push faction ID
    chunk.pushString(params[1]);
    return true;
  }//if GetPCRank
  if (lowerFunction == "getpos")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetPos needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetPos);
    //parameter is axis name as upper case letter
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to GetPos.\n";
    }
    //push axis
    chunk.data.push_back(Axis);
    return true;
  }//if GetPos
  if (lowerFunction == "getrace")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetRace needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetRace);
    //parameter is race name
    //push name's length
    chunk.data.push_back(params[1].length());
    //push name
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "getsoundplaying")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetSoundPlaying needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetSoundPlaying);
    //parameter is sound name
    //push name's length
    chunk.data.push_back(params[1].length());
    //push sound name
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "getspell")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetSpell needs one parameter!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeGetSpell);
    //push extra compare data
    if (isCompare)
    {
      /*I don't know why this is there in the compiled data, but it has to be
        there to produce identical compiled data.*/
      chunk.data.push_back(0x20);
      chunk.data.push_back(0x6F);
    }//if
    //parameter is spell ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push spell ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "getspelleffects")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetSpellEffects needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetSpellEffects);
    //parameter is spell ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push spell ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "getsquareroot")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetSquareRoot needs one parameter!\n";
      return false;
    }
    //parameter is floating point value or float var
    //check for float or local var
    float float_value;
    SC_VarRef localRef = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[1], float_value))
    {
      //Go for the local var here.
      localRef = chunk.getVariableTypeWithIndex(params[1]);
      if (localRef.Type==vtGlobal)
      {
        //No match found, we have an invalid expression here.
        std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no floating"
                  << " point value and no local var either!\n";
        return false;
      }
    }
    //push function code
    chunk.pushCode(CodeGetSquareRoot);
    if (localRef.Type==vtGlobal)
    {
      //push float
      chunk.pushFloat(float_value);
    }
    else
    {
      //push local var
      chunk.pushNonGlobalRefWithTwoZeroFillers(localRef);
    }
    return true;
  }//if GetSquareRoot
  if (lowerFunction == "getstartingangle")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetStartingAngle needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetStartingAngle);
    //parameter is axis, given as upper case character
    //push axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to GetStartingAngle.\n";
    }
    chunk.data.push_back(Axis);
    return true;
  }//if
  if (lowerFunction == "getstartingpos")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetStartingPos needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetStartingPos);
    //parameter is axis, given as upper case character
    //push axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to GetStartingPos.\n";
    }
    chunk.data.push_back(Axis);
    return true;
  }//if
  if (lowerFunction == "gettarget")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetTarget needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetTarget);
    //parameter is npc/creature ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "hasitemequipped")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: HasItemEquipped needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeHasItemEquipped);
    //parameter is item ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "hassoulgem")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: HasSoulgem needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeHasSoulgem);
    //parameter is creature ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "hitattemptonme")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: HitAttemptOnMe needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeHitAttemptOnMe);
    //parameter is weapon ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "hitonme")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: HitOnMe needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeHitOnMe);
    //parameter is weapon ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "hurtcollidingactor")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: HurtCollidingActor needs one parameter!\n";
      return false;
    }
    //parameter is damage per second as float
    float f_value;
    if (!stringToFloat(params[1], f_value))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a proper "
                << "floating point value.\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeHurtCollidingActor);
    //push float
    chunk.pushFloat(f_value);
    return true;
  }//if
  if (lowerFunction == "hurtstandingactor")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: HurtStandingActor needs one parameter!\n";
      return false;
    }
    //parameter is damage per second as float
    float f_value;
    if (!stringToFloat(params[1], f_value))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a proper "
                << "floating point value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeHurtStandingActor);
    //push float
    chunk.pushFloat(f_value);
    return true;
  }//if
  if (lowerFunction == "lock")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: Lock needs one parameter!\n";
      return false;
    }
    //parameter is lock level
    int16_t lock_level;
    if (!stringToShort(params[1], lock_level))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a proper "
                << "short value.\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeLock);
    //push lock level
    chunk.pushShort(lock_level);
    return true;
  }//if Lock
  if (lowerFunction == "menutest")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: MenuTest needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeMenuTest);
    //parameter is usually a short value, but this function takes it as string
    //push string's length as short (not byte)
    chunk.pushShort(params[1].length());
    //push parameter string
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "messagebox")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: MessageBox needs (at least) one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeMessageBox);
    //parameter is a string (in one param version)
    //push string's length as short (not byte)
    chunk.pushShort(params[1].length());
    //push parameter string
    chunk.pushString(params[1]);
    //push number of arguments (zero in this case)
    chunk.data.push_back(0);
    //push number of buttons (zero in this case)
    chunk.data.push_back(0);
    return true;
  }//if
  if (lowerFunction.substr(0,3)=="mod")
  {
    //could be a function that modifies stats like ModAcrobatics, so check
    if (ScriptFunctions_ModStatFunctions(params, chunk))
    {
      return true;
    }
    if (lowerFunction == "modpcfacrep")
    {
      if (params.size()<2)
      {
        std::cout << "ScriptCompiler: Error: ModPCFacRep needs one parameter!\n";
        return false;
      }
      //get/check floating point valued parameter
      float mod_value;
      if (!stringToFloat(params[1], mod_value))
      {
        std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a float"
                  << " value.\n";
        return false;
      }
      //push function
      chunk.pushCode(CodeModPCFacRep);
      //first parameter is float value, second is omitted
      //push float
      chunk.pushFloat(mod_value);
      //instead of pushing string (optional, we push a zero byte to indicate no string)
      chunk.data.push_back(0);
      return true;
    }//if ModPCFacRep
    //Since all ModSomething functions with one param should be handled in the
    //section above and nothing was found, but function name begins with "mod...",
    //we can return false here. Checking the other founctions would not bring a
    // result (instead of taking more time).
    return false;
  }
  if (lowerFunction == "pcclearexpelled")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PCClearExpelled needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePCClearExpelled);
    //parameter is faction ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PCClearExpelled
  if (lowerFunction == "pcexpell")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PCExpell needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePCExpell);
    //parameter is faction ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PCExpell
  if (lowerFunction == "pcexpelled")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PCExpelled needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePCExpelled);
    //parameter is faction ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PCExpelled
  if (lowerFunction == "pcjoinfaction")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PCJoinFaction needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePCJoinFaction);
    //parameter is faction ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PCJoinFaction
  if (lowerFunction == "pclowerrank")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PCLowerRank needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePCLowerRank);
    //parameter is faction ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "pcraiserank")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PCRaiseRank needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePCRaiseRank);
    //parameter is faction ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PCRaiseRank
  if (lowerFunction == "playgroup")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PlayGroup expects one parameter.\n";
      return false;
    }
    //first parameter is group name (will be converted into short later)
    //second parameter is a byte flag (but omitted here, default is zero)
    // -- check for group name
    int16_t groupIndex = 0;
    if (!getAnimationGroupIndex(params[1], groupIndex))
    {
      std::cout << "ScriptCompiler: Error: Couldn't find animation group index "
                << "for group name \""<<params[1]<<"\".\n";
      return false;
    }
    //push function
    chunk.pushCode(CodePlayGroup);
    //push group index
    chunk.pushShort(groupIndex);
    //push flag (byte, not short) - use zero as default
    chunk.data.push_back(0);
    return true;
  }//if PlayGroup
  if (lowerFunction == "playloopsound3d")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PlayLoopSound3D needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePlayLoopSound3D);
    //parameter is sound ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PlayLoopSound3D
  if (lowerFunction == "playsound")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PlaySound needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePlaySound);
    //parameter is sound ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PlaySound
  if (lowerFunction == "playsound3d")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: PlaySound3D needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodePlaySound3D);
    //parameter is sound ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if PlaySound3D
  if (lowerFunction == "random")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: Random needs one parameter!\n";
      return false;
    }
    //parameter is short value
    int16_t limit;
    if (!stringToShort(params[1], limit))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a proper "
                << "short value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeRandom);
    //push limit
    chunk.pushShort(limit);
    return true;
  }//if Random
  if (lowerFunction == "removeeffects")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: RemoveEffects needs one parameter!\n";
      return false;
    }
    //parameter is short value (effect index)
    int16_t effect_index;
    if (!stringToShort(params[1], effect_index))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a proper "
                << "short value.\n";
      return false;
    }
    /* Valid effect indices range from zero to 136. Indices outside of that
       range will be accepted by the Construction Set, too, but trying to move
       an effect that does not exist is kind of useless. We put a warning here,
       just in case.
    */
    if ((effect_index < 0) || (effect_index > 136))
    {
      std::cout << "ScriptCompiler: Warning: given index for RemoveEffects is "
                << effect_index <<", but valid range is [0;136].\n";
    }
    //push function
    chunk.pushCode(CodeRemoveEffects);
    //push limit
    chunk.pushShort(effect_index);
    return true;
  }//if RemoveEffects
  if (lowerFunction == "removesoulgem")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: RemoveSoulgem needs one parameter!\n";
      return false;
    }
    //parameter is creature ID
    //push function
    chunk.pushCode(CodeRemoveSoulGem);
    //push ID's length
    chunk.data.push_back(params[1].length());
    chunk.pushString(params[1]);
    return true;
  }//if RemoveSoulGem
  if (lowerFunction == "removespell")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: RemoveSpell needs one parameter!\n";
      return false;
    }
    //parameter is spell ID
    //push function
    chunk.pushCode(CodeRemoveSpell);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if RemoveSpell
  if (lowerFunction == "removespelleffects")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: RemoveSpellEffects needs one parameter!\n";
      return false;
    }
    //parameter is spell ID
    //push function
    chunk.pushCode(CodeRemoveSpellEffects);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if RemoveSpellEffects
  if (lowerFunction == "repairedonme")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: RepairedOnMe needs one parameter!\n";
      return false;
    }
    //parameter is repair item ID
    //push function
    chunk.pushCode(CodeRepairedOnMe);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if RepairedOnMe
  if (lowerFunction == "scriptrunning")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: ScriptRunning needs one parameter!\n";
      return false;
    }
    //parameter is script ID
    //push function
    chunk.pushCode(CodeScriptRunning);
    //push script ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if ScriptRunning
  if (lowerFunction.substr(0,3)=="set")
  {
    //could be a function that sets stats like SetAcrobatics, so check
    if (ScriptFunctions_SetStatFunctions(params, chunk))
    {
      return true;
    }
    //check other set functions
    if (lowerFunction =="setdelete")
    {
      if (params.size()<2)
      {
        std::cout << "ScriptCompiler: Error: SetDelete needs one parameter!\n";
        return false;
      }
      //SetDelete was introduced in Tribunal, it's not available in MW-only.
      //First parameter is flag, which can be either 0 or 1. Other values are
      // not allowed.
      int16_t delete_flag;
      if (!stringToShort(params[1], delete_flag))
      {
        std::cout << "ScriptCompiler: Error: SetDelete requires a short value, "
                  << "but \"" << params[1] << "\" is no short.\n";
        return false;
      }//if
      if ((delete_flag != 0) && (delete_flag != 1))
      {
        std::cout << "ScriptCompiler: Error: SetDelete's flag has to be 0 or 1, "
                  << "but the given value is " << delete_flag << ".\n";
        return false;
      }
      //push function
      chunk.pushCode(CodeSetDelete);
      //push flag
      chunk.pushShort(delete_flag);
      return true;
    }//if SetDelete
    //check other set functions
    if (lowerFunction =="setlevel")
    {
      if (params.size()<2)
      {
        std::cout << "ScriptCompiler: Error: SetLevel needs one parameter!\n";
        return false;
      }
      //Parameter is new level.
      int16_t new_level;
      if (!stringToShort(params[1], new_level))
      {
        std::cout << "ScriptCompiler: Error: SetLevel requires a short value, "
                  << "but \""<<params[1]<<"\" is no short.\n";
        return false;
      }//if
      //push function
      chunk.pushCode(CodeSetLevel);
      //push new level
      chunk.pushShort(new_level);
      return true;
    }//if SetLevel
    if (lowerFunction == "setpcfacrep")
    {
      if (params.size()<2)
      {
        std::cout << "ScriptCompiler: Error: SetPCFacRep needs one parameter!\n";
        return false;
      }
      //first parameter is new reputation value, second is ID of faction, but omitted here
      float new_reputation;
      if (!stringToFloat(params[1], new_reputation))
      {
        std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a float.\n";
        return false;
      }//if
      //push function
      chunk.pushCode(CodeSetPCFacRep);
      //push float
      chunk.pushFloat(new_reputation);
      //push zero
      chunk.data.push_back(0);
      return true;
    }//if SetPCFacRep

    //Since all SetSomething functions with one param should be handled in the
    //section above and nothing was found, but function name begins with "set...",
    //we can return false here. Checking the other founctions would not bring a
    // result (besides of taking more time).
    return false;
  }//if substr(0,3)=="Set..."
  if (lowerFunction =="showmap")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: ShowMap needs one parameter!\n";
      return false;
    }
    //parameter is cell name or part thereof
    //push function
    chunk.pushCode(CodeShowMap);
    //push cell name's length
    chunk.data.push_back(params[1].length());
    //push cell ID
    chunk.pushString(params[1]);
    return true;
  }//if ShowMap
  if (lowerFunction =="startscript")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: StartScript needs one parameter!\n";
      return false;
    }
    //parameter is script ID
    //push function
    chunk.pushCode(CodeStartScript);
    //push Script ID's length
    chunk.data.push_back(params[1].length());
    //push script ID
    chunk.pushString(params[1]);
    return true;
  }//if StartScript
  if (lowerFunction == "startcombat")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: StartCombat needs one parameter!\n";
      return false;
    }
    //parameter is NPC ID
    //push function
    chunk.pushCode(CodeStartCombat);
    //push NPC ID's length
    chunk.data.push_back(params[1].length());
    //push NPC ID
    chunk.pushString(params[1]);
    return true;
  }//if StartCombat

  if (lowerFunction=="stopcombat")
  {
    //StopCombat usually has no parameters, but some scripts specify an ID for
    // a target, too. However, this ID will not be compiled into the script.
    chunk.pushCode(CodeStopCombat);
    return true;
  }//if stopCombat
  //check for StopScript
  if (lowerFunction == "stopscript")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: StopScript needs one parameter!\n";
      return false;
    }
    //parameter is script ID
    //push function
    chunk.pushCode(CodeStopScript);
    //push script ID's length
    chunk.data.push_back(params[1].length());
    //push script ID
    chunk.pushString(params[1]);
    return true;
  }//if StopScript
  if (lowerFunction == "stopsound")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: StopSound needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeStopSound);
    //parameter is sound ID
    //push IDs length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }//if
  if (lowerFunction == "streammusic")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: StreamMusic needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeStreamMusic);
    //parameter is music file (MP3, Wave or Midi)
    //push file name's length
    chunk.data.push_back(params[1].length());
    //push file name
    chunk.pushString(params[1]);
    return true;
  }//if

  //no match found
  return false;
}

bool ScriptFunctions_TwoParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "additem")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: AddItem needs two parameters!\n";
      return false;
    }
    //first parameter is ID of item, second is number (short)
    //check count
    int16_t count;
    if (!stringToShort(params[2], count))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    chunk.pushCode(CodeAddItem);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    //push count
    chunk.pushShort(count);
    return true;
  }//if AddItem
  if (lowerFunction == "addsoulgem")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: AddSoulGem needs two parameters!\n";
      return false;
    }
    //first parameter is ID of creature, second is ID of gem
    chunk.pushCode(CodeAddSoulGem);
    //push creature ID's length
    chunk.data.push_back(params[1].length());
    //push creature ID
    chunk.pushString(params[1]);
    //push gem ID's length
    chunk.data.push_back(params[2].length());
    //push gem ID
    chunk.pushString(params[2]);
    return true;
  }//if AddSoulGem
  if (lowerFunction == "cast")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: Cast needs two parameters!\n";
      return false;
    }
    //first parameter is ID of spell, second is ID of target
    chunk.pushCode(CodeCast);
    //push spell ID's length
    chunk.data.push_back(params[1].length());
    //push spell ID
    chunk.pushString(params[1]);
    //push target ID's length
    chunk.data.push_back(params[2].length());
    //push target ID
    chunk.pushString(params[2]);
    return true;
  }//if Cast
  if (lowerFunction == "changeweather")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: ChangeWeather needs two parameters!\n";
      return false;
    }
    //first parameter is ID of region, second is short that indicates the new weather
    //check new weather type
    int16_t weather_type;
    if (!stringToShort(params[2], weather_type))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeChangeWeather);
    //push region ID's length
    chunk.data.push_back(params[1].length());
    //push region ID
    chunk.pushString(params[1]);
    //push new weather type
    chunk.pushShort(weather_type);
    return true;
  }//if ChangeWeather
  if (lowerFunction == "drop")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: Drop needs two parameters!\n";
      return false;
    }
    //first parameter is ID of item, second is short that indicates the count
    //check count
    int16_t count;
    if (!stringToShort(params[2], count))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    chunk.pushCode(CodeDrop);
    //push item ID's length
    chunk.data.push_back(params[1].length());
    //push item ID
    chunk.pushString(params[1]);
    //push count
    chunk.pushShort(count);
    return true;
  }//if Drop
  if (lowerFunction == "face")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: Face needs two parameters!\n";
      return false;
    }
    //first parameter is float, and so is second
    float first;
    if (!stringToFloat(params[1], first))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "float value.\n";
      return false;
    }//if
    float second;
    if (!stringToFloat(params[2], second))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeFace);
    //push both floats
    chunk.pushFloat(first);
    chunk.pushFloat(second);
    return true;
  }//if Face
  if (lowerFunction == "fadeto")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: FadeTo needs two parameters!\n";
      return false;
    }
    //first parameter is amount of fading (long?)
    //check long
    int32_t fade_amount;
    if (!stringToLong(params[1], fade_amount))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "long value.\n";
      return false;
    }//if
    //second parameter is time for fading (float)
    //push float
    float fade_time;
    if (!stringToFloat(params[2], fade_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }//if
    //push function
    chunk.pushCode(CodeFadeTo);
    //push long
    chunk.pushLong(fade_amount);
    //push float
    chunk.pushFloat(fade_time);
    return true;
  }//if FadeTo
  if (lowerFunction == "journal")
  {
    if (params.size() < 3)
    {
      std::cout << "ScriptCompiler: Error: Journal command expects two params"
                << ", not "<<params.size()<<".\n";
      return false;
    }
    //second parameter should be index
    int16_t journal_index;
    if (!stringToShort(params[2], journal_index))
    {
      std::cout << "ScriptCompiler: Error: Journal command expects short value"
                << " as second parameter, but \""<<params[2]<<"\" is not a "
                << "short value.\n";
      return false;
    }
    chunk.pushCode(CodeJournal);
    //push journal ID's length
    chunk.data.push_back(params[1].length());
    //push journal ID itself
    chunk.pushString(params[1]);
    //push index
    chunk.pushShort(journal_index);
    //fill data so it's four bytes
    chunk.data.push_back(255);
    chunk.data.push_back(255);
    return true;
  }//if Journal
  if (lowerFunction == "loopgroup")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: LoopGroup needs two parameters!\n";
      return false;
    }
    //first parameter is group name (will be translated into short)
    //second parameters is number (short)
    //third parameter will be omitted, so we push zero-byte.
    int16_t groupIndex = 0;
    if (!getAnimationGroupIndex(params[1], groupIndex))
    {
      std::cout << "ScriptCompiler: Error: Couldn't find animation group index "
                << "for group name \""<<params[1]<<"\".\n";
      return false;
    }
    int16_t number;
    if (!stringToShort(params[2], number))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value.\n";
      return false;
    }//if
    //push function
    chunk.pushCode(CodeLoopGroup);
    //push animation group index
    chunk.pushShort(groupIndex);
    //push number as byte(!)
    chunk.data.push_back(number);
    //push zero-byte to indicate no flags
    chunk.data.push_back(0);
    return true;
  }//if LoopGroup
  if (lowerFunction == "messagebox")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: MessageBox needs two parameters!\n";
      return false;
    }
    chunk.pushCode(CodeMessageBox);
    //first parameter is a string
    //second parameter is either a local var or another string
    //push string's length as short (not byte)
    chunk.pushShort(params[1].length());
    //push parameter string
    chunk.pushString(params[1]);
    //now check if second param is a local var
    uint8_t arg_count = 0;
    uint8_t button_count = 0;
    SC_VarRef localRef = chunk.getVariableTypeWithIndex(params[2]);
    if (localRef.Type!=vtGlobal)
    {
      //it's a local var
      arg_count = 1;
    }
    else if (Globals::get().hasRecord(params[2]))
    {
      //It's a global var, that counts as argument.
      arg_count = 1;
    }
    else
    {
      //It's a string.
      button_count = 1;
    }
    //push number of arguments
    chunk.data.push_back(arg_count);
    //push argument, if present
    if (arg_count==1)
    {
      if (localRef.Type!=vtGlobal)
      {
        //push local ref
        chunk.pushNonGlobalRef(localRef);
      }
      else
      {
        //push global
        const std::string& globName = Globals::get().getRecord(params[2]).recordID;
        //push G for global
        chunk.data.push_back('G');
        //push length (including null-terminating byte)
        chunk.data.push_back(globName.length()+1);
        //push global name itself
        chunk.pushString(globName);
        //push terminating NUL character
        chunk.data.push_back(0);
      }
    }//argument
    //push number of buttons
    chunk.data.push_back(button_count);
    //push button, if present
    if (button_count==1)
    {
      //push length of string + one byte for NUL
      chunk.data.push_back(params[2].length()+1);
      //push the string
      chunk.pushString(params[2]);
      //push NUL byte
      chunk.data.push_back(0);
    }
    return true;
  }//if MessageBox
  if (lowerFunction == "modpcfacrep")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: ModPCFacRep needs two parameters!\n";
      return false;
    }
    //get/check floating point valued parameter
    float mod_value;
    if (!stringToFloat(params[1], mod_value))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a float"
                << " value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeModPCFacRep);
    //first parameter is float value, second is faction ID
    //push float (1st param)
    chunk.pushFloat(mod_value);
    //ID's length
    chunk.data.push_back(params[2].length());
    //push ID
    chunk.pushString(params[2]);
    return true;
  }//if ModPCFacRep
  if (lowerFunction == "move")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: Move needs two parameters!\n";
      return false;
    }
    //first parameter is axis, second is float
    //get/check floating point valued parameter
    float move_value;
    if (!stringToFloat(params[2], move_value))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a float"
                << " value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeMove);
    //push axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter and even longer strings, as it seems. I'm not sure about the
      consequences in-game. We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to Move.\n";
    }
    chunk.data.push_back(Axis);
    //push float (2nd param)
    chunk.pushFloat(move_value);
    return true;
  }//if Move
  if (lowerFunction == "moveworld")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: MoveWorld needs two parameters!\n";
      return false;
    }
    //first parameter is axis, second is float
    //get/check floating point valued parameter
    float move_value;
    if (!stringToFloat(params[2], move_value))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a float"
                << " value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeMoveWorld);
    //push axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter and even longer strings, as it seems. I'm not sure about the
      consequences in-game. We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to MoveWorld.\n";
    }
    chunk.data.push_back(Axis);
    //push float (2nd param)
    chunk.pushFloat(move_value);
    return true;
  }//if MoveWorld
  if (lowerFunction == "playbink")
  {
    if (params.size() < 3)
    {
      std::cout << "ScriptCompiler: Error: PlayBink expects two params"
                << ", not "<<params.size()<<".\n";
      return false;
    }
    //second parameter should be flag (0 or 1, byte)
    int16_t bink_flag;
    if (!stringToShort(params[2], bink_flag))
    {
      std::cout << "ScriptCompiler: Error: PlayBink expects byte value as "
                << "second parameter, but \""<<params[2]<<"\" is not a short "
                << "value.\n";
      return false;
    }
    chunk.pushCode(CodePlayBink);
    //push bink file's length
    chunk.data.push_back(params[1].length());
    //push bink file
    chunk.pushString(params[1]);
    //push flag
    chunk.data.push_back(bink_flag);//flag uses just one byte!
    return true;
  }//if PlayBink
  if (lowerFunction == "playgroup")
  {
    if (params.size() < 3)
    {
      std::cout << "ScriptCompiler: Error: PlayGroup expects two params"
                << ", not "<<params.size()<<".\n";
      return false;
    }
    //first parameter is group name (will be converted into short later)
    //second parameter is a byte flag
    // -- check for group name
    int16_t groupIndex = 0;
    if (!getAnimationGroupIndex(params[1], groupIndex))
    {
      std::cout << "ScriptCompiler: Error: Couldn't find animation group index "
                << "for group name \""<<params[1]<<"\".\n";
      return false;
    }
    //second parameter should be flag (byte, not short!)
    int16_t groupFlag;
    if (!stringToShort(params[2], groupFlag))
    {
      std::cout << "ScriptCompiler: Error: PlayGroup expects byte value as "
                << "second parameter, but \""<<params[2]<<"\" is not a short "
                << "value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodePlayGroup);
    //push group index
    chunk.pushShort(groupIndex);
    //push flag
    chunk.data.push_back(groupFlag); //(byte, not short!)
    return true;
  }//if PlayGroup
  if (lowerFunction == "removeitem")
  {
    if (params.size() < 3)
    {
      std::cout << "ScriptCompiler: Error: RemoveItem expects two params"
                << ", not "<<params.size()<<".\n";
      return false;
    }
    //first parameter is item's ID, second is count (short)
    int16_t count;
    if (!stringToShort(params[2], count))
    {
      std::cout << "ScriptCompiler: Error: RemoveItem expects byte value as "
                << "second parameter, but \""<<params[2]<<"\" is not a short "
                << "value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeRemoveItem);
    //push item ID's length
    chunk.data.push_back(params[1].length());
    //push item ID
    chunk.pushString(params[1]);
    //push count
    chunk.pushShort(count);
    return true;
  }//if RemoveItem
  if (lowerFunction == "removesoulgem")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: RemoveSoulgem needs one parameter!\n";
      return false;
    }
    //parameter is creature ID
    // second parameter is count, according to some references, but there is no
    //   true second parameter, the function just removes one soul gem. Thus,
    //   the second "parameter" is ignored.
    //push function
    chunk.pushCode(CodeRemoveSoulGem);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push creature ID
    chunk.pushString(params[1]);
    return true;
  }//if RemoveSoulGem
  if (lowerFunction == "rotate")
  {
    if (params.size() < 3)
    {
      std::cout << "ScriptCompiler: Error: Rotate expects two params"
                << ", not "<<params.size()<<".\n";
      return false;
    }
    //first parameter is axis, second is amount (float)
    // ---- get axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid axis parameter to Rotate.\n";
    }
    // ---- second param
    float amount;
    if (!stringToFloat(params[2], amount))
    {
      std::cout << "ScriptCompiler: Error: \"" << params[2] << "\" is not a float.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeRotate);
    //push axis
    chunk.data.push_back(Axis);
    //push amount (float)
    chunk.pushFloat(amount);
    return true;
  }//if Rotate
  if (lowerFunction == "rotateworld")
  {
    if (params.size() < 3)
    {
      std::cout << "ScriptCompiler: Error: RotateWorld expects two params, not "
                <<params.size()<<".\n";
      return false;
    }
    //first parameter is axis, second is amount (float)
    // ---- get axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid axis parameter to RotateWorld.\n";
    }
    // ---- second param
    float amount;
    if (!stringToFloat(params[2], amount))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a float.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeRotateWorld);
    //push axis
    chunk.data.push_back(Axis);
    //push amount (float)
    chunk.pushFloat(amount);
    return true;
  }//if RotateWorld
  if (lowerFunction == "say")
  {
    if (params.size() < 3)
    {
      std::cout << "ScriptCompiler: Error: Say expects two params, not "
                <<params.size()<<".\n";
      return false;
    }
    //first parameter is Wave/MIDI/MP3 file, second is text
    //push function
    chunk.pushCode(CodeSay);
    //push file name's length
    chunk.data.push_back(params[1].length());
    //push file name
    chunk.pushString(params[1]);
    //push text's length as short(!), not byte
    chunk.pushShort(params[2].length());
    //push text
    chunk.pushString(params[2]);
    return true;
  }//if Say
  if (lowerFunction == "setangle")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: SetAngle needs two parameters!\n";
      return false;
    }
    //first parameter is axis, given as upper case character
    //second parameter is new angle value (float)
    //get axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to SetAngle.\n";
    }
    //second param
    float new_angle;
    SC_VarRef localRef = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[2], new_angle))
    {
      //In Tribunal or Bloodmoon, local vars are allowed, too.
      localRef = chunk.getVariableTypeWithIndex(params[2]);
      if (localRef.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is neither a "
                  << "float value nor a local var.\n";
        return false;
      }
    }
    //push function
    chunk.pushCode(CodeSetAngle);
    //push axis
    chunk.data.push_back(Axis);
    if (localRef.Type!=vtGlobal)
    {
      //It's a global var, push it!
      chunk.pushNonGlobalRefWithTwoZeroFillers(localRef);
    }
    else
    {
      //It's a simple float, push!
      chunk.pushFloat(new_angle);
    }
    return true;
  }//if SetAngle
  if (lowerFunction == "setjournalindex")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: SetJournalIndex needs two parameters!\n";
      return false;
    }
    //first parameter is journal ID, second is index (short)
    int16_t j_index;
    if (!stringToShort(params[2], j_index))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a short.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodeSetJournalIndex);
    //push journal ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    //push index
    chunk.pushShort(j_index);
    //push two FF bytes as filler
    chunk.data.push_back(255);
    chunk.data.push_back(255);
    return true;
  }//if SetJournalIndex
  if (lowerFunction == "setpcfacrep")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: SetPCFacRep needs two parameters!\n";
      return false;
    }
    //first parameter is new reputation value, second is ID of faction
    float new_reputation;
    if (!stringToFloat(params[1], new_reputation))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a float.\n";
      return false;
    }//if
    //push function
    chunk.pushCode(CodeSetPCFacRep);
    //push float
    chunk.pushFloat(new_reputation);
    //push faction ID's length
    chunk.data.push_back(params[2].length());
    //push faction ID
    chunk.pushString(params[2]);
    return true;
  }//if SetPCFacRep
  if (lowerFunction == "setpos")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: SetPos needs two parameters!\n";
      return false;
    }
    //first parameter is axis, given as upper case character
    //second parameter is new position value (float or local var)
    //get axis
    const char Axis = toupper(params[1].at(0));
    /*The CS does not check, if the parameter is really X, Y or Z, but allows
      any letter, as it seems. I'm not sure about the consequences in-game.
      We should put a warning at least.
    */
    if ((Axis != 'X') && (Axis != 'Y') && (Axis != 'Z'))
    {
      std::cout << "ScriptCompiler: Warning: invalid parameter to SetPos.\n";
    }
    //second param
    float new_pos;
    SC_VarRef localRef = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[2], new_pos))
    {
      //In Tribunal or Bloodmoon, local vars are allowed, too.
      localRef = chunk.getVariableTypeWithIndex(params[2]);
      if (localRef.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is neither a "
                  << "float value nor a local var.\n";
        return false;
      }
    }
    //push function
    chunk.pushCode(CodeSetPos);
    //push axis
    chunk.data.push_back(Axis);
    if (localRef.Type!=vtGlobal)
    {
      //It's a global var, push it!
      chunk.pushNonGlobalRefWithTwoZeroFillers(localRef);
    }
    else
    {
      //It's a simple float, push!
      chunk.pushFloat(new_pos);
    }
    return true;
  }//if SetPos
  //end - no matching function found, if we are here
  return false;
}

bool ScriptFunctions_ThreeParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "addtolevcreature")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: AddToLevCreature needs three parameters!\n";
      return false;
    }
    //first parameter is ID of leveled creature, second is ID of creature to be added
    //third is corresponding level (float, believe it or not)
    // ---- check for level
    float level;
    if (!stringToFloat(params[3], level))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //push functio code
    chunk.pushCode(CodeAddToLevCreature);
    //push list ID's length
    chunk.data.push_back(params[1].length());
    //push list ID
    chunk.pushString(params[1]);
    //push creature ID's length
    chunk.data.push_back(params[2].length());
    //push creature ID
    chunk.pushString(params[2]);
    //push level
    chunk.pushFloat(level);
    return true;
  }//if AddToLevCreature
  if (lowerFunction == "addtolevitem")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: AddToLevItem needs three parameters!\n";
      return false;
    }
    //first parameter is ID of leveled item, second is ID of item to be added
    //third is corresponding level (float, believe it or not)
    // ---- check for level
    float level;
    if (!stringToFloat(params[3], level))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //push functio code
    chunk.pushCode(CodeAddToLevItem);
    //push list ID's length
    chunk.data.push_back(params[1].length());
    //push list ID
    chunk.pushString(params[1]);
    //push item ID's length
    chunk.data.push_back(params[2].length());
    //push item ID
    chunk.pushString(params[2]);
    //push level
    chunk.pushFloat(level);
    return true;
  }//if AddToLevItem
  if (lowerFunction == "aitravel")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: AITravel needs three parameters!\n";
      return false;
    }
    //first, second and third parameter is x, y, z coord. (float)
    // ---- check x
    float x_coord;
    if (!stringToFloat(params[1], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[2], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[3], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAITravel);
    //push coordinates
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push NUL byte to indicate absence of reset flag
    chunk.data.push_back(0);
    return true;
  }//if AITravel
  if (lowerFunction == "aiwander")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs three parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push seems like three NUL bytes follow in that three param version
    chunk.data.push_back(0);
    chunk.data.push_back(0);
    chunk.data.push_back(0);
    return true;
  }//if AIWander
  // workaround for 3-param-version of Face
  if (lowerFunction == "face")
  {
    if (params.size() < 4)
    {
      std::cout << "ScriptCompiler: Error: Face needs two (three) parameters!\n";
      return false;
    }
    //first parameter is float, and so is second
    //third parameter is ignored, because it won't be in the script data anyway
    float first;
    if (!stringToFloat(params[1], first))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "float value.\n";
      return false;
    }//if
    float second;
    if (!stringToFloat(params[2], second))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeFace);
    //push both floats
    chunk.pushFloat(first);
    chunk.pushFloat(second);
    return true;
  }//if Face
  if (lowerFunction == "loopgroup")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: LoopGroup needs three parameters!\n";
      return false;
    }
    //first parameter is group name (will be translated into short)
    //second parameters is number (short)
    //third parameter is flag (byte-sized integer)
    int16_t groupIndex = 0;
    if (!getAnimationGroupIndex(params[1], groupIndex))
    {
      std::cout << "ScriptCompiler: Error: Couldn't find animation group index "
                << "for group name \""<<params[1]<<"\".\n";
      return false;
    }
    int16_t number;
    if (!stringToShort(params[2], number))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value.\n";
      return false;
    }//if
    int16_t flag;
    if (!stringToShort(params[3], flag))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no short value.\n";
      return false;
    }//if
    //push function
    chunk.pushCode(CodeLoopGroup);
    //push animation group index
    chunk.pushShort(groupIndex);
    //push number as byte(!)
    chunk.data.push_back(number);
    //push flag as byte (not as short)
    chunk.data.push_back(flag);
    return true;
  }//if LoopGroup
  if (lowerFunction == "messagebox")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: MessageBox needs three parameters!\n";
      return false;
    }
    chunk.pushCode(CodeMessageBox);
    //first parameter is a string
    //second and third parameters are either a local var or another string
    //push string's length as short (not byte)
    chunk.pushShort(params[1].length());
    //push parameter string
    chunk.pushString(params[1]);
    //now check if second param is a local var
    uint8_t arg_count = 0;
    uint8_t button_count = 0;
    SC_VarRef firstRef = chunk.getVariableTypeWithIndex(params[2]);
    if (firstRef.Type!=vtGlobal)
    {
      //it's a local var
      arg_count = 1;
    }
    else if (Globals::get().hasRecord(params[2]))
    {
      //It's a global var, that counts as argument.
      arg_count = 1;
    }
    else
    {
      //It's a string.
      button_count = 1;
    }
    SC_VarRef secondRef = SC_VarRef(vtGlobal, 0);
    //only check this one if previous param was a variable
    if (arg_count>0)
    {
      secondRef = chunk.getVariableTypeWithIndex(params[3]);
      if (secondRef.Type!=vtGlobal)
      {
        //it's a local var
        ++arg_count;
      }
      else if (Globals::get().hasRecord(params[3]))
      {
        //It's a global var, that counts as argument.
        ++arg_count;
      }
      else
      {
        //It's a string.
        ++button_count;
      }
    }//if first param was var

    //push number of arguments
    chunk.data.push_back(arg_count);
    //push arguments, if present
    if (arg_count>0)
    {
      if (firstRef.Type!=vtGlobal)
      {
        //push local ref
        chunk.pushNonGlobalRef(firstRef);
      }
      else
      {
        //push global
        const std::string& globName = Globals::get().getRecord(params[2]).recordID;
        //push G for global
        chunk.data.push_back('G');
        //push length (including null-terminating byte)
        chunk.data.push_back(globName.length()+1);
        //push global name itself
        chunk.pushString(globName);
        //push terminating NUL character
        chunk.data.push_back(0);
      }
    }//first argument
    if (arg_count>1)
    {
      if (secondRef.Type!=vtGlobal)
      {
        //push local ref
        chunk.pushNonGlobalRef(secondRef);
      }
      else
      {
        //push global
        const std::string& globName = Globals::get().getRecord(params[3]).recordID;
        //push G for global
        chunk.data.push_back('G');
        //push length (including null-terminating byte)
        chunk.data.push_back(globName.length()+1);
        //push global name itself
        chunk.pushString(globName);
        //push terminating NUL character
        chunk.data.push_back(0);
      }
    }//second argument
    //push number of buttons
    chunk.data.push_back(button_count);
    //push button, if present
    if (button_count>0)
    {
      unsigned int i;
      for (i=2+arg_count; i<=3; ++i)
      {
        //push length of string + one byte for NUL
        chunk.data.push_back(params[i].length()+1);
        //push the string
        chunk.pushString(params[i]);
        //push NUL byte
        chunk.data.push_back(0);
      }//for
    }//buttons
    return true;
  }//if MessageBox
  if (lowerFunction == "modfactionreaction")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: ModFactionReaction needs three parameters!\n";
      return false;
    }
    //first parameter is ID of first faction, second is ID of other faction
    //third is reaction (short)
    //check for reaction
    int16_t new_reaction;
    if (!stringToShort(params[3], new_reaction))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeModFactionReaction);
    //first ID's length
    chunk.data.push_back(params[1].length());
    //push first ID
    chunk.pushString(params[1]);
    //second ID's length
    chunk.data.push_back(params[2].length());
    //push second ID
    chunk.pushString(params[2]);
    //push new reaction
    chunk.pushShort(new_reaction);
    return true;
  }//if ModFactionReaction
  if (lowerFunction == "playloopsound3dvp")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: PlayLoopSound3DVP needs three parameters!\n";
      return false;
    }
    //first parameter is sound ID, second is volume (float), third is pitch (float)
    // ---- go for volume
    float volume;
    if (!stringToFloat(params[2], volume))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a float "
                << "value.\n";
      return false;
    }
    // ---- go for pitch
    float pitch;
    if (!stringToFloat(params[3], pitch))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is not a float "
                << "value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodePlayLoopSound3DVP);
    //push sound ID's length
    chunk.data.push_back(params[1].length());
    //push sound ID
    chunk.pushString(params[1]);
    //push volume
    chunk.pushFloat(volume);
    //push pitch
    chunk.pushFloat(pitch);
    return true;
  }//if PlayLoopSound3DVP
  if (lowerFunction == "playsoundvp")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: PlaySoundVP needs three parameters!\n";
      return false;
    }
    //first parameter is sound ID, second is volume (float), third is pitch (float)
    // ---- go for volume
    float volume;
    if (!stringToFloat(params[2], volume))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a float "
                << "value.\n";
      return false;
    }
    // ---- go for pitch
    float pitch;
    if (!stringToFloat(params[3], pitch))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is not a float "
                << "value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodePlaySoundVP);
    //push sound ID's length
    chunk.data.push_back(params[1].length());
    //push sound ID
    chunk.pushString(params[1]);
    //push volume
    chunk.pushFloat(volume);
    //push pitch
    chunk.pushFloat(pitch);
    return true;
  }//if PlaySoundVP
  if (lowerFunction == "playsound3dvp")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: PlaySound3DVP needs three parameters!\n";
      return false;
    }
    //first parameter is sound ID, second is volume (float), third is pitch (float)
    // ---- go for volume
    float volume;
    if (!stringToFloat(params[2], volume))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a float "
                << "value.\n";
      return false;
    }
    // ---- go for pitch
    float pitch;
    if (!stringToFloat(params[3], pitch))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is not a float "
                << "value.\n";
      return false;
    }
    //push function
    chunk.pushCode(CodePlaySound3DVP);
    //push sound ID's length
    chunk.data.push_back(params[1].length());
    //push sound ID
    chunk.pushString(params[1]);
    //push volume
    chunk.pushFloat(volume);
    //push pitch
    chunk.pushFloat(pitch);
    return true;
  }//if PlaySound3DVP
  if (lowerFunction == "removefromlevcreature")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: RemoveFromLevCreature needs three parameters!\n";
      return false;
    }
    //first parameter is ID of leveled creature, second is ID of creature to be removed
    //third is corresponding level (float, believe it or not)
    // ---- check for level
    float level;
    if (!stringToFloat(params[3], level))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //push functio code
    chunk.pushCode(CodeRemoveFromLevCreature);
    //push list ID's length
    chunk.data.push_back(params[1].length());
    //push list ID
    chunk.pushString(params[1]);
    //push creature ID's length
    chunk.data.push_back(params[2].length());
    //push creature ID
    chunk.pushString(params[2]);
    //push level
    chunk.pushFloat(level);
    return true;
  }//if RemoveFromLevCreature
  if (lowerFunction == "removefromlevitem")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: RemoveFromLevItem needs three parameters!\n";
      return false;
    }
    //first parameter is ID of leveled item, second is ID of item to be removed
    //third is corresponding level (float, believe it or not)
    // ---- check for level
    float level;
    if (!stringToFloat(params[3], level))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //push functio code
    chunk.pushCode(CodeRemoveFromLevItem);
    //push list ID's length
    chunk.data.push_back(params[1].length());
    //push list ID
    chunk.pushString(params[1]);
    //push item ID's length
    chunk.data.push_back(params[2].length());
    //push item ID
    chunk.pushString(params[2]);
    //push level
    chunk.pushFloat(level);
    return true;
  }//if RemoveFromLevItem
  if (lowerFunction == "setfactionreaction")
  {
    if (params.size()<4)
    {
      std::cout << "ScriptCompiler: Error: SetFactionReaction needs three parameters!\n";
      return false;
    }
    //first parameter is ID of first faction, second is ID of other faction
    //third is reaction change (short)
    //check for reaction
    int16_t add_reaction;
    if (!stringToShort(params[3], add_reaction))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeSetFactionReaction);
    //first ID's length
    chunk.data.push_back(params[1].length());
    //push first ID
    chunk.pushString(params[1]);
    //second ID's length
    chunk.data.push_back(params[2].length());
    //push second ID
    chunk.pushString(params[2]);
    //push new reaction
    chunk.pushShort(add_reaction);
    return true;
  }//if SetFactionReaction
  //if we get to this point, no match was found
  return false;
}

bool ScriptFunctions_FourParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "aitravel")
  {
    if (params.size()<5)
    {
      std::cout << "ScriptCompiler: Error: AITravel needs four parameters!\n";
      return false;
    }
    //first, second and third parameter is x, y, z coord. (float)
    //fourth parameter is length of flag
    // ---- check x
    float x_coord;
    if (!stringToFloat(params[1], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[2], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[3], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAITravel);
    //push coordinates
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push length of reset flag's string (yeah, that's what it does)
    chunk.data.push_back(params[4].length());
    return true;
  }//if AITravel
  if (lowerFunction == "placeatme")
  {
    if (params.size()<5)
    {
      std::cout << "ScriptCompiler: Error: PlaceAtMe needs four parameters!\n";
      return false;
    }
    //first parameter is ID of item, second is short that indicates the count,
    //third is distance (float), fourth is direction (short)
    //check for count
    int16_t count;
    if (!stringToShort(params[2], count))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    //check for distance
    float distance;
    if (!stringToFloat(params[3], distance))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check for direction
    int16_t direction;
    if (!stringToShort(params[4], direction))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodePlaceAtMe);
    //push item ID's length
    chunk.data.push_back(params[1].length());
    //push item ID
    chunk.pushString(params[1]);
    //push further parameters
    chunk.pushShort(count);
    chunk.pushFloat(distance);
    chunk.pushShort(direction);
    return true;
  }//if PlaceAtMe
  if (lowerFunction == "placeatpc")
  {
    if (params.size()<5)
    {
      std::cout << "ScriptCompiler: Error: PlaceAtPC needs four parameters!\n";
      return false;
    }
    //first parameter is ID of item, second is short that indicates the count,
    //third is distance (float), fourth is direction (short)
    //check for count
    int16_t count;
    if (!stringToShort(params[2], count))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    //check for distance
    float distance;
    if (!stringToFloat(params[3], distance))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check for direction
    int16_t direction;
    if (!stringToShort(params[4], direction))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodePlaceAtPC);
    //push item ID's length
    chunk.data.push_back(params[1].length());
    //push item ID
    chunk.pushString(params[1]);
    //push further parameters
    chunk.pushShort(count);
    chunk.pushFloat(distance);
    chunk.pushShort(direction);
    return true;
  }//if PlaceAtPC
  if (lowerFunction == "position")
  {
    if (params.size()<5)
    {
      std::cout << "ScriptCompiler: Error: Position needs four parameters!\n";
      return false;
    }
    //first parameter is x-coordinate, second is y-coordinate, third is z-coord.
    //fourth is z-rotation
    // ---- check for x-coord
    float x_coord;
    if (!stringToFloat(params[1], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check for y-coord
    float y_coord;
    if (!stringToFloat(params[2], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check for z-coord
    float z_coord;
    if (!stringToFloat(params[3], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    // ---- check for z-coord
    float z_rotation;
    if (!stringToFloat(params[4], z_rotation))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    //push function
    chunk.pushCode(CodePosition);
    //push coordinates and rotation value
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    chunk.pushFloat(z_rotation);
    return true;
  }//if Position
  //nothing found, return false
  return false;
}

bool ScriptFunctions_FiveParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  uint16_t functionCode = 0;
  if (lowerFunction == "aiescort")
  {
    functionCode = CodeAIEscort;
  }
  else if (lowerFunction == "aifollow")
  {
    functionCode = CodeAIFollow;
  }
  if (functionCode!=0)
  {
    if (params.size()<6)
    {
      std::cout << "ScriptCompiler: Error: "<<params[0]<<" needs five parameters!\n";
      return false;
    }
    //first parameter is NPC ID
    //second parameter is duration (short, as far as I can see)
    //third, fourth and fifth parameters are x, y, z-coordinates (float)
    //sixth parameter is flag of some kind, but absent in this version
    int16_t duration;
    if (!stringToShort(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    float x_coord;
    if (!stringToFloat(params[3], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[4], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[5], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value!\n";
      return false;
    }
    //push function
    chunk.pushCode(functionCode);
    //push target ID's length
    chunk.data.push_back(params[1].length());
    //push target ID
    chunk.pushString(params[1]);
    //push duration
    chunk.pushShort(duration);
    //push x, y, z
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push NUL byte (it always seems to be there and indicate the absence of the
    // flag)
    chunk.data.push_back(0);
    return true;
  }//if AIEscort or AIFollow

  if (lowerFunction == "placeitem")
  {
    if (params.size()<6)
    {
      std::cout << "ScriptCompiler: Error: PlaceItem needs five parameters!\n";
      return false;
    }
    //first parameter is ID of item
    // second to fourth is X, Y, Z (all float)
    // fifth is ZRot (float)

    //check coordinates
    // ---- X
    float x_coord;
    SC_VarRef x_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[2], x_coord))
    {
      //could still be a float var
      x_ref = chunk.getVariableTypeWithIndex(params[2]);
      if (x_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value or var!\n";
        return false;
      }
    }
    // ---- Y
    float y_coord;
    SC_VarRef y_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[3], y_coord))
    {
      //could still be a float var
      y_ref = chunk.getVariableTypeWithIndex(params[3]);
      if (y_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value or var!\n";
        return false;
      }
    }
    // ---- Z
    float z_coord;
    SC_VarRef z_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[4], z_coord))
    {
      //could still be a float var
      z_ref = chunk.getVariableTypeWithIndex(params[4]);
      if (z_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value or var!\n";
        return false;
      }
    }
    // ---- rot
    float rot_coord;
    SC_VarRef rot_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[5], rot_coord))
    {
      //could still be a float var
      rot_ref = chunk.getVariableTypeWithIndex(params[5]);
      if (rot_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value or var!\n";
        return false;
      }
    }
    //push function
    chunk.pushCode(CodePlaceItem);
    //now push params
    //ID first
    //ID's length
    chunk.data.push_back(params[1].length());
    //the ID itself
    chunk.pushString(params[1]);
    // push x
    if (x_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(x_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(x_coord);
    }
    // push y
    if (y_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(y_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(y_coord);
    }
    // push z
    if (z_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(z_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(z_coord);
    }
    // push rotation
    if (rot_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(rot_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(rot_coord);
    }
    return true;
  }//if function==PlaceItem

  if (lowerFunction == "positioncell")
  {
    if (params.size()<6)
    {
      std::cout << "ScriptCompiler: Error: PositionCell needs five parameters!\n";
      return false;
    }
    //first parameter is x-coordinate, second is y-coordinate, third is z-coord.
    //fourth is z-rotation, fifth is cell name
    // ---- check for x-coord
    float x_coord;
    if (!stringToFloat(params[1], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check for y-coord
    float y_coord;
    if (!stringToFloat(params[2], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check for z-coord
    float z_coord;
    if (!stringToFloat(params[3], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    // ---- check for z-coord
    float z_rotation;
    if (!stringToFloat(params[4], z_rotation))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    //push function
    chunk.pushCode(CodePositionCell);
    //push coordinates and rotation value
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    chunk.pushFloat(z_rotation);
    //push cell name's length
    chunk.data.push_back(params[5].length());
    //push cell name
    chunk.pushString(params[5]);
    return true;
  }//if PositionCell

  //nothing found, return false
  return false;
}//function Five

bool ScriptFunctions_SixParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  uint16_t functionCode = 0;
  if (lowerFunction == "aiescort")
  {
    functionCode = CodeAIEscort;
  }
  else if (lowerFunction == "aifollow")
  {
    functionCode = CodeAIFollow;
  }
  if (functionCode!=0)
  {
    if (params.size()<7)
    {
      std::cout << "ScriptCompiler: Error: "<<params[0]<<" needs six parameters!\n";
      return false;
    }
    //first parameter is NPC ID
    //second parameter is duration (short, as far as I can see)
    //third, fourth and fifth parameters are x, y, z-coordinates (float)
    //sixth parameter is flag of some kind
    int16_t duration;
    if (!stringToShort(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    float x_coord;
    if (!stringToFloat(params[3], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[4], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[5], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value!\n";
      return false;
    }
    //Sixth param is flag, but it does not seem to have an impact on the compiled
    //script data, so we ignore it, whatever it is.

    //push function
    chunk.pushCode(functionCode);
    //push target ID's length
    chunk.data.push_back(params[1].length());
    //push target ID
    chunk.pushString(params[1]);
    //push duration
    chunk.pushShort(duration);
    //push x, y, z
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push 01 byte (it always seems to be there and indicate the presence of the
    // flag)
    chunk.data.push_back(1);
    return true;
  }//if AIEscort or AIFollow

  if (lowerFunction == "aiescortcell")
  {
    functionCode = CodeAIEscortCell;
  }
  else if (lowerFunction == "aifollowcell")
  {
    functionCode = CodeAIFollowCell;
  }
  if (functionCode!=0)
  {
    if (params.size()<7)
    {
      std::cout << "ScriptCompiler: Error: "<<params[0]<<" needs six parameters!\n";
      return false;
    }
    //first parameter is NPC ID
    //second parameter is cell ID
    //third parameter is duration (short, as far as I can see)
    //fourth, fifth and sixth parameters are x, y, z-coordinates (float)
    int16_t duration;
    if (!stringToShort(params[3], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no short value!\n";
      return false;
    }
    float x_coord;
    if (!stringToFloat(params[4], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[5], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[6], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no float value!\n";
      return false;
    }
    //push function
    chunk.pushCode(functionCode);
    //push target ID's length
    chunk.data.push_back(params[1].length());
    //push target ID
    chunk.pushString(params[1]);
    //push cell ID's length
    chunk.data.push_back(params[2].length());
    //push cell ID
    chunk.pushString(params[2]);
    //push duration
    chunk.pushShort(duration);
    //push x, y, z
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push NUL byte (it always seems to be there and indicate the absence of the
    // flag)
    chunk.data.push_back(0);
    return true;
  }//if AIEscortCell or AI FollowCell


  if (lowerFunction == "aiwander")
  {
    if (params.size()<7)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs six parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    //fourth to sixth parameter is idle2 to idle4 (each short)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check Idle2
    int16_t idle2;
    if (!stringToShort(params[4], idle2))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no short value!\n";
      return false;
    }
    //check Idle3
    int16_t idle3;
    if (!stringToShort(params[5], idle3))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no short value!\n";
      return false;
    }
    //check Idle4
    int16_t idle4;
    if (!stringToShort(params[6], idle4))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push number of relevant shorts - two in this case (as short)
    chunk.pushShort(2);
    //idle2 seems to be omitted in compiled code, so we don't push that
    //push idle3 to idle4
    chunk.pushShort(idle3);
    chunk.pushShort(idle4);
    //seems like there's a 01 byte at the end (or was it 02?)
    chunk.data.push_back(1);
    return true;
  }//if AIWander
  if (lowerFunction == "placeitemcell")
  {
    if (params.size()<7)
    {
      std::cout << "ScriptCompiler: Error: PlaceItem needs six parameters!\n";
      return false;
    }
    //first parameter is ID of item
    // second is cell ID
    // third to fifth is X, Y, Z (all float)
    // sixth is ZRot (float)

    //check coordinates
    // ---- X
    float x_coord;
    SC_VarRef x_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[3], x_coord))
    {
      //could still be a float var
      x_ref = chunk.getVariableTypeWithIndex(params[3]);
      if (x_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value or var!\n";
        return false;
      }
    }
    // ---- Y
    float y_coord;
    SC_VarRef y_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[4], y_coord))
    {
      //could still be a float var
      y_ref = chunk.getVariableTypeWithIndex(params[4]);
      if (y_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value or var!\n";
        return false;
      }
    }
    // ---- Z
    float z_coord;
    SC_VarRef z_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[5], z_coord))
    {
      //could still be a float var
      z_ref = chunk.getVariableTypeWithIndex(params[5]);
      if (z_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value or var!\n";
        return false;
      }
    }
    // ---- rot
    float rot_coord;
    SC_VarRef rot_ref = SC_VarRef(vtGlobal, 0);
    if (!stringToFloat(params[6], rot_coord))
    {
      //could still be a float var
      rot_ref = chunk.getVariableTypeWithIndex(params[6]);
      if (rot_ref.Type==vtGlobal)
      {
        std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no float value or var!\n";
        return false;
      }
    }
    //push function
    chunk.pushCode(CodePlaceItemCell);
    //now push params
    //item ID first
    //ID's length
    chunk.data.push_back(params[1].length());
    //the ID itself
    chunk.pushString(params[1]);
    //cell ID second
    //ID's length
    chunk.data.push_back(params[2].length());
    //the ID itself
    chunk.pushString(params[2]);
    // push x
    if (x_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(x_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(x_coord);
    }
    // push y
    if (y_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(y_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(y_coord);
    }
    // push z
    if (z_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(z_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(z_coord);
    }
    // push rotation
    if (rot_ref.Type!=vtGlobal)
    {
      //it's a reference to a local var, so push here!
      chunk.pushNonGlobalRefWithTwoZeroFillers(rot_ref);
    }
    else
    {
      //it's a plain float
      chunk.pushFloat(rot_coord);
    }
    return true;
  }//if function==PlaceItemCell
  //nothing found, return false
  return false;
}//function Six

bool ScriptFunctions_SevenParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  uint16_t functionCode = 0;
  if (lowerFunction == "aiescortcell")
  {
    functionCode = CodeAIEscortCell;
  }
  else if (lowerFunction == "aifollowcell")
  {
    functionCode = CodeAIFollowCell;
  }
  if (functionCode!=0)
  {
    if (params.size()<8)
    {
      std::cout << "ScriptCompiler: Error: "<<params.at(0)<<" needs seven parameters!\n";
      return false;
    }
    //first parameter is NPC ID
    //second parameter is cell ID
    //third parameter is duration (short, as far as I can see)
    //fourth, fifth and sixth parameters are x, y, z-coordinates (float)
    //seventh parameter is a flag
    int16_t duration;
    if (!stringToShort(params[3], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no short value!\n";
      return false;
    }
    float x_coord;
    if (!stringToFloat(params[4], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[5], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[6], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no float value!\n";
      return false;
    }
    //seventh parameter, the flag, is ignored. What counts is its presence.

    //push function
    chunk.pushCode(functionCode);
    //push target ID's length
    chunk.data.push_back(params[1].length());
    //push target ID
    chunk.pushString(params[1]);
    //push cell ID's length
    chunk.data.push_back(params[2].length());
    //push cell ID
    chunk.pushString(params[2]);
    //push duration
    chunk.pushShort(duration);
    //push x, y, z
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push 01 byte (it always seems to be there and indicate the presence of the
    // flag)
    chunk.data.push_back(1);
    return true;
  }//if AIEscortCell or AIFollowCell

  //workaround for wrong number of parameters AIFollow (usually only six or five)
  if (lowerFunction == "aifollow")
  {
    //first parameter is NPC ID
    //second parameter is duration (short, as far as I can see)
    //third, fourth and fifth parameters are x, y, z-coordinates (float)
    //sixth parameter is flag of some kind
    //seventh parameter is too much, but that's what we're here for
    int16_t duration;
    if (!stringToShort(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    float x_coord;
    if (!stringToFloat(params[3], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[4], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[5], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value!\n";
      return false;
    }
    //Sixth param is flag, but it does not seem to have an impact on the compiled
    //script data, so we ignore it, whatever it is.

    //push function code
    chunk.pushCode(CodeAIFollow);
    //push target ID's length
    chunk.data.push_back(params[1].length());
    //push target ID
    chunk.pushString(params[1]);
    //push duration
    chunk.pushShort(duration);
    //push x, y, z
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push 01 byte (it always seems to be there and indicate the presence of the
    // flag)
    chunk.data.push_back(1);
    return true;
  }//if AIFollow
  if (lowerFunction == "aiwander")
  {
    if (params.size()<8)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs seven parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    //fourth to seventh parameter is idle2 to idle5 (each short)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check Idle2
    int16_t idle2;
    if (!stringToShort(params[4], idle2))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no short value!\n";
      return false;
    }
    //check Idle3
    int16_t idle3;
    if (!stringToShort(params[5], idle3))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no short value!\n";
      return false;
    }
    //check Idle4
    int16_t idle4;
    if (!stringToShort(params[6], idle4))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no short value!\n";
      return false;
    }
    //check Idle5
    int16_t idle5;
    if (!stringToShort(params[7], idle5))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push number of relevant shorts - three in this case (as short)
    chunk.pushShort(3);
    //idle2 seems to be omitted in compiled code, so we don't push that
    //push idle3 to idle5
    chunk.pushShort(idle3);
    chunk.pushShort(idle4);
    chunk.pushShort(idle5);
    //seems like there's a 03 byte at the end
    chunk.data.push_back(3);
    return true;
  }//if AIWander
  //nothing found, return false here
  return false;
}

bool ScriptFunctions_EightParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  //workaround for wrong number of parameters AIFollow (usually only six or five)
  if (lowerFunction == "aifollow")
  {
    //first parameter is NPC ID
    //second parameter is duration (short, as far as I can see)
    //third, fourth and fifth parameters are x, y, z-coordinates (float)
    //sixth parameter is flag of some kind
    //seventh & eighth parameters are too much, but that's what we're here for
    int16_t duration;
    if (!stringToShort(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    float x_coord;
    if (!stringToFloat(params[3], x_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    float y_coord;
    if (!stringToFloat(params[4], y_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no float value!\n";
      return false;
    }
    float z_coord;
    if (!stringToFloat(params[5], z_coord))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no float value!\n";
      return false;
    }
    //Sixth param is flag, but it does not seem to have an impact on the compiled
    //script data, so we ignore it, whatever it is. Seventh and eighth parameter
    //are ignored any way.

    //push function code
    chunk.pushCode(CodeAIFollow);
    //push target ID's length
    chunk.data.push_back(params[1].length());
    //push target ID
    chunk.pushString(params[1]);
    //push duration
    chunk.pushShort(duration);
    //push x, y, z
    chunk.pushFloat(x_coord);
    chunk.pushFloat(y_coord);
    chunk.pushFloat(z_coord);
    //push 01 byte (it always seems to be there and indicate the presence of the
    // flag)
    chunk.data.push_back(1);
    return true;
  }//if AIFollow

  if (lowerFunction == "aiwander")
  {
    if (params.size()<9)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs eight parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    //fourth to eightth parameter is idle2 to idle6 (each short)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check Idle2
    int16_t idle2;
    if (!stringToShort(params[4], idle2))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no short value!\n";
      return false;
    }
    //check Idle3
    int16_t idle3;
    if (!stringToShort(params[5], idle3))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no short value!\n";
      return false;
    }
    //check Idle4
    int16_t idle4;
    if (!stringToShort(params[6], idle4))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no short value!\n";
      return false;
    }
    //check Idle5
    int16_t idle5;
    if (!stringToShort(params[7], idle5))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is no short value!\n";
      return false;
    }
    //check Idle6
    int16_t idle6;
    if (!stringToShort(params[8], idle6))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[8]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push number of relevant shorts - four in this case (as short)
    chunk.pushShort(4);
    //idle2 seems to be omitted in compiled code, so we don't push that
    //push idle3 to idle5
    chunk.pushShort(idle3);
    chunk.pushShort(idle4);
    chunk.pushShort(idle5);
    chunk.pushShort(idle6);
    //seems like there's a 03 byte at the end
    chunk.data.push_back(3);
    return true;
  }//if AIWander
  //nothing found, return false here
  return false;
}//function Eight

bool ScriptFunctions_NineParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "aiwander")
  {
    if (params.size()<10)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs nine parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    //fourth to ninth parameter is idle2 to idle7 (each short)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check Idle2
    int16_t idle2;
    if (!stringToShort(params[4], idle2))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no short value!\n";
      return false;
    }
    //check Idle3
    int16_t idle3;
    if (!stringToShort(params[5], idle3))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no short value!\n";
      return false;
    }
    //check Idle4
    int16_t idle4;
    if (!stringToShort(params[6], idle4))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no short value!\n";
      return false;
    }
    //check Idle5
    int16_t idle5;
    if (!stringToShort(params[7], idle5))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is no short value!\n";
      return false;
    }
    //check Idle6
    int16_t idle6;
    if (!stringToShort(params[8], idle6))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[8]<<"\" is no short value!\n";
      return false;
    }
    //check Idle7
    int16_t idle7;
    if (!stringToShort(params[9], idle7))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[9]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push number of relevant shorts - five in this case (as short)
    chunk.pushShort(5);
    //idle2 seems to be omitted in compiled code, so we don't push that
    //push idle3 to idle7
    chunk.pushShort(idle3);
    chunk.pushShort(idle4);
    chunk.pushShort(idle5);
    chunk.pushShort(idle6);
    chunk.pushShort(idle7);
    //seems like there's a 01 byte at the end
    chunk.data.push_back(1);
    return true;
  }//if AIWander
  if (lowerFunction == "modregion")
  {
    if (params.size()<10)
    {
      std::cout << "ScriptCompiler: Error: ModRegion needs nine parameters!\n";
      return false;
    }
    //first parameter is region ID
    //second to ninth parameter is possibility for certain weather types
    // ---- clear (2nd param)
    int16_t chanceClear;
    if (!stringToShort(params[2], chanceClear))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- cloudy (3rd param)
    int16_t chanceCloudy;
    if (!stringToShort(params[3], chanceCloudy))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- foggy (4th param)
    int16_t chanceFoggy;
    if (!stringToShort(params[4], chanceFoggy))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- overcast (5th param)
    int16_t chanceOvercast;
    if (!stringToShort(params[5], chanceOvercast))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- rain (6th param)
    int16_t chanceRain;
    if (!stringToShort(params[6], chanceRain))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- thunder (7th param)
    int16_t chanceThunder;
    if (!stringToShort(params[7], chanceThunder))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- ash (8th param)
    int16_t chanceAsh;
    if (!stringToShort(params[8], chanceAsh))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[8]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- blight (9th param)
    int16_t chanceBlight;
    if (!stringToShort(params[9], chanceBlight))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[9]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeModRegion);
    //push region ID's length
    chunk.data.push_back(params[1].length());
    //push region ID
    chunk.pushString(params[1]);
    //push weather chances
    chunk.data.push_back(chanceClear);
    chunk.data.push_back(chanceCloudy);
    chunk.data.push_back(chanceFoggy);
    chunk.data.push_back(chanceOvercast);
    chunk.data.push_back(chanceRain);
    chunk.data.push_back(chanceThunder);
    chunk.data.push_back(chanceAsh);
    chunk.data.push_back(chanceBlight);
    //push snow and blizzard chances (Bloodmoon only), which are zero in that case
    chunk.data.push_back(0);//snow
    chunk.data.push_back(0);//blizzard
    return true;
  }//if ModRegion
  //end reached, no match found
  return false;
}//function Nine

bool ScriptFunctions_TenParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "aiwander")
  {
    if (params.size()<11)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs ten parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    //fourth to 10th parameter is idle2 to idle8 (each short)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check Idle2
    int16_t idle2;
    if (!stringToShort(params[4], idle2))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no short value!\n";
      return false;
    }
    //check Idle3
    int16_t idle3;
    if (!stringToShort(params[5], idle3))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no short value!\n";
      return false;
    }
    //check Idle4
    int16_t idle4;
    if (!stringToShort(params[6], idle4))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no short value!\n";
      return false;
    }
    //check Idle5
    int16_t idle5;
    if (!stringToShort(params[7], idle5))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is no short value!\n";
      return false;
    }
    //check Idle6
    int16_t idle6;
    if (!stringToShort(params[8], idle6))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[8]<<"\" is no short value!\n";
      return false;
    }
    //check Idle7
    int16_t idle7;
    if (!stringToShort(params[9], idle7))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[9]<<"\" is no short value!\n";
      return false;
    }
    //check Idle8
    int16_t idle8;
    if (!stringToShort(params[10], idle8))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[10]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push number of relevant shorts - six in this case (as short)
    chunk.pushShort(6);
    //idle2 seems to be omitted in compiled code, so we don't push that
    //push idle3 to idle8
    chunk.pushShort(idle3);
    chunk.pushShort(idle4);
    chunk.pushShort(idle5);
    chunk.pushShort(idle6);
    chunk.pushShort(idle7);
    chunk.pushShort(idle8);
    //seems like there's a 01 byte at the end
    chunk.data.push_back(1);
    return true;
  }//if AIWander
  //end reached, no match found
  return false;
}//function Ten

bool ScriptFunctions_ElevenParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "aiwander")
  {
    if (params.size()<12)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs eleven parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    //fourth to 11th parameter is idle2 to idle9 (each short)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check Idle2
    int16_t idle2;
    if (!stringToShort(params[4], idle2))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no short value!\n";
      return false;
    }
    //check Idle3
    int16_t idle3;
    if (!stringToShort(params[5], idle3))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no short value!\n";
      return false;
    }
    //check Idle4
    int16_t idle4;
    if (!stringToShort(params[6], idle4))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no short value!\n";
      return false;
    }
    //check Idle5
    int16_t idle5;
    if (!stringToShort(params[7], idle5))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is no short value!\n";
      return false;
    }
    //check Idle6
    int16_t idle6;
    if (!stringToShort(params[8], idle6))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[8]<<"\" is no short value!\n";
      return false;
    }
    //check Idle7
    int16_t idle7;
    if (!stringToShort(params[9], idle7))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[9]<<"\" is no short value!\n";
      return false;
    }
    //check Idle8
    int16_t idle8;
    if (!stringToShort(params[10], idle8))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[10]<<"\" is no short value!\n";
      return false;
    }
    //check Idle9
    int16_t idle9;
    if (!stringToShort(params[11], idle9))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[11]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push number of relevant shorts - seven in this case (as short)
    chunk.pushShort(7);
    //idle2 seems to be omitted in compiled code, so we don't push that
    //push idle3 to idle9
    chunk.pushShort(idle3);
    chunk.pushShort(idle4);
    chunk.pushShort(idle5);
    chunk.pushShort(idle6);
    chunk.pushShort(idle7);
    chunk.pushShort(idle8);
    chunk.pushShort(idle9);
    //seems like there's a 02 byte at the end
    chunk.data.push_back(2);
    return true;
  }//if AIWander
  if (lowerFunction == "modregion")
  {
    if (params.size()<12)
    {
      std::cout << "ScriptCompiler: Error: ModRegion needs eleven parameters!\n";
      return false;
    }
    //first parameter is region ID
    //second to eleventh parameter is possibility for certain weather types
    // ---- clear (2nd param)
    int16_t chanceClear;
    if (!stringToShort(params[2], chanceClear))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- cloudy (3rd param)
    int16_t chanceCloudy;
    if (!stringToShort(params[3], chanceCloudy))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- foggy (4th param)
    int16_t chanceFoggy;
    if (!stringToShort(params[4], chanceFoggy))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- overcast (5th param)
    int16_t chanceOvercast;
    if (!stringToShort(params[5], chanceOvercast))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- rain (6th param)
    int16_t chanceRain;
    if (!stringToShort(params[6], chanceRain))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- thunder (7th param)
    int16_t chanceThunder;
    if (!stringToShort(params[7], chanceThunder))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- ash (8th param)
    int16_t chanceAsh;
    if (!stringToShort(params[8], chanceAsh))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[8]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- blight (9th param)
    int16_t chanceBlight;
    if (!stringToShort(params[9], chanceBlight))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[9]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- snow (10th param)
    int16_t chanceSnow;
    if (!stringToShort(params[10], chanceSnow))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[10]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    // ---- blizzard (11th param)
    int16_t chanceBlizzard;
    if (!stringToShort(params[11], chanceBlizzard))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[11]<<"\" is not a valid "
                << "short value.\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeModRegion);
    //push region ID's length
    chunk.data.push_back(params[1].length());
    //push region ID
    chunk.pushString(params[1]);
    //push weather chances
    chunk.data.push_back(chanceClear);
    chunk.data.push_back(chanceCloudy);
    chunk.data.push_back(chanceFoggy);
    chunk.data.push_back(chanceOvercast);
    chunk.data.push_back(chanceRain);
    chunk.data.push_back(chanceThunder);
    chunk.data.push_back(chanceAsh);
    chunk.data.push_back(chanceBlight);
    chunk.data.push_back(chanceSnow);
    chunk.data.push_back(chanceBlizzard);
    return true;
  }//if ModRegion
  //end reached, no match found
  return false;
}//function Eleven

bool ScriptFunctions_TwelveParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction == "aiwander")
  {
    if (params.size()<13)
    {
      std::cout << "ScriptCompiler: Error: AIWander needs twelve parameters!\n";
      return false;
    }
    //first, second and third parameter is range, duration, time (each float)
    //fourth to 11th parameter is idle2 to idle9 (each short)
    //12th is reset flag (short?)
    // ---- check range
    float range;
    if (!stringToFloat(params[1], range))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no float value!\n";
      return false;
    }
    // ---- check duration
    float duration;
    if (!stringToFloat(params[2], duration))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no float value!\n";
      return false;
    }
    // ---- check time
    float wander_time;
    if (!stringToFloat(params[3], wander_time))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[3]<<"\" is no float value!\n";
      return false;
    }
    //check Idle2
    int16_t idle2;
    if (!stringToShort(params[4], idle2))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[4]<<"\" is no short value!\n";
      return false;
    }
    //check Idle3
    int16_t idle3;
    if (!stringToShort(params[5], idle3))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[5]<<"\" is no short value!\n";
      return false;
    }
    //check Idle4
    int16_t idle4;
    if (!stringToShort(params[6], idle4))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[6]<<"\" is no short value!\n";
      return false;
    }
    //check Idle5
    int16_t idle5;
    if (!stringToShort(params[7], idle5))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[7]<<"\" is no short value!\n";
      return false;
    }
    //check Idle6
    int16_t idle6;
    if (!stringToShort(params[8], idle6))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[8]<<"\" is no short value!\n";
      return false;
    }
    //check Idle7
    int16_t idle7;
    if (!stringToShort(params[9], idle7))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[9]<<"\" is no short value!\n";
      return false;
    }
    //check Idle8
    int16_t idle8;
    if (!stringToShort(params[10], idle8))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[10]<<"\" is no short value!\n";
      return false;
    }
    //check Idle9
    int16_t idle9;
    if (!stringToShort(params[11], idle9))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[11]<<"\" is no short value!\n";
      return false;
    }
    //check flag
    int16_t reset_flag;
    if (!stringToShort(params[12], reset_flag))
    {
      std::cout << "ScriptCompiler: Error: \""<<params[12]<<"\" is no short value!\n";
      return false;
    }
    //push function code
    chunk.pushCode(CodeAIWander);
    //push range
    chunk.pushFloat(range);
    //push duration
    chunk.pushFloat(duration);
    //push time
    chunk.pushFloat(wander_time);
    //push number of relevant shorts - eight in this case (as short)
    chunk.pushShort(8);
    //idle2 seems to be omitted in compiled code, so we don't push that
    //push idle3 to idle9
    chunk.pushShort(idle3);
    chunk.pushShort(idle4);
    chunk.pushShort(idle5);
    chunk.pushShort(idle6);
    chunk.pushShort(idle7);
    chunk.pushShort(idle8);
    chunk.pushShort(idle9);
    //push flag
    chunk.pushShort(reset_flag);
    //seems like there's a 02 byte at the end
    chunk.data.push_back(2);
    return true;
  }
  //end reached, no match found
  return false;
}//function Twelve

bool ScriptFunctions(const std::string& line, CompiledChunk& chunk, const bool isCompare)
{
  if (line.empty()) return false;
  // split line into separate parameters
  std::vector<std::string> parameters = explodeParams(line);
  /* Now the first entry in vector params should be the function name, the rest
     is the list of parameters. That's why the size of the vector is one more
     than the number of parameters.
  */
  if (parameters.size()>2)
  {
    //check for message box
    if (ScriptFunctions_MessageBox(parameters, chunk))
    {
      return true;
    }
  }
  switch(parameters.size())
  {
    case 13:
         if (ScriptFunctions_TwelveParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 12:
         if (ScriptFunctions_ElevenParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 11:
         if (ScriptFunctions_TenParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 10:
         if (ScriptFunctions_NineParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 9:
         if (ScriptFunctions_EightParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 8:
         if (ScriptFunctions_SevenParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 7:
         if (ScriptFunctions_SixParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 6:
         if (ScriptFunctions_FiveParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 5:
         if (ScriptFunctions_FourParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 4:
         if (ScriptFunctions_ThreeParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 3:
         if (ScriptFunctions_TwoParameters(parameters, chunk))
         {
           return true;
         }
         break;
    case 2:
         if (ScriptFunctions_OneParameter(parameters, chunk, isCompare))
         {
           return true;
         }
         break;
    case 1:
         if (ScriptFunctions_ZeroParameters(parameters, chunk))
         {
           return true;
         }
         return false;
         break;
    default:
         std::cout << "ScriptCompiler: Warning: no functions defined for "
                   << parameters.size()-1 << " parameters.\nLine was \""
                   << line << "\".\n";
         return false;
  }//switch
  //If we get to this point, no matching function was found
  return false;
}

bool CompareToBinary(const std::string& compareStatement, std::vector<uint8_t>& bin_out, const CompiledChunk& theChunk)
{
  //search for comparison operator
  SC_CompareType comparator = compNone;
  std::string::size_type compPos = getComparePos(compareStatement, comparator);
  if (compPos!=std::string::npos)
  {
    //found it
    //parse left part
    ParserNode leftPart;
    if (!leftPart.splitToTree(compareStatement.substr(0, compPos), theChunk))
    {
      std::cout << "ScriptCompiler: Error: left splitToTree() failed for compare statement.\n";
      return false;
    }
    //parse right part
    ParserNode rightPart;
    bool right_success = false;
    switch (comparator)
    {
      case compEqual:
      case compLessEqual:
      case compGreaterEqual:
      case compNotEqual:
           right_success = rightPart.splitToTree(compareStatement.substr(compPos+2), theChunk);
           break;
      case compLess:
      case compGreater:
           right_success = rightPart.splitToTree(compareStatement.substr(compPos+1), theChunk);
           break;
      case compNone:
           //this should never happen
           std::cout << "ScriptCompiler: Error: no comparator found!\n";
           return false;
           break;
    }//swi
    if (!right_success)
    {
      std::cout << "ScriptCompiler: Error: right splitToTree() failed for compare statement.\n";
      return false;
    }
    //first part of binary data
    bin_out = leftPart.getBinaryContent();
    //push compare operator
    // --- one space first
    bin_out.push_back(' ');
    // --- compare operator itself
    switch (comparator)
    {
      case compEqual:
           bin_out.push_back('=');
           bin_out.push_back('=');
           break;
      case compLessEqual:
           bin_out.push_back('<');
           bin_out.push_back('=');
           break;
      case compGreaterEqual:
           bin_out.push_back('>');
           bin_out.push_back('=');
           break;
      case compNotEqual:
           bin_out.push_back('!');
           bin_out.push_back('=');
           break;
      case compLess:
           bin_out.push_back('<');
           break;
      case compGreater:
           bin_out.push_back('>');
           break;
      case compNone:
           //this should never happen
           std::cout << "ScriptCompiler: Error: no comparator found!\n";
           return false;
           break;
    }//swi
    //push right part of statement
    const std::vector<uint8_t> rightBin = rightPart.getBinaryContent();
    unsigned int counter;
    for (counter=0; counter<rightBin.size(); ++counter)
    {
      bin_out.push_back(rightBin[counter]);
    }
    return true;
  }//if compare operator found
  else
  {
    //no comparator found, so try single function statement
    //parse left part
    ParserNode completePart;
    if (!completePart.splitToTree(compareStatement, theChunk))
    {
      std::cout << "ScriptCompiler: Error: splitToTree() failed for compare "
                << "statement without comparator.\n";
      return false;
    }
    //first part of binary data
    bin_out = completePart.getBinaryContent();
    //push null byte at the end
    bin_out.push_back(0);
    std::cout << "ScriptCompiler: Warning: no comparator found in compare "
              << "statement!\nStatement was \""<< compareStatement <<"\".\n";
    return true;
  }
}//function CompareToBinary

bool CompileScript(const std::string& Text, ScriptRecord& result)
{
  std::vector<std::string> lines;

  size_t offset = 0;
  const size_t len = Text.length();
  std::string::size_type pos = std::string::npos;
  std::string new_line;
  //split text into single lines
  while (offset<len)
  {
    pos = Text.find("\n", offset);
    if (pos!=std::string::npos)
    {
      //found something
      // -> extract complete line
      new_line = Text.substr(offset, pos-offset);
      offset = pos+1;
    }//if
    else
    {
      //found nothing -> rest of string is one line
      new_line = Text.substr(offset);
      offset = len;
    }//else

    // -> remove any comments, if present (comments start with ';')
    pos = getCommentStart(new_line);
    if (pos!=std::string::npos)
    {
      //remove comment - everything after ";" is a comment
      new_line.erase(pos);
    }//if comment found
    //Make sure we don't access an empty string with at() here.
    if (!new_line.empty())
    {
      // -> remove \r characters at the end, if present
      if (new_line.at(new_line.length()-1)=='\r')
      {
        new_line.erase(new_line.length()-1);
      }
    }
    //cut off spaces and tabulators
    trim(new_line);
    if (!new_line.empty())
    {
      lines.push_back(new_line);
    }
  }//while

  // split lines like "else set var to 5" into two lines "else" and "set var to 5"
  for (offset = 0; offset < lines.size(); ++offset)
  {
    if ((lines.at(offset).substr(0,5) == "else ") || (lines.at(offset).substr(0,5) == "else\t"))
    {
      lines.insert(lines.begin() + offset + 1, lines.at(offset).substr(5));
      trimLeft(lines.at(offset + 1));
      lines.at(offset) = "else";
    }
  }

  std::string ScriptName = "";
  std::string WorkString;

  CompiledChunk CompiledData;

  //now check the lines of the script
  //first line should always be begin
  if ((lowerCase(lines.at(0).substr(0,6)) == "begin ") || (lowerCase(lines.at(0).substr(0,6)) == "begin\t"))
  {
    ScriptName = lines.at(0).substr(6);
    trimLeft(ScriptName);//We only trim left side, because right side was
                         //already trimmed for original string.
    if (ScriptName.empty())
    {
      std::cout << "ScriptCompiler: Error: begin clause found with no script name!\n";
      return false;
    }
  }//if
  else
  {
    std::cout << "ScriptCompiler: Error: no begin clause found!\n";
    return false;
  }

  bool EndLineFound = false;
  //check other lines
  unsigned int i;
  for (i=1; i<lines.size(); ++i)
  {
    const std::string::size_type qualStart = getQualifierStart(lines[i]);
    //check for shorts
    if ((lowerCase(lines.at(i).substr(0,6)) == "short ") || (lowerCase(lines.at(i).substr(0,6)) == "short\t"))
    {
      WorkString = lines.at(i).substr(6);
      trim(WorkString);
      if (WorkString.empty())
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of short found!\n";
        return false;
      }
      CompiledData.varsShort.push_back(WorkString);
    }//if short
    //check for longs
    else if ((lowerCase(lines.at(i).substr(0,5)) == "long ") || (lowerCase(lines.at(i).substr(0,5)) == "long\t"))
    {
      WorkString = lines.at(i).substr(5);
      trim(WorkString);
      if (WorkString.empty())
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of long found!\n";
        return false;
      }
      CompiledData.varsLong.push_back(WorkString);
    }//if long
    //check for floats
    else if ((lowerCase(lines.at(i).substr(0,6)) == "float ") || (lowerCase(lines.at(i).substr(0,6)) == "float\t"))
    {
      WorkString = lines.at(i).substr(6);
      trim(WorkString);
      if (WorkString.empty())
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of float found!\n";
        return false;
      }
      CompiledData.varsFloat.push_back(WorkString);
    }//if float
    //check for Set
    else if (lowerCase(lines.at(i).substr(0,4)) == "set ")
    {
      //erase "set " from the line
      WorkString = lines.at(i).substr(4);
      trimLeft(WorkString);
      //now select the bits of "set variable To value/expression"
      const std::string::size_type pos_of_to = getPosOf_To_(WorkString);
      if (pos_of_to == std::string::npos)
      {
        std::cout << "ScriptCompiler: Error: Set statement has to be like "
                  << "'set variable to value', but no 'to' was found.\n"
                  << "Complete line was \"" << lines.at(i) << "\".\n";
        return false;
      }

      /*If there is a qualifier in expression, that one has to be pushed before
        the set function code.*/
      std::string set_expr = WorkString.substr(pos_of_to+4);/*pos_of_to is
          position of " to " (including spaces). We add four to get rid of "to"
          and both spaces, too. */
      trim(set_expr);
      if (removeEnclosingBrackets(set_expr))
      {
        //We need to trim again, because there are most likely spaces between
        //  the brackets and the first resp. last non-space character of the
        //  string.
        trim(set_expr);
      }
      const size_t expr_qual_pos = getQualifierStart(set_expr);
      if (expr_qual_pos!=std::string::npos)
      {
        //there's a qualifier -> extract part before it
        std::string objectName = set_expr.substr(0, expr_qual_pos);
        trim(objectName);
        StripEnclosingQuotes(objectName);
        if (objectName.empty())
        {
          std::cout << "ScriptCompiler: Error: no object name before qualifier "
                    << "in set expression found.\n";
          return false;
        }
        //get proper upper/ lower case spelling
        objectName = getObjectsProperID(objectName);
        //push qualifier code
        CompiledData.pushCode(CodeQualifier);
        //push length byte for object name
        CompiledData.data.push_back(objectName.length());
        //push object's ID
        CompiledData.pushString(objectName);
        //strip stuff before qualifier and qualifier itself from set expression
        set_expr.erase(0, expr_qual_pos+2);
      }//if qualifier found in expression

      //push set code
      CompiledData.pushCode(CodeSet);
      //get name of variable that will be set
      std::string varName = WorkString.substr(0, pos_of_to);
      trim(varName);
      //now search for the variable name
      SC_VarRef ref = CompiledData.getVariableTypeWithIndex(varName);
      if (ref.Type!=vtGlobal)
      {
        //push type indicator
        switch(ref.Type)
        {
          case vtShort:
               CompiledData.data.push_back('s');
               break;
          case vtLong:
               CompiledData.data.push_back('l');
               break;
          case vtFloat:
               CompiledData.data.push_back('f');
               break;
          case vtGlobal:
               //This case is just here to silence compiler warnings.
               //We should never reach that line.
               throw std::logic_error("CompileScript(): We should never reach that line.");
               break;
        }//switch
        //push index (one byte)
        CompiledData.data.push_back((ref.Index)&255);
        //fill byte
        CompiledData.data.push_back(0);
      }
      else if (Globals::get().hasRecord(varName))
      {
        //global var identifier or more complex expression found
        // So we assume global here.
        //push type indicator - G for global
        CompiledData.data.push_back('G');
        //push length of global name
        const std::string& vn_ref = Globals::get().getRecord(varName).recordID;
        CompiledData.data.push_back(vn_ref.length());
        //push name
        CompiledData.pushString(vn_ref);
      }//else if Global
      else
      {
        //could still be a foreign reference
        const std::string::size_type dot_pos = getDotPosition(varName);
        if (dot_pos==std::string::npos)
        {
          std::cout << "ScriptCompiler: Error: \""<<varName<<"\" does not name a "
                    << "local or global variable or foreign reference for SET "
                    << "statement.\n";
          return false;
        }
        std::string objectName = varName.substr(0, dot_pos);
        StripEnclosingQuotes(objectName);
        const SC_VarRef foreignRef = getForeignVariableTypeWithIndex(objectName,
                                         varName.substr(dot_pos+1));
        if (foreignRef.Type==vtGlobal)
        {
          std::cout << "ScriptCompiler: Error: could not find foreign reference in \""
                    << varName<<"\" for SET statement.\n";
          #ifdef MW_SC_DEBUG
          std::cout << "Debug: object name was \""<<objectName<<"\", var name was \""
                    << varName.substr(dot_pos+1)<<"\".\n";
          #endif
          return false;
        }//if
        //push r for reference
        CompiledData.data.push_back('r');
        //get proper object name (=ID)
        objectName = getObjectsProperID(objectName);
        //push object ID's length
        CompiledData.data.push_back(objectName.length());
        //push object ID
        CompiledData.pushString(objectName);
        //push reference
        CompiledData.pushNonGlobalRef(foreignRef);
      }
      //get stuff after keyword "to"
      //---> it's already in set_expr
      trimLeft(set_expr);
      if (set_expr.empty())
      {
        std::cout << "ScriptCompiler: Error: empty value/expression at end of "
                  << "SET statement.\n";
        return false;
      }

      // Try to "parse" the expression
      ParserNode setNode;
      if (setNode.splitToTree(set_expr, CompiledData))
      {
        //parsing was successful, get the result and push it
        const std::vector<uint8_t> binContent = setNode.getBinaryContent();
        //push length
        CompiledData.data.push_back(binContent.size());
        //push values
        CompiledData.data.insert(CompiledData.data.end(), binContent.begin(), binContent.end());
      }
      else
      {
        //Assume literal value, i.e. "123.4", "5" or something like that.
        //I know it isn't right here, but so we preserve the old behaviour.
        /* Literal: We can push the value's length +1 space byte for stack push-op.
           <length> <space> <literal as string>
           Where: <length> is the length of the data, including space
                  <space>  is a space character (' ')
                  <literal as string> - the name says it
        */
        //push length
        CompiledData.data.push_back(set_expr.length()+1);
        //push space (=stack push operator)
        CompiledData.data.push_back(' ');//space is used as push by MW's interpreter
        //push value, finally
        CompiledData.pushString(set_expr);
        //We are basically done, but issue a warning for cases where the last part
        // is not a literal value.
        std::cout << "ScriptCompiler: Warning: Set statement not completely "
                  << "implemented yet.\n";
      }
    }//if Set
    //check for choice (rarely present, but check anyway)
    else if ((lowerCase(lines.at(i).substr(0,7)) == "choice ") || (lowerCase(lines.at(i).substr(0,7)) == "choice,"))
    {
      WorkString = lines.at(i).substr(7);
      trimLeft(WorkString);
      //push code
      CompiledData.pushCode(CodeChoice);
      //push length of string as short (not byte)
      CompiledData.pushShort(WorkString.length());
      CompiledData.pushString(WorkString);
    }//if Choice found
    //check for return
    else if (lowerCase(lines.at(i)) == "return")
    {
      CompiledData.pushCode(CodeReturn);
    }//if return found
    //check for if
    else if (lowerCase(lines.at(i).substr(0,3)) == "if ")
    {
      unsigned int end_of_if = getEndOfIf(lines, i);
      if (end_of_if == i)
      {
        std::cerr << "ScriptCompiler: Error: if/elseif/endif does not match.\n";
        return false;
      }
      // Does the if-block contain more than 255 statements/lines?
      if (end_of_if - i - 1 > 255)
      {
        std::cerr << "ScriptCompiler: Error: if-block contains more than 255"
                  << " statements, it cannot be handled properly!\n";
        return false;
      }
      //first try to implement if-statement - It's still far from correct or
      //complete, but we have to start somewhere.

      //rest of line is compare statement, but this can get complicated
      WorkString = lines.at(i).substr(3);
      trim(WorkString);
      if (removeEnclosingBrackets(WorkString))
      {
        trim(WorkString);
      }
      //check left side for statement with qualifier - that gets pushed before
      //the if statement itself, if present
      SC_CompareType dummy;//not used later
      const std::string::size_type comp_pos = getComparePos(WorkString, dummy);
      const std::string::size_type qual_pos = getQualifierStart(WorkString);
      //check if qualifier is before compare statement
      if ((comp_pos != std::string::npos) && (qual_pos != std::string::npos) &&
          (qual_pos + 2 < comp_pos))
      {
        // we have a qualifier on the left side, extract part before qualifier
        std::string leftist = WorkString.substr(0, qual_pos);
        if (isSingleToken(leftist))
        {
          //remove brackets and trim afterwards
          if (removeEnclosingBrackets(leftist))
          {
            trim(leftist);
          }
          //We can push the qualifier stuff here
          //push qualifier
          CompiledData.pushCode(CodeQualifier);
          //get proper ID of object
          leftist = getObjectsProperID(leftist);
          //push length of object name
          CompiledData.data.push_back(leftist.length());
          //push object name
          CompiledData.pushString(leftist);
          //qualifier's part is done, now chop off the part that we already did
          // from the compare statement's string
          WorkString.erase(0, qual_pos+2);
        }//if single token
      }//if qualifier and that stuff

      //push if
      CompiledData.pushCode(CodeIf);
      //next is statement count (byte)
      CompiledData.data.push_back(end_of_if - i - 1);

      //process comparison statement
      std::vector<uint8_t> comparePart;
      if (CompareToBinary(WorkString, comparePart, CompiledData))
      {
        //push length of compare statement
        CompiledData.data.push_back(comparePart.size());
        //push compare statement itself
        unsigned int counter;
        for (counter=0; counter<comparePart.size(); ++counter)
        {
          CompiledData.data.push_back(comparePart[counter]);
        }//for
      }//if compare statement could be processed
      else
      {
        //no comparator found or invalid statement
        std::cout << "ScriptCompiler: Error: invalid compare part in if-statement!\n";
        return false;
      }
    }//if IF found
    //check for elseif
    else if (lowerCase(lines.at(i).substr(0,7)) == "elseif ")
    {
      unsigned int end_of_elseif = getEndOfElseIf(lines, i);
      if (end_of_elseif == i)
      {
        std::cerr << "ScriptCompiler: Error: if/elseif/endif does not match.\n";
        return false;
      }
      // Does the elseif-block contain more than 255 statements/lines?
      if (end_of_elseif - i - 1 > 255)
      {
        std::cerr << "ScriptCompiler: Error: elseif-block contains more than 255"
                  << " statements, it cannot be handled properly!\n";
        return false;
      }
      //First try to implement elseif-block - It's still far from complete, but we
      // have to start somewhere.

      //next is compare statement, but this can get complicated
      WorkString = lines.at(i).substr(7);
      trim(WorkString);
      if (removeEnclosingBrackets(WorkString))
      {
        trim(WorkString);
      }
      //Check left side for statement with qualifier - that gets pushed before
      //the elseif statement itself, if present.
      SC_CompareType dummy;//not used later
      const std::string::size_type comp_pos = getComparePos(WorkString, dummy);
      const std::string::size_type qual_pos = getQualifierStart(WorkString);
      //check if qualifier is before compare statement
      if ((comp_pos != std::string::npos) && (qual_pos != std::string::npos) &&
          (qual_pos + 2 < comp_pos))
      {
        //we have a qualifier on the left side, extract part before qualifier
        std::string leftist = WorkString.substr(0, qual_pos);
        if (isSingleToken(leftist))
        {
          //remove brackets and trim afterwards
          if (removeEnclosingBrackets(leftist))
          {
            trim(leftist);
          }
          //We can push the qualifier stuff here
          //push qualifier
          CompiledData.pushCode(CodeQualifier);
          //get proper ID of object
          leftist = getObjectsProperID(leftist);
          //push length of object name
          CompiledData.data.push_back(leftist.length());
          //push object name
          CompiledData.pushString(leftist);
          //qualifier's part is done, now chop off the part that we already did
          // from the compare statement's string
          WorkString.erase(0, qual_pos+2);
        }//if single token
      }//if qualifier and that stuff
      //push elseif
      CompiledData.pushCode(CodeElseIf);
      //next is statement count (byte)
      CompiledData.data.push_back(end_of_elseif-i-1);
      //process comparison statement
      std::vector<uint8_t> comparePart;
      if (CompareToBinary(WorkString, comparePart, CompiledData))
      {
        //push length of compare statement
        CompiledData.data.push_back(comparePart.size());
        //push compare statement itself
        unsigned int counter;
        for (counter=0; counter<comparePart.size(); ++counter)
        {
          CompiledData.data.push_back(comparePart[counter]);
        }//for
      }//if compare statement was processed successfully
      else
      {
        //no comparator found or invalid statement
        std::cout << "ScriptCompiler: Error: invalid compare part in elseif-statement!\n";
        return false;
      }
    }//if ElseIf found
    //check for else
    else if (lowerCase(lines.at(i))=="else")
    {
      unsigned int end_of_else = getEndOfElse(lines, i);
      if (end_of_else==i)
      {
        std::cout << "ScriptCompiler: Error: if/elseif/endif does not match.\n";
        return false;
      }
      //Does the else-block contain more than 255 statements/lines?
      if (end_of_else-i-1>255)
      {
        std::cout << "ScriptCompiler: Error: else-block contains more than 255"
                  << " statements, it cannot be handled properly!\n";
        return false;
      }
      //First try to implement else-block - It's still far from complete, but we
      // have to start somewhere.
      CompiledData.pushCode(CodeElse);
      //next is statement count (byte)
      CompiledData.data.push_back(end_of_else-i-1);
    }//if Else found
    //check for endif
    else if (lowerCase(lines.at(i))=="endif")
    {
      CompiledData.pushCode(CodeEndIf);
    }//if EndIf found
    //check for while loop
    else if (lowerCase(lines.at(i).substr(0,6))=="while ")
    {
      unsigned int end_of_while = getEndOfWhile(lines, i);
      if (end_of_while==i)
      {
        std::cout << "ScriptCompiler: Error: while/endwhile does not match.\n";
        return false;
      }
      //Does the while loop contain more than 255 statements/lines?
      if (end_of_while-i-1>255)
      {
        std::cout << "ScriptCompiler: Error: while loop contains more than 255"
                  << " statements, it cannot be handled properly!\n";
        return false;
      }
      //first try to implement while-statement - It's still far from correct or
      //complete, but we have to start somewhere.

      //next is compare statement, but this can get complicated
      WorkString = lines.at(i).substr(6);
      trim(WorkString);
      if (removeEnclosingBrackets(WorkString))
      {
        trim(WorkString);
      }
      //Check left side for statement with qualifier - that gets pushed before
      //the while statement itself, if present.
      SC_CompareType dummy;//not used later
      const std::string::size_type comp_pos = getComparePos(WorkString, dummy);
      const std::string::size_type qual_pos = getQualifierStart(WorkString);
      //check if qualifier is before compare statement
      if ((comp_pos != std::string::npos) && (qual_pos != std::string::npos) &&
          (qual_pos + 2 < comp_pos))
      {
        //we have a qualifier on the left side, extract part before qualifier
        std::string leftist = WorkString.substr(0, qual_pos);
        if (isSingleToken(leftist))
        {
          //remove brackets and trim afterwards
          if (removeEnclosingBrackets(leftist))
          {
            trim(leftist);
          }
          //We can push the qualifier stuff here.
          //push qualifier
          CompiledData.pushCode(CodeQualifier);
          //get object's proper ID
          leftist = getObjectsProperID(leftist);
          //push length of object name
          CompiledData.data.push_back(leftist.length());
          //push object name
          CompiledData.pushString(leftist);
          //qualifier's part is done, now chop off the part that we already did
          // from the compare statement's string
          WorkString.erase(0, qual_pos+2);
        }//if single token
      }//if qualifier and that stuff

      //push While
      CompiledData.pushCode(CodeWhile);
      //next is statement count (byte)
      /*******
       * TODO:
       *
       * -Statement count is one too much for nested while, as far as I can see.
       *  I guess, MW does not count either while or endwhile as a statement.
       *
       *******/
      CompiledData.data.push_back(end_of_while-i-1);
      //process comparison statement
      std::vector<uint8_t> comparePart;
      if (CompareToBinary(WorkString, comparePart, CompiledData))
      {
        //push length of compare statement
        CompiledData.data.push_back(comparePart.size());
        //push compare statement itself
        unsigned int counter;
        for (counter=0; counter<comparePart.size(); ++counter)
        {
          CompiledData.data.push_back(comparePart[counter]);
        }//for
      }//if compare statement was processed successfully
      else
      {
        //no comparator found or invalid statement
        std::cout << "ScriptCompiler: Error: invalid compare part in while-statement!\n";
        return false;
      }
    }//if While found
    //check for endWhile
    else if (lowerCase(lines.at(i))=="endwhile")
    {
      CompiledData.pushCode(CodeEndWhile);
    }//if EndWhile found
    //check for end of script
    else if ((lowerCase(lines.at(i)) == "end") || (lowerCase(lines.at(i).substr(0,4)) == "end ")
             || (lowerCase(lines.at(i).substr(0,4)) == "end\t"))
    {
      CompiledData.pushCode(CodeEnd);
      EndLineFound = true;
      //this should be the end of the script - if not, there's junk ahead.
      if (i+1<lines.size())
      {
        std::cout << "ScriptCompiler: Warning: there is more code after end clause.\n";
        i = lines.size();
        break;
      }
    }//if End found
    //check for arrow (qualifier)
    else if (qualStart!=std::string::npos)
    {
      //part before qualifier should be object name
      WorkString = lines.at(i).substr(0, qualStart);
      StripEnclosingQuotes(WorkString);
      if (WorkString.empty() || (qualStart + 2 >= lines[i].length()))
      {
        std::cout << "ScriptCompiler: Error: invalid position of -> encountered.\n"
                  << "String is \"" << WorkString << "\", pos. of qual. is "
                  << qualStart << " and line's length is " << lines[i].length()
                  <<".\nThe line itself is \"" << lines[i] << "\".\n";
        return false;
      }
      //push qualifier code
      CompiledData.pushCode(CodeQualifier);
      //get proper object ID
      WorkString = getObjectsProperID(WorkString);
      //push length of object name
      CompiledData.data.push_back(WorkString.length());
      //push object name
      CompiledData.pushString(WorkString);
      //Now the rest of the expression after -> should be a function. If not,
      // return false/error.
      if (!ScriptFunctions(lines[i].substr(qualStart+2) , CompiledData, false))
      {
        std::cout << "ScriptCompiler: Error: could not handle part after "
                  << "qualifier in line \"" << lines[i] << "\".\n";
        return false;
      }
    }//if qualifier found
    //check for functions
    else if (ScriptFunctions(lines.at(i), CompiledData, false))
    {
      //empty
      //std::cout << "Debug: ScriptCompiler: Hint: Function processed.\n";
    }//functions
    else
    {
      //None of the above possibilities did match, so this is an invalid line
      // or something we can't handle yet.
      std::cout << "ScriptCompiler: Error: could not process line \""
                << lines.at(i) <<"\".\n";
      return false;
    }
  }//for

  if (!EndLineFound)
  {
    std::cout << "ScriptCompiler: Error: there is no End line in the script!\n";
    return false;
  }


  /***** copy the data into result *****/
  //set result values
  result.NumShorts = CompiledData.varsShort.size();
  result.NumLongs = CompiledData.varsLong.size();
  result.NumFloats = CompiledData.varsFloat.size();
  //variables
  result.LocalVars.clear();
  // ---- write shorts
  for (i=0; i<CompiledData.varsShort.size(); ++i)
  {
    result.LocalVars.push_back(CompiledData.varsShort.at(i));
  }//for
  // ---- write longs
  for (i=0; i<CompiledData.varsLong.size(); ++i)
  {
    result.LocalVars.push_back(CompiledData.varsLong.at(i));
  }//for
  // ---- write floats
  for (i=0; i<CompiledData.varsFloat.size(); ++i)
  {
    result.LocalVars.push_back(CompiledData.varsFloat.at(i));
  }//for

  //script's ID
  result.recordID = ScriptName;
  //text
  result.ScriptText = Text;
  // script data
  result.ScriptData.copy_from(CompiledData.data.data(), CompiledData.data.size());

  #ifdef MW_SC_DEBUG
  std::cout << "ScriptCompiler: Warning: compiler is still far from complete! "
            << "The resulting script might not run at all or not as intended "
            << "within the game!\n";
  #endif
  return true;
}

} //namespace ScriptCompiler

} //namespace MWTP
