/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project test suite.
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

#include "TestFactionsReader.hpp"

namespace SRTP
{

int TestFactionsReader::processGroup(std::istream& input, const bool withHeader, const bool localized, const StringTable& table)
{
  return ESMReaderSingleType<FactionRecord, Factions, cFACT>::processGroup(input, withHeader, localized, table);
}

int TestFactionsReader::readGroup(std::istream& input, const GroupData& g_data, const bool localized, const StringTable& table)
{
  return ESMReaderSingleType<FactionRecord, Factions, cFACT>::readGroup(input, g_data, localized, table);
}


bool TestFactionsReaderGroupStartFails::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  return false;
}


bool TestFactionsReaderGroupFinishFails::groupFinished(const GroupData& g_data)
{
  return false;
}

} // namespace
