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

std::string::size_type getCommentStart(const std::string& line)
{
  const std::string::size_type len = line.length();
  std::string::size_type look = 0;
  bool outsideQuote = true;
  while (look<len)
  {
    if (line.at(look)=='"')
    {
      outsideQuote = not outsideQuote;
    }
    else if ((outsideQuote) and (line.at(look)==';'))
    {
      //found a place where comment starts
      return look;
    }//else
    ++look;
  }//while
  return std::string::npos;
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
     in alphabetical sequence.
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

bool ScriptFunctions_ZeroParameters(const std::vector<std::string>& params, CompiledChunk& chunk)
{
  //entry at index zero is the function's name
  const std::string lowerFunction = lowerCase(params.at(0));
  if (lowerFunction=="activate")
  {
    chunk.pushCode(CodeActivate);
    return true;
  }
  if (lowerFunction=="becomewerewolf")
  {
    chunk.pushCode(CodeBecomeWerewolf);
    return true;
  }
  if (lowerFunction=="cellchanged")
  {
    chunk.pushCode(CodeCellChanged);
    return true;
  }
  if (lowerFunction=="clearforcejump")
  {
    chunk.pushCode(CodeClearForceJump);
    return true;
  }
  if (lowerFunction=="clearforcemovejump")
  {
    chunk.pushCode(CodeClearForceMoveJump);
    return true;
  }
  if (lowerFunction=="clearforcerun")
  {
    chunk.pushCode(CodeClearForceRun);
    return true;
  }
  if (lowerFunction=="clearforcesneak")
  {
    chunk.pushCode(CodeClearForceSneak);
    return true;
  }
  if (lowerFunction=="disable")
  {
    chunk.pushCode(CodeDisable);
    return true;
  }
  if (lowerFunction=="disablelevitation")
  {
    chunk.pushCode(CodeDisableLevitation);
    return true;
  }
  if (lowerFunction=="disableplayercontrols")
  {
    chunk.pushCode(CodeDisablePlayerControls);
    return true;
  }
  if (lowerFunction=="disableplayerfighting")
  {
    chunk.pushCode(CodeDisablePlayerFighting);
    return true;
  }
  if (lowerFunction=="disableplayerjumping")
  {
    chunk.pushCode(CodeDisablePlayerJumping);
    return true;
  }
  if (lowerFunction=="disableplayerlooking")
  {
    chunk.pushCode(CodeDisablePlayerLooking);
    return true;
  }
  if (lowerFunction=="disableplayermagic")
  {
    chunk.pushCode(CodeDisablePlayerMagic);
    return true;
  }
  if (lowerFunction=="disableplayerviewswitch")
  {
    chunk.pushCode(CodeDisablePlayerViewSwitch);
    return true;
  }
  if (lowerFunction=="disableteleporting")
  {
    chunk.pushCode(CodeDisableTeleporting);
    return true;
  }
  if (lowerFunction=="disablevanitymode")
  {
    chunk.pushCode(CodeDisableVanityMode);
    return true;
  }
  if (lowerFunction=="dontsaveobject")
  {
    chunk.pushCode(CodeDontSaveObject);
    return true;
  }
  if (lowerFunction=="enable")
  {
    chunk.pushCode(CodeEnable);
    return true;
  }
  if (lowerFunction=="enablebirthmenu")
  {
    chunk.pushCode(CodeEnableBirthMenu);
    return true;
  }
  if (lowerFunction=="enableclassmenu")
  {
    chunk.pushCode(CodeEnableClassMenu);
    return true;
  }
  if (lowerFunction=="enableinventorymenu")
  {
    chunk.pushCode(CodeEnableInventoryMenu);
    return true;
  }
  if (lowerFunction=="enablelevelupmenu")
  {
    chunk.pushCode(CodeEnableLevelUpMenu);
    return true;
  }
  if (lowerFunction=="enablelevitation")
  {
    chunk.pushCode(CodeEnableLevitation);
    return true;
  }
  if (lowerFunction=="enablemagicmenu")
  {
    chunk.pushCode(CodeEnableMagicMenu);
    return true;
  }
  if (lowerFunction=="enablemapmenu")
  {
    chunk.pushCode(CodeEnableMapMenu);
    return true;
  }
  if (lowerFunction=="enablenamemenu")
  {
    chunk.pushCode(CodeEnableNameMenu);
    return true;
  }
  if (lowerFunction=="enableplayercontrols")
  {
    chunk.pushCode(CodeEnablePlayerControls);
    return true;
  }
  if (lowerFunction=="enableplayerfighting")
  {
    chunk.pushCode(CodeEnablePlayerFighting);
    return true;
  }
  if (lowerFunction=="enableplayerjumping")
  {
    chunk.pushCode(CodeEnablePlayerJumping);
    return true;
  }
  if (lowerFunction=="enableplayerlooking")
  {
    chunk.pushCode(CodeEnablePlayerLooking);
    return true;
  }
  if (lowerFunction=="enableplayermagic")
  {
    chunk.pushCode(CodeEnablePlayerMagic);
    return true;
  }
  if (lowerFunction=="enableplayerviewswitch")
  {
    chunk.pushCode(CodeEnablePlayerViewSwitch);
    return true;
  }
  if (lowerFunction=="enableracemenu")
  {
    chunk.pushCode(CodeEnableRaceMenu);
    return true;
  }
  if (lowerFunction=="enablerest")
  {
    chunk.pushCode(CodeEnableRest);
    return true;
  }
  if (lowerFunction=="enablestatreviewmenu")
  {
    chunk.pushCode(CodeEnableStatReviewMenu);
    return true;
  }
  if (lowerFunction=="enablestatsmenu")
  {
    chunk.pushCode(CodeEnableStatsMenu);
    return true;
  }
  if (lowerFunction=="enableteleporting")
  {
    chunk.pushCode(CodeEnableTeleporting);
    return true;
  }
  if (lowerFunction=="enablevanitymode")
  {
    chunk.pushCode(CodeEnableVanityMode);
    return true;
  }
  if (lowerFunction=="fall")
  {
    chunk.pushCode(CodeFall);
    return true;
  }
  if (lowerFunction=="forcegreeting")
  {
    chunk.pushCode(CodeForceGreeting);
    return true;
  }
  if (lowerFunction=="forcejump")
  {
    chunk.pushCode(CodeForceJump);
    return true;
  }
  if (lowerFunction=="forcemovejump")
  {
    chunk.pushCode(CodeForceMoveJump);
    return true;
  }
  if (lowerFunction=="forcerun")
  {
    chunk.pushCode(CodeForceRun);
    return true;
  }

  if (lowerFunction=="forcesneak")
  {
    chunk.pushCode(CodeForceSneak);
    return true;
  }
  if ((lowerFunction.substr(0,3)=="get") and (lowerFunction.length()>3))
  {
    if (lowerFunction.at(3)=='a')
    {
      if (lowerFunction=="getacrobatics")
      {
        chunk.pushCode(CodeGetAcrobatics);
        return true;
      }
      if (lowerFunction=="getagility")
      {
        chunk.pushCode(CodeGetAgility);
        return true;
      }
      if (lowerFunction=="getaipackagedone")
      {
        chunk.pushCode(CodeGetAIPackageDone);
        return true;
      }
      if (lowerFunction=="getalarm")
      {
        chunk.pushCode(CodeGetAlarm);
        return true;
      }
      if (lowerFunction=="getalchemy")
      {
        chunk.pushCode(CodeGetAlchemy);
        return true;
      }
      if (lowerFunction=="getalteration")
      {
        chunk.pushCode(CodeGetAlteration);
        return true;
      }
      if (lowerFunction=="getarmorbonus")
      {
        chunk.pushCode(CodeGetArmorBonus);
        return true;
      }
      if (lowerFunction=="getarmorer")
      {
        chunk.pushCode(CodeGetArmorer);
        return true;
      }
      if (lowerFunction=="getathletics")
      {
        chunk.pushCode(CodeGetAthletics);
        return true;
      }
      if (lowerFunction=="getattackbonus")
      {
        chunk.pushCode(CodeGetAttackBonus);
        return true;
      }
      if (lowerFunction=="getattacked")
      {
        chunk.pushCode(CodeGetAttacked);
        return true;
      }
      if (lowerFunction=="getaxe")
      {
        chunk.pushCode(CodeGetAxe);
        return true;
      }
      return false;//all zero argument functions with "GetA..." done
    }//if fourth character is 'a'
    if (lowerFunction=="getblightdisease")
    {
      chunk.pushCode(CodeGetBlightDisease);
      return true;
    }
    if (lowerFunction=="getblindness")
    {
      chunk.pushCode(CodeGetBlindness);
      return true;
    }
    if (lowerFunction=="getblock")
    {
      chunk.pushCode(CodeGetBlock);
      return true;
    }
    if (lowerFunction=="getbluntweapon")
    {
      chunk.pushCode(CodeGetBluntWeapon);
      return true;
    }
    if (lowerFunction=="getbuttonpressed")
    {
      chunk.pushCode(CodeGetButtonPressed);
      return true;
    }
    if (lowerFunction=="getcastpenalty")
    {
      chunk.pushCode(CodeGetCastPenalty);
      return true;
    }
    if (lowerFunction=="getchameleon")
    {
      chunk.pushCode(CodeGetChameleon);
      return true;
    }
    if (lowerFunction=="getcollidingactor")
    {
      chunk.pushCode(CodeGetCollidingActor);
      return true;
    }
    if (lowerFunction=="getcollidingpc")
    {
      chunk.pushCode(CodeGetCollidingPC);
      return true;
    }
    if (lowerFunction=="getcommondisease")
    {
      chunk.pushCode(CodeGetCommonDisease);
      return true;
    }
    if (lowerFunction=="getconjuration")
    {
      chunk.pushCode(CodeGetConjuration);
      return true;
    }
    if (lowerFunction=="getcurrentaipackage")
    {
      chunk.pushCode(CodeGetCurrentAIPackage);
      return true;
    }
    if (lowerFunction=="getcurrenttime")
    {
      chunk.pushCode(CodeGetCurrentTime);
      return true;
    }
    if (lowerFunction=="getcurrentweather")
    {
      chunk.pushCode(CodeGetCurrentWeather);
      return true;
    }
    if (lowerFunction=="getdefendbonus")
    {
      chunk.pushCode(CodeGetDefendBonus);
      return true;
    }
    if (lowerFunction=="getdestruction")
    {
      chunk.pushCode(CodeGetDestruction);
      return true;
    }
    if (lowerFunction=="getdisabled")
    {
      chunk.pushCode(CodeGetDisabled);
      return true;
    }
    if (lowerFunction=="getdisposition")
    {
      chunk.pushCode(CodeGetDisposition);
      return true;
    }
    if (lowerFunction=="getenchant")
    {
      chunk.pushCode(CodeGetEnchant);
      return true;
    }
    if (lowerFunction=="getendurance")
    {
      chunk.pushCode(CodeGetEndurance);
      return true;
    }
    if (lowerFunction=="getfatigue")
    {
      chunk.pushCode(CodeGetFatigue);
      return true;
    }
    if (lowerFunction=="getfight")
    {
      chunk.pushCode(CodeGetFight);
      return true;
    }
    if (lowerFunction=="getflee")
    {
      chunk.pushCode(CodeGetFlee);
      return true;
    }
    if (lowerFunction=="getflying")
    {
      chunk.pushCode(CodeGetFlying);
      return true;
    }
    if (lowerFunction=="gethandtohand")
    {
      chunk.pushCode(CodeGetHandToHand);
      return true;
    }
    if (lowerFunction=="gethealth")
    {
      chunk.pushCode(CodeGetHealth);
      return true;
    }
    if (lowerFunction=="getheavyarmor")
    {
      chunk.pushCode(CodeGetHeavyArmor);
      return true;
    }
    if (lowerFunction=="gethello")
    {
      chunk.pushCode(CodeGetHello);
      return true;
    }
    if (lowerFunction=="getillusion")
    {
      chunk.pushCode(CodeGetIllusion);
      return true;
    }
    if (lowerFunction=="getintelligence")
    {
      chunk.pushCode(CodeGetIntelligence);
      return true;
    }
    //check for both correct and earlier misspelled version of function name
    if ((lowerFunction=="getinvisible") or (lowerFunction=="getinvisibile"))
    {
      chunk.pushCode(CodeGetInvisible);
      return true;
    }
    if (lowerFunction=="getlevel")
    {
      chunk.pushCode(CodeGetLevel);
      return true;
    }
    if (lowerFunction=="getlightarmor")
    {
      chunk.pushCode(CodeGetLightArmor);
      return true;
    }
    if (lowerFunction=="getlongblade")
    {
      chunk.pushCode(CodeGetLongBlade);
      return true;
    }
    if (lowerFunction=="getluck")
    {
      chunk.pushCode(CodeGetLuck);
      return true;
    }
    if (lowerFunction=="getmagicka")
    {
      chunk.pushCode(CodeGetMagicka);
      return true;
    }
    if (lowerFunction=="getmarksman")
    {
      chunk.pushCode(CodeGetMarksman);
      return true;
    }
    if (lowerFunction=="getmediumarmor")
    {
      chunk.pushCode(CodeGetMediumArmor);
      return true;
    }
    if (lowerFunction=="getmercantile")
    {
      chunk.pushCode(CodeGetMercantile);
      return true;
    }
    if (lowerFunction=="getmysticism")
    {
      chunk.pushCode(CodeGetMysticism);
      return true;
    }
    if (lowerFunction=="getparalysis")
    {
      chunk.pushCode(CodeGetParalysis);
      return true;
    }
    if (lowerFunction=="getpccrimelevel")
    {
      chunk.pushCode(CodeGetPCCrimeLevel);
      return true;
    }
    if (lowerFunction=="getpcvisionbonus")
    {
      chunk.pushCode(CodeGetPCVisionBonus);
      return true;
    }
    if (lowerFunction=="getpersonality")
    {
      chunk.pushCode(CodeGetPersonality);
      return true;
    }
    if (lowerFunction=="getreputation")
    {
      chunk.pushCode(CodeGetReputation);
      return true;
    }
    if (lowerFunction=="getresistblight")
    {
      chunk.pushCode(CodeGetResistBlight);
      return true;
    }
    if (lowerFunction=="getresistcorprus")
    {
      chunk.pushCode(CodeGetResistCorprus);
      return true;
    }
    if (lowerFunction=="getresistdisease")
    {
      chunk.pushCode(CodeGetResistDisease);
      return true;
    }
    if (lowerFunction=="getresistfire")
    {
      chunk.pushCode(CodeGetResistFire);
      return true;
    }
    if (lowerFunction=="getresistfrost")
    {
      chunk.pushCode(CodeGetResistFrost);
      return true;
    }
    if (lowerFunction=="getresistmagicka")
    {
      chunk.pushCode(CodeGetResistMagicka);
      return true;
    }
    if (lowerFunction=="getresistnormalweapons")
    {
      chunk.pushCode(CodeGetResistNormalWeapons);
      return true;
    }
    if (lowerFunction=="getresistparalysis")
    {
      chunk.pushCode(CodeGetResistParalysis);
      return true;
    }
    if (lowerFunction=="getresistpoison")
    {
      chunk.pushCode(CodeGetResistPoison);
      return true;
    }
    if (lowerFunction=="getresistshock")
    {
      chunk.pushCode(CodeGetResistShock);
      return true;
    }
    if (lowerFunction=="getrestoration")
    {
      chunk.pushCode(CodeGetRestoration);
      return true;
    }
    if (lowerFunction=="getscale")
    {
      chunk.pushCode(CodeGetScale);
      return true;
    }
    if (lowerFunction=="getsecurity")
    {
      chunk.pushCode(CodeGetSecurity);
      return true;
    }
    if (lowerFunction=="getshortblade")
    {
      chunk.pushCode(CodeGetShortBlade);
      return true;
    }
    if (lowerFunction=="getsilence")
    {
      chunk.pushCode(CodeGetSilence);
      return true;
    }
    if (lowerFunction=="getsneak")
    {
      chunk.pushCode(CodeGetSneak);
      return true;
    }
    if (lowerFunction=="getspear")
    {
      chunk.pushCode(CodeGetSpear);
      return true;
    }
    if (lowerFunction=="getspeechcraft")
    {
      chunk.pushCode(CodeGetSpeechcraft);
      return true;
    }
    if (lowerFunction=="getspeed")
    {
      chunk.pushCode(CodeGetSpeed);
      return true;
    }
    if (lowerFunction=="getstrength")
    {
      chunk.pushCode(CodeGetStrength);
      return true;
    }
    if (lowerFunction=="getsuperjump")
    {
      chunk.pushCode(CodeGetSuperJump);
      return true;
    }
    if (lowerFunction=="getswimspeed")
    {
      chunk.pushCode(CodeGetSwimSpeed);
      return true;
    }
    if (lowerFunction=="getunarmored")
    {
      chunk.pushCode(CodeGetUnarmored);
      return true;
    }
    if (lowerFunction=="getwaterbreathing")
    {
      chunk.pushCode(CodeGetWaterBreathing);
      return true;
    }
    if (lowerFunction=="getwaterwalking")
    {
      chunk.pushCode(CodeGetWaterWalking);
      return true;
    }
    if (lowerFunction=="getwillpower")
    {
      chunk.pushCode(CodeGetWillpower);
      return true;
    }
  }//get functions
  return false;
}

bool ScriptFunctions_OneParameter(const std::vector<std::string>& params, CompiledChunk& chunk)
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
    chunk.pushCode(CodeFadeIn);
    //parameter is time for fading (float)
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
  if (lowerFunction == "fadeout")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: FadeOut needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeFadeOut);
    //parameter is time for fading (float)
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
    if ((Axis!='X') and (Axis!='Y') and (Axis!='Z'))
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
    chunk.pushCode(CodeGetArmorType);
    //parameter is slot index
    int16_t slot;
    if (stringToShort(params[1], slot))
    {
      chunk.pushShort(slot);
    }//if
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "short value.\n";
      return false;
    }
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
  }//if
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
  }//if
  if (lowerFunction == "getdistance")
  {
    if (params.size()<2)
    {
      std::cout << "ScriptCompiler: Error: GetDistance needs one parameter!\n";
      return false;
    }
    chunk.pushCode(CodeGetDistance);
    //parameter is object ID
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
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
    chunk.pushCode(CodeAddItem);
    //push ID's length
    chunk.data.push_back(params[1].length());
    //push ID
    chunk.pushString(params[1]);
    //push count
    int16_t count;
    if (stringToShort(params[2], count))
    {
      chunk.pushShort(count);
    }
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    return true;
  }
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
  }
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
  }
  if (lowerFunction == "changeweather")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: ChangeWeather needs two parameters!\n";
      return false;
    }
    //first parameter is ID of region, second is short that indicates the new weather
    chunk.pushCode(CodeChangeWeather);
    //push region ID's length
    chunk.data.push_back(params[1].length());
    //push region ID
    chunk.pushString(params[1]);
    //push new weather type
    int16_t weather_type;
    if (stringToShort(params[2], weather_type))
    {
      chunk.pushShort(weather_type);
    }
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    return true;
  }
  if (lowerFunction == "drop")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: Drop needs two parameters!\n";
      return false;
    }
    //first parameter is ID of item, second is short that indicates the count
    chunk.pushCode(CodeDrop);
    //push item ID's length
    chunk.data.push_back(params[1].length());
    //push item ID
    chunk.pushString(params[1]);
    //push count
    int16_t count;
    if (stringToShort(params[2], count))
    {
      chunk.pushShort(count);
    }
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is no short value!\n";
      return false;
    }
    return true;
  }
  if (lowerFunction == "fadeto")
  {
    if (params.size()<3)
    {
      std::cout << "ScriptCompiler: Error: FadeTo needs two parameters!\n";
      return false;
    }
    chunk.pushCode(CodeFadeTo);
    //first parameter is amount of fading (long?)
    //push long
    int32_t fade_amount;
    if (stringToLong(params[1], fade_amount))
    {
      chunk.pushLong(fade_amount);
    }//if
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[1]<<"\" is not a "
                << "long value.\n";
      return false;
    }
    //second parameter is time for fading (float)
    //push float
    float fade_time;
    if (stringToFloat(params[2], fade_time))
    {
      chunk.pushFloat(fade_time);
    }//if
    else
    {
      std::cout << "ScriptCompiler: Error: \""<<params[2]<<"\" is not a "
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
  //split line into seperate parameters
  std::vector<std::string> parameters = explodeParams(line);
  /* Now the first entry in vector params should be the function name, the rest
     is the list of parameters. That's why the size of the vector is one more
     than the number of parameters.
  */
  switch(parameters.size())
  {
    case 3:
         if (ScriptFunctions_TwoParameters(parameters, chunk))
         {
           return true;
         }
    case 2:
         if (ScriptFunctions_OneParameter(parameters, chunk))
         {
           return true;
         }
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
    pos = getCommentStart(new_line);
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
