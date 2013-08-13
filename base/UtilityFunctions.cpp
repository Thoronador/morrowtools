/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#include "UtilityFunctions.h"
#include <sstream>

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

std::string floatToString(const float f)
{
  std::stringstream s_str;
  s_str << f;
  return s_str.str();
}

void trimLeft(std::string& str1)
{
  if (str1.empty()) return;
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
  if (str1.empty()) return;
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

/*tries to remove the bracket pair surrounding the rest of the string, if they
  match. Returns true, if something was removed, false otherwise.
*/
bool removeEnclosingBrackets(std::string& expr)
{
  const std::string::size_type len = expr.length();
  //can't remove brackets, if there is not enough space for them
  if (len<2) return false;
  //... or if there aren't any
  if ((expr.at(0)!='(') or (expr.at(len-1)!=')')) return false;

  std::string::size_type look = 0;
  int bracket_level = 0;
  while (look<len)
  {
    if (expr.at(look)=='(')
    {
      ++bracket_level;
    }
    else if (expr.at(look)==')')
    {
      --bracket_level;
      if (bracket_level==0)
      {
        //Are we at the end?
        if (len-1==look)
        {
          //remove first and last character of string
          expr.erase(len-1, 1); //last one
          expr.erase(0,1); //first one
          return true;
        }
        return false;
      }
    }
    ++look;
  }//while
  return false;
}

bool stripEnclosingQuotes(std::string& str1)
{
  if (str1.length()<2) return false;
  if ((str1.at(0)=='"') and (str1.at(str1.length()-1)=='"'))
  {
    str1 = str1.substr(1, str1.length()-2);
    return true;
  }
  return false;
}

int lowerCaseCompare(const std::string& left, const std::string& right)
{
  const std::string::size_type l_size = left.size();
  const std::string::size_type r_size = right.size();
  const std::string::size_type len = std::min(l_size, r_size);

  std::string::size_type i;
  for (i=0; i<len; ++i)
  {
    const char l = tolower(left.at(i));
    const char r = tolower(right.at(i));
    if (l<r)
    {
      return -1;
    }
    if (l>r)
    {
      return 1;
    }
  }//for
  //if they are equal so far, the length decides
  return l_size-r_size;
}
