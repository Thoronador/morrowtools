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

#ifndef SRTP_BSACLI_COMMANDFACTORY_HPP
#define SRTP_BSACLI_COMMANDFACTORY_HPP

#include <memory>
#include "Command.hpp"
#include "Operations.hpp"

namespace SRTP::bsa_cli::CommandFactory
{

/** \brief Creates a new Command instance to match the given operation.
 *
 * \param op   the selected operation
 * \return Returns a pointer to the created instance.
 */
std::unique_ptr<Command> createCommand(const Operation op);

} // namespace

#endif // SRTP_BSACLI_COMMANDFACTORY_HPP
