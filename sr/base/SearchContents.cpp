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

#include "SearchContents.h"
#include "Books.h"
#include "Containers.h"
#include "Factions.h"
#include "FormLists.h"
#include "LeveledItems.h"
#include "NPCs.h"
#include "FormIDFunctions.h"

namespace SRTP
{

namespace Search
{

/*
bool containsItemShallow(const std::vector<SRTP::ComponentData>& list, const uint32_t itemID)
{
  unsigned int i;
  for (i=0; i<list.size(); ++i)
  {
    if (list[i].formID==itemID)
    {
      return true;
    }
  }//for
  return false;
}

bool containsItemShallow(const LeveledListBaseRecord& llBase, const uint32_t itemID)
{
  unsigned int i;
  for (i=0; i<llBase.entries.size(); ++i)
  {
    if (llBase.entries[i].entry.formID==itemID)
    {
      return true;
    }
  }//for
  return false;
}
*/

bool containsItemDeep(const LeveledListBaseRecord& llBase, const uint32_t itemID)
{
  unsigned int i;
  for (i=0; i<llBase.entries.size(); ++i)
  {
    if (llBase.entries[i].entry.formID==itemID)
    {
      return true;
    }
    //check, if there is a leveled list with that ID
    if (SRTP::LeveledItems::getSingleton().hasRecord(llBase.entries[i].entry.formID))
    {
      if (containsItemDeep(SRTP::LeveledItems::getSingleton().getRecord(llBase.entries[i].entry.formID), itemID))
        return true;
    }
  }//for
  return false;
}

bool containsItemDeep(const std::vector<SRTP::ComponentData>& list, const uint32_t needleFormID)
{
  unsigned int i;
  for (i=0; i<list.size(); ++i)
  {
    if (list[i].formID==needleFormID)
    {
      return true;
    }
    //check, if there is a leveled list with that ID
    if (SRTP::LeveledItems::getSingleton().hasRecord(list[i].formID))
    {
      if (containsItemDeep(SRTP::LeveledItems::getSingleton().getRecord(list[i].formID), needleFormID))
        return true;
    }
  }//for
  return false;
}

bool hasItemDeep(const NPCRecord& npc, const uint32_t needleFormID, const std::map<uint32_t, uint32_t>& refToBase, const bool tradeOnly)
{
  if (!tradeOnly and containsItemDeep(npc.items, needleFormID)) return true;
  //search factions
  unsigned int i;
  for (i=0; i<npc.factions.size(); ++i)
  {
    if (SRTP::Factions::getSingleton().hasRecord(npc.factions[i].formID))
    {
      const SRTP::FactionRecord& facRec = SRTP::Factions::getSingleton().getRecord(npc.factions[i].formID);
      if (facRec.isVendor())
      {
        //check vendor container
        if (facRec.vendorContainterFormID!=0)
        {
          const std::map<uint32_t, uint32_t>::const_iterator rtbIter = refToBase.find(facRec.vendorContainterFormID);
          if (rtbIter==refToBase.end())
          {
            //if (facRec.vendorContainterFormID!=0)
            std::cout << "Warning: No entry for reference ID "<< SRTP::getFormIDAsString(facRec.vendorContainterFormID)
                      << " in rtb map.\n";
          }
          else
          {
            //search the container
            if (SRTP::Containers::getSingleton().hasRecord(rtbIter->second))
            {
              //if (containsItemDeep(SRTP::Containers::getSingleton().getRecord(rtbIter->second), itemID))
              if (containsItemDeep(SRTP::Containers::getSingleton().getRecord(rtbIter->second), needleFormID))
              {
                //found it!
                return true;
              }
            }
            else if (rtbIter->second!=0)
            {
              std::cout << "Warning: No container for ID "<< SRTP::getFormIDAsString(rtbIter->second)
                        << " in database.\n";
            }
          }//else
        }
      }//if vendor
    }//if faction exists
    else
    {
      std::cout << "Warning: No faction entry for ID "<< SRTP::getFormIDAsString(npc.factions[i].formID)
                << ".\n";
    }
  }//for
  return false;
}

std::vector<uint32_t> findNPCsWithItem(const uint32_t itemID, const std::map<uint32_t, uint32_t>& refToBase, const bool tradeOnly)
{
  std::vector<uint32_t> result;
  SRTP::NPCs::ListIterator npcIter = SRTP::NPCs::getSingleton().getBegin();
  while (npcIter!=SRTP::NPCs::getSingleton().getEnd())
  {
    if (hasItemDeep(npcIter->second, itemID, refToBase, tradeOnly))
    {
      result.push_back(npcIter->first);
    }
    ++npcIter;
  }//while
  return result;
}

bool hasKeyword(const NPCRecord& npc, const uint32_t keyword)
{
  unsigned int i;
  for (i=0; i<npc.keywordArray.size(); ++i)
  {
    if (npc.keywordArray[i]==keyword) return true;
  }//for
  return false;
}

bool formListContains(const FormListRecord& fList, const uint32_t needleID)
{
  unsigned int i;
  for (i=0; i<fList.listFormIDs.size(); ++i)
  {
    if (fList.listFormIDs[i]==needleID) return true;
  }//for
  return false;
}

bool sellListAllowsKeyword(const NPCRecord& npc, const uint32_t keyword)
{
  unsigned int i;
  for (i=0; i<npc.factions.size(); ++i)
  {
    if (Factions::getSingleton().hasRecord(npc.factions[i].formID))
    {
      const FactionRecord& facRec = Factions::getSingleton().getRecord(npc.factions[i].formID);
      if (facRec.isVendor())
      {
        if (facRec.vendorListFormID!=0)
        {
          if (FormLists::getSingleton().hasRecord(facRec.vendorListFormID))
          {
            return (
            formListContains(FormLists::getSingleton().getRecord(facRec.vendorListFormID), keyword) and

            (facRec.vendorStuff.isPresent and ((facRec.vendorStuff.flagsVendor&FactionRecord::VendorData::cFlagNotBuySell)==0) )
                    );

          }
        }
        else
          return true;
      }
    }
  }//for
  return false;
}

}//namespace Search

}//namespace SRTP
