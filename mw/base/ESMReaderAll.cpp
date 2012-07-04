/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012  Thoronador

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

#include "ESMReaderAll.h"
#include <iostream>
#include "Activators.h"
#include "AlchemyPotions.h"
#include "Apparatuses.h"
#include "Armours.h"
#include "BirthSigns.h"
#include "BodyParts.h"
#include "Books.h"
#include "Cells.h"
#include "Classes.h"
#include "Clothings.h"
#include "Containers.h"
#include "Creatures.h"
#include "DialogueInfos.h"
#include "DialogueTopics.h"
#include "Doors.h"
#include "Enchantings.h"
#include "Factions.h"
#include "GameSettings.h"
#include "Globals.h"
#include "HelperIO.h"
#include "Ingredients.h"
#include "Landscape.h"
#include "LandscapeTextures.h"
#include "LeveledCreatures.h"
#include "LeveledItems.h"
#include "Lights.h"
#include "Lockpicks.h"
#include "MagicEffects.h"
#include "MiscItems.h"
#include "MW_Constants.h"
#include "NPCs.h"
#include "PathGrids.h"
#include "Probes.h"
#include "Races.h"
#include "Regions.h"
#include "RepairItems.h"
#include "Scripts.h"
#include "Skills.h"
#include "Spells.h"
#include "Statics.h"
#include "StartScripts.h"
#include "Sounds.h"
#include "SoundGenerators.h"
#include "Weapons.h"

namespace MWTP
{

ESMReaderAll::ESMReaderAll()
{
  //empty
}

ESMReaderAll::~ESMReaderAll()
{
  //empty
}

int ESMReaderAll::processNextRecord(std::ifstream& in_File)
{
  uint32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
         lastResult = Activators::getSingleton().readNextRecord(in_File);
         break;
    case cALCH:
         lastResult = AlchemyPotions::getSingleton().readNextRecord(in_File);
         break;
    case cAPPA:
         lastResult = Apparatuses::getSingleton().readNextRecord(in_File);
         break;
    case cARMO:
         lastResult = Armours::getSingleton().readNextRecord(in_File);
         break;
    case cBODY:
         lastResult = BodyParts::getSingleton().readNextRecord(in_File);
         break;
    case cBOOK:
         lastResult = Books::getSingleton().readNextRecord(in_File);
         break;
    case cBSGN:
         lastResult = BirthSigns::getSingleton().readNextRecord(in_File);
         break;
    case cCELL:
         lastResult = Cells::getSingleton().readRecordCELL(in_File);
         break;
    case cCLAS:
         lastResult = Classes::getSingleton().readNextRecord(in_File);
         break;
    case cCLOT:
         lastResult = Clothings::getSingleton().readNextRecord(in_File);
         break;
    case cCONT:
         lastResult = Containers::getSingleton().readNextRecord(in_File);
         break;
    case cCREA:
         lastResult = Creatures::getSingleton().readNextRecord(in_File);
         break;
    case cDIAL:
         lastResult = DialogueTopics::getSingleton().readRecordDIAL(in_File);
         break;
    case cDOOR:
         lastResult = Doors::getSingleton().readNextRecord(in_File);
         break;
    case cENCH:
         lastResult = Enchantings::getSingleton().readNextRecord(in_File);
         break;
    case cFACT:
         lastResult = Factions::getSingleton().readNextRecord(in_File);
         break;
    case cGLOB:
         lastResult = Globals::getSingleton().readNextRecord(in_File);
         break;
    case cGMST:
         lastResult = GameSettings::getSingleton().readRecordGMST(in_File);
         break;
    case cINFO:
         lastResult = DialogueInfos::getSingleton().readNextRecord(in_File);
         break;
    case cINGR:
         lastResult = Ingredients::getSingleton().readNextRecord(in_File);
         break;
    case cLAND:
         lastResult = Landscape::getSingleton().readRecordLAND(in_File);
         break;
    case cLEVC:
         lastResult = LeveledCreatures::getSingleton().readNextRecord(in_File);
         break;
    case cLEVI:
         lastResult = LeveledItems::getSingleton().readNextRecord(in_File);
         break;
    case cLIGH:
         lastResult = Lights::getSingleton().readNextRecord(in_File);
         break;
    case cLOCK:
         lastResult = Lockpicks::getSingleton().readNextRecord(in_File);
         break;
    case cLTEX:
         lastResult = LandscapeTextures::getSingleton().readNextRecord(in_File);
         break;
    case cMGEF:
         lastResult = MagicEffects::getSingleton().readRecordMGEF(in_File);
         break;
    case cMISC:
         lastResult = MiscItems::getSingleton().readNextRecord(in_File);
         break;
    case cNPC_:
         lastResult = NPCs::getSingleton().readNextRecord(in_File);
         break;
    case cPGRD:
         lastResult = PathGrids::getSingleton().readRecordPGRD(in_File);
         break;
    case cPROB:
         lastResult = Probes::getSingleton().readNextRecord(in_File);
         break;
    case cRACE:
         lastResult = Races::getSingleton().readNextRecord(in_File);
         break;
    case cREGN:
         lastResult = Regions::getSingleton().readNextRecord(in_File);
         break;
    case cREPA:
         lastResult = RepairItems::getSingleton().readNextRecord(in_File);
         break;
    case cSCPT:
         lastResult = Scripts::getSingleton().readNextRecord(in_File);
         break;
    case cSKIL:
         lastResult = Skills::getSingleton().readRecordSKIL(in_File);
         break;
    case cSNDG:
         lastResult = SoundGenerators::getSingleton().readNextRecord(in_File);
         break;
    case cSOUN:
         lastResult = Sounds::getSingleton().readNextRecord(in_File);
         break;
    case cSPEL:
         lastResult = Spells::getSingleton().readNextRecord(in_File);
         break;
    case cSSCR:
         lastResult = StartScripts::getSingleton().readNextRecord(in_File);
         break;
    case cSTAT:
         lastResult = Statics::getSingleton().readNextRecord(in_File);
         break;
    case cWEAP:
         lastResult = Weapons::getSingleton().readNextRecord(in_File);
         break;
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }//swi
  return lastResult;
}//processNextRecord of ESMReaderAll class

} //namespace
