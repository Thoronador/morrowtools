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

`bsafs` only allows reading from the files in an archive but not writing to
them. In that way it behaves like a read-only file system. This means that files
in `bsafs` cannot be deleted, moved / renamed or written to by the user. It also
does not allow the creation of new files in the archive.

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

## Quick start

_Note: This section assumes that the `bsafs` executable is reachable via
`$PATH`, i. e. it can be invoked directly as `bsafs` without the need to specify
the full path to it._

First, you have to create a new directory where you want to mount the BSA file.
In this example we are going to use `/tmp/my_bsa` as mount point:

```sh
mkdir /tmp/my_bsa
```

Next, a BSA file is needed. Let's assume the file is located at
`/opt/games/some-plugin/archive.bsa` on your machine. Then it can be mounted to
the previously created mount point via:

```
bsafs --archive /opt/games/some-plugin/archive.bsa /tmp/my_bsa
```

After that, the files that are in the archive can be seen in `/tmp/my_bsa`.
You can now browse those files in the usual fashion, but remember that they are
read-only, so no modifications are possible.

Finally, if you do not need it anymore, you can unmount the archive:

```
umount /tmp/my_bsa
```

That's it. Have fun.

## Use cases

### When to use `bsafs`

Typical use cases of `bsafs` include:

* You want to browse the contents of a BSA file without the need to extract
  those contents manually.
* You want to inspect the contents of files within a BSA file and do not need
  to modify those files.

### When not to use `bsafs`

While `bsafs` is a good way to inspect BSA files interactively, it may not fit
all use cases. Among such use cases are:

* You want to modify the content of a BSA file. Since `bsafs` only provides
  read-only access, it is obviously not fit for that purpose.
* You want to inspect most or all of the files in a BSA archive. While this is
  possible with `bsafs`, it may be a bit slow, because it extracts any file
  from which users read in the background. In that case it is more effective to
  use a tool like `bsa_cli` and just extract all the content of the BSA file
  once and work on that extracted content directly.
* You mainly want to read from a very large file inside a BSA file. The reason
  is the same as for the previous bullet point: background extraction of files.
  It works, but it will probably be a bit slow. So the obvious workaround here
  is again to use a tool like `bsa_cli` instead and just extract the large file
  from the BSA and then directly work on the extracted file.

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
