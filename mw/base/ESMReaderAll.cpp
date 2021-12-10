/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012  Thoronador

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

#include "ESMReaderAll.hpp"
#include <iostream>
#include "Activators.hpp"
#include "AlchemyPotions.hpp"
#include "Apparatuses.hpp"
#include "Armours.hpp"
#include "BirthSigns.hpp"
#include "BodyParts.hpp"
#include "Books.hpp"
#include "Cells.hpp"
#include "Classes.hpp"
#include "Clothings.hpp"
#include "Containers.hpp"
#include "Creatures.hpp"
#include "DialogueInfos.hpp"
#include "DialogueTopics.hpp"
#include "Doors.hpp"
#include "Enchantings.hpp"
#include "Factions.hpp"
#include "GameSettings.hpp"
#include "Globals.hpp"
#include "HelperIO.hpp"
#include "Ingredients.hpp"
#include "Landscape.hpp"
#include "LandscapeTextures.hpp"
#include "LeveledCreatures.hpp"
#include "LeveledItems.hpp"
#include "Lights.hpp"
#include "Lockpicks.hpp"
#include "MagicEffects.hpp"
#include "MiscItems.hpp"
#include "MW_Constants.hpp"
#include "NPCs.hpp"
#include "PathGrids.hpp"
#include "Probes.hpp"
#include "Races.hpp"
#include "Regions.hpp"
#include "RepairItems.hpp"
#include "Scripts.hpp"
#include "Skills.hpp"
#include "Spells.hpp"
#include "Statics.hpp"
#include "StartScripts.hpp"
#include "Sounds.hpp"
#include "SoundGenerators.hpp"
#include "Weapons.hpp"

namespace MWTP
{

ESMReaderAll::ESMReaderAll()
{
}

int ESMReaderAll::processNextRecord(std::istream& input)
{
  uint32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  //read record name
  input.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
         lastResult = Activators::get().readNextRecord(input);
         break;
    case cALCH:
         lastResult = AlchemyPotions::get().readNextRecord(input);
         break;
    case cAPPA:
         lastResult = Apparatuses::get().readNextRecord(input);
         break;
    case cARMO:
         lastResult = Armours::get().readNextRecord(input);
         break;
    case cBODY:
         lastResult = BodyParts::get().readNextRecord(input);
         break;
    case cBOOK:
         lastResult = Books::get().readNextRecord(input);
         break;
    case cBSGN:
         lastResult = BirthSigns::get().readNextRecord(input);
         break;
    case cCELL:
         lastResult = Cells::get().readRecordCELL(input);
         break;
    case cCLAS:
         lastResult = Classes::get().readNextRecord(input);
         break;
    case cCLOT:
         lastResult = Clothings::get().readNextRecord(input);
         break;
    case cCONT:
         lastResult = Containers::get().readNextRecord(input);
         break;
    case cCREA:
         lastResult = Creatures::get().readNextRecord(input);
         break;
    case cDIAL:
         lastResult = DialogueTopics::get().readRecordDIAL(input);
         break;
    case cDOOR:
         lastResult = Doors::get().readNextRecord(input);
         break;
    case cENCH:
         lastResult = Enchantings::get().readNextRecord(input);
         break;
    case cFACT:
         lastResult = Factions::get().readNextRecord(input);
         break;
    case cGLOB:
         lastResult = Globals::get().readNextRecord(input);
         break;
    case cGMST:
         lastResult = GameSettings::get().readNextRecord(input);
         break;
    case cINFO:
         lastResult = DialogueInfos::get().readNextRecord(input);
         break;
    case cINGR:
         lastResult = Ingredients::get().readNextRecord(input);
         break;
    case cLAND:
         lastResult = Landscape::get().readRecordLAND(input);
         break;
    case cLEVC:
         lastResult = LeveledCreatures::get().readNextRecord(input);
         break;
    case cLEVI:
         lastResult = LeveledItems::get().readNextRecord(input);
         break;
    case cLIGH:
         lastResult = Lights::get().readNextRecord(input);
         break;
    case cLOCK:
         lastResult = Lockpicks::get().readNextRecord(input);
         break;
    case cLTEX:
         lastResult = LandscapeTextures::get().readNextRecord(input);
         break;
    case cMGEF:
         lastResult = MagicEffects::get().readRecordMGEF(input);
         break;
    case cMISC:
         lastResult = MiscItems::get().readNextRecord(input);
         break;
    case cNPC_:
         lastResult = NPCs::get().readNextRecord(input);
         break;
    case cPGRD:
         lastResult = PathGrids::get().readRecordPGRD(input);
         break;
    case cPROB:
         lastResult = Probes::get().readNextRecord(input);
         break;
    case cRACE:
         lastResult = Races::get().readNextRecord(input);
         break;
    case cREGN:
         lastResult = Regions::get().readNextRecord(input);
         break;
    case cREPA:
         lastResult = RepairItems::get().readNextRecord(input);
         break;
    case cSCPT:
         lastResult = Scripts::get().readNextRecord(input);
         break;
    case cSKIL:
         lastResult = Skills::get().readRecordSKIL(input);
         break;
    case cSNDG:
         lastResult = SoundGenerators::get().readNextRecord(input);
         break;
    case cSOUN:
         lastResult = Sounds::get().readNextRecord(input);
         break;
    case cSPEL:
         lastResult = Spells::get().readNextRecord(input);
         break;
    case cSSCR:
         lastResult = StartScripts::get().readNextRecord(input);
         break;
    case cSTAT:
         lastResult = Statics::get().readNextRecord(input);
         break;
    case cWEAP:
         lastResult = Weapons::get().readNextRecord(input);
         break;
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: " << input.tellg() << " bytes.\n";
         lastResult = -1;
         break;
  }
  return lastResult;
}

} //namespace
