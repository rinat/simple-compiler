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
#include "stdafx.h"
#include "token.h"

using namespace compiler;

#pragma warning(push)
#pragma warning(disable:4355) // warning about this

void AstIdentifierExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {}
}

void AstNumberLiteralExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {}
}

void AstStringLiteralExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {}
}

void AstBlockStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_statements, visitor);
    }
}

void AstStatementList::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AstStatementList *it = this;
        do 
        {
            AcceptChild(it->m_statement, visitor);
            it = it->m_next;
        } while (it);
    }
}

void AstDoWhileStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_statement, visitor);
        AcceptChild(m_condition, visitor);
    }
}

void AstWhileStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_condition, visitor);
        AcceptChild(m_statement, visitor);
    }
}

void AstForStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_initializer, visitor);
        AcceptChild(m_condition, visitor);
        AcceptChild(m_expression, visitor);
        AcceptChild(m_statement, visitor);
    }
}

void AstAssignmentList::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AstAssignmentList *it = this;
        do 
        {
            AcceptChild(it->m_assignment, visitor);
            it = it->m_next;
        } while (it);
    }
}

void AstAssignmentExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_variable, visitor);
        AcceptChild(m_expression, visitor);
    }
}

void AstVariableExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_identifier, visitor);
        AcceptChild(m_array, visitor);
    }
}

void AstArrayExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_expression, visitor);
        AcceptChild(m_nextArray, visitor);
    }
}

void AstReadStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_argumentList, visitor);
    }
}

void AstReadArgumentList::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AstReadArgumentList *it = this;
        do 
        {
            AcceptChild(it->m_id, visitor);
            it = it->m_next;
        } while (it);
    }
}

void AstConditionExpression::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_expr1, visitor);
        AcceptChild(m_expr2, visitor);
    }
}

void AstCharacterConstant::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {}
}

void AstWriteStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_arguments, visitor);
    }
}

void AstWriteArgumentList::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AstWriteArgumentList *it = this;
        do 
        {
            AcceptChild(it->m_expression, visitor);
            it = it->m_next;
        } while (it);
    }
}

void AstTerm::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_expression, visitor);
    }
}

void AstMultExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_unaryExpr, visitor);
        AcceptChild(m_term, visitor);
    }
}

void AstDivExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_unaryExpr, visitor);
        AcceptChild(m_term, visitor);
    }
}

void AstSubExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_term, visitor);
        AcceptChild(m_expression, visitor);
    }
}

void AstAddExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_term, visitor);
        AcceptChild(m_expression, visitor);
    }
}

void AstFactorExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_expression, visitor);
    }
}

void AstUnaryExpr::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_factor, visitor);
    }
}

void AstIfStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_condition, visitor);
        AcceptChild(m_thenBlock, visitor);
        AcceptChild(m_elseBlock, visitor);
    }
}

void AstBreakStatement::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {}
}

void AstDeclaration::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_variable, visitor);
    }
}

void AstGlobalVarDeclarationList::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AstGlobalVarDeclarationList *it = this;
        do 
        {
            AcceptChild(it->m_declaration, visitor);
            it = it->m_next;
        } while (it);
    }
}

void AstMain::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_block, visitor);
    }
}

void AstProgram::Accept(AstVisitor& visitor)
{
    if (visitor.Visit(this))
    {
        AcceptChild(m_globalDecls, visitor);
        AcceptChild(m_mainFunction, visitor);
    }
}

#pragma warning(pop)