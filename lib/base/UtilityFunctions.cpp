/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2015, 2021, 2024  Dirk Stolle

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

#include "UtilityFunctions.hpp"
#include <limits>
#include <sstream>

// tries to get the integer representation of a string
bool stringToShort(const std::string& str, int16_t& value)
{
  if (str.length() == 0)
    return false;
  value = 0;
  unsigned int i;
  bool negative;
  if (str.at(0) == '-')
  {
    if (str.length() == 1)
      return false;
    i = 1;
    negative = true;
  }
  else
  {
    i = 0;
    negative = false;
  }
  constexpr int16_t cTenthLimit = std::numeric_limits<int16_t>::max() / 10;
  constexpr int16_t cRealLimit = std::numeric_limits<int16_t>::max();
  for ( ; i < str.length(); ++i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      /* If the result of the multiplication in the next line would go out of
         the type range, then the result is not useful anyway, so quit here. */
      if (value > cTenthLimit)
        return false;
      value = static_cast<int16_t>(value * 10);
      /* If the result of the addition in the next line would go out of the
         type's range, then the result is not useful anyway, so quit here. */
      if (value > cRealLimit - (str[i] - '0'))
        return false;
      value = static_cast<int16_t>(value + (str[i]-'0'));
    }
    else
    {
      // unknown or invalid character detected
      return false;
    }
  }
  if (negative)
  {
    value = static_cast<int16_t>(-value);
  }
  return true;
}

bool stringToLong(const std::string& str, int32_t& value)
{
  if (str.empty())
  {
    return false;
  }
  value = 0;
  unsigned int i;
  bool negative;
  if (str.at(0) == '-')
  {
    if (str.length() == 1)
      return false;
    i = 1;
    negative = true;
  }
  else
  {
    i = 0;
    negative = false;
  }
  constexpr int32_t cTenthLimit = std::numeric_limits<int32_t>::max() / 10;
  constexpr int32_t cRealLimit = std::numeric_limits<int32_t>::max();
  for ( ; i < str.length(); ++i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      /* If the result of the multiplication in the next line would go out of
         the type range, then the result is not useful anyway, so quit here. */
      if (value > cTenthLimit)
        return false;
      value = value * 10;
      /* If the result of the addition in the next line would go out of the
         type's range, then the result is not useful anyway, so quit here. */
      if (value > cRealLimit - (str[i] - '0'))
        return false;
      value = value + (str.at(i) - '0');
    }
    else
    {
      // unknown or invalid character detected
      return false;
    }
  }
  if (negative)
  {
    value = -value;
  }
  return true;
}

std::string intToString(const int value)
{
  std::ostringstream s_str;
  s_str << value;
  return s_str.str();
}

// tries to get the floating point representation of a string
bool stringToFloat(const std::string& str, float& value)
{
  if (str.length() == 0)
    return false;
  value = 0.0f;
  std::string::size_type i, next_look;
  bool negative;
  if (str.at(0) == '-')
  {
    if (str.length() == 1)
      return false;
    i = 1;
    negative = true;
    next_look = 1;
  }
  else
  {
    i = 0;
    negative = false;
    next_look = 0;
  }
  constexpr float cTenthLimit = std::numeric_limits<float>::max() / 10.0f;
  constexpr float cRealLimit = std::numeric_limits<float>::max();
  for ( ; i < str.length(); ++i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      /* If the result of the multiplication in the next line would go out of
         the type range, then the result is not useful anyway, so quit here. */
      if (value > cTenthLimit)
        return false;
      value = value * 10.0f;
      /* If the result of the addition in the next line would go out of the
         type's range, then the result is not useful anyway, so quit here. */
      if (value > cRealLimit - static_cast<float>(str[i] - '0'))
        return false;
      value = value + static_cast<float>(str.at(i) - '0');
      ++next_look;
    }
    else if (str.at(i) == '.')
    {
      // decimal separator found - break out of loop
      next_look = i + 1;
      break;
    }
    else
    {
      // unknown or invalid character detected
      return false;
    }
  } //for
  // now go for the stuff after the separator
  float second = 0.0f;
  for (i = str.length() - 1; i >= next_look; --i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      second = second + static_cast<float>(str.at(i) - '0');
      second = second / 10.0f;
    }
    else
    {
      // unknown or invalid character detected
      return false;
    }
  } // for, second loop
  value = value + second;
  if (negative)
    value = -value;
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
  if (str1.empty())
  {
    return;
  }

  // trim stuff at begin
  const auto len = str1.length();
  std::string::size_type pos = 0;
  bool go_on = true;
  while (go_on)
  {
    char& ch = str1.at(pos);
    if ((ch == ' ') || (ch == '\t'))
    {
      ++pos;
      go_on = pos < len;
    }
    else
    {
      go_on = false;
    }
  }
  if (pos >= len)
  {
    str1 = "";
    return;
  }
  else if (pos > 0)
  {
    str1.erase(0, pos);
  }
  return;
}

void trimRight(std::string& str1)
{
  if (str1.empty())
  {
    return;
  }

  // trim stuff at end
  using signed_size_type = std::make_signed_t<std::string::size_type>;
  const signed_size_type len = str1.length();
  signed_size_type pos = len - 1;
  bool go_on = true;
  while (go_on)
  {
    char& ch = str1.at(pos);
    if ((ch == ' ') || (ch == '\t'))
    {
      --pos;
      go_on = pos >= 0;
    }
    else
    {
      go_on = false;
    }
  }
  if (pos == -1)
  {
    str1 = "";
  }
  else if (pos < len - 1)
  {
    str1.erase(pos + 1);
  }
  return;
}

void trim(std::string& str1)
{
  trimLeft(str1);
  trimRight(str1);
  return;
}

std::string lowerCase(const std::string& str1)
{
  /* We assume ASCII or compatible charset where the characters for letters are
     in alphabetical sequence.
  */
  const char lowerDiff = 'a' - 'A';
  std::string result = str1;
  for (std::string::size_type i = 0; i < str1.length(); ++i)
  {
    if ((result[i] <= 'Z') && (result[i] >= 'A'))
    {
      result[i] = static_cast<char>(result[i] + lowerDiff);
    }
  }
  return result;
}

/*tries to remove the bracket pair surrounding the rest of the string, if they
  match. Returns true, if something was removed, false otherwise.
*/
bool removeEnclosingBrackets(std::string& expr)
{
  const std::string::size_type len = expr.length();
  // can't remove brackets, if there is not enough space for them
  if (len < 2)
    return false;
  // ... or if there aren't any
  if ((expr.at(0) != '(') || (expr.at(len - 1) != ')'))
    return false;

  std::string::size_type look = 0;
  int bracket_level = 0;
  while (look < len)
  {
    if (expr.at(look) == '(')
    {
      ++bracket_level;
    }
    else if (expr.at(look) == ')')
    {
      --bracket_level;
      if (bracket_level == 0)
      {
        // Are we at the end?
        if (len - 1 == look)
        {
          // remove first and last character of string
          expr.erase(len - 1, 1); // last one
          expr.erase(0, 1); // first one
          return true;
        }
        return false;
      }
    }
    ++look;
  } // while
  return false;
}

bool stripEnclosingQuotes(std::string& str1)
{
  if (str1.length() < 2)
    return false;
  if ((str1.at(0) == '"') && (str1.at(str1.length() - 1) == '"'))
  {
    str1 = str1.substr(1, str1.length() - 2);
    return true;
  }
  return false;
}

int lowerCaseCompare(const std::string& left, const std::string& right)
{
  const auto l_size = left.size();
  const auto r_size = right.size();
  const std::string::size_type len = std::min(l_size, r_size);

  for (std::string::size_type i = 0; i < len; ++i)
  {
    const char l = static_cast<char>(tolower(static_cast<unsigned char>(left[i])));
    const char r = static_cast<char>(tolower(static_cast<unsigned char>(right[i])));
    if (l < r)
    {
      return -1;
    }
    if (l > r)
    {
      return 1;
    }
  }
  // if they are equal so far, the length decides
  return static_cast<int>(l_size - r_size);
}
