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

#ifndef SKILLS_H
#define SKILLS_H

#include <map>
#include <string>
#include <fstream>

//constants for skill indices
const int32_t SkillBlock = 0;
const int32_t SkillArmorer = 1;
const int32_t SkillMediumArmor = 2;
const int32_t SkillHeavyArmor = 3;
const int32_t SkillBluntWeapon = 4;
const int32_t SkillLongBlade = 5;
const int32_t SkillAxe = 6;
const int32_t SkillSpear = 7;
const int32_t SkillAthletics = 8;
const int32_t SkillEnchant = 9;
const int32_t SkillDestruction = 10;
const int32_t SkillAlteration = 11;
const int32_t SkillIllusion = 12;
const int32_t SkillConjuration = 13;
const int32_t SkillMysticism = 14;
const int32_t SkillRestoration = 15;
const int32_t SkillAlchemy = 16;
const int32_t SkillUnarmored = 17;
const int32_t SkillSecurity = 18;
const int32_t SkillSneak = 19;
const int32_t SkillAcrobatics = 20;
const int32_t SkillLightArmor = 21;
const int32_t SkillShortBlade = 22;
const int32_t SkillMarksman = 23;
const int32_t SkillMercantile = 24;
const int32_t SkillSpeechcraft = 25;
const int32_t SkillHandToHand = 26;


//constants for specialization of skills
const int32_t cSpecializationCombat = 0;
const int32_t cSpecializationMagic = 1;
const int32_t cSpecializationStealth = 2;

//record type for skill data
struct SkillRecord
{
  int32_t Attribute;
  int32_t Specialization;
  float UseValue[4];
  std::string Description;

  /* returns true, if this record contains the same data as the other record */
  bool equals(const SkillRecord& other) const;

  /* writes the record to the given output stream and returns true on success

    parameters:
        output - the output file stream
  */
  bool saveToStream(std::ofstream& output, const int32_t SkillIndex) const;
};

//iterator type
typedef std::map<int32_t, SkillRecord>::const_iterator SkillListIterator;

class Skills
{
  public:
    /* destructor */
    ~Skills();

    /* singleton access method */
    static Skills& getSingleton();

    /* adds a new skill record to the list

       parameters:
           Index     - the index of the skill
           SkillData - data record of the skill
    */
    void addSkill(const int32_t Index, const SkillRecord& SkillData);

    /* returns true, if the skill with the given index exists

       parameters:
           Index - the index of the skill
    */
    bool hasSkill(const int32_t Index) const;

    /* returns the data record of the skill with the given index

       parameters:
           Index - the index of the skill

       remarks:
           If there is no skill with the given index, this function will throw
           an exception. Therefore you should use hasSkill() to check for the
           presence of a certain skill record first.
    */
    const SkillRecord& getSkillData(const int32_t Index) const;

    /* returns the name of the game setting that contains the name of the
       skill specified by the given index

       parameters:
           Index - the index of the skill
    */
    static std::string getSettingNameForSkill(const int32_t Index);

    /* returns the number of current skills */
    unsigned int getNumberOfSkills() const;

    /* tries to read a skill record from the given input file stream.

       return value:
           If an error occured, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           in_File - the input file stream that is used to read the record
    */
    int readRecordSKIL(std::ifstream& in_File);

    /* returns a constant iterator the the start of the list */
    SkillListIterator getBegin() const;

    /* returns a constant iterator to the end of the list */
    SkillListIterator getEnd() const;

    /* tries to save all available skills to the given stream and returns true
       on success, false on failure

       parameters:
           output - the output file stream that shall be used to save the skills
    */
    bool saveAllToStream(std::ofstream& output) const;

    /* removes all skills from the list */
    void clearAll();
  private:
    /* constrcutor - private due to singleton pattern */
    Skills();

    /* copy constructor - empty due to singleton pattern */
    Skills(const Skills& op) {}

    /* internal data list */
    std::map<int32_t, SkillRecord> m_Skills;
}; //class

#endif // SKILLS_H
