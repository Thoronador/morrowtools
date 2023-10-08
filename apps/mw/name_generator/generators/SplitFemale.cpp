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

#include "SplitFemale.hpp"
#include <random>
#include "../../../../lib/mw/NPCs.hpp"

namespace MWTP
{

SplitFemale::SplitFemale()
: first_names({ }),
  last_names({ })
{
}

std::vector<std::string> SplitFemale::generate(const uint_least16_t n, const std::string& raceId)
{
  if (first_names.empty() && last_names.empty())
  {
    prepare(raceId);
  }

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<decltype(first_names)::size_type> first_distrib(0, first_names.size() - 1);
  std::uniform_int_distribution<decltype(last_names)::size_type> last_distrib(0, last_names.size() - 1);

  std::vector<std::string> result;
  for (uint_least16_t i = 0; i < n; ++i)
  {
    result.emplace_back(first_names[first_distrib(generator)] + " " + last_names[last_distrib(generator)]);
  }

  return result;
}

void SplitFemale::prepare(const std::string& raceId)
{
  auto iter = NPCs::get().begin();
  while (iter != NPCs::get().end())
  {
    if (iter->second.RaceID == raceId)
    {
      const auto pos = iter->second.Name.find(' ');
      const auto female = iter->second.isFemale();
      if (pos == std::string::npos)
      {
        // First name only - can only be used if gender matches.
        if (female)
          first_names.push_back(iter->second.Name);
      }
      else
      {
        if (female)
        {
          first_names.push_back(iter->second.Name.substr(0, pos));
          last_names.push_back(iter->second.Name.substr(pos + 1));
        }
        else
        {
          // Last name is gender-neutral, so it can be used, too.
          last_names.push_back(iter->second.Name.substr(pos + 1));
        }
      }
    }
    ++iter;
  }
}

} // namespace
