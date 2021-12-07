/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2010, 2011, 2013  Thoronador

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

#ifndef MW_MAGICEFFECTS_HPP
#define MW_MAGICEFFECTS_HPP

#include <string>
#include <map>
#include <fstream>
#include "records/MagicEffectRecord.hpp"

namespace MWTP
{

//iterator type for list
typedef std::map<int32_t, MGEF_Data>::const_iterator EffectListIterator;

class MagicEffects
{
  public:
    /* destructor */
    ~MagicEffects();

    /* singleton access */
    static MagicEffects& get();

    /* adds an effect to the list */
    void addEffect(const MGEF_Data& Data);

    /* returns true, if an effect with the given index exists */
    bool hasEffect(const int32_t Index) const;

    /* returns the number of different effects */
    unsigned int getNumberOfEffects() const;

    /* retrieves data of a specific magic effect

       parameters:
           Index - the index of the effect

       remarks:
           If there is no effect with the given index, the function throws an
           error. Use hasEffect() to determine if an effect exists.
    */
    const MGEF_Data& getEffect(const int32_t Index) const;

    /* returns the name of the game setting that contains the name of the
       effect specified by the given index

       parameters:
           Index - the index of the effect
    */
    static std::string getSettingNameForEffect(const int32_t Index);

    /* returns true, if the given skill index identifies a skill-related effect

       parameters:
           Index - the index of the effect
    */
    static bool isSkillRelatedEffect(const int32_t Index);

    /* returns true, if the given skill index identifies an attribute-related effect

       parameters:
           Index - the index of the effect
    */
    static bool isAttributeRelatedEffect(const int32_t Index);

    /* tries to read a magic effect record from the given input stream.

       return value:
           If an error occurRed, the function returns -1. Otherwise it returns
           the number of updated records. (Usually that is one. If, however, the
           record that was read is equal to the one already in the list, zero is
           returned.)

       parameters:
           input - the input stream that is used to read the record
    */
    int readRecordMGEF(std::istream& input);

    /* returns constant iterator to the beginning of the internal list */
    EffectListIterator begin() const;

    /* returns constant iterator to the end of the internal list */
    EffectListIterator end() const;

    /* tries to save all available magic effects to the given stream and returns
       true on success, false on failure

       parameters:
           output - the output stream that shall be used to save the magic
                    effects
    */
    bool saveAllToStream(std::ostream& output) const;

    /* deletes all effects */
    void clear();
  private:
    /* constructor */
    MagicEffects();

    /* copy constructor - empty */
    MagicEffects(const MGEF_Data& op)
    : m_Effects(std::map<int32_t, MGEF_Data>()) {}

    /* internal list of effects */
    std::map<int32_t, MGEF_Data> m_Effects;
}; //class

} //namespace

#endif // MW_MAGICEFFECTS_HPP
