# Code for handling of Skyrim's BSA file format

This directory contains classes that handle Skyrim's archive file format: BSA.
Currently (as of 2021-10-31), the code only has been tested with version 104 of
the file format. That particular version is used in the original Skyrim of 2011.

Skyrim Special Edition uses version 105, and the code may or may not work with
that version.

TES IV: Oblivion uses the version 103. The code has not been tested with that
version, so it will probably not work with BSA files from Oblivion.

# BSA version support

The code has only been testes with version 104 archives so far.

| BSA version | Used in                  | Version supported |
| ----------- | ------------------------ | ----------------- |
| 103         | TES IV: Oblivion         | no (but may work) |
| 104         | 2011's Skyrim ("Oldrim") | __yes__           |
| 105         | Skyrim Special Edition   | no                |
