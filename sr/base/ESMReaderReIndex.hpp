/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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

/* class ESMReaderReIndexMod:
     This descendant of the ESMReader class provides some basic functions and
     data members to allow re-indexing the mod index of the read records,
     depending on their load order.

     Still not complete yet. And not tested.
*/
class ESMReaderReIndexMod: public ESMReader
{
  public:
    /* constructor */
    ESMReaderReIndexMod(const std::vector<std::string>& loadOrder);

    /* destructor */
    virtual ~ESMReaderReIndexMod();

    /* non-binding request to update/re-calculate the index map for the current
       file header. It is up to the class whether and when the update is
       performed, because the ESMReaderReindexMod class itself does not
       implement any mechanism to process this request in a timely manner.
       It is up to the derived classes to do that. Or in other words:
       It's implementation-dependent if and when the request is fulfilled.

       parameters:
           currentModFile - name of the current .esm file without directory path
    */
    void requestIndexMapUpdate(const std::string& currentModFile);

    /* returns true, if the index map is flagged for update but has not been
       updated yet
    */
    bool indexMapsNeedsUpdate() const;
  protected:
    /* updates the index map for the current file header and returns true, if
       all went well. Returns false, if an error occurred.

       parameters:
           currentModFile - name of the current .esm file without directory path
    */
    bool updateIndexMap(const std::string& currentModFile);
    std::string m_CurrentMod;

    /* tries to remap the mod index of the given form ID and returns true in
       case of success, false in case of error.

       parameters:
           formID - the form ID that needs to be changed
    */
    bool reIndex(uint32_t& formID) const;
  private:
    std::vector<std::string> m_LoadOrder;
    bool m_MapIsUpToDate;
    std::map<uint8_t, uint8_t> m_IndexMap;
}; //class

} //namespace

#endif // SR_ESMREADERREINDEX_HPP
