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

#include "Ingredients.h"
#include <iostream>

Ingredients::Ingredients()
{
  m_Ingredients.clear();
}

Ingredients::~Ingredients()
{
  m_Ingredients.clear();
}

Ingredients& Ingredients::getSingleton()
{
  static Ingredients Instance;
  return Instance;
}

void Ingredients::addIngredient(const IngredientRecord& data)
{
  m_Ingredients[data.IngredientID] = data;
}

void Ingredients::clearAll()
{
  m_Ingredients.clear();
}

unsigned int Ingredients::getNumberOfIngredients() const
{
  return m_Ingredients.size();
}

bool Ingredients::hasIngredient(const std::string& ID) const
{
  return m_Ingredients.find(ID)!=m_Ingredients.end();
}

const IngredientRecord& Ingredients::getIngredientData(const std::string& ID) const
{
  const std::map<const std::string, IngredientRecord>::const_iterator iter =
        m_Ingredients.find(ID);
  if (iter!=m_Ingredients.end())
  {
    return iter->second;
  }
  throw 42;
}

void Ingredients::listAll() const
{
  std::cout << "Ingredient list ("<<m_Ingredients.size()<<" items):\n";
  std::map<const std::string, IngredientRecord>::const_iterator iter = m_Ingredients.begin();
  while (iter!=m_Ingredients.end())
  {
    std::cout << "  "<<iter->first <<" (\""<<iter->second.IngredientName<<"\")\n";
    ++iter;
  }//while
}

IngredListIterator Ingredients::getBegin() const
{
  return m_Ingredients.begin();
}
IngredListIterator Ingredients::getEnd() const
{
  return m_Ingredients.end();
}

int Ingredients::readRecordINGR(std::ifstream& in_File)
{
  IngredientRecord tempRec;
  if (tempRec.loadFromStream(in_File))
  {
    //check for presence of ingredient
    if (hasIngredient(tempRec.IngredientID))
    {
      if (getIngredientData(tempRec.IngredientID).equals(tempRec))
      {
        return 0; //return zero, nothing was changed
      }
    }//if
    addIngredient(tempRec);
    return 1;
  }
  std::cout << "Error while reading ingredient record.\n";
  return -1;
}//readRecordINGR

bool Ingredients::saveAllToStream(std::ofstream& output) const
{
  if (!output.good())
  {
    std::cout << "Ingredients::saveAllToStream: Error: bad stream.\n";
    return false;
  }
  IngredListIterator iter = m_Ingredients.begin();
  const IngredListIterator end_iter = m_Ingredients.end();
  while (iter!=end_iter)
  {
    if (!iter->second.saveToStream(output))
    {
      std::cout << "Ingredients::saveAllToStream: Error while writing record.\n";
      return false;
    }
    ++iter;
  }//while
  return output.good();
}
