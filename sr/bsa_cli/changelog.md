# Version history of bsa-cli

## Next version (2022-07-??)

The new `file-metadata` command to show metadata of the files in a BSA file is
added. Metadata contains the name hash, the file size, the file offset, the
compression flag and the file name.

The new `directory-metadata` command to show metadata of the directories in a
BSA file is added. Metadata contains the name hash, the number of files in the
directory, the file offset, and the directory name.

## Version 0.7.2 (2022-06-19)

__[feature]__
The `info` command will now show whether the archive's flag for embedded file
names is set.

_(Note that `bsa-cli` is not yet able to extract archive files when the embedded
file name flag is set. This feature may become available in a later version of
`bsa-cli`.)_

## Version 0.7.1 (2021-11-08)

__[bugfix]__
The error message that is displayed when no recognized command is given did not
include all the possible commands. It does now.

## Version 0.7.0 (2021-11-02)

__[bugfix]__
A bug with the extraction of compressed files where only parts of the
decompressed data was written to the file has been fixed. Extracted compressed
files will now have the correct size.

__[feature]__
The `info` command will now show more information about the archive's flags.
This includes two XBox-related flags as well as information about the content
types of the files in the archive. Note that the later is only a rough
indication for the actual contents.

__[feature]__
The Linux version of `bsa-cli` does now support extraction of files that were
compressed with lz4. That usually occurs in version 105 of the BSA format.
_(This feature may or may not become available on Windows in later versions of
`bsa-cli`.)_

__[feature]__
`bsa-cli`'s version information does now show the versions of the used
(de-)compression libraries, too.

## Version 0.6.1 (2021-10-31)

`bsa-cli` can now also handle the newer archive version 105 which is used in
Skyrim Special Edition.

## Version 0.6.0 (2021-10-31)

The new `extract-all` command to extract all content from a BSA file is added.

## Version 0.5.0 (2021-10-31)

The new `folders` command to list all folders inside a BSA file is added.

The new `extract-folder` command to extract all files from a folder of a BSA
file is added.

## Version 0.4.1 (2021-10-30)

A new `help` command is added to provide a bit more detailed help for the
various other commands.

## Version 0.4.0 (2021-10-29)

The new `extract-file` command to extract a single file from a BSA file is
added.

## Version 0.3.0 (2021-10-27)

The new `commands` command is added. It shows available commands.

## Version 0.2.1 (2021-10-25)

The output of the `list` command has been changed to include the path directly
before the file name. Instead of showing the files a.dds, b.png and c.txt in a
directory foo inside the BSA file as

```
foo
    a.dds
    b.png
    c.txt
```

the listing for the same files will now be shown as

```
foo\a.dds
foo\b.png
foo\c.txt
```

instead. That way the full path can be seen at a glance, and processing the
output is also easier in many cases.

## Version 0.2.0 (2021-10-22)

The new command `info` is added. It shows the header information of the BSA
file.

## Version 0.1.0 (2021-10-22)

This is the initial version of `bsa-cli`. It only supports listing the names of
files and directories in a BSA file via the `list` command.
