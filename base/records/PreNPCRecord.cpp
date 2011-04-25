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

#include "PreNPCRecord.h"

const int32_t pnfTraining = 16384;
const int32_t pnfSpellmaking = 32768;
const int32_t pnfEnchanting = 65536;
const int32_t pnfRepair = 131072;

PreNPCRecord::~PreNPCRecord()
{
  removeAIPackages();
}

bool PreNPCRecord::hasEqualAIPackages(const PreNPCRecord& other) const
{
  const size_t len = AIPackages.size();
  if (other.AIPackages.size()!=len) return false;
  unsigned int i;
  for (i=0; i<len; ++i)
  {
    if ((AIPackages.at(i)==NULL) xor (other.AIPackages.at(i)==NULL))
      return false;
    if (AIPackages.at(i)!=NULL)
    {
      //Do they have the same type?
      if (AIPackages.at(i)->getPackageType()!=other.AIPackages.at(i)->getPackageType())
      {
        return false;
      }
      switch (AIPackages.at(i)->getPackageType())
      {
        case ptActivate:
             if (!(dynamic_cast<NPC_AIActivate*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AIActivate*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptEscort:
        case ptFollow:
             if (!(dynamic_cast<NPC_AIEscortFollow*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AIEscortFollow*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptTravel:
             if (!(dynamic_cast<NPC_AITravel*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AITravel*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptWander:
             if (!(dynamic_cast<NPC_AIWander*>(AIPackages.at(i)))->equals(*dynamic_cast<NPC_AIWander*>(other.AIPackages.at(i))))
               return false;
             break;
      }//swi
    }//not NULL
  }//for
  return true;
}

void PreNPCRecord::removeAIPackages()
{
  while (!AIPackages.empty())
  {
    delete AIPackages.back();
    AIPackages.pop_back();
  }//while
}

void PreNPCRecord::copyAIPackages(const PreNPCRecord& source)
{
  unsigned int i;
  NPC_BasicAIPackage* pkgPtr;
  for (i=0; i<source.AIPackages.size(); ++i)
  {
    switch (source.AIPackages[i]->getPackageType())
    {
      case ptActivate:
           pkgPtr = new NPC_AIActivate;
           *pkgPtr = *(static_cast<NPC_AIActivate*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptEscort:
           pkgPtr = new NPC_AIEscort;
           *pkgPtr = *(static_cast<NPC_AIEscort*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptFollow:
           pkgPtr = new NPC_AIFollow;
           *pkgPtr = *(static_cast<NPC_AIFollow*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptTravel:
           pkgPtr = new NPC_AITravel;
           *pkgPtr = *(static_cast<NPC_AITravel*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
      case ptWander:
           pkgPtr = new NPC_AIWander;
           *pkgPtr = *(static_cast<NPC_AIWander*>(source.AIPackages[i]));
           AIPackages.push_back(pkgPtr);
           break;
    }//swi
  }//for
}

bool PreNPCRecord::isTrainer() const
{
  return (AIData.isPresent and ((AIData.Flags & pnfTraining)!=0));
}

bool PreNPCRecord::isEnchanter() const
{
  return (AIData.isPresent and ((AIData.Flags & pnfEnchanting)!=0));
}

bool PreNPCRecord::isSpellmaker() const
{
  return (AIData.isPresent and ((AIData.Flags & pnfSpellmaking)!=0));
}

bool PreNPCRecord::doesRepair() const
{
  return (AIData.isPresent and ((AIData.Flags & pnfRepair)!=0));
}
