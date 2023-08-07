/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2021  Dirk Stolle

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

#include "ESMWriterSpells.hpp"
#include "../../lib/mw/Spells.hpp"

namespace MWTP
{

int32_t ESMWriterSpells::getTotalRecords() const
{
  // only writes spells
  return Spells::get().getNumberOfRecords();
}

bool ESMWriterSpells::writeRecords(std::ofstream& output) const
{
  return Spells::get().saveAllToStream(output);
}

} // namespace
