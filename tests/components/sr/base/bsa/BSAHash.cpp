/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Skyrim Tools Project.
    Copyright (C) 2022  Dirk Stolle

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

#include <filesystem>
#include <type_traits>
#include "../../../locate_catch.hpp"
#include "../../../../../sr/base/bsa/BSAHash.hpp"

TEST_CASE("BSAHash")
{
  using namespace SRTP;

  SECTION("check hash type")
  {
    REQUIRE( std::is_integral_v<BSAHash> );
    REQUIRE( std::is_unsigned_v<BSAHash> );
  }

  SECTION("calculateHash")
  {
    SECTION("calculation is case insensitive")
    {
      const BSAHash hash = calculateHash("foo.dds");

      REQUIRE( calculateHash("Foo.dds") == hash );
      REQUIRE( calculateHash("FOO.dds") == hash );
      REQUIRE( calculateHash("foo.Dds") == hash );
      REQUIRE( calculateHash("foo.DDS") == hash );
      REQUIRE( calculateHash("FOO.DDS") == hash );
    }

    SECTION("test against known values")
    {
      SECTION("DDS files")
      {
        REQUIRE( calculateHash("dog.dds") == 0x8ddba9c56403efe7 );
        REQUIRE( calculateHash("dog_n.dds") == 0x8e4ac57d6405dfee );
        REQUIRE( calculateHash("dog_s.dds") == 0x8e4ac57d6405dff3 );
        REQUIRE( calculateHash("falmervampireteeth.dds") == 0x31ade1fa6612f4e8 );
        REQUIRE( calculateHash("fxbutterflygreen.dds") == 0xbc6aa5446610e5ee );
      }

      SECTION("NIF files")
      {
        REQUIRE( calculateHash("caveishaftrstraight01.nif") ==  0x011ede086315b031);
        REQUIRE( calculateHash("hayscatter01.nif") == 0x7688bca3680cb031 );
        REQUIRE( calculateHash("hayscatter02.nif") == 0x7688bca3680cb032 );
        REQUIRE( calculateHash("thalmorbootsf.nif") == 0x79bfee0c740df366 );
        REQUIRE( calculateHash("circletargonianm10.nif") == 0xffd5a3286312b130 );
      }

      SECTION("TXT files")
      {
        REQUIRE( calculateHash("controlmap.txt") == 0xafb8af35630a6170 );
        REQUIRE( calculateHash("fontconfig.txt") == 0xf5d4747b660a6967 );
        REQUIRE( calculateHash("gamepad.txt") == 0x6cf7725967076164 );
        REQUIRE( calculateHash("keyboard_english.txt") == 0x1fdc44b06b107368 );
        REQUIRE( calculateHash("mouse.txt") == 0x963fc2886d057365 );
      }

      SECTION("WAV files")
      {
        REQUIRE( calculateHash("amb_telmithryn_float_magic_01_lp.wav") ==  0x071f78eee1206c70);
        REQUIRE( calculateHash("qst_da16_magic_barrier_key_off.wav") == 0x97874854f11e6666 );
        REQUIRE( calculateHash("fx_melee_draugr_01.wav") == 0xfc79793ee6123031 );
        REQUIRE( calculateHash("fx_melee_draugr_02.wav") == 0xfc79793ee6123032 );
        REQUIRE( calculateHash("fx_melee_draugr_03.wav") == 0xfc79793ee6123033 );
      }
    }
  }

  SECTION("calculateDirectoryHash")
  {
    SECTION("calculation is case insensitive")
    {
      const BSAHash hash = calculateDirectoryHash("foo\\bar\\bazinga");

      REQUIRE( calculateDirectoryHash("Foo\\Bar\\Bazinga") == hash );
      REQUIRE( calculateDirectoryHash("FOO\\BAR\\BAZINGA") == hash );
      REQUIRE( calculateDirectoryHash("FOO\\bar\\BAZinga") == hash );
    }

    SECTION("calculation is insensitive to forward slashes")
    {
      const BSAHash hash = calculateDirectoryHash("foo\\bar\\baz");

      REQUIRE( calculateDirectoryHash("foo/bar/baz") == hash );
      REQUIRE( calculateDirectoryHash("foo\\bar/baz") == hash );
      REQUIRE( calculateDirectoryHash("foo/bar\\baz") == hash );
    }

    SECTION("test against known values")
    {
      SECTION("single directory level")
      {
        REQUIRE( calculateDirectoryHash("grass") == 0x00721c6f67057373 );
        REQUIRE( calculateDirectoryHash("interface") == 0xe1e631ce69096365 );
        REQUIRE( calculateDirectoryHash("music") == 0x00751d3e6d056963 );
        REQUIRE( calculateDirectoryHash("scripts") == 0x36aebb9673077473 );
        REQUIRE( calculateDirectoryHash("seq") == 0x0000000073036571 );
        REQUIRE( calculateDirectoryHash("strings") == 0x4da2984373076773 );
        REQUIRE( calculateDirectoryHash("textures") == 0xd507789e74086573 );
      }

      SECTION("multiple directory levels")
      {
        REQUIRE( calculateDirectoryHash("interface\\exported") == 0x4b5abe2069126564 );
        REQUIRE( calculateDirectoryHash("interface\\controls\\pc") == 0xe5a2baba69157063 );
        REQUIRE( calculateDirectoryHash("meshes\\actors\\cow\\characters") == 0x052e72646d1c7273 );
        REQUIRE( calculateDirectoryHash("meshes\\dungeons\\dwemer\\animated\\astrolabe\\lexiconstand") == 0x1c7263416d366e64 );
        REQUIRE( calculateDirectoryHash("meshes\\landscape") == 0xff7c9b926d107065 );
        REQUIRE( calculateDirectoryHash("sound\\fx") == 0xeda95b2073086678 );
        REQUIRE( calculateDirectoryHash("textures\\actors\\character\\eyes") == 0x70ca23b5741e6573 );
        REQUIRE( calculateDirectoryHash("textures\\actors\\dlc01\\spriggan") == 0x7b3a1922741e616e );
      }
    }
  }
}
