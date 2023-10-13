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

#include "GeneratorOrcBoth.hpp"
#include <algorithm>
#include <random>
#include "GeneratorOrcFemale.hpp"
#include "GeneratorOrcMale.hpp"

namespace MWTP
{

std::vector<std::string> GeneratorOrcBoth::generate(const uint_least16_t n)
{
  GeneratorOrcFemale f;
  GeneratorOrcMale m;

  auto result = f.generate(n / 2);
  const auto male = m.generate(n / 2);
  result.insert(result.end(), male.begin(), male.end());

  std::random_device device;
  std::mt19937 generator(device());

  if ((n & 0x0001) != 0)
  {
    std::uniform_int_distribution<uint16_t> distrib(0, 1);

    const auto elem =  distrib(generator) == 1 ? f.generate(1).at(0) : m.generate(1).at(0);
    result.emplace_back(elem);
  }

  std::shuffle(result.begin(), result.end(), generator);

  return result;
}

} // namespace
