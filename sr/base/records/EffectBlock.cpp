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

namespace SRTP
{

bool EffectBlock::operator==(const EffectBlock& other) const
{
  return ((unknownEFID==other.unknownEFID) and (unknownEFITs[0]==other.unknownEFITs[0])
      and (unknownEFITs[1]==other.unknownEFITs[1]) and (unknownEFITs[2]==other.unknownEFITs[2])
      and (unknownCTDAs==other.unknownCTDAs));
}

} //namespace
