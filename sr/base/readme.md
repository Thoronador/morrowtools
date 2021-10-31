# General Skyrim-related classes

The `base/` directory contains various classes and functions that are general
enough to be (potentially) useful in any Skyrim-related tool. This does not mean
that every Skyrim-related program uses all of them, but most certainly some of
them.

## Subdirectories

* **bsa**: contains code related to the BSA archive format
* **records**: contains classes for almost all possible records that can occur
  in Skyrim's ESM / ESP file format

## Contents

Most files in this directory contain just `typedef`s (aliases) for the
respective management class template, e. g. `Activators.hpp` defines an alias
for `MapBasedRecordManager` using the `ActivatorRecord` class.

However, there are some notable exceptions:

* `DependencySolver.hpp` + `DependencySolver.hpp`: methods to determine the load
  order of ESM files (think of a dependency tree)
* `ESMReader.hpp` + `ESMReader.cpp` contain the `ESMReader` class which
  provides the basic mechanism to load an ESM or ESP file of Skyrim. Note that
  in most cases the class needs to be subclassed to read only the the desired
  data from those files.
* `ESMWriter.hpp` + `ESMWriter.cpp` contain the `ESMWriter` class which is the
  counterpart to `ESMReader` and allows to write ESM / ESP files.
* `FormIDFunctions.hpp` + `FormIDFunctions.cpp` contain functions that work with
  form IDs.
* `MapBasedRecordManager.hpp` contains a templated class that can manage all
  records of a single type.
* `PathFunctions.hpp` contains functions that deal with Skyrim-related paths
  (e. g. the path to the `Data/` directory).
* `RegistryFunctions.hpp` provides a way to get Skyrim's installation directory
  from the Windows registry. Obviously, that will only work on Windows systems.
* `StringTable.hpp` + `StringTable.cpp` contain the `StringTable` class which
  handles files containing localized string data (e. g. `*.dlstrings`,
  `*.ilstrings` and `*.strings` files).
