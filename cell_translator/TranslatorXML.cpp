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

#include "TranslatorXML.h"
#include <fstream>
#include <iostream>
#include "XMLParser.h"

bool readCellListFromXML(const std::string& fileName, CellListType& cells, const TransDir direction)
{
  std::ifstream input;
  input.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!input)
  {
    return false;
  }
  input.seekg(0, std::ios::end);
  const size_t total = input.tellg();
  input.seekg(0, std::ios::beg);
  char* Buffer = new char[total+1];
  input.read(Buffer, total);
  if (!input.good())
  {
    input.close();
    std::cout << "Error while reading.\n";
    delete Buffer;
    return false;
  }
  input.close();
  Buffer[total] = '\0';
  XMLParser parser;
  {
    std::string contents = std::string(Buffer);
    delete Buffer;
    Buffer = NULL;
    const std::string::size_type pos = contents.find("<cellnames>");
    if (pos==std::string::npos)
    {
      std::cout << "Error: Couldn't find <cellnames> in file.\n";
      return false;
    }
    contents.erase(0, pos);
    parser.feed(contents);
  }

  //now go to work
  cells.clear();//clear old entries
  ParserToken result;
  std::string next = "";
  result = parser.parse(next);
  if ((result!=ptOpeningTag) or (next!="cellnames"))
    return false;
  enum {plNone, plCellnames, plCell, plEn, plDe} level;
  level = plCellnames;
  std::string en_cell, de_cell;
  en_cell = de_cell = "";
  do
  {
    result = parser.parse(next);
    switch (result)
    {
      case ptNone:
           break;
      case ptError:
           std::cout << "Error while parsing \""<<fileName<<"\".\n";
           return false;
           break;
      case ptOpeningTag:
           switch (level)
           {
             case plCellnames:
                  if (next!="cell")
                  {
                    std::cout << "Error: Element \""<<next<<"\" is not allowed here.\n";
                    return false;
                  }
                  level = plCell;
                  break;
             case plCell:
                  if ((next!="de") and (next!="en"))
                  {
                    std::cout << "Error: Element \""<<next<<"\" is not allowed here.\n";
                    return false;
                  }
                  if (next=="de")
                  {
                    level = plDe;
                  }
                  else
                    level = plEn;
                  break;
             case plEn:
             case plDe:
             case plNone:
                  //should never occur within a valid file
                  std::cout << "Error: Element \""<<next<<"\" is not allowed here.\n";
                  return false;
                  break;
           };//swi (inner)
           break;
      case ptClosingTag:
           switch (level)
           {
             case plEn:
                  if (next!="en")
                  {
                    std::cout << "Error: Closing tag \""<<next<<"\" is not allowed here.\n";
                    return false;
                  }
                  level = plCell;
                  break;
             case plDe:
                  if (next!="de")
                  {
                    std::cout << "Error: Closing tag \""<<next<<"\" is not allowed here.\n";
                    return false;
                  }
                  level = plCell;
                  break;
             case plCell:
                  if (next!="cell")
                  {
                    std::cout << "Error: Closing tag \""<<next<<"\" is not allowed here.\n";
                    return false;
                  }
                  //now push the stuff into the list/map
                  if (!de_cell.empty() and !en_cell.empty())
                  {
                    switch (direction)
                    {
                      case td_en_de:
                           cells[en_cell] = de_cell;
                           break;
                      case td_de_en:
                           cells[de_cell] = en_cell;
                           break;
                      default:
                           //not used yet, but we have this here for the case of
                           // future updates
                           std::cout << "Error: unknown translation direction encountered.\n";
                           break;
                    }//swi (innermost)
                  }//if
                  de_cell.clear();
                  en_cell.clear();
                  level = plCellnames;
                  break;
             case plCellnames:
                  if (next!="cellnames")
                  {
                    std::cout << "Error: Closing tag \""<<next<<"\" is not allowed here.\n";
                    return false;
                  }
                  level = plNone;
                  break;
             case plNone:
                  //should never come to this point
                  std::cout << "Error: Closing tag is not allowed here.\n";
                  return false;
                  break;
           };//swi (inner)
           break;
      case ptText:
           switch(level)
           {
             case plDe:
                  if (de_cell.empty())
                  {
                    de_cell = next;
                  }
                  else
                  {
                    std::cout << "Error: Text is not allowed here, 'de' was "
                              << "already set.\n";
                    return false;
                  }
                  break;
             case plEn:
                  if (en_cell.empty())
                  {
                    en_cell = next;
                  }
                  else
                  {
                    std::cout << "Error: Text is not allowed here, 'en' was "
                              << "already set.\n";
                    return false;
                  }
                  break;
             case plCell:
             case plCellnames:
             case plNone:
                  //shouldn't happen in a valid file
                  std::cout << "Error: Text outside tags is not allowed here.\n";
                  return false;
                  break;
           }//swi (inner)
           break;
    }//swi
  } while (result!=ptNone);
  //if everything went well, the last result should have been ptNone and the
  //  last level should be plNone
  return ((result==ptNone) or (level==plNone));
}
