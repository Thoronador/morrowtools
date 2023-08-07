# Classes for records in Morrowinds's ESM and ESP file format

This directory contains classes for almost all possible records that can occur
in Morrowind's ESM / ESP file format. Some records may contain data whose
purpose is not known.

All of these records have some common methods:

* `bool equals(const FooRecord& other) const;` provides an equality comparison
  between records of the same type (here the fictional type `FooRecord`).
* `virtual bool loadFromStream(std::istream& input);`
  allows to load a record from a stream of an ESM / ESP file.
* `virtual bool saveToStream(std::ostream& output) const;` allows to save a
  record to a stream.

In general, the constructors of those records create an instance where all data
members are empty. This is not a "valid" state, because there is no "empty"
record in any plugin file (`*.esm` or `*.esp`) for Morrowind. Only after the
`loadFromStream()` method has been called and was successful (i. e. it returned
`true`) all data members have a valid state that constitutes a full record.

Usually all data members of the records are public.
