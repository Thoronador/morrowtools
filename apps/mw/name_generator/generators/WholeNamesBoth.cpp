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

#include "WholeNamesBoth.hpp"
#include <random>
#include "../../../../lib/mw/NPCs.hpp"

namespace MWTP
{

WholeNamesBoth::WholeNamesBoth()
: names({ })
{
}

std::vector<std::string> WholeNamesBoth::generate(const uint_least16_t n, const std::string& raceId)
{
  if (names.empty())
  {
    prepare(raceId);
  }

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<decltype(names)::size_type> distribution(0, names.size() - 1);

  std::vector<std::string> result;
  for (uint_least16_t i = 0; i < n; ++i)
  {
    result.emplace_back(names[distribution(generator)]);
  }

  return result;
}

void WholeNamesBoth::prepare(const std::string& raceId)
{
  auto iter = NPCs::get().begin();
  while (iter != NPCs::get().end())
  {
    if (iter->second.RaceID == raceId)
    {
      names.push_back(iter->second.Name);
    }
    ++iter;
  }
}

} // namespace
