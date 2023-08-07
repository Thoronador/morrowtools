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

#ifndef SRTP_BSACLI_COMMANDS_ARGUMENTPARSINGUTILITIES_HPP
#define SRTP_BSACLI_COMMANDS_ARGUMENTPARSINGUTILITIES_HPP

#include <string>

namespace SRTP::bsa_cli
{

/** \brief Parses command line arguments for commands that only need the BSA file name.
 *
 * \param  bsaFileName  the string where the BSA file name will be stored;
 *                      MUST be empty when entering the function
 * \return Returns zero in case of success.
 *         Returns a non-zero exit code in case of failure.
 * \remark The return value can be used as exit code of the main() function.
 */
int parseArgumentsBsaFileNameOnly(int argc, char** argv, std::string& bsaFileName);

} // namespace

#endif // SRTP_BSACLI_COMMANDS_ARGUMENTPARSINGUTILITIES_HPP
