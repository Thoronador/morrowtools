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

#include "ParserNode.h"
#include <iostream>
#include "UtilityFunctions.h"
#include "CompiledChunk.h"
#include "ScriptCompiler.h"

namespace ScriptCompiler
{

ParserNode::ParserNode()
{
  content = "";
  type = ctNone;
  left = NULL;
  right = NULL;
}

ParserNode::~ParserNode()
{
  //clean up left and right branches
  clearBranches();
}

void ParserNode::clearBranches()
{
  //clean up left and right branches
  if (left!=NULL)
  {
    delete left;
    left = NULL;
  }
  if (right!=NULL)
  {
    delete right;
    right = NULL;
  }
}

bool ParserNode::splitToTree(std::string expression, const CompiledChunk& chunkVars)
{
  trim(expression);
  if (removeEnclosingBrackets(expression))
  {
    //There might be new spaces at the beginning and end of the string after the
    //brackets were removed, so we have to trim again.
    trim(expression);
  }
  std::string::size_type pos = getNextOperatorPos(expression, 0);
  clearBranches();
  binary_content.clear();
  if (pos==std::string::npos)
  {
    //There is no operator here, whole thing is one expression.
    //TODO: We should check for float or function here
    content = expression;
    //check for float
    float fVal;
    if (stringToFloat(expression, fVal))
    {
      type = ctFloat;
      CompiledChunk temp_chunk;
      temp_chunk.pushString(expression);
      binary_content = temp_chunk.data;
    }
    else
    {
      //guess it has to be a function or a local var
      CompiledChunk temp_chunk;
      //temp_chunk should get the same local vars as the real chunk, because
      // they will be needed to properly compile functions with local vars.
      temp_chunk.varsShort = chunkVars.varsShort;
      temp_chunk.varsLong = chunkVars.varsLong;
      temp_chunk.varsFloat = chunkVars.varsFloat;
      //check for local var
      SC_VarRef locRef = chunkVars.getVariableTypeWithIndex(expression);
      if (locRef.Type!=vtGlobal)
      {
        //found a local var here
        type = ctLocalVar;
        switch (locRef.Type)
        {
          case vtShort:
               binary_content.push_back('s');
               break;
          case vtLong:
               binary_content.push_back('l');
               break;
          case vtFloat:
               binary_content.push_back('f');
               break;
          case vtGlobal:
               //this should never happen
               std::cout << "ScriptCompiler: Error: got global var, but only "
                         << "local vars are accepted.\n";
               return false;
        }//swi
        //push index as short
        binary_content.push_back(locRef.Index&255);
        binary_content.push_back(locRef.Index>>8);
      }
      //check for function
      else if (ScriptFunctions(expression, temp_chunk))
      {
        //success
        type = ctFunction;
        binary_content = temp_chunk.data;
        //push X at start of function
        binary_content.insert(binary_content.begin(), 'X');
      }
      else
      {
        //failure
        std::cout << "ScriptCompiler: Error: Couldn't match expression \""
                  << expression <<"\" with any function or variable!\n";
        return false;
      }
    }
    return true;
  }//if
  //There's an operator, so we have to split.

  //check for presence of next operator to get precedence
  const std::string::size_type pos2 = getNextOperatorPos(expression, pos+1);
  //If there is no next operator, then go ahead
  if (pos2!=std::string::npos)
  {
    if (lowerOrEqualPrecedence(expression.at(pos2), expression.at(pos)))
    {
      //set second operator's position as the position for the split
      pos = pos2;
    }
  }
  if (pos==0)
  {
    std::cout << "ScriptCompiler: Error: operator at beginning of expression!\n";
    return false;
  }
  else if (pos==expression.length()-1)
  {
    std::cout << "ScriptCompiler: Error: operator at end of expression!\n";
    return false;
  }

  //get left part
  left = new ParserNode;
  if (!(left->splitToTree(expression.substr(0, pos), chunkVars)))
  {
    std::cout << "ScriptCompiler: Error: could not handle expression!\n";
    //clear up left side
    delete left;
    left = NULL;
    return false;
  }
  //get right part
  right = new ParserNode;
  if (!(right->splitToTree(expression.substr(pos+1), chunkVars)))
  {
    std::cout << "ScriptCompiler: Error: could not handle expression!\n";
    //clear up left side
    delete right;
    right = NULL;
    return false;
  }
  content = expression.at(pos);
  type = ctOperator;
  binary_content.push_back(expression.at(pos));
  return true;
}

std::string ParserNode::getStackOrderedContent() const
{
  std::string result;
  if (left!=NULL)
  {
    result = left->getStackOrderedContent();
  }
  if (right!=NULL)
  {
    result += right->getStackOrderedContent();
  }
  if (!content.empty())
  {
    result = result + " "+content;
  }
  return result;
}

std::vector<uint8_t> ParserNode::getBinaryContent() const
{
  std::vector<uint8_t> result;
  if (left!=NULL)
  {
    result = left->getBinaryContent();
  }
  if (right!=NULL)
  {
    const std::vector<uint8_t> right_part = right->getBinaryContent();
    unsigned int i;
    for (i=0; i<right_part.size(); ++i)
    {
      result.push_back(right_part.at(i));
    }
  }
  if (!binary_content.empty())
  {
    //push space
    result.push_back(' ');
    unsigned int i;
    for (i=0; i<binary_content.size(); ++i)
    {
      result.push_back(binary_content.at(i));
    }
  }
  return result;
}

std::string::size_type getNextOperatorPos(const std::string& expr, const std::string::size_type offset)
{
  const std::string::size_type len = expr.length();
  std::string::size_type look = offset;
  int bracket_level = 0;
  while (look<len)
  {
    if (expr.at(look)=='(')
    {
      ++bracket_level;
    }
    else if (expr.at(look)==')')
    {
      --bracket_level;
    }
    else if ((bracket_level==0) and ((expr.at(look)=='+') or (expr.at(look)=='-')
                                  or (expr.at(look)=='*') or (expr.at(look)=='/')))
    {
      //found a place where next operator is located
      return look;
    }//else
    ++look;
  }//while
  return std::string::npos;
}

bool lowerOrEqualPrecedence(const char op1, const char op2)
{
  if (op1==op2) return true;
  //if op1 is add or subtract, op2 cannot be less
  if ((op1=='+') or (op1=='-')) return true;
  //if both are mult or div -> equal
  if (((op1=='*') or (op1=='/')) and ((op2=='*') or (op2=='/'))) return true;
  //if op2 is add or subtract, and op1 is mult or div, op2 is less, i.e. false
  if (((op1=='*') or (op1=='/')) and ((op2=='+') or (op2=='-'))) return false;
  //if we are still here, something is wrong
  std::cout << "You shouldn't be here!\n";
  throw 42;
}

}//namespace
