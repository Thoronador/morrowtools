/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2025  Dirk Stolle

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

#include "ESMReaderContents.hpp"
#include <iostream>
#include "records/ActivatorRecord.hpp"
#include "records/AlchemyPotionRecord.hpp"
#include "records/ApparatusRecord.hpp"
#include "records/ArmourRecord.hpp"
#include "records/BirthSignRecord.hpp"
#include "records/BodyPartRecord.hpp"
#include "records/BookRecord.hpp"
#include "records/CellRecord.hpp"
#include "records/ClassRecord.hpp"
#include "records/ClothingRecord.hpp"
#include "records/ContainerRecord.hpp"
#include "records/CreatureRecord.hpp"
#include "records/DialogueInfoRecord.hpp"
#include "records/DialogueTopicRecord.hpp"
#include "records/DoorRecord.hpp"
#include "records/EnchantingRecord.hpp"
#include "records/FactionRecord.hpp"
#include "records/GameSettingRecord.hpp"
#include "records/GlobalRecord.hpp"
#include "HelperIO.hpp"
#include "records/IngredientRecord.hpp"
#include "records/LandscapeRecord.hpp"
#include "records/LandscapeTextureRecord.hpp"
#include "records/LeveledCreatureRecord.hpp"
#include "records/LeveledItemRecord.hpp"
#include "records/LightRecord.hpp"
#include "records/LockpickRecord.hpp"
#include "records/MagicEffectRecord.hpp"
#include "records/MiscItemRecord.hpp"
#include "MW_Constants.hpp"
#include "records/NPCRecord.hpp"
#include "records/PathGridRecord.hpp"
#include "records/ProbeRecord.hpp"
#include "records/RaceRecord.hpp"
#include "records/RegionRecord.hpp"
#include "records/RepairItemRecord.hpp"
#include "records/ScriptRecord.hpp"
#include "records/SkillRecord.hpp"
#include "records/SpellRecord.hpp"
#include "records/StaticRecord.hpp"
#include "records/StartScriptRecord.hpp"
#include "records/SoundRecord.hpp"
#include "records/SoundGeneratorRecord.hpp"
#include "records/WeaponRecord.hpp"

namespace MWTP
{

ESMReaderContents::ESMReaderContents(VectorType& storage)
: contents(storage)
{
}

int ESMReaderContents::processNextRecord(std::istream& input)
{
  // normally should be 4 char, but char is not eligible for switch
  uint32_t RecordName = 0;

  // read record name
  input.read(reinterpret_cast<char*>(&RecordName), 4);
  BasicRecord * recPtr = nullptr;
  switch(RecordName)
  {
    case cACTI:
         recPtr = new ActivatorRecord;
         break;
    case cALCH:
         recPtr = new AlchemyPotionRecord;
         break;
    case cAPPA:
         recPtr = new ApparatusRecord;
         break;
    case cARMO:
         recPtr = new ArmourRecord;
         break;
    case cBODY:
         recPtr = new BodyPartRecord;
         break;
    case cBOOK:
         recPtr = new BookRecord;
         break;
    case cBSGN:
         recPtr = new BirthSignRecord;
         break;
    case cCELL:
         recPtr = new CellRecord;
         break;
    case cCLAS:
         recPtr = new ClassRecord;
         break;
    case cCLOT:
         recPtr = new ClothingRecord;
         break;
    case cCONT:
         recPtr = new ContainerRecord;
         break;
    case cCREA:
         recPtr = new CreatureRecord;
         break;
    case cDIAL:
         recPtr = new DialogueTopicRecord;
         break;
    case cDOOR:
         recPtr = new DoorRecord;
         break;
    case cENCH:
         recPtr = new EnchantingRecord;
         break;
    case cFACT:
         recPtr = new FactionRecord;
         break;
    case cGLOB:
         recPtr = new GlobalRecord;
         break;
    case cGMST:
         recPtr = new GameSettingRecord;
         break;
    case cINFO:
         recPtr = new DialogueInfoRecord;
         break;
    case cINGR:
         recPtr = new IngredientRecord;
         break;
    case cLAND:
         recPtr = new LandscapeRecord;
         break;
    case cLEVC:
         recPtr = new LeveledCreatureRecord;
         break;
    case cLEVI:
         recPtr = new LeveledItemRecord;
         break;
    case cLIGH:
         recPtr = new LightRecord;
         break;
    case cLOCK:
         recPtr = new LockpickRecord;
         break;
    case cLTEX:
         recPtr = new LandscapeTextureRecord;
         break;
    case cMGEF:
         recPtr = new MagicEffectRecord;
         break;
    case cMISC:
         recPtr = new MiscItemRecord;
         break;
    case cNPC_:
         recPtr = new NPCRecord;
         break;
    case cPGRD:
         recPtr = new PathGridRecord;
         break;
    case cPROB:
         recPtr = new ProbeRecord;
         break;
    case cRACE:
         recPtr = new RaceRecord;
         break;
    case cREGN:
         recPtr = new RegionRecord;
         break;
    case cREPA:
         recPtr = new RepairItemRecord;
         break;
    case cSCPT:
         recPtr = new ScriptRecord;
         break;
    case cSKIL:
         recPtr = new SkillRecord;
         break;
    case cSNDG:
         recPtr = new SoundGeneratorRecord;
         break;
    case cSOUN:
         recPtr = new SoundRecord;
         break;
    case cSPEL:
         recPtr = new SpellRecord;
         break;
    case cSSCR:
         recPtr = new StartScriptRecord;
         break;
    case cSTAT:
         recPtr = new StaticRecord;
         break;
    case cWEAP:
         recPtr = new WeaponRecord;
         break;
    default:
         std::cerr << "processNextRecord: ERROR: unknown record type found: \""
                   << IntTo4Char(RecordName) << "\".\n"
                   << "Current file position: " << input.tellg() << " bytes.\n";
         return -1;
  }

  if (recPtr->loadFromStream(input))
  {
    // Successfully loaded record, put it into vector.
    contents.emplace_back(recPtr);
    return 1;
  }

  // failure to load record
  delete recPtr;
  recPtr = nullptr;
  return -1;
}

} // namespace
