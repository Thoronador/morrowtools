/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ESMReaderAll.h"
#include <iostream>
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
#include "HelperIO.h"
#include "Ingredients.h"
#include "Landscape.h"
#include "LandscapeTextures.h"
#include "LeveledCreatures.h"
#include "LeveledItems.h"
#include "Lights.h"
#include "Lockpicks.h"
#include "MagicEffects.h"
#include "MiscItems.h"
#include "MW_Constants.h"
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

ESMReaderAll::ESMReaderAll()
{
  //empty
}

ESMReaderAll::~ESMReaderAll()
{
  //empty
}

int ESMReaderAll::processNextRecord(std::ifstream& in_File)
{
  int32_t RecordName = 0; //normally should be 4 char, but char is not eligible for switch
  int lastResult = 0;

  //read record name
  in_File.read((char*) &RecordName, 4);
  switch(RecordName)
  {
    case cACTI:
         lastResult = Activators::getSingleton().readRecordACTI(in_File);
         break;
    case cALCH:
         lastResult = AlchemyPotions::getSingleton().readRecordALCH(in_File);
         break;
    case cAPPA:
         lastResult = Apparatuses::getSingleton().readRecordAPPA(in_File);
         break;
    case cARMO:
         lastResult = Armours::getSingleton().readRecordARMO(in_File);
         break;
    case cBODY:
         lastResult = BodyParts::getSingleton().readRecordBODY(in_File);
         break;
    case cBOOK:
         lastResult = Books::getSingleton().readRecordBOOK(in_File);
         break;
    case cBSGN:
         lastResult = BirthSigns::getSingleton().readRecordBSGN(in_File);
         break;
    case cCELL:
         lastResult = Cells::getSingleton().readRecordCELL(in_File);
         break;
    case cCLAS:
         lastResult = Classes::getSingleton().readRecordCLAS(in_File);
         break;
    case cCLOT:
         lastResult = Clothings::getSingleton().readRecordCLOT(in_File);
         break;
    case cCONT:
         lastResult = Containers::getSingleton().readRecordCONT(in_File);
         break;
    case cCREA:
         lastResult = Creatures::getSingleton().readRecordCREA(in_File);
         break;
    case cDIAL:
         lastResult = DialogueTopics::getSingleton().readRecordDIAL(in_File);
         break;
    case cDOOR:
         lastResult = Doors::getSingleton().readRecordDOOR(in_File);
         break;
    case cENCH:
         lastResult = Enchantings::getSingleton().readRecordENCH(in_File);
         break;
    case cFACT:
         lastResult = Factions::getSingleton().readRecordFACT(in_File);
         break;
    case cGLOB:
         lastResult = Globals::getSingleton().readRecordGLOB(in_File);
         break;
    case cGMST:
         lastResult = GameSettings::getSingleton().readRecordGMST(in_File);
         break;
    case cINFO:
         lastResult = DialogueInfos::getSingleton().readRecordINFO(in_File);
         break;
    case cINGR:
         lastResult = Ingredients::getSingleton().readRecordINGR(in_File);
         break;
    case cLAND:
         lastResult = Landscape::getSingleton().readRecordLAND(in_File);
         break;
    case cLEVC:
         lastResult = LeveledCreatures::getSingleton().readRecordLEVC(in_File);
         break;
    case cLEVI:
         lastResult = LeveledItems::getSingleton().readRecordLEVI(in_File);
         break;
    case cLIGH:
         lastResult = Lights::getSingleton().readRecordLIGH(in_File);
         break;
    case cLOCK:
         lastResult = Lockpicks::getSingleton().readRecordLOCK(in_File);
         break;
    case cLTEX:
         lastResult = LandscapeTextures::getSingleton().readRecordLTEX(in_File);
         break;
    case cMGEF:
         lastResult = MagicEffects::getSingleton().readRecordMGEF(in_File);
         break;
    case cMISC:
         lastResult = MiscItems::getSingleton().readRecordMISC(in_File);
         break;
    case cNPC_:
         lastResult = NPCs::getSingleton().readRecordNPC_(in_File);
         break;
    case cPGRD:
         lastResult = PathGrids::getSingleton().readRecordPGRD(in_File);
         break;
    case cPROB:
         lastResult = Probes::getSingleton().readRecordPROB(in_File);
         break;
    case cRACE:
         lastResult = Races::getSingleton().readRecordRACE(in_File);
         break;
    case cREGN:
         lastResult = Regions::getSingleton().readRecordREGN(in_File);
         break;
    case cREPA:
         lastResult = RepairItems::getSingleton().readRecordREPA(in_File);
         break;
    case cSCPT:
         lastResult = Scripts::getSingleton().readRecordSCPT(in_File);
         break;
    case cSKIL:
         lastResult = Skills::getSingleton().readRecordSKIL(in_File);
         break;
    case cSNDG:
         lastResult = SoundGenerators::getSingleton().readRecordSNDG(in_File);
         break;
    case cSOUN:
         lastResult = Sounds::getSingleton().readRecordSOUN(in_File);
         break;
    case cSPEL:
         lastResult = Spells::getSingleton().readRecordSPEL(in_File);
         break;
    case cSSCR:
         lastResult = StartScripts::getSingleton().readRecordSSCR(in_File);
         break;
    case cSTAT:
         lastResult = Statics::getSingleton().readRecordSTAT(in_File);
         break;
    case cWEAP:
         lastResult = Weapons::getSingleton().readRecordWEAP(in_File);
         break;
    default:
         std::cout << "processNextRecord: ERROR: unknown record type found: \""
                   <<IntTo4Char(RecordName)<<"\".\n"
                   << "Current file position: "<<in_File.tellg()<< " bytes.\n";
         lastResult = -1;
         break;
  }//swi
  return lastResult;
}//processNextRecord of ESMReaderAll class

} //namespace
