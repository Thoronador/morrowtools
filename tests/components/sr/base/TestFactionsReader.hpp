/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project test suite.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#ifndef SR_TEST_FACTIONSREADER_HPP
#define SR_TEST_FACTIONSREADER_HPP

#include "../../../../sr/base/ESMReaderSingleType.hpp"
#include "../../../../sr/base/Factions.hpp"
#include "../../../../sr/base/SR_Constants.hpp"

namespace SRTP
{

class TestFactionsReader: public ESMReaderSingleType<FactionRecord, Factions, cFACT>
{
  public:
    TestFactionsReader() = default;

    int processGroup(std::istream& input, const bool withHeader, const bool localized, const StringTable& table);

    int readGroup(std::istream& input, const GroupData& g_data, const bool localized, const StringTable& table);
}; // class

class TestFactionsReaderGroupStartFails: public TestFactionsReader
{
  public:
    TestFactionsReaderGroupStartFails() = default;

    virtual bool nextGroupStarted(const GroupData& g_data, const bool sub) override;
}; // class

class TestFactionsReaderGroupFinishFails: public TestFactionsReader
{
  public:
    TestFactionsReaderGroupFinishFails() = default;

    virtual bool groupFinished(const GroupData& g_data) override;
}; // class

} // namespace

#endif // SR_TEST_FACTIONSREADER_HPP
