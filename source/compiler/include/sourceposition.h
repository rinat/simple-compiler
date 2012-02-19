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
#ifndef __SourceRange_h__
#define __SourceRange_h__

/*
    source file position
*/

#include "isourcefile.h"
#include "isourceposition.h"
#include "astnode.h"

namespace compiler
{
    class ISourceFile;

    class SourcePosition : public ISourcePosition
    {
    public:
        SourcePosition();
        SourcePosition(const ISourceFile*, size_t offset);
        SourcePosition(AstNode *node, size_t offset);

        virtual size_t GetLine() const;
        virtual size_t GetOffset() const;
        virtual std::string GetSourcePath() const;

    private:
        const ISourceFile *m_sourceFile;
        size_t m_beginOffset;
        AstNode *m_astNode;
    };
}

#endif