/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef MW_PRENPCRECORD_H
#define MW_PRENPCRECORD_H

#include <vector>
#include "BasicRecord.h"
#include "ItemRecord.h"
#include "AIData.h"
#include "AIPackages.h"
#include "TravelDestination.h"

namespace MWTP
{

/* the pre-NPC record - holds all (AI) data that is common to both NPCs and
   creatures
*/
struct PreNPCRecord: public BasicRecord
{
  //Items and spells
  std::vector<ItemRecord> Items;
  std::vector<std::string> NPC_Spells;

  //AI data
  NPC_AIData AIData;
  std::vector<NPC_BasicAIPackage*> AIPackages;

  //travel destinations
  std::vector<TravelDestination> Destinations;

  /* destructor */
  virtual ~PreNPCRecord();

  /* returns true, if the other PreNPCRecord has the same AI packages*/
  bool hasEqualAIPackages(const PreNPCRecord& other) const;

  /* returns true, if the trainer flag is set in the creature's/NPC's AI
     data flags */
  bool isTrainer() const;

  /* returns true, if the enchanting flag is set in the creature's/NPC's AI
     data flags */
  bool isEnchanter() const;

  /* returns true, if the spellmaking flag is set in the creature's/NPC's AI
     data flags */
  bool isSpellmaker() const;

  /* returns true, if the repair flag is set in the creature's/NPC's AI
     data flags */
  bool doesRepair() const;

  protected:
    /* removes all AI packages from the vector and deletes the pointed to memory
    */
    void removeAIPackages();

    /* adds the AI packages from the source record to this record*/
    void copyAIPackages(const PreNPCRecord& source);

    bool writeItemsSpellsAIDataDestinations(std::ofstream& output) const;
};//struct

} //namespace

#endif // MW_PRENPCRECORD_H
