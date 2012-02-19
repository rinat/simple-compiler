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
#ifndef __Compiler_Parser_h__
#define __Compiler_Parser_h__

/*
    language parser
*/

#include "grammar.h"
#include "scanner.h"
#include "cerrosobserver.h"
#include "stack.h"

namespace compiler
{
    class ISourceFile;
    class AstNodeBuilder;

    class Parser 
        : protected Grammar
        , public CErrorsContainer
        , private utils::NonCopyable
    {
    public:
        Parser();
        ~Parser();

        bool Parse(ISourceFile& sourceFile, AstNodeBuilder& nodeBuilder);

    private:
        void ReallocateStack();
        void ErrorReport(CError::ErrorType type, const std::string& errorMessage);
        size_t NextOffset();
        void UpdateNodePositions(AstNode *node, size_t startLine);
        void UpdateNodePositions(AstNode *node, AstNode *from);
        void UpdateNodePositions(AstNode *node, AstNode *fromStart, AstNode *fromEnd);

    private:
        ISourceFile *m_sourceFile;
        int tos;
        int stack_size;
        Stack<AstNode> m_nodes;
        std::vector<int> m_states;

        Scanner m_scanner;
        std::auto_ptr<CErrorsObserver> m_errosObserver;
    };
}

#endif