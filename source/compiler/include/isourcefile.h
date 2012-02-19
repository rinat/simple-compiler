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
#ifndef __Compiler_ISourceFile_h__
#define __Compiler_ISourceFile_h__

#include "itextbuffer.h"

namespace compiler
{
    class ISourceFile
    {
    public:
        virtual const std::string& GetPath() const = 0;
        virtual size_t GetLine(size_t offset) const = 0;

        virtual ITextBuffer* GetBuffer() = 0;
        virtual ITextBuffer* GetBuffer(const std::string& inSource) = 0;

        virtual ~ISourceFile() {}
    };
}

#endif