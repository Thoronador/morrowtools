/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project test suite.
    Copyright (C) 2022  Dirk Stolle

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

#include "TestReaderReIndexMod.hpp"

namespace SRTP
{

TestReaderReIndexMod::TestReaderReIndexMod(const std::vector<std::string>& loadOrder)
: ESMReaderReIndexMod(loadOrder)
{
}

bool TestReaderReIndexMod::needGroup(const GroupData& g_data) const
{
  return false;
}

bool TestReaderReIndexMod::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  return true;
}

int TestReaderReIndexMod::readGroup(std::istream& input, const GroupData& g_data, const bool localized, const StringTable& table)
{
  return ESMReader::skipGroup(input, g_data);
}

bool TestReaderReIndexMod::groupFinished(const GroupData& g_data)
{
  return true;
}

int TestReaderReIndexMod::readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table)
{
  return ESMReader::skipRecord(input);
}

} // namespace
