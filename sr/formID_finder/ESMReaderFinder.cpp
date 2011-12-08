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

#include "ESMReaderFinder.h"
#include "../base/SR_Constants.h"

namespace SRTP
{

ESMReaderFinder::ESMReaderFinder()
{
  //empty
}

ESMReaderFinder::~ESMReaderFinder()
{
  //empty
}

bool ESMReaderFinder::needGroup(const GroupData& g_data) const
{
  switch (g_data.getGroupName())
  {
    //case cALCH:
    //case cAMMO:
    //case cAPPA:
    //case cARMO:
    case cBOOK:
    //case cINGR:
    //case cKEYM:
    case cMISC:
    //case cPERK;
    //case cSCRL:
    case cSHOU:
    //case cSPEL:
    //case cWEAP:
    case cWOOP:
         return true;
         break;
  }//swi
  return false;
}

int ESMReaderFinder::readGroup(std::ifstream& in_File, const GroupData& g_data)
{
  #warning Not implemented yet!
  return -1;
}

int ESMReaderFinder::readNextRecord(std::ifstream& in_File)
{
  #warning Not implemented yet!
  return -1;
}

} //namespace
