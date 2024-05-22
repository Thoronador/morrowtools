/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2021, 2022, 2024  Dirk Stolle

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

#include "ESMWriterAll.hpp"
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
#include "Ingredients.hpp"
#include "Landscape.hpp"
#include "LandscapeTextures.hpp"
#include "LeveledCreatures.hpp"
#include "LeveledItems.hpp"
#include "Lights.hpp"
#include "Lockpicks.hpp"
#include "MagicEffects.hpp"
#include "MiscItems.hpp"
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

ESMWriterAll::ESMWriterAll()
{
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
  return GameSettings::get().getNumberOfRecords()
       + Activators::get().getNumberOfRecords()
       + AlchemyPotions::get().getNumberOfRecords()
       + Apparatuses::get().getNumberOfRecords()
       + Armours::get().getNumberOfRecords()
       + BirthSigns::get().getNumberOfRecords()
       + BodyParts::get().getNumberOfRecords()
       + Books::get().getNumberOfRecords()
       + Cells::get().getNumberOfCells()
       + Classes::get().getNumberOfRecords()
       + Clothings::get().getNumberOfRecords()
       + Containers::get().getNumberOfRecords()
       + Creatures::get().getNumberOfRecords()
       + DialogueInfos::get().getNumberOfRecords()
       + DialogueTopics::get().getNumberOfRecords()
       + Doors::get().getNumberOfRecords()
       + Enchantings::get().getNumberOfRecords()
       + Factions::get().getNumberOfRecords()
       + Globals::get().getNumberOfRecords()
       + Ingredients::get().getNumberOfRecords()
       + Landscape::get().getNumberOfLandscapeRecords()
       + LandscapeTextures::get().getNumberOfRecords()
       + LeveledCreatures::get().getNumberOfRecords()
       + LeveledItems::get().getNumberOfRecords()
       + Lights::get().getNumberOfRecords()
       + Lockpicks::get().getNumberOfRecords()
       + MagicEffects::get().getNumberOfEffects()
       + MiscItems::get().getNumberOfRecords()
       + NPCs::get().getNumberOfRecords()
       + PathGrids::get().getNumberOfPathGrids()
       + Probes::get().getNumberOfRecords()
       + Races::get().getNumberOfRecords()
       + Regions::get().getNumberOfRecords()
       + RepairItems::get().getNumberOfRecords()
       + Scripts::get().getNumberOfRecords()
       + Skills::get().getNumberOfSkills()
       + Spells::get().getNumberOfRecords()
       + Statics::get().getNumberOfRecords()
       + StartScripts::get().getNumberOfRecords()
       + Sounds::get().getNumberOfRecords()
       + SoundGenerators::get().getNumberOfRecords()
       + Weapons::get().getNumberOfRecords();
}

bool ESMWriterAll::writeRecords(std::ostream& output) const
{
  if (!GameSettings::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Activators::get().saveAllToStream(output))
  {
    return false;
  }
  if (!AlchemyPotions::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Apparatuses::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Armours::get().saveAllToStream(output))
  {
    return false;
  }
  if (!BirthSigns::get().saveAllToStream(output))
  {
    return false;
  }
  if (!BodyParts::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Books::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Cells::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Classes::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Clothings::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Containers::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Creatures::get().saveAllToStream(output))
  {
    return false;
  }
  if (!DialogueInfos::get().saveAllToStream(output))
  {
    return false;
  }
  if (!DialogueTopics::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Doors::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Enchantings::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Factions::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Globals::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Ingredients::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Landscape::get().saveAllToStream(output))
  {
    return false;
  }
  if (!LandscapeTextures::get().saveAllToStream(output))
  {
    return false;
  }
  if (!LeveledCreatures::get().saveAllToStream(output))
  {
    return false;
  }
  if (!LeveledItems::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Lights::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Lockpicks::get().saveAllToStream(output))
  {
    return false;
  }
  if (!MagicEffects::get().saveAllToStream(output))
  {
    return false;
  }
  if (!MiscItems::get().saveAllToStream(output))
  {
    return false;
  }
  if (!NPCs::get().saveAllToStream(output))
  {
    return false;
  }
  if (!PathGrids::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Probes::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Races::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Regions::get().saveAllToStream(output))
  {
    return false;
  }
  if (!RepairItems::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Scripts::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Skills::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Statics::get().saveAllToStream(output))
  {
    return false;
  }
  if (!StartScripts::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Sounds::get().saveAllToStream(output))
  {
    return false;
  }
  if (!SoundGenerators::get().saveAllToStream(output))
  {
    return false;
  }
  if (!Weapons::get().saveAllToStream(output))
  {
    return false;
  }
  return Spells::get().saveAllToStream(output);
}

} // namespace
