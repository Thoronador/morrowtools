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

#include "SplitNamesOrc.hpp"
#include <random>
#include "../../../../lib/mw/NPCs.hpp"

namespace MWTP
{

SplitNamesOrc::SplitNamesOrc()
: first_names({ }),
  clan_names({ })
{
}

std::vector<std::string> SplitNamesOrc::generate(const uint_least16_t n, const bool female)
{
  if (first_names.empty() && clan_names.empty())
  {
    prepare(female);
  }

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<decltype(first_names)::size_type> first_distrib(0, first_names.size() - 1);
  std::uniform_int_distribution<decltype(clan_names)::size_type> clan_distrib(0, clan_names.size() - 1);

  std::vector<std::string> result;
  const std::string clan_prefix = female ? " gra-" : " gro-";
  for (uint_least16_t i = 0; i < n; ++i)
  {
    result.emplace_back(first_names[first_distrib(generator)] + clan_prefix + clan_names[clan_distrib(generator)]);
  }

  return result;
}

std::string extractClanName(const std::string& last_name)
{
  if ((last_name.substr(0, 4) == "gro-") || (last_name.substr(0, 4) == "gra-"))
    return last_name.substr(4);
  return last_name;
}

void SplitNamesOrc::prepare(const bool female)
{
  auto iter = NPCs::get().begin();
  while (iter != NPCs::get().end())
  {
    if (iter->second.RaceID == "Orc")
    {
      const auto pos = iter->second.Name.find(' ');
      const auto is_female = iter->second.isFemale();
      if (pos == std::string::npos)
      {
        // First name only - can only be used if gender matches.
        if (female == is_female)
          first_names.push_back(iter->second.Name);
      }
      else
      {
        if (female == is_female)
        {
          first_names.push_back(iter->second.Name.substr(0, pos));
          clan_names.push_back(extractClanName(iter->second.Name.substr(pos + 1)));
        }
        else
        {
          // Clan name is gender-neutral (after removing gra-/gro-), so it can be used, too.
          clan_names.push_back(extractClanName(iter->second.Name.substr(pos + 1)));
        }
      }
    }
    ++iter;
  }
}

} // namespace
