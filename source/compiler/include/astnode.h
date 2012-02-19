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
#ifndef __Compiler_AstNode_h__
#define __Compiler_AstNode_h__

/*
   Abstract syntax tree structures.
   All AST nodes is simple structures
   with function Visit and without functional on destructor,
   because it's must be allocated in memory pool
*/

#include "token.h"
#include "astforwarddeclarations.h"
#include "astvisitor.h"

namespace compiler
{
    class ISourceFile;

    class AstNode
        : private utils::NonCopyable
    {
    public:
        
        AstNode()
            : m_beginOffset(0)
            , m_sourceFile(NULL)
        {}
        virtual ~AstNode() {}

        static void AcceptChild(AstNode *node, AstVisitor& visitor)
        { if (node) node->Accept(visitor); }

        virtual void Accept(AstVisitor& visitor) = 0;

        size_t GetStartOffset()
        { return (m_beginOffset); }

        size_t m_beginOffset;
        ISourceFile* m_sourceFile;
    };

    class AstExpression : public AstNode
    {
    public:
        AstExpression() {};
        virtual ~AstExpression() {};
    };

    class AstStatement : public AstNode
    {
    public:
        AstStatement() {};
        virtual ~AstStatement() {}
    };

    class AstIdentifierExpr : public AstExpression
    {
    public:
		AstIdentifierExpr(std::string *value)
            : m_name(value)
        { assert(value != NULL); }
		virtual ~AstIdentifierExpr() {}

        virtual void Accept(AstVisitor& visitor);

        const std::string& GetName() const
        { return (*m_name); }

        // attributes
		std::string *m_name;
    };

    class AstStringLiteralExpr : public AstExpression
    {
    public:
		AstStringLiteralExpr(std::string *value)
            : m_name(value)
        { assert(value != NULL); }
		virtual ~AstStringLiteralExpr() {}

        virtual void Accept(AstVisitor& visitor);

        const std::string& GetName() const
        { return (*m_name); }
	
        // attributes
		std::string *m_name;
    };

    class AstNumberLiteralExpr : public AstExpression
    {
    public:
        AstNumberLiteralExpr(std::string *value)
            : m_value(value)
        { assert(value != NULL); }
        virtual ~AstNumberLiteralExpr() {}

        virtual void Accept(AstVisitor& visitor);

        const std::string& GetName() const
        { return (*m_value); }

        // attriubutes
        std::string *m_value;
    };

    class AstCharacterConstant : public AstExpression
    {
    public:
        AstCharacterConstant(std::string *value)
            : m_value(value)
        { assert(value != NULL); }
        virtual ~AstCharacterConstant() {}

        virtual void Accept(AstVisitor& visitor);

        const std::string& GetName() const
        { return (*m_value); }

        // attriubutes
        std::string *m_value;
    };

    class AstBlockStatement : public AstStatement
    {
    public:
        AstBlockStatement(AstStatementList *slist)
            : m_statements(slist)
        {}
        virtual ~AstBlockStatement() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstStatementList *m_statements;
    };

    class AstStatementList : public AstNode
    {
    public:
        AstStatementList(AstStatement *statement)
            : m_statement(statement)
            , m_next(NULL)
        {}
        AstStatementList(AstStatement *statement, AstStatementList *next)
            : m_statement(statement)
            , m_next(next)
        {}
        virtual ~AstStatementList() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstStatement *m_statement;
        AstStatementList *m_next;
    };

    class AstDoWhileStatement : public AstStatement
    {
    public:
        AstDoWhileStatement(AstBlockStatement *statement,
            AstConditionExpression *condition)
            : m_statement(statement)
            , m_condition(condition)
        {}
        virtual ~AstDoWhileStatement() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstBlockStatement *m_statement;
        AstConditionExpression *m_condition;
    };

    class AstWhileStatement : public AstStatement
    {
    public:
        AstWhileStatement(AstConditionExpression *condition,
            AstBlockStatement *statement)
            : m_condition(condition)
            , m_statement(statement)
        {}
        virtual ~AstWhileStatement() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstConditionExpression *m_condition;
        AstBlockStatement *m_statement;
    };

    class AstForStatement : public AstStatement
    {
    public:
        AstForStatement(AstAssignmentList *initializer, 
            AstConditionExpression *condition,
            AstAssignmentList *expression,
            AstBlockStatement *statement)
            : m_initializer(initializer)
            , m_condition(condition)
            , m_expression(expression)
            , m_statement(statement)
        {}
        virtual ~AstForStatement() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstAssignmentList *m_initializer;
        AstConditionExpression *m_condition;
        AstAssignmentList *m_expression;
        AstBlockStatement *m_statement;
    };

    class AstAssignmentList : public AstStatement
    {
    public:
        AstAssignmentList(AstAssignmentExpr *assignment)
            : m_assignment(assignment)
            , m_next(NULL)
        {}
        AstAssignmentList(AstAssignmentExpr *assignment, AstAssignmentList *next)
            : m_assignment(assignment)
            , m_next(next)
        {}
        virtual ~AstAssignmentList() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstAssignmentExpr *m_assignment;
        AstAssignmentList *m_next;
    };

    class AstAssignmentExpr : public AstExpression
    {
    public:
        AstAssignmentExpr(AstVariableExpr *variable, AstExpression *expression)
            : m_variable(variable)
            , m_expression(expression)
        {}
        virtual ~AstAssignmentExpr() {};

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstVariableExpr *m_variable;
        AstExpression *m_expression;
    };

    class AstVariableExpr : public AstExpression
    {
    public:
        AstVariableExpr(AstIdentifierExpr *id, AstArrayExpr *arr)
            : m_identifier(id)
            , m_array(arr)
        {}
        virtual ~AstVariableExpr() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstIdentifierExpr *m_identifier;
        AstArrayExpr *m_array;
    };

    class AstArrayExpr : public AstExpression
    {
    public:
        AstArrayExpr(AstExpression *expr, AstArrayExpr *next)
            : m_expression(expr)
            , m_nextArray(next)
        {}
        virtual ~AstArrayExpr() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstExpression *m_expression;
        AstArrayExpr *m_nextArray;
    };

    class AstReadStatement : public AstStatement
    {
    public:
        AstReadStatement(AstReadArgumentList *argumentList)
            : m_argumentList(argumentList)
        {}
        virtual ~AstReadStatement() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstReadArgumentList *m_argumentList;
    };

    class AstReadArgumentList : public AstExpression
    {
    public:
        AstReadArgumentList(AstVariableExpr *id)
            : m_id(id)
            , m_next(NULL)
        {}
        AstReadArgumentList(AstVariableExpr *id, AstReadArgumentList *next)
            : m_id(id)
            , m_next(next)
        {}
        virtual ~AstReadArgumentList() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstVariableExpr *m_id;
        AstReadArgumentList *m_next;
    };

    class AstConditionExpression : public AstExpression
    {
    public:
        AstConditionExpression(AstExpression *expr1, AstExpression *expr2, Token::Kind operation)
            : m_expr1(expr1)
            , m_expr2(expr2)
            , m_operation(operation)
        {}
        virtual ~AstConditionExpression() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstExpression *m_expr1;
        AstExpression *m_expr2;
        Token::Kind m_operation;
    };

    class AstWriteStatement : public AstStatement
    {
    public:
        AstWriteStatement(AstWriteArgumentList *argumentList)
            : m_arguments(argumentList)
        {}
		virtual ~AstWriteStatement() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstWriteArgumentList *m_arguments;
    };

    class AstWriteArgumentList : public AstExpression
    {
    public:
        AstWriteArgumentList(AstExpression *expression)
            : m_expression(expression)
            , m_next(NULL)
        {}
        AstWriteArgumentList(AstExpression *expression, AstWriteArgumentList *next)
            : m_expression(expression)
            , m_next(next)
        {}
        virtual ~AstWriteArgumentList() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstExpression *m_expression;
        AstWriteArgumentList *m_next;
    };

    class AstTerm : public AstExpression
    {
	public:
        AstTerm(AstExpression *expr)
            : m_expression(expr)
        {}
        virtual ~AstTerm() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstExpression *m_expression;
    };

    class AstMultExpr : public AstExpression
    {
    public:
        AstMultExpr(AstUnaryExpr *unaryExpr, AstTerm *term)
            : m_unaryExpr(unaryExpr)
            , m_term(term)
        {}
        virtual ~AstMultExpr() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstUnaryExpr *m_unaryExpr;
        AstTerm *m_term;
    };

    class AstDivExpr : public AstExpression
    {
    public:
        AstDivExpr(AstUnaryExpr *unaryExpr, AstTerm *term)
            : m_unaryExpr(unaryExpr)
            , m_term(term)
        {}
        virtual ~AstDivExpr() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstUnaryExpr *m_unaryExpr;
        AstTerm *m_term;
    };

    class AstSubExpr : public AstExpression
    {
    public:
        AstSubExpr(AstExpression *expr, AstTerm *term)
            : m_expression(expr)
            , m_term(term)
        {}
        virtual ~AstSubExpr() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstExpression *m_expression;
        AstTerm *m_term;
    };

    class AstAddExpr : public AstExpression
    {
    public:
        AstAddExpr(AstExpression *expr, AstTerm *term)
            : m_expression(expr)
            , m_term(term)
        {}
        virtual ~AstAddExpr() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstExpression *m_expression;
        AstTerm *m_term;
    };

    class AstFactorExpr : public AstExpression
    {
    public:
        AstFactorExpr(AstExpression *expr)
            : m_expression(expr)
        {}
        virtual ~AstFactorExpr() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstExpression *m_expression; // may be: variable, Number constant, Expression
    };

    class AstUnaryExpr : public AstExpression
    {
    public:
        AstUnaryExpr(Token::Kind unaryOperator, AstFactorExpr *factor)
            : m_unaryOperator(unaryOperator)
            , m_factor(factor)
        {}

        AstUnaryExpr(AstFactorExpr *factor)
            : m_unaryOperator(Token::Token_Add) // it's value must be > 0
            , m_factor(factor)
        {}

        virtual ~AstUnaryExpr() {}

        virtual void Accept(AstVisitor& visitor);
        
        // attributes
        Token::Kind m_unaryOperator;
        AstFactorExpr *m_factor;
    };

    class AstIfStatement : public AstStatement
    {
    public:
        AstIfStatement(AstConditionExpression *condition,
            AstBlockStatement *thenBlock,
            AstBlockStatement *elseBlock)
            : m_condition(condition)
            , m_thenBlock(thenBlock)
            , m_elseBlock(elseBlock)
        {}
        virtual ~AstIfStatement() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstConditionExpression *m_condition;
        AstBlockStatement *m_thenBlock;
        AstBlockStatement *m_elseBlock;
    };

    class AstBreakStatement : public AstStatement
    {
    public:
        AstBreakStatement()
        {}
        virtual ~AstBreakStatement() {}

        virtual void Accept(AstVisitor& visitor);
    };

    class AstDeclaration : public AstStatement
    {
    public:
        AstDeclaration(Token::Kind type, AstVariableExpr *variable)
            : m_type(type)
            , m_variable(variable)
        {}
        virtual ~AstDeclaration() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        Token::Kind m_type;
        AstVariableExpr *m_variable;
    };

    class AstGlobalVarDeclarationList : public AstStatement
    {
    public:
        AstGlobalVarDeclarationList(AstDeclaration *declaration)
            : m_declaration(declaration)
            , m_next(NULL)
        {}

        AstGlobalVarDeclarationList(AstDeclaration *declaration, AstGlobalVarDeclarationList *next)
            : m_declaration(declaration)
            , m_next(next)
        {}
        virtual ~AstGlobalVarDeclarationList() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstDeclaration *m_declaration;
        AstGlobalVarDeclarationList *m_next;
    };

    class AstMain : public AstNode
    {
    public:
        AstMain(AstBlockStatement *block)
            : m_block(block)
        {}
        virtual ~AstMain() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstBlockStatement *m_block;
    };

    class AstProgram : public AstNode
    {
    public:
        AstProgram(AstGlobalVarDeclarationList *globalDecls, AstMain *mainFunction)
            : m_globalDecls(globalDecls)
            , m_mainFunction(mainFunction)
        {}
        virtual ~AstProgram() {}

        virtual void Accept(AstVisitor& visitor);

        // attributes
        AstGlobalVarDeclarationList *m_globalDecls;
        AstMain *m_mainFunction;
    };
}

#endif