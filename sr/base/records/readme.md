# Classes for records in Skyrim's ESM and ESP file format

This directory contains classes for almost all possible records that can occur
in Skyrim's ESM / ESP file format. Some records may contain data whose purpose
is not known.

All of these records have some common methods:

* `bool equals(const FooRecord& other) const;` provides an equality comparison
  between records of the same type (here the fictional type `FooRecord`).
* `virtual bool loadFromStream(std::istream& input, const bool localized, const StringTable& table);`
  allows to load a record from a stream of an ESM / ESP file.
* `virtual bool saveToStream(std::ostream& output) const;` allows to save a
  record to a stream. This method may not always be completely implemented (see
  e. g. `QuestRecord` for an incomplete implementation).
* `virtual uint32_t getRecordType() const;` returns the type of the record as it
  is seen in the record's header in the file stream, e. g. for the struct 
  `ActivatorRecord` it returns `0x49544341` - which translates to the character
  sequence "ACTI".

In general, the constructors of those records create an instance where all data
members are empty. This is not a "valid" state, because there is no "empty"
record in any plugin file (`*.esm` or `*.esp`) for Skyrim. Only after the
`loadFromStream()` method has been called and was successful (i. e. it returned
`true`) all data members have a valid state that constitutes a full record.

Usually all data members of the records are public.
