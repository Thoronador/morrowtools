/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "ESMReaderContentsAll.hpp"
#include <iostream>
#include "SR_Constants.hpp"
#include "records/AcousticSpaceRecord.hpp"
#include "records/ActionRecord.hpp"
#include "records/ActivatorRecord.hpp"
#include "records/AddOnNodeRecord.hpp"
#include "records/AlchemyPotionRecord.hpp"
#include "records/AmmunitionRecord.hpp"
#include "records/AnimatedObjectRecord.hpp"
#include "records/ApparatusRecord.hpp"
#include "records/ArmourRecord.hpp"
#include "records/ArtObjectRecord.hpp"
#include "records/AssociationTypeRecord.hpp"
#include "records/BookRecord.hpp"
#include "records/CameraPathRecord.hpp"
#include "records/CameraShotRecord.hpp"
#include "records/CellRecord.hpp"
#include "records/CharacterReferenceRecord.hpp"
#include "records/ClassRecord.hpp"
#include "records/ClimateRecord.hpp"
#include "records/CollisionRecord.hpp"
#include "records/ColourFormRecord.hpp"
#include "records/CombatStyleRecord.hpp"
#include "records/ContainerRecord.hpp"
#include "records/CraftableObjectRecord.hpp"
#include "records/DebrisRecord.hpp"
#include "records/DialogBranchRecord.hpp"
#include "records/DialogueRecord.hpp"
#include "records/DialogViewRecord.hpp"
#include "records/DOBJRecord.hpp"
#include "records/DoorRecord.hpp"
#include "records/DualCastDataRecord.hpp"
#include "records/EffectShaderRecord.hpp"
#include "records/EnchantmentRecord.hpp"
#include "records/EncounterZoneRecord.hpp"
#include "records/EquipmentSlotRecord.hpp"
#include "records/ExplosionRecord.hpp"
#include "records/EyeRecord.hpp"
#include "records/FactionRecord.hpp"
#include "records/FloraRecord.hpp"
#include "records/FootstepRecord.hpp"
#include "records/FootstepSetRecord.hpp"
#include "records/FormListRecord.hpp"
#include "records/FurnitureRecord.hpp"
#include "records/GMSTRecord.hpp"
#include "records/GlobalRecord.hpp"
#include "records/GrassRecord.hpp"
#include "records/HazardRecord.hpp"
#include "records/HeadPartRecord.hpp"
#include "records/IdleAnimationRecord.hpp"
#include "records/ImageSpaceModifierRecord.hpp"
#include "records/ImageSpaceRecord.hpp"
#include "records/ImpactDataRecord.hpp"
#include "records/ImpactDataSetRecord.hpp"
#include "records/IngredientRecord.hpp"
#include "records/KeyRecord.hpp"
#include "records/KeywordRecord.hpp"
#include "records/LandscapeTextureRecord.hpp"
#include "records/LeveledCharacterRecord.hpp"
#include "records/LeveledItemRecord.hpp"
#include "records/LeveledSpellRecord.hpp"
#include "records/LightingTemplateRecord.hpp"
#include "records/LightRecord.hpp"
#include "records/LoadScreenRecord.hpp"
#include "records/LocationRecord.hpp"
#include "records/LocationReferenceTypeRecord.hpp"
#include "records/MagicEffectRecord.hpp"
#include "records/MaterialObjectRecord.hpp"
#include "records/MaterialTypeRecord.hpp"
#include "records/MessageRecord.hpp"
#include "records/MiscObjectRecord.hpp"
#include "records/MovableStaticRecord.hpp"
#include "records/MovementTypeRecord.hpp"
#include "records/MusicTrackRecord.hpp"
#include "records/MusicTypeRecord.hpp"
#include "records/NAVIRecord.hpp"
#include "records/NavMeshRecord.hpp"
#include "records/NPCRecord.hpp"
#include "records/OutfitRecord.hpp"
#include "records/PerkRecord.hpp"
#include "records/PlacedGrenadeRecord.hpp"
#include "records/PlacedHazardRecord.hpp"
#include "records/ProjectileRecord.hpp"
#include "records/QuestRecord.hpp"
#include "records/RaceRecord.hpp"
#include "records/ReferenceRecord.hpp"
#include "records/RelationshipRecord.hpp"
#include "records/ReverbRecord.hpp"
#include "records/ScrollRecord.hpp"
#include "records/ShaderParticleGeometryRecord.hpp"
#include "records/ShoutRecord.hpp"
#include "records/SoulGemRecord.hpp"
#include "records/SoundCategoryRecord.hpp"
#include "records/SoundDescriptorRecord.hpp"
#include "records/SoundRecord.hpp"
#include "records/SpellRecord.hpp"
#include "records/StaticRecord.hpp"
#include "records/TalkingActivatorRecord.hpp"
#include "records/TextureSetRecord.hpp"
#include "records/TreeRecord.hpp"
#include "records/VisualEffectRecord.hpp"
#include "records/VoiceTypeRecord.hpp"
#include "records/WaterTypeRecord.hpp"
#include "records/WeaponRecord.hpp"
#include "records/WordOfPowerRecord.hpp"
#include "records/WorldSpaceRecord.hpp"
#include "records/GenericRecord.hpp"

namespace SRTP
{

ESMReaderContentsAll::ESMReaderContentsAll()
{
  contents.removeContents();
}

ESMReaderContentsAll::~ESMReaderContentsAll()
{
  contents.removeContents();
}

bool ESMReaderContentsAll::needGroup(const GroupData& g_data) const
{
  return true;
}

int ESMReaderContentsAll::readNextRecord(std::ifstream& in_File, const uint32_t recName, const bool localized, const StringTable& table)
{
  BasicRecord * recPtr = NULL;
  switch (recName)
  {
    case cAACT:
         recPtr = new ActionRecord;
         break;
    case cACHR:
         recPtr = new CharacterReferenceRecord;
         break;
    case cACTI:
         recPtr = new ActivatorRecord;
         break;
    case cADDN:
         recPtr = new AddOnNodeRecord;
         break;
    case cALCH:
         recPtr = new AlchemyPotionRecord;
         break;
    case cAMMO:
         recPtr = new AmmunitionRecord;
         break;
    case cANIO:
         recPtr = new AnimatedObjectRecord;
         break;
    case cAPPA:
         recPtr = new ApparatusRecord;
         break;
    case cARMO:
         recPtr = new ArmourRecord;
         break;
    case cARTO:
         recPtr = new ArtObjectRecord;
         break;
    case cASPC:
         recPtr = new AcousticSpaceRecord;
         break;
    case cASTP:
         recPtr = new AssociationTypeRecord;
         break;
    case cBOOK:
         recPtr = new BookRecord;
         break;
    case cCAMS:
         recPtr = new CameraShotRecord;
         break;
    case cCELL:
         recPtr = new CellRecord;
         break;
    case cCLAS:
         recPtr = new ClassRecord;
         break;
    case cCLFM:
         recPtr = new ColourFormRecord;
         break;
    case cCLMT:
         recPtr = new ClimateRecord;
         break;
    case cCOBJ:
         recPtr = new CraftableObjectRecord;
         break;
    case cCOLL:
         recPtr = new CollisionRecord;
         break;
    case cCONT:
         recPtr = new ContainerRecord;
         break;
    case cCPTH:
         recPtr = new CameraPathRecord;
         break;
    case cCSTY:
         recPtr = new CombatStyleRecord;
         break;
    case cDEBR:
         recPtr = new DebrisRecord;
         break;
    case cDIAL:
         recPtr = new DialogueRecord;
         break;
    case cDLBR:
         recPtr = new DialogBranchRecord;
         break;
    case cDLVW:
         recPtr = new DialogViewRecord;
         break;
    case cDOBJ:
         recPtr = new DOBJRecord;
         break;
    case cDOOR:
         recPtr = new DoorRecord;
         break;
    case cDUAL:
         recPtr = new DualCastDataRecord;
         break;
    case cECZN:
         recPtr = new EncounterZoneRecord;
         break;
    case cEFSH:
         recPtr = new EffectShaderRecord;
         break;
    case cENCH:
         recPtr = new EnchantmentRecord;
         break;
    case cEQUP:
         recPtr = new EquipmentSlotRecord;
         break;
    case cEXPL:
         recPtr = new ExplosionRecord;
         break;
    case cEYES:
         recPtr = new EyeRecord;
         break;
    case cFACT:
         recPtr = new FactionRecord;
         break;
    case cFLOR:
         recPtr = new FloraRecord;
         break;
    case cFLST:
         recPtr = new FormListRecord;
         break;
    case cFSTP:
         recPtr = new FootstepRecord;
         break;
    case cFSTS:
         recPtr = new FootstepSetRecord;
         break;
    case cFURN:
         recPtr = new FurnitureRecord;
         break;
    case cGMST:
         recPtr = new GMSTRecord;
         break;
    case cGLOB:
         recPtr = new GlobalRecord;
         break;
    case cGRAS:
         recPtr = new GrassRecord;
         break;
    case cHAZD:
         recPtr = new HazardRecord;
         break;
    case cHDPT:
         recPtr = new HeadPartRecord;
         break;
    case cIDLE:
         recPtr = new IdleAnimationRecord;
         break;
    case cIMAD:
         recPtr = new ImageSpaceModifierRecord;
         break;
    case cIMGS:
         recPtr = new ImageSpaceRecord;
         break;
    case cIPCT:
         recPtr = new ImpactDataRecord;
         break;
    case cIPDS:
         recPtr = new ImpactDataSetRecord;
         break;
    case cINGR:
         recPtr = new IngredientRecord;
         break;
    case cKEYM:
         recPtr = new KeyRecord;
         break;
    case cKYWD:
         recPtr = new KeywordRecord;
         break;
    case cLCRT:
         recPtr = new LocationReferenceTypeRecord;
         break;
    case cLCTN:
         recPtr = new LocationRecord;
         break;
    case cLGTM:
         recPtr = new LightingTemplateRecord;
         break;
    case cLIGH:
         recPtr = new LightRecord;
         break;
    case cLSCR:
         recPtr = new LoadScreenRecord;
         break;
    case cLTEX:
         recPtr = new LandscapeTextureRecord;
         break;
    case cLVLI:
         recPtr = new LeveledItemRecord;
         break;
    case cLVLN:
         recPtr = new LeveledCharacterRecord;
         break;
    case cLVSP:
         recPtr = new LeveledSpellRecord;
         break;
    case cMATO:
         recPtr = new MaterialObjectRecord;
         break;
    case cMATT:
         recPtr = new MaterialTypeRecord;
         break;
    case cMESG:
         recPtr = new MessageRecord;
         break;
    case cMGEF:
         recPtr = new MagicEffectRecord;
         break;
    case cMISC:
         recPtr = new MiscObjectRecord;
         break;
    case cMOVT:
         recPtr = new MovementTypeRecord;
         break;
    case cMSTT:
         recPtr = new MovableStaticRecord;
         break;
    case cMUSC:
         recPtr = new MusicTypeRecord;
         break;
    case cMUST:
         recPtr = new MusicTrackRecord;
         break;
    case cNAVI:
         recPtr = new NAVIRecord;
         break;
    case cNAVM:
         recPtr = new NavMeshRecord;
         break;
    case cNPC_:
         recPtr = new NPCRecord;
         break;
    case cOTFT:
         recPtr = new OutfitRecord;
         break;
    case cPERK:
         recPtr = new PerkRecord;
         break;
    case cPGRE:
         recPtr = new PlacedGrenadeRecord;
         break;
    case cPHZD:
         recPtr = new PlacedHazardRecord;
         break;
    case cPROJ:
         recPtr = new ProjectileRecord;
         break;
    case cQUST:
         recPtr = new QuestRecord;
         break;
    case cRACE:
         recPtr = new RaceRecord;
         break;
    case cREFR:
         recPtr = new ReferenceRecord;
         break;
    case cRELA:
         recPtr = new RelationshipRecord;
         break;
    case cREVB:
         recPtr = new ReverbRecord;
         break;
    case cRFCT:
         recPtr = new VisualEffectRecord;
         break;
    case cSCRL:
         recPtr = new ScrollRecord;
         break;
    case cSHOU:
         recPtr = new ShoutRecord;
         break;
    case cSLGM:
         recPtr = new SoulGemRecord;
         break;
    case cSNCT:
         recPtr = new SoundCategoryRecord;
         break;
    case cSNDR:
         recPtr = new SoundDescriptorRecord;
         break;
    case cSOUN:
         recPtr = new SoundRecord;
         break;
    case cSPEL:
         recPtr = new SpellRecord;
         break;
    case cSPGD:
         recPtr = new ShaderParticleGeometryRecord;
         break;
    case cSTAT:
         recPtr = new StaticRecord;
         break;
    case cTACT:
         recPtr = new TalkingActivatorRecord;
         break;
    case cTREE:
         recPtr = new TreeRecord;
         break;
    case cTXST:
         recPtr = new TextureSetRecord;
         break;
    case cVTYP:
         recPtr = new VoiceTypeRecord;
         break;
    case cWATR:
         recPtr = new WaterTypeRecord;
         break;
    case cWEAP:
         recPtr = new WeaponRecord;
         break;
    case cWOOP:
         recPtr = new WordOfPowerRecord;
         break;
    case cWRLD:
         recPtr = new WorldSpaceRecord;
         break;
    default:
         //use generic record type for all other / unimplemented types
         recPtr = new GenericRecord;
         break;
  }//swi
  if (recPtr->loadFromStream(in_File, localized, table))
  {
    contents.m_Groups.back().addRecord(recPtr);
    return 1; //success
  }
  delete recPtr;
  recPtr = NULL;
  return -1; //error
}

} //namespace
