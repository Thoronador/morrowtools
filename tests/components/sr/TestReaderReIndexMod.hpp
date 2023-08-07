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

#ifndef SR_TEST_READERREINDEXMOD_HPP
#define SR_TEST_READERREINDEXMOD_HPP

#include "../../../lib/sr/ESMReaderReIndex.hpp"

namespace SRTP
{

class TestReaderReIndexMod: public ESMReaderReIndexMod
{
  public:
    /** Constructor.
     *
     * \param loadOrder  file names of ESM files in load order
     */
    TestReaderReIndexMod(const std::vector<std::string>& loadOrder);

    // wrapper to get public access to protected updateIndexMap() method for tests
    inline bool updateIndexMap_public(const std::string& currentModFile)
    {
      return updateIndexMap(currentModFile);
    }

    // wrapper to get public access to protected reIndex() method for tests
    inline bool reIndex_public(uint32_t& formID) const
    {
      return reIndex(formID);
    }

    // utility method to artificially fill dependency list
    void addDependencyToHeaderData(const Tes4HeaderRecord::MasterFile& dep)
    {
      currentHead.dependencies.emplace_back(dep);
    }

    bool needGroup(const GroupData& g_data) const override;
    bool nextGroupStarted(const GroupData& g_data, const bool sub) override;
    int readGroup(std::istream& input, const GroupData& g_data, const bool localized, const StringTable& table) override;
    bool groupFinished(const GroupData& g_data) override;
    int readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table) override;
}; // class

} // namespace

#endif // SR_TEST_READERREINDEXMOD_HPP
