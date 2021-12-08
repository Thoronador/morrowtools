===============================================================================
                          Data Cleaner for Morrowind
                                 by Thoronador
                               (May 30th, 2011)
===============================================================================


Purpose of the programme
========================

The programme Data Cleaner will help to find and delete unused files within the
Data Files directory of Morrowind (currently only meshes and icons). To achieve
that, all present plugins will be scanned for required files, and all other
files within the respective directories will be deleted.

The programme works with every language version of Morrowind.


Versions of the programme
=========================

All explanations in that document refer to the programme version 0.2 as of
December 8th, 2021. Some features of the programme may change in future versions,
or new features might be added, which aren't documented here yet.


Programme call
==============

You can call the programme via command line; the programme itself can be placed
into an arbitrary directory, it does not have to be inside the Morrowind
directory. A typical programme call could look like this:

  data_cleaner.exe -d "D:\Games\Bethesda\Morrowind\Data Files"

The path following behind -d specifies the directory where Morrowind's master
file (and possibly plugin files, too) is located. This should be the Data Files
directory of your Morrowind installation. This parameter is optional, you don't
need to specify it, but the programme will assume some standard directory in
that case (currently that's C:\Program Files\Bethesda Softworks\Morrowind\
Data Files), so that this might not work on every system or will produce un-
desired results.


Parameters
==========

The following section lists all possible programme parameters for Data Cleaner.
All parameters are optional, that means you don't have to specify them. However,
missing parameters could lead to the programme not being executed properly and/
or cancelling before it's done. That's why you should at least specify the path
of the Data Files directory of Morrowind by using the parameter -d.

--data-files DIRECTORY   - sets the directory DIRECTORY as Data Files directory
                           of Morrowind. This directory has to contain
                           Morrowind.esm and all other plugin files, which
                           should be scanned. If that directory does not exist,
                           the programme gets cancelled prematurely.
-d DIRECTORY             - short for --data-files
--help                   - displays a help text about the programme's parameters
                           and exits. All other parameters will be discarded and
                           no unused files will be deleted.
-?                       - short for --help
--version                - displays the version of the programme and exits. All
                           other parameters will be discarded and no unused
                           files will be deleted.
--all                    - tries to load all master and plugin files from the
                           given Data Files directory and scans them for
                           required files (default).
--all-data-files         - same as --all
--explicit               - sets explicit mode where every data file that has to
                           be scanned has to be given explicitly. (See the
                           parameters --add-file and --ini for that.) This option is
                           mutually exclusive with --all.
-e                       - short for --explicit
--add-file Plugin.esp    - adds the plugin Plugin.esp to the list of files that
                           will be searched for data files. Files required by
                           Plugin.esp won't be deleted. This parameter can be
                           repeated to add more than one plugin file. This
                           parameter is only allowed in explicit mode.
-f Plugin.esp            - short for --add-file
--ini                    - gets the plugin files from the [Game Files] section
                           of Morrowind.ini and adds those files to the list of
                           plugins. If there is no Morrowind.ini (that should
                           not happen with a normal Morrowind installation), the
                           programme will stop and will not delete any unused
                           files. This parameter is only allowed in explicit
                           mode.
-i                       - short for --ini


Licence, disclaimer and source code
===================================

The programme data_cleaner.exe is released under the GNU General Public Licence 3,
a free software licence. For the full text of the licence consult the file GPL.txt
or view it online at http://www.gnu.org/licenses/gpl-3.0.html

This programme is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

The programme's source code is published at Sourceforge.net, the project itself
is located at http://sourceforge.net/projects/morrowtools/


Credits
=======

Thank you, Bethesda Softworks for The Elder Scrolls III: Morrowind.
Without this great game this programme wouldn't exist, too.
Special thanks to Dave Humphrey of UESP.net for his information about the format
of Morrowind's ESM file format. It was a great help while creating this pro-
gramme. If you're interested in it, you can find that information at  http://www.uesp.net/morrow/tech/mw_esm.txt.
