/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "ESMWriterGeneric.h"
#include <iostream>

namespace MWTP
{

ESMWriterGeneric::ESMWriterGeneric(VectorType* vec)
{
  if (NULL==vec)
  {
    std::cout << "ESMWriterGeneric: Error: supplied pointer is NULL!\n";
    throw 42;
  }
  m_VectorPointer = vec;
}

ESMWriterGeneric::~ESMWriterGeneric()
{
  //empty
}

int32_t ESMWriterGeneric::getTotalRecords() const
{
  return m_VectorPointer->size();
}

bool ESMWriterGeneric::writeRecords(std::ofstream& output) const
{
  const VectorType::const_iterator end_iter = m_VectorPointer->end();
  VectorType::const_iterator iter = m_VectorPointer->begin();
  while (iter!=end_iter)
  {
    if (*iter!=NULL)
    {
      if (!((*iter)->saveToStream(output)))
        return false;
    }//if not NULL
    ++iter;
  }//while
  return true;
}

} //namespace