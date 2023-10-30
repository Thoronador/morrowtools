/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2023  Dirk Stolle

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

#include "NameGenFunctions.hpp"
#include <iostream>
#include <utility>
#include <vector>
#include "../../../lib/base/UtilityFunctions.hpp"
#include "../../../lib/mw/Races.hpp"

namespace MWTP
{

void removeNonPlayableRaces()
{
  auto iter = Races::get().begin();
  while(iter != Races::get().end())
  {
    if (!iter->isPlayable())
    {
      std::cout << "Removing non-playable race " << iter->recordID << " (" << iter->Name << ").\n";
      Races::get().removeRecord(iter->recordID);
      iter = Races::get().begin();
    }
    else
    {
      ++iter;
    }
  }
}

std::string selectRace()
{
  std::vector<std::pair<std::string, std::string> > id_name_pairs;
  for (auto iter = Races::get().begin(); iter != Races::get().end(); ++iter)
  {
    id_name_pairs.push_back(std::make_pair(iter->recordID, iter->Name));
  }

  int idx = 1;
  for (const auto& id_name: id_name_pairs)
  {
    std::cout << "[" << idx << "]: " << id_name.second << '\n';
    ++idx;
  }
  std::cout << "Enter number of the race [1-" << id_name_pairs.size() << "]: ";

  std::string userInput;
  std::getline(std::cin, userInput);
  int32_t selectedIndex = -1;
  if (!stringToLong(userInput, selectedIndex))
  {
    std::cout << "Error: \"" << userInput << "\" is not a valid number." << std::endl;
  }
  else
  {
    if ((selectedIndex < 1) || (static_cast<uint32_t>(selectedIndex) > id_name_pairs.size()))
    {
      std::cout << "Error: The number must be in the range between 1 and "
                << id_name_pairs.size() << "." << std::endl;
    }
    else
    {
      return id_name_pairs.at(selectedIndex - 1).first;
    }
  }

  // no valid selection
  return "";
}

std::optional<Gender> selectGender()
{
  const std::vector<Gender> selection = { Gender::Male, Gender::Female, Gender::Both };

  int idx = 1;
  for (const auto& item: selection)
  {
    std::cout << "[" << idx << "]: " << to_string(item) << '\n';
    ++idx;
  }
  std::cout << "Enter number of the gender [1-3]: ";

  std::string userInput;
  std::getline(std::cin, userInput);
  int32_t selectedIndex = -1;
  if (!stringToLong(userInput, selectedIndex))
  {
    std::cout << "Error: \"" << userInput << "\" is not a valid number." << std::endl;
  }
  else
  {
    if ((selectedIndex < 1) || (static_cast<uint32_t>(selectedIndex) > selection.size()))
    {
      std::cout << "Error: The number must be in the range between 1 and "
                << selection.size() << "." << std::endl;
    }
    else
    {
      return selection.at(selectedIndex - 1);
    }
  }

  // no valid selection
  return std::nullopt;
}

std::optional<uint_least16_t> selectCount()
{
  std::cout << "Enter number of names to generate: ";

  std::string userInput;
  std::getline(std::cin, userInput);
  int16_t count = -1;
  if (!stringToShort(userInput, count))
  {
    std::cout << "Error: \"" << userInput << "\" is not a valid number." << std::endl;
  }
  else
  {
    if (count < 1)
    {
      std::cout << "Error: The number must be larger than zero." << std::endl;
    }
    else
    {
      return static_cast<uint_least16_t>(count);
    }
  }

  return std::nullopt;
}

} // namespace
