/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012, 2013, 2014  Thoronador

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

#include "ParserNode.h"
#include <iostream>
#include <stdexcept>
#include "../../../base/UtilityFunctions.h"
#include "CompiledChunk.h"
#include "ScriptCompiler.h"
#include "../Globals.h"

namespace MWTP
{

namespace ScriptCompiler
{

ParserNode::ParserNode()
: content(""),
  type(ctNone),
  binary_content(std::vector<uint8_t>()),
  left(NULL),
  right(NULL)
{ }

ParserNode::ParserNode(const ParserNode& op)
: content(op.content),
  type(op.type),
  binary_content(op.binary_content),
  left(NULL),
  right(NULL)
{
  if (op.left!=NULL)
  {
    left = new ParserNode(*op.left);
  }
  if (op.right!=NULL)
  {
    right = new ParserNode(*op.right);
  }
}

ParserNode& ParserNode::operator=(const ParserNode& op)
{
  if (&op == this) return *this;
  content = op.content;
  type = op.type;
  binary_content = op.binary_content;
  left = NULL;
  right = NULL;
  if (op.left!=NULL)
  {
    left = new ParserNode(*op.left);
  }
  if (op.right!=NULL)
  {
    right = new ParserNode(*op.right);
  }
  return *this;
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
    const std::string::size_type dotPos = getDotPosition(expression);
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
      //check for global
      else if (Globals::getSingleton().hasRecord(expression))
      {
        //found a global var
        type = ctGlobalVar;
        content = Globals::getSingleton().getRecord(expression).recordID;
        //push G for global
        binary_content.push_back('G');
        //push length of name
        const unsigned int glob_len = content.length();
        binary_content.push_back(glob_len);
        //push global itself
        unsigned int i;
        for (i=0; i<glob_len; ++i)
        {
          binary_content.push_back(content[i]);
        }
      }//globals
      //check for foreign ref
      else if (dotPos!=std::string::npos)
      {
        std::string objectName = expression.substr(0, dotPos);
        StripEnclosingQuotes(objectName);
        const SC_VarRef foreignRef = getForeignVariableTypeWithIndex(objectName,
                                         expression.substr(dotPos+1));
        if (foreignRef.Type==vtGlobal)
        {
          std::cout << "ScriptCompiler: Error: couldn't find foreign reference in \""
                    << expression<<"\" for compare/parsed statement.\n";
          return false;
        }//if
        temp_chunk.data.clear();
        //push r for reference
        temp_chunk.data.push_back('r');
        //push object ID's length
        temp_chunk.data.push_back(objectName.length());
        //push object ID
        temp_chunk.pushString(objectName);
        //push reference
        temp_chunk.pushNonGlobalRef(foreignRef);
        //get binary data from chunk
        binary_content = temp_chunk.data;
        type = ctForeignRef;
      }
      //check for function
      else if (ScriptFunctions(expression, temp_chunk, true))
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

  // --- check for minus in first character
  if (expression.at(0)=='-')
  {
    //could still be a negative float value
    //check for float
    float fVal;
    if (stringToFloat(expression, fVal))
    {
      type = ctFloat;
      CompiledChunk temp_chunk;
      temp_chunk.pushString(expression);
      binary_content = temp_chunk.data;
      content = expression;
      return true;
    }
  }//if minus as first character

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
    std::cout << "ScriptCompiler: Error: operator at beginning of expression!\n"
              << "Expression was \""<<expression<<"\".\n";
    return false;
  }
  else if (pos==expression.length()-1)
  {
    std::cout << "ScriptCompiler: Error: operator at end of expression!\n"
              << "Expression was \""<<expression<<"\".\n";
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
  bool outsideQuote = true;
  std::string::size_type look = 0;
  //check for quotes before offset
  while ((look<offset) and (look<len))
  {
    if (expr.at(look)=='"')
    {
      outsideQuote = not outsideQuote;
    }
    ++look;
  }//while
  //now go for the part at offset and thereafter
  look = offset;
  int bracket_level = 0;
  while (look<len)
  {
    if (expr.at(look)=='"')
    {
      outsideQuote = not outsideQuote;
    }
    else if (outsideQuote)
    {
      //Brackets and operators only count outside of quotes.
      if (expr.at(look)=='(')
      {
        ++bracket_level;
      }
      else if (expr.at(look)==')')
      {
        --bracket_level;
      }
      else if (bracket_level==0)
      {
        if ((expr.at(look)=='+') or (expr.at(look)=='*') or (expr.at(look)=='/'))
        {
          //found a place where next operator is located
          return look;
        }//if
        //minus could also be start of qualifier "->", so make sure that's not
        // the case here
        else if (expr.at(look)=='-')
        {
          if (look+1<len)
          {
            if (expr.at(look+1)!='>') return look;
          }
          //string is too short or next character isn't '>'
          return look;
        }//else if minus
      }//else if (bracket level is zero)
    }//else if not within quotes
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
  throw std::logic_error("lowerOrEqualPrecedence(): You should not be here!");
}

}//namespace ScriptCompiler

} //namespace MWTP
