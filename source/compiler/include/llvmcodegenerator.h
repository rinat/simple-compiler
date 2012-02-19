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
#ifndef __Compiler_LLVMCodeGenerator_h__
#define __Compiler_LLVMCodeGenerator_h__

/*
    Abstract syntax tree structures generator to LLVM representation
*/

#include "cerrorscontainer.h"
#include "cerrosobserver.h"

#include "identifier.h"
#include "programelement.h"
#include "counters.h"
#include "icodegenerator.h"

namespace compiler
{
    class LLVMCodeGenerator
        : public ICodegenerator
        , public AstVisitor
        , public CErrorsContainer
        , private utils::NonCopyable
    {
        typedef std::map<std::string, Identifier> IdentifiersTable;
        typedef std::stack<ProgramElement> ProgramStack;        

    public:
        LLVMCodeGenerator();
        bool Generate(std::ostream& outStream, AstNode& program);

    public:    
        virtual bool Visit(AstProgram *);
        virtual bool Visit(AstMain *);
        virtual bool Visit(AstGlobalVarDeclarationList *);
        virtual bool Visit(AstDeclaration *);
        virtual bool Visit(AstVariableExpr *);
        virtual bool Visit(AstIdentifierExpr *);
        virtual bool Visit(AstArrayExpr *);
        virtual bool Visit(AstAssignmentList *);
        virtual bool Visit(AstAssignmentExpr *);
        virtual bool Visit(AstTerm *);        
        virtual bool Visit(AstMultExpr *);
        virtual bool Visit(AstDivExpr *);
        virtual bool Visit(AstSubExpr *);
        virtual bool Visit(AstAddExpr *);
        virtual bool Visit(AstUnaryExpr *);
        virtual bool Visit(AstFactorExpr *);
        virtual bool Visit(AstNumberLiteralExpr *);
        virtual bool Visit(AstCharacterConstant *);
        virtual bool Visit(AstWriteStatement *);
        virtual bool Visit(AstWriteArgumentList *);
        virtual bool Visit(AstReadStatement *);
        virtual bool Visit(AstReadArgumentList *);
        virtual bool Visit(AstIfStatement *);
        virtual bool Visit(AstConditionExpression *);
        virtual bool Visit(AstWhileStatement *);
        virtual bool Visit(AstBreakStatement *);
        virtual bool Visit(AstBlockStatement *);
        virtual bool Visit(AstStatementList *);

        // TODO: futures
        virtual bool Visit(AstStringLiteralExpr *);
        virtual bool Visit(AstDoWhileStatement *);
        virtual bool Visit(AstForStatement *);
        
        // it's for catch invalid tree structure;
        virtual bool Visit(AstNode *) { assert(false); return (false); }
        virtual bool Visit(AstExpression *) { assert(false); return (false); }
        virtual bool Visit(AstStatement *) { assert(false); return (false); }
 
    private:
        void ErrorReport(CError::ErrorType type,
            size_t beginOffset, const
            std::string& extendedInfo = std::string());
        std::string CreateTypeErrorMessage(Token::Kind left, Token::Kind right);
        bool CheckVariable(ProgramElement);

        void ReadAllVariables();
        void ReadVariable(const ProgramElement&);
        void WriteAllStackElements();
        void WriteProgramElement(const ProgramElement&);
        std::string LoadVariable(std::string);
        std::string LoadFromArray(std::string, std::string);
        std::vector<ProgramElement> LoadArrayToVariables(std::string arrayName);
        void StoreVariable(std::string, std::string);
        void StoreValueInArray(std::string, std::string, std::string);
        ProgramElement PopAndLoadProgramElement();
        ProgramElement ExecuteAssignmentOperation();
        ProgramElement ExecuteBinaryOperation(const ProgramElement&);       
        void Accept(AstNode *node);

    private:
        IdentifiersTable m_identifiersTable;
        ProgramStack m_programStack;
        Counters m_counters;
        std::string m_currentEndLabel;
        std::auto_ptr<CErrorsObserver> m_errosObserver;
        
        AstNode* m_rootProgramNode;
        std::ostream* m_outStream;

        bool m_isDeclaration;
    };
}

#endif
