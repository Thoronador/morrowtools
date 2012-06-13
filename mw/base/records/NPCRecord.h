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

#ifndef MW_NPCRECORD_H
#define MW_NPCRECORD_H

#include <string>
#include <fstream>
#include <vector>
#include "PreNPCRecord.h"

namespace MWTP
{

/* enumeration type for the NPDT subrecord: there are two versions. One is
    12 bytes and the other one is 52 bytes. This enumeration type will be used
    to indicate which type of NPC data was read from the stream.
*/
enum NPDT_Type {ndtNone, ndt12Bytes, ndt52Bytes};

/* the NPC record - holds all data about an NPC */
struct NPCRecord: public PreNPCRecord
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
  //Items and spells are now part of PreNPCRecord structure, search there.
  //AI data
  //This stuff is now part of PreNPCRecord structure, search there.

  /* constructor */
  NPCRecord();

  /* destructor */
  virtual ~NPCRecord();

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

  /* returns true, if the female flag is set */
  bool isFemale() const;

  /* returns true, if the Essential flag is set */
  bool isEssential() const;

  /* returns true, if the Respawn flag is set */
  bool doesRespawn() const;

  /* returns true, if the Auto Calculate Stats flag is set */
  bool hasAutoCalcStats() const;

  /* returns true, if the white blood texture (skeleton) flag is set */
  bool hasWhiteBloodTex() const;

  /* returns true, if the gold blood texture (metal) flag is set */
  bool hasGoldBloodTex() const;
};//struct

} //namespace

#endif // MW_NPCRECORD_H
