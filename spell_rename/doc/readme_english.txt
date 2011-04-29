===============================================================================
                          Spell Renamer for Morrowind
                                 by Thoronador
                               (April 30th, 2011)
===============================================================================


Purpose of the programme
========================

The programme Spell Renamer can help to sort the spells in Morrowind's spell
menu according to their spell schools by creating a plugin file that changes the
spell names. Every spell's name will be changed by adding the first letter of
the spell's school to it - that is A for Alteration, C for Conjuration, D for
Destruction, I for Illusion, M for Mysticism, and R for Restoration spells.

This creates spell names like in the following examples:

A Water Walking
C Bound Longsword
D Fireball
I Invisibility
M Almsivi Intervention

The programme also works with the German language version of Morrowind. There
the spells will get the corresponding letters of the German names of the spell
schools (B for Beschwörung, I for Illusion, M for Mystik, V for Veränderung,
W for Wiederherstellung, Z for Zerstörung). Basically the programme should work
with every language version of Morrowind and adjust the letters according to the
language used in the master and plugin files, because it reads the appropriate
GMST entries. However, the programme has only been tested with the English and
German version yet.


More patterns
=============

Future versions of the programme will probably have more patterns or combina-
tions of letters and numbers that can be put before the spell's name. However,
I haven't been creative enough to think of some more, useful patterns, yet. If
you have an idea for that, tell me about it and maybe that pattern will soon be
implemented in a future version of the programme. But keep in mind that those
patterns should not exceed three or four characters, because longer patterns
might possibly cause problems with (too) long spell names.


Versions of the programme
=========================

All explanations in that document refer to the programme version 0.1_rev219 as
of April 30th, 2011. Some features of the programme may change in future
versions, or new features might be added, which aren't documented here yet.


Programme call
==============

You can call the programme via command line; the programme itself can be placed
into an arbitrary directory, it does not have to be inside the Morrowind
directory. A typical programme call could look like this:

  spell_rename.exe -d "D:\Games\Bethesda\Morrowind\Data Files" -o "New File.esp"

You might have noticed the two path/ file names within the parameters. The path
following behind -d specifies the directory where Morrowind's master file (and
possibly plugin files, too) is located. This should be the Data Files directory
of your Morrowind installation. The name after -o is the name of the new plugin
file that will be created by the programme. That file will be placed into the
specified Data Files directory, so make sure you have write permission for that
directory. Both parameters are optional, you don't need to specify them, but the
programme will assume some standard directory in that case (currently that's
C:\Program Files\Bethesda Softworks\Morrowind\Data Files) and a default file
name (out.esp) will be used, so that this might not work on every system or will
produce undesired results.

If one wants an additional plugin, e.g. a spell plugin, to be considered by the
programme, too, then you can add it after the -f parameter:

  spell_rename.exe -d "D:\Games\Bethesda\Morrowind\Data Files" -o "New File.esp" -f "Spell Plugin.esp"

You can repeat that multiple times for further plugins.
The following would be valid, too:

  spell_rename.exe -d "D:\Games\Bethesda\Morrowind\Data Files" -o "New File.esp" -f "Spell Plugin.esp" -f "another plugin.esp"
        -f "ManyGreatSpells.esp"

Of course, the specified plugins have to exist, too.
If one of the plugins cannot be found, the programme will get cancelled.


Parameters
==========

The following section lists all possible programme parameters for Spell Renamer.
All parameters are optional, that means you don't have to specify them. However,
missing parameters could lead to the programme not being executed properly and/
or cancelling before the creation of the plugin. That's why you should at least
specify the path of the Data Files directory of Morrowind by using the
parameter -d.


--data-files DIRECTORY   - sets the directory DIRECTORY as Data Files directory
                           of Morrowind. This directory has to contain
                           Morrowind.esm and all other plugin files, which
                           should be processed. If that directory does not exist,
                           the programme gets cancelled and no plugin file will be created.
-d DIRECTORY             - short for --data-files
--output NewPlugin.esp   - sets NewPlugin.esp as name for the new, to be crea-
                           ted plugin file. If this parameter is not specified,
                           the output file will be called just out.esp.
-o NewPlugin.esp         - short for --output
--add-files Plugin.esp   - adds the file Plugin.esp to the list of processed
                           plugins. Spells from Plugin.esp will be prefixed with
                           with the letter of their school, too. This parameter
                           can be repeated to add more than one plugin file.
-f Plugin.esp            - short for --add-files
--ini                    - gets the plugin files from the [Game Files] section
                           of Morrowind.ini and adds those files to the list of
                           plugins. If there is no Morrowind.ini (that should
                           not happen with a normal Morrowind installation), the
                           programme will stop and will not create a plugin
                           file.
-i                       - short for --ini
--ini-with-discard       - like --ini, but all plugin files that might have been
                           added via the --add-files or -f parameters won't get
                           scanned for spells.
--allow-truncate         - In order to avoid errors during loading of the crea-
                           ted plugin file, the programme usually does not
                           change the name of spells that get too long for
                           Morrowind. (Names will get longer due to the added
                           prefix for the sorting order.) However, this can re-
                           sult in some spells not being sorted in the manner
                           you expect them to be sorted. If you want to sort
                           those spells, too, then specify this parameter. But
                           be aware that this can result in error messages
                           during the loading process of the plugin and/or names
                           that are just cut off at the end (i.e. they might be
                           missing the last few letters). You have been warned.
--verbose                - shows some information about the scanned plugin files
--silent                 - opposite of --verbose; does not show that information
--help                   - displays a help text about the programme's parameters
                           and exits. All other parameters will be discarded and
                           no plugin file is created.
-?                       - short for --help
--version                - displays the version of the programme and exits. All
                           other parameters will be discarded and no plugin file
                           is created.


Licence and source code
=======================

The programme spell_renamer.exe is released under the GNU General Public Licence 3,
a free software licence. For the full text of the licence consult the file GPL.txt
or view it online at http://www.gnu.org/licenses/gpl-3.0.html

The programme's source code is published at Sourceforge.net, the project itself
is located at http://sourceforge.net/projects/morrowtools/


Credits
=======

Thank you, Bethesda Softworks for The Elder Scrolls III: Morrowind.
Without this great game this programme wouldn't exist, too.
Special thanks to Dave Humphrey of UESP.net for his information about the format
of Morrowind's ESM file format. It was a great help while creating this pro-
gramme. If you're interested in it, you can find that information at  http://www.uesp.net/morrow/tech/mw_esm.txt.
