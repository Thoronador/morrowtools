/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

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

#include "ESMReaderCleaner.hpp"
#include <iostream>
#include "../base/MW_Constants.hpp"
#include "../base/HelperIO.hpp"
#include "../base/records/ActivatorRecord.hpp"
#include "../base/records/AlchemyPotionRecord.hpp"
#include "../base/records/ApparatusRecord.hpp"
#include "../base/records/ArmourRecord.hpp"
#include "../base/records/BasicRecord.hpp"
#include "../base/records/BodyPartRecord.hpp"
#include "../base/records/BookRecord.hpp"
#include "../base/records/ClothingRecord.hpp"
#include "../base/records/ContainerRecord.hpp"
#include "../base/records/CreatureRecord.hpp"
#include "../base/records/DoorRecord.hpp"
#include "../base/records/IngredientRecord.hpp"
#include "../base/records/LightRecord.hpp"
#include "../base/records/LockpickRecord.hpp"
#include "../base/records/MiscItemRecord.hpp"
#include "../base/records/NPCRecord.hpp"
#include "../base/records/ProbeRecord.hpp"
#include "../base/records/RepairItemRecord.hpp"
#include "../base/records/StaticRecord.hpp"
#include "../base/records/WeaponRecord.hpp"

namespace MWTP
{

ESMReaderCleaner::ESMReaderCleaner(const std::string& base)
{
  MeshSet.clear();
  IconSet.clear();
  baseDirIcon = base+ "Icons\\";
  baseDirMesh = base+ "Meshes\\";
}

ESMReaderCleaner::~ESMReaderCleaner()
{
  MeshSet.clear();
  IconSet.clear();
}

int ESMReaderCleaner::processNextRecord(std::ifstream& in_File)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  BasicRecord* baseRec = NULL;

  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
         baseRec = new ActivatorRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<ActivatorRecord*>(baseRec)->ModelPath);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cALCH:
         baseRec = new AlchemyPotionRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<AlchemyPotionRecord*>(baseRec)->ModelPath);
           IconSet.insert(baseDirIcon+static_cast<AlchemyPotionRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cAPPA:
         baseRec = new ApparatusRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<ApparatusRecord*>(baseRec)->Model);
           IconSet.insert(baseDirIcon+static_cast<ApparatusRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cARMO:
         baseRec = new ArmourRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<ArmourRecord*>(baseRec)->Model);
           IconSet.insert(baseDirIcon+static_cast<ArmourRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cBODY:
         baseRec = new BodyPartRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<BodyPartRecord*>(baseRec)->MeshPath);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cBOOK:
         baseRec = new BookRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<BookRecord*>(baseRec)->MeshPath);
           IconSet.insert(baseDirIcon+static_cast<BookRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cBSGN:
    case cCELL:
    case cCLAS:
         return ESMReader::skipRecord(in_File);
         break;
    case cCLOT:
         baseRec = new ClothingRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<ClothingRecord*>(baseRec)->Model);
           IconSet.insert(baseDirIcon+static_cast<ClothingRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cCONT:
         baseRec = new ContainerRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<ContainerRecord*>(baseRec)->ModelPath);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cCREA:
         baseRec = new CreatureRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<CreatureRecord*>(baseRec)->Model);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cDIAL:
         return ESMReader::skipRecord(in_File);
         break;
    case cDOOR:
         baseRec = new DoorRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<DoorRecord*>(baseRec)->ModelPath);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cENCH:
    case cFACT:
    case cGLOB:
    case cGMST:
    case cINFO:
         return ESMReader::skipRecord(in_File);
         break;
    case cINGR:
         baseRec = new IngredientRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<IngredientRecord*>(baseRec)->ModelName);
           IconSet.insert(baseDirIcon+static_cast<IngredientRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cLAND:
    case cLEVC:
    case cLEVI:
         return ESMReader::skipRecord(in_File);
         break;
    case cLIGH:
         baseRec = new LightRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<LightRecord*>(baseRec)->ModelPath);
           IconSet.insert(baseDirIcon+static_cast<LightRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cLOCK:
         baseRec = new LockpickRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<LockpickRecord*>(baseRec)->ModelPath);
           IconSet.insert(baseDirIcon+static_cast<LockpickRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cLTEX:
    case cMGEF:
         return ESMReader::skipRecord(in_File);
         break;
    case cMISC:
         baseRec = new MiscItemRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<MiscItemRecord*>(baseRec)->ModelPath);
           IconSet.insert(baseDirIcon+static_cast<MiscItemRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cNPC_:
         baseRec = new NPCRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<NPCRecord*>(baseRec)->ModelPath);
           MeshSet.insert(baseDirMesh+static_cast<NPCRecord*>(baseRec)->HeadModel);
           MeshSet.insert(baseDirMesh+static_cast<NPCRecord*>(baseRec)->HairModel);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cPGRD:
         return ESMReader::skipRecord(in_File);
         break;
    case cPROB:
         baseRec = new ProbeRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<ProbeRecord*>(baseRec)->ModelPath);
           IconSet.insert(baseDirIcon+static_cast<ProbeRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cRACE:
    case cREGN:
         return ESMReader::skipRecord(in_File);
         break;
    case cREPA:
         baseRec = new RepairItemRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<RepairItemRecord*>(baseRec)->ModelPath);
           IconSet.insert(baseDirIcon+static_cast<RepairItemRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cSCPT:
    case cSKIL:
    case cSNDG:
    case cSOUN:
    case cSPEL:
    case cSSCR:
         return ESMReader::skipRecord(in_File);
         break;
    case cSTAT:
         baseRec = new StaticRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<StaticRecord*>(baseRec)->Mesh);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    case cWEAP:
         baseRec = new WeaponRecord;
         if (baseRec->loadFromStream(in_File))
         {
           MeshSet.insert(baseDirMesh+static_cast<WeaponRecord*>(baseRec)->ModelPath);
           IconSet.insert(baseDirIcon+static_cast<WeaponRecord*>(baseRec)->InventoryIcon);
           delete baseRec;
           return 1;
         }
         return -1;
         break;
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         return -1;
         break;
  }//swi
}//processNextRecord of ESMReaderCleaner class

} //namespace
