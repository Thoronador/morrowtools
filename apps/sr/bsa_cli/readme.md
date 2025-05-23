# bsa-cli - a simple command-line interface for BSA files

`bsa-cli` is a command-line tool to perform some operations on BSA files, an
archive format that is used by The Elder Scrolls V: Skyrim (and some other
games). It may also work with BSA files of The Elder Scrolls IV: Oblivion, but
this has not been tested yet. It will certainly _not_ work with BSA files of
The Elder Scrolls III: Morrowind, because Morrowind uses a different BSA format,
despite using the same file extension.

While this tool technically does not need a working installation of The Elder
Scrolls V: Skyrim, it needs BSA files to operate on, and those usually come with
Skyrim (and also with some other games).

## Usage

```
bsa-cli [OPTIONS] COMMAND BSA_FILE

options:
  --help     - displays this help message and quits
  -?         - same as --help
  --version  - displays the version of the program and quits
  -v         - same as --version
  COMMAND    - sets the command that shall be performed on the BSA
               file. Allowed commands are:

                   commands           - lists available commands
                   check-hashes       - checks hashes of files and directories
                                        in the archive
                   directories        - lists all directories in the archive
                   directory-metadata - shows metadata of the directories in
                                        the archive
                   extract-all        - extracts everything from the archive
                   extract-directory  - extracts a single directory from the
                                        archive
                   extract-file       - extracts a single file from the archive
                   file-metadata      - shows metadata of the files in the
                                        archive
                   help               - shows help for a command
                   info               - shows BSA header information
                   list               - lists all directories and files in the
                                        archive

               More commands may be added in the future.
               The command must be given in most cases.
  BSA_FILE   - set path to the BSA file to operate on to BSA_FILE.
               The BSA_FILE must be given in most cases.

Use 'bsa-cli help <command>' for more information on a specific command.
```

## History of changes

A changelog is available in [changelog.md](./changelog.md).

## Third-party libraries

Used third-party libraries and their copyright notices are listed in
[third-party.md](./third-party.md).

## Copyright and Licensing

Copyright 2021, 2022, 2023, 2024, 2025  Dirk Stolle

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
