/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ScriptCompiler.h"
#include <iostream>
#include "CompilerCodes.h"

namespace ScriptCompiler
{

void CompiledChunk::pushCode(const uint16_t code)
{
  data.push_back(code&255);
  data.push_back(code>>8);
}

void CompiledChunk::pushShort(const int16_t value)
{
  data.push_back(value&255);
  data.push_back(value>>8);
}

void CompiledChunk::pushLong(const int32_t value)
{
  data.push_back(value&255);
  data.push_back((value>>8)&255);
  data.push_back((value>>16)&255);
  data.push_back(value>>24);
}

void CompiledChunk::pushFloat(const float value)
{
  const int32_t * ptr = (int32_t*) &value;
  data.push_back(*ptr & 255);
  data.push_back((*ptr>>8)&255);
  data.push_back((*ptr>>16)&255);
  data.push_back((*ptr>>24)&255);
}

void CompiledChunk::pushString(const std::string& str)
{
  unsigned int i;
  const unsigned int len = str.length();
  for (i=0; i<len; ++i)
  {
    data.push_back(str[i]);
  }//for
}

SC_VarRef::SC_VarRef(const SC_VarType t, const uint16_t i)
{
  Type = t;
  Index = i;
}

//tries to get the integer representation of a string
bool stringToShort(const std::string& str, int16_t& value)
{
  if (str.length()==0) return false;
  value = 0;
  unsigned int i;
  bool negative;
  if (str.at(0)=='-')
  {
    i=1;
    negative = true;
  }
  else
  {
    i=0;
    negative = false;
  }
  for ( ; i<str.length(); ++i)
  {
    if ((str.at(i)>='0') and (str.at(i)<='9'))
    {
      value = value * 10;
      value = value + (str.at(i)-'0');
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  if (negative) value = -value;
  return true;
}

bool stringToLong(const std::string& str, int32_t& value)
{
  if (str.length()==0) return false;
  value = 0;
  unsigned int i;
  bool negative;
  if (str.at(0)=='-')
  {
    i=1;
    negative = true;
  }
  else
  {
    i=0;
    negative = false;
  }
  for ( ; i<str.length(); ++i)
  {
    if ((str.at(i)>='0') and (str.at(i)<='9'))
    {
      value = value * 10;
      value = value + (str.at(i)-'0');
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  if (negative) value = -value;
  return true;
}

//tries to get the floating point representation of a string
bool stringToFloat(const std::string& str, float& value)
{
  if (str.length()==0) return false;
  value = 0.0f;
  unsigned int i, next_look;
  bool negative;
  if (str.at(0)=='-')
  {
    i=1;
    negative = true;
    next_look = 1;
  }
  else
  {
    i=0;
    negative = false;
    next_look = 0;
  }
  for ( ; i<str.length(); ++i)
  {
    if ((str.at(i)>='0') and (str.at(i)<='9'))
    {
      value = value * 10.0f;
      value = value + (str.at(i)-'0');
      ++next_look;
    }//if
    else if (str.at(i)=='.')
    {
      //decimal separator found - break out of loop
      next_look = i+1;
      break;
    }
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  //now go for the stuff after the separator
  float second = 0.0f;
  for (i=str.length()-1; i>=next_look; --i)
  {
    if ((str.at(i)>='0') and (str.at(i)<='9'))
    {
      second = second + (str.at(i)-'0');
      second = second / 10.0f;
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for, second loop
  value = value + second;
  if (negative) value = -value;
  return true;
}


void trimLeft(std::string& str1)
{
  if (str1=="") return;
  //trim stuff at begin
  int32_t len = str1.length();
  int32_t pos = 0;
  bool go_on = true;
  while (go_on)
  {
    char& ch = str1.at(pos);
    if ((ch==' ') or (ch=='\t'))
    {
      ++pos;
      go_on = (pos<len);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos>=len)
  {
    str1 = "";
    return;
  }
  else if (pos>0)
  {
    str1.erase(0, pos);
  }
  return;
}

void trimRight(std::string& str1)
{
  if (str1=="") return;
  //trim stuff at end
  int32_t len = str1.length();
  int32_t pos = len-1;
  bool go_on = true;
  while (go_on)
  {
    char& ch = str1.at(pos);
    if ((ch==' ') or (ch=='\t'))
    {
      --pos;
      go_on = (pos>=0);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos==-1)
  {
    str1 = "";
  }
  else if (pos<len-1)
  {
    str1.erase(pos+1);
  }
  return;
}

void trim(std::string& str1)
{
  trimLeft(str1);
  trimRight(str1);
  return;
} //end of trim

void StripEnclosingQuotes(std::string& str1)
{
  if ((str1=="") or (str1.length()<2)) return;
  if ((str1.at(0)=='"') and (str1.at(str1.length()-1)=='"'))
  {
    str1 = str1.substr(1, str1.length()-2);
  }
  return;
}

std::vector<std::string> explodeParams(const std::string& source)
{
  std::vector<std::string> result;
  if (source.empty()) return result;

  const unsigned int len = source.length();
  unsigned int look = 0;
  unsigned int offset = 0;
  bool insideQuote = false;
  while (look<len)
  {
    if (source.at(look)=='"')
    {
      insideQuote = not insideQuote;
    }
    else if ((not insideQuote) and ((source.at(look)==' ') or (source.at(look)==',')))
    {
      //found a place where to split
      unsigned int len = look-offset;
      if (len>0)//skip empty params
      {
        result.push_back(source.substr(offset, len));
        StripEnclosingQuotes(result.back());
      }//if
      offset=look+1;
    }//else
    ++look;
  }//while
  //add the rest, if not finished yet
  if (offset<len)
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

std::string lowerCase(const std::string& str1)
{
  /* We assume ASCII or compatible charset where the characters for letters are
     in sequence.
  */
  const char lowerDiff = 'a'-'A';
  std::string result = str1;
  unsigned int i;
  for (i=0; i<str1.length(); ++i)
  {
    if ((result[i]<='Z') and (result[i]>='A'))
    {
      result[i] = result[i]+ lowerDiff;
    }
  }//for
  return result;
}

SC_VarRef getVariableTypeWithIndex(const std::string& varName, const std::vector<std::string>& vShorts,
                  const std::vector<std::string>& vLongs, const std::vector<std::string>& vFloats)
{
  const std::string lcVar = lowerCase(varName);
  unsigned int i;
  //check list of shorts
  for (i=0; i<vShorts.size(); ++i)
  {
    if (lcVar==lowerCase(vShorts.at(i)))
    {
      return SC_VarRef(vtShort, i+1);
    }
  }//for
  //check list of longs
  for (i=0; i<vLongs.size(); ++i)
  {
    if (lcVar==lowerCase(vLongs.at(i)))
    {
      return SC_VarRef(vtLong, i+1);
    }
  }//for
  //check list of floats
  for (i=0; i<vFloats.size(); ++i)
  {
    if (lcVar==lowerCase(vFloats.at(i)))
    {
      return SC_VarRef(vtFloat, i+1);
    }
  }//for
  //if we get to this point, nothing was found and it has to be a non-local var,
  // i.e. a global (or some other kind of expression we can't identify yet).
  return SC_VarRef(vtGlobal, 0);
}

bool ScriptFunctions_ZeroParameters(const std::string& line, CompiledChunk& chunk)
{
  const std::string lowerLine = lowerCase(line);
  if (lowerLine=="activate")
  {
    chunk.pushCode(CodeActivate);
    return true;
  }
  if (lowerLine=="becomewerewolf")
  {
    chunk.pushCode(CodeBecomeWerewolf);
    return true;
  }
  if (lowerLine=="cellchanged")
  {
    chunk.pushCode(CodeCellChanged);
    return true;
  }
  if (lowerLine=="clearforcejump")
  {
    chunk.pushCode(CodeClearForceJump);
    return true;
  }
  if (lowerLine=="clearforcemovejump")
  {
    chunk.pushCode(CodeClearForceMoveJump);
    return true;
  }
  if (lowerLine=="clearforcerun")
  {
    chunk.pushCode(CodeClearForceRun);
    return true;
  }
  if (lowerLine=="clearforcesneak")
  {
    chunk.pushCode(CodeClearForceSneak);
    return true;
  }
  if (lowerLine=="disable")
  {
    chunk.pushCode(CodeDisable);
    return true;
  }
  if (lowerLine=="disablelevitation")
  {
    chunk.pushCode(CodeDisableLevitation);
    return true;
  }
  if (lowerLine=="disableplayercontrols")
  {
    chunk.pushCode(CodeDisablePlayerControls);
    return true;
  }
  if (lowerLine=="disableplayerfighting")
  {
    chunk.pushCode(CodeDisablePlayerFighting);
    return true;
  }
  if (lowerLine=="disableplayerjumping")
  {
    chunk.pushCode(CodeDisablePlayerJumping);
    return true;
  }
  if (lowerLine=="disableplayerlooking")
  {
    chunk.pushCode(CodeDisablePlayerLooking);
    return true;
  }
  if (lowerLine=="disableplayermagic")
  {
    chunk.pushCode(CodeDisablePlayerMagic);
    return true;
  }
  if (lowerLine=="disableplayerviewswitch")
  {
    chunk.pushCode(CodeDisablePlayerViewSwitch);
    return true;
  }
  if (lowerLine=="disableteleporting")
  {
    chunk.pushCode(CodeDisableTeleporting);
    return true;
  }
  if (lowerLine=="disablevanitymode")
  {
    chunk.pushCode(CodeDisableVanityMode);
    return true;
  }
  if (lowerLine=="dontsaveobject")
  {
    chunk.pushCode(CodeDontSaveObject);
    return true;
  }
  if (lowerLine=="enable")
  {
    chunk.pushCode(CodeEnable);
    return true;
  }
  if (lowerLine=="enablebirthmenu")
  {
    chunk.pushCode(CodeEnableBirthMenu);
    return true;
  }
  if (lowerLine=="enableclassmenu")
  {
    chunk.pushCode(CodeEnableClassMenu);
    return true;
  }
  if (lowerLine=="enableinventorymenu")
  {
    chunk.pushCode(CodeEnableInventoryMenu);
    return true;
  }
  if (lowerLine=="enablelevelupmenu")
  {
    chunk.pushCode(CodeEnableLevelUpMenu);
    return true;
  }
  if (lowerLine=="enablelevitation")
  {
    chunk.pushCode(CodeEnableLevitation);
    return true;
  }
  if (lowerLine=="enablemagicmenu")
  {
    chunk.pushCode(CodeEnableMagicMenu);
    return true;
  }
  if (lowerLine=="enablemapmenu")
  {
    chunk.pushCode(CodeEnableMapMenu);
    return true;
  }
  if (lowerLine=="enablenamemenu")
  {
    chunk.pushCode(CodeEnableNameMenu);
    return true;
  }
  if (lowerLine=="enableplayercontrols")
  {
    chunk.pushCode(CodeEnablePlayerControls);
    return true;
  }
  if (lowerLine=="enableplayerfighting")
  {
    chunk.pushCode(CodeEnablePlayerFighting);
    return true;
  }
  if (lowerLine=="enableplayerjumping")
  {
    chunk.pushCode(CodeEnablePlayerJumping);
    return true;
  }
  if (lowerLine=="enableplayerlooking")
  {
    chunk.pushCode(CodeEnablePlayerLooking);
    return true;
  }
  if (lowerLine=="enableplayermagic")
  {
    chunk.pushCode(CodeEnablePlayerMagic);
    return true;
  }
  if (lowerLine=="enableplayerviewswitch")
  {
    chunk.pushCode(CodeEnablePlayerViewSwitch);
    return true;
  }
  if (lowerLine=="enableracemenu")
  {
    chunk.pushCode(CodeEnableRaceMenu);
    return true;
  }
  if (lowerLine=="enablerest")
  {
    chunk.pushCode(CodeEnableRest);
    return true;
  }
  if (lowerLine=="enablestatreviewmenu")
  {
    chunk.pushCode(CodeEnableStatReviewMenu);
    return true;
  }
  if (lowerLine=="enablestatsmenu")
  {
    chunk.pushCode(CodeEnableStatsMenu);
    return true;
  }
  if (lowerLine=="enableteleporting")
  {
    chunk.pushCode(CodeEnableTeleporting);
    return true;
  }
  if (lowerLine=="enablevanitymode")
  {
    chunk.pushCode(CodeEnableVanityMode);
    return true;
  }
  if (lowerLine=="fall")
  {
    chunk.pushCode(CodeFall);
    return true;
  }
  if (lowerLine=="forcegreeting")
  {
    chunk.pushCode(CodeForceGreeting);
    return true;
  }
  if (lowerLine=="forcejump")
  {
    chunk.pushCode(CodeForceJump);
    return true;
  }
  if (lowerLine=="forcemovejump")
  {
    chunk.pushCode(CodeForceMoveJump);
    return true;
  }
  if (lowerLine=="forcerun")
  {
    chunk.pushCode(CodeForceRun);
    return true;
  }

  if (lowerLine=="forcesneak")
  {
    chunk.pushCode(CodeForceSneak);
    return true;
  }
  if ((lowerLine.substr(0,3)=="get") and (line.length()>3))
  {
    if (lowerLine.at(3)=='a')
    {
      if (lowerLine=="getacrobatics")
      {
        chunk.pushCode(CodeGetAcrobatics);
        return true;
      }
      if (lowerLine=="getagility")
      {
        chunk.pushCode(CodeGetAgility);
        return true;
      }
      if (lowerLine=="getalarm")
      {
        chunk.pushCode(CodeGetAlarm);
        return true;
      }
      if (lowerLine=="getalchemy")
      {
        chunk.pushCode(CodeGetAlchemy);
        return true;
      }
      if (lowerLine=="getalteration")
      {
        chunk.pushCode(CodeGetAlteration);
        return true;
      }
      if (lowerLine=="getarmorbonus")
      {
        chunk.pushCode(CodeGetArmorBonus);
        return true;
      }
      if (lowerLine=="getarmorer")
      {
        chunk.pushCode(CodeGetArmorer);
        return true;
      }
      if (lowerLine=="getathletics")
      {
        chunk.pushCode(CodeGetAthletics);
        return true;
      }
      if (lowerLine=="getatttackbonus")
      {
        chunk.pushCode(CodeGetAttackBonus);
        return true;
      }
      if (lowerLine=="getaxe")
      {
        chunk.pushCode(CodeGetAxe);
        return true;
      }
      return false;//all zero argument functions with "GetA..." done
    }//if fourth character is 'a'
    if (lowerLine=="getblindness")
    {
      chunk.pushCode(CodeGetBlindness);
      return true;
    }
    if (lowerLine=="getblock")
    {
      chunk.pushCode(CodeGetBlock);
      return true;
    }
    if (lowerLine=="getbluntweapon")
    {
      chunk.pushCode(CodeGetBluntWeapon);
      return true;
    }
    if (lowerLine=="getcastpenalty")
    {
      chunk.pushCode(CodeGetCastPenalty);
      return true;
    }
    if (lowerLine=="getchameleon")
    {
      chunk.pushCode(CodeGetChameleon);
      return true;
    }
    if (lowerLine=="getconjuration")
    {
      chunk.pushCode(CodeGetConjuration);
      return true;
    }
    if (lowerLine=="getdefendbonus")
    {
      chunk.pushCode(CodeGetDefendBonus);
      return true;
    }
    if (lowerLine=="getdestruction")
    {
      chunk.pushCode(CodeGetDestruction);
      return true;
    }
    if (lowerLine=="getdisposition")
    {
      chunk.pushCode(CodeGetDisposition);
      return true;
    }
    if (lowerLine=="getenchant")
    {
      chunk.pushCode(CodeGetEnchant);
      return true;
    }
    if (lowerLine=="getendurance")
    {
      chunk.pushCode(CodeGetEndurance);
      return true;
    }
    if (lowerLine=="getfatigue")
    {
      chunk.pushCode(CodeGetFatigue);
      return true;
    }
    if (lowerLine=="getfight")
    {
      chunk.pushCode(CodeGetFight);
      return true;
    }
    if (lowerLine=="getflee")
    {
      chunk.pushCode(CodeGetFlee);
      return true;
    }
    if (lowerLine=="getflying")
    {
      chunk.pushCode(CodeGetFlying);
      return true;
    }
    if (lowerLine=="gethandtohand")
    {
      chunk.pushCode(CodeGetHandToHand);
      return true;
    }
    if (lowerLine=="gethealth")
    {
      chunk.pushCode(CodeGetHealth);
      return true;
    }
    if (lowerLine=="getheavyarmor")
    {
      chunk.pushCode(CodeGetHeavyArmor);
      return true;
    }
    if (lowerLine=="gethello")
    {
      chunk.pushCode(CodeGetHello);
      return true;
    }
    if (lowerLine=="getillusion")
    {
      chunk.pushCode(CodeGetIllusion);
      return true;
    }
    if (lowerLine=="getintelligence")
    {
      chunk.pushCode(CodeGetIntelligence);
      return true;
    }
    //check for both correct and earlier misspelled version of function name
    if ((lowerLine=="getinvisible") or (lowerLine=="getinvisibile"))
    {
      chunk.pushCode(CodeGetInvisible);
      return true;
    }
    if (lowerLine=="getlevel")
    {
      chunk.pushCode(CodeGetLevel);
      return true;
    }
    if (lowerLine=="getlightarmor")
    {
      chunk.pushCode(CodeGetLightArmor);
      return true;
    }
    if (lowerLine=="getlongblade")
    {
      chunk.pushCode(CodeGetLongBlade);
      return true;
    }
    if (lowerLine=="getluck")
    {
      chunk.pushCode(CodeGetLuck);
      return true;
    }
    if (lowerLine=="getmagicka")
    {
      chunk.pushCode(CodeGetMagicka);
      return true;
    }
    if (lowerLine=="getmarksman")
    {
      chunk.pushCode(CodeGetMarksman);
      return true;
    }
    if (lowerLine=="getmediumarmor")
    {
      chunk.pushCode(CodeGetMediumArmor);
      return true;
    }
    if (lowerLine=="getmercantile")
    {
      chunk.pushCode(CodeGetMercantile);
      return true;
    }
    if (lowerLine=="getmysticism")
    {
      chunk.pushCode(CodeGetMysticism);
      return true;
    }
    if (lowerLine=="getparalysis")
    {
      chunk.pushCode(CodeGetParalysis);
      return true;
    }
    if (lowerLine=="getpccrimelevel")
    {
      chunk.pushCode(CodeGetPCCrimeLevel);
      return true;
    }
    if (lowerLine=="getpcvisionbonus")
    {
      chunk.pushCode(CodeGetPCVisionBonus);
      return true;
    }
    if (lowerLine=="getpersonality")
    {
      chunk.pushCode(CodeGetPersonality);
      return true;
    }
    if (lowerLine=="getreputation")
    {
      chunk.pushCode(CodeGetReputation);
      return true;
    }
    if (lowerLine=="getresistblight")
    {
      chunk.pushCode(CodeGetResistBlight);
      return true;
    }
    if (lowerLine=="getresistcorprus")
    {
      chunk.pushCode(CodeGetResistCorprus);
      return true;
    }
    if (lowerLine=="getresistdisease")
    {
      chunk.pushCode(CodeGetResistDisease);
      return true;
    }
    if (lowerLine=="getresistfire")
    {
      chunk.pushCode(CodeGetResistFire);
      return true;
    }
    if (lowerLine=="getresistfrost")
    {
      chunk.pushCode(CodeGetResistFrost);
      return true;
    }
    if (lowerLine=="getresistmagicka")
    {
      chunk.pushCode(CodeGetResistMagicka);
      return true;
    }
    if (lowerLine=="getresistnormalweapons")
    {
      chunk.pushCode(CodeGetResistNormalWeapons);
      return true;
    }
    if (lowerLine=="getresistparalysis")
    {
      chunk.pushCode(CodeGetResistParalysis);
      return true;
    }
    if (lowerLine=="getresistpoison")
    {
      chunk.pushCode(CodeGetResistPoison);
      return true;
    }
    if (lowerLine=="getresistshock")
    {
      chunk.pushCode(CodeGetResistShock);
      return true;
    }
    if (lowerLine=="getrestoration")
    {
      chunk.pushCode(CodeGetRestoration);
      return true;
    }
    if (lowerLine=="getscale")
    {
      chunk.pushCode(CodeGetScale);
      return true;
    }
    if (lowerLine=="getsecurity")
    {
      chunk.pushCode(CodeGetSecurity);
      return true;
    }
    if (lowerLine=="getshortblade")
    {
      chunk.pushCode(CodeGetShortBlade);
      return true;
    }
    if (lowerLine=="getsilence")
    {
      chunk.pushCode(CodeGetSilence);
      return true;
    }
    if (lowerLine=="getsneak")
    {
      chunk.pushCode(CodeGetSneak);
      return true;
    }
    if (lowerLine=="getspear")
    {
      chunk.pushCode(CodeGetSpear);
      return true;
    }
    if (lowerLine=="getspeechcraft")
    {
      chunk.pushCode(CodeGetSpeechcraft);
      return true;
    }
    if (lowerLine=="getspeed")
    {
      chunk.pushCode(CodeGetSpeed);
      return true;
    }
    if (lowerLine=="getstrength")
    {
      chunk.pushCode(CodeGetStrength);
      return true;
    }
    if (lowerLine=="getsuperjump")
    {
      chunk.pushCode(CodeGetSuperJump);
      return true;
    }
    if (lowerLine=="getswimspeed")
    {
      chunk.pushCode(CodeGetSwimSpeed);
      return true;
    }
    if (lowerLine=="getunarmored")
    {
      chunk.pushCode(CodeGetUnarmored);
      return true;
    }
    if (lowerLine=="getwaterbreathing")
    {
      chunk.pushCode(CodeGetWaterBreathing);
      return true;
    }
    if (lowerLine=="getwaterwalking")
    {
      chunk.pushCode(CodeGetWaterWalking);
      return true;
    }
    if (lowerLine=="getwillpower")
    {
      chunk.pushCode(CodeGetWillpower);
      return true;
    }
  }//get functions
  return false;
}

bool ScriptFunctions_OneParameter(const std::string& line, CompiledChunk& chunk)
{
  const std::string lowerLine = lowerCase(line);
  if (lowerLine.substr(0,8) == "addspell")
  {
    std::vector<std::string> params = explodeParams(line.substr(8));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: AddSpell needs one parameter!\n";
      return false;
    }
    //parameter is ID of spell
    chunk.pushCode(CodeAddSpell);
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push ID
    chunk.pushString(params[0]);
    return true;
  }
  if (lowerLine.substr(0,8) == "addtopic")
  {
    std::vector<std::string> params = explodeParams(line.substr(8));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: AddTopic needs one parameter!\n";
      return false;
    }
    //parameter is ID of topic
    chunk.pushCode(CodeAddTopic);
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push ID
    chunk.pushString(params[0]);
    return true;
  }
  if (lowerLine.substr(0,11) == "dropsoulgem")
  {
    std::vector<std::string> params = explodeParams(line.substr(11));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: DropSoulGem needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeDropSoulGem);
    //parameter is ID of creature
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push creature ID
    chunk.pushString(params[0]);
    return true;
  }
  if (lowerLine.substr(0,5) == "equip")
  {
    std::vector<std::string> params = explodeParams(line.substr(5));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: Equip needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeEquip);
    //parameter is ID of item
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push item ID
    chunk.pushString(params[0]);
    return true;
  }
  if (lowerLine.substr(0,12) == "explodespell")
  {
    std::vector<std::string> params = explodeParams(line.substr(12));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: ExplodeSpell needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeExplodeSpell);
    //parameter is ID of spell
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push spell ID
    chunk.pushString(params[0]);
    return true;
  }
  if (lowerLine.substr(0,6) == "fadein")
  {
    std::vector<std::string> params = explodeParams(line.substr(6));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: FadeIn needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeFadeIn);
    //parameter is time for fading (float)
    //push float
    float fade_time;
    if (stringToFloat(params[0], fade_time))
    {
      chunk.pushFloat(fade_time);
    }//if
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[0]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }
    return true;
  }
  if (lowerLine.substr(0,7) == "fadeout")
  {
    std::vector<std::string> params = explodeParams(line.substr(7));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: FadeOut needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeFadeOut);
    //parameter is time for fading (float)
    //push float
    float fade_time;
    if (stringToFloat(params[0], fade_time))
    {
      chunk.pushFloat(fade_time);
    }//if
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[0]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }
    return true;
  }

  //no match found
  return false;
}

bool ScriptFunctions_TwoParameters(const std::string& line, CompiledChunk& chunk)
{
  const std::string lowerLine = lowerCase(line);
  if (lowerLine.substr(0,7) == "additem")
  {
    std::vector<std::string> params = explodeParams(line.substr(7));
    if (params.size()!=2)
    {
      std::cout << "ScriptCompiler: Error: AddItem needs two parameters!\n";
      return false;
    }
    //first parameter is ID of item, second is number (short)
    chunk.pushCode(CodeAddItem);
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push ID
    chunk.pushString(params[0]);
    //push count
    int16_t count;
    if (stringToShort(params[1], count))
    {
      chunk.pushShort(count);
    }
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no short value!\n";
      return false;
    }
    return true;
  }
  if (lowerLine.substr(0,10) == "addsoulgem")
  {
    std::vector<std::string> params = explodeParams(line.substr(10));
    if (params.size()!=2)
    {
      std::cout << "ScriptCompiler: Error: AddSoulGem needs two parameters!\n";
      return false;
    }
    //first parameter is ID of creature, second is ID of gem
    chunk.pushCode(CodeAddSoulGem);
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push ID
    chunk.pushString(params[0]);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }
  if (lowerLine.substr(0,4) == "cast")
  {
    std::vector<std::string> params = explodeParams(line.substr(4));
    if (params.size()!=2)
    {
      std::cout << "ScriptCompiler: Error: Cast needs two parameters!\n";
      return false;
    }
    //first parameter is ID of spell, second is ID of target
    chunk.pushCode(CodeCast);
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push ID
    chunk.pushString(params[0]);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    return true;
  }
  if (lowerLine.substr(0,13) == "changeweather")
  {
    std::vector<std::string> params = explodeParams(line.substr(13));
    if (params.size()!=2)
    {
      std::cout << "ScriptCompiler: Error: ChangeWeather needs two parameters!\n";
      return false;
    }
    //first parameter is ID of region, second is short that indicates the new weather
    chunk.pushCode(CodeChangeWeather);
    //push ID's length
    chunk.data.push_back(params[0].length());
    //push ID
    chunk.pushString(params[0]);
    //push new weather type
    int16_t weather_type;
    if (stringToShort(params[1], weather_type))
    {
      chunk.pushShort(weather_type);
    }
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no short value!\n";
      return false;
    }
    return true;
  }
  if ((lowerLine.substr(0,4) == "drop") and ((lowerLine.at(4)==' ') or (lowerLine.at(4)==',')))
  {
    std::vector<std::string> params = explodeParams(line.substr(5));
    if (params.size()!=2)
    {
      std::cout << "ScriptCompiler: Error: Drop needs two parameters!\n";
      return false;
    }
    //first parameter is ID of item, second is short that indicates the count
    chunk.pushCode(CodeDrop);
    //push item ID's length
    chunk.data.push_back(params[0].length());
    //push item ID
    chunk.pushString(params[0]);
    //push count
    int16_t count;
    if (stringToShort(params[1], count))
    {
      chunk.pushShort(count);
    }
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is no short value!\n";
      return false;
    }
    return true;
  }
  if (lowerLine.substr(0,6) == "fadeto")
  {
    std::vector<std::string> params = explodeParams(line.substr(6));
    if (params.size()!=1)
    {
      std::cout << "ScriptCompiler: Error: FadeTo needs two parameters!\n";
      return false;
    }
    chunk.pushCode(CodeFadeTo);
    //first parameter is amount of fading (long?)
    //push long
    int32_t fade_amount;
    if (stringToLong(params[0], fade_amount))
    {
      chunk.pushLong(fade_amount);
    }//if
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[0]<<"\" is not a "
                << "long value.\n";
      return false;
    }
    //second parameter is time for fading (float)
    //push float
    float fade_time;
    if (stringToFloat(params[1], fade_time))
    {
      chunk.pushFloat(fade_time);
    }//if
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "floating point value.\n";
      return false;
    }
    return true;
  }



  //end - no matching function found, if we are here
  return false;
}

bool ScriptFunctions(const std::string& line, CompiledChunk& chunk)
{
  if (line=="") return false;
  if (ScriptFunctions_ZeroParameters(line, chunk))
  {
    return true;
  }
  if (ScriptFunctions_OneParameter(line, chunk))
  {
    return true;
  }
  return ScriptFunctions_TwoParameters(line, chunk);
}

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

    // -> remove any commments, if present (comments start with ';')
    pos = new_line.find(";");
    if (pos!=std::string::npos)
    {
      //remove comment - everything after ";" is a comment
      new_line.erase(pos);
    }//if comment found
    // -> remove \r characters at the end, if present
    if (new_line.at(new_line.length()-1)=='\r')
    {
      new_line.erase(new_line.length()-1);
    }
    //cut off spaces and tabulators
    trim(new_line);
    if (new_line!="")
    {
      lines.push_back(new_line);
    }
  }//while

  std::vector<std::string> varsShort;
  std::vector<std::string> varsLong;
  std::vector<std::string> varsFloat;
  std::string ScriptName = "";
  std::string WorkString;

  CompiledChunk CompiledData;

  //now check the lines of the script
  //first line should always be begin
  if (lowerCase(lines.at(0).substr(0,6))=="begin ")
  {
    ScriptName = lines.at(0).substr(6);
    trimLeft(ScriptName);//We only trim left side, because right side was
                         //already trimmed for original string.
    if (ScriptName=="")
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
    //check for shorts
    if (lowerCase(lines.at(i).substr(0,6))=="short ")
    {
      WorkString = lines.at(i).substr(6);
      trim(WorkString);
      if (WorkString=="")
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of short found!\n";
        return false;
      }
      varsShort.push_back(WorkString);
    }//if short
    //check for longs
    else if (lowerCase(lines.at(i).substr(0,5))=="long ")
    {
      WorkString = lines.at(i).substr(5);
      trim(WorkString);
      if (WorkString=="")
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of long found!\n";
        return false;
      }
      varsLong.push_back(WorkString);
    }//if long
    //check for floats
    else if (lowerCase(lines.at(i).substr(0,6))=="float ")
    {
      WorkString = lines.at(i).substr(6);
      trim(WorkString);
      if (WorkString=="")
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of float found!\n";
        return false;
      }
      varsFloat.push_back(WorkString);
    }//if float
    //check for Journal
    else if (lowerCase(lines.at(i).substr(0,8))=="journal ")
    {
      CompiledData.pushCode(CodeJournal);
      WorkString = lines.at(i).substr(8);
      trimLeft(WorkString);
      std::vector<std::string> params = explodeParams(WorkString);
      if (params.size() != 2)
      {
        std::cout << "ScriptCompiler: Error: Journal command expects two params"
                  << ", not "<<params.size()<<".\n";
        return false;
      }
      StripEnclosingQuotes(params[0]);
      //push journal ID's length
      CompiledData.data.push_back(params[0].length());
      CompiledData.pushString(params[0]);
      //second parameter should be index
      int16_t journal_index;
      if (stringToShort(params[1], journal_index))
      {
        CompiledData.pushShort(journal_index);
      }
      else
      {
        std::cout << "ScriptCompiler: Error: Journal command expects short value"
                  << " as second parameter, but \""<<params[1]<<"\" is not a "
                  << "short value.\n";
        return false;
      }
      //fill data so it's four bytes
      CompiledData.data.push_back(255);
      CompiledData.data.push_back(255);
    }//if Journal
    //check for Set
    else if (lowerCase(lines.at(i).substr(0,4))=="set ")
    {
      CompiledData.pushCode(CodeSet);
      WorkString = lines.at(i).substr(4);
      trimLeft(WorkString);
      WorkString = lowerCase(WorkString);
      //now select the bits of "set variable To value/expression"
      const std::string::size_type pos = WorkString.find(" to ");
      if (pos==std::string::npos)
      {
        std::cout << "ScriptCompiler: Error: Set statement has to be like "
                  << "'set variable to value', but no 'to' was found.\n";
        return false;
      }
      std::string varName = WorkString.substr(0, pos);
      trim(varName);
      //now search for the variable name
      SC_VarRef ref = getVariableTypeWithIndex(varName, varsShort, varsLong, varsFloat);
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
        }//switch
        //push index (one byte)
        CompiledData.data.push_back((ref.Index)&255);
        //fill byte
        CompiledData.data.push_back(0);
      }
      else
      {
        //global var identifier or more complex expression found
        // So we assume global here.
        //push type indicator - G for global
        CompiledData.data.push_back('G');
        //push length of global name
        CompiledData.data.push_back(varName.length());
        //push name
        CompiledData.pushString(varName);
      }//else
      //get stuff after keyword "to"
      WorkString.erase(0, pos+4); //pos is position of " to " (including spaces)
                                  //We add four to get rid of "to" and both spaces, too.
      trimLeft(WorkString);
      if (WorkString=="")
      {
        std::cout << "ScriptCompiler: Error: empty value/expression at end of "
                  << "SET statement.\n";
        return false;
      }
      //assume literal value, i.e. "123.4", "5" or something like that
      /* Literal: We can push the value's length +1 space byte for stack push-op.
         <length> <space> <literal as string>
         Where: <length> is the length of the data, including space
                <space>  is a space character (' ')
                <literal as string> - the name says it
      */
      //push length
      CompiledData.data.push_back(WorkString.length()+1);
      //push space (=stack push operator)
      CompiledData.data.push_back(' ');//space is used as push by MW's interpreter
      //push value, finally
      CompiledData.pushString(WorkString);
      //We are basically done, but issue a warning for cases where the last part
      // is not a literal value.
      std::cout << "ScriptCompiler: Warning: Set statement not completely "
                << "implemented yet.\n";
    }//if Set
    //check for StartScript
    else if (lowerCase(lines.at(i).substr(0,12))=="startscript ")
    {
      CompiledData.pushCode(CodeStartScript);
      WorkString = lines.at(i).substr(12);
      trimLeft(WorkString);
      StripEnclosingQuotes(WorkString);
      CompiledData.data.push_back(WorkString.length());
      CompiledData.pushString(WorkString);
    }//if StartScript
    //check for StopScript
    else if (lowerCase(lines.at(i).substr(0,11))=="stopscript ")
    {
      CompiledData.pushCode(CodeStopScript);
      WorkString = lines.at(i).substr(11);
      trimLeft(WorkString);
      StripEnclosingQuotes(WorkString);
      CompiledData.data.push_back(WorkString.length());
      CompiledData.pushString(WorkString);
    }//if StopScript
    //check for return
    else if (lowerCase(lines.at(i))=="return")
    {
      CompiledData.pushCode(CodeReturn);
    }//if return found
    //check for end of script
    else if ((lowerCase(lines.at(i))=="end") or (lowerCase(lines.at(i).substr(0,4))=="end "))
    {
      CompiledData.pushCode(CodeEnd);
      EndLineFound = true;
      //this should be the end of the script - if not, there's junk ahead.
      if (i+1<lines.size())
      {
        std::cout << "ScriptCompiler: Error: there is more code after end clause.\n";
        return false;
      }
    }//if End found
    //check for functions
    else if (ScriptFunctions(lines.at(i), CompiledData))
    {
      std::cout << "Debug: ScriptCompiler: Hint: Function processed.\n";
    }

  }//for

  if (!EndLineFound)
  {
    std::cout << "ScriptCompiler: Error: there is no End line in the script!\n";
    return false;
  }


  /***** copy the data into result *****/
  //set result values
  result.NumShorts = varsShort.size();
  result.NumLongs = varsLong.size();
  result.NumFloats = varsFloat.size();
  //variables
  result.LocalVars = "";
  result.LocalVarSize = 0;
  // ---- write shorts
  for (i=0; i<varsShort.size(); ++i)
  {
    result.LocalVars.append(varsShort.at(i));
    result.LocalVars.append(1, '\0');
    result.LocalVarSize += varsShort.at(i).length()+1;
  }//for
  // ---- write longs
  for (i=0; i<varsLong.size(); ++i)
  {
    result.LocalVars.append(varsLong.at(i));
    result.LocalVars.append(1, '\0');
    result.LocalVarSize += varsLong.at(i).length()+1;
  }//for
  // ---- write floats
  for (i=0; i<varsFloat.size(); ++i)
  {
    result.LocalVars.append(varsFloat.at(i));
    result.LocalVars.append(1, '\0');
    result.LocalVarSize += varsFloat.at(i).length()+1;
  }//for

  //script's ID
  result.ScriptID = ScriptName;
  //text
  result.ScriptText = Text;
  //data size
  result.ScriptDataSize = CompiledData.data.size();

  /// TODO: properly transform data of CompiledChunk into result.ScriptData
  /** possible memory leak here **/
  result.ScriptData = new char[CompiledData.data.size()];
  //fill
  for (i=0; i<CompiledData.data.size(); ++i)
  {
    result.ScriptData[i] = CompiledData.data.at(i);
  }//for
  std::cout << "ScriptCompiler: Warning: compiler is still far from complete! "
            << "The resulting script might not run at all or not as intended "
            << "within the game!\n";
  return true;
}

} //namespace
