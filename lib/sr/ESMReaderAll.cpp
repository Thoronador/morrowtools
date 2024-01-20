/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014, 2021  Dirk Stolle

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
#include <stdexcept>
#include "../mw/HelperIO.hpp"
#include "SR_Constants.hpp"
#include "AcousticSpaces.hpp"
#include "Actions.hpp"
#include "Activators.hpp"
#include "AddOnNodes.hpp"
#include "AlchemyPotions.hpp"
#include "Ammunitions.hpp"
#include "AnimatedObjects.hpp"
#include "Apparatuses.hpp"
#include "Armours.hpp"
#include "ArtObjects.hpp"
#include "AssociationTypes.hpp"
#include "Books.hpp"
#include "CameraPaths.hpp"
#include "CameraShots.hpp"
#include "Cells.hpp"
#include "CharacterReferences.hpp"
#include "Classes.hpp"
#include "Climates.hpp"
#include "Collisions.hpp"
#include "ColourForms.hpp"
#include "CombatStyles.hpp"
#include "Containers.hpp"
#include "CraftableObjects.hpp"
#include "Debris.hpp"
#include "DialogBranches.hpp"
#include "Dialogues.hpp"
#include "DialogViews.hpp"
#include "DOBJRecords.hpp"
#include "Doors.hpp"
#include "DualCastData.hpp"
#include "EffectShaders.hpp"
#include "Enchantments.hpp"
#include "EncounterZones.hpp"
#include "EquipmentSlots.hpp"
#include "Explosions.hpp"
#include "Eyes.hpp"
#include "Factions.hpp"
#include "Floras.hpp"
#include "Footsteps.hpp"
#include "FootstepSets.hpp"
#include "FormLists.hpp"
#include "Furniture.hpp"
#include "GameSettings.hpp"
#include "Globals.hpp"
#include "Grasses.hpp"
#include "Hazards.hpp"
#include "HeadParts.hpp"
#include "IdleAnimations.hpp"
#include "ImageSpaceModifiers.hpp"
#include "ImageSpaces.hpp"
#include "ImpactData.hpp"
#include "ImpactDataSets.hpp"
#include "Ingredients.hpp"
#include "Keys.hpp"
#include "Keywords.hpp"
#include "LandscapeTextures.hpp"
#include "LeveledCharacters.hpp"
#include "LeveledItems.hpp"
#include "LeveledSpells.hpp"
#include "LightingTemplates.hpp"
#include "Lights.hpp"
#include "LoadScreens.hpp"
#include "Locations.hpp"
#include "LocationReferenceTypes.hpp"
#include "MagicEffects.hpp"
#include "MaterialObjects.hpp"
#include "MaterialTypes.hpp"
#include "Messages.hpp"
#include "MiscObjects.hpp"
#include "MovableStatics.hpp"
#include "MovementTypes.hpp"
#include "MusicTracks.hpp"
#include "MusicTypes.hpp"
#include "NAVIRecords.hpp"
#include "NavMeshes.hpp"
#include "NPCs.hpp"
#include "Outfits.hpp"
#include "Perks.hpp"
#include "PlacedGrenades.hpp"
#include "PlacedHazards.hpp"
#include "Projectiles.hpp"
#include "Quests.hpp"
#include "Races.hpp"
#include "References.hpp"
#include "Relationships.hpp"
#include "Reverbs.hpp"
#include "Scrolls.hpp"
#include "Shouts.hpp"
#include "ShaderParticleGeometries.hpp"
#include "SoulGems.hpp"
#include "SoundCategories.hpp"
#include "SoundDescriptors.hpp"
#include "Sounds.hpp"
#include "Spells.hpp"
#include "Statics.hpp"
#include "TalkingActivators.hpp"
#include "TextureSets.hpp"
#include "Trees.hpp"
#include "VisualEffects.hpp"
#include "VoiceTypes.hpp"
#include "WaterTypes.hpp"
#include "Weapons.hpp"
#include "WordsOfPower.hpp"
#include "WorldSpaces.hpp"

namespace SRTP
{

ESMReaderAll::ESMReaderAll()
{
}

ESMReaderAll::~ESMReaderAll()
{
}

bool ESMReaderAll::needGroup(const GroupData& g_data) const
{
  return true;
}

bool ESMReaderAll::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  return true;
}

bool ESMReaderAll::groupFinished(const GroupData& g_data)
{
  // ESMReaderAll does not care about group structure.
  return true;
}

int ESMReaderAll::readNextRecord(std::istream& input, const uint32_t recName, const bool localized, const StringTable& table)
{
  switch (recName)
  {
    case cAACT:
         return Actions::get().readNextRecord(input, localized, table);
         break;
    case cACHR:
         return CharacterReferences::get().readNextRecord(input, localized, table);
         break;
    case cACTI:
         return Activators::get().readNextRecord(input, localized, table);
         break;
    case cADDN:
         return AddOnNodes::get().readNextRecord(input, localized, table);
         break;
    case cALCH:
         return AlchemyPotions::get().readNextRecord(input, localized, table);
         break;
    case cAMMO:
         return Ammunitions::get().readNextRecord(input, localized, table);
         break;
    case cANIO:
         return AnimatedObjects::get().readNextRecord(input, localized, table);
         break;
    case cAPPA:
         return Apparatuses::get().readNextRecord(input, localized, table);
         break;
    case cARMO:
         return Armours::get().readNextRecord(input, localized, table);
         break;
    case cARTO:
         return ArtObjects::get().readNextRecord(input, localized, table);
         break;
    case cASPC:
         return AcousticSpaces::get().readNextRecord(input, localized, table);
         break;
    case cASTP:
         return AssociationTypes::get().readNextRecord(input, localized, table);
         break;
    case cBOOK:
         return Books::get().readNextRecord(input, localized, table);
         break;
    case cCAMS:
         return CameraShots::get().readNextRecord(input, localized, table);
         break;
    case cCELL:
         return Cells::get().readNextRecord(input, localized, table);
         break;
    case cCLAS:
         return Classes::get().readNextRecord(input, localized, table);
         break;
    case cCLFM:
         return ColourForms::get().readNextRecord(input, localized, table);
         break;
    case cCLMT:
         return Climates::get().readNextRecord(input, localized, table);
         break;
    case cCOBJ:
         return CraftableObjects::get().readNextRecord(input, localized, table);
         break;
    case cCOLL:
         return Collisions::get().readNextRecord(input, localized, table);
         break;
    case cCONT:
         return Containers::get().readNextRecord(input, localized, table);
         break;
    case cCPTH:
         return CameraPaths::get().readNextRecord(input, localized, table);
         break;
    case cCSTY:
         return CombatStyles::get().readNextRecord(input, localized, table);
         break;
    case cDEBR:
         return Debris::get().readNextRecord(input, localized, table);
         break;
    case cDIAL:
         return Dialogues::get().readNextRecord(input, localized, table);
         break;
    case cDLBR:
         return DialogBranches::get().readNextRecord(input, localized, table);
         break;
    case cDLVW:
         return DialogViews::get().readNextRecord(input, localized, table);
         break;
    case cDOBJ:
         return DOBJRecords::get().readNextRecord(input, localized, table);
         break;
    case cDOOR:
         return Doors::get().readNextRecord(input, localized, table);
         break;
    case cDUAL:
         return DualCastData::get().readNextRecord(input, localized, table);
         break;
    case cECZN:
         return EncounterZones::get().readNextRecord(input, localized, table);
         break;
    case cEFSH:
         return EffectShaders::get().readNextRecord(input, localized, table);
         break;
    case cENCH:
         return Enchantments::get().readNextRecord(input, localized, table);
         break;
    case cEQUP:
         return EquipmentSlots::get().readNextRecord(input, localized, table);
         break;
    case cEXPL:
         return Explosions::get().readNextRecord(input, localized, table);
         break;
    case cEYES:
         return Eyes::get().readNextRecord(input, localized, table);
         break;
    case cFACT:
         return Factions::get().readNextRecord(input, localized, table);
         break;
    case cFLOR:
         return Floras::get().readNextRecord(input, localized, table);
         break;
    case cFLST:
         return FormLists::get().readNextRecord(input, localized, table);
         break;
    case cFSTP:
         return Footsteps::get().readNextRecord(input, localized, table);
         break;
    case cFSTS:
         return FootstepSets::get().readNextRecord(input, localized, table);
         break;
    case cFURN:
         return Furniture::get().readNextRecord(input, localized, table);
         break;
    case cGMST:
         return GameSettings::get().readNextRecord(input, localized, table);
         break;
    case cGLOB:
         return Globals::get().readNextRecord(input, localized, table);
         break;
    case cGRAS:
         return Grasses::get().readNextRecord(input, localized, table);
         break;
    case cHAZD:
         return Hazards::get().readNextRecord(input, localized, table);
         break;
    case cHDPT:
         return HeadParts::get().readNextRecord(input, localized, table);
         break;
    case cIDLE:
         return IdleAnimations::get().readNextRecord(input, localized, table);
         break;
    case cIMAD:
         return ImageSpaceModifiers::get().readNextRecord(input, localized, table);
         break;
    case cIMGS:
         return ImageSpaces::get().readNextRecord(input, localized, table);
         break;
    case cIPCT:
         return ImpactData::get().readNextRecord(input, localized, table);
         break;
    case cIPDS:
         return ImpactDataSets::get().readNextRecord(input, localized, table);
         break;
    case cINGR:
         return Ingredients::get().readNextRecord(input, localized, table);
         break;
    case cKEYM:
         return Keys::get().readNextRecord(input, localized, table);
         break;
    case cKYWD:
         return Keywords::get().readNextRecord(input, localized, table);
         break;
    case cLCRT:
         return LocationReferenceTypes::get().readNextRecord(input, localized, table);
         break;
    case cLCTN:
         return Locations::get().readNextRecord(input, localized, table);
         break;
    case cLGTM:
         return LightingTemplates::get().readNextRecord(input, localized, table);
         break;
    case cLIGH:
         return Lights::get().readNextRecord(input, localized, table);
         break;
    case cLSCR:
         return LoadScreens::get().readNextRecord(input, localized, table);
         break;
    case cLTEX:
         return LandscapeTextures::get().readNextRecord(input, localized, table);
         break;
    case cLVLI:
         return LeveledItems::get().readNextRecord(input, localized, table);
         break;
    case cLVLN:
         return LeveledCharacters::get().readNextRecord(input, localized, table);
         break;
    case cLVSP:
         return LeveledSpells::get().readNextRecord(input, localized, table);
         break;
    case cMATO:
         return MaterialObjects::get().readNextRecord(input, localized, table);
         break;
    case cMATT:
         return MaterialTypes::get().readNextRecord(input, localized, table);
         break;
    case cMESG:
         return Messages::get().readNextRecord(input, localized, table);
         break;
    case cMGEF:
         return MagicEffects::get().readNextRecord(input, localized, table);
         break;
    case cMISC:
         return MiscObjects::get().readNextRecord(input, localized, table);
         break;
    case cMOVT:
         return MovementTypes::get().readNextRecord(input, localized, table);
         break;
    case cMSTT:
         return MovableStatics::get().readNextRecord(input, localized, table);
         break;
    case cMUSC:
         return MusicTypes::get().readNextRecord(input, localized, table);
         break;
    case cMUST:
         return MusicTracks::get().readNextRecord(input, localized, table);
         break;
    case cNAVI:
         return NAVIRecords::get().readNextRecord(input, localized, table);
         break;
    case cNAVM:
         return NavMeshes::get().readNextRecord(input, localized, table);
         break;
    case cNPC_:
         return NPCs::get().readNextRecord(input, localized, table);
         break;
    case cOTFT:
         return Outfits::get().readNextRecord(input, localized, table);
         break;
    case cPERK:
         return Perks::get().readNextRecord(input, localized, table);
         break;
    case cPGRE:
         return PlacedGrenades::get().readNextRecord(input, localized, table);
         break;
    case cPHZD:
         return PlacedHazards::get().readNextRecord(input, localized, table);
         break;
    case cPROJ:
         return Projectiles::get().readNextRecord(input, localized, table);
         break;
    case cQUST:
         return Quests::get().readNextRecord(input, localized, table);
         break;
    case cRACE:
         return Races::get().readNextRecord(input, localized, table);
         break;
    case cREFR:
         return References::get().readNextRecord(input, localized, table);
         break;
    case cRELA:
         return Relationships::get().readNextRecord(input, localized, table);
         break;
    case cREVB:
         return Reverbs::get().readNextRecord(input, localized, table);
         break;
    case cRFCT:
         return VisualEffects::get().readNextRecord(input, localized, table);
         break;
    case cSCRL:
         return Scrolls::get().readNextRecord(input, localized, table);
         break;
    case cSHOU:
         return Shouts::get().readNextRecord(input, localized, table);
         break;
    case cSLGM:
         return SoulGems::get().readNextRecord(input, localized, table);
         break;
    case cSNCT:
         return SoundCategories::get().readNextRecord(input, localized, table);
         break;
    case cSNDR:
         return SoundDescriptors::get().readNextRecord(input, localized, table);
         break;
    case cSOUN:
         return Sounds::get().readNextRecord(input, localized, table);
         break;
    case cSPEL:
         return Spells::get().readNextRecord(input, localized, table);
         break;
    case cSPGD:
         return ShaderParticleGeometries::get().readNextRecord(input, localized, table);
         break;
    case cSTAT:
         return Statics::get().readNextRecord(input, localized, table);
         break;
    case cTACT:
         return TalkingActivators::get().readNextRecord(input, localized, table);
         break;
    case cTREE:
         return Trees::get().readNextRecord(input, localized, table);
         break;
    case cTXST:
         return TextureSets::get().readNextRecord(input, localized, table);
         break;
    case cVTYP:
         return VoiceTypes::get().readNextRecord(input, localized, table);
         break;
    case cWATR:
         return WaterTypes::get().readNextRecord(input, localized, table);
         break;
    case cWEAP:
         return Weapons::get().readNextRecord(input, localized, table);
         break;
    case cWOOP:
         return WordsOfPower::get().readNextRecord(input, localized, table);
         break;
    case cWRLD:
         return WorldSpaces::get().readNextRecord(input, localized, table);
         break;
    default:
         // This branch should not be necessary once the reader class is finished.
         if (encounters.find(recName) == encounters.end())
         {
           std::cout << "Warning: unknown record type \"" << IntTo4Char(recName)
                     <<"\" encountered!\n    File position is "
                     << (unsigned int) input.tellg() << ".\n";
           encounters.insert(recName);
         }
         return skipRecord(input);
         break;
  }
  // We should never get to this point.
  throw std::logic_error("ESMReaderAll::readNextRecord(): We should never get to this point!");
}

} // namespace
