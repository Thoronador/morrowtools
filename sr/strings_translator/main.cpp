/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2013  Thoronador

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
#include <fstream>
#include <algorithm>
#include <vector>
#include "../base/PathFunctions.h"
#include "../base/StringTable.h"
#include "../../base/UtilityFunctions.h"

struct TranslationElem
{
  std::string English;
  uint32_t index;
  std::string German;

  bool operator<(const TranslationElem& other) const
  {
    const int e = lowerCaseCompare(English, other.English);
    if (e<0) return true;
    if (e>0) return false;
    //e is zero
    const int g = lowerCaseCompare(German, other.German);
    if (g<0) return true;
    if (g>0) return false;
    //g is zero
    return (index<other.index);
  }//operator
};//struct

int main()
{
  std::string dataDir;
  //Get the data directory?
  SRTP::getDataDir(dataDir);


  //String tables
  std::string stringsPath = dataDir+"Strings\\Skyrim_English";
  SRTP::StringTable table_dl_en, table_il_en, table_normal_en;
  if (table_dl_en.readTable(stringsPath+".DLStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Table read successfully! Table contains "<<table_dl_en.getNumberOfTableEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading table "<< stringsPath<<".DLStrings failed!\n";
  }

  if (table_il_en.readTable(stringsPath+".ILStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Table read successfully! Table contains "<<table_il_en.getNumberOfTableEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading table "<<stringsPath<<".ILStrings failed!\n";
  }

  if (table_normal_en.readTable(stringsPath+".Strings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Table read successfully! Table contains "<<table_normal_en.getNumberOfTableEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading table "<< stringsPath<<".Strings failed!\n";
  }

  std::cout << "Merging 3rd and 2nd table, resulting table should have "
            << table_normal_en.getNumberOfTableEntries()+table_il_en.getNumberOfTableEntries() << " entries.\n";
  table_normal_en.mergeTables(table_il_en);
  std::cout << "Entries after first merge: "<<table_normal_en.getNumberOfTableEntries();
  std::cout << "\n\nMerging with 1st table, resulting table should have "
            << table_normal_en.getNumberOfTableEntries()+table_dl_en.getNumberOfTableEntries() << " entries.\n";
  table_normal_en.mergeTables(table_dl_en);
  std::cout << "Entries after second merge: "<<table_normal_en.getNumberOfTableEntries()<<"\n\n";

  table_il_en.tabulaRasa();
  table_dl_en.tabulaRasa();
  std::cout << "Table has string with ID 0: "<<table_normal_en.hasString(0)<<"\n";


  /* **** German tables **** */

  stringsPath = dataDir+"Strings\\Skyrim_German";
  SRTP::StringTable table_dl_de, table_il_de, table_normal_de;
  if (table_dl_de.readTable(stringsPath+".DLStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Table read successfully! Table contains "<<table_dl_de.getNumberOfTableEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading table "<< stringsPath<<".DLStrings failed!\n";
  }

  if (table_il_de.readTable(stringsPath+".ILStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Table read successfully! Table contains "<<table_il_de.getNumberOfTableEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading table "<<stringsPath<<".ILStrings failed!\n";
  }

  if (table_normal_de.readTable(stringsPath+".Strings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Table read successfully! Table contains "<<table_normal_de.getNumberOfTableEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading table "<< stringsPath<<".Strings failed!\n";
  }

  std::cout << "Merging 3rd and 2nd table, resulting table should have "
            << table_normal_de.getNumberOfTableEntries()+table_il_de.getNumberOfTableEntries() << " entries.\n";
  table_normal_de.mergeTables(table_il_de);
  std::cout << "Entries after first merge: "<<table_normal_de.getNumberOfTableEntries();
  std::cout << "\n\nMerging with 1st table, resulting table should have "
            << table_normal_de.getNumberOfTableEntries()+table_dl_de.getNumberOfTableEntries() << " entries.\n";
  table_normal_de.mergeTables(table_dl_de);
  std::cout << "Entries after second merge: "<<table_normal_de.getNumberOfTableEntries()<<"\n\n";

  table_il_de.tabulaRasa();
  table_dl_de.tabulaRasa();
  std::cout << "Table has string with ID 0: "<<table_normal_de.hasString(0)<<"\n";


  //check for matches
  SRTP::StringTable::TableIterator iter_de = table_normal_de.getBegin();
  const SRTP::StringTable::TableIterator iter_de_end = table_normal_de.getEnd();

  SRTP::StringTable::TableIterator iter_en = table_normal_en.getBegin();
  const SRTP::StringTable::TableIterator iter_en_end = table_normal_en.getEnd();

  while ((iter_en!=iter_en_end) and (iter_de!=iter_de_end))
  {
    if (iter_en->first!=iter_de->first)
    {
      std::cout << "Error: string indices in tables do not match: en is "
                << iter_en->first <<", but de is "<<iter_de->first<<".\n";
      return 0;
    }//if
    ++iter_de;
    ++iter_en;
  }//while

  if ((iter_en!=iter_en_end) != (iter_de!=iter_de_end))
  {
    std::cout << "Error: one table is finished but the other is not!\n";
    return 0;
  }//if

  std::cout << "String indices match so far!\nCreating list...\n";

  std::vector<TranslationElem> dictionary;

  iter_de = table_normal_de.getBegin();
  iter_en = table_normal_en.getBegin();


  //Note: Recommended length for DB is 34 for both.
  //Note: 16/17 works for Skyrim.
  const std::string::size_type cMaxEnLength = 16;
  const std::string::size_type cMaxDeLength = 16;

  TranslationElem temp;
  while ((iter_en!=iter_en_end) and (iter_de!=iter_de_end))
  {
    temp.English = iter_en->second;
    temp.index = iter_de->first;
    temp.German = iter_de->second;
    //length check
    if ((temp.English.length()<=cMaxEnLength) and (temp.German.length()<=cMaxDeLength))
      dictionary.push_back(temp);
    ++iter_de;
    ++iter_en;
  }//while

  std::cout << "Dictionary has "<<dictionary.size()<<" entries.\n";
  std::cout << "Sorting...\n";
  std::sort(dictionary.begin(), dictionary.end());
  std::cout << "Done.\n";

  //remove duplicates
  std::cout << "Removing duplicate translations...\n";
  unsigned int i = 1;
  while (i<dictionary.size())
  {
    if ((dictionary[i-1].English==dictionary[i].English)
       and (dictionary[i-1].German==dictionary[i].German))
    {
      dictionary.erase(dictionary.begin()+i);
    }//if
    else
    {
      ++i;
    }
  }//while
  std::cout << "Dictionary has "<<dictionary.size()<<" entries after duplicate removal.\n";

  std::cout << "Writing file...\n";

  std::ofstream output;
  output.open("dictionary.txt" , std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
  if (!output)
  {
    std::cout << "Error while opening file!\n";
    return 0;
  }

  output << "Skyrim:\n\n[list]";
  std::vector<TranslationElem>::const_iterator vec_iter = dictionary.begin();
  const std::vector<TranslationElem>::const_iterator vec_iter_end = dictionary.end();
  while (vec_iter!=vec_iter_end)
  {
    output << "[*]" << vec_iter->English << " - " << vec_iter->German <<"\n";
    ++vec_iter;
  }//while
  output << "[/list]";
  if (!output.good())
  {
    std::cout << "Error while writing to file!\n";
    output.close();
    return 0;
  }
  output.close();
  std::cout << "File was written successfully!\n";
  return 0;
}
