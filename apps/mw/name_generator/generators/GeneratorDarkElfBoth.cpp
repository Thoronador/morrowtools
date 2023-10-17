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

#include "GeneratorDarkElfBoth.hpp"
#include <algorithm>
#include <random>
#include "GeneratorDarkElfFemale.hpp"
#include "GeneratorDarkElfMale.hpp"

namespace MWTP
{

std::vector<std::string> GeneratorDarkElfBoth::generate(const uint_least16_t n)
{
  GeneratorDarkElfFemale f;
  GeneratorDarkElfMale m;

  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<uint_least16_t> distrib(n / 2, n / 2 + (n & 0x0001u));
  const uint_least16_t female_count = distrib(generator);

  auto result = f.generate(female_count);
  const auto male = m.generate(n - female_count);
  result.insert(result.end(), male.begin(), male.end());
  std::shuffle(result.begin(), result.end(), generator);
  return result;
}

} // namespace
