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

#include "ESMReaderContentsBase.h"
#include <iostream>
#include "SR_Constants.h"

namespace SRTP
{

ESMReaderContentsBase::ESMReaderContentsBase()
{
  contents.removeContents();
}

ESMReaderContentsBase::~ESMReaderContentsBase()
{
  contents.removeContents();
}

void ESMReaderContentsBase::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  /*add a new group to the file content representation and set its GroupData
    (i.e. header) to the stuff that was read from the file stream */
  Group& newGroup = contents.addNewGroup();
  newGroup.headerData = g_data;
}

void ESMReaderContentsBase::groupFinished(const GroupData& g_data)
{
  //still empty
}

} //namespace
