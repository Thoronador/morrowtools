/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef MW_ENCHANTINGS_H
#define MW_ENCHANTINGS_H

#include "MapBasedRecordManager.h"
#include "records/EnchantingRecord.h"

namespace MWTP
{

//singleton type for enchanting records
typedef MapBasedRecordManager<EnchantingRecord> Enchantings;

} //namespace

#endif // MW_ENCHANTINGS_H
