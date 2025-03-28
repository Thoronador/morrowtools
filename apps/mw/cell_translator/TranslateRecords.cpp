/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2022, 2025  Dirk Stolle

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

#include "TranslateRecords.hpp"
#include "../../../lib/base/UtilityFunctions.hpp"
#include "../../../lib/mw/script_compiler/ScriptCompiler.hpp"

namespace MWTP
{

bool translatePreNPCRecord(PreNPCRecord* c_rec, const CellListType& cells, unsigned int& changedRecords)
{
  if (c_rec == nullptr)
  {
    return false;
  }

  bool changed = false;
  // translate destinations
  for (auto& destination: c_rec->Destinations)
  {
    const auto cells_iter = cells.find(destination.CellName);
    if (cells_iter != cells.end())
    {
      destination.CellName = cells_iter->second;
      changed = true;
    }
  }
  // translate AI packages
  CellListType::const_iterator cells_iter;
  for (auto* package: c_rec->AIPackages)
  {
    switch (package->getPackageType())
    {
      case PackageType::ptActivate:
      case PackageType::ptTravel:
      case PackageType::ptWander:
           // We need to do nothing here, those packages do not have cell names.
           break;
      case PackageType::ptEscort:
      case PackageType::ptFollow:
           cells_iter = cells.find(static_cast<NPC_AIEscortFollow*>(package)->CellName);
           if (cells_iter != cells.end())
           {
             static_cast<NPC_AIEscortFollow*>(package)->CellName = cells_iter->second;
             changed = true;
           }
           break;
    }
  }
  if (changed)
  {
    ++changedRecords;
  }
  return true;
}

bool translateReferences(std::vector<ReferencedObject>& references, const CellListType& cells)
{
  bool changed = false;
  for (auto& current_reference: references)
  {
    if (current_reference.DoorData.has_value())
    {
      const auto cells_iter = cells.find(current_reference.DoorData.value().ExitName);
      if (cells_iter != cells.end())
      {
        current_reference.DoorData.value().ExitName = cells_iter->second;
        changed = true;
      }
    }
  }
  return changed;
}

bool translateCellRecord(CellRecord* c_rec, const CellListType& cells, unsigned int& changedRecords)
{
  if (c_rec == nullptr)
  {
    return false;
  }

  bool changed = false;
  // translate cell name
  const CellListType::const_iterator cells_iter = cells.find(c_rec->CellID);
  if (cells_iter != cells.end())
  {
    c_rec->CellID = cells_iter->second;
    changed = true;
  }
  // On to the references we go!
  changed |= translateReferences(c_rec->ReferencesPersistent, cells);
  changed |= translateReferences(c_rec->ReferencesOther, cells);
  if (changed)
  {
    ++changedRecords;
  }
  return true;
}

bool getNextScriptParameter(const std::string& scriptText, size_t& start, std::string& param)
{
  const std::string::size_type len = scriptText.length();
  if (start >= len)
  {
    return false;
  }

  unsigned int look = start;
  unsigned int offset = start;
  bool insideQuote = false;
  while (look < len)
  {
    if (scriptText.at(look) == '"')
    {
      insideQuote = !insideQuote;
    }
    else if ((!insideQuote) && ((scriptText[look] == ' ')
              || (scriptText[look] == ',') || (scriptText[look] == '\t')))
    {
      // found a place where to split
      const unsigned int param_len = look-offset;
      if (param_len > 0) // skip empty params
      {
        param = scriptText.substr(offset, param_len);
        stripEnclosingQuotes(param);
        start = look + 1;
        return true;
      }
      offset = look + 1;
    }
    ++look;
  }
  // We are at the end of the string, whole stuff is one piece.
  param = scriptText.substr(offset);
  stripEnclosingQuotes(param);
  start = len;
  return (param.length() > 0);
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

    if ((std::string::npos != pos_AIEscortCell) || (std::string::npos != pos_AIFollowCell))
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
      else
      {
        param_start = pos_AIFollowCell;
      }
      // skip function name
      if (getNextScriptParameter(newText, param_start, param))
      {
        // skip first param
        if (getNextScriptParameter(newText, param_start, param))
        {
          // remember position where cell name starts
          const size_t cell_start = param_start;
          // get and skip cell name
          if (getNextScriptParameter(newText, param_start, param))
          {
            // now param contains the cell name
            const CellListType::const_iterator iter = cells.find(param);
            if (iter != cells.end())
            {
              newText = newText.substr(0, cell_start) + "\"" +iter->second
                       + "\" " + newText.substr(param_start);
              lowerText = lowerCase(newText);
              changed = true;
            } // if some match found
          } // if (cell)
        } // if (1st param)
      } // if (function name)
      // when done, set offset to occurrence + 1
      if (std::string::npos != pos_AIEscortCell)
      {
        offset = pos_AIEscortCell + 1;
      }
      else
      {
        offset = pos_AIFollowCell + 1;
      }
    }
    else if (std::string::npos != pos_GetPCCell)
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
          if (iter != cells.end())
          {
            newText = newText.substr(0, cell_start) + "\"" + iter->second
                     + "\" " + newText.substr(param_start);
            lowerText = lowerCase(newText);
            changed = true;
          } // if some match found
        } // if first param
      } // if function name
      // when done, set offset to occurrence + 1
      offset = pos_GetPCCell + 1;
    }
    else if (std::string::npos != pos_PlaceItemCell)
    {
      //replace cell name in PlaceItemCell command
      //params: 6: ObjectID, CellID, X, Y, Z, ZRot
      // --> skip first, get second
      std::string param;
      size_t param_start = pos_PlaceItemCell;
      // skip function name
      if (getNextScriptParameter(newText, param_start, param))
      {
        // skip first param
        if (getNextScriptParameter(newText, param_start, param))
        {
          // remember position where cell name starts
          const size_t cell_start = param_start;
          // get and skip cell name
          if (getNextScriptParameter(newText, param_start, param))
          {
            // now param contains the cell name
            const CellListType::const_iterator iter = cells.find(param);
            if (iter != cells.end())
            {
              newText = newText.substr(0, cell_start) + "\"" + iter->second
                       + "\" " + newText.substr(param_start);
              lowerText = lowerCase(newText);
              changed = true;
            } // if some match found
          } // if (cell)
        } // if (1st param)
      } // if (function name)
      // when done, set offset to occurrence + 1
      offset = pos_PlaceItemCell + 1;
    }
    else if (std::string::npos != pos_PositionCell)
    {
      //replace cell name in PositionCell command
      //params: 5: x, y, z, zRot, CellID
      // --> skip first four params, get fifth
      std::string param;
      size_t param_start = pos_PositionCell;
      // skip function name
      if (getNextScriptParameter(newText, param_start, param))
      {
        // skip first param
        if (getNextScriptParameter(newText, param_start, param))
        {
          // skip second param
          if (getNextScriptParameter(newText, param_start, param))
          {
            // skip third param
            if (getNextScriptParameter(newText, param_start, param))
            {
              // skip fourth param
              if (getNextScriptParameter(newText, param_start, param))
              {
                // remember position where cell name starts
                const size_t cell_start = param_start;
                // get and skip cell name
                if (getNextScriptParameter(newText, param_start, param))
                {
                  // now param contains the cell name
                  const CellListType::const_iterator iter = cells.find(param);
                  if (iter != cells.end())
                  {
                    newText = newText.substr(0, cell_start) + "\"" + iter->second
                             + "\" " + newText.substr(param_start);
                    lowerText = lowerCase(newText);
                    changed = true;
                  } // if some match found
                } // if fifth param/cell name
              } // if fourth param
            } // if third param
          } // if second param
        } // if first param
      } // if function name
      // when done, set offset to occurrence + 1
      offset = pos_PositionCell + 1;
    }
    else
    {
      //break out of loop
      break;
    }
    /// TODO: Might not work properly yet!
  } // while

  if (changed)
  {
    scriptText = newText;
    return true;
  }
  return false;
}

bool translateInfoRecord(DialogueInfoRecord* di_rec, const CellListType& cells, unsigned int& changedRecords)
{
  if (di_rec == nullptr)
  {
    return false;
  }

  bool changed = false;
  // translate cell ID
  const auto cells_iter = cells.find(di_rec->CellID);
  if (cells_iter != cells.end())
  {
    di_rec->CellID = cells_iter->second;
    changed = true;
  }
  // result string might be a script
  if (!di_rec->ResultString.empty())
  {
    if (replaceCellsInScriptText(di_rec->ResultString, cells))
    {
      changed = true;
    }
  }
  if (changed)
  {
    ++changedRecords;
  }
  return true;
}

/*
bool translateScriptRecord(ScriptRecord* script_rec, const CellListType& cells, unsigned int& changedRecords)
{
  if (script_rec == nullptr)
  {
    return false;
  }
  if (replaceCellsInScriptText(script_rec->ScriptText, cells))
  {
    ++changedRecords;
    // something was replaced, so we have to recompile the script here
    return ScriptCompiler::CompileScript(script_rec->ScriptText, *script_rec);
  }
  return true;
}
*/

bool canCompileScriptProperly(const ScriptRecord& original)
{
  ScriptRecord temp;
  if (!ScriptCompiler::CompileScript(original.ScriptText, temp))
  {
    return false;
  }
  return original.equals(temp);
}

} // namespace
