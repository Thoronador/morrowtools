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

#include "RandomFunctions.hpp"
#include <random>

namespace MWTP
{

std::string randomAlphaNumericSequence(const unsigned int length)
{
  static const std::string characters = "abcdefghijklmnopqrstuvwxyz0123456789";
  std::random_device randDev;
  std::mt19937 generator(randDev());
  std::uniform_int_distribution<unsigned int> distribution(0, characters.size());
  std::string result(length, 'a');
  for (unsigned int i = 0; i < length; ++i)
  {
    result[i] = characters[distribution(generator)];
  }
  return result;
}

} // namespace
