/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013, 2022  Dirk Stolle

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

#ifndef SR_ESMREADERREINDEXMOD_HPP
#define SR_ESMREADERREINDEXMOD_HPP

#include "ESMReader.hpp"
#include <map>

namespace SRTP
{

/** This descendant of the ESMReader class provides some basic functions and
    data members to allow re-indexing the mod index of the read records,
    depending on their load order.
*/
class ESMReaderReIndexMod: public ESMReader
{
  public:
    /** Constructor.
     *
     * \param loadOrder   file names of ESM files in load order
     */
    ESMReaderReIndexMod(const std::vector<std::string>& loadOrder);

    virtual ~ESMReaderReIndexMod() = default;

    /** \brief Non-binding request to update/re-calculate the index map for the current file header.
     *
     * \param currentModFile  name of the current .esm file without directory path
     * \remarks It is up to the class whether and when the update is performed,
     *          because the ESMReaderReIndexMod class itself does not implement
     *          any mechanism to process this request in a timely manner.
     *          It is up to the derived classes to do that. Or in other words:
     *          It's implementation-dependent if and when the request is
     *          fulfilled.
     */
    void requestIndexMapUpdate(const std::string& currentModFile);

    /** \brief Checks whether the index map is flagged for update but has not
     * been updated yet.
     *
     * \return Returns true, if the index map is flagged for update but has not
     * been updated yet.
     */
    bool indexMapsNeedsUpdate() const;
  protected:
    /** \brief Updates the index map for the current file header.
     *
     * \param currentModFile  name of the current .esm file without directory path
     * \return Returns true, if all went well.
     *         Returns false, if an error occurred.
     */
    bool updateIndexMap(const std::string& currentModFile);

    std::string m_CurrentMod; /**< name of the current .esm file (no directory path) */

    /** \brief Tries to remap the mod index of the given form ID in place.
     *
     * \param formID  the form ID that needs to be changed
     * \return Returns true in case of success, false in case of error.
     */
    bool reIndex(uint32_t& formID) const;
  private:
    std::vector<std::string> m_LoadOrder; /**< global load order, including all ESM/ESP files */
    bool m_MapIsUpToDate; /**< whether the index map is current */
    std::map<uint8_t, uint8_t> m_IndexMap; /**< maps mod index of current mod to global mod index */
}; // class

} // namespace

#endif // SR_ESMREADERREINDEX_HPP
