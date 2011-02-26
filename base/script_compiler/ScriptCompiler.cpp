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

#include "ScriptCompiler.h"
#include <iostream>
#include "CompilerCodes.h"

namespace ScriptCompiler
{

void CompiledChunk::pushCode(const uint16_t code)
{
  data.push_back(code&255);
  data.push_back(code>>8);
}

void CompiledChunk::pushString(const std::string& str)
{
  unsigned int i;
  const unsigned int len = str.length();
  for (i=0; i<len; ++i)
  {
    data.push_back(str[i]);
  }//for
}

//tries to get the integer representation of a string
uint16_t stringToShort(const std::string& str)
{
  uint16_t result = 0;
  unsigned int i;
  for (i=0; i<str.length(); ++i)
  {
    if ((str.at(i)>='0') and (str.at(i)<='9'))
    {
      result = result * 10;
      result = result + (str.at(i)-'0');
    }//if
  }//for
  return result;
}

void trimLeft(std::string& str1)
{
  if (str1=="") return;
  //trim stuff at begin
  int32_t len = str1.length();
  int32_t pos = 0;
  bool go_on = true;
  while (go_on)
  {
    char& ch = str1.at(pos);
    if ((ch==' ') or (ch=='\t'))
    {
      ++pos;
      go_on = (pos<len);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos>=len)
  {
    str1 = "";
    return;
  }
  else if (pos>0)
  {
    str1.erase(0, pos);
  }
  return;
}

void trimRight(std::string& str1)
{
  if (str1=="") return;
  //trim stuff at end
  int32_t len = str1.length();
  int32_t pos = len-1;
  bool go_on = true;
  while (go_on)
  {
    char& ch = str1.at(pos);
    if ((ch==' ') or (ch=='\t'))
    {
      --pos;
      go_on = (pos>=0);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos==-1)
  {
    str1 = "";
  }
  else if (pos<len-1)
  {
    str1.erase(pos+1);
  }
  return;
}

void trim(std::string& str1)
{
  trimLeft(str1);
  trimRight(str1);
  return;
} //end of trim

void StripEnclosingQuotes(std::string& str1)
{
  if ((str1=="") or (str1.length()<2)) return;
  if ((str1.at(0)=='"') and (str1.at(str1.length()-1)=='"'))
  {
    str1 = str1.substr(1, str1.length()-2);
  }
  return;
}

std::vector<std::string> explodeParams(const std::string& source)
{
  std::vector<std::string> result;
  if (source.empty()) return result;

  const unsigned int len = source.length();
  unsigned int look = 0;
  unsigned int offset = 0;
  bool insideQuote = false;
  while (look<len)
  {
    if (source.at(look)=='"')
    {
      insideQuote = not insideQuote;
    }
    else if ((not insideQuote) and ((source.at(look)==' ') or (source.at(look)==',')))
    {
      //found a place where to split
      unsigned int len = look-offset;
      if (len>0)//skip empty params
      {
        result.push_back(source.substr(offset, len));
        StripEnclosingQuotes(result.back());
      }//if
      offset=look+1;
    }//else
    ++look;
  }//while
  //add the rest, if not finished yet
  if (offset<len)
  {
    result.push_back(source.substr(offset));
    StripEnclosingQuotes(result.back());
  }
  if (insideQuote)
  {
    std::cout << "Warning: explodeParams: Quotes did not match!\n";
  }
  return result;
}

std::string lowerCase(const std::string& str1)
{
  /* We assume ASCII or compatible charset where the characters for letters are
     in sequence.
  */
  const char lowerDiff = 'a'-'A';
  std::string result = str1;
  unsigned int i;
  for (i=0; i<str1.length(); ++i)
  {
    if ((result[i]<='Z') and (result[i]>='A'))
    {
      result[i] = result[i]+ lowerDiff;
    }
  }//for
  return result;
}

bool CompileScript(const std::string& Text, ScriptRecord& result)
{
  std::vector<std::string> lines;

  size_t offset = 0;
  const size_t len = Text.length();
  std::string::size_type pos = std::string::npos;
  std::string new_line;
  //split text into single lines
  while (offset<len)
  {
    pos = Text.find("\n", offset);
    if (pos!=std::string::npos)
    {
      //found something
      // -> extract complete line
      new_line = Text.substr(offset, pos-offset);
      offset = pos+1;
    }//if
    else
    {
      //found nothing -> rest of string is one line
      new_line = Text.substr(offset);
      offset = len;
    }//else

    // -> remove any commments, if present (comments start with ';')
    pos = new_line.find(";");
    if (pos!=std::string::npos)
    {
      //remove comment - everything after ";" is a comment
      new_line.erase(pos);
    }//if comment found
    // -> remove \r characters at the end, if present
    if (new_line.at(new_line.length()-1)=='\r')
    {
      new_line.erase(new_line.length()-1);
    }
    //cut off spaces and tabulators
    trim(new_line);
    if (new_line!="")
    {
      lines.push_back(new_line);
    }
  }//while

  std::vector<std::string> varsShort;
  std::vector<std::string> varsLong;
  std::vector<std::string> varsFloat;
  std::string ScriptName = "";
  std::string WorkString;

  CompiledChunk CompiledData;

  //now check the lines of the script
  //first line should always be begin
  if (lowerCase(lines.at(0).substr(0,6))=="begin ")
  {
    ScriptName = lines.at(0).substr(6);
    trimLeft(ScriptName);//We only trim left side, because right side was
                         //already trimmed for original string.
    if (ScriptName=="")
    {
      std::cout << "ScriptCompiler: Error: begin clause found with no script name!\n";
      return false;
    }
  }//if
  else
  {
    std::cout << "ScriptCompiler: Error: no begin clause found!\n";
    return false;
  }

  bool EndLineFound = false;
  //check other lines
  unsigned int i;
  for (i=1; i<lines.size(); ++i)
  {
    //check for shorts
    if (lowerCase(lines.at(i).substr(0,6))=="short ")
    {
      WorkString = lines.at(i).substr(6);
      trim(WorkString);
      if (WorkString=="")
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of short found!\n";
        return false;
      }
      varsShort.push_back(WorkString);
    }//if short
    //check for longs
    else if (lowerCase(lines.at(i).substr(0,5))=="long ")
    {
      WorkString = lines.at(i).substr(5);
      trim(WorkString);
      if (WorkString=="")
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of long found!\n";
        return false;
      }
      varsLong.push_back(WorkString);
    }//if long
    //check for floats
    else if (lowerCase(lines.at(i).substr(0,6))=="float ")
    {
      WorkString = lines.at(i).substr(6);
      trim(WorkString);
      if (WorkString=="")
      {
        std::cout << "ScriptCompiler: Error: incomplete declaration of float found!\n";
        return false;
      }
      varsFloat.push_back(WorkString);
    }//if float
    //check for Journal
    else if (lowerCase(lines.at(i).substr(0,8))=="journal ")
    {
      CompiledData.pushCode(CodeJournal);
      WorkString = lines.at(i).substr(8);
      trimLeft(WorkString);
      std::vector<std::string> params = explodeParams(WorkString);
      if (params.size() != 2)
      {
        std::cout << "ScriptCompiler: Error: Journal command expects two params"
                  << ", not "<<params.size()<<".\n";
        return false;
      }
      StripEnclosingQuotes(params[0]);
      //push journal ID's length
      CompiledData.data.push_back(params[0].length());
      CompiledData.pushString(params[0]);
      //second parameter should be index
      CompiledData.pushCode(stringToShort(params[1]));
      //fill data so it's four bytes
      CompiledData.data.push_back(255);
      CompiledData.data.push_back(255);
    }//if StopScript

    //check for StopScript
    else if (lowerCase(lines.at(i).substr(0,11))=="stopscript ")
    {
      CompiledData.pushCode(CodeStopScript);
      WorkString = lines.at(i).substr(11);
      trimLeft(WorkString);
      StripEnclosingQuotes(WorkString);
      CompiledData.data.push_back(WorkString.length());
      CompiledData.pushString(WorkString);
    }//if StopScript
    //check for return
    else if (lowerCase(lines.at(i))=="return")
    {
      CompiledData.pushCode(CodeReturn);
    }//if return found
    //check for end of script
    else if ((lowerCase(lines.at(i))=="end") or (lowerCase(lines.at(i).substr(0,4))=="end "))
    {
      CompiledData.pushCode(CodeEnd);
      EndLineFound = true;
      //this should be the end of the script - if not, there's junk ahead.
      if (i+1<lines.size())
      {
        std::cout << "ScriptCompiler: Error: there is more code after end clause.\n";
        return false;
      }
    }//if End found

  }//for

  std::cout << "Debug: SC done. Name is \""<<ScriptName<<"\". Shorts: "<<varsShort.size()<<", Longs: "<<varsLong.size()<<", Floats: "<<varsFloat.size()<<"\n";
  if (!EndLineFound)
  {
    std::cout << "ScriptCompiler: Error: there is no End line in the script!\n";
    return false;
  }


  /***** copy the data into result *****/
  //set result values
  result.NumShorts = varsShort.size();
  result.NumLongs = varsLong.size();
  result.NumFloats = varsFloat.size();
  //variables
  result.LocalVars = "";
  result.LocalVarSize = 0;
  // ---- write shorts
  for (i=0; i<varsShort.size(); ++i)
  {
    result.LocalVars.append(varsShort.at(i));
    result.LocalVars.append(1, '\0');
    result.LocalVarSize += varsShort.at(i).length()+1;
  }//for
  // ---- write longs
  for (i=0; i<varsLong.size(); ++i)
  {
    result.LocalVars.append(varsLong.at(i));
    result.LocalVars.append(1, '\0');
    result.LocalVarSize += varsLong.at(i).length()+1;
  }//for
  // ---- write floats
  for (i=0; i<varsFloat.size(); ++i)
  {
    result.LocalVars.append(varsFloat.at(i));
    result.LocalVars.append(1, '\0');
    result.LocalVarSize += varsFloat.at(i).length()+1;
  }//for

  //script's ID
  result.ScriptID = ScriptName;
  //text
  result.ScriptText = Text;
  //data size
  result.ScriptDataSize = CompiledData.data.size();

  /// TODO: properly transform data of CompiledChunk into result.ScriptData
  /** possible memory leak here **/
  result.ScriptData = new char[CompiledData.data.size()];
  //fill
  for (i=0; i<CompiledData.data.size(); ++i)
  {
    result.ScriptData[i] = CompiledData.data.at(i);
  }//for
  std::cout << "ScriptCompiler: Warning: compiler is still far from complete! "
            << "The resulting script might not run at all or not as intended "
            << "within the game!\n";
  return true;
}

} //namespace
