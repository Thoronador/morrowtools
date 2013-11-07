/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_EFFECTSHADERS_H
#define SR_EFFECTSHADERS_H

#include "records/EffectShaderRecord.h"
#include "MapBasedRecordManager.h"

namespace SRTP
{

//singleton for effect shader records
typedef MapBasedRecordManager<EffectShaderRecord> EffectShaders;

} //namespace

#endif // SR_EFFECTSHADERS_H
