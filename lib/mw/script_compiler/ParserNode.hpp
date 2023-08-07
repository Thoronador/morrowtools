/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2013, 2021  Dirk Stolle

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

#ifndef MWTP_PARSERNODE_HPP
#define MWTP_PARSERNODE_HPP

#include <string>
#include <vector>
#include "CompiledChunk.hpp"

namespace MWTP::ScriptCompiler
{

/* tries to find the next operator in the given string, starting at position offset

   parameters:
       expr   - the string that will be searched for operators (+, -, *, /)
       offset - position where the search starts
*/
std::string::size_type getNextOperatorPos(const std::string& expr, const std::string::size_type offset);

/* returns true, of the operator character op1 has lower or equal operator
   precedence, compared to op2

   parameters:
       op1 - first operator
       op2 - second operator

   remarks:
       Both op1 and op2 have to be one of '+', '-', '*' or '/'. Otherwise the
       function might throw an exception.
*/
bool lowerOrEqualPrecedence(const char op1, const char op2);

//simple structure for nodes of a sort of binary tree
struct ParserNode
{
  public:
    ParserNode();
    ParserNode(const ParserNode& op);
    ParserNode& operator=(const ParserNode& op);
    ~ParserNode();

    /* deletes all branches of the node, if they exist */
    void clearBranches();

    /* tries to build up a tree from the content of string expression and
       returns true on success

       parameters:
           expression - the string that contains the mathematical expression
           chunkVars  - the compiled chunk that holds the local vars for this
                        expression/ script
    */
    bool splitToTree(std::string expression, const CompiledChunk& chunkVars);

    /*returns the data of the node and its branches in the order MW would use it*/
    std::string getStackOrderedContent() const;

    /*returns the data of the node and its branches in the order MW would use it*/
    std::vector<uint8_t> getBinaryContent() const;
  private:
    //content of this node
    std::string content;

    enum ContentType {ctNone, ctFloat, ctOperator, ctLocalVar, ctGlobalVar,
                      ctForeignRef, ctFunction};
    //type of content
    ContentType type;
    std::vector<uint8_t> binary_content;
    // pointers to the left and right branch
    ParserNode* left;
    ParserNode* right;
}; // struct

} // namespace

#endif // MWTP_PARSERNODE_HPP
