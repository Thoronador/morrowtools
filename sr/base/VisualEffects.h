/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_VISUALEFFECTS_H
#define SR_VISUALEFFECTS_H

#include "records/VisualEffectRecord.h"
#include "MapBasedRecordManager.h"

namespace SRTP
{

//singleton for visual effect records
typedef MapBasedRecordManager<VisualEffectRecord> VisualEffects;

} //namespace

#endif // SR_VISUALEFFECTS_H