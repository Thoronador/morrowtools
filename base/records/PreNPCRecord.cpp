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
#include "../MW_Constants.h"

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

bool PreNPCRecord::writeItemsSpellsAIDataDestinations(std::ofstream& output) const
{
  //items and spells
  unsigned int i, len;
  uint32_t SubLength;
  for (i=0; i<Items.size(); ++i)
  {
    //Items
    //write NPCO
    output.write((char*) &cNPCO, 4);
    SubLength = 36; //fixed length of 36 bytes
    //write NPCO's length
    output.write((char*) &SubLength, 4);
    //write item stuff
    // ---- count
    output.write((char*) &(Items[i].Count), 4);
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
    output.write((char*) &cNPCS, 4);
    SubLength = 32; //fixed length of 32 bytes
    //write NPCS's length
    output.write((char*) &SubLength, 4);
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
    output.write((char*) &cAIDT, 4);
    SubLength = 12; //fixed length of 12 bytes
    //write AIDT's length
    output.write((char*) &SubLength, 4);
    //write AI data
    output.write((char*) &(AIData.Hello), 1);
    output.write((char*) &(AIData.Unknown1), 1);
    output.write((char*) &(AIData.Fight), 1);
    output.write((char*) &(AIData.Flee), 1);
    output.write((char*) &(AIData.Alarm), 1);
    output.write((char*) &(AIData.Unknown2), 1);
    output.write((char*) &(AIData.Unknown3), 1);
    output.write((char*) &(AIData.Unknown4), 1);
    output.write((char*) &(AIData.Flags), 4);
  }//if AIData is present

  //AI packages
  NPC_AIActivate* activatePointer;
  NPC_AIEscortFollow* escort_followPointer;
  NPC_AITravel* travelPointer;
  NPC_AIWander* wanderPointer;
  for (i=0; i<AIPackages.size(); ++i)
  {
    switch (AIPackages[i]->getPackageType())
    {
      case ptActivate:
           //write AI_A
           output.write((char*) &cAI_A, 4);
           SubLength = 33; //fixed length of 33 bytes
           //write AI_A's length
           output.write((char*) &SubLength, 4);
           //write AI activate data
           activatePointer = static_cast<NPC_AIActivate*>(AIPackages[i]);
           // ---- write target ID
           len = activatePointer->TargetID.length();
           if (len>31)
           {
             len = 31;
           }
           output.write(activatePointer->TargetID.c_str(), len);
           // ---- fill rest with NUL
           output.write(NULof32, 32-len);
           // ---- reset flag
           output.write((char*) &(activatePointer->Reset), 1);
           break;
      case ptEscort:
      case ptFollow:
           if (AIPackages[i]->getPackageType()==ptEscort)
           {
             //write AI_E
             output.write((char*) &cAI_E, 4);
           }
           else
           {
             //write AI_F
             output.write((char*) &cAI_F, 4);
           }
           SubLength = 48; //fixed length of 48 bytes
           //write AI_E's/AI_F's length
           output.write((char*) &SubLength, 4);
           //write AI escort/follow data
           escort_followPointer = static_cast<NPC_AIEscortFollow*>(AIPackages[i]);
           output.write((char*) &(escort_followPointer->X), 4);
           output.write((char*) &(escort_followPointer->Y), 4);
           output.write((char*) &(escort_followPointer->Z), 4);
           output.write((char*) &(escort_followPointer->Duration), 2);
           // ---- write target ID
           len = escort_followPointer->TargetID.length();
           if (len>31)
           {
             len = 31;
           }
           output.write(escort_followPointer->TargetID.c_str(), len);
           // ---- fill rest with NUL
           output.write(NULof32, 32-len);
           // ---- reset flag
           output.write((char*) &(escort_followPointer->Reset), 2);

           //check for presence of cell
           if (escort_followPointer->CellName!="")
           {
             //write CNDT
             output.write((char*) &cCNDT, 4);
             SubLength = escort_followPointer->CellName.length()+1; //length of cell name +1 byte for NUL
             //write CNDT's length
             output.write((char*) &SubLength, 4);
             //write AI escort's/follow's cell name
             output.write(escort_followPointer->CellName.c_str(), SubLength);
           }
           break;
      case ptTravel:
           //write AI_T
           output.write((char*) &cAI_T, 4);
           SubLength = 16; //fixed length of 16 bytes
           //write AI_T's length
           output.write((char*) &SubLength, 4);
           //write AI travel data
           travelPointer = static_cast<NPC_AITravel*>(AIPackages[i]);
           output.write((char*) &(travelPointer->X), 4);
           output.write((char*) &(travelPointer->Y), 4);
           output.write((char*) &(travelPointer->Z), 4);
           output.write((char*) &(travelPointer->Reset), 4);
           break;
      case ptWander:
           //write AI_W
           output.write((char*) &cAI_W, 4);
           SubLength = 14; //fixed length of 14 bytes
           //write AI_W's length
           output.write((char*) &SubLength, 4);
           //write AI wander data
           wanderPointer = static_cast<NPC_AIWander*>(AIPackages[i]);
           output.write((char*) &(wanderPointer->Distance), 2);
           output.write((char*) &(wanderPointer->Duration), 2);
           output.write((char*) &(wanderPointer->Time), 1);
           output.write((char*) (wanderPointer->Idle), 8);
           output.write((char*) &(wanderPointer->Reset), 1);
           break;
    }//swi
  }//for AIPackages

  //travel service destinations
  for (i=0; i<Destinations.size(); ++i)
  {
    //write DODT
    output.write((char*) &cDODT, 4);
    SubLength = 24; //fixed length of 24 bytes
    //write DODT's length
    output.write((char*) &SubLength, 4);
    //write destination data
    output.write((char*) &(Destinations[i].XPos), 4);
    output.write((char*) &(Destinations[i].YPos), 4);
    output.write((char*) &(Destinations[i].ZPos), 4);
    output.write((char*) &(Destinations[i].XRot), 4);
    output.write((char*) &(Destinations[i].YRot), 4);
    output.write((char*) &(Destinations[i].ZRot), 4);
    //see if there's a cell name, too
    if (Destinations[i].CellName!="")
    {
      //write DNAM
      output.write((char*) &cDNAM, 4);
      SubLength = Destinations[i].CellName.length()+1; //length of cell name +1 byte for NUL
      //write DNAM's length
      output.write((char*) &SubLength, 4);
      //write destination cell name
      output.write(Destinations[i].CellName.c_str(), SubLength);
    }//if
  }//for

  return output.good();
}
