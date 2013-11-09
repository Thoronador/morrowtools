/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2012, 2013  Thoronador

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

#include <iostream>
#include <set>
#include <map>
#include "../base/ESMReaderSingleType.h"
#include "../base/FormIDFunctions.h"
#include "../base/Ingredients.h"
#include "../base/MagicEffects.h"
#include "../base/SR_Constants.h"
#include "../base/records/TES4HeaderRecord.h"
#include "../../base/UtilityFunctions.h"
#include "../../base/ComparisonFunctor.h"

struct ingredListEffect
{
  public:
    std::string effectName;
    std::set<std::string> ingreds;

    ingredListEffect()
    : effectName(""),
      ingreds(std::set<std::string>())
    {}

    ingredListEffect(const std::string& effName)
    : effectName(effName),
      ingreds(std::set<std::string>())
    {}

    bool operator<(const ingredListEffect& other) const
    {
      return (lowerCaseCompare(effectName, other.effectName)<0);
    }//op
};//struct



int main()
{
  std::cout << "Hello world!" << std::endl;

  SRTP::Tes4HeaderRecord header_rec;
  SRTP::ESMReaderSingleType<SRTP::IngredientRecord, SRTP::Ingredients, SRTP::cINGR> reader_ingred;
  SRTP::ESMReaderSingleType<SRTP::MagicEffects, SRTP::MagicEffects, SRTP::cMGEF> reader_effect;

  int readResult = reader_ingred.readESM("G:\\Download\\tes5_data\\Skyrim.esm", header_rec);
  if (readResult>=0)
  {
    std::cout << "File was read/skipped successfully! Groups read: "<<readResult<<".\n";
  }
  else
  {
    std::cout << "Reading file failed!\n";
    return 0;
  }

  readResult = reader_effect.readESM("G:\\Download\\tes5_data\\Skyrim.esm", header_rec);
  if (readResult>=0)
  {
    std::cout << "File was read/skipped successfully! Groups read: "<<readResult<<".\n";
  }
  else
  {
    std::cout << "Reading file failed!\n";
    return 0;
  }


  std::cout << "INGR records read so far: "<<SRTP::Ingredients::getSingleton().getNumberOfRecords()<<"\n";
  std::cout << "   Four times that: "<<SRTP::Ingredients::getSingleton().getNumberOfRecords()*4<<"\n";
  std::cout << "MGEF records read so far: "<<SRTP::MagicEffects::getSingleton().getNumberOfRecords()<<"\n";


  std::map<std::string, ingredListEffect, MWTP::ci_less> theList;

  unsigned int i, entries;
  entries = 0;
  SRTP::Ingredients::ListIterator iter = SRTP::Ingredients::getSingleton().getBegin();
  while (iter!=SRTP::Ingredients::getSingleton().getEnd())
  {
    if (iter->second.name.isPresent())
    {
      const std::string& ingredName = iter->second.name.getString();
      for (i=0; i<iter->second.effects.size(); ++i)
      {
        if (SRTP::MagicEffects::getSingleton().hasRecord(iter->second.effects[i].effectFormID))
        {
          const SRTP::MagicEffectRecord& eff_rec = SRTP::MagicEffects::getSingleton().getRecord(iter->second.effects[i].effectFormID);
          if (eff_rec.name.isPresent())
          {
            //add it to the list
            const std::string& theEffectName = eff_rec.name.getString();

            theList[theEffectName].ingreds.insert(ingredName);
            ++entries;
          }
          else
          {
            std::cout << "Warning: No string for sID "<<eff_rec.name.getIndex()<<"!\n";
          }
        }
        else
        {
          std::cout << "Warning: No MGEF for fID "<<SRTP::getFormIDAsString(iter->second.effects[i].effectFormID)<<"!\n";
        }

      }//for
    }//if
    ++iter;
  }//while

  std::cout << "Entries: "<<entries<<"\n"
            << "Different effects: "<<theList.size() ;

  //now go through the list
  {
  std::ofstream output;
  output.open("5_zutatenliste_nach_effekt.txt", std::ios::out | std::ios::binary);
  if (!output)
  {
    std::cout << "Error: could not open file.\n";
    return -1;
  }

  std::map<std::string, ingredListEffect, MWTP::ci_less>::iterator mapIter;
  mapIter = theList.begin();
  std::set<std::string>::const_iterator inner_iter;
  i=0;
  while (mapIter!=theList.end())
  {
    if ((i%2)==0)
    {
      output << "<tr>\n";
    }
    output << "<td valign=\"top\">\n<u>"<<mapIter->first<<"</u>\n<ul>\n";
    inner_iter = mapIter->second.ingreds.begin();
    while (inner_iter!=mapIter->second.ingreds.end())
    {
      output << "  <li>"<< *inner_iter<<"</li>\n";
      ++inner_iter;
    }//while (inner)
    output << "</ul>\n</td>\n";
    if ((i%2)==1)
    {
      output << "</tr>\n";
    }
    ++i;
    ++mapIter;
  }//while

  output.close();
  }


  //andere Liste

  //maps name to Form ID
  std::map<std::string, uint32_t, MWTP::ci_less> name_fid_map;
  iter = SRTP::Ingredients::getSingleton().getBegin();
  while (iter!=SRTP::Ingredients::getSingleton().getEnd())
  {
    name_fid_map[iter->second.name.getString()] = iter->second.headerFormID;
    ++iter;
  }//while

  std::ofstream output;
  output.open("5_zutatenliste_nach_zutat.txt", std::ios::out | std::ios::binary);
  if (!output)
  {
    std::cout << "Error: could not open file.\n";
    return -1;
  }

  std::map<std::string, uint32_t, MWTP::ci_less>::iterator mapIter;
  mapIter = name_fid_map.begin();
  std::vector<SRTP::EffectBlock>::const_iterator inner_iter;
  i=0;
  while (mapIter!=name_fid_map.end())
  {
    if ((i%2)==0)
    {
      output << "<tr>\n";
    }
    const SRTP::IngredientRecord& rec = SRTP::Ingredients::getSingleton().getRecord(mapIter->second);
    output << "<td valign=\"top\">\n<u>"<<rec.name.getString()<<"</u>\n<ul>\n";
    inner_iter = rec.effects.begin();
    while (inner_iter!=rec.effects.end())
    {
      output << "  <li>"<< SRTP::MagicEffects::getSingleton().getRecord(inner_iter->effectFormID).name.getString() <<"</li>\n";
      ++inner_iter;
    }//while (inner)
    output << "</ul>\n</td>\n";
    if ((i%2)==1)
    {
      output << "</tr>\n";
    }
    ++i;
    ++mapIter;
  }//while

  output.close();

  return 0;
}
