/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2021  Dirk Stolle

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

#include "ByteBuffer.hpp"
#include <cstring>

namespace MWTP
{

ByteBuffer::ByteBuffer()
: mData(nullptr),
  mSize(0)
{
}

ByteBuffer::ByteBuffer(const ByteBuffer& other)
: mData(nullptr),
  mSize(other.mSize)
{
  if (mSize != 0)
  {
    mData = new uint8_t[mSize];
    memcpy(mData, other.mData, mSize);
  }
}
ByteBuffer::ByteBuffer(ByteBuffer&& other)
: mData(other.mData),
  mSize(other.size())
{
  other.mData = nullptr;
  other.mSize = 0;
}

ByteBuffer& ByteBuffer::operator=(const ByteBuffer& other)
{
  // avoid self-assignment
  if (this == &other)
  {
    return *this;
  }

  copy_from(other.data(), other.size());
  return *this;
}

ByteBuffer::~ByteBuffer()
{
  delete[] mData;
  mData = nullptr;
}

bool ByteBuffer::operator==(const ByteBuffer& op) const
{
  return (mSize == op.size()) && ((mSize == 0) || (memcmp(mData, op.data(), mSize) == 0));
}

const uint8_t* ByteBuffer::data() const
{
  return mData;
}

std::size_t ByteBuffer::size() const
{
  return mSize;
}

void ByteBuffer::copy_from(const uint8_t* buffer, const std::size_t buffer_size)
{
  reset();
  if ((buffer == nullptr) || (buffer_size == 0))
  {
    return;
  }

  mData = new uint8_t[buffer_size];
  memcpy(mData, buffer, buffer_size);
  mSize = buffer_size;
}

void ByteBuffer::reset()
{
  delete[] mData;
  mData = nullptr;
  mSize = 0;
}

} // namespace
