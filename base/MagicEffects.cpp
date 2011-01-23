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

#include "MagicEffects.h"
#include <iostream>

MagicEffects::MagicEffects()
{
  //empty
}

MagicEffects::~MagicEffects()
{
  m_Effects.clear();
}

MagicEffects& MagicEffects::getSingleton()
{
  static MagicEffects Instance;
  return Instance;
}

void MagicEffects::addEffect(const int Index, const MGEF_Data& Data)
{
  m_Effects[Index] = Data;
}

bool MagicEffects::hasEffect(const int Index) const
{
  return m_Effects.find(Index)!=m_Effects.end();
}

unsigned int MagicEffects::getNumberOfEffects() const
{
  return m_Effects.size();
}

const MGEF_Data& MagicEffects::getEffect(const int Index) const
{
  std::map<int, MGEF_Data>::const_iterator iter;
  iter = m_Effects.find(Index);
  if (iter!=m_Effects.end())
  {
    return iter->second;
  }
  //nothing found
  std::cout << "No magic effect with index "<<Index<<" found!\n";
  throw 1;
}

void MagicEffects::clearAll()
{
  m_Effects.clear();
}

EffectListIterator MagicEffects::getBegin() const
{
  return m_Effects.begin();
}

EffectListIterator MagicEffects::getEnd() const
{
  return m_Effects.end();
}
