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
#ifndef __Compiler_InputBuffer_h__
#define __Compiler_InputBuffer_h__

/*
    source file utility: lines map
*/

#include "itextbuffer.h"
#include "noncopyable.h"

namespace compiler
{
    class TextBuffer
        : public ITextBuffer
        , private utils::NonCopyable
    {
    public:
        TextBuffer();
        TextBuffer(const std::string& source);

        virtual char Next();
        virtual char Peek();

        virtual void Reset();
        virtual void Reset(const std::string& src);

        virtual size_t GetOffset() const;
        virtual size_t GetLineNumber(size_t offset) const;

        size_t GetLength() const;
        size_t GetLinesCount() const;
        size_t GetLineOffset(size_t lineNumber) const;
        size_t GetLineLength(size_t lineNumber) const;

        const std::string& GetSource() const;

    private:
        typedef std::vector<size_t> Lines;
        const Lines& GetLines() const;

        std::string m_source;
        size_t m_offset;
        mutable Lines m_lines;
    };
};

#endif

