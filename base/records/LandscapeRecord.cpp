/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2009, 2011 Thoronador

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

#include "LandscapeRecord.h"
#include <iostream>
#include "../MW_Constants.h"
#include "../HelperIO.h"

namespace MWTP
{

LandscapeRecord::LandscapeRecord()
{
  CellX = CellY = 0;
  UnknownData = 0;
  hasNormals = false;
  memset(&(Normals[0][0][0]), '\0', 65*65*3);
  hasHeightSubrecord = false;
  HeightOffset = 0.0f;
  UnknownByte = 0;
  memset(&(HeightData[0][0]), 0, 65*65);
  UnknownShort = 0;
  hasWNAMSubrecord = false;
  memset(Data, 0, 81);
  hasVertexColour = false;
  memset(&(VertexColour[0][0][0]), '\0', 65*65*3);
  hasVertexTextures = false;
  memset (&(VertexTextures[0][0]), 0, 16*16*2);
}

bool LandscapeRecord::equals(const LandscapeRecord& other) const
{
  if ((CellX==other.CellX) and (CellY==other.CellY) and (UnknownData==other.UnknownData)
    and (hasNormals==other.hasNormals) and (hasHeightSubrecord==other.hasHeightSubrecord)
    and (hasWNAMSubrecord==other.hasWNAMSubrecord) and (hasVertexColour==other.hasVertexColour)
    and (hasVertexTextures==other.hasVertexTextures))
  {
    //check other stuff
    // ---- normals
    //if (hasNormals xor other.hasNormals)
    //  return false; //one is present and one is not - return
    if (hasNormals)
    {
      if (memcmp(&(Normals[0][0][0]), &(other.Normals[0][0][0]), 65*65*3)!=0)
        return false;
    }
    // ---- height data
    if (hasHeightSubrecord)
    {
      if ((HeightOffset!=other.HeightOffset) or (UnknownByte!=other.UnknownByte)
        or (UnknownShort!=other.UnknownShort))
        return false;
      if (memcmp(&(HeightData[0][0]), &(other.HeightData[0][0]), 65*65)!=0)
        return false;
    }
    // ---- unknown data
    if (hasWNAMSubrecord)
    {
      if (memcmp(Data, other.Data, 81)!=0)
        return false;
    }
    //vertex colour
    if (hasVertexColour)
    {
      if (memcmp(&(VertexColour[0][0][0]), &(other.VertexColour[0][0][0]), 65*65*3)!=0)
        return false;
    }//if

    //vertex textures
    if (hasVertexTextures)
    {
      if (memcmp(&(VertexTextures[0][0]), &(other.VertexTextures[0][0]), 16*16*2)!=0)
        return false;
    }//if
    //all seems to be the same
    return true;
  }
  return false;
}

bool LandscapeRecord::saveToStream(std::ofstream& output) const
{
  output.write((char*) &cLAND, 4);
  uint32_t Size;
  Size = 4 /* INTV */ +4 /* 4 bytes for length */ +8 /* size of INTV */
        +4 /* DATA */ +4 /* 4 bytes for length */ +4 /* size of DATA */;
  if (hasHeightSubrecord)
  {
    Size = Size +4 /* VHGT */ +4 /* 4 bytes for length */ +4232 /* size of VHGT */;
  }
  if (hasWNAMSubrecord)
  {
    Size = Size +4 /* WNAM */ +4 /* 4 bytes for length */ +81 /* size of WNAM */;
  }
  if (hasNormals)
  {
    Size = Size + 4 /* VNML */ +4 /* 4 bytes for length */ +12675 /* size of VNML */;
  }

  if (hasVertexColour)
  {
    Size = Size + 4 /* VCLR */ +4 /* 4 bytes for length */ +12675 /* size of VCLR */;
  }
  if (hasVertexTextures)
  {
    Size = Size + 4 /* VTEX */ +4 /* 4 bytes for length */ +512 /* size of VTEX */;
  }
  output.write((char*) &Size, 4);
  output.write((char*) &HeaderOne, 4);
  output.write((char*) &HeaderFlags, 4);
  /*Landscape:
    INTV (8 bytes)
        long CellX }
        long CellY } The cell coordinates of the cell.
    DATA (4 bytes)
        long Unknown (default of 0x09)
    VNML (12675 bytes)
        struct {
          signed byte X
          signed byte Y
          signed byte Z
        } normals[65][65];
            A RGB color map 65x65 pixels in size representing the land normal vectors.
            The signed value of the 'color' represents the vector's component. Blue
            is vertical (Z), Red the X direction and Green the Y direction. Note that
            the y-direction of the data is from the bottom up.
    VHGT (4232 bytes)
        float HeightOffset
            A height offset for the entire cell. Decreasing this value will shift the
            entire cell land down.
        byte Unknown1 (0x00)
        signed byte  HeightData[65][65]
            Contains the height data for the cell in the form of a 65x65 pixel array. The
            height data is not absolute values but uses differences between adjacent pixels.
            Thus a pixel value of 0 means it has the same height as the last pixel. Note that
            the y-direction of the data is from the bottom up.
        short Unknown2 (0x0000)
    WNAM (81 bytes)
        byte Data[9][9] (Unknown byte data.)
    VCLR (12675 bytes) optional
        Vertex color array, looks like another RBG image 65x65 pixels in size.
    VTEX (512 bytes) optional
        A 16x16 array of short texture indices (from a LTEX record I think). */

  //write INTV
  output.write((char*) &cINTV, 4);
  uint32_t SubLength = 8; /* fixed size is eight bytes*/
  //write INTV's length
  output.write((char*) &SubLength, 4);
  //write cell offsets
  output.write((char*) &CellX, 4);
  output.write((char*) &CellY, 4);

  //write DATA
  output.write((char*) &cDATA, 4);
  SubLength = 4; //always four bytes
  //write DATA's length
  output.write((char*) &SubLength, 4);
  //write unknown data
  output.write((char*) &UnknownData, 4);

  if (hasNormals)
  {
    //write VNML
    output.write((char*) &cVNML, 4);
    SubLength = 12675; //always 12675 bytes
    //write VNML's length
    output.write((char*) &SubLength, 4);
    //write normals
    output.write((char*) &(Normals[0][0][0]), 65*65*3);
  }

  if (hasHeightSubrecord)
  {
    //write VHGT
    output.write((char*) &cVHGT, 4);
    SubLength = 4232; //always 4232 bytes
    //write VHGT's length
    output.write((char*) &SubLength, 4);
    //write height data
    // --- offset
    output.write((char*) &HeightOffset, 4);
    // --- unknown byte
    output.write((char*) &UnknownByte, 1);
    // --- the actual height data
    output.write((char*) &(HeightData[0][0]), 65*65);
    // --- unknown short
    output.write((char*) &UnknownShort, 2);
  }

  if (hasWNAMSubrecord)
  {
    //write WNAM
    output.write((char*) &cWNAM, 4);
    SubLength = 81; //always 81 bytes
    //write WNAM's length
    output.write((char*) &SubLength, 4);
    //write WNAM
    output.write(Data, 81);
  }

  if (hasVertexColour)
  {
    //write VCLR
    output.write((char*) &cVCLR, 4);
    SubLength = 12675; //always 12675 bytes
    //write VNML's length
    output.write((char*) &SubLength, 4);
    //write vertex colour
    output.write((char*) &(VertexColour[0][0][0]), 65*65*3);
  }

  if (hasVertexTextures)
  {
    //write VTEX
    output.write((char*) &cVTEX, 4);
    SubLength = 512; //always 512 bytes
    //write VTEX's length
    output.write((char*) &SubLength, 4);
    //write vertex textures
    output.write((char*) &(VertexTextures[0][0]), 16*16*2);
  }

  return output.good();
}

bool LandscapeRecord::loadFromStream(std::ifstream& in_File)
{
  uint32_t Size;
  in_File.read((char*) &Size, 4);
  in_File.read((char*) &HeaderOne, 4);
  in_File.read((char*) &HeaderFlags, 4);

  /*Landscape:
    INTV (8 bytes)
        long CellX }
        long CellY } The cell coordinates of the cell.
    DATA (4 bytes)
        long Unknown (default of 0x09)
    VNML (12675 bytes)
        struct {
          signed byte X
          signed byte Y
          signed byte Z
        } normals[65][65];
            A RGB color map 65x65 pixels in size representing the land normal vectors.
            The signed value of the 'color' represents the vector's component. Blue
            is vertical (Z), Red the X direction and Green the Y direction. Note that
            the y-direction of the data is from the bottom up.
    VHGT (4232 bytes)
        float HeightOffset
            A height offset for the entire cell. Decreasing this value will shift the
            entire cell land down.
        byte Unknown1 (0x00)
        signed byte  HeightData[65][65]
            Contains the height data for the cell in the form of a 65x65 pixel array. The
            height data is not absolute values but uses differences between adjacent pixels.
            Thus a pixel value of 0 means it has the same height as the last pixel. Note that
            the y-direction of the data is from the bottom up.
        short Unknown2 (0x0000)
    WNAM (81 bytes)
        byte Data[9][9] (Unknown byte data.)
    VCLR (12675 bytes) optional
        Vertex color array, looks like another RBG image 65x65 pixels in size.
    VTEX (512 bytes) optional
        A 16x16 array of short texture indices (from a LTEX record I think). */

  int32_t SubRecName;
  uint32_t SubLength, BytesRead;
  SubRecName = SubLength = 0;

  //read INTV
  in_File.read((char*) &SubRecName, 4);
  BytesRead = 4;
  if (SubRecName!=cINTV)
  {
    UnexpectedRecord(cINTV, SubRecName);
    return false;
  }
  //INTV's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=8)
  {
    std::cout <<"Error: sub record INTV of LAND has invalid length ("<<SubLength
              <<" bytes). Should be 8 bytes.\n";
    return false;
  }
  //read cell offsets
  in_File.read((char*) &CellX, 4);
  in_File.read((char*) &CellY, 4);
  BytesRead += 8;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord INTV of LAND.\n";
    return false;
  }

  //read DATA
  in_File.read((char*) &SubRecName, 4);
  BytesRead += 4;
  if (SubRecName!=cDATA)
  {
    UnexpectedRecord(cDATA, SubRecName);
    return false;
  }
  //DATA's length
  in_File.read((char*) &SubLength, 4);
  BytesRead += 4;
  if (SubLength!=4)
  {
    std::cout <<"Error: sub record DATA of LAND has invalid length ("<<SubLength
              <<" bytes). Should be 4 bytes.\n";
    return false;
  }
  //read UnknownData
  in_File.read((char*) &UnknownData, 4);
  BytesRead += 4;
  if (!in_File.good())
  {
    std::cout << "Error while reading subrecord DATA of LAND.\n";
    return false;
  }

  hasNormals = false;
  hasHeightSubrecord = false;
  hasWNAMSubrecord = false;
  hasVertexColour = false;
  hasVertexTextures = false;

  bool readVNML = false;
  bool readVHGT = false;
  bool readWNAM = false;
  bool readVCLR = false;
  bool readVTEX = false;
  //read optinal part
  while (BytesRead<Size)
  {
    //read next subrecord
    in_File.read((char*) &SubRecName, 4);
    BytesRead += 4;
    switch(SubRecName)
    {
      case cVNML:
           if (readVNML)
           {
             std::cout << "Error: record LAND seems to have two VNML subrecords.\n";
             return false;
           }
           //VNML's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=65*65*3)
           {
             std::cout <<"Error: sub record VNML of LAND has invalid length ("<<SubLength
                       <<" bytes). Should be 12.675 bytes.\n";
             return false;
           }
           //read normal vectors
           in_File.read((char*) &(Normals[0][0][0]), 65*65*3);
           BytesRead += (65*65*3);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord VNML of LAND.\n";
             return false;
           }
           hasNormals = true;
           readVNML = true;
           break;
      case cVHGT:
           if (readVHGT)
           {
             std::cout << "Error: record LAND seems to have two VHGT subrecords.\n";
             return false;
           }
           //VHGT's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=4232)
           {
             std::cout <<"Error: sub record VHGT of LAND has invalid length ("<<SubLength
                       <<" bytes). Should be 4.232 bytes.\n";
             return false;
           }
            //read height offset
           in_File.read((char*) &HeightOffset, 4);
           //read UnknownByte
           in_File.read((char*) &UnknownByte, 1);
           //read height data
           in_File.read((char*) &(HeightData[0][0]), 65*65);
           //read UnknownShort
           in_File.read((char*) &UnknownShort, 2);
           BytesRead += 4232;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord VHGT of LAND.\n";
             return false;
           }
           hasHeightSubrecord = true;
           readVHGT = true;
           break;
      case cWNAM:
           if (readWNAM)
           {
             std::cout << "Error: record LAND seems to have two WNAM subrecords.\n";
             return false;
           }
           //WNAM's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=81)
           {
             std::cout <<"Error: sub record WNAM of LAND has invalid length ("<<SubLength
                       <<" bytes). Should be 81 bytes.\n";
             return false;
           }
           //read WNAM
           in_File.read(Data, 81);
           BytesRead += 81;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord WNAM of LAND.\n";
             return false;
           }
           hasWNAMSubrecord = true;
           readWNAM = true;
           break;
      case cVCLR:
           if (readVCLR)
           {
             std::cout << "Error: record LAND seems to have two VCLR subrecords.\n";
             return false;
           }
           //VCLR's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=65*65*3)
           {
             std::cout <<"Error: sub record VCLR of LAND has invalid length ("<<SubLength
                       <<" bytes). Should be 12.675 bytes.\n";
             return false;
           }
           //read colour data
           in_File.read((char*) &VertexColour[0][0][0], 65*65*3);
           BytesRead += (65*65*3);
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord VCLR of LAND.\n";
             return false;
           }
           hasVertexColour = true;
           readVCLR = true;
           break;
      case cVTEX:
           if (readVTEX)
           {
             std::cout << "Error: record LAND seems to have two VTEX subrecords.\n";
             return false;
           }
           //VTEX's length
           in_File.read((char*) &SubLength, 4);
           BytesRead += 4;
           if (SubLength!=512)
           {
             std::cout <<"Error: sub record VTEX of LAND has invalid length ("<<SubLength
                       <<" bytes). Should be 512 bytes.\n";
             return false;
           }
           //read texture data
           in_File.read((char*) &VertexTextures[0][0], 16*16*2);
           BytesRead += 512;
           if (!in_File.good())
           {
             std::cout << "Error while reading subrecord VTEX of LAND.\n";
             return false;
           }
           hasVertexTextures = true;
           readVTEX = true;
           break;
      default:
           //unknown/ wron subrecord encountered
           std::cout << "Error: expected record name VNML, VHGT, WNAM, VCLR, or"
                     << " VTEX was not found. Instead, \""
                     <<IntTo4Char(SubRecName)<< "\" was found.\n";
           return false;
           break;
    }//swi
  }//while

  return in_File.good();
}

} //namespace
