/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2014, 2024  Dirk Stolle

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

#include "ESMWriterGeneric.hpp"
#include <iostream>
#include <stdexcept>

namespace MWTP
{

ESMWriterGeneric::ESMWriterGeneric(VectorType* vec)
: ESMWriter(),
  m_VectorPointer(vec)
{
  if (nullptr == vec)
  {
    std::cerr << "ESMWriterGeneric: Error: supplied pointer is NULL!\n";
    throw std::runtime_error("ESMWriterGeneric: Error: supplied pointer is NULL!");
  }
}

int32_t ESMWriterGeneric::getTotalRecords() const
{
  return m_VectorPointer->size();
}

bool ESMWriterGeneric::writeRecords(std::ostream& output) const
{
  const VectorType::const_iterator end_iter = m_VectorPointer->end();
  VectorType::const_iterator iter = m_VectorPointer->begin();
  while (iter != end_iter)
  {
    if (*iter != nullptr)
    {
      if (!((*iter)->saveToStream(output)))
        return false;
    }
    ++iter;
  }
  return true;
}

} // namespace
