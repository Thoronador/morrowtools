# formID_finder - a command-line interface to search for form IDs in Skyrim

`formID_finder` is a command-line tool to search for
[form IDs](https://en.uesp.net/wiki/Skyrim:Form_ID) in TES V: Skyrim's game
data, more specifically its ESM files. Form IDs are a kind of identifier used
for everything in the game. They are mainly used in console commands on PC,
e. g. as a workaround to fix broken quests or similar bugs.

This tool needs a working installation of The Elder Scrolls V: Skyrim. Any of
the available editions for PC - the original Skyrim, Skyrim Legendary Edition,
Skyrim Special Edition or Skyrim Anniversary Edition - will do.

The search is performed based on the name of objects in the game. This means
that a player using an English language version of Skyrim might get different
search results than another player performing the same search using the German
or French language version of Skyrim.

## Usage

```
formID_finder [-d DIRECTORY] -p PATTERN

options:
  --help           - displays this help message and quits
  -?               - same as --help
  --version        - displays the version of the program and quits
  -v               - same as --version
  -d DIRECTORY     - set path to the Data Files directory of Skyrim to
                     DIRECTORY
  -dir DIRECTORY   - same as -d
  -p TEXT          - set the search pattern to be TEXT.
  --keyword TEXT   - same as -p
  --case-sensitive - use case-sensitive search, i.e. upper and lower case
                     version of the same letter won't match. Deactivated by
                     default.
  --all-quest-info - shows complete quest info, including texts for journal
                     entries and objectives.
  --faction-ranks  - shows the ranks of matching factions, too.
  --ranks          - same as --faction-ranks
  --ref-id         - try to find reference IDs, too. With this parameter the
                     program will need a significantly longer amount of time
                     to complete a search.
  --skyrim-se      - assume that Skyrim Special Edition is installed and use
                     that installation.
  --oldrim         - assume that the old Skyrim of 2011 is installed and use
                     that installation.
  --english | --en - set the language to load to English. This only triggers,
                     if there are no lose string table files and loading from
                     the BSA file takes place. Same holds for other language
                     options.  Default is German, if no language is set.
  --french | --fr  - set the language to load to French.
  --german | --de  - set the language to load to German.
  --italian | --it - set the language to load to Italian.
  --polish | --pl  - set the language to load to Polish.
  --russian | --ru - set the language to load to Russian.
  --spanish | --es - set the language to load to Spanish.
```

## History of changes

A changelog is provided as [separate file](./ChangeLog.en.md).

## Copyright and Licensing

Copyright 2011-2021  Dirk Stolle

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
