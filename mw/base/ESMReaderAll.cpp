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
  //empty
}

ESMReaderAll::~ESMReaderAll()
{
  //empty
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
         lastResult = Activators::getSingleton().readNextRecord(input);
         break;
    case cALCH:
         lastResult = AlchemyPotions::getSingleton().readNextRecord(input);
         break;
    case cAPPA:
         lastResult = Apparatuses::getSingleton().readNextRecord(input);
         break;
    case cARMO:
         lastResult = Armours::getSingleton().readNextRecord(input);
         break;
    case cBODY:
         lastResult = BodyParts::getSingleton().readNextRecord(input);
         break;
    case cBOOK:
         lastResult = Books::getSingleton().readNextRecord(input);
         break;
    case cBSGN:
         lastResult = BirthSigns::getSingleton().readNextRecord(input);
         break;
    case cCELL:
         lastResult = Cells::getSingleton().readRecordCELL(input);
         break;
    case cCLAS:
         lastResult = Classes::getSingleton().readNextRecord(input);
         break;
    case cCLOT:
         lastResult = Clothings::getSingleton().readNextRecord(input);
         break;
    case cCONT:
         lastResult = Containers::getSingleton().readNextRecord(input);
         break;
    case cCREA:
         lastResult = Creatures::getSingleton().readNextRecord(input);
         break;
    case cDIAL:
         lastResult = DialogueTopics::getSingleton().readRecordDIAL(input);
         break;
    case cDOOR:
         lastResult = Doors::getSingleton().readNextRecord(input);
         break;
    case cENCH:
         lastResult = Enchantings::getSingleton().readNextRecord(input);
         break;
    case cFACT:
         lastResult = Factions::getSingleton().readNextRecord(input);
         break;
    case cGLOB:
         lastResult = Globals::getSingleton().readNextRecord(input);
         break;
    case cGMST:
         lastResult = GameSettings::getSingleton().readRecordGMST(input);
         break;
    case cINFO:
         lastResult = DialogueInfos::getSingleton().readNextRecord(input);
         break;
    case cINGR:
         lastResult = Ingredients::getSingleton().readNextRecord(input);
         break;
    case cLAND:
         lastResult = Landscape::getSingleton().readRecordLAND(input);
         break;
    case cLEVC:
         lastResult = LeveledCreatures::getSingleton().readNextRecord(input);
         break;
    case cLEVI:
         lastResult = LeveledItems::getSingleton().readNextRecord(input);
         break;
    case cLIGH:
         lastResult = Lights::getSingleton().readNextRecord(input);
         break;
    case cLOCK:
         lastResult = Lockpicks::getSingleton().readNextRecord(input);
         break;
    case cLTEX:
         lastResult = LandscapeTextures::getSingleton().readNextRecord(input);
         break;
    case cMGEF:
         lastResult = MagicEffects::getSingleton().readRecordMGEF(input);
         break;
    case cMISC:
         lastResult = MiscItems::getSingleton().readNextRecord(input);
         break;
    case cNPC_:
         lastResult = NPCs::getSingleton().readNextRecord(input);
         break;
    case cPGRD:
         lastResult = PathGrids::getSingleton().readRecordPGRD(input);
         break;
    case cPROB:
         lastResult = Probes::getSingleton().readNextRecord(input);
         break;
    case cRACE:
         lastResult = Races::getSingleton().readNextRecord(input);
         break;
    case cREGN:
         lastResult = Regions::getSingleton().readNextRecord(input);
         break;
    case cREPA:
         lastResult = RepairItems::getSingleton().readNextRecord(input);
         break;
    case cSCPT:
         lastResult = Scripts::getSingleton().readNextRecord(input);
         break;
    case cSKIL:
         lastResult = Skills::getSingleton().readRecordSKIL(input);
         break;
    case cSNDG:
         lastResult = SoundGenerators::getSingleton().readNextRecord(input);
         break;
    case cSOUN:
         lastResult = Sounds::getSingleton().readNextRecord(input);
         break;
    case cSPEL:
         lastResult = Spells::getSingleton().readNextRecord(input);
         break;
    case cSSCR:
         lastResult = StartScripts::getSingleton().readNextRecord(input);
         break;
    case cSTAT:
         lastResult = Statics::getSingleton().readNextRecord(input);
         break;
    case cWEAP:
         lastResult = Weapons::getSingleton().readNextRecord(input);
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
