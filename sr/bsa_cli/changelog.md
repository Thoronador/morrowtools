# Version history of bsa-cli

## Version 0.3.0 (2021-10-27)

The new `commands` operation is added. It shows available operations / commands.

## Version 0.2.1 (2021-10-25)

The output of the `list` operation has been changed to include the path directly
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

The new operation `info` is added. It shows the header information of the BSA
file.

## Version 0.1.0 (2021-10-22)

This is the initial version of `bsa-cli`. It only supports listing the names of
files and directories in a BSA file via the `list` operation.
