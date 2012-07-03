/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef MW_DIALOGUEINFOS_H
#define MW_DIALOGUEINFOS_H

#include "MapBasedRecordManager.h"
#include "records/DialogueInfoRecord.h"

namespace MWTP
{

//singleton type for dialogue info records
typedef MapBasedRecordManager<DialogueInfoRecord> DialogueInfos;

} //namespace

#endif // MW_DIALOGUEINFOS_H
