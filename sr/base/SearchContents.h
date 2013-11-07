/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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

#ifndef SR_SEARCHCONTENTS_H
#define SR_SEARCHCONTENTS_H

#include <map>
#include <vector>
#include "records/ContainerRecord.h"
#include "records/FormListRecord.h"
#include "records/LeveledListBaseRecord.h"
#include "records/NPCRecord.h"

namespace SRTP
{

  namespace Search
  {

    //shallow search functions
    /*
    bool containsItemShallow(const std::vector<SRTP::ComponentData>& list, const uint32_t itemID);

    inline bool containsItemShallow(const NPCRecord& npc, const uint32_t itemID)
    {
      return containsItemShallow(npc.items, itemID);
    }

    inline bool containsItemShallow(const ContainerRecord& cont, const uint32_t itemID)
    {
      return containsItemShallow(cont.contents, itemID);
    }

    bool containsItemShallow(const LeveledListBaseRecord& llBase, const uint32_t itemID);
    */

    //deep search functions
    /* returns true, if the given leveled list contains the form ID itemID

       parameters:
           llBase - the leveled list
           itemID - the form ID of the item to search for
    */
    bool containsItemDeep(const LeveledListBaseRecord& llBase, const uint32_t itemID);

    /* returns true, if the given vector of components contains an entry with
       form ID itemID#

       parameter:
           list   - vector that shall be searched
           itemID - form ID to search for
    */
    bool containsItemDeep(const std::vector<SRTP::ComponentData>& list, const uint32_t itemID);

    /* returns true, if the given NPC record contains the form ID needleFormID

       parameters:
           npc          - the NPCRecord that shall be searched
           needleFormID - form ID to search for
           refToBase    - map of reference IDs (keys) to their base IDs (values)
           tradeOnly    - if set to true, only the items from the NPC's vendor
                          faction's merchant container will be searched

    */
    bool hasItemDeep(const NPCRecord& npc, const uint32_t needleFormID, const std::map<uint32_t, uint32_t>& refToBase, const bool tradeOnly);

    /* returns true, if the given container contains itemID

       parameters:
           cont   - the container record that shall be searched
           itemID - the form ID of the item to search for
    */
    inline bool containsItemDeep(const ContainerRecord& cont, const uint32_t itemID)
    {
      return containsItemDeep(cont.contents, itemID);
    }

    /* returns a vector of form IDs identifying the NPCs that have the item with the form ID itemID

       parameters:
           itemID - form ID of the item to search for
           refToBase - map of reference IDs (keys) to their base IDs (values)
           tradeOnly - if set to true, only the items from the NPC's vendor
                       faction's merchant container will be searched
    */
    std::vector<uint32_t> findNPCsWithItem(const uint32_t itemID, const std::map<uint32_t, uint32_t>& refToBase, const bool tradeOnly);

    /* returns true, if the given NPC record contains the specified keyword

       parameters:
           npc     - the NPC record whose keyword list shall be searched
           keyword - the keyword to search for
    */
    bool hasKeyword(const NPCRecord& npc, const uint32_t keyword);

    /* returns true, if the given form list contains needleID

       parameters:
           fList    - the FormListRecord that shall be searched
           needleID - the form ID of the search "keyword"
    */
    bool formListContains(const FormListRecord& fList, const uint32_t needleID);

    /* returns true, if the NPC's vendor faction */
    bool sellListAllowsKeyword(const NPCRecord& npc, const uint32_t keyword);

  }//namespace Search

}//namespace SRTP

#endif // SR_SEARCHCONTENTS_H
