/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "EffectBlock.h"
#include "../SR_Constants.h"

namespace SRTP
{

EffectBlock::EffectBlock()
: effectFormID(0),
  magnitude(0.0f),
  areaOfEffect(0),
  duration(0),
  unknownCTDA_CIS2s(std::vector<CTDA_CIS2_compound>())
{
}

bool EffectBlock::operator==(const EffectBlock& other) const
{
  return ((effectFormID==other.effectFormID) and (magnitude==other.magnitude)
      and (areaOfEffect==other.areaOfEffect) and (duration==other.duration)
      and (unknownCTDA_CIS2s==other.unknownCTDA_CIS2s));
}

#ifndef SR_UNSAVEABLE_RECORDS
bool EffectBlock::saveToStream(std::ofstream& output) const
{
  //write EFID
  output.write((const char*) &cEFID, 4);
  //EFID's length
  uint16_t subLength = 4; //fixed
  output.write((const char*) &subLength, 2);
  //write EFID's stuff
  output.write((const char*) &effectFormID, 4);

  //write EFIT
  output.write((const char*) &cEFIT, 4);
  //EFIT's length
  subLength = 12; //fixed
  output.write((const char*) &subLength, 2);
  //write EFIT's stuff
  output.write((const char*) &magnitude, 4);
  output.write((const char*) &areaOfEffect, 4);
  output.write((const char*) &duration, 4);

  unsigned int jay;
  for (jay=0; jay<unknownCTDA_CIS2s.size(); ++jay)
  {
    if (!unknownCTDA_CIS2s[jay].saveToStream(output)) return false;
  }//for jay

  return output.good();
}

uint32_t EffectBlock::getWriteSize() const
{
  uint32_t writeSize = 4 /* EFID */ +2 /* 2 bytes for length */ +4 /* fixed length */
                      +4 /* EFIT */ +2 /* 2 bytes for length */ +12 /* fixed length */;
  unsigned int i;
  for (i=0; i<unknownCTDA_CIS2s.size(); ++i)
  {
    writeSize += unknownCTDA_CIS2s[i].getWriteSize();
  }//for
  return writeSize;
}
#endif

} //namespace
