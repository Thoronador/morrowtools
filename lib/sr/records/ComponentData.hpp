/*
 -------------------------------------------------------------------------------
    This file is part of the Skyrim Tools Project.
    Copyright (C) 2011, 2021  Thoronador

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

#ifndef SR_COMPONENTDATA_HPP
#define SR_COMPONENTDATA_HPP

#include <cstdint>

namespace SRTP
{

/** Holds information about an object and its count.
 *
 * \remarks This data can be part of e. g. an inventory or a recipe. In the
 *          inventory case it denotes how often an object is in the inventory.
 *          In the recipe case it denotes how much of that component is required
 *          to craft the resulting object.
 */
struct ComponentData
{
  uint32_t formID; /**< the form ID of the referenced component */
  uint32_t count;  /**< how often that component occurs / is required */

  /** Constructs a new object with its data members set to zero. */
  ComponentData();

  /** equality operator */
  bool operator==(const ComponentData& other) const;
}; // struct

} // namespace

#endif // SR_COMPONENTDATA_HPP
