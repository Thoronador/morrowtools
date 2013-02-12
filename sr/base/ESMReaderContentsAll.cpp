/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include "ESMReaderContentsAll.h"
#include <iostream>
#include "SR_Constants.h"
#include "records/AcousticSpaceRecord.h"
#include "records/ActionRecord.h"
#include "records/ActivatorRecord.h"
#include "records/AddOnNodeRecord.h"
#include "records/AlchemyPotionRecord.h"
#include "records/AmmunitionRecord.h"
#include "records/AnimatedObjectRecord.h"
#include "records/ApparatusRecord.h"
#include "records/ArmourRecord.h"
#include "records/ArtObjectRecord.h"
#include "records/AssociationTypeRecord.h"
#include "records/BookRecord.h"
#include "records/CameraPathRecord.h"
#include "records/CameraShotRecord.h"
#include "records/CellRecord.h"
#include "records/CharacterReferenceRecord.h"
#include "records/ClassRecord.h"
#include "records/ClimateRecord.h"
#include "records/CollisionRecord.h"
#include "records/ColourFormRecord.h"
#include "records/CombatStyleRecord.h"
#include "records/ContainerRecord.h"
#include "records/CraftableObjectRecord.h"
#include "records/DebrisRecord.h"
#include "records/DialogBranchRecord.h"
#include "records/DialogueRecord.h"
#include "records/DialogViewRecord.h"
#include "records/DOBJRecord.h"
#include "records/DoorRecord.h"
#include "records/DualCastDataRecord.h"
#include "records/EffectShaderRecord.h"
#include "records/EnchantmentRecord.h"
#include "records/EncounterZoneRecord.h"
#include "records/EquipmentSlotRecord.h"
#include "records/ExplosionRecord.h"
#include "records/EyeRecord.h"
#include "records/FactionRecord.h"
#include "records/FloraRecord.h"
#include "records/FootstepRecord.h"
#include "records/FootstepSetRecord.h"
#include "records/FormListRecord.h"
#include "records/FurnitureRecord.h"
#include "records/GMSTRecord.h"
#include "records/GlobalRecord.h"
#include "records/GrassRecord.h"
#include "records/HazardRecord.h"
#include "records/HeadPartRecord.h"
#include "records/IdleAnimationRecord.h"
#include "records/ImageSpaceModifierRecord.h"
#include "records/ImageSpaceRecord.h"
#include "records/ImpactDataRecord.h"
#include "records/ImpactDataSetRecord.h"
#include "records/IngredientRecord.h"
#include "records/KeyRecord.h"
#include "records/KeywordRecord.h"
#include "records/LandscapeTextureRecord.h"
#include "records/LeveledCharacterRecord.h"
#include "records/LeveledItemRecord.h"
#include "records/LeveledSpellRecord.h"
#include "records/LightingTemplateRecord.h"
#include "records/LightRecord.h"
#include "records/LoadScreenRecord.h"
#include "records/LocationRecord.h"
#include "records/LocationReferenceTypeRecord.h"
#include "records/MagicEffectRecord.h"
#include "records/MaterialObjectRecord.h"
#include "records/MaterialTypeRecord.h"
#include "records/MessageRecord.h"
#include "records/MiscObjectRecord.h"
#include "records/MovableStaticRecord.h"
#include "records/MovementTypeRecord.h"
#include "records/MusicTrackRecord.h"
#include "records/MusicTypeRecord.h"
#include "records/NAVIRecord.h"
#include "records/NavMeshRecord.h"
#include "records/NPCRecord.h"
#include "records/OutfitRecord.h"
#include "records/PerkRecord.h"
#include "records/PlacedGrenadeRecord.h"
#include "records/PlacedHazardRecord.h"
#include "records/ProjectileRecord.h"
#include "records/QuestRecord.h"
#include "records/RaceRecord.h"
#include "records/ReferenceRecord.h"
#include "records/RelationshipRecord.h"
#include "records/ReverbRecord.h"
#include "records/ScrollRecord.h"
#include "records/ShaderParticleGeometryRecord.h"
#include "records/ShoutRecord.h"
#include "records/SoulGemRecord.h"
#include "records/SoundCategoryRecord.h"
#include "records/SoundDescriptorRecord.h"
#include "records/SoundRecord.h"
#include "records/SpellRecord.h"
#include "records/StaticRecord.h"
#include "records/TalkingActivatorRecord.h"
#include "records/TextureSetRecord.h"
#include "records/TreeRecord.h"
#include "records/VisualEffectRecord.h"
#include "records/VoiceTypeRecord.h"
#include "records/WaterTypeRecord.h"
#include "records/WeaponRecord.h"
#include "records/WordOfPowerRecord.h"
#include "records/WorldSpaceRecord.h"
#include "records/GenericRecord.h"

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

int ESMReaderContentsAll::readNextRecord(std::ifstream& in_File, const uint32_t recName)
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
  if (recPtr->loadFromStream(in_File))
  {
    contents.m_Groups.back().addRecord(recPtr);
    return 1; //success
  }
  delete recPtr;
  recPtr = NULL;
  return -1; //error
}

} //namespace
