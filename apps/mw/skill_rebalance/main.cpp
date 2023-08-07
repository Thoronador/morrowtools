/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2012, 2013, 2021  Dirk Stolle

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
#include <string>
#include "../../../lib/base/UtilityFunctions.hpp"
#include "../../../lib/base/FileFunctions.hpp"
#include "../../../lib/base/DirectoryFunctions.hpp"
#include "../../../lib/mw/ReturnCodes.hpp"
#include "../../../lib/mw/records/GameSettingRecord.hpp"
#include "../../../lib/mw/records/SkillRecord.hpp"
#include "../../../lib/mw/records/TES3Record.hpp"
#include "../../../lib/mw/GameSettings.hpp"
#include "../../../lib/mw/Skills.hpp"
#include "../../../lib/mw/RegistryFunctions.hpp"
#include "ESMReaderSkillRebalance.hpp"
#include "ESMWriterSkillRebalance.hpp"

void showHelp()
{
  std::cout << "\nskill_rebalance [-o FILENAME1] [--minimalistic] -d DIRECTORY\n"
            << "\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the program and quits\n"
            << "  --scaling-factor - sets the scaling factor that determines how fast or slow\n"
            << "                     the skills advance after the activation of the created\n"
            << "                     plugin file. This parameter is mandatory!\n"
            << "  -s               - same as --scaling-factor\n"
            << "  -o FILENAME1     - sets the name of the created output file to FILENAME1.\n"
            << "                     If omitted, the default output file is \"out.esp\".\n"
            << "  -d DIRECTORY     - path to the Data Files directory of Morrowind, including\n"
            << "                     trailing backslash\n"
            << "  -dir DIRECTORY   - same as -d\n"
            << "  --minimalistic   - Creates a smaller, minimalistic plugin that just uses\n"
            << "                     game settings to achieve the rebalance and doesn't change\n"
            << "                     any skill records.\n"
            << "  --no-training    - does not change iTrainingMod to 25.\n";
}

void showGPLNotice()
{
  std::cout << "Skill Rebalancer for Morrowind\n"
            << "  This program is part of the Morrowind Tools Project.\n"
            << "  Copyright (C) 2012, 2013, 2021  Thoronador\n"
            << "\n"
            << "  This program is free software: you can redistribute it and/or modify\n"
            << "  it under the terms of the GNU General Public License as published by\n"
            << "  the Free Software Foundation, either version 3 of the License, or\n"
            << "  (at your option) any later version.\n"
            << "\n"
            << "  This program is distributed in the hope that they will be useful,\n"
            << "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
            << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
            << "  GNU General Public License for more details.\n"
            << "\n"
            << "  You should have received a copy of the GNU General Public License\n"
            << "  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
            << "\n";
}

void showVersion()
{
  std::cout << "Skill Rebalancer for Morrowind, version 0.1.2, 2021-12-08\n";
}

int main(int argc, char **argv)
{
  showGPLNotice();
  bool goMinimalistic = false;
  bool noTraining = false;
  float scalingFactor = -1.0f;
  std::string outputFileName = "out.esp";
  std::string baseDir = ""; //"C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";

  if (argc > 1 && argv != nullptr)
  {
    int i = 1;
    while (i < argc)
    {
      if (argv[i] != nullptr)
      {
        const std::string param = std::string(argv[i]);
        // help parameter
        if (param == "--help" || param== "-?" || param== "/?")
        {
          showHelp();
          return 0;
        }
        // version information requested?
        else if (param == "--version")
        {
          showVersion();
          return 0;
        }
        // create minimalistic plugin, using only game settings and not skills
        else if (param == "--minimalistic")
        {
          goMinimalistic = true;
          std::cout << "Using minimalistic approach due to \"" << param
                    << "\".\n";
        }
        // don't change training mod
        else if (param == "--no-training")
        {
          noTraining = true;
          std::cout << "Won't change iTrainingMod due to \"" << param << "\".\n";
        }
        // parameter for setting output file
        else if ((param == "-o") || (param == "--output"))
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            outputFileName = std::string(argv[i+1]);
            ++i; // skip next parameter, because it's used as file name already
            std::cout << "Output file was set to \"" << outputFileName
                      << "\".\n";
          }
          else
          {
            std::cerr << "Error: You have to specify a file name after \""
                      << param<<"\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // data files directory
        else if ((param == "-d") || (param == "-dir") || (param == "--data-files"))
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Is it long enough to be a directory? (Minimum should be "./".)
            if (std::string(argv[i+1]).size() > 1)
            {
              baseDir = std::string(argv[i+1]);
              ++i; // skip next parameter, because it's used as directory name already
              // Does it have a trailing (back)slash? If not, add one.
              baseDir = slashify(baseDir);
              std::cout << "Data files directory was set to \"" << baseDir << "\".\n";
            }
            else
            {
              std::cerr << "Parameter \"" << std::string(argv[i+1])
                        << "\" is too short to be a proper directory path.\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: You have to specify a directory name after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        // scaling factor
        else if ((param == "-s") || (param == "--scale") || (param == "--scaling-factor"))
        {
          if ((i+1 < argc) && (argv[i+1] != nullptr))
          {
            // Does it contain a floating point value?
            if (stringToFloat(std::string(argv[i+1]), scalingFactor))
            {
              if (scalingFactor < 0.01f)
              {
                std::cout << "Chosen scaling factor was too small, setting it to 0.01 instead.\n";
                scalingFactor = 0.01f;
              }
              else if (scalingFactor > 100.0f)
              {
                std::cout << "Chosen scaling factor was too large, setting it to 100 instead.\n";
                scalingFactor = 100.0f;
              }
              ++i; // skip next parameter, because it's used as scaling factor already
              std::cout << "Scaling factor was set to " << scalingFactor << ".\n";
            }
            else
            {
              std::cerr << "Parameter \"" << std::string(argv[i+1])
                        << "\" is not a proper floating point value.\n";
              return MWTP::rcInvalidParameter;
            }
          }
          else
          {
            std::cerr << "Error: You have to specify a floating point value after \""
                      << param << "\".\n";
            return MWTP::rcInvalidParameter;
          }
        }
        else
        {
          // unknown or wrong parameter
          std::cout << "Invalid parameter given: \"" << param << "\".\n"
                    << "Use --help to get a list of valid parameters.\n";
          return MWTP::rcInvalidParameter;
        }
      }
      else
      {
        std::cerr << "Parameter at index " << i << " is NULL.\n";
        return MWTP::rcInvalidParameter;
      }
      ++i;
    }
  }

  if (scalingFactor <= 0.0f)
  {
    std::cerr << "Error: You have to specify a scaling factor for this "
              << "program to run properly. Use the --scaling-factor parameter"
              << " for that purpose. Consult the help (parameter --help) for "
              << " further information on parameters for this program.\n";
    return MWTP::rcInvalidParameter;
  }

  if ((lowerCaseCompare(outputFileName, "Morrowind.esm") == 0) ||
     (lowerCaseCompare(outputFileName, "Tribunal.esm") == 0) ||
     (lowerCaseCompare(outputFileName, "Bloodmoon.esm")) == 0)
  {
    std::cerr << "Error: Your specified output file name is the main master "
              << "file of Morrowind, Tribunal or Bloodmoon. In order to "
              << "prevent you from overwriting one of these files, the program"
              << " will be aborted. Think again!\n";
    return MWTP::rcInvalidParameter;
  }

  // Has the user specified a data directory?
  if (baseDir.empty())
  {
    // No, so let's search the registry first...
    std::cout << "Warning: Data Files directory of Morrowind was not specified, "
              << "will try to read it from the registry.\n";
    if (!MWTP::getMorrowindPathFromRegistry(baseDir))
    {
      std::cout << "Error: Could not find Morrowind's installation path in registry!\n";
      baseDir.clear();
    }
    else
    {
      if (!baseDir.empty())
      {
        // Does it have a trailing (back)slash? If not, add one.
        baseDir = slashify(baseDir);
        /* add data dir to path, because installed path points only to
           Morrowind's main directory */
        baseDir = baseDir + "Data Files" + MWTP::pathDelimiter;
        std::cout << "Data files directory was set to \"" << baseDir
                  << "\" via registry.\n";
      }
      else
      {
        std::cout << "Error: Installation path in registry is empty!\n";
      }
    }

    // check again, in case registry failed
    if (baseDir.empty())
    {
      // empty, so let's try a default value.
      baseDir = "C:\\Program Files\\Bethesda Softworks\\Morrowind\\Data Files\\";
      std::cout << "Warning: Data files directory of Morrowind was not specified, "
                << "will use default path \"" << baseDir << "\". This might not work"
                << " properly on your machine, use the parameter -d to specify "
                << "the proper path.\n";
    }
  } // if no data dir is given

  // next: read Morrowind.esm
  MWTP::TES3Record tes3Head;
  MWTP::ESMReaderSkillRebalance reader;

  const int readResult = reader.readESM(baseDir + "Morrowind.esm", tes3Head);
  if (readResult < 0)
  {
    std::cerr << "Error while reading file \"" << baseDir << "Morrowind.esm\".\nAborting.\n";
    return MWTP::rcFileError;
  }

  // process stuff
  MWTP::GameSettingRecord tempSetting;
  if (goMinimalistic)
  {
    // remove skill records, we won't need them
    MWTP::Skills::get().clear();
    // add game settings
    // -- add fMajorSkillBonus (default: 0.75)
    tempSetting.Type = MWTP::GMSTType::Float;
    tempSetting.recordID = "fMajorSkillBonus";
    tempSetting.fVal = 0.75f * scalingFactor;
    MWTP::GameSettings::get().addRecord(tempSetting);
    // -- add fMinorSkillBonus (default: 1.00)
    tempSetting.Type = MWTP::GMSTType::Float;
    tempSetting.recordID = "fMinorSkillBonus";
    tempSetting.fVal = 1.00f * scalingFactor;
    MWTP::GameSettings::get().addRecord(tempSetting);
    // -- add fMiscSkillBonus (default: 1.25)
    tempSetting.Type = MWTP::GMSTType::Float;
    tempSetting.recordID = "fMiscSkillBonus";
    tempSetting.fVal = 1.25f * scalingFactor;
    MWTP::GameSettings::get().addRecord(tempSetting);
  } // if minimalistic
  else
  {
    // non-minimalistic approach (like in plugin "More Morrowind")
    auto iter = MWTP::Skills::get().begin();
    while (iter != MWTP::Skills::get().end())
    {
      MWTP::SkillRecord scaledSkill = iter->second;
      scaledSkill.UseValues[0] /= scalingFactor;
      scaledSkill.UseValues[1] /= scalingFactor;
      scaledSkill.UseValues[2] /= scalingFactor;
      scaledSkill.UseValues[3] /= scalingFactor;
      MWTP::Skills::get().addSkill(scaledSkill);
      ++iter;
    }
  } // else (non-minimalistic)
  // training modifier
  if (!noTraining)
  {
    // add training mod
    tempSetting.Type = MWTP::GMSTType::Integer;
    tempSetting.recordID = "iTrainingMod";
    tempSetting.iVal = 25;
    MWTP::GameSettings::get().addRecord(tempSetting);
  }

  // prepare header data for writing (yes, description is German)
  if (scalingFactor > 1.0f)
  {
    tes3Head.description = "Leveln in Morrowind benoetigt mit diesem Plugin "
                         + floatToString(scalingFactor) + " mal so lange wie normalerweise";
  }
  else
  {
    tes3Head.description = "Stufenaufstiege in Morrowind benoetigen mit diesem Plugin nur noch "
                         + floatToString(scalingFactor * 100.0f) + "% der ueblichen Zeit";
  }
  tes3Head.companyName = "Thoronador";
  // get size of Morrowind.esm, because we need Morrowind.esm as dependency
  int64_t MorrowindSize = getFileSize64(baseDir+"Morrowind.esm");
  if (MorrowindSize == -1)
  {
    // use size of German Morrowind.esm of GOTY Edition as fallback
    MorrowindSize = 80640776;
  }
  tes3Head.dependencies.clear();
  tes3Head.dependencies.push_back(MWTP::DepFile("Morrowind.esm"));
  tes3Head.dependencies.at(0).size = MorrowindSize;

  MWTP::ESMWriterSkillRebalance writer;
  if (!writer.writeESM(baseDir + outputFileName, false, tes3Head))
  {
    std::cerr << "Error while creating/ writing to file \""
              << baseDir + outputFileName << "\". Aborting!\n";
    return MWTP::rcFileError;
  }

  std::cout << "Plugin file \"" << baseDir + outputFileName << "\" was created. Success!\n";
  return 0;
}
