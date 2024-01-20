# bsafs - a FUSE file system for BSA files

`bsafs` is a command-line tool to mount BSA files, an archive format that is
used by The Elder Scrolls V: Skyrim (and some other games), into the file
system, making those archives browsable via normal file managers and similar
tools. It may also work with BSA files of The Elder Scrolls IV: Oblivion, but
this has not been tested yet. It will certainly _not_ work with BSA files of
The Elder Scrolls III: Morrowind, because Morrowind uses a different BSA format,
despite using the same file extension.

While this tool technically does not need a working installation of The Elder
Scrolls V: Skyrim, it needs BSA files to operate on, and those usually come with
Skyrim (and also with some other games).

## Usage

```
bsafs [OPTIONS] --archive BSA_FILE MOUNT_POINT

options:
  --help             - Displays this help message and quits.
  -?                 - same as --help
  --version          - Displays the version of the program and quits.
  -v                 - same as --version
  --archive BSA_FILE - Set path to the BSA file to operate on to BSA_FILE.
                       The BSA_FILE must be given.

Furthermore, some FUSE-specific parameters apply.
```

## History of changes

A changelog is available in [changelog.md](./changelog.md).

## Third-party libraries

Used third-party libraries and their copyright notices are listed in
[third-party.md](./third-party.md).

## Copyright and Licensing

Copyright 2024  Dirk Stolle

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
