/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "CTDAData.h"
#include <cstring>
#include <iostream>
#include "../SR_Constants.h"

namespace SRTP
{

bool CTDAData::loadFromStream(std::istream& in_Stream, uint32_t& bytesRead)
{
  //CTDA's length
  uint16_t subLength = 0;
  in_Stream.read((char*) &subLength, 2);
  bytesRead += 2;
  if (subLength!=32)
  {
    std::cout <<"Error: sub record CTDA has invalid length ("<<subLength
              <<" bytes). Should be 32 bytes.\n";
    return false;
  }
  //read CTDA data
  in_Stream.read((char*) content, 32);
  bytesRead += 32;
  if (!in_Stream.good())
  {
    std::cout << "Error while reading subrecord CTDA!\n";
    return false;
  }
  return true;
}

#ifndef SR_UNSAVEABLE_RECORDS
bool CTDAData::saveToStream(std::ostream& output) const
{
  //write CTDA
  output.write((const char*) &cCTDA, 4);
  //CTDA's length
  const uint16_t subLength = 32; //fixed size
  output.write((const char*) &subLength, 2);
  //write content
  output.write((const char*) content, 32);
  return output.good();
}
#endif

bool CTDAData::operator==(const CTDAData& other) const
{
  return (memcmp(content, other.content, 32)==0);
}

/* CTDA_CIS2_compound's functions */

CTDA_CIS2_compound::CTDA_CIS2_compound()
{
  memset(unknownCTDA.content, 0, 32);
  unknownCIS2 = "";
}

CTDA_CIS2_compound::CTDA_CIS2_compound(const CTDAData& ctda, const std::string& cis2)
{
  unknownCTDA = ctda;
  unknownCIS2 = cis2;
}

bool CTDA_CIS2_compound::operator==(const CTDA_CIS2_compound& other) const
{
  return ((unknownCTDA==other.unknownCTDA) and (unknownCIS2==other.unknownCIS2));
}

} //namespace
