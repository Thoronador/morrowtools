/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021, 2022  Dirk Stolle

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
 -------------------------------------------------------------------------------
*/

#ifndef MW_AIPACKAGES_HPP
#define MW_AIPACKAGES_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <string>

namespace MWTP
{

/** enumeration for possible types of AI packages */
enum class PackageType { ptActivate, ptEscort, ptFollow, ptTravel, ptWander };

// pure virtual base class / interface for real AI packages like NPC_AIWander or NPC_AITravel
struct NPC_BasicAIPackage
{
  virtual ~NPC_BasicAIPackage();

  /** \brief Gets the type of the package.
   *
   * \return  Returns an enumeration value indicating the package type.
   * \remarks Each derived class has to implement its own version of that
   *          method.
   */
  virtual PackageType getPackageType() const = 0;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the instances's data would occupy in a
   *         stream.
   *
   * \return Returns the size in bytes that the instance's data needs.
   */
  virtual uint32_t getStreamSize() const = 0;

  /** \brief Writes the package to the given output stream.
   *
   * \param output  the output stream
   * \return  Returns true on success (package was written to stream).
   *          Returns false, if an error occurred.
   */
  virtual bool saveToStream(std::ostream& output) const = 0;
  #endif
}; // struct

/** Holds the AI activate data of an NPC or creature. */
struct NPC_AIActivate: public NPC_BasicAIPackage
{
  std::string TargetID;
  int8_t Reset;

  NPC_AIActivate();

  /** \brief Checks whether another package contains the same data.
   *
   * \param other   the other package to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const NPC_AIActivate& other) const;

  /** \brief Gets the type of the package.
   *
   * \return  Returns an enumeration value indicating the package type.
   */
  PackageType getPackageType() const override;

  /** \brief Loads the package from the given input stream.
   *
   * \param input   the input stream
   * \param buffer  a pre-allocated array of char that can hold at least 33 bytes
   * \param bytesRead  the variable that holds the number of bytes read so far
   * \return Returns true on success (package was loaded from stream).
   *         Returns false, if an error occurred.
   */
  bool loadFromStream(std::istream& input, char * buffer, uint32_t& bytesRead);

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the instances's data would occupy in a
   *         stream.
   *
   * \return Returns the size in bytes that the instance's data needs.
   */
  uint32_t getStreamSize() const override;

  /** \brief Writes the package to the given output stream.
   *
   * \param output  the output stream
   * \return  Returns true on success (package was written to stream).
   *          Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif
}; // struct

/** Holds the escort/follow data of an NPC or creature.
 *  (Both escort and follow have the same data members, so we can use one
 *   structure for both.)
 */
struct NPC_AIEscortFollow: public NPC_BasicAIPackage
{
  float X, Y, Z;
  int16_t Duration;
  std::string TargetID;
  int16_t Reset;
  std::string CellName;

  NPC_AIEscortFollow();

  /** Sets all members to zero. */
  void clear();

  /** \brief Checks whether another package contains the same data.
   *
   * \param other   the other package to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const NPC_AIEscortFollow& other) const;

  /** \brief Loads the package (excluding cell name) from the given input stream.
   *
   * \param input   the input stream
   * \param buffer  a pre-allocated array of char that can hold at least 33 bytes
   * \param bytesRead  the variable that holds the number of bytes read so far
   * \return Returns true on success (package was loaded from stream).
   *         Returns false, if an error occurred.
   * \remarks The CellName is NOT loaded by calling this method, even if there
   *          is one.
   */
  bool loadFromStream(std::istream& input, char * buffer, uint32_t& bytesRead);

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the instances's data would occupy in a
   *         stream.
   *
   * \return Returns the size in bytes that the instance's data needs.
   */
  uint32_t getStreamSize() const override;

  /** \brief Writes the package to the given output stream.
   *
   * \param output  the output stream
   * \return  Returns true on success (package was written to stream).
   *          Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif
}; // struct

struct NPC_AIEscort: public NPC_AIEscortFollow
{
  /** \brief Gets the type of the package.
   *
   * \return  Returns an enumeration value indicating the package type.
   */
  PackageType getPackageType() const override;
};

struct NPC_AIFollow: public NPC_AIEscortFollow
{
  /** \brief Gets the type of the package.
   *
   * \return  Returns an enumeration value indicating the package type.
   */
  PackageType getPackageType() const override;
};

/** Hold the AI travel data of an NPC or creature. */
struct NPC_AITravel: public NPC_BasicAIPackage
{
  float X, Y, Z;
  int32_t Reset;

  NPC_AITravel();

  /** Sets all members to zero. */
  void clear();

  /** \brief Checks whether another package contains the same data.
   *
   * \param other   the other package to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const NPC_AITravel& other) const;

  /** \brief Gets the type of the package.
   *
   * \return  Returns an enumeration value indicating the package type.
   */
  PackageType getPackageType() const override;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the instances's data would occupy in a
   *         stream.
   *
   * \return Returns the size in bytes that the instance's data needs.
   */
  uint32_t getStreamSize() const override;

  /** \brief Writes the package to the given output stream.
   *
   * \param output  the output stream
   * \return  Returns true on success (package was written to stream).
   *          Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif
}; // struct

/** Holds the AI wandering data of an NPC or creature. */
struct NPC_AIWander: public NPC_BasicAIPackage
{
  int16_t Distance;
  int16_t Duration;
  int8_t Time;
  std::array<uint8_t, 8> Idle;
  uint8_t Reset;

  NPC_AIWander();

  /** Sets all members to zero. */
  void clear();

  /** \brief Checks whether another package contains the same data.
   *
   * \param other   the other package to compare with
   * \return Returns true, if @other contains the same data as this instance.
   *         Returns false otherwise.
   */
  bool equals(const NPC_AIWander& other) const;

  /** \brief Gets the type of the package.
   *
   * \return  Returns an enumeration value indicating the package type.
   */
  PackageType getPackageType() const override;

  #ifndef MW_UNSAVEABLE_RECORDS
  /** \brief Gets the size in bytes that the instances's data would occupy in a
   *         stream.
   *
   * \return Returns the size in bytes that the instance's data needs.
   */
  uint32_t getStreamSize() const override;

  /** \brief Writes the package to the given output stream.
   *
   * \param output  the output stream
   * \return  Returns true on success (package was written to stream).
   *          Returns false, if an error occurred.
   */
  bool saveToStream(std::ostream& output) const override;
  #endif
}; // struct

} // namespace

#endif // MW_AIPACKAGES_HPP
