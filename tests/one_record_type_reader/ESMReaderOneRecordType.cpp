/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for the Skyrim Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#include "ESMReaderOneRecordType.hpp"
#include "../../sr/base/SR_Constants.hpp"
#include "../../sr/base/Cells.hpp"
#include "../../sr/base/records/CellRecord.hpp"

namespace SRTP
{

bool ESMReaderOneRecordType::needGroup(const GroupData& g_data) const
{
  return true;
}

bool ESMReaderOneRecordType::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  return true;
}

bool ESMReaderOneRecordType::groupFinished(const GroupData& g_data)
{
  return true;
}

int ESMReaderOneRecordType::readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table)
{
  if (recName == cCELL)
  {
    CellRecord record;
    if (!record.loadFromStream(input, localized, table))
      return -1;
    Cells::get().addRecord(record);
    return 1;
  }
  return skipRecord(input);
}

} // namespace
