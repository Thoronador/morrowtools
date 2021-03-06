/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012  Thoronador

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

#include "ESMWriterAll.h"
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
#include "Ingredients.h"
#include "Landscape.h"
#include "LandscapeTextures.h"
#include "LeveledCreatures.h"
#include "LeveledItems.h"
#include "Lights.h"
#include "Lockpicks.h"
#include "MagicEffects.h"
#include "MiscItems.h"
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

ESMWriterAll::ESMWriterAll()
{
  //empty
}

ESMWriterAll::~ESMWriterAll()
{
  //empty
}

int32_t ESMWriterAll::getTotalRecords() const
{
  //only writes activators, potions, apparatuses, armours, body parts, books,
  //    cells, classes, clothings, containers, creatures, dialogue infos,
  //    dialogue topics, doors, enchanting effects, factions, globals, landscape
  //    records, landscape textures, leveled creatures, leveled items, lights,
  //    lockpicks, magic effects, misc. items, NPCs, path grids, probes, races,
  //    regions, repair items, scripts, skills, spells, statics, start scripts,
  //    sounds, sound generators, weapons and game settings at the moment
  return GameSettings::getSingleton().getNumberOfSettings()
        +Activators::getSingleton().getNumberOfRecords()
        +AlchemyPotions::getSingleton().getNumberOfRecords()
        +Apparatuses::getSingleton().getNumberOfRecords()
        +Armours::getSingleton().getNumberOfRecords()
        +BirthSigns::getSingleton().getNumberOfRecords()
        +BodyParts::getSingleton().getNumberOfRecords()
        +Books::getSingleton().getNumberOfRecords()
        +Cells::getSingleton().getNumberOfCells()
        +Classes::getSingleton().getNumberOfRecords()
        +Clothings::getSingleton().getNumberOfRecords()
        +Containers::getSingleton().getNumberOfRecords()
        +Creatures::getSingleton().getNumberOfRecords()
        +DialogueInfos::getSingleton().getNumberOfRecords()
        +DialogueTopics::getSingleton().getNumberOfDialogueTopics()
        +Doors::getSingleton().getNumberOfRecords()
        +Enchantings::getSingleton().getNumberOfRecords()
        +Factions::getSingleton().getNumberOfRecords()
        +Globals::getSingleton().getNumberOfRecords()
        +Ingredients::getSingleton().getNumberOfRecords()
        +Landscape::getSingleton().getNumberOfLandscapeRecords()
        +LandscapeTextures::getSingleton().getNumberOfRecords()
        +LeveledCreatures::getSingleton().getNumberOfRecords()
        +LeveledItems::getSingleton().getNumberOfRecords()
        +Lights::getSingleton().getNumberOfRecords()
        +Lockpicks::getSingleton().getNumberOfRecords()
        +MagicEffects::getSingleton().getNumberOfEffects()
        +MiscItems::getSingleton().getNumberOfRecords()
        +NPCs::getSingleton().getNumberOfRecords()
        +PathGrids::getSingleton().getNumberOfPathGrids()
        +Probes::getSingleton().getNumberOfRecords()
        +Races::getSingleton().getNumberOfRecords()
        +Regions::getSingleton().getNumberOfRecords()
        +RepairItems::getSingleton().getNumberOfRecords()
        +Scripts::getSingleton().getNumberOfRecords()
        +Skills::getSingleton().getNumberOfSkills()
        +Spells::getSingleton().getNumberOfRecords()
        +Statics::getSingleton().getNumberOfRecords()
        +StartScripts::getSingleton().getNumberOfRecords()
        +Sounds::getSingleton().getNumberOfRecords()
        +SoundGenerators::getSingleton().getNumberOfRecords()
        +Weapons::getSingleton().getNumberOfRecords();
}

bool ESMWriterAll::writeRecords(std::ofstream& output) const
{
  if (!GameSettings::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Activators::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!AlchemyPotions::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Apparatuses::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Armours::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!BirthSigns::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!BodyParts::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Books::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Cells::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Classes::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Clothings::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Containers::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Creatures::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!DialogueInfos::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!DialogueTopics::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Doors::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Enchantings::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Factions::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Globals::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Ingredients::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Landscape::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!LandscapeTextures::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!LeveledCreatures::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!LeveledItems::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Lights::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Lockpicks::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!MagicEffects::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!MiscItems::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!NPCs::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!PathGrids::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Probes::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Races::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Regions::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!RepairItems::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Scripts::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Skills::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Statics::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!StartScripts::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Sounds::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!SoundGenerators::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  if (!Weapons::getSingleton().saveAllToStream(output))
  {
    return false;
  }
  return Spells::getSingleton().saveAllToStream(output);
}

} //namespace
