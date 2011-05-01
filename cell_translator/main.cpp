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

#include <iostream>
#include <string>
#include <map>
#include <typeinfo>
#include "ESMReaderTranslator.h"
#include "TranslatorXML.h"
#include "../base/ESMWriterGeneric.h"
#include "../base/records/GenericRecord.h"
#include "../base/records/CellRecord.h"
#include "../base/records/CreatureRecord.h"
#include "../base/records/DialogueInfoRecord.h"
#include "../base/records/NPCRecord.h"
#include "../base/records/PathGridRecord.h"
#include "../base/records/ScriptRecord.h"
#include "../base/UtilityFunctions.h"

//return codes
const int rcInvalidParameter = 1;
const int rcFileError = 2;
const int rcOutputFailed = 4;

const int rcXMLError = 6;
const int rcXMLEmpty = 7;
const int rcScriptError = 8;

bool translatePreNPCRecord(PreNPCRecord* c_rec, const CellListType& cells)
{
  if (c_rec==NULL) return false;

  CellListType::const_iterator cells_iter;
  //translate destinations
  unsigned int i;
  for (i=0; i<c_rec->Destinations.size(); ++i)
  {
    cells_iter = cells.find(c_rec->Destinations.at(i).CellName);
    if (cells_iter!=cells.end())
    {
      c_rec->Destinations.at(i).CellName = cells_iter->second;
    }
  }//for
  //translate AI packages
  for (i=0; i<c_rec->AIPackages.size(); ++i)
  {
    switch (c_rec->AIPackages.at(i)->getPackageType())
    {
      case ptActivate:
      case ptTravel:
      case ptWander:
           //we need to do nothing here, those packages do not have cell names
           break;
      case ptEscort:
      case ptFollow:
           cells_iter = cells.find(static_cast<NPC_AIEscortFollow*>(c_rec->AIPackages.at(i))->CellName);
           if (cells_iter!=cells.end())
           {
             static_cast<NPC_AIEscortFollow*>(c_rec->AIPackages.at(i))->CellName = cells_iter->second;
           }
           break;
    }//swi
  }//for
  return true;
}

bool translateCellRecord(CellRecord* c_rec, const CellListType& cells)
{
  if (c_rec==NULL) return false;

  CellListType::const_iterator cells_iter;
  //translate cell name
  cells_iter = cells.find(c_rec->CellID);
  if (cells_iter!=cells.end())
  {
    c_rec->CellID = cells_iter->second;
  }
  //on to the references we go!
  unsigned int i;
  for (i=0; i<c_rec->References.size(); ++i)
  {
    if (c_rec->References.at(i).hasDoorData)
    {
      cells_iter = cells.find(c_rec->References.at(i).DoorData.ExitName);
      if (cells_iter!=cells.end())
      {
        c_rec->References.at(i).DoorData.ExitName = cells_iter->second;
      }
    }//if
  }//for
  return true;
}

bool getNextScriptParameter(const std::string& scriptText, size_t& start, std::string& param)
{
  const std::string::size_type len = scriptText.length();
  if (start>=len) return false;

  unsigned int look = start;
  unsigned int offset = start;
  bool insideQuote = false;
  while (look<len)
  {
    if (scriptText.at(look)=='"')
    {
      insideQuote = not insideQuote;
    }
    else if ((not insideQuote) and ((scriptText.at(look)==' ')
              or (scriptText.at(look)==',') or (scriptText.at(look)=='\t')))
    {
      //found a place where to split
      const unsigned int param_len = look-offset;
      if (param_len>0)//skip empty params
      {
        param = scriptText.substr(offset, param_len);
        StripEnclosingQuotes(param);
        start = look+1;
        return true;
      }//if
      offset=look+1;
    }//else
    ++look;
  }//while
  //we are at the end of the string, whole stuff is one piece
  param = scriptText.substr(offset);
  StripEnclosingQuotes(param);
  start = len;
  if (param.length()>0) return true;
  return false;
}

bool replaceCellsInScriptText(std::string& scriptText, const CellListType& cells)
{
  std::string::size_type offset = 0;
  std::string lowerText = lowerCase(scriptText);
  std::string newText = scriptText;

  bool changed = false;
  //potential endless loop
  while (true)
  {
    /*Instead of searching for thousands of cell names, we just try to find the
      script functions that take cell names as one of their parameters. That's
      much faster.
    */
    std::string::size_type pos_AIEscortCell = lowerText.find("aiescortcell", offset);
    std::string::size_type pos_AIFollowCell = lowerText.find("aifollowcell", offset);
    std::string::size_type pos_GetPCCell = lowerText.find("getpccell", offset);
    std::string::size_type pos_PlaceItemCell = lowerText.find("placeitemcell", offset);
    std::string::size_type pos_PositionCell = lowerText.find("positioncell", offset);

    if ((std::string::npos!=pos_AIEscortCell) or (std::string::npos!=pos_AIFollowCell))
    {
      //replace cell name in AIEscortCell/AIFollowCell command
      //params: 6 or 7: target ID, cell ID, duration, x, y, z[, reset]
      // --> skip first param, get second
      std::string param;
      size_t param_start;
      if (std::string::npos!=pos_AIEscortCell)
      {
        param_start = pos_AIEscortCell;
      }
      else param_start = pos_AIFollowCell;
      //skip function name
      if (getNextScriptParameter(newText, param_start, param))
      {
        //skip first param
        if (getNextScriptParameter(newText, param_start, param))
        {
          //remember position where cell name starts
          const size_t cell_start = param_start;
          //get and skip cell name
          if (getNextScriptParameter(newText, param_start, param))
          {
            //now param contains the cell name
            const CellListType::const_iterator iter = cells.find(param);
            if (iter!=cells.end())
            {
              newText = newText.substr(0, cell_start)+"\""+iter->second
                       +"\" "+newText.substr(param_start);
              lowerText = lowerCase(newText);
              changed = true;
            }//if some match found
          }//if (cell)
        }//if (1st param)
      }//if (function name)
      //when done, set offset to occurence +1
      if (std::string::npos!=pos_AIEscortCell)
      {
        offset = pos_AIEscortCell+1;
      }
      else param_start = pos_AIFollowCell;
    }
    else if (std::string::npos!=pos_GetPCCell)
    {
      //replace cell name in GetPCCell command
      //params: 1: cell ID
      // --> get first param
      std::string param;
      size_t param_start = pos_GetPCCell;
      //skip function name
      if (getNextScriptParameter(newText, param_start, param))
      {
        //remember position where cell name starts
        const size_t cell_start = param_start;
        //get first param
        if (getNextScriptParameter(newText, param_start, param))
        {
          const CellListType::const_iterator iter = cells.find(param);
          if (iter!=cells.end())
          {
            newText = newText.substr(0, cell_start)+"\""+iter->second
                     +"\" "+newText.substr(param_start);
            lowerText = lowerCase(newText);
            changed = true;
          }//if some match found
        }//if first param
      }//if function name
      //when done, set offset to occurence +1
      offset = pos_GetPCCell+1;
    }
    else if (std::string::npos!=pos_PlaceItemCell)
    {
      //replace cell name in PlaceItemCell command
      //params: 6: ObjectID, CellID, X, Y, Z, ZRot
      // --> skip first, get second
      std::string param;
      size_t param_start = pos_PlaceItemCell;
      //skip function name
      if (getNextScriptParameter(newText, param_start, param))
      {
        //skip first param
        if (getNextScriptParameter(newText, param_start, param))
        {
          //remember position where cell name starts
          const size_t cell_start = param_start;
          //get and skip cell name
          if (getNextScriptParameter(newText, param_start, param))
          {
            //now param contains the cell name
            const CellListType::const_iterator iter = cells.find(param);
            if (iter!=cells.end())
            {
              newText = newText.substr(0, cell_start)+"\""+iter->second
                       +"\" "+newText.substr(param_start);
              lowerText = lowerCase(newText);
              changed = true;
            }//if some match found
          }//if (cell)
        }//if (1st param)
      }//if (function name)
      //when done, set offset to occurence +1
      offset = pos_PlaceItemCell+1;
    }
    else if (std::string::npos!=pos_PositionCell)
    {
      //replace cell name in PositionCell command
      //params: 5: x, y, z, zRot, CellID
      // --> skip first four params, get fifth
      std::string param;
      size_t param_start = pos_PositionCell;
      //skip function name
      if (getNextScriptParameter(newText, param_start, param))
      {
        //skip first param
        if (getNextScriptParameter(newText, param_start, param))
        {
          //skip second param
          if (getNextScriptParameter(newText, param_start, param))
          {
            //skip third param
            if (getNextScriptParameter(newText, param_start, param))
            {
              //skip fourth param
              if (getNextScriptParameter(newText, param_start, param))
              {
                //remember position where cell name starts
                const size_t cell_start = param_start;
                //get and skip cell name
                if (getNextScriptParameter(newText, param_start, param))
                {
                  //now param contains the cell name
                  const CellListType::const_iterator iter = cells.find(param);
                  if (iter!=cells.end())
                  {
                    newText = newText.substr(0, cell_start)+"\""+iter->second
                             +"\" "+newText.substr(param_start);
                    lowerText = lowerCase(newText);
                    changed = true;
                  }//if some match found
                }//if fifth param/cell name
              }//if fourth param
            }//if third param
          }//if second param
        }//if first param
      }//if function name
      //when done, set offset to occurence +1
      offset = pos_PositionCell+1;
    }
    else
    {
      //break out of loop
      break;
    }
    #warning Might not work properly yet!
  }//while

  if (changed)
  {
    scriptText = newText;
    return true;
  }
  return false;
}

bool translateInfoRecord(DialogueInfoRecord* di_rec, const CellListType& cells)
{
  if (di_rec==NULL) return false;

  CellListType::const_iterator cells_iter;
  //translate cell ID
  cells_iter = cells.find(di_rec->CellID);
  if (cells_iter!=cells.end())
  {
    di_rec->CellID = cells_iter->second;
  }
  //result string might be a script
  if (!di_rec->ResultString.empty())
  {
    replaceCellsInScriptText(di_rec->ResultString, cells);
  }
  return true;
}

bool translateScriptRecord(ScriptRecord* script_rec, const CellListType& cells)
{
  if (script_rec==NULL) return false;
  if (replaceCellsInScriptText(script_rec->ScriptText, cells))
  {
    //something was replaced, so we have to recompile the script here
    ///TODO: translate script here
    #warning Incomplete function!
  }//if stuff was replaces
  return true;
}

void showHelp()
{
  std::cout << "\ncell_translator -f PLUGINFILE [-o FILENAME1] [-xml FILENAME2]\n"
            << "\n"
            << "(Note: This help is still incomplete.)\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the programme and quits\n"
            << "  -f PLUGINFILE    - sets the plugin PLUGINFILE as the files that will be\n"
            << "                     translated by this programme. This option is required.\n"
            << "  -o FILENAME1     - sets the name of the created output file to FILENAME1.\n"
            << "                     If omitted, the default output file is \"out.esp\".\n"
            << "  -xml FILENAME2   - sets the name of the XML file that contains the cell names\n"
            << "                     to FILENAME2. If omitted, the default XML file name is\n"
            << "                     \"cells.xml\".\n";
}

void showGPLNotice()
{
  std::cout << "Cell Translator for Morrowind\n"
            << "  This programme is part of the Morrowind Tools Project.\n"
            << "  Copyright (C) 2011 Thoronador\n"
            << "\n"
            << "  The Morrowind Tools are free software: you can redistribute them and/or\n"
            << "  modify them under the terms of the GNU General Public License as published\n"
            << "  by the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  The Morrowind Tools are distributed in the hope that they will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showVersion()
{
  std::cout << "Cell Translator for Morrowind, version 0.3_rev225, 2011-05-01\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();

  std::string pathToCellsXML = "";
  std::string pluginFile = "";
  std::string outputFileName = "";

  if ((argc>1) and (argv!=NULL))
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        //help parameter
        if ((param=="--help") or (param=="-?"))
        {
          showHelp();
          return 0;
        }//if help wanted
        //version information requested?
        else if (param=="--version")
        {
          showVersion();
          return 0;
        }
        else if (param=="-f")
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!pluginFile.empty())
            {
              std::cout << "Parameter -f must not occur more than once!\n";
              return rcInvalidParameter;
            }
            pluginFile = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "Plugin file was set to \""<<pluginFile<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }
        else if (param=="-o")
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!outputFileName.empty())
            {
              std::cout << "Parameter -o must not occur more than once!\n";
              return rcInvalidParameter;
            }
            outputFileName = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "Output file was set to \""<<outputFileName<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }
        else if (param=="-xml")
        {
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            if (!pathToCellsXML.empty())
            {
              std::cout << "Parameter -xml must not occur more than once!\n";
              return rcInvalidParameter;
            }
            pathToCellsXML = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as file name already
            std::cout << "XML file was set to \""<<pathToCellsXML<<"\".\n";
          }
          else
          {
            std::cout << "Error: You have to specify a file name after \""
                      << param <<"\".\n";
            return rcInvalidParameter;
          }
        }
        else
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \""<<param<<"\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cout << "Parameter at index "<<i<<" is NULL.\n";
        return rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if arguments present
  else
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "Use --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }

  //check for plugin file
  if (pluginFile.empty())
  {
    std::cout << "You have to specify certain parameters for this programme to run properly.\n"
              << "In particular, the parameter -f followed by a plugin file name is required.\n"
              << "Use the parameter --help to get a list of valid parameters.\n";
    return rcInvalidParameter;
  }
  //check XML file
  if (pathToCellsXML.empty())
  {
    pathToCellsXML = "cells.xml";
    std::cout << "Setting XML file name to cells.xml, because no XML file name "
              << "was specified.\n";
  }
  //check output
  if (outputFileName.empty())
  {
    outputFileName = "out.esp";
    std::cout << "Setting output file name to out.esp, because no output file "
              << "name was specified.\n";
  }

  if (pluginFile==outputFileName)
  {
    std::cout << "Warning: plugin file name and output file name are identical!\n"
              << "The plugin file will be overwritten.";
  }

  CellListType cells;
  if (readCellListFromXML(pathToCellsXML, cells, td_en_de))
  {
    if (cells.size()==0)
    {
      std::cout << "The XML file \""<<pathToCellsXML<<"\" did not contain any "
                << "cell name pairs. Thus, nothing can be translated here.\n";
      return rcXMLEmpty;
    }//if (the inner one)
    std::cout << "Parsing XML was successful. There are "<<cells.size()<<" pairs in the list.\n";
  }
  else
  {
    std::cout << "Parsing of file \""<<pathToCellsXML<<"\" failed.\n";
    return rcXMLError;
  }

  //now read the file
  ESMReaderTranslator::VectorType recordVec;
  DepFileList deps;
  deps.clear();
  ESMReaderTranslator reader(&recordVec);
  if (!reader.readESM(pluginFile, deps, false))
  {
    std::cout << "Error while reading file \""<<pluginFile<<"\".\nAborting.\n";
    reader.deallocateRecordsInVector();
    return rcFileError;
  }

  const std::string genericID = typeid(GenericRecord*).name();
  const std::string cellID = typeid(CellRecord*).name();
  const std::string creatureID = typeid(CreatureRecord*).name();
  const std::string infoID = typeid(DialogueInfoRecord*).name();
  const std::string npcID = typeid(NPCRecord*).name();
  const std::string pathgridID = typeid(PathGridRecord*).name();
  const std::string scriptID = typeid(ScriptRecord*).name();

  CellListType::const_iterator cell_iter;
  ESMReaderGeneric::VectorType::const_iterator v_iter = recordVec.begin();
  while (v_iter!=recordVec.end())
  {
    const std::string type_name = typeid(*v_iter).name();
    if (type_name==pathgridID)
    {
      cell_iter = cells.find(dynamic_cast<PathGridRecord*>(*v_iter)->CellName);
      if (cell_iter!=cells.end())
      {
        dynamic_cast<PathGridRecord*>(*v_iter)->CellName = cell_iter->second;
      }
    }//if path grid
    else if (type_name==cellID)
    {
      translateCellRecord(dynamic_cast<CellRecord*>(*v_iter), cells);
    }
    else if ((type_name==creatureID) or (type_name==npcID))
    {
      translatePreNPCRecord(dynamic_cast<PreNPCRecord*>(*v_iter), cells);
    }//if creature or NPC
    else if (type_name==infoID)
    {
      translateInfoRecord(dynamic_cast<DialogueInfoRecord*>(*v_iter), cells);
    }
    else if (type_name==scriptID)
    {
      if (!translateScriptRecord(dynamic_cast<ScriptRecord*>(*v_iter), cells))
      {
        std::cout << "Error: couldn't translate cells in script \""
                  << dynamic_cast<ScriptRecord*>(*v_iter)->ScriptID<<"\"!\n";
        reader.deallocateRecordsInVector();
        return rcScriptError;
      }//if not translated
    }//if script record
    /// TODO: Maybe translate region names, too.
    ++v_iter;
  }//while

  //try to write stuff to the output file
  ESMWriterGeneric writer(&recordVec);
  if (!writer.writeESM(outputFileName, false /* no master */, deps,
         "(TODO: put description here)"))
  {
    std::cout << "Error: Could not create or write to output file \""<<outputFileName<<"\".\n";
    reader.deallocateRecordsInVector();
    return rcOutputFailed;
  }
  reader.deallocateRecordsInVector();
  return 0;
}
