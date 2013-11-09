#include <iostream>
#include <set>
#include <map>
#include "../base/ESMReaderSingleType.h"
#include "../base/records/Tes4HeaderRecord.h"
#include "../base/FormIDFunctions.h"
#include "../base/Ingredients.h"
#include "../base/MagicEffects.h"
#include "../base/SR_Constants.h"
#include "../base/StringTable.h"
#include "../../base/UtilityFunctions.h"
#include "../../base/ComparisonFunctor.h"

struct ingredListEffect
{
  public:
    std::string effectName;
    std::set<std::string> ingreds;

    ingredListEffect()
    {
      effectName = "";
      ingreds.clear();
    }

    ingredListEffect(const std::string& effName)
    {
      effectName = effName;
      ingreds.clear();
    }

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

  //String tables
  SRTP::StringTable table, table_2nd;
  if (!table.readTable("C:\\Temp\\Skyrim_German.DLStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Reading table C:\\Temp\\Skyrim_German.DLStrings failed!\n";
    return 0;
  }

  if (!table_2nd.readTable("C:\\Temp\\Skyrim_German.ILStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Reading table C:\\Temp\\Skyrim_German.ILStrings failed!\n";
    return 0;
  }
  table.mergeTables(table_2nd);
  table_2nd.tabulaRasa();

  if (!table_2nd.readTable("C:\\Temp\\Skyrim_German.Strings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Reading table C:\\Temp\\Skyrim_German.Strings failed!\n";
  }

  table.mergeTables(table_2nd);
  table_2nd.tabulaRasa();


  std::cout << "INGR records read so far: "<<SRTP::Ingredients::getSingleton().getNumberOfRecords()<<"\n";
  std::cout << "   Four times that: "<<SRTP::Ingredients::getSingleton().getNumberOfRecords()*4<<"\n";
  std::cout << "MGEF records read so far: "<<SRTP::MagicEffects::getSingleton().getNumberOfRecords()<<"\n";


  std::map<std::string, ingredListEffect, MWTP::ci_less> theList;

  unsigned int i, entries;
  entries = 0;
  SRTP::Ingredients::ListIterator iter = SRTP::Ingredients::getSingleton().getBegin();
  while (iter!=SRTP::Ingredients::getSingleton().getEnd())
  {
    if (table.hasString(iter->second.name.getIndex()))
    {
      const std::string& ingredName = table.getString(iter->second.name.getIndex());
      for (i=0; i<iter->second.effects.size(); ++i)
      {
        if (SRTP::MagicEffects::getSingleton().hasRecord(iter->second.effects[i].effectFormID))
        {
          const SRTP::MagicEffectRecord& eff_rec = SRTP::MagicEffects::getSingleton().getRecord(iter->second.effects[i].effectFormID);
          if (table.hasString(eff_rec.name.getIndex()))
          {
            //add it to the list
            const std::string& theEffectName = table.getString(eff_rec.name.getIndex());

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
    name_fid_map[table.getString(iter->second.name.getIndex())] = iter->second.headerFormID;
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
    output << "<td valign=\"top\">\n<u>"<<table.getString(rec.name.getIndex())<<"</u>\n<ul>\n";
    inner_iter = rec.effects.begin();
    while (inner_iter!=rec.effects.end())
    {
      output << "  <li>"<< table.getString(SRTP::MagicEffects::getSingleton().getRecord(inner_iter->effectFormID).name.getIndex()) <<"</li>\n";
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
