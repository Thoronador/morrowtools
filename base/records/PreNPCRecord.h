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

#ifndef PRENPCRECORD_H
#define PRENPCRECORD_H

#include <vector>
#include "BasicRecord.h"
#include "AIData.h"
#include "AIPackages.h"
#include "TravelDestination.h"

/* the pre-NPC record - holds all (AI) data that is common to both NPCs and
   creatures
*/
struct PreNPCRecord: public BasicRecord
{
  //AI data
  NPC_AIData AIData;
  std::vector<NPC_BasicAIPackage*> AIPackages;

  //travel destinations
  std::vector<TravelDestination> Destinations;

  /* destructor */
  ~PreNPCRecord();

  /* returns true, if the other PreNPCRecord has the same AI packages*/
  bool hasEqualAIPackages(const PreNPCRecord& other) const;

  protected:
    void removeAIPackages();
};//struct

#endif // PRENPCRECORD_H
