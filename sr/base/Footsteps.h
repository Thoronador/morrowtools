/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef SR_FOOTSTEPS_H
#define SR_FOOTSTEPS_H

#include "records/FootstepRecord.h"
#include "MapBasedRecordManager.h"

namespace SRTP
{

//singleton for footstep records
typedef MapBasedRecordManager<FootstepRecord> Footsteps;

} //namespace

#endif // SR_FOOTSTEPS_H
