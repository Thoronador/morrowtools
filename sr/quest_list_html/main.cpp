/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2012 Thoronador

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
#include "../base/ESMReaderSingleType.h"
#include "../base/Quests.h"
#include "../base/records/TES4HeaderRecord.h"
#include "../base/FormIDFunctions.h"
#include "../base/PathFunctions.h"
#include "../base/SR_Constants.h"
#include "../base/StringTable.h"
#include "../../base/UtilityFunctions.h"

std::string replace_ltgt(std::string str)
{
  std::string::size_type pos = str.find('<');
  while (pos!=std::string::npos)
  {
    str.replace(pos, 1, "&lt;");
    pos = str.find('<');
  }
  pos = str.find('>');
  while (pos!=std::string::npos)
  {
    str.replace(pos, 1, "&gt;");
    pos = str.find('>');
  }
  return str;
}

struct QuestListElement
{
  std::string questName;
  uint32_t    formID;

  /* constructor */
  QuestListElement(const std::string& qName, const uint32_t fID)
  : questName(qName), formID(fID)
  { }

  /* comparison operator */
  bool operator<(const QuestListElement& other) const
  {
    const int lcc = lowerCaseCompare(questName, other.questName);
    if (lcc<0) return true;
    if (lcc>0) return false;
    //form ID decides, if the names are equal
    return (formID<other.formID);
  }
};//struct

int main()
{
  SRTP::Tes4HeaderRecord header_rec;
  SRTP::ESMReaderSingleType<SRTP::QuestRecord, SRTP::Quests, SRTP::cQUST> reader;

  std::string dataDir;
  SRTP::getDataDir(dataDir);
  const std::string esxFile = "Skyrim.esm";
  const std::string esxName = "Skyrim";

  const int readResult = reader.readESM(dataDir+esxFile, header_rec);
  if (readResult>=0)
  {
    std::cout << "File was read successfully! Groups read: "<<readResult<<".\n";
  }
  else
  {
    std::cout << "Reading file failed!\n";
    return 0;
  }
  std::cout << "Records read so far: "<<SRTP::Quests::getSingleton().getNumberOfRecords()<<"\n";


  std::string lang;
  const int lcc_return = SRTP::getLanguageComponent(dataDir, esxName, lang);
  if (lcc_return!=0)
    return lcc_return;

  //String tables
  SRTP::StringTable table;
  if (!table.readTable(dataDir+"Strings\\"+esxName+"_"+lang+".DLStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Reading table "<<dataDir+"Strings\\"+esxName+"_"+lang+".DLStrings failed!\n";
    return 0;
  }

  if (!table.readTable(dataDir+"Strings\\"+esxName+"_"+lang+".ILStrings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Reading table "<<dataDir+"Strings\\"+esxName+"_"+lang+".ILStrings failed!\n";
    return 0;
  }

  if (table.readTable(dataDir+"Strings\\"+esxName+"_"+lang+".Strings", SRTP::StringTable::sdUnknown))
  {
    std::cout << "Table read successfully! Table contains "<<table.getNumberOfTableEntries()<<" entries.\n";
  }
  else
  {
    std::cout << "Reading table "<<dataDir+"Strings\\"+esxName+"_"+lang+".Strings failed!\n";
    return 0;
  }

  std::set<QuestListElement> efSet;

  SRTP::Quests::ListIterator iter = SRTP::Quests::getSingleton().getBegin();
  while (iter!=SRTP::Quests::getSingleton().getEnd())
  {
    if (iter->second.hasFULL)
    {
      if (table.hasString(iter->second.unknownFULL))
      {
        efSet.insert(QuestListElement(table.getString(iter->second.unknownFULL), iter->second.headerFormID));
      }
    }
    ++iter;
  }
  std::cout << "efSet has now "<<efSet.size()<<" entries.\n";


  std::ofstream output;
  output.open(("Questliste_EditorID_"+lang+"_html.txt").c_str(), std::ios_base::out | /*std::ios_base::binary |*/ std::ios_base::trunc);
  if (!output)
  {
    std::cout << "Fehler: Datei konnte nicht geöffnet werden!\n";
    return 0;
  }

  std::set<QuestListElement>::const_iterator setIter = efSet.begin();
  //unsigned int num = 0;
  output << "<table cellspacing=\"3\" cellpadding=\"3\" border=\"1\">\n";
  while (setIter!=efSet.end())
  {
    const SRTP::QuestRecord& q_rec = SRTP::Quests::getSingleton().getRecord(setIter->formID);
    output << "<tr>\n  <td colspan=\"2\"><b>";
    output << "\""<<replace_ltgt(table.getString(q_rec.unknownFULL))<< "\", Quest-ID \""
           << q_rec.editorID<<"\"</b></td>\n</tr>\n";

    unsigned int i, j;
    const unsigned int count = q_rec.indices.size();

    //full quest info requested
    if (count==0)
      output << "<tr>\n  <td colspan=\"2\">Indices: keine</td>\n</tr>\n";
    for (i=0; i<count; ++i)
    {
      output << "<tr>\n  <td>";
      output << "Index "<<q_rec.indices[i].index<<"</td>\n";
      output << "  <td>";
      //run through QSDTs
      const unsigned int qsdt_count = q_rec.indices[i].theQSDTs.size();
      for (j=0; j<qsdt_count; ++j)
      {
        if (q_rec.indices[i].theQSDTs[j].isFinisher)
        {
          output << "(Questende) ";
        }
        if (table.hasString(q_rec.indices[i].theQSDTs[j].unknownCNAM))
        {
          output <<replace_ltgt(table.getString(q_rec.indices[i].theQSDTs[j].unknownCNAM));
        }
        output << "\n";
      }//for j
      //check for objective
      if (q_rec.hasQOBJForIndex(q_rec.indices[i].index))
      {
        const SRTP::QuestRecord::QOBJEntry& ziel = q_rec.getQOBJForIndex(q_rec.indices[i].index);
        if (table.hasString(ziel.unknownNNAM))
        {
          output <<"<br>[Neues Ziel] "<<replace_ltgt(table.getString(ziel.unknownNNAM))<<"\n";
        }
      }
      output << "</td>\n</tr>\n";
    }//for i
    ++setIter;
  }//while
  output << "</table>";
  output.close();

  return 0;
}
