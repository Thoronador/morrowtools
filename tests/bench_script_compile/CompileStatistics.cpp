/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#include "CompileStatistics.hpp"
#include "../../mw/base/Scripts.hpp"
#include "../../mw/base/records/ScriptRecord.hpp"
#include "../../mw/base/script_compiler/ScriptCompiler.hpp"

namespace MWTP
{

bool canCompileScriptProperly(const MWTP::ScriptRecord& original)
{
  ScriptRecord temp;
  if (!ScriptCompiler::CompileScript(original.ScriptText, temp))
    return false;
  return original.equals(temp);
}

void displayCompileStatistics()
{
  const auto totalScripts = Scripts::get().getNumberOfRecords();
  if (totalScripts == 0)
  {
    std::cout << "Hint: There are no scripts, nothing to do here!\n";
    return;
  }
  uint_least32_t successfulScripts = 0;
  std::vector<std::string> failedScripts;

  const auto sc_end = Scripts::get().end();
  auto sc_iter = Scripts::get().begin();
  while (sc_iter != sc_end)
  {
    if (canCompileScriptProperly(sc_iter->second))
    {
      ++successfulScripts;
    }
    else
    {
      failedScripts.push_back(sc_iter->first);
      std::cout << "Warning: Could not compile script " << sc_iter->first << ".\n";
    }
    ++sc_iter;
  }

  const double percentageSuccess = static_cast<double>(successfulScripts) / totalScripts * 100.0;
  const double percentageFail = 100.0 - percentageSuccess;
  std::cout << "Status:\n"
            << "    total number of scripts: " << totalScripts << "\n"
            << "    successfully compiled:   " << successfulScripts << " (" << percentageSuccess << " %)\n"
            << "    failed to compile:       " << failedScripts.size() << " (" << percentageFail << " %)\n";
}

void compileSingleScript(const std::string& scriptId)
{
  if (!Scripts::get().hasRecord(scriptId))
  {
    std::cout << "Hint: There is no script with the id '" << scriptId
              << "'.\n";
    return;
  }

  const auto& originalScript = Scripts::get().getRecord(scriptId);
  ScriptRecord temp;
  if (!ScriptCompiler::CompileScript(originalScript.ScriptText, temp))
  {
    std::cerr << "Error: Script with the id '" << scriptId
              << "' could not be compiled.\n";
    return;
  }

  std::ofstream outOrig("analysis_original.dat", std::ios::trunc | std::ios::binary);
  if (!originalScript.saveToStream(outOrig))
  {
    std::cerr << "Error: Could not save original script to analysis_original.dat.\n";
    return;
  }
  outOrig.close();
  std::cout << "Original script data was written to analysis_original.dat.\n";

  std::ofstream outCompiled("analysis_compiled.dat", std::ios::trunc | std::ios::binary);
  if (!temp.saveToStream(outCompiled))
  {
    std::cerr << "Error: Could not save compiled script to analysis_compiled.dat.\n";
    return;
  }
  outCompiled.close();
  std::cout << "Compiled script data was written to analysis_compiled.dat.\n";
}

}
