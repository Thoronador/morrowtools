/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014  Thoronador

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

#include "ESMReaderAll.h"

#include <iostream>
#include <stdexcept>
#include "../../mw/base/HelperIO.h"
#include "SR_Constants.h"
#include "AcousticSpaces.h"
#include "Actions.h"
#include "Activators.h"
#include "AddOnNodes.h"
#include "AlchemyPotions.h"
#include "Ammunitions.h"
#include "AnimatedObjects.h"
#include "Apparatuses.h"
#include "Armours.h"
#include "ArtObjects.h"
#include "AssociationTypes.h"
#include "Books.h"
#include "CameraPaths.h"
#include "CameraShots.h"
#include "Cells.h"
#include "CharacterReferences.h"
#include "Classes.h"
#include "Climates.h"
#include "Collisions.h"
#include "ColourForms.h"
#include "CombatStyles.h"
#include "Containers.h"
#include "CraftableObjects.h"
#include "Debris.h"
#include "DialogBranches.h"
#include "Dialogues.h"
#include "DialogViews.h"
#include "DOBJRecords.h"
#include "Doors.h"
#include "DualCastData.h"
#include "EffectShaders.h"
#include "Enchantments.h"
#include "EncounterZones.h"
#include "EquipmentSlots.h"
#include "Explosions.h"
#include "Eyes.h"
#include "Factions.h"
#include "Floras.h"
#include "Footsteps.h"
#include "FootstepSets.h"
#include "FormLists.h"
#include "Furniture.h"
#include "GameSettings.h"
#include "Globals.h"
#include "Grasses.h"
#include "Hazards.h"
#include "HeadParts.h"
#include "IdleAnimations.h"
#include "ImageSpaceModifiers.h"
#include "ImageSpaces.h"
#include "ImpactData.h"
#include "ImpactDataSets.h"
#include "Ingredients.h"
#include "Keys.h"
#include "Keywords.h"
#include "LandscapeTextures.h"
#include "LeveledCharacters.h"
#include "LeveledItems.h"
#include "LeveledSpells.h"
#include "LightingTemplates.h"
#include "Lights.h"
#include "LoadScreens.h"
#include "Locations.h"
#include "LocationReferenceTypes.h"
#include "MagicEffects.h"
#include "MaterialObjects.h"
#include "MaterialTypes.h"
#include "Messages.h"
#include "MiscObjects.h"
#include "MovableStatics.h"
#include "MovementTypes.h"
#include "MusicTracks.h"
#include "MusicTypes.h"
#include "NAVIRecords.h"
#include "NavMeshes.h"
#include "NPCs.h"
#include "Outfits.h"
#include "Perks.h"
#include "PlacedGrenades.h"
#include "PlacedHazards.h"
#include "Projectiles.h"
#include "Quests.h"
#include "Races.h"
#include "References.h"
#include "Relationships.h"
#include "Reverbs.h"
#include "Scrolls.h"
#include "Shouts.h"
#include "ShaderParticleGeometries.h"
#include "SoulGems.h"
#include "SoundCategories.h"
#include "SoundDescriptors.h"
#include "Sounds.h"
#include "Spells.h"
#include "Statics.h"
#include "TalkingActivators.h"
#include "TextureSets.h"
#include "Trees.h"
#include "VisualEffects.h"
#include "VoiceTypes.h"
#include "WaterTypes.h"
#include "Weapons.h"
#include "WordsOfPower.h"
#include "WorldSpaces.h"

namespace SRTP
{

ESMReaderAll::ESMReaderAll()
{
  //empty
}

ESMReaderAll::~ESMReaderAll()
{
  //empty
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
  //ESMReaderAll does not care about group structure
  return true;
}

int ESMReaderAll::readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table)
{
  switch (recName)
  {
    case cAACT:
         return Actions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cACHR:
         return CharacterReferences::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cACTI:
         return Activators::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cADDN:
         return AddOnNodes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cALCH:
         return AlchemyPotions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cAMMO:
         return Ammunitions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cANIO:
         return AnimatedObjects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cAPPA:
         return Apparatuses::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cARMO:
         return Armours::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cARTO:
         return ArtObjects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cASPC:
         return AcousticSpaces::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cASTP:
         return AssociationTypes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cBOOK:
         return Books::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCAMS:
         return CameraShots::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCELL:
         return Cells::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCLAS:
         return Classes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCLFM:
         return ColourForms::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCLMT:
         return Climates::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCOBJ:
         return CraftableObjects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCOLL:
         return Collisions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCONT:
         return Containers::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCPTH:
         return CameraPaths::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cCSTY:
         return CombatStyles::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cDEBR:
         return Debris::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cDIAL:
         return Dialogues::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cDLBR:
         return DialogBranches::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cDLVW:
         return DialogViews::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cDOBJ:
         return DOBJRecords::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cDOOR:
         return Doors::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cDUAL:
         return DualCastData::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cECZN:
         return EncounterZones::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cEFSH:
         return EffectShaders::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cENCH:
         return Enchantments::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cEQUP:
         return EquipmentSlots::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cEXPL:
         return Explosions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cEYES:
         return Eyes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFACT:
         return Factions::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFLOR:
         return Floras::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFLST:
         return FormLists::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFSTP:
         return Footsteps::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFSTS:
         return FootstepSets::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cFURN:
         return Furniture::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cGMST:
         return GameSettings::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cGLOB:
         return Globals::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cGRAS:
         return Grasses::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cHAZD:
         return Hazards::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cHDPT:
         return HeadParts::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cIDLE:
         return IdleAnimations::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cIMAD:
         return ImageSpaceModifiers::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cIMGS:
         return ImageSpaces::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cIPCT:
         return ImpactData::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cIPDS:
         return ImpactDataSets::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cINGR:
         return Ingredients::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cKEYM:
         return Keys::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cKYWD:
         return Keywords::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLCRT:
         return LocationReferenceTypes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLCTN:
         return Locations::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLGTM:
         return LightingTemplates::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLIGH:
         return Lights::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLSCR:
         return LoadScreens::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLTEX:
         return LandscapeTextures::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLVLI:
         return LeveledItems::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLVLN:
         return LeveledCharacters::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cLVSP:
         return LeveledSpells::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMATO:
         return MaterialObjects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMATT:
         return MaterialTypes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMESG:
         return Messages::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMGEF:
         return MagicEffects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMISC:
         return MiscObjects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMOVT:
         return MovementTypes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMSTT:
         return MovableStatics::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMUSC:
         return MusicTypes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cMUST:
         return MusicTracks::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cNAVI:
         return NAVIRecords::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cNAVM:
         return NavMeshes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cNPC_:
         return NPCs::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cOTFT:
         return Outfits::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cPERK:
         return Perks::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cPGRE:
         return PlacedGrenades::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cPHZD:
         return PlacedHazards::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cPROJ:
         return Projectiles::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cQUST:
         return Quests::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cRACE:
         return Races::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cREFR:
         return References::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cRELA:
         return Relationships::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cREVB:
         return Reverbs::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cRFCT:
         return VisualEffects::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSCRL:
         return Scrolls::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSHOU:
         return Shouts::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSLGM:
         return SoulGems::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSNCT:
         return SoundCategories::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSNDR:
         return SoundDescriptors::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSOUN:
         return Sounds::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSPEL:
         return Spells::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSPGD:
         return ShaderParticleGeometries::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cSTAT:
         return Statics::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cTACT:
         return TalkingActivators::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cTREE:
         return Trees::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cTXST:
         return TextureSets::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cVTYP:
         return VoiceTypes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cWATR:
         return WaterTypes::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cWEAP:
         return Weapons::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cWOOP:
         return WordsOfPower::getSingleton().readNextRecord(in_File, localized, table);
         break;
    case cWRLD:
         return WorldSpaces::getSingleton().readNextRecord(in_File, localized, table);
         break;
    default:
         //this branch should not be neccessary once the reader class is finished
         if (encounters.find(recName)==encounters.end())
         {
           std::cout << "Warning: unknown record type \""<< IntTo4Char(recName)
                     <<"\" encountered!\n    File position is "
                     <<(unsigned int) in_File.tellg()<<".\n";
           encounters.insert(recName);
         }
         return skipRecord(in_File);
         break;
  }//swi
  //we should never get to this point
  throw std::logic_error("ESMReaderAll::readNextRecord(): We should never get to this point!");
}

} //namespace
