/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2021, 2022  Dirk Stolle

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

#include <array>
#include <optional>
#include <string>

namespace SRTP
{

/// enumeration type for possible operations of bsa-cli
enum class Operation
{
  Commands,
  ExtractAll,
  ExtractFile,
  ExtractFolder,
  Folders,
  Help,
  Info,
  List,
  Metadata
};

/** \brief Parses a string containing an operation into an enumeration.
 *
 * \param op   the string containing the operation name
 * \return Returns an optional containing the associated enumeration, if a
 *         match was found. Returns an empty optional otherwise.
 */
std::optional<Operation> parseOperation(const std::string_view op);

/** \brief Returns string representation of an operation.
 *
 * \param op  the operation
 * \return Returns the operation name.
 */
std::string operationToString(const Operation op);

/** \brief Gets a container containing all possible operations.
 *
 * \remarks There is no real reflection in C++, so we use this instead.
 * \return Returns an array containing all possible operations.
 */
constexpr std::array<Operation, 9> allOperations()
{
  return {
    Operation::Commands,
    Operation::ExtractAll,
    Operation::ExtractFile,
    Operation::ExtractFolder,
    Operation::Folders,
    Operation::Help,
    Operation::Info,
    Operation::List,
    Operation::Metadata
  };
}

} // namespace

#endif // SRTP_BSACLI_OPERATIONS_HPP
