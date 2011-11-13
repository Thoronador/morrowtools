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

#ifndef PARSERNODE_H
#define PARSERNODE_H

#include <string>
#include <vector>
#include "CompiledChunk.h"

namespace MWTP
{

namespace ScriptCompiler
{

/* tries to find the next operator in the given string, starting at position offset

   parameters:
       expr   - the string that will be searched for operators (+, -, *, /)
       offset - position where the search starts
*/
std::string::size_type getNextOperatorPos(const std::string& expr, const std::string::size_type offset);

/* returns true, of the operator character op1 has lower or equal operator
   precendence, compared to op2

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
    /* constructor */
    ParserNode();

    /* destructor */
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
    //pointers to the left and right branch
    ParserNode* left;
    ParserNode* right;
};//struct

} //namespace ScriptCompiler

} //namespace MWTP

#endif // PARSERNODE_H
