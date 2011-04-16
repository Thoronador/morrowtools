/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

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

#ifndef NPCRECORD_H
#define NPCRECORD_H

#include <string>
#include <fstream>
#include <vector>
#include "BasicRecord.h"
#include "ItemRecord.h"
#include "AIData.h"
#include "AIPackages.h"
#include "TravelDestination.h"

/* enumeration type for the NPDT subrecord: there are two versions. One is
    12 bytes and the other one is 52 bytes. This enumeration type will be used
    to indicate which type of NPC data was read from the stream.
*/
enum NPDT_Type {ndtNone, ndt12Bytes, ndt52Bytes};

/* the NPC record - holds all data about an NPC */
struct NPCRecord: public BasicRecord
{
  std::string NPCID;
  std::string Name;
  std::string ModelPath;
  std::string RaceID;
  std::string FactionID;
  std::string HeadModel;
  std::string ClassID;
  std::string HairModel;
  std::string ScriptID;
  //NPC data
  int16_t Level;
  uint8_t Strength;
  uint8_t Intelligence;
  uint8_t Willpower;
  uint8_t Agility;
  uint8_t Speed;
  uint8_t Endurance;
  uint8_t Personality;
  uint8_t Luck;
  uint8_t Skills[27];
  uint8_t Reputation;
  int16_t Health;
  int16_t SpellPoints;
  int16_t Fatigue;
  uint8_t Disposition;
  uint8_t Data_FactionID;
  int8_t Rank;
  uint8_t Unknown1;
  int32_t Gold;
  NPDT_Type NPCDataType;
  //end of NPC data
  int32_t NPC_Flag;
  std::vector<ItemRecord> Items;
  std::vector<std::string> NPC_Spells;
  //AI data
  NPC_AIData AIData;
  //NPC_AIEscortFollow AIEscort;
  //NPC_AIEscortFollow AIFollow;
  //std::string EscortFollowCell;
  //NPC_AITravel AITravel;
  //NPC_AIWander AIWander;
  std::vector<NPC_BasicAIPackage*> AIPackages;

  //travel destinations
  std::vector<TravelDestination> Destinations;

  /* constructor */
  NPCRecord();

  /* destructor */
  ~NPCRecord();

  /* assignment operator */
  NPCRecord& operator=(const NPCRecord& source);

  /* returns true, if the other record contains the same data */
  bool equals(const NPCRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output   - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);

  bool hasEqualAIPackages(const NPCRecord& other) const;

  private:
    void removeAIPackages();
};//struct

#endif // NPCRECORD_H
