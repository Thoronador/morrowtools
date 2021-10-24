/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021  Thoronador

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

#ifndef SRTP_BSACLI_OPERATIONS_HPP
#define SRTP_BSACLI_OPERATIONS_HPP

#include <optional>
#include <string_view>

namespace SRTP
{

enum class Operation
{
  List,
  Info
};

/** \brief Parses a string containing an operation into an enumeration.
 *
 * \param op   the string containing the operation name
 * \return Returns an optional containing the associated enumeration, if a
 *         match was found. Returns an empty optional otherwise.
 */
std::optional<Operation> parseOperation(const std::string_view op);

} // namespace

#endif // SRTP_BSACLI_OPERATIONS_HPP
