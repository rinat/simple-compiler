/*
* This code is under GNU Lesser General Public License.
* Simple C Compiler to Low Level Virtual Machine representation
*
* Authors: Rinat Shaikhutdinov (aka Sagit) - architecture, compiler core
*          Alexei Zhdanov (aka Alex) - parser
*          Timofey Tatarinov (aka Tikers) - code generator
*
* University Mari State University
* Faculty Informatics and Computer Science
* Course 4
* Specialization PS-41
* Subject Theory of programming languages and translation methods
* Yoshkar-Ola 2010
*/
#ifndef __Compiler_AstVisitor_h__
#define __Compiler_AstVisitor_h__

/*
    Abstract syntax tree structures visitor
*/

#include "astforwarddeclarations.h"
#include "typelist.h"
#include "typelistcreator.h"
#include "visitor.h"

namespace compiler
{
    using namespace typelist;
    using namespace visitor;

    typedef TypeListVisitor
    <
        bool,
        CreateTypeList<AstNode, AstExpression, AstStatement, AstIdentifierExpr, AstStringLiteralExpr,
                    AstBlockStatement, AstStatementList, AstCharacterConstant, AstConditionExpression,
                    AstDoWhileStatement, AstWhileStatement, AstForStatement, AstAssignmentList,
                    AstAssignmentExpr, AstVariableExpr, AstArrayExpr, AstReadArgumentList,
                    AstReadStatement, AstWriteArgumentList, AstWriteStatement, AstUnaryExpr, 
                    AstMultExpr, AstDivExpr, AstSubExpr, AstAddExpr, AstFactorExpr, AstTerm,
                    AstIfStatement, AstBreakStatement, AstDeclaration, AstGlobalVarDeclarationList,
                    AstMain, AstProgram, AstNumberLiteralExpr>::Result
    >
    AstVisitor;
}

#endif