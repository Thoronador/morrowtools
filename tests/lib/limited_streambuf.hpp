/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for Morrowind Tools Project.
    Copyright (C) 2025  Dirk Stolle

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

#ifndef MWTP_TEST_LIMITED_STREAMBUF_HPP
#define MWTP_TEST_LIMITED_STREAMBUF_HPP

#include <array>
#include <cstddef>
#include <streambuf>

namespace MWTP
{

template<std::size_t limit, class CharT = char>
struct limited_streambuf : std::basic_streambuf<CharT>
{
  // public:
    using Base = std::basic_streambuf<CharT>;
    using int_type = typename Base::int_type;

    limited_streambuf()
    {
      // Set put area pointers to work with array.
      Base::setp(buffer.data(), buffer.data() + limit);
    }

    int_type overflow(int_type ch)
    {
      // Base class just returns traits_type::eof().
      return Base::overflow(ch);
    }
  private:
    std::array<CharT, limit> buffer{};
};

} // namespace

#endif // MWTP_TEST_LIMITED_STREAMBUF_HPP
