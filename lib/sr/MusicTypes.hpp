/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012, 2022  Dirk Stolle

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

#ifndef SR_MUSICTYPES_HPP
#define SR_MUSICTYPES_HPP

#include "records/MusicTypeRecord.hpp"
#include "MapBasedRecordManager.hpp"

namespace SRTP
{

// singleton for music type records
using MusicTypes =  MapBasedRecordManager<MusicTypeRecord>;

}

#endif // SR_MUSICTYPES_HPP