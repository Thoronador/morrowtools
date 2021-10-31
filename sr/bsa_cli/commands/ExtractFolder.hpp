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

#ifndef SRTP_BSACLI_COMMAND_EXTRACTFOLDER_HPP
#define SRTP_BSACLI_COMMAND_EXTRACTFOLDER_HPP

#include <string>
#include "Command.hpp"

namespace SRTP::bsa_cli
{

class ExtractFolder: public Command
{
  public:
    ExtractFolder();

    /** \brief Parses command line arguments.
     *
     * \param argc  number of arguments
     * \param argv  array of argument values
     * \return Returns zero in case of success.
     *         Returns a non-zero exit code in case of failure.
     * \remark The return value can be used as exit code of the main() function.
     */
    int parseArguments(int argc, char** argv) final;

    /** \brief Extracts a folder and its contents from the BSA.
     *
     * \return Returns zero in case of success.
     *         Returns a non-zero exit code in case of failure.
     * \remark The return value can be used as exit code of the main() function.
     */
    int run() final;

    /** \brief Gets a short help message (e. g. one line) for the command.
     *
     * \return Returns a short help message describing the command.
     */
    std::string helpShort() const final;

    /** \brief Gets the full help message (e. g. several lines) for the command.
     *
     * \param  binaryName  name of the binary file
     * \return Returns a help message describing the command in detail.
     */
    std::string helpFull(const std::string_view binaryName) const final;
  private:
    std::string bsaFileName; /**< name of the BSA file */
    std::string archiveFolderName; /**< name of the folder inside the BSA */
    std::string outputDirectoryName; /**< destination for extraction */
};

} // namespace

#endif // SRTP_BSACLI_COMMAND_EXTRACTFOLDER_HPP
