/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2013  Thoronador

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

#include "FormIDFunctions.hpp"

namespace SRTP
{

const char hexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                            'A', 'B', 'C', 'D', 'E', 'F'};

uint8_t extractModIndex(const uint32_t fID)
{
  return (fID>>24);
}

uint32_t extractObjectIndex(const uint32_t fID)
{
  return ((fID<<8)>>8);
}

// returns a string representation of the form ID
std::string getFormIDAsString(uint32_t fID)
{
  std::string result = "";
  while (fID>0)
  {
    result.insert(result.begin(), hexDigits[fID%16]);
    fID /= 16;
  }//while
  //check, if there are all digits
  const std::string::size_type len = result.length();
  if (len<8)
  {
    result.insert(result.begin(), 8-len, '0');
  }
  return result;
}

std::string getFormIDAsStringXX(const uint32_t fID)
{
  std::string result(getFormIDAsString(fID));
  if (extractModIndex(fID)!=0)
  {
    result[0] = 'x';
    result[1] = 'x';
  }
  return result;
}

std::string getFormIDAsStringWithFile(const uint32_t fID, const std::vector<std::string>& loadOrder, const bool withFiles)
{
  std::string result(getFormIDAsString(fID));
  const uint8_t modIndex = extractModIndex(fID);
  if (modIndex!=0)
  {
    result[0] = 'x';
    result[1] = 'x';
  }
  if (withFiles)
  {
    result.append(" (file: ");
    if (modIndex<loadOrder.size())
    {
      result.append(loadOrder[modIndex]);
    }
    else
    {
      result.append("unknown");
    }
    result.append(")");
  }
  return result;
}

} //namespace
