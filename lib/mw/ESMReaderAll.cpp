/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2012, 2022  Dirk Stolle

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
  // normally should be 4 char, but char is not eligible for switch
  uint32_t RecordName = 0;

  // read record name
  input.read(reinterpret_cast<char*>(&RecordName), 4);
  switch(RecordName)
  {
    case cACTI:
         return Activators::get().readNextRecord(input);
    case cALCH:
         return AlchemyPotions::get().readNextRecord(input);
    case cAPPA:
         return Apparatuses::get().readNextRecord(input);
    case cARMO:
         return Armours::get().readNextRecord(input);
    case cBODY:
         return BodyParts::get().readNextRecord(input);
    case cBOOK:
         return Books::get().readNextRecord(input);
    case cBSGN:
         return BirthSigns::get().readNextRecord(input);
    case cCELL:
         return Cells::get().readRecordCELL(input);
    case cCLAS:
         return Classes::get().readNextRecord(input);
    case cCLOT:
         return Clothings::get().readNextRecord(input);
    case cCONT:
         return Containers::get().readNextRecord(input);
    case cCREA:
         return Creatures::get().readNextRecord(input);
    case cDIAL:
         return DialogueTopics::get().readNextRecord(input);
    case cDOOR:
         return Doors::get().readNextRecord(input);
    case cENCH:
         return Enchantings::get().readNextRecord(input);
    case cFACT:
         return Factions::get().readNextRecord(input);
    case cGLOB:
         return Globals::get().readNextRecord(input);
    case cGMST:
         return GameSettings::get().readNextRecord(input);
    case cINFO:
         return DialogueInfos::get().readNextRecord(input);
    case cINGR:
         return Ingredients::get().readNextRecord(input);
    case cLAND:
         return Landscape::get().readRecordLAND(input);
    case cLEVC:
         return LeveledCreatures::get().readNextRecord(input);
    case cLEVI:
         return LeveledItems::get().readNextRecord(input);
    case cLIGH:
         return Lights::get().readNextRecord(input);
    case cLOCK:
         return Lockpicks::get().readNextRecord(input);
    case cLTEX:
         return LandscapeTextures::get().readNextRecord(input);
    case cMGEF:
         return MagicEffects::get().readNextRecord(input);
    case cMISC:
         return MiscItems::get().readNextRecord(input);
    case cNPC_:
         return NPCs::get().readNextRecord(input);
    case cPGRD:
         return PathGrids::get().readRecordPGRD(input);
    case cPROB:
         return Probes::get().readNextRecord(input);
    case cRACE:
         return Races::get().readNextRecord(input);
    case cREGN:
         return Regions::get().readNextRecord(input);
    case cREPA:
         return RepairItems::get().readNextRecord(input);
    case cSCPT:
         return Scripts::get().readNextRecord(input);
    case cSKIL:
         return Skills::get().readNextRecord(input);
    case cSNDG:
         return SoundGenerators::get().readNextRecord(input);
    case cSOUN:
         return Sounds::get().readNextRecord(input);
    case cSPEL:
         return Spells::get().readNextRecord(input);
    case cSSCR:
         return StartScripts::get().readNextRecord(input);
    case cSTAT:
         return Statics::get().readNextRecord(input);
    case cWEAP:
         return Weapons::get().readNextRecord(input);
    default:
         std::cerr << "processNextRecord: ERROR: unknown record type found: \""
                   << IntTo4Char(RecordName) << "\".\n"
                   << "Current file position: " << input.tellg() << " bytes.\n";
         return -1;
  }
}

} // namespace
