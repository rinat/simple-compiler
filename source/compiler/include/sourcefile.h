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
#ifndef __Compiler_SourceFile_h__
#define __Compiler_SourceFile_h__

/*
    source file presentation
*/

#include "textbuffer.h"
#include "isourcefile.h"

namespace compiler
{
    class SourceFile
        : public ISourceFile
        , private utils::NonCopyable
    {
    public:
        SourceFile(const std::string &inPath, size_t inFirstLineIndex = 1);
        ~SourceFile();

        virtual const std::string& GetPath() const;
        virtual size_t GetLine(size_t offset) const;

        virtual ITextBuffer* GetBuffer();
        virtual ITextBuffer* GetBuffer(const std::string& inSource);

    private:
        TextBuffer* ReadFile(const std::string& filename);

    private:
        const std::string m_path;
        size_t m_firstLineIndex;
        std::auto_ptr<TextBuffer> m_textBuffer;
    };
}

#endif