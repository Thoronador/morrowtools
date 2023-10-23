# name_generator - a simple name generator for Morrowind

`name_generator` is a command-line tool to generate lore-friendly character
names by using the names of The Elder Scrolls III: Morrowind as reference.

The name generator needs an existing installation of TES III: Morrowind in order
to work. Addons Tribunal and Bloodmoon are recommended to get a broader range of
names, but they are not required.

## Usage

```
name_generator [-d DIRECTORY]

options:
  --help             - displays this help message and quits
  -?                 - same as --help
  --version          - displays the version of the program and quits
  -d DIRECTORY       - set path to the Data Files directory of Morrowind to
                       DIRECTORY. If omitted, the path will be read from the
                       Windows registry or a default value will be used.
  -dir DIRECTORY     - same as -d
  --male             - generate male names
  --female           - generate female names
  --both             - generate both male and female names
                       If none of --male, --female or --both is given, then the
                       program will prompt the user to select one of those
                       options.
```

## Copyright and Licensing

Copyright 2023  Dirk Stolle

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
