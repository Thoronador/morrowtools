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

#include "EffectBlock.h"
#include "../SR_Constants.h"

namespace SRTP
{

bool EffectBlock::operator==(const EffectBlock& other) const
{
  return ((unknownEFID==other.unknownEFID) and (unknownEFITs[0]==other.unknownEFITs[0])
      and (unknownEFITs[1]==other.unknownEFITs[1]) and (unknownEFITs[2]==other.unknownEFITs[2])
      and (unknownCTDAs==other.unknownCTDAs));
}

bool EffectBlock::saveToStream(std::ofstream& output) const
{
  //write EFID
  output.write((const char*) &cEFID, 4);
  //EFID's length
  uint16_t subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write EFID's stuff
  output.write((const char*) &(unknownEFID), 4);

  //write EFIT
  output.write((const char*) &cEFIT, 4);
  //EFIT's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write EFIT's stuff
  output.write((const char*) &(unknownEFITs[0]), 4);
  output.write((const char*) &(unknownEFITs[1]), 4);
  output.write((const char*) &(unknownEFITs[2]), 4);

  unsigned int jay;
  for (jay=0; jay<unknownCTDAs.size(); ++jay)
  {
    if (!unknownCTDAs[jay].saveToStream(output)) return false;
  }//for jay

  return output.good();
}

} //namespace
