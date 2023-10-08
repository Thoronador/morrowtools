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

#include "Factory.hpp"
#include "GeneratorBretonBoth.hpp"
#include "GeneratorBretonFemale.hpp"
#include "GeneratorBretonMale.hpp"
#include "GeneratorImperialBoth.hpp"
#include "GeneratorImperialFemale.hpp"
#include "GeneratorImperialMale.hpp"

namespace MWTP
{

std::unique_ptr<Generator> Factory::create(const std::string& race, const Gender gender)
{
  if (race == "Breton")
  {
    switch (gender)
    {
      case Gender::Male:
           return std::make_unique<GeneratorBretonMale>();
      case Gender::Female:
           return std::make_unique<GeneratorBretonFemale>();
      case Gender::Both:
           return std::make_unique<GeneratorBretonBoth>();
    }
  }

  if (race == "Imperial")
  {
    switch (gender)
    {
      case Gender::Male:
           return std::make_unique<GeneratorImperialMale>();
      case Gender::Female:
           return std::make_unique<GeneratorImperialFemale>();
      case Gender::Both:
           return std::make_unique<GeneratorImperialBoth>();
    }
  }

  // Not supported or not implemented yet.
  return nullptr;
}

} // namespace
