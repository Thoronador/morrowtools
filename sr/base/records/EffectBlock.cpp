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

#include "EffectBlock.h"
#include "../SR_Constants.h"

namespace SRTP
{

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
    if (!unknownCTDA_CIS2s[jay].unknownCTDA.saveToStream(output)) return false;

    if (!unknownCTDA_CIS2s[jay].unknownCIS2.empty())
    {
      //write CIS2
      output.write((const char*) &cCIS2, 4);
      //CIS2's length
      subLength = unknownCTDA_CIS2s[jay].unknownCIS2.length()+1;
      output.write((const char*) &subLength, 2);
      //write CIS2's stuff
      output.write(unknownCTDA_CIS2s[jay].unknownCIS2.c_str(), subLength);
    }//if CIS2
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
    writeSize = writeSize +4 /* CTDA */ +2 /* 2 bytes for length */ +32 /* fixed length */;
    if (!unknownCTDA_CIS2s[i].unknownCIS2.empty())
    {
      writeSize = writeSize +4 /* CIS2 */ +2 /* 2 bytes for length */
                 +unknownCTDA_CIS2s[i].unknownCIS2.length()+1;
    }//if CIS2
  }//for
  return writeSize;
}
#endif

} //namespace
