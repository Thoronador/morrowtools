/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
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

#include "../../../../lib/locate_catch.hpp"
#include "../../../../../apps/mw/name_generator/generators/Factory.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorArgonianBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorArgonianFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorArgonianMale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorBretonBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorBretonFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorBretonMale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorHighElfBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorHighElfFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorHighElfMale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorImperialBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorImperialFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorImperialMale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorKhajiitBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorKhajiitFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorKhajiitMale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorOrcBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorOrcFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorOrcMale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorRedguardBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorRedguardFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorRedguardMale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorWoodElfBoth.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorWoodElfFemale.hpp"
#include "../../../../../apps/mw/name_generator/generators/GeneratorWoodElfMale.hpp"

TEST_CASE("MWTP name_generator: Factory")
{
  using namespace MWTP;

  SECTION("Argonians")
  {
    const auto arg_b = Factory::create("Argonian", Gender::Both);
    GeneratorArgonianBoth* ptr_b = dynamic_cast<GeneratorArgonianBoth*>(arg_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto arg_f = Factory::create("Argonian", Gender::Female);
    GeneratorArgonianFemale* ptr_f = dynamic_cast<GeneratorArgonianFemale*>(arg_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto arg_m = Factory::create("Argonian", Gender::Male);
    GeneratorArgonianMale* ptr_m = dynamic_cast<GeneratorArgonianMale*>(arg_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("Bretons")
  {
    const auto arg_b = Factory::create("Breton", Gender::Both);
    GeneratorBretonBoth* ptr_b = dynamic_cast<GeneratorBretonBoth*>(arg_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto arg_f = Factory::create("Breton", Gender::Female);
    GeneratorBretonFemale* ptr_f = dynamic_cast<GeneratorBretonFemale*>(arg_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto arg_m = Factory::create("Breton", Gender::Male);
    GeneratorBretonMale* ptr_m = dynamic_cast<GeneratorBretonMale*>(arg_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("High Elf")
  {
    const auto alt_b = Factory::create("High Elf", Gender::Both);
    GeneratorHighElfBoth* ptr_b = dynamic_cast<GeneratorHighElfBoth*>(alt_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto alt_f = Factory::create("High Elf", Gender::Female);
    GeneratorHighElfFemale* ptr_f = dynamic_cast<GeneratorHighElfFemale*>(alt_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto alt_m = Factory::create("High Elf", Gender::Male);
    GeneratorHighElfMale* ptr_m = dynamic_cast<GeneratorHighElfMale*>(alt_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("Imperials")
  {
    const auto arg_b = Factory::create("Imperial", Gender::Both);
    GeneratorImperialBoth* ptr_b = dynamic_cast<GeneratorImperialBoth*>(arg_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto arg_f = Factory::create("Imperial", Gender::Female);
    GeneratorImperialFemale* ptr_f = dynamic_cast<GeneratorImperialFemale*>(arg_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto arg_m = Factory::create("Imperial", Gender::Male);
    GeneratorImperialMale* ptr_m = dynamic_cast<GeneratorImperialMale*>(arg_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("Khajiit")
  {
    const auto kha_b = Factory::create("Khajiit", Gender::Both);
    GeneratorKhajiitBoth* ptr_b = dynamic_cast<GeneratorKhajiitBoth*>(kha_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto kha_f = Factory::create("Khajiit", Gender::Female);
    GeneratorKhajiitFemale* ptr_f = dynamic_cast<GeneratorKhajiitFemale*>(kha_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto kha_m = Factory::create("Khajiit", Gender::Male);
    GeneratorKhajiitMale* ptr_m = dynamic_cast<GeneratorKhajiitMale*>(kha_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("Orc")
  {
    const auto orc_b = Factory::create("Orc", Gender::Both);
    GeneratorOrcBoth* ptr_b = dynamic_cast<GeneratorOrcBoth*>(orc_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto orc_f = Factory::create("Orc", Gender::Female);
    GeneratorOrcFemale* ptr_f = dynamic_cast<GeneratorOrcFemale*>(orc_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto orc_m = Factory::create("Orc", Gender::Male);
    GeneratorOrcMale* ptr_m = dynamic_cast<GeneratorOrcMale*>(orc_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("Redguard")
  {
    const auto red_b = Factory::create("Redguard", Gender::Both);
    GeneratorRedguardBoth* ptr_b = dynamic_cast<GeneratorRedguardBoth*>(red_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto red_f = Factory::create("Redguard", Gender::Female);
    GeneratorRedguardFemale* ptr_f = dynamic_cast<GeneratorRedguardFemale*>(red_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto red_m = Factory::create("Redguard", Gender::Male);
    GeneratorRedguardMale* ptr_m = dynamic_cast<GeneratorRedguardMale*>(red_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("Wood Elf")
  {
    const auto bos_b = Factory::create("Wood Elf", Gender::Both);
    GeneratorWoodElfBoth* ptr_b = dynamic_cast<GeneratorWoodElfBoth*>(bos_b.get());
    REQUIRE( ptr_b != nullptr );

    const auto bos_f = Factory::create("Wood Elf", Gender::Female);
    GeneratorWoodElfFemale* ptr_f = dynamic_cast<GeneratorWoodElfFemale*>(bos_f.get());
    REQUIRE( ptr_f != nullptr );

    const auto bos_m = Factory::create("Wood Elf", Gender::Male);
    GeneratorWoodElfMale* ptr_m = dynamic_cast<GeneratorWoodElfMale*>(bos_m.get());
    REQUIRE( ptr_m != nullptr );
  }

  SECTION("unsupported id return nullptr")
  {
    const auto gen_b = Factory::create("Foo", Gender::Both);
    REQUIRE( gen_b == nullptr );
    const auto gen_f = Factory::create("Foo", Gender::Female);
    REQUIRE( gen_f == nullptr );
    const auto gen_m = Factory::create("Foo", Gender::Male);
    REQUIRE( gen_m == nullptr );
  }
}
