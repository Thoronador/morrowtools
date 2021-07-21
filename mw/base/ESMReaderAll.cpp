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
