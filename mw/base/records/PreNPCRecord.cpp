/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
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

#include "PreNPCRecord.hpp"
#include "../MW_Constants.hpp"

namespace MWTP
{

const int32_t pnfTraining = 16384;
const int32_t pnfSpellmaking = 32768;
const int32_t pnfEnchanting = 65536;
const int32_t pnfRepair = 131072;

PreNPCRecord::PreNPCRecord()
: BasicRecord(),
  Items(std::vector<ItemRecord>()),
  NPC_Spells(std::vector<std::string>()),
  //AI data
  AIData(NPC_AIData()),
  AIPackages(std::vector<NPC_BasicAIPackage*>()),
  //travel destinations
  Destinations(std::vector<TravelDestination>())
{
}

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
             if (!(static_cast<NPC_AIActivate*>(AIPackages.at(i)))->equals(*static_cast<NPC_AIActivate*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptEscort:
        case ptFollow:
             if (!(static_cast<NPC_AIEscortFollow*>(AIPackages.at(i)))->equals(*static_cast<NPC_AIEscortFollow*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptTravel:
             if (!(static_cast<NPC_AITravel*>(AIPackages.at(i)))->equals(*static_cast<NPC_AITravel*>(other.AIPackages.at(i))))
               return false;
             break;
        case ptWander:
             if (!(static_cast<NPC_AIWander*>(AIPackages.at(i)))->equals(*static_cast<NPC_AIWander*>(other.AIPackages.at(i))))
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

#ifndef MW_UNSAVEABLE_RECORDS
bool PreNPCRecord::writeItemsSpellsAIDataDestinations(std::ofstream& output) const
{
  //items and spells
  unsigned int i, len;
  uint32_t SubLength;
  for (i=0; i<Items.size(); ++i)
  {
    //Items
    //write NPCO
    output.write((const char*) &cNPCO, 4);
    SubLength = 36; //fixed length of 36 bytes
    //write NPCO's length
    output.write((const char*) &SubLength, 4);
    //write item stuff
    // ---- count
    output.write((const char*) &(Items[i].Count), 4);
    // ---- item ID
    len = Items[i].Item.length();
    if (len>31)
    {
      len = 31;
    }
    output.write(Items[i].Item.c_str(), len);
    //fill rest with NUL bytes
    output.write(NULof32, 32-len);
  }//for

  for (i=0; i<NPC_Spells.size(); ++i)
  {
    //Spells
    //write NPCS
    output.write((const char*) &cNPCS, 4);
    SubLength = 32; //fixed length of 32 bytes
    //write NPCS's length
    output.write((const char*) &SubLength, 4);
    //write spell ID
    len = NPC_Spells[i].length();
    if (len>31)
    {
      len = 31;
    }
    output.write(NPC_Spells[i].c_str(), len);
    //fill rest with NUL bytes
    output.write(NULof32, 32-len);
  }//for

  if (AIData.isPresent)
  {
    //write AIDT
    output.write((const char*) &cAIDT, 4);
    SubLength = 12; //fixed length of 12 bytes
    //write AIDT's length
    output.write((const char*) &SubLength, 4);
    //write AI data
    output.write((const char*) &(AIData.Hello), 1);
    output.write((const char*) &(AIData.Unknown1), 1);
    output.write((const char*) &(AIData.Fight), 1);
    output.write((const char*) &(AIData.Flee), 1);
    output.write((const char*) &(AIData.Alarm), 1);
    output.write((const char*) &(AIData.Unknown2), 1);
    output.write((const char*) &(AIData.Unknown3), 1);
    output.write((const char*) &(AIData.Unknown4), 1);
    output.write((const char*) &(AIData.Flags), 4);
  }//if AIData is present

  //AI packages
  for (i=0; i<AIPackages.size(); ++i)
  {
    AIPackages[i]->saveToStream(output);
  }//for AIPackages

  //travel service destinations
  for (i=0; i<Destinations.size(); ++i)
  {
    //write DODT
    output.write((const char*) &cDODT, 4);
    SubLength = 24; //fixed length of 24 bytes
    //write DODT's length
    output.write((const char*) &SubLength, 4);
    //write destination data
    output.write((const char*) &(Destinations[i].XPos), 4);
    output.write((const char*) &(Destinations[i].YPos), 4);
    output.write((const char*) &(Destinations[i].ZPos), 4);
    output.write((const char*) &(Destinations[i].XRot), 4);
    output.write((const char*) &(Destinations[i].YRot), 4);
    output.write((const char*) &(Destinations[i].ZRot), 4);
    //see if there's a cell name, too
    if (!Destinations[i].CellName.empty())
    {
      //write DNAM
      output.write((const char*) &cDNAM, 4);
      SubLength = Destinations[i].CellName.length()+1; //length of cell name +1 byte for NUL
      //write DNAM's length
      output.write((const char*) &SubLength, 4);
      //write destination cell name
      output.write(Destinations[i].CellName.c_str(), SubLength);
    }//if
  }//for

  return output.good();
}
#endif

} //namespace
