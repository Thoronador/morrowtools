/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published
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

#ifndef SPELLS_H
#define SPELLS_H

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "Enchantment.h"

const int32_t stSpell = 0;
const int32_t stAbility = 1;
const int32_t stBlight = 2;
const int32_t stDisease = 3;
const int32_t stCurse = 4;
const int32_t stPower = 5;

struct SpellRecord
{
  std::string Name;
  int32_t Type;
  int32_t Cost;
  int32_t Flags;
  std::vector<EnchantmentData> Enchs;
  bool saveToStream(std::ofstream& output, const std::string& SpellID) const;
};//struct

typedef std::map<std::string, SpellRecord>::const_iterator SpellListIterator;

class Spells
{
  public:
    ~Spells();
    static Spells& getSingleton();
    void addSpell(const std::string& ID, const SpellRecord& record);
    bool hasSpell(const std::string& ID) const;
    unsigned int getNumberOfSpells() const;
    const SpellRecord& getSpell(const std::string& ID) const;
    bool removeSpell(const std::string& ID);
    void clearAll();
    SpellListIterator getBegin() const;
    SpellListIterator getEnd() const;
    bool saveAllToStream(std::ofstream& output) const;
    static const size_t cMaximumSpellNameLength;
  private:
    Spells();
    Spells(const Spells& op) {}
    std::map<std::string, SpellRecord> m_Spells;
}; //class

#endif // SPELLS_H
