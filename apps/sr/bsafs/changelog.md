# Version history of bsafs

## Version 0.3.0 (2024-01-31)

bsafs does now use version 3 instead of version 2 of the FUSE library.

## Version 0.2.0 (2024-01-23)

Files from archives can now be read. The required decompression happens on the
fly.

This is the first version of `bsafs` which is feature-complete by providing a
full implementation of a read-only file system for Skyrim BSA files.

## Version 0.1.2 (2024-01-20)

Incorrect formatting in the binary's help message is fixed.

## Version 0.1.1 (2024-01-20)

* The block size of a file is now reported correctly instead of always being
  zero.
* Furthermore, the time for last access, last modification and last status
  change of files and directories in the archive are now set to the respective
  value of the archive file instead of just reporting the current time.

## Version 0.1.0 (2024-01-20)

This is the initial version of `bsafs`.
