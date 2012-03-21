/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012 Thoronador

    The Skyrim Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Skyrim Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Skyrim Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ESMReaderAll.h"

#include <iostream>
#include "../../mw/base/HelperIO.h"
#include "SR_Constants.h"
#include "AcousticSpaces.h"
#include "Actions.h"
#include "Activators.h"
#include "AlchemyPotions.h"
#include "Ammunitions.h"
#include "AnimatedObjects.h"
#include "Apparatuses.h"
#include "Armours.h"
#include "ArtObjects.h"
#include "AssociationTypes.h"
#include "Books.h"
#include "CameraShots.h"
#include "Cells.h"
#include "CharacterReferences.h"
#include "Classes.h"
#include "Collisions.h"
#include "ColourForms.h"
#include "Containers.h"
#include "CraftableObjects.h"
#include "Debris.h"
#include "DialogBranches.h"
#include "DialogViews.h"
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
#include "GameSettings.h"
#include "Globals.h"
#include "Grasses.h"
#include "Hazards.h"
#include "HeadParts.h"
#include "ImageSpaces.h"
#include "ImpactDataSets.h"
#include "Ingredients.h"
#include "Keys.h"
#include "Keywords.h"
#include "LandscapeTextures.h"
#include "LightingTemplates.h"
#include "LoadScreens.h"
#include "Locations.h"
#include "LocationReferenceTypes.h"
#include "MagicEffects.h"
#include "Messages.h"
#include "MiscObjects.h"
#include "MovementTypes.h"
#include "MusicTracks.h"
#include "MusicTypes.h"
#include "NAVIRecords.h"
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
#include "SoulGems.h"
#include "SoundCategories.h"
#include "Sounds.h"
#include "Spells.h"
#include "Statics.h"
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

void ESMReaderAll::nextGroupStarted(const GroupData& g_data, const bool sub)
{
  //empty
}

void ESMReaderAll::groupFinished(const GroupData& g_data)
{
  //empty, ESMReaderAll does not care about group structure
}

int ESMReaderAll::readNextRecord(std::ifstream& in_File, const int32_t recName)
{
  switch (recName)
  {
    case cAACT:
         return Actions::getSingleton().readNextRecord(in_File);
         break;
    case cACHR:
         return CharacterReferences::getSingleton().readNextRecord(in_File);
         break;
    case cACTI:
         return Activators::getSingleton().readNextRecord(in_File);
         break;
    case cALCH:
         return AlchemyPotions::getSingleton().readNextRecord(in_File);
         break;
    case cAMMO:
         return Ammunitions::getSingleton().readNextRecord(in_File);
         break;
    case cANIO:
         return AnimatedObjects::getSingleton().readNextRecord(in_File);
         break;
    case cAPPA:
         return Apparatuses::getSingleton().readNextRecord(in_File);
         break;
    case cARMO:
         return Armours::getSingleton().readNextRecord(in_File);
         break;
    case cARTO:
         return ArtObjects::getSingleton().readNextRecord(in_File);
         break;
    case cASPC:
         return AcousticSpaces::getSingleton().readNextRecord(in_File);
         break;
    case cASTP:
         return AssociationTypes::getSingleton().readNextRecord(in_File);
         break;
    case cBOOK:
         return Books::getSingleton().readNextRecord(in_File);
         break;
    case cCAMS:
         return CameraShots::getSingleton().readNextRecord(in_File);
         break;
    case cCELL:
         return Cells::getSingleton().readNextRecord(in_File);
         break;
    case cCLAS:
         return Classes::getSingleton().readNextRecord(in_File);
         break;
    case cCLFM:
         return ColourForms::getSingleton().readNextRecord(in_File);
         break;
    case cCOBJ:
         return CraftableObjects::getSingleton().readNextRecord(in_File);
         break;
    case cCOLL:
         return Collisions::getSingleton().readNextRecord(in_File);
         break;
    case cCONT:
         return Containers::getSingleton().readNextRecord(in_File);
         break;
    case cDEBR:
         return Debris::getSingleton().readNextRecord(in_File);
         break;
    case cDLBR:
         return DialogBranches::getSingleton().readNextRecord(in_File);
         break;
    case cDLVW:
         return DialogViews::getSingleton().readNextRecord(in_File);
         break;
    case cDOOR:
         return Doors::getSingleton().readNextRecord(in_File);
         break;
    case cDUAL:
         return DualCastData::getSingleton().readNextRecord(in_File);
         break;
    case cECZN:
         return EncounterZones::getSingleton().readNextRecord(in_File);
         break;
    case cEFSH:
         return EffectShaders::getSingleton().readNextRecord(in_File);
         break;
    case cENCH:
         return Enchantments::getSingleton().readNextRecord(in_File);
         break;
    case cEQUP:
         return EquipmentSlots::getSingleton().readNextRecord(in_File);
         break;
    case cEXPL:
         return Explosions::getSingleton().readNextRecord(in_File);
         break;
    case cEYES:
         return Eyes::getSingleton().readNextRecord(in_File);
         break;
    case cFACT:
         return Factions::getSingleton().readNextRecord(in_File);
         break;
    case cFLOR:
         return Floras::getSingleton().readNextRecord(in_File);
         break;
    case cFLST:
         return FormLists::getSingleton().readNextRecord(in_File);
         break;
    case cFSTP:
         return Footsteps::getSingleton().readNextRecord(in_File);
         break;
    case cFSTS:
         return FootstepSets::getSingleton().readNextRecord(in_File);
         break;
    case cGMST:
         return GameSettings::getSingleton().readNextRecord(in_File);
         break;
    case cGLOB:
         return Globals::getSingleton().readNextRecord(in_File);
         break;
    case cGRAS:
         return Grasses::getSingleton().readNextRecord(in_File);
         break;
    case cHAZD:
         return Hazards::getSingleton().readNextRecord(in_File);
         break;
    case cHDPT:
         return HeadParts::getSingleton().readNextRecord(in_File);
         break;
    case cIMGS:
         return ImageSpaces::getSingleton().readNextRecord(in_File);
         break;
    case cIPDS:
         return ImpactDataSets::getSingleton().readNextRecord(in_File);
         break;
    case cINGR:
         return Ingredients::getSingleton().readNextRecord(in_File);
         break;
    case cKEYM:
         return Keys::getSingleton().readNextRecord(in_File);
         break;
    case cKYWD:
         return Keywords::getSingleton().readNextRecord(in_File);
         break;
    case cLCRT:
         return LocationReferenceTypes::getSingleton().readNextRecord(in_File);
         break;
    case cLCTN:
         return Locations::getSingleton().readNextRecord(in_File);
         break;
    case cLGTM:
         return LightingTemplates::getSingleton().readNextRecord(in_File);
         break;
    case cLSCR:
         return LoadScreens::getSingleton().readNextRecord(in_File);
         break;
    case cLTEX:
         return LandscapeTextures::getSingleton().readNextRecord(in_File);
         break;
    case cMESG:
         return Messages::getSingleton().readNextRecord(in_File);
         break;
    case cMGEF:
         return MagicEffects::getSingleton().readNextRecord(in_File);
         break;
    case cMISC:
         return MiscObjects::getSingleton().readNextRecord(in_File);
         break;
    case cMOVT:
         return MovementTypes::getSingleton().readNextRecord(in_File);
         break;
    case cMUSC:
         return MusicTypes::getSingleton().readNextRecord(in_File);
         break;
    case cMUST:
         return MusicTracks::getSingleton().readNextRecord(in_File);
         break;
    case cNAVI:
         return NAVIRecords::getSingleton().readNextRecord(in_File);
         break;
    case cNPC_:
         return NPCs::getSingleton().readNextRecord(in_File);
         break;
    case cOTFT:
         return Outfits::getSingleton().readNextRecord(in_File);
         break;
    case cPERK:
         return Perks::getSingleton().readNextRecord(in_File);
         break;
    case cPGRE:
         return PlacedGrenades::getSingleton().readNextRecord(in_File);
         break;
    case cPHZD:
         return PlacedHazards::getSingleton().readNextRecord(in_File);
         break;
    case cPROJ:
         return Projectiles::getSingleton().readNextRecord(in_File);
         break;
    case cQUST:
         return Quests::getSingleton().readNextRecord(in_File);
         break;
    case cRACE:
         return Races::getSingleton().readNextRecord(in_File);
         break;
    case cREFR:
         return References::getSingleton().readNextRecord(in_File);
         break;
    case cRELA:
         return Relationships::getSingleton().readNextRecord(in_File);
         break;
    case cREVB:
         return Reverbs::getSingleton().readNextRecord(in_File);
         break;
    case cRFCT:
         return VisualEffects::getSingleton().readNextRecord(in_File);
         break;
    case cSCRL:
         return Scrolls::getSingleton().readNextRecord(in_File);
         break;
    case cSHOU:
         return Shouts::getSingleton().readNextRecord(in_File);
         break;
    case cSLGM:
         return SoulGems::getSingleton().readNextRecord(in_File);
         break;
    case cSNCT:
         return SoundCategories::getSingleton().readNextRecord(in_File);
         break;
    case cSOUN:
         return Sounds::getSingleton().readNextRecord(in_File);
         break;
    case cSPEL:
         return Spells::getSingleton().readNextRecord(in_File);
         break;
    case cSTAT:
         return Statics::getSingleton().readNextRecord(in_File);
         break;
    case cTREE:
         return Trees::getSingleton().readNextRecord(in_File);
         break;
    case cTXST:
         return TextureSets::getSingleton().readNextRecord(in_File);
         break;
    case cVTYP:
         return VoiceTypes::getSingleton().readNextRecord(in_File);
         break;
    case cWATR:
         return WaterTypes::getSingleton().readNextRecord(in_File);
         break;
    case cWEAP:
         return Weapons::getSingleton().readNextRecord(in_File);
         break;
    case cWOOP:
         return WordsOfPower::getSingleton().readNextRecord(in_File);
         break;
    case cWRLD:
         return WorldSpaces::getSingleton().readNextRecord(in_File);
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
  throw 42;
}

} //namespace
