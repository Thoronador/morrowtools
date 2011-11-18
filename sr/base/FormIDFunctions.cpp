/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "FormIDFunctions.h"

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
  //chek, if there are all digits
  const std::string::size_type len = result.length();
  if (len<8)
  {
    result.insert(result.begin(), 8-len, '0');
  }
  return result;
}

} //namespace
