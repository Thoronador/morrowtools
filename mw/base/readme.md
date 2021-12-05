# General Morrowind-related classes

The `base/` directory contains various classes and functions that are general
enough to be (potentially) useful in any Morrowind-related tool. This does not
mean that every Morrowind-related program uses all of them, but most certainly
some of them.

## Subdirectories

* **records**: contains classes for almost all possible records that can occur
  in Morrowind's ESM / ESP file format
* **script_compiler**: contains classes for an _experimental_ "compiler" that
  can compile the text of Morrowind's scripts into the corresponding binary
  representation / byte code

## Contents

Most files in this directory contain just `typedef`s (aliases) for the
respective management class template, e. g. `Activators.hpp` defines an alias
for `MapBasedRecordManager` using the `ActivatorRecord` class.

However, there are some notable exceptions:

* `ESMReader.hpp` + `ESMReader.cpp` contain the `ESMReader` class which
  provides the basic mechanism to load an ESM or ESP file of Morrowind. Note
  that in most cases the class needs to be subclassed to read only the the
  desired data from those files.
* `ESMWriter.hpp` + `ESMWriter.cpp` contain the `ESMWriter` class which is the
  counterpart to `ESMReader` and allows to write ESM / ESP files.
* `IniFunctions.hpp` contains functions that can extract data from the
  `Morrowind.ini`.
* `MapBasedRecordManager.hpp` contains a templated class that can manage all
  records of a single type.
* `RegistryFunctions.hpp` provides a way to get Morrowind's installation
  directory from the Windows registry. Obviously, that will only work on Windows
  systems.
* `SetBasedRecordManager.hpp` contains a templated class that can manage all
  records of a single type. Works like `MapBasedRecordManager` but used a
  `std::set` internally to manage the data.
