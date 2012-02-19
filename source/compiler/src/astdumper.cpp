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
#include "astdumper.h"
#include "sourcefile.h"

using namespace compiler;

void AstDumper::Accept(AstNode *node)
{
    AstNode::AcceptChild(node, *this);
}

void AstDumper::PrintOffsets(AstNode *node)
{
    std::cout << " |s:" << node->m_sourceFile->GetLine(node->m_beginOffset) << "|";
}

void AstDumper::NewLineAndIndent()
{
    m_out << std::endl;
    for (int i = 0; i < (m_indentLevel * m_indent); ++i)
    {
        m_out << ' ';
    }
}

void AstDumper::IncLevel()
{
    ++m_indentLevel;
}

void AstDumper::DecLevel()
{
    --m_indentLevel;
}

int AstDumper::IndentLevel(int level)
{
    int previous = m_indentLevel;
    m_indentLevel = level;
    return (previous);
}

bool AstDumper::Visit(AstIdentifierExpr *expr) 
{
    m_out << "(AstId: ";

    m_out << '\'' << expr->GetName() << '\''; PrintOffsets(expr);

    m_out << ")";
    return (false);
}

bool AstDumper::Visit(AstStringLiteralExpr *expr) 
{
    m_out << "(AstString: ";

    m_out << '\'' << expr->GetName() << '\''; PrintOffsets(expr);

    m_out << ")";
	return (false);
}

bool AstDumper::Visit(AstBlockStatement *node) 
{
    NewLineAndIndent();
    m_out << "(AstBlockStatement: ";
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_statements);
    DecLevel();
    m_out << ")";
	return (false);
}

bool AstDumper::Visit(AstStatementList *node) 
{
    m_out << "(AstStatementList"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_statement);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        NewLineAndIndent();
        Accept(node->m_statement);
    }
    NewLineAndIndent();
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstDoWhileStatement *node) 
{
    m_out << "(AstDoWhileStatement"; PrintOffsets(node);
    IncLevel();
    Accept(node->m_statement);
    Accept(node->m_condition);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstWhileStatement *node) 
{
    m_out << "(AstWhile"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_condition);

    NewLineAndIndent();
    Accept(node->m_statement);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstForStatement *node) 
{
    m_out << "(AstForStatement"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_initializer);
    NewLineAndIndent();
    Accept(node->m_condition);
    NewLineAndIndent();
    Accept(node->m_expression);
    Accept(node->m_statement);
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstAssignmentList *node) 
{
    m_out << "(AstAssignmentList"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_assignment);
    NewLineAndIndent();
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        Accept(node->m_assignment);
        NewLineAndIndent();
    }
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstAssignmentExpr *expr) 
{
    m_out << "(AstAssignmentExpr"; PrintOffsets(expr);
    
    IncLevel();
    NewLineAndIndent();
    Accept(expr->m_variable);
    NewLineAndIndent();
    Accept(expr->m_expression);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstVariableExpr *node) 
{
    m_out << "(AstVariableExpr"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_identifier);
    NewLineAndIndent();
    Accept(node->m_array);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstArrayExpr *node) 
{
    m_out << "(AstArrayExpr"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_expression);
    if (node->m_nextArray)
    {
        NewLineAndIndent();
        Accept(node->m_nextArray);
    }
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstReadStatement *node) 
{
    m_out << "(AstRead"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_argumentList);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstReadArgumentList *node) 
{
    m_out << "(AstReadArgumentList"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_id);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        NewLineAndIndent();
        Accept(node->m_id);
    }
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstConditionExpression *node) 
{
    m_out << "(AstCondition: ";
    m_out << "\'" << Token::TokenToString(node->m_operation) << "\'"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_expr1);
    NewLineAndIndent();
    Accept(node->m_expr2);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstCharacterConstant *node) 
{
    m_out << "(AstCharacterConstant: ";
    std::string name = node->GetName();
    if(name == "\n")
    {
        name = "\\n";
    }
    else if(name == "\t")
    {
        name  = "\\t";
    }
    else if(name == "\r")
    {
        name = "\\r";
    }
    
    m_out << '\'' << name << '\''; PrintOffsets(node);
    m_out << ")";
	return (false);
}

bool AstDumper::Visit(AstWriteStatement *node) 
{
    m_out << "(AstWriteStatement"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_arguments);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstWriteArgumentList *node) 
{
    m_out << "(AstWriteArgumentList"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_expression);
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        NewLineAndIndent();
        Accept(node->m_expression);
    }
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstTerm *node) 
{
    m_out << "(AstTerm"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_expression);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstMultExpr *node) 
{
    m_out << "(AstMult"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_unaryExpr);
    NewLineAndIndent();
    Accept(node->m_term);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstDivExpr *node) 
{
    m_out << "(AstDiv"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_unaryExpr);
    NewLineAndIndent();
    Accept(node->m_term);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstSubExpr *node) 
{
    m_out << "(AstSub"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_term);
    NewLineAndIndent();
    Accept(node->m_expression);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstAddExpr *node) 
{
    m_out << "(AstAdd"; PrintOffsets(node);
    IncLevel();
    
    NewLineAndIndent();
    Accept(node->m_term);
    NewLineAndIndent();
    Accept(node->m_expression);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstFactorExpr *node) 
{
    m_out << "(AstFactor"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_expression);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstUnaryExpr *expr) 
{
    m_out << "(AstUnaryExpr";
    if (expr->m_unaryOperator != Token::Token_Add)
    {
        m_out << ": \'" << Token::TokenToString(expr->m_unaryOperator) << '\'';
    }
    PrintOffsets(expr);
    IncLevel();
    NewLineAndIndent();
    Accept(expr->m_factor);

    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstIfStatement *node) 
{
    m_out << "(AstIfStatement"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();
    Accept(node->m_condition);
    NewLineAndIndent();
    Accept(node->m_thenBlock);
    Accept(node->m_elseBlock);
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstBreakStatement *node) 
{
    m_out << "(AstBreakStatement)"; PrintOffsets(node);

    return (false);
}

bool AstDumper::Visit(AstDeclaration *node) 
{
    m_out << "(AstDeclaration"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();

    m_out << "(" << Token::TokenToString(node->m_type) << ")";
    NewLineAndIndent();

    Accept(node->m_variable);
    DecLevel();
    m_out << ")";
	return (false);
}

bool AstDumper::Visit(AstGlobalVarDeclarationList *node) 
{
    m_out << "(AstGlobalVarDeclarationList"; PrintOffsets(node);
    IncLevel();
    NewLineAndIndent();

    Accept(node->m_declaration);
    NewLineAndIndent();
    for (node = node->m_next; node != NULL; node = node->m_next)
    {
        Accept(node->m_declaration);
        NewLineAndIndent();
    }
    m_out << ")";
    DecLevel();
	return (false);
}

bool AstDumper::Visit(AstMain *node) 
{ 
    m_out << "(AstMain"; PrintOffsets(node);
    IncLevel();
    Accept(node->m_block);
    DecLevel();
    m_out << ")";
	return (false);
}

bool AstDumper::Visit(AstProgram *node) 
{
    m_out << "(AstProgram"; PrintOffsets(node);

    IncLevel();
    NewLineAndIndent();

    Accept(node->m_globalDecls);
    NewLineAndIndent();
    Accept(node->m_mainFunction);
    DecLevel();

    m_out << "\n)";
	return (false);
}

bool AstDumper::Visit(AstNumberLiteralExpr *node) 
{
    m_out << "(AstNumberLiteralExpr: ";
    m_out << '\'' << node->GetName() << '\''; PrintOffsets(node);
    m_out << ")";
	return (false);
}