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

#ifndef CREATURERECORD_H
#define CREATURERECORD_H

#include <string>
#include <vector>
#include "PreNPCRecord.h"
#include "ItemRecord.h"

struct CreatureRecord: public PreNPCRecord
{
  std::string CreatureID;
  std::string Model;
  std::string Name;
  std::string SoundGenCreature;
  //creature data
  int32_t CreatureType;
  int32_t Level;
  int32_t Strength;
  int32_t Intelligence;
  int32_t Willpower;
  int32_t Agility;
  int32_t Speed;
  int32_t Endurance;
  int32_t Personality;
  int32_t Luck;
  int32_t Health;
  int32_t SpellPoints;
  int32_t Fatigue;
  int32_t Soul;
  int32_t Combat;
  int32_t Magic;
  int32_t Stealth;
  int32_t AttackMin1;
  int32_t AttackMax1;
  int32_t AttackMin2;
  int32_t AttackMax2;
  int32_t AttackMin3;
  int32_t AttackMax3;
  int32_t Gold;
  //end of creature data
  int32_t CreatureFlag;
  std::vector<ItemRecord> Items;
  std::vector<std::string> CreatureSpells;

  //The AI-related data members are already declared in PreNPCRecord.

  std::string ScriptID;
  float Scale;

  /* constructor */
  CreatureRecord();

  /* destructor */
  ~CreatureRecord();

  /* assignment operator */
  CreatureRecord& operator=(const CreatureRecord& source);

  /* returns true, if the other record contains the same data */
  bool equals(const CreatureRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output      - the output file stream
  */
  bool saveToStream(std::ofstream& output) const;

  /* loads the record from the given input stream and returns true on success

    parameters:
        in_File - the input file stream
  */
  bool loadFromStream(std::ifstream& in_File);
};//struct

#endif // CREATURERECORD_H
