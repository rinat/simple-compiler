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
#ifndef __Compiler_AstForwardDeclarations_h__
#define __Compiler_AstForwardDeclarations_h__

namespace compiler
{
    class AstNode;
    class AstExpression;
    class AstStatement;
    class AstIdentifierExpr;
    class AstStringLiteralExpr;
    class AstBlockStatement;
    class AstStatementList;
    class AstCharacterConstant;
    class AstConditionExpression;
    class AstDoWhileStatement;
    class AstWhileStatement;
    class AstForStatement;
    class AstAssignmentList;
    class AstAssignmentExpr;
    class AstVariableExpr;
    class AstArrayExpr;
    class AstReadArgumentList;
    class AstReadStatement;
    class AstWriteArgumentList;
    class AstWriteStatement;
    class AstUnaryExpr;
    class AstMultExpr;
    class AstDivExpr;
    class AstSubExpr;
    class AstAddExpr;
    class AstFactorExpr;
    class AstTerm;
    class AstIfStatement;
    class AstBreakStatement;
    class AstDeclaration;
    class AstGlobalVarDeclarationList;
    class AstMain;
    class AstProgram;
    class AstNumberLiteralExpr;
}

#endif