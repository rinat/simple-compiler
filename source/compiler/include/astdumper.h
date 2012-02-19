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
#ifndef __Compiler_AstDumper_h__
#define __Compiler_AstDumper_h__

/*
    Abstract syntax tree structures dumper
*/

#include "astvisitor.h"

namespace compiler
{
    class AstDumper
        : public AstVisitor
        , private utils::NonCopyable
    {
    public:
        AstDumper(int indent, std::ostream& out)
            : m_indent(indent)
            , m_out(out)
            , m_indentLevel(0)
        {}
        ~AstDumper() {}

    public:
        virtual bool Visit(AstIdentifierExpr *);
        virtual bool Visit(AstStringLiteralExpr *);
        virtual bool Visit(AstBlockStatement *);
        virtual bool Visit(AstStatementList *);
        virtual bool Visit(AstDoWhileStatement *);
        virtual bool Visit(AstWhileStatement *);
        virtual bool Visit(AstForStatement *);
        virtual bool Visit(AstAssignmentList *);
        virtual bool Visit(AstAssignmentExpr *);
        virtual bool Visit(AstVariableExpr *);
        virtual bool Visit(AstArrayExpr *);
        virtual bool Visit(AstReadStatement *);
        virtual bool Visit(AstReadArgumentList *);
        virtual bool Visit(AstConditionExpression *);
        virtual bool Visit(AstCharacterConstant *);
        virtual bool Visit(AstWriteStatement *);
        virtual bool Visit(AstWriteArgumentList *);
        virtual bool Visit(AstTerm *);
        virtual bool Visit(AstMultExpr *);
        virtual bool Visit(AstDivExpr *);
        virtual bool Visit(AstSubExpr *);
        virtual bool Visit(AstAddExpr *);
        virtual bool Visit(AstFactorExpr *);
        virtual bool Visit(AstUnaryExpr *);
        virtual bool Visit(AstIfStatement *);
        virtual bool Visit(AstBreakStatement *);
        virtual bool Visit(AstDeclaration *);
        virtual bool Visit(AstGlobalVarDeclarationList *);
        virtual bool Visit(AstMain *);
        virtual bool Visit(AstProgram *);
        virtual bool Visit(AstNumberLiteralExpr *);

        // it's for catch invalid tree structure
        virtual bool Visit(AstNode *) { assert(false); return (false); }
        virtual bool Visit(AstExpression *) { assert(false); return (false); }
        virtual bool Visit(AstStatement *) { assert(false); return (false); }
        
    protected:
        void Accept(AstNode *node);

    private:
        int  IndentLevel(int level);
        void NewLineAndIndent();
        void IncLevel();
        void DecLevel();
        void PrintOffsets(AstNode *node);

    private:
        int m_indent;
        std::ostream& m_out;
        int m_indentLevel;
    };
}

#endif