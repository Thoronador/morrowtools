# bsa-cli - a simple command-line interface for BSA files

bsa-cli is a command-line tool to perform some operations on BSA files, an
archive format that is used by TES V: Skyrim (and some other games).

_Note: The bsa-cli is still in an early stage of development. So far it can only
perform very few operations._

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
                   commands     - lists available commands
                   extract-file - extracts a single file from the archive
                   folders      - lists all folders in the archive
                   help         - shows help for a command
                   info         - shows BSA header information
                   list         - lists all folders and files in the archive

               More commands may be added in the future.
               The command must be given in most cases.
  BSA_FILE   - set path to the BSA file to operate on to BSA_FILE.
               The BSA_FILE must be given in most cases.

Use `bsa-cli help <command>` for more information on a specific command.
```

## History of changes

A changelog is provided as [separate file](./changelog.md).

## Copyright and Licensing

Copyright 2021  Dirk Stolle

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
